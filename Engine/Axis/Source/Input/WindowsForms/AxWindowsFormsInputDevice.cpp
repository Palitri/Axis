//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWindowsFormsInputDevice.h"

#include <iostream>

#pragma comment(lib, "comctl32.lib")

#include <windows.h>
#include <windowsx.h>
#include <CommCtrl.h>



LRESULT CALLBACK SubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
{
	AxWindowsFormsInputDevice *device = (AxWindowsFormsInputDevice*)dwRefData;

	switch (uMsg)
	{
		case WM_KEYDOWN:						
		{
			int key = wParam;

			device->SetSpecialKeyState(key);

			AxProperty *keyProp;
			if (device->keyProperties.GetValue(key, keyProp))
				keyProp->SetBool(true);
				
			break;
		}

		case WM_KEYUP:								
		{
			int key = wParam;
				
			device->SetSpecialKeyState(key);

			AxProperty *keyProp;
			if (device->keyProperties.GetValue(key, keyProp))
				keyProp->SetBool(false);

			break;
		}

		case WM_LBUTTONDOWN:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);

			device->mouseLeft->SetBool(true);
			device->SetMouseCoords(x, y);
				
			break;
		}

		case WM_LBUTTONUP:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
				
			device->mouseLeft->SetBool(false);
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_RBUTTONDOWN:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
				
			device->mouseRight->SetBool(true);
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_RBUTTONUP:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
				
			device->mouseRight->SetBool(false);
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_MBUTTONDOWN:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
				
			device->mouseMiddle->SetBool(true);
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_MBUTTONUP:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
				
			device->mouseMiddle->SetBool(false);
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_MOUSEMOVE:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
				
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_MOUSEWHEEL:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
			int deltaFactor = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
				
			device->mouseScroll->SetFloat(deltaFactor);
			device->SetMouseCoords(x, y);

			break;
		}

		case WM_MOUSEHWHEEL:
		{
			int x = GET_X_LPARAM(lParam); 
			int y = GET_Y_LPARAM(lParam);
			int deltaFactor = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
				
			device->mouseHScroll->SetFloat(deltaFactor);
			device->SetMouseCoords(x, y);

			break;
		}
	}
			
	return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

AxWindowsFormsInputDevice::AxWindowsFormsInputDevice(void)
{
}


AxWindowsFormsInputDevice::~AxWindowsFormsInputDevice(void)
{
}

