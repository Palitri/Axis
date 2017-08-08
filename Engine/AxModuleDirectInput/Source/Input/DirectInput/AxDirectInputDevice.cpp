//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDirectInputDevice.h"

#include "AxDirectInputController.h"

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>


#include <iostream>

// Stuff to filter out XInput devices
#include <wbemidl.h>
HRESULT CreateXInputDevicesList();
bool IsXInputDevice( const GUID* pGuidProductFromDirectInput );
void CleanupXInputDevicesList();

struct XINPUT_DEVICE_NODE
{
    DWORD dwVidPid;
    XINPUT_DEVICE_NODE* pNext;
};

bool                    g_bFilterOutXinputDevices = false;
XINPUT_DEVICE_NODE*     g_pXInputDeviceList = NULL;


//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

//-----------------------------------------------------------------------------
// Enum each PNP device using WMI and check each device ID to see if it contains 
// "IG_" (ex. "VID_045E&PID_028E&IG_00").  If it does, then it’s an XInput device
// Unfortunately this information can not be found by just using DirectInput.
// Checking against a VID/PID of 0x028E/0x045E won't find 3rd party or future 
// XInput devices.
//
// This function stores the list of xinput devices in a linked list 
// at g_pXInputDeviceList, and IsXInputDevice() searchs that linked list
//-----------------------------------------------------------------------------
HRESULT LoadXInputDevicesList()
{
    IWbemServices* pIWbemServices = NULL;
    IEnumWbemClassObject* pEnumDevices = NULL;
    IWbemLocator* pIWbemLocator = NULL;
    IWbemClassObject* pDevices[20] = {0};
    BSTR bstrDeviceID = NULL;
    BSTR bstrClassName = NULL;
    BSTR bstrNamespace = NULL;
    DWORD uReturned = 0;
    bool bCleanupCOM = false;
    UINT iDevice = 0;
    VARIANT var;
    HRESULT hr;

    // CoInit if needed
    hr = CoInitialize( NULL );
    bCleanupCOM = SUCCEEDED( hr );

    // Create WMI
    hr = CoCreateInstance( __uuidof( WbemLocator ),
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           __uuidof( IWbemLocator ),
                           ( LPVOID* )&pIWbemLocator );
    if( FAILED( hr ) || pIWbemLocator == NULL )
        goto LCleanup;

    // Create BSTRs for WMI
    bstrNamespace = SysAllocString( L"\\\\.\\root\\cimv2" ); if( bstrNamespace == NULL ) goto LCleanup;
    bstrDeviceID = SysAllocString( L"DeviceID" );           if( bstrDeviceID == NULL )  goto LCleanup;
    bstrClassName = SysAllocString( L"Win32_PNPEntity" );    if( bstrClassName == NULL ) goto LCleanup;

    // Connect to WMI 
    hr = pIWbemLocator->ConnectServer( bstrNamespace, NULL, NULL, 0L,
                                       0L, NULL, NULL, &pIWbemServices );
    if( FAILED( hr ) || pIWbemServices == NULL )
        goto LCleanup;

    // Switch security level to IMPERSONATE
    CoSetProxyBlanket( pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
                       RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0 );

    // Get list of Win32_PNPEntity devices
    hr = pIWbemServices->CreateInstanceEnum( bstrClassName, 0, NULL, &pEnumDevices );
    if( FAILED( hr ) || pEnumDevices == NULL )
        goto LCleanup;

    // Loop over all devices
    for(; ; )
    {
        // Get 20 at a time
        hr = pEnumDevices->Next( 10000, 20, pDevices, &uReturned );
        if( FAILED( hr ) )
            goto LCleanup;
        if( uReturned == 0 )
            break;

        for( iDevice = 0; iDevice < uReturned; iDevice++ )
        {
            // For each device, get its device ID
            hr = pDevices[iDevice]->Get( bstrDeviceID, 0L, &var, NULL, NULL );
            if( SUCCEEDED( hr ) && var.vt == VT_BSTR && var.bstrVal != NULL )
            {
                // Check if the device ID contains "IG_".  If it does, then it’s an XInput device
                // Unfortunately this information can not be found by just using DirectInput 
                if( wcsstr( var.bstrVal, L"IG_" ) )
                {
                    // If it does, then get the VID/PID from var.bstrVal
                    DWORD dwPid = 0, dwVid = 0;
                    WCHAR* strVid = wcsstr( var.bstrVal, L"VID_" );
                    if( strVid && swscanf( strVid, L"VID_%4X", &dwVid ) != 1 )
                        dwVid = 0;
                    WCHAR* strPid = wcsstr( var.bstrVal, L"PID_" );
                    if( strPid && swscanf( strPid, L"PID_%4X", &dwPid ) != 1 )
                        dwPid = 0;

                    DWORD dwVidPid = MAKELONG( dwVid, dwPid );

                    // Add the VID/PID to a linked list
                    XINPUT_DEVICE_NODE* pNewNode = new XINPUT_DEVICE_NODE;
                    if( pNewNode )
                    {
                        pNewNode->dwVidPid = dwVidPid;
                        pNewNode->pNext = g_pXInputDeviceList;
                        g_pXInputDeviceList = pNewNode;
                    }
                }
            }
            SAFE_RELEASE( pDevices[iDevice] );
        }
    }

LCleanup:
    if( bstrNamespace )
        SysFreeString( bstrNamespace );
    if( bstrDeviceID )
        SysFreeString( bstrDeviceID );
    if( bstrClassName )
        SysFreeString( bstrClassName );
    for( iDevice = 0; iDevice < 20; iDevice++ )
    SAFE_RELEASE( pDevices[iDevice] );
    SAFE_RELEASE( pEnumDevices );
    SAFE_RELEASE( pIWbemLocator );
    SAFE_RELEASE( pIWbemServices );

    return hr;
}


							   
//-----------------------------------------------------------------------------
// Returns true if the DirectInput device is also an XInput device.
// Checks whether the GUID is contained in the linked list witht he XInput devices
// Call SetupForIsXInputDevice() before, and CleanupForIsXInputDevice() after
//-----------------------------------------------------------------------------
bool IsXInputDevice(const GUID* pGuidProductFromDirectInput)
{
    // Check each xinput device to see if this device's vid/pid matches
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while (pNode)
    {
        if (pNode->dwVidPid == pGuidProductFromDirectInput->Data1)
            return true;
        pNode = pNode->pNext;
    }

    return false;
}


