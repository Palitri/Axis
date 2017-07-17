//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWindowsForm.h"

#ifndef UNICODE
#	define UNICODE
#endif

#include <windowsx.h>

const wchar_t* AxWindowsForm::FormClassName = L"MyWindowsForm";

AxList<AxWindowsForm*> AxWindowsForm::instances;

LRESULT CALLBACK AxWindowsForm::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//AxWindowsForm* instance = AxWindowsForm::GetInstance(hWnd);
	AxWindowsForm* instance = (AxWindowsForm*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	if (instance != NULL)
	{
		switch (uMsg)									
		{
			case WM_SYSCOMMAND:							
			{
				switch (wParam)							
				{
					case SC_SCREENSAVE:
						{
							if (instance->suppressScreenSaver)
								return 0;
						}
					case SC_MONITORPOWER:
						{
							if (instance->suppressMonitorPowerSave)
								return 0;
						}
				}
				break;									
			}

			case WM_ACTIVATE:							
			{
				// Minimization state
				if (!HIWORD(wParam))					
				{
					instance->active=TRUE;				
				}
				else
				{
					instance->active=FALSE;				
				}

				return 0;								
			}

			case WM_CLOSE:							
			{
				if (instance->Close())
				{
					PostQuitMessage(0);					
				}
				return 0;							
			}

			case WM_SIZE:								
			{
				int width = LOWORD(lParam);
				int height = HIWORD(lParam);

				int windowState = instance->windowState;
				if (wParam == SIZE_MAXIMIZED)
					windowState = AxWindowsForm::WindowState_Maximized;
				if (wParam == SIZE_MINIMIZED)
					windowState = AxWindowsForm::WindowState_Minimized;
				if (wParam == SIZE_RESTORED)
					windowState = AxWindowsForm::WindowState_Normal;

				bool fakeResize = (windowState == AxWindowsForm::WindowState_Minimized) || (instance->windowState == AxWindowsForm::WindowState_Minimized);
				bool stateChange = (windowState != instance->windowState);
				if (!fakeResize)
					instance->Resized(width, height, stateChange);

				instance->MainLoop();

				instance->width = width;
				instance->height = height;

				if (stateChange)
				{
					instance->StateChanged(instance->windowState, windowState);
					instance->windowState = windowState;
				}

				instance->isSizing = true;

				return 0;								
			}

			case WM_ENTERSIZEMOVE:
			{
				instance->isSizeOrMove = true;
				instance->isSizing = false;

				return 0;
			}

			case WM_EXITSIZEMOVE:
			{
				if (instance->isSizeOrMove && instance->isSizing)
					instance->Resized(instance->width, instance->height, true);

				instance->isSizing = false;
				instance->isSizeOrMove = false;

				return 0;								
			}
			
			case WM_WINDOWPOSCHANGED:
			{
				break;
				bool sizeChanged = (lParam & SWP_NOSIZE) != 0;
				if (sizeChanged)
				{
					instance->Resized(instance->width, instance->height, true);
					return 0;
				}

				break;
			}

			case WM_PAINT:
			{
				if (instance->Paint())
				{
					ValidateRect(instance->handle, NULL);
				}

				return 0;
			}

			case WM_KEYDOWN:						
			{
				int key = wParam;

				instance->KeyDown(key);

				if ((key >=0 ) && (key < 256))
					instance->keys[key] = TRUE;			
				return 0;							
			}

			case WM_KEYUP:								
			{
				int key = wParam;

				instance->KeyUp(key);

				if ((key >=0 ) && (key < 256))
					instance->keys[key] = FALSE;				
				return 0;								
			}

			case WM_LBUTTONDOWN:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseDown(x, y, AxWindowsForm::MouseButton_Left);
					instance->mouseButtons |= AxWindowsForm::MouseButton_Left;

					return 0;
				}

			case WM_LBUTTONUP:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseUp(x, y, AxWindowsForm::MouseButton_Left);
					instance->mouseButtons &= !AxWindowsForm::MouseButton_Left;

					return 0;
				}

			case WM_RBUTTONDOWN:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseDown(x, y, AxWindowsForm::MouseButton_Right);
					instance->mouseButtons |= AxWindowsForm::MouseButton_Right;

					return 0;
				}

			case WM_RBUTTONUP:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseUp(x, y, AxWindowsForm::MouseButton_Right);
					instance->mouseButtons &= !AxWindowsForm::MouseButton_Right;

					return 0;
				}

			case WM_MBUTTONDOWN:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseDown(x, y, AxWindowsForm::MouseButton_Middle);
					instance->mouseButtons |= AxWindowsForm::MouseButton_Middle;

					return 0;
				}

			case WM_MBUTTONUP:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseUp(x, y, AxWindowsForm::MouseButton_Middle);
					instance->mouseButtons &= !AxWindowsForm::MouseButton_Middle;

					return 0;
				}

			case WM_MOUSEMOVE:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);

					instance->MouseMove(x, y);

					if (instance->mouseMoved)
					{
						instance->mouseDeltaX = x - instance->mouseX;
						instance->mouseDeltaY = y - instance->mouseY;
					}
					else 
						instance->mouseMoved = true;
					
					instance->mouseX = x;
					instance->mouseY = y;

					return 0;
				}

			case WM_MOUSEHWHEEL:
				{
					int x = GET_X_LPARAM(lParam); 
					int y = GET_Y_LPARAM(lParam);
					int deltaFactor = GET_WHEEL_DELTA_WPARAM(wParam);

					instance->MouseWheel(x, y, deltaFactor, WHEEL_DELTA);

					return 0;
				}
		}

		int result;
		if (instance->HandleMessage(uMsg, wParam, lParam, &result))
			return result;
	}

	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

