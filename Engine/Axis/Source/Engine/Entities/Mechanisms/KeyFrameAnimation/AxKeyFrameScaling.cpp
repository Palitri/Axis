//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyFrameScaling.h"


AxKeyFrameScaling::AxKeyFrameScaling(void)
	: AxKeyFrame()
{
	this->typeId = AxKeyFrameScaling::classId;
}

AxKeyFrameScaling::AxKeyFrameScaling(float time, AxVector3 &scaling)
	: AxKeyFrame(time)
{
	this->typeId = AxKeyFrameScaling::classId;

	this->scaling = scaling;
}

AxKeyFrameScaling::~AxKeyFrameScaling(void)
{
}

void AxKeyFrameScaling::Process(AxMatrix &result)
{
	AxMatrix::CreateScaling(result, this->scaling);
}

void AxKeyFrameScaling::Process(AxMatrix &result, AxKeyFrame *next, float blend)
{
	AxVector3 v;
	AxVector3::Lerp(v, this->scaling, ((AxKeyFrameScaling*)next)->scaling, blend);
	AxMatrix::CreateScaling(result, v);
}

void AxKeyFrameScaling::Serialize(AxStream &destination)
{
	AxKeyFrame::Serialize(destination);

	destination.WriteFloat32(this->scaling.x);
	destination.WriteFloat32(this->scaling.y);
	destination.WriteFloat32(this->scaling.z);
}

bool AxKeyFrameScaling::Deserialize(AxStream &source)
{
	if (!AxKeyFrame::Deserialize(source))
		return false;

	this->scaling.x = source.ReadFloat32();
	this->scaling.y = source.ReadFloat32();
	this->scaling.z = source.ReadFloat32();

	return true;
}