//-----------------------------------------------------------------------------
// Cleanup needed for IsXInputDevice()
// Frees the linked list with the XInput device GUIDS
//-----------------------------------------------------------------------------
void FreeXInputDevicesList()
{
    // Cleanup linked list
    XINPUT_DEVICE_NODE* pNode = g_pXInputDeviceList;
    while( pNode )
    {
        XINPUT_DEVICE_NODE* pDelete = pNode;
        pNode = pNode->pNext;
        SAFE_DELETE( pDelete );
    }
}





// Context, passed to the Callback
struct DIEnumDevicesCallbackContext
{
    AxDirectInputDevice *axDeviceContext;
	DIJOYCONFIG* preferredJoystickConfig;
    bool hasPreferredJoystick;
};


//-----------------------------------------------------------------------------
// Name: EnumControllersCallback()
// Desc: Called once for each enumerated joystick. If we find one, create a
//       device interface on it so we can play with it.
//-----------------------------------------------------------------------------
BOOL CALLBACK EnumControllersCallback(const DIDEVICEINSTANCE* pdidInstance, VOID* pContext)
{
    DIEnumDevicesCallbackContext* callbackContext = (DIEnumDevicesCallbackContext*)pContext;
    HRESULT hr;

    bool isXInputDevice = IsXInputDevice(&pdidInstance->guidProduct);
	if (g_bFilterOutXinputDevices && isXInputDevice)
        return DIENUM_CONTINUE;

	bool isPreferredDevice = callbackContext->hasPreferredJoystick && !IsEqualGUID(pdidInstance->guidInstance, callbackContext->preferredJoystickConfig->guidInstance);

	// Obtain an interface to the enumerated joystick.
	LPDIRECTINPUTDEVICE8 controllerDevice;
	hr = callbackContext->axDeviceContext->dInput->CreateDevice(pdidInstance->guidInstance, &controllerDevice, NULL);

    if (FAILED(hr))
        return DIENUM_CONTINUE;

	AxDirectInputController *controller = callbackContext->axDeviceContext->controllers.Add(new AxDirectInputController());
	controller->isXInput = isXInputDevice;
	controller->controlProperties = callbackContext->axDeviceContext->AcquireControllerProperties(callbackContext->axDeviceContext->controllers.count - 1);
	controller->Init(callbackContext->axDeviceContext, controllerDevice);
	controller->controlProperties->padAvailable->SetBool(true);

    // Continue enumeration of any other devices
	return DIENUM_CONTINUE;
}




AxDirectInputDevice::AxDirectInputDevice(void)
{
	this->context = 0;

	this->dInput = NULL;
}


AxDirectInputDevice::~AxDirectInputDevice(void)
{
	this->FreeDirectInput();

	for (void *node = this->controlProperties.GetNodeNext(0); node != 0; node = this->controlProperties.GetNodeNext(node))
	{
		AxDirectInputControlProperties *controlProperties = this->controlProperties.GetNodeValue(node).value;
		delete controlProperties;
	}
}

