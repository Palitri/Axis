#pragma once

#include "..\..\AxisEngine.h"

#include "AxDirectInputDevice.h"

class AxDirectInputController
{
private:
	static const int analogAxesCount = 6;

	static float GetAnalogAxisValue(int rawValue);

	float analogAxesValues[AxDirectInputController::analogAxesCount];
	bool analogAxesAssigned[AxDirectInputController::analogAxesCount];

	void BeginAnalogAxesAssignment();
	int AssignAnalogAxisIndex(int preferredIndex);

public:
	static const int analogAxisLX = 0;
	static const int analogAxisLY = 1;
	static const int analogAxisLZ = 2;
	static const int analogAxisRX = 3;
	static const int analogAxisRY = 4;
	static const int analogAxisRZ = 5;


	int padIndex;
	int povCount, slidersCount;
	bool analogAxesAvailable[AxDirectInputController::analogAxesCount];
	int analogAxesAvailableCount;

	int lXIndex, lYIndex, rXIndex, rYIndex, triggerIndex;

	AxDirectInputDevice *context;

	LPDIRECTINPUTDEVICE8 device;
	bool isXInput;

	AxDirectInputControlProperties *controlProperties;

	AxDirectInputController(void);
	virtual ~AxDirectInputController(void);

	bool UpdateInput();

	bool Init(AxDirectInputDevice *context, LPDIRECTINPUTDEVICE8 device);
};

