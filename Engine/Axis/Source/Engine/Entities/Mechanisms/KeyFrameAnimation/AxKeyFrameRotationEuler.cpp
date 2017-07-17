//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyFrameRotationEuler.h"


AxKeyFrameRotationEuler::AxKeyFrameRotationEuler(void)
	: AxKeyFrame()
{
	this->typeId = AxKeyFrameRotationEuler::classId;
}

AxKeyFrameRotationEuler::AxKeyFrameRotationEuler(float time, AxVector3 &eulerAngles)
	: AxKeyFrame(time)
{
	this->typeId = AxKeyFrameRotationEuler::classId;

	this->eulerAngles = eulerAngles;
}

AxKeyFrameRotationEuler::~AxKeyFrameRotationEuler(void)
{
}

void AxKeyFrameRotationEuler::Process(AxMatrix &result)
{
	AxMatrix m;
	AxMatrix::CreateRotationX(result, this->eulerAngles.x);
	AxMatrix::CreateRotationY(m, this->eulerAngles.y);
	AxMatrix::Multiply(result, result, m);
	AxMatrix::CreateRotationZ(m, this->eulerAngles.z);
	AxMatrix::Multiply(result, result, m);
}

void AxKeyFrameRotationEuler::Process(AxMatrix &result, AxKeyFrame *next, float blend)
{
	AxVector3 v;
	AxVector3::Lerp(v, this->eulerAngles, ((AxKeyFrameRotationEuler*)next)->eulerAngles, blend);

	AxMatrix m;
	AxMatrix::CreateRotationX(result, v.x);
	AxMatrix::CreateRotationY(m, v.y);
	AxMatrix::Multiply(result, result, m);
	AxMatrix::CreateRotationZ(m, v.z);
	AxMatrix::Multiply(result, result, m);
}

void AxKeyFrameRotationEuler::Serialize(AxStream &destination)
{
	AxKeyFrame::Serialize(destination);

	destination.WriteFloat32(this->eulerAngles.x);
	destination.WriteFloat32(this->eulerAngles.y);
	destination.WriteFloat32(this->eulerAngles.z);
}

bool AxKeyFrameRotationEuler::Deserialize(AxStream &source)
{
	if (!AxKeyFrame::Deserialize(source))
		return false;

	this->eulerAngles.x = source.ReadFloat32();
	this->eulerAngles.y = source.ReadFloat32();
	this->eulerAngles.z = source.ReadFloat32();

	return true;
}
