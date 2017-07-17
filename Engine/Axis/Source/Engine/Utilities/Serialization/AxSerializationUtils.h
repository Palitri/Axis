// Provides serialization and deserialization routines for simpler Axis types such as vectors, matrices and etc.
// These formats are considered primitive, which is why they don't implement their serialization and, as atom types such as int, are not wrapped inside container chunks when serialized in the stream
// Unlike modules, such as image, scene and sound, which can be serialized in various different formats (.png, .jpg; .3ds, .obj; .wav, .ogg and ect.), these types are always serizlized the same way in a simple, raw form.
// The serialization methods are not members of the respective serialized type, because these types tend to be as simple as possible and follow the pattern of atom types such as int, which don't implement their own serialization, but are rather serialized by other objects

#pragma once

#include "..\..\..\AxGlobals.h"

class AxStream;

class AxVector2;
class AxVector3;
class AxVector4;
class AxMatrix;
class AxBoundingVolume;
class AxPixelFormat;

class AXDLLCLASS AxSerializationUtils
{
public:
	AxSerializationUtils(void);
	~AxSerializationUtils(void);

	static void SerializeVector2(AxStream &stream, AxVector2 &vector);
	static AxVector2 DeserializeVector2(AxStream &stream);

	static void SerializeVector3(AxStream &stream, AxVector3 &vector);
	static AxVector3 DeserializeVector3(AxStream &stream);

	static void SerializeVector4(AxStream &stream, AxVector4 &vector);
	static AxVector4 DeserializeVector4(AxStream &stream);

	static void SerializeMatrix(AxStream &destination, AxMatrix &matrix);
	static void DeserializeMatrix(AxMatrix &result, AxStream &source);

	static void SerializePixelFormat(AxStream &destination, AxPixelFormat &format);
	static AxPixelFormat DeserializePixelFormat(AxStream &source);
};

