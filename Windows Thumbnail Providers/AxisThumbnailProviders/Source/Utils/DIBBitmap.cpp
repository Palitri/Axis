#include "DIBBitmap.h"

DIBBitmap::DIBBitmap(void)
{
}


DIBBitmap::~DIBBitmap(void)
{
}

HBITMAP DIBBitmap::CreateBitmap(int width, int height, AxPixelFormat &dataFormat, void* data)
{
	HBITMAP result;

	BITMAPINFO bmi = { sizeof(bmi.bmiHeader) };
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = height;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    unsigned char *dibBits;
    result = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&dibBits, NULL, 0);
	//memcpy(dibBits, data, width * height * 4);
	//return result;

	unsigned char *source = (unsigned char*)data;
	int sourceOffset = 0;
	int destOffset = 0;
	int alphaOffset = dataFormat.FindChannelIdIndex(AxPixelFormat::ChannelIdAlpha);
	int redOffset = dataFormat.FindChannelIdIndex(AxPixelFormat::ChannelIdRed);
	int greenOffset = dataFormat.FindChannelIdIndex(AxPixelFormat::ChannelIdGreen);
	int blueOffset = dataFormat.FindChannelIdIndex(AxPixelFormat::ChannelIdBlue);
	for (int y = 0; y < height; y++)
	{
		//int destOffset = ((height - 1 - y) * width) * 4;
		for (int x = 0; x < width; x++)
		{
			dibBits[destOffset++] = source[sourceOffset + blueOffset];
			dibBits[destOffset++] = source[sourceOffset + greenOffset];
			dibBits[destOffset++] = source[sourceOffset + redOffset];
			dibBits[destOffset++] = source[sourceOffset + alphaOffset];

			sourceOffset += 4;
		}
	}

	return result;
}
