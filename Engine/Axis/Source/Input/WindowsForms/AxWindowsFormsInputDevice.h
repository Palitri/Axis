#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxInputDevice.h"

#include "..\..\Tools\AxDictionary.h"

#include "..\..\Engine\Axis.h"

class AxWindowsFormsInputDevice :
	public AxInputDevice
{
private:
	AxProperty *keyLShift, *keyRShift, *keyLCtrl, *keyRCtrl, *keyLAlt, *keyRAlt, *keyStateNumLock, *keyStateCapsLock, *keyStateScrollLock;
	AxVector2 mousePos, lastMousePos;

public:
	Axis *context;
	AxDictionary<int, AxProperty*> keyProperties;

	AxProperty *mouseLeft, *mouseRight, *mouseMiddle, *mouseScroll, *mouseHScroll, *mouseX, *mouseY, *mouseXPos, *mouseYPos;

	AxWindowsFormsInputDevice(void);
	virtual ~AxWindowsFormsInputDevice(void);

	virtual void SetContext(Axis *context);
	virtual void Update();

	void SetSpecialKeyState(unsigned int key);
	void SetMouseCoords(int x, int y);
};

