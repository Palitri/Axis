#pragma once

#include "AxPixelFormat.h"

#include <windows.h>

class DIBBitmap
{
public:
	DIBBitmap(void);
	~DIBBitmap(void);

	static HBITMAP CreateBitmap(int width, int height, AxPixelFormat &dataFormat, void* data);
};
