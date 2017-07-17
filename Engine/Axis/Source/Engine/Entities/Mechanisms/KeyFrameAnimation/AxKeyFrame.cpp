//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyFrame.h"


AxKeyFrame::AxKeyFrame(void)
{
	this->time = 0.0f;
}

AxKeyFrame::AxKeyFrame(float time)
{
	this->time = time;
}

AxKeyFrame::~AxKeyFrame(void)
{
}

void AxKeyFrame::Serialize(AxStream &destination)
{
	destination.WriteFloat32(this->time);
}

bool AxKeyFrame::Deserialize(AxStream &source)
{
	this->time = source.ReadFloat32();
	
	return true;
}
