//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirectInputController.h"


//-----------------------------------------------------------------------------
// Name: EnumObjectsCallback()
// Desc: Callback function for enumerating objects (axes, buttons, POVs) on a 
//       joystick. This function enables user interface elements for objects
//       that are found to exist, and scales axes min/max values.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumObjectsCallback(const DIDEVICEOBJECTINSTANCE* pdidoi, VOID* pContext)
{
    AxDirectInputController *controller = (AxDirectInputController*) pContext;

    // For axes that are returned, set the DIPROP_RANGE property for the
    // enumerated axis in order to scale min/max values.
    if (pdidoi->dwType & DIDFT_AXIS)
    {
        DIPROPRANGE diprg;
        diprg.diph.dwSize = sizeof(DIPROPRANGE);
        diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
        diprg.diph.dwHow = DIPH_BYID;
        diprg.diph.dwObj = pdidoi->dwType; // Specify the enumerated axis
        diprg.lMin = -1000;
        diprg.lMax = +1000;

        // Set the range for the axis
		if (FAILED(controller->device->SetProperty(DIPROP_RANGE, &diprg.diph)))
            return DIENUM_STOP;
    }


    // Set the UI to reflect what objects the joystick supports
    if( pdidoi->guidType == GUID_XAxis )
    {
		controller->analogAxesAvailable[AxDirectInputController::analogAxisLX] = true;
    }
    else if( pdidoi->guidType == GUID_YAxis )
    {
		controller->analogAxesAvailable[AxDirectInputController::analogAxisLY] = true;
    }
    else if( pdidoi->guidType == GUID_ZAxis )
    {
		controller->analogAxesAvailable[AxDirectInputController::analogAxisLZ] = true;
    }
    else if( pdidoi->guidType == GUID_RxAxis )
    {
		controller->analogAxesAvailable[AxDirectInputController::analogAxisRX] = true;
    }
    else if( pdidoi->guidType == GUID_RyAxis )
    {
		controller->analogAxesAvailable[AxDirectInputController::analogAxisRY] = true;
    }
    else if( pdidoi->guidType == GUID_RzAxis )
    {
		controller->analogAxesAvailable[AxDirectInputController::analogAxisRZ] = true;
    }
    else if( pdidoi->guidType == GUID_Slider )
    {
		controller->slidersCount++;
    }
    else if( pdidoi->guidType == GUID_POV )
    {
		controller->povCount++;
    }

    return DIENUM_CONTINUE;
}



AxDirectInputController::AxDirectInputController(void)
{
	this->padIndex = 0;

	this->device = NULL;

	this->isXInput = false;

	this->povCount = 0;
	this->slidersCount = 0;

	for (int i = 0; i < AxDirectInputController::analogAxesCount; i++)
		this->analogAxesAvailable[i] = false;

	this->analogAxesAvailableCount = 0;
}


AxDirectInputController::~AxDirectInputController(void)
{
    if (this->device != NULL)
	{
        this->device->Unacquire();
		this->device->Release();
		this->device = NULL;
	}
}

void AxDirectInputController::BeginAnalogAxesAssignment()
{
	for (int i = 0; i < AxDirectInputController::analogAxesCount; i++)
		this->analogAxesAssigned[i] = false;

	this->analogAxesAvailableCount = 0;
	for (int i = 0; i < AxDirectInputController::analogAxesCount; i++)
		if (this->analogAxesAvailable[i])
			this->analogAxesAvailableCount++;
}

int AxDirectInputController::AssignAnalogAxisIndex(int preferredIndex)
{
	for (int i = preferredIndex; i < AxDirectInputController::analogAxesCount; i++)
	{
		if (this->analogAxesAvailable[i] && !this->analogAxesAssigned[i])
		{
			this->analogAxesAssigned[i] = true;
			return i;
		}
	}

	return -1;
}

float AxDirectInputController::GetAnalogAxisValue(int rawValue)
{
	const int threshold = 200;
	const int range = 1000;
	const float usedRange = range - threshold;

	return rawValue > threshold ? (float)(rawValue - threshold) / usedRange : rawValue < -threshold ? (float)(rawValue + threshold) / usedRange : 0.0f;
}

