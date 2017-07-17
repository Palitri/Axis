//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyFrameRotationAxis.h"


AxKeyFrameRotationAxis::AxKeyFrameRotationAxis(void)
	: AxKeyFrame()
{
	this->typeId = AxKeyFrameRotationAxis::classId;
}

AxKeyFrameRotationAxis::AxKeyFrameRotationAxis(float time, float angle, AxVector3 &axis)
	: AxKeyFrame(time)
{
	this->typeId = AxKeyFrameRotationAxis::classId;
	
	this->axis = axis;
	this->angle = angle;
}


AxKeyFrameRotationAxis::~AxKeyFrameRotationAxis(void)
{
}

void AxKeyFrameRotationAxis::Process(AxMatrix &result)
{
	AxMatrix::Copy(result, this->previousTransform);
}

void AxKeyFrameRotationAxis::Process(AxMatrix &result, AxKeyFrame *next, float blend)
{
	AxMatrix rotationMatrix;
	AxMatrix::CreateRotationAxis(rotationMatrix, ((AxKeyFrameRotationAxis*)next)->axis, ((AxKeyFrameRotationAxis*)next)->angle * blend);
	AxMatrix::Multiply(result, this->previousTransform, rotationMatrix);
}

void AxKeyFrameRotationAxis::Serialize(AxStream &destination)
{
	AxKeyFrame::Serialize(destination);

	destination.WriteFloat32(this->axis.x);
	destination.WriteFloat32(this->axis.y);
	destination.WriteFloat32(this->axis.z);

	destination.WriteFloat32(this->angle);
}

bool AxKeyFrameRotationAxis::Deserialize(AxStream &source)
{
	if (!AxKeyFrame::Deserialize(source))
		return false;

	this->axis.x = source.ReadFloat32();
	this->axis.y = source.ReadFloat32();
	this->axis.z = source.ReadFloat32();

	this->angle = source.ReadFloat32();
	
	return true;
}
