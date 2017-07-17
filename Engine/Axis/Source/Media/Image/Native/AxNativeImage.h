// The native Axis image file format supports multiple images of different sizes, pixel format and encoding, each image with three dimensions - width, height and depth
// Generally, an image file would consist of a single two dimensional image (depth = 1)
// The data chunk contains the actual pixel data of an image. The chunks encountered before the data chunk are feature which apply to the data chunk. Typically, a data chunk should be preceeded by at least a size and a format chunks, however if not present, by specification, chunks should have definite default values to be assumed.
// Multiple data chunks may consequently follow one after another, in which case they will have the same features.
// New features can be added to the file format in the future, by adding new types of feature chunks to apply to the image data
// For future encodings other than raw (compressed encodings), the encoding may present its specific chunks, for example with parameters of the encoding and the encoded data itself, which can be subchunks of the Data chunk

#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxImage.h"

class AXDLLCLASS AxNativeImage
	: public AxImage
{
private:
	static const unsigned int SerializationId_Main		= 0x4d495841; // AXIM
	static const unsigned int SerializationId_Data		= 0x00004d49;
	static const unsigned int SerializationId_Size		= 0x00014d49;
	static const unsigned int SerializationId_Format	= 0x00024d49;
	static const unsigned int SerializationId_Encoding	= 0x00034d49;

	static const unsigned short Encoding_Raw		= 0x0000;

public:
	static const char *formatName;

	AxNativeImage(void);
	AxNativeImage(AxImage &source);
	AxNativeImage(AxImage &source, AxPixelFormat &pixelFormat);
	~AxNativeImage(void);

	virtual bool Load(AxStream &source);
	virtual bool Save(AxStream &destination, const char *format = "");
};