// Example input XBox360 Controller		: b0-b3:ABXY, 4:TriggerHighLeft, 5:TriggerHighRight, 6-Select, 7-Start, 8-ThumbLeft, 9-ThumbRight, lX-lY:ThumbLeft, rX-rY:ThumbRight, lZ(-/+)-Trigger(Left/Right)
// Example input Generic USB Joystick	: b0-b3:ABXY, 4:TriggerHighLeft, 5:TriggerHighRight, 6-TriggerLowLeft, 7-TriggerLowRight, 8-Select, 9-Start, 10-ThumbLeft, 11-ThumbRight, lX-lY:ThumbLeft, lZ-rZ:ThumbRight
bool AxDirectInputController::UpdateInput()
{
    HRESULT hr;

    if (this->device == NULL)
		return false;

    // Poll the device to read the current state
    hr = this->device->Poll();
    if (FAILED(hr))
    {
        // DInput is telling us that the input stream has been
        // interrupted. We aren't tracking any state between polls, so
        // we don't have any special reset that needs to be done. We
        // just re-acquire and try again.
        do
		{
			hr = this->device->Acquire();
		}
        while (hr == DIERR_INPUTLOST);

        // hr may be DIERR_OTHERAPPHASPRIO or other errors.  This
        // may occur when the app is minimized or in the process of 
        // switching, so just try again later 
        return true;
    }

    // Get the input's device state
    DIJOYSTATE2 deviceState; 
	hr = this->device->GetDeviceState(sizeof(DIJOYSTATE2), &deviceState);
    if (FAILED(hr))
        return false; // The device should have been acquired during the Poll()

	this->analogAxesValues[0] = deviceState.lX;
	this->analogAxesValues[1] = deviceState.lY;
	this->analogAxesValues[2] = deviceState.lZ;
	this->analogAxesValues[3] = deviceState.lRx;
	this->analogAxesValues[4] = deviceState.lRy;
	this->analogAxesValues[5] = deviceState.lRz;

	if (this->lXIndex != -1)
		this->controlProperties->padAnalogXLeft->SetFloat(AxDirectInputController::GetAnalogAxisValue(this->analogAxesValues[this->lXIndex]));
	if (this->lYIndex != -1)
		this->controlProperties->padAnalogYLeft->SetFloat(AxDirectInputController::GetAnalogAxisValue(-this->analogAxesValues[this->lYIndex]));
	if (this->rXIndex != -1)
		this->controlProperties->padAnalogXRight->SetFloat(AxDirectInputController::GetAnalogAxisValue(this->analogAxesValues[this->rXIndex]));
	if (this->rYIndex != -1)
		this->controlProperties->padAnalogYRight->SetFloat(AxDirectInputController::GetAnalogAxisValue(-this->analogAxesValues[this->rYIndex]));
	if (this->triggerIndex != -1)
	{
		float triggersValue = this->analogAxesValues[this->triggerIndex];
		if (triggersValue > 0)
		{
			this->controlProperties->padTriggerAnalogLeft->SetFloat(triggersValue / 1000.0f);
			this->controlProperties->padTriggerAnalogRight->SetFloat(0.0f);
		}
		else
		{
			this->controlProperties->padTriggerAnalogLeft->SetFloat(0.0f);
			this->controlProperties->padTriggerAnalogRight->SetFloat(-triggersValue / 1000.0f);
		}
	}

	if (this->povCount > 0)
	{
		int pov = deviceState.rgdwPOV[0];
		this->controlProperties->padUp->SetBool((pov != -1) && ((pov >= 31500) || (pov <= 4500)));
		this->controlProperties->padRight->SetBool((pov != -1) && (pov >= 4500) && (pov <= 13500));
		this->controlProperties->padDown->SetBool((pov != -1) && (pov >= 13500) && (pov <= 22500));
		this->controlProperties->padLeft->SetBool((pov != -1) && (pov >= 22500) && (pov <= 31500));
	}

    for( int i = 0; i < 128; i++ )
    {
		AxProperty* keyProp;
		if (this->controlProperties->keyProperties.GetValue(i, keyProp))
			keyProp->SetBool((deviceState.rgbButtons[i] & 0x80) != 0);

		//if (keyProp->GetBool())
		//	std::cout << keyProp->name.GetCharContents() << " (" << i << ")" << "\r\n";
    }

    return true;
}

bool AxDirectInputController::Init(AxDirectInputDevice *context, LPDIRECTINPUTDEVICE8 device)
{
	this->context = context;
	this->device = device;

    HRESULT hr;

	// Set the data format to "simple joystick" - a predefined data format 
    //
    // A data format specifies which controls on a device we are interested in,
    // and how they should be reported. This tells DInput that we will be
    // passing a DIJOYSTATE2 structure to IDirectInputDevice::GetDeviceState().
	hr = this->device->SetDataFormat(&c_dfDIJoystick2);
	if (FAILED(hr))
        return false;

	HWND deviceWindow = *(HWND*)this->context->context->GetRenderingWindow();
	// Set the cooperative level to let DInput know how this device should
    // interact with the system and with other DInput applications.
	hr = this->device->SetCooperativeLevel(deviceWindow, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr))
        return false;

	// Enumerate the joystick objects. The callback function enabled user
    // interface elements for objects that are found, and sets the min/max
    // values property for discovered axes.
	hr = this->device->EnumObjects(EnumObjectsCallback, (VOID*)this, DIDFT_ALL);
	if (FAILED(hr))
        return false;


	this->BeginAnalogAxesAssignment();

	this->lXIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisLX);
	this->lYIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisLY);

	if (this->analogAxesAvailableCount < 5)
	{
		this->rXIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisLZ);
		this->rYIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisRX);

		this->triggerIndex = -1;
	}
	else
	{
		this->rXIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisRX);
		this->rYIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisRY);

		this->triggerIndex = this->AssignAnalogAxisIndex(AxDirectInputController::analogAxisLZ);
	}

	return true;
}