void AxWindowsFormsInputDevice::SetContext(Axis *context)
{
	this->context = context;

	UINT uIdSubclass = 0;
	DWORD_PTR dwRefData = DWORD_PTR(this);
	SetWindowSubclass(*((HWND*)this->context->GetRenderingWindow()), SubclassProc, uIdSubclass, dwRefData);

	this->keyProperties.Add(0x30			, this->context->input->properties.Add(new AxProperty(AxString("Key 0"), false)));
	this->keyProperties.Add(0x31			, this->context->input->properties.Add(new AxProperty(AxString("Key 1"), false)));
	this->keyProperties.Add(0x32			, this->context->input->properties.Add(new AxProperty(AxString("Key 2"), false)));
	this->keyProperties.Add(0x33			, this->context->input->properties.Add(new AxProperty(AxString("Key 3"), false)));
	this->keyProperties.Add(0x34			, this->context->input->properties.Add(new AxProperty(AxString("Key 4"), false)));
	this->keyProperties.Add(0x35			, this->context->input->properties.Add(new AxProperty(AxString("Key 5"), false)));
	this->keyProperties.Add(0x36			, this->context->input->properties.Add(new AxProperty(AxString("Key 6"), false)));
	this->keyProperties.Add(0x37			, this->context->input->properties.Add(new AxProperty(AxString("Key 7"), false)));
	this->keyProperties.Add(0x38			, this->context->input->properties.Add(new AxProperty(AxString("Key 8"), false)));
	this->keyProperties.Add(0x39			, this->context->input->properties.Add(new AxProperty(AxString("Key 9"), false)));
	this->keyProperties.Add(0x41			, this->context->input->properties.Add(new AxProperty(AxString("Key A"), false)));
	this->keyProperties.Add(0x42			, this->context->input->properties.Add(new AxProperty(AxString("Key B"), false)));
	this->keyProperties.Add(0x43			, this->context->input->properties.Add(new AxProperty(AxString("Key C"), false)));
	this->keyProperties.Add(0x44			, this->context->input->properties.Add(new AxProperty(AxString("Key D"), false)));
	this->keyProperties.Add(0x45			, this->context->input->properties.Add(new AxProperty(AxString("Key E"), false)));
	this->keyProperties.Add(0x46			, this->context->input->properties.Add(new AxProperty(AxString("Key F"), false)));
	this->keyProperties.Add(0x47			, this->context->input->properties.Add(new AxProperty(AxString("Key G"), false)));
	this->keyProperties.Add(0x48			, this->context->input->properties.Add(new AxProperty(AxString("Key H"), false)));
	this->keyProperties.Add(0x49			, this->context->input->properties.Add(new AxProperty(AxString("Key I"), false)));
	this->keyProperties.Add(0x4A			, this->context->input->properties.Add(new AxProperty(AxString("Key J"), false)));
	this->keyProperties.Add(0x4B			, this->context->input->properties.Add(new AxProperty(AxString("Key K"), false)));
	this->keyProperties.Add(0x4C			, this->context->input->properties.Add(new AxProperty(AxString("Key L"), false)));
	this->keyProperties.Add(0x4D			, this->context->input->properties.Add(new AxProperty(AxString("Key M"), false)));
	this->keyProperties.Add(0x4E			, this->context->input->properties.Add(new AxProperty(AxString("Key N"), false)));
	this->keyProperties.Add(0x4F			, this->context->input->properties.Add(new AxProperty(AxString("Key O"), false)));
	this->keyProperties.Add(0x50			, this->context->input->properties.Add(new AxProperty(AxString("Key P"), false)));
	this->keyProperties.Add(0x51			, this->context->input->properties.Add(new AxProperty(AxString("Key Q"), false)));
	this->keyProperties.Add(0x52			, this->context->input->properties.Add(new AxProperty(AxString("Key R"), false)));
	this->keyProperties.Add(0x53			, this->context->input->properties.Add(new AxProperty(AxString("Key S"), false)));
	this->keyProperties.Add(0x54			, this->context->input->properties.Add(new AxProperty(AxString("Key T"), false)));
	this->keyProperties.Add(0x55			, this->context->input->properties.Add(new AxProperty(AxString("Key U"), false)));
	this->keyProperties.Add(0x56			, this->context->input->properties.Add(new AxProperty(AxString("Key V"), false)));
	this->keyProperties.Add(0x57			, this->context->input->properties.Add(new AxProperty(AxString("Key W"), false)));
	this->keyProperties.Add(0x58			, this->context->input->properties.Add(new AxProperty(AxString("Key X"), false)));
	this->keyProperties.Add(0x59			, this->context->input->properties.Add(new AxProperty(AxString("Key Y"), false)));
	this->keyProperties.Add(0x5A			, this->context->input->properties.Add(new AxProperty(AxString("Key Z"), false)));
	this->keyProperties.Add(VK_F1			, this->context->input->properties.Add(new AxProperty(AxString("Key F1"), false)));
	this->keyProperties.Add(VK_F2			, this->context->input->properties.Add(new AxProperty(AxString("Key F2"), false)));
	this->keyProperties.Add(VK_F3			, this->context->input->properties.Add(new AxProperty(AxString("Key F3"), false)));
	this->keyProperties.Add(VK_F4			, this->context->input->properties.Add(new AxProperty(AxString("Key F4"), false)));
	this->keyProperties.Add(VK_F5			, this->context->input->properties.Add(new AxProperty(AxString("Key F5"), false)));
	this->keyProperties.Add(VK_F6			, this->context->input->properties.Add(new AxProperty(AxString("Key F6"), false)));
	this->keyProperties.Add(VK_F7			, this->context->input->properties.Add(new AxProperty(AxString("Key F7"), false)));
	this->keyProperties.Add(VK_F8			, this->context->input->properties.Add(new AxProperty(AxString("Key F8"), false)));
	this->keyProperties.Add(VK_F9			, this->context->input->properties.Add(new AxProperty(AxString("Key F9"), false)));
	this->keyProperties.Add(VK_F10			, this->context->input->properties.Add(new AxProperty(AxString("Key F10"), false)));
	this->keyProperties.Add(VK_F11			, this->context->input->properties.Add(new AxProperty(AxString("Key F11"), false)));
	this->keyProperties.Add(VK_F12			, this->context->input->properties.Add(new AxProperty(AxString("Key F12"), false)));
	this->keyProperties.Add(VK_OEM_3		, this->context->input->properties.Add(new AxProperty(AxString("Key Tilde"), false)));
	this->keyProperties.Add(VK_OEM_MINUS	, this->context->input->properties.Add(new AxProperty(AxString("Key Minus"), false)));
	this->keyProperties.Add(VK_OEM_PLUS		, this->context->input->properties.Add(new AxProperty(AxString("Key Equals"), false)));
	this->keyProperties.Add(VK_BACK			, this->context->input->properties.Add(new AxProperty(AxString("Key Backspace"), false)));
	this->keyProperties.Add(VK_OEM_4		, this->context->input->properties.Add(new AxProperty(AxString("Key Square bracket left"), false)));
	this->keyProperties.Add(VK_OEM_6		, this->context->input->properties.Add(new AxProperty(AxString("Key Square bracket right"), false)));
	this->keyProperties.Add(VK_OEM_5		, this->context->input->properties.Add(new AxProperty(AxString("Key Backslash"), false)));
	this->keyProperties.Add(VK_OEM_1		, this->context->input->properties.Add(new AxProperty(AxString("Key Semicolon"), false)));
	this->keyProperties.Add(VK_OEM_7		, this->context->input->properties.Add(new AxProperty(AxString("Key Quote"), false)));
	this->keyProperties.Add(VK_OEM_COMMA	, this->context->input->properties.Add(new AxProperty(AxString("Key Comma"), false)));
	this->keyProperties.Add(VK_OEM_PERIOD	, this->context->input->properties.Add(new AxProperty(AxString("Key Period"), false)));
	this->keyProperties.Add(VK_OEM_2		, this->context->input->properties.Add(new AxProperty(AxString("Key Slash"), false)));
	this->keyProperties.Add(VK_SPACE		, this->context->input->properties.Add(new AxProperty(AxString("Key Space"), false)));
	this->keyProperties.Add(VK_ESCAPE		, this->context->input->properties.Add(new AxProperty(AxString("Key Escape"), false)));
	this->keyProperties.Add(VK_RETURN		, this->context->input->properties.Add(new AxProperty(AxString("Key Enter"), false)));
	this->keyProperties.Add(VK_TAB			, this->context->input->properties.Add(new AxProperty(AxString("Key Tab"), false)));
	this->keyProperties.Add(VK_CAPITAL		, this->context->input->properties.Add(new AxProperty(AxString("Key Caps"), false)));
	this->keyProperties.Add(VK_SHIFT		, this->context->input->properties.Add(new AxProperty(AxString("Key Shift"), false)));
	this->keyProperties.Add(VK_CONTROL		, this->context->input->properties.Add(new AxProperty(AxString("Key Control"), false)));
	this->keyProperties.Add(VK_MENU			, this->context->input->properties.Add(new AxProperty(AxString("Key Alt"), false)));
	this->keyProperties.Add(VK_SNAPSHOT		, this->context->input->properties.Add(new AxProperty(AxString("Key Print screen"), false)));
	this->keyProperties.Add(VK_SCROLL		, this->context->input->properties.Add(new AxProperty(AxString("Key Scroll lock"), false)));
	this->keyProperties.Add(VK_PAUSE		, this->context->input->properties.Add(new AxProperty(AxString("Key Pause"), false)));
	this->keyProperties.Add(VK_INSERT		, this->context->input->properties.Add(new AxProperty(AxString("Key Insert"), false)));
	this->keyProperties.Add(VK_DELETE		, this->context->input->properties.Add(new AxProperty(AxString("Key Delete"), false)));
	this->keyProperties.Add(VK_HOME			, this->context->input->properties.Add(new AxProperty(AxString("Key Home"), false)));
	this->keyProperties.Add(VK_END			, this->context->input->properties.Add(new AxProperty(AxString("Key End"), false)));
	this->keyProperties.Add(VK_PRIOR		, this->context->input->properties.Add(new AxProperty(AxString("Key Page up"), false)));
	this->keyProperties.Add(VK_NEXT			, this->context->input->properties.Add(new AxProperty(AxString("Key Page down"), false)));
	this->keyProperties.Add(VK_LEFT			, this->context->input->properties.Add(new AxProperty(AxString("Key Arrow left"), false)));
	this->keyProperties.Add(VK_RIGHT		, this->context->input->properties.Add(new AxProperty(AxString("Key Arrow right"), false)));
	this->keyProperties.Add(VK_UP			, this->context->input->properties.Add(new AxProperty(AxString("Key Arrow up"), false)));
	this->keyProperties.Add(VK_DOWN			, this->context->input->properties.Add(new AxProperty(AxString("Key Arrow down"), false)));
	this->keyProperties.Add(VK_LWIN			, this->context->input->properties.Add(new AxProperty(AxString("Key Win left"), false)));
	this->keyProperties.Add(VK_RWIN			, this->context->input->properties.Add(new AxProperty(AxString("Key Win right"), false)));
	this->keyProperties.Add(VK_APPS			, this->context->input->properties.Add(new AxProperty(AxString("Key Context menu"), false)));
	this->keyProperties.Add(VK_NUMPAD0		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 0"), false)));
	this->keyProperties.Add(VK_NUMPAD1		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 1"), false)));
	this->keyProperties.Add(VK_NUMPAD2		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 2"), false)));
	this->keyProperties.Add(VK_NUMPAD3		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 3"), false)));
	this->keyProperties.Add(VK_NUMPAD4		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 4"), false)));
	this->keyProperties.Add(VK_NUMPAD5		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 5"), false)));
	this->keyProperties.Add(VK_NUMPAD6		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 6"), false)));
	this->keyProperties.Add(VK_NUMPAD7		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 7"), false)));
	this->keyProperties.Add(VK_NUMPAD8		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 8"), false)));
	this->keyProperties.Add(VK_NUMPAD9		, this->context->input->properties.Add(new AxProperty(AxString("Key Num 9"), false)));
	this->keyProperties.Add(VK_DIVIDE		, this->context->input->properties.Add(new AxProperty(AxString("Key Num divide"), false)));
	this->keyProperties.Add(VK_MULTIPLY		, this->context->input->properties.Add(new AxProperty(AxString("Key Num multiply"), false)));
	this->keyProperties.Add(VK_SUBTRACT		, this->context->input->properties.Add(new AxProperty(AxString("Key Num minus"), false)));
	this->keyProperties.Add(VK_ADD			, this->context->input->properties.Add(new AxProperty(AxString("Key Num plus"), false)));
	this->keyProperties.Add(VK_DECIMAL		, this->context->input->properties.Add(new AxProperty(AxString("Key Num decimal"), false)));
	this->keyProperties.Add(VK_NUMLOCK		, this->context->input->properties.Add(new AxProperty(AxString("Key Num lock"), false)));

	this->keyLShift				= this->context->input->properties.Add(new AxProperty(AxString("Key Shift left"), false));
	this->keyRShift				= this->context->input->properties.Add(new AxProperty(AxString("Key Shift right"), false));
	this->keyLCtrl				= this->context->input->properties.Add(new AxProperty(AxString("Key Control left"), false));
	this->keyRCtrl				= this->context->input->properties.Add(new AxProperty(AxString("Key Control right"), false));
	this->keyLAlt				= this->context->input->properties.Add(new AxProperty(AxString("Key Alt left"), false));
	this->keyRAlt				= this->context->input->properties.Add(new AxProperty(AxString("Key Alt right"), false));
	this->keyStateNumLock		= this->context->input->properties.Add(new AxProperty(AxString("Key state Num lock"), false));
	this->keyStateCapsLock		= this->context->input->properties.Add(new AxProperty(AxString("Key state Caps lock"), false));
	this->keyStateScrollLock	= this->context->input->properties.Add(new AxProperty(AxString("Key state Scroll lock"), false));

	this->SetSpecialKeyState(VK_NUMLOCK);
	this->SetSpecialKeyState(VK_CAPITAL);
	this->SetSpecialKeyState(VK_SCROLL);

	this->mouseLeft		= this->context->input->properties.Add(new AxProperty(AxString("Mouse Left"), false));
	this->mouseRight	= this->context->input->properties.Add(new AxProperty(AxString("Mouse Right"), false));
	this->mouseMiddle	= this->context->input->properties.Add(new AxProperty(AxString("Mouse Middle"), false));
	this->mouseScroll	= this->context->input->properties.Add(new AxProperty(AxString("Mouse Scroll"), 0));
	this->mouseHScroll	= this->context->input->properties.Add(new AxProperty(AxString("Mouse Scroll horizontal"), 0));
	this->mouseX		= this->context->input->properties.Add(new AxProperty(AxString("Mouse X"), 0.0f));
	this->mouseY		= this->context->input->properties.Add(new AxProperty(AxString("Mouse Y"), 0.0f));
	this->mouseXPos		= this->context->input->properties.Add(new AxProperty(AxString("Mouse X pos"), 0.0f));
	this->mouseYPos		= this->context->input->properties.Add(new AxProperty(AxString("Mouse Y pos"), 0.0f));
}

void AxWindowsFormsInputDevice::Update()
{
	this->mouseScroll->SetFloat(0.0f);
	this->mouseHScroll->SetFloat(0.0f);
	this->mouseX->SetFloat(this->mousePos.x - this->lastMousePos.x);
	this->mouseY->SetFloat(this->mousePos.y - this->lastMousePos.y);
	this->mouseXPos->SetFloat(this->mousePos.x);
	this->mouseYPos->SetFloat(this->mousePos.y);

	this->lastMousePos = this->mousePos;
}

void AxWindowsFormsInputDevice::SetSpecialKeyState(unsigned int key)
{
	switch (key)
	{
		case VK_SHIFT:
		{
			this->keyLShift->SetBool((GetKeyState(VK_LSHIFT) & 0x80000) != 0);
			this->keyRShift->SetBool((GetKeyState(VK_RSHIFT) & 0x80000) != 0);
			break;
		}

		case VK_CONTROL:
		{
			this->keyLCtrl->SetBool((GetKeyState(VK_LCONTROL) & 0x80000) != 0);
			this->keyRCtrl->SetBool((GetKeyState(VK_RCONTROL) & 0x80000) != 0);
			break;
		}

		case VK_MENU:
		{
			this->keyLAlt->SetBool((GetKeyState(VK_LMENU) & 0x80000) != 0);
			this->keyRAlt->SetBool((GetKeyState(VK_RMENU) & 0x80000) != 0);
			break;
		}

		case VK_CAPITAL:
		{
			this->keyStateCapsLock->SetBool((GetKeyState(VK_CAPITAL) & 0xffff) != 0);
			break;
		}

		case VK_SCROLL:
		{
			this->keyStateScrollLock->SetBool((GetKeyState(VK_SCROLL) & 0xffff) != 0);
			break;
		}

		case VK_NUMLOCK:
		{
			this->keyStateNumLock->SetBool((GetKeyState(VK_NUMLOCK) & 0xffff) != 0);
			break;
		}
	}
}

void AxWindowsFormsInputDevice::SetMouseCoords(int x, int y)
{
	this->mousePos.Set((float)x / (float)this->context->viewportWidth, -(float)y / (float)this->context->viewportHeight);
	//this->mousePos = this->context->ScreenCoordsPixelToUnit(AxVector2(x, y));
}
