//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyFrameTranslation.h"


AxKeyFrameTranslation::AxKeyFrameTranslation(void)
	: AxKeyFrame()
{
	this->typeId = AxKeyFrameTranslation::classId;
}

AxKeyFrameTranslation::AxKeyFrameTranslation(float time, AxVector3 &position)
	: AxKeyFrame(time)
{
	this->typeId = AxKeyFrameTranslation::classId;

	this->position = position;
}

AxKeyFrameTranslation::~AxKeyFrameTranslation(void)
{
}

void AxKeyFrameTranslation::Process(AxMatrix &result)
{
	AxMatrix::CreateTranslation(result, this->position);
}

void AxKeyFrameTranslation::Process(AxMatrix &result, AxKeyFrame *next, float blend)
{
	AxVector3 v;
	AxVector3::Lerp(v, this->position, ((AxKeyFrameTranslation*)next)->position, blend);
	AxMatrix::CreateTranslation(result, v);
}

void AxKeyFrameTranslation::Serialize(AxStream &destination)
{
	AxKeyFrame::Serialize(destination);

	destination.WriteFloat32(this->position.x);
	destination.WriteFloat32(this->position.y);
	destination.WriteFloat32(this->position.z);
}

bool AxKeyFrameTranslation::Deserialize(AxStream &source)
{
	if (!AxKeyFrame::Deserialize(source))
		return false;

	this->position.x = source.ReadFloat32();
	this->position.y = source.ReadFloat32();
	this->position.z = source.ReadFloat32();

	return true;
}
