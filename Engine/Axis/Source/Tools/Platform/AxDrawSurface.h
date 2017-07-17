#pragma once

#include "..\..\AxGlobals.h"

#include <windows.h>

class AXDLLCLASS AxDrawSurface
{
public:
	void* pixelData;
	int width, height, pixelDataSize;
	BITMAPINFO bitmapInfo;

	AxDrawSurface(void);
	~AxDrawSurface(void);

	void SetSize(int width, int height);

	void Copy(void* pixelData);

	void Present(int x, int y, HDC deviceContext);
};