AxWindowsForm* AxWindowsForm::GetInstance(HWND instanceHandle)
{
	for (int i = 0; i < AxWindowsForm::instances.count; i++)
	{
		if (AxWindowsForm::instances[i]->handle == instanceHandle)
		{
			return AxWindowsForm::instances[i];
		}
	}

	return NULL;
}

bool AxWindowsForm::ProcessMessages()
{
	bool result = false;

    MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

		result = true;
    }

	return result;
}

int AxWindowsForm::PerformFormLoop()
{
    MSG msg;

	bool done = false;
	while(!done)										
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	
		{
			if (msg.message == WM_QUIT)					
			{
				done = true;							
			}
			else										
			{
				TranslateMessage(&msg);					
				DispatchMessage(&msg);					
			}
		}

		if (!done)
		{
			for (int i = 0; i < AxWindowsForm::instances.count; i++)
			{
				AxWindowsForm::instances[i]->MainLoop();

				AxWindowsForm::instances[i]->ResetLoopVariables();
			}
		}
	}

	return (int)msg.wParam;
	
}

void AxWindowsForm::ResetLoopVariables()
{
	this->mouseDeltaX = 0;
	this->mouseDeltaY = 0;
}

AxWindowsForm::AxWindowsForm(void)
{
	this->ResetLoopVariables();

	this->applicationInstance = GetModuleHandle(NULL);
	ZeroMemory(keys, 256);

	this->left = 0;
	this->top = 0;
	this->width = 800;
	this->height = 600;
	this->caption = L"Axis";
	this->windowMode = AxWindowsForm::WindowMode_Windowed;
	//this->windowMode = AxWindowsForm::WindowMode_FullScreen;
	this->mouseMoved = false;
	this->isSizeOrMove = false;
	this->isSizing = false;
	this->windowState = AxWindowsForm::WindowState_None;

	AxWindowsForm::instances.Add(this);
	//AxWindowsForm::instances.push_back(this);

	this->RegisterWindowClass();
	this->CreateFormWindow();
	this->ShowFormWindow();
}


AxWindowsForm::~AxWindowsForm(void)
{
	AxWindowsForm::instances.Remove(this);
}