void AxDirectInputDevice::SetContext(Axis *context)
{
	this->context = context;

	this->AcquireControllerProperties(0);
	this->AcquireControllerProperties(1);
	this->AcquireControllerProperties(2);
	this->AcquireControllerProperties(3);

	this->InitDirectInput();
}

void AxDirectInputDevice::Update()
{
	for (int i = 0; i < this->controllers.count; i++)
		this->controllers[i]->UpdateInput();
}

bool AxDirectInputDevice::InitDirectInput()
{
    HRESULT hr;

    // Register with the DirectInput subsystem and get a pointer
    // to a IDirectInput interface we can use.
    // Create a DInput object
    hr = DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&this->dInput, NULL);
	if (FAILED(hr))
        return false;


    if(g_bFilterOutXinputDevices)
        LoadXInputDevicesList();


    DIJOYCONFIG preferredJoystickConfig = {0};
    DIEnumDevicesCallbackContext enumContext;
	enumContext.preferredJoystickConfig = &preferredJoystickConfig;
	enumContext.hasPreferredJoystick = false;
	enumContext.axDeviceContext = this;

	// Get the system's preferred joystick information
	IDirectInputJoyConfig8* joystickConfiguration = NULL;
    hr = this->dInput->QueryInterface(IID_IDirectInputJoyConfig8, (void**)&joystickConfiguration);
	if (SUCCEEDED(hr))
	{
		preferredJoystickConfig.dwSize = sizeof(preferredJoystickConfig);
		if (SUCCEEDED(joystickConfiguration->GetConfig(0, &preferredJoystickConfig, DIJC_GUIDINSTANCE))) // This function is expected to fail if no joystick is attached
			enumContext.hasPreferredJoystick = true;
		SAFE_RELEASE(joystickConfiguration);
	}

    
	hr = this->dInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumControllersCallback, &enumContext, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr))
        return false;


    if (g_bFilterOutXinputDevices)
		FreeXInputDevicesList();

    return true;
}

void AxDirectInputDevice::FreeDirectInput()
{
	for (int i = 0; i < this->controllers.count; i++)
		delete this->controllers[i];
	this->controllers.Clear();

	if (this->dInput != NULL)
	{
		this->dInput->Release();
		this->dInput = NULL;
	}
}

AxDirectInputControlProperties* AxDirectInputDevice::AcquireControllerProperties(int controllerIndex)
{
	AxDirectInputControlProperties* controlProperties;
	if (this->controlProperties.GetValue(controllerIndex, controlProperties))
		return controlProperties;

	controlProperties = new AxDirectInputControlProperties();
	this->controlProperties.Add(controllerIndex, controlProperties);

	AxString propNamePrefix = "Pad ";
	if (controllerIndex != 0)
		propNamePrefix += AxString(controllerIndex + 1) + " ";

	controlProperties->keyProperties.Add(0, this->context->input->properties.Add(new AxProperty(propNamePrefix + "A", false)));
	controlProperties->keyProperties.Add(1, this->context->input->properties.Add(new AxProperty(propNamePrefix + "B", false)));
	controlProperties->keyProperties.Add(2, this->context->input->properties.Add(new AxProperty(propNamePrefix + "X", false)));
	controlProperties->keyProperties.Add(3, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Y", false)));
	controlProperties->keyProperties.Add(4, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Trigger button left", false)));
	controlProperties->keyProperties.Add(5, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Trigger button right", false)));
	controlProperties->keyProperties.Add(6, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Select", false)));
	controlProperties->keyProperties.Add(7, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Start", false)));
	// On a generic usb gamepad, thumbsticks may appear alternatively on buttons 10 and 11
	controlProperties->keyProperties.Add(8, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Thumb left", false)));
	controlProperties->keyProperties.Add(9, this->context->input->properties.Add(new AxProperty(propNamePrefix + "Thumb right", false)));

	controlProperties->padUp					= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Up", false));
	controlProperties->padDown					= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Down", false));
	controlProperties->padLeft					= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Left", false));
	controlProperties->padRight					= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Right", false));
	controlProperties->padAnalogXLeft			= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Analog X left", 0.0f));
	controlProperties->padAnalogYLeft			= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Analog Y left", 0.0f));
	controlProperties->padAnalogXRight			= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Analog X right", 0.0f));
	controlProperties->padAnalogYRight			= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Analog Y right", 0.0f));
	controlProperties->padTriggerAnalogLeft		= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Trigger analog left", false));
	controlProperties->padTriggerAnalogRight	= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Trigger analog right", false));
	controlProperties->padAvailable				= this->context->input->properties.Add(new AxProperty(propNamePrefix + "Available", false));

	return controlProperties;
}
