#pragma once

#include "..\..\AxisEngine.h"

#include <dinput.h>
#include <dinputd.h>

class AxDirectInputControlProperties
{
public:
	AxDictionary<int, AxProperty*> keyProperties;
	AxProperty *padUp, *padDown, *padLeft, *padRight, *padAnalogXLeft, *padAnalogYLeft, *padAnalogXRight, *padAnalogYRight, *padTriggerAnalogLeft, *padTriggerAnalogRight, *padAvailable;
};

class AxDirectInputController;

class AxDirectInputDevice :
	public AxInputDevice
{
private:
	bool InitDirectInput();
	void FreeDirectInput();

public:
	LPDIRECTINPUT8 dInput;

	Axis *context;
	AxList<AxDirectInputController*> controllers;

	AxDictionary<int, AxDirectInputControlProperties*> controlProperties;

	AxDirectInputDevice(void);
	virtual ~AxDirectInputDevice(void);

	virtual void SetContext(Axis *context);
	virtual void Update();

	AxDirectInputControlProperties* AcquireControllerProperties(int controllerIndex);
};

