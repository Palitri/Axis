//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSerializationUtils.h"

#include "..\..\..\Tools\Streams\AxStream.h"

#include "..\..\Primitive\AxVector2.h"
#include "..\..\Primitive\AxVector3.h"
#include "..\..\Primitive\AxVector4.h"
#include "..\..\Primitive\AxMatrix.h"
#include "..\..\Primitive\AxBoundingVolume.h"
#include "..\..\Primitive\AxPixelFormat.h"


AxSerializationUtils::AxSerializationUtils(void)
{
}


AxSerializationUtils::~AxSerializationUtils(void)
{
}

void AxSerializationUtils::SerializeVector2(AxStream &destination, AxVector2 &vector)
{
	destination.WriteFloat32(vector.x);
	destination.WriteFloat32(vector.y);
}

AxVector2 AxSerializationUtils::DeserializeVector2(AxStream &source)
{
	AxVector2 result;
	result.x = source.ReadFloat32();
	result.y = source.ReadFloat32();
	return result;
}

void AxSerializationUtils::SerializeVector3(AxStream &destination, AxVector3 &vector)
{
	destination.WriteFloat32(vector.x);
	destination.WriteFloat32(vector.y);
	destination.WriteFloat32(vector.z);
}

AxVector3 AxSerializationUtils::DeserializeVector3(AxStream &source)
{
	AxVector3 result;
	result.x = source.ReadFloat32();
	result.y = source.ReadFloat32();
	result.z = source.ReadFloat32();
	return result;
}

void AxSerializationUtils::SerializeVector4(AxStream &destination, AxVector4 &vector)
{
	destination.WriteFloat32(vector.x);
	destination.WriteFloat32(vector.y);
	destination.WriteFloat32(vector.z);
	destination.WriteFloat32(vector.w);
}

AxVector4 AxSerializationUtils::DeserializeVector4(AxStream &source)
{
	AxVector4 result;
	result.x = source.ReadFloat32();
	result.y = source.ReadFloat32();
	result.z = source.ReadFloat32();
	result.w = source.ReadFloat32();
	return result;
}

void AxSerializationUtils::SerializeMatrix(AxStream &destination, AxMatrix &matrix)
{
	destination.WriteFloat32(matrix._11);
	destination.WriteFloat32(matrix._12);
	destination.WriteFloat32(matrix._13);
	destination.WriteFloat32(matrix._14);
	destination.WriteFloat32(matrix._21);
	destination.WriteFloat32(matrix._22);
	destination.WriteFloat32(matrix._23);
	destination.WriteFloat32(matrix._24);
	destination.WriteFloat32(matrix._31);
	destination.WriteFloat32(matrix._32);
	destination.WriteFloat32(matrix._33);
	destination.WriteFloat32(matrix._34);
	destination.WriteFloat32(matrix._41);
	destination.WriteFloat32(matrix._42);
	destination.WriteFloat32(matrix._43);
	destination.WriteFloat32(matrix._44);
}

void AxSerializationUtils::DeserializeMatrix(AxMatrix &result, AxStream &source)
{
	result._11 = source.ReadFloat32();
	result._12 = source.ReadFloat32();
	result._13 = source.ReadFloat32();
	result._14 = source.ReadFloat32();
	result._21 = source.ReadFloat32();
	result._22 = source.ReadFloat32();
	result._23 = source.ReadFloat32();
	result._24 = source.ReadFloat32();
	result._31 = source.ReadFloat32();
	result._32 = source.ReadFloat32();
	result._33 = source.ReadFloat32();
	result._34 = source.ReadFloat32();
	result._41 = source.ReadFloat32();
	result._42 = source.ReadFloat32();
	result._43 = source.ReadFloat32();
	result._44 = source.ReadFloat32();
}

void AxSerializationUtils::SerializePixelFormat(AxStream &destination, AxPixelFormat &format)
{
	destination.WriteInt16(format.numChannels);
	destination.WriteInt16(format.sizeInBytes);
	destination.WriteData(format.channelId, format.numChannels);
	destination.WriteData(format.channelBitCount, format.numChannels);
}

AxPixelFormat AxSerializationUtils::DeserializePixelFormat(AxStream &source)
{
	AxPixelFormat result;

	result.numChannels = source.ReadInt16();
	result.sizeInBytes = source.ReadInt16();
	source.ReadData(result.channelId, result.numChannels);
	source.ReadData(result.channelBitCount, result.numChannels);

	return result;
}

