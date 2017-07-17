#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxMedia.h"

#include "..\..\Engine\Primitive\AxPixelFormat.h"

#include "..\..\Tools\Streams\AxStream.h"

// TODO : For symmetry, either merge AxImage and AxTexture (by having AxImage as AxDeviceIndependentTexture, for example) or make a class which corresponds to AxImage for meshes (having the functionality of AxDeviceIndependentMesh)
class AXDLLCLASS AxImage :
	public AxMedia
{
private:
	void Init();

//protected:
	//int exportEncoding;

public:
	int width, height, depth;
	unsigned char *pixelData;
	AxPixelFormat pixelFormat;


	AxImage(void);
	AxImage(AxImage &source);
	AxImage(AxImage &source, AxPixelFormat &pixelFormat);
	virtual ~AxImage(void);

	virtual bool Load(AxStream &source);
	virtual bool Save(AxStream &destination, const char *format);

	void SetPixelFormat(AxPixelFormat &pixelFormat, bool dither = true);
	AxPixelFormat GetPixelFormat();
	
	void SetSize(int width, int height, int depth);

	void CopyPixels(AxImage &source, int srcX, int srcY, int srcZ, int destX, int destY, int destZ, int width, int height, int depth);
	void CopyPixels(AxImage &source, int srcX, int srcY, int srcZ, int width, int height, int depth);
	void CopyPixels(AxImage &source);
	void Copy(AxImage &source);

	void SetPixel(int x, int y, int z, int unsigned value);
	unsigned int GetPixel(int x, int y, int z);
	unsigned int GetSample(float u, float v, int z);
	unsigned int GetSample(float u, float v, float w);
	// TODO: Consider adding a sampler class (Nearest, Linear, Cubic, etc) and moving the GetSample methods there
};