bool AxWindowsForm::RegisterWindowClass()
{
	this->windowClass.cbSize = sizeof(WNDCLASSEX);
    this->windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	this->windowClass.lpfnWndProc = (WNDPROC)AxWindowsForm::WndProc;
    this->windowClass.cbClsExtra = 0;
    this->windowClass.cbWndExtra = 0;
    this->windowClass.hInstance = this->applicationInstance;
    this->windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    this->windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    this->windowClass.hbrBackground = NULL;
    this->windowClass.lpszMenuName = NULL;
    this->windowClass.hIconSm = NULL;
	this->windowClass.lpszClassName = AxWindowsForm::FormClassName;

	return RegisterClassExW(&this->windowClass) != 0;
}

bool AxWindowsForm::CreateFormWindow()
{
    long wndStyle = this->windowMode == AxWindowsForm::WindowMode_FullScreen ? WS_POPUP : WS_OVERLAPPEDWINDOW;
	wndStyle |= WS_CLIPCHILDREN | WS_CLIPSIBLINGS;

	// Translate from client rect to window rect
	RECT wRect = { this->left, this->top, this->left + this->width, this->top + this->height };
	AdjustWindowRect(&wRect, wndStyle, false);

	this->handle = CreateWindowW(AxWindowsForm::FormClassName, this->caption,
								 wndStyle,
								 this->left, this->top, wRect.right - wRect.left, wRect.bottom - wRect.top,
								 NULL, NULL,
								 this->applicationInstance,
								 NULL);

	SetWindowLongPtr(this->handle, GWLP_USERDATA, (long)this);

	return this->handle != 0;
}

void AxWindowsForm::ShowFormWindow()
{
	ShowWindow(this->handle,SW_SHOW);
    //UpdateWindow(this->handle);
	SetForegroundWindow(this->handle);						// Slightly Higher Priority
	SetFocus(this->handle);									// Sets Keyboard Focus To The Window
}

void AxWindowsForm::SetPosition(int left, int top)
{
	this->left = left;
	this->top = top;

	SetWindowPos(this->handle, HWND_TOP, this->left, this->top, this->width, this->height, SWP_NOSIZE);
}

void AxWindowsForm::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;

	SetWindowPos(this->handle, HWND_TOP, this->left, this->top, this->width, this->height, SWP_NOMOVE);
}

void AxWindowsForm::SetStyle()
{
	SetWindowLong(this->handle, GWL_STYLE, WS_POPUP | WS_MINIMIZEBOX);
	SetWindowLong(this->handle, GWL_EXSTYLE, 0);
	//SetWindowLong(this->handle, GWL_STYLE, WS_POPUP);
	//SetWindowLong(this->handle, GWL_EXSTYLE, WS_EX_APPWINDOW);
}

void AxWindowsForm::CloseWindow()
{
	PostMessage(this->handle, WM_CLOSE, 0, 0);
}

void AxWindowsForm::SetActive()
{
	SetActiveWindow(this->handle);
}

void AxWindowsForm::SetCaption(const wchar_t *caption)
{
	SetWindowTextW(this->handle, caption);
}


// Virtual Methods
bool AxWindowsForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, int *result) { return false; }
void AxWindowsForm::MainLoop() { }
void AxWindowsForm::Created() { }
bool AxWindowsForm::Close() { return true; }
bool AxWindowsForm::Paint() { return true; }
void AxWindowsForm::KeyDown(int keyCode) { }
void AxWindowsForm::KeyUp(int keyCode) { }
void AxWindowsForm::Resized(int width, int height, bool done) { }
void AxWindowsForm::StateChanged(int oldState, int newState) {}
void AxWindowsForm::MouseMove(int x, int y) { }
void AxWindowsForm::MouseDown(int x, int y, int mouseButton) { }
void AxWindowsForm::MouseUp(int x, int y, int mouseButton) { }
void AxWindowsForm::MouseWheel(int x, int y, int deltaFactor, int delta) { }

