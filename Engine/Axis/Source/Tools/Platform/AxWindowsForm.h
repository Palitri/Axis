#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxList.h"

#include <windows.h>

class AxWindowsForm;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxWindowsForm*>;

class AXDLLCLASS AxWindowsForm
{
private:
	bool mouseMoved;
	bool isSizeOrMove, isSizing;
	int windowState;

	void ResetLoopVariables();

public:
	static const int WindowMode_Windowed = 0;
	static const int WindowMode_FullScreen = 1;
					 
	static const int MouseButton_None = 0;
	static const int MouseButton_Left = 1;
	static const int MouseButton_Right = 2;
	static const int MouseButton_Middle = 4;

	static const int WindowState_None = 0;
	static const int WindowState_Normal = 1;
	static const int WindowState_Maximized = 2;
	static const int WindowState_Minimized = 3;


	static const wchar_t* FormClassName;

	static AxList<AxWindowsForm*> instances;

	HINSTANCE applicationInstance;
	WNDCLASSEXW windowClass;
	HWND handle;

	bool keys[256]; // Left- and right-distinguishing constants are available to an application only through the GetKeyboardState, SetKeyboardState, GetAsyncKeyState, GetKeyState, and MapVirtualKey functions.
	int mouseX, mouseY,	mouseDeltaX, mouseDeltaY, mouseButtons;
	bool suppressScreenSaver, suppressMonitorPowerSave;
	
	bool active;
	int windowMode;
	int left, top, width, height;
	wchar_t *caption;


	AxWindowsForm(void);
	virtual ~AxWindowsForm(void);

	bool RegisterWindowClass();
	bool CreateFormWindow();
	void ShowFormWindow();
	void SetPosition(int left, int top);
	void SetSize(int width, int height);
	void SetStyle();
	void CloseWindow();
	void SetActive();
	void SetCaption(const wchar_t *caption);

	virtual bool HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, int *result);
	virtual void MainLoop();
	virtual void Created();
	virtual bool Close();
	virtual bool Paint();
	virtual void KeyDown(int keyCode);
	virtual void KeyUp(int keyCode);
	virtual void Resized(int width, int height, bool done);
	virtual void StateChanged(int oldState, int newState);
	virtual void MouseMove(int x, int y);
	virtual void MouseDown(int x, int y, int mouseButton);
	virtual void MouseUp(int x, int y, int mouseButton);
	virtual void MouseWheel(int x, int y, int deltaFactor, int delta);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	static AxWindowsForm* GetInstance(HWND instanceHandle);
	static bool ProcessMessages();
	static int PerformFormLoop();
};

