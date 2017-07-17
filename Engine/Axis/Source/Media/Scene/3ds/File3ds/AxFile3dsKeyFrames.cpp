//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3dsKeyFrames.h"

AxFile3dsKeyFrameBase::AxFile3dsKeyFrameBase(void)
{
}

AxFile3dsKeyFrameBase::~AxFile3dsKeyFrameBase(void)
{
}


AxFile3dsKeyFramesTrack::AxFile3dsKeyFramesTrack(void)
{
}

AxFile3dsKeyFramesTrack::~AxFile3dsKeyFramesTrack(void)
{
	for (int i = 0; i < this->data.count; i++)
		delete this->data[i];
}


AxFile3dsKeyFrameObject::AxFile3dsKeyFrameObject(void)
{
	this->position = 0;
	this->rotation = 0;
	this->scaling = 0;

	this->pivot.x = 0.0f;
	this->pivot.y = 0.0f;
	this->pivot.z = 0.0f;
}

AxFile3dsKeyFrameObject::~AxFile3dsKeyFrameObject(void)
{
	if (this->position != 0)
		delete this->position;

	if (this->rotation != 0)
		delete this->rotation;

	if (this->scaling != 0)
		delete this->scaling;
}


AxFile3dsKeyFrames::AxFile3dsKeyFrames(void)
{
}

AxFile3dsKeyFrames::~AxFile3dsKeyFrames(void)
{
	for (int i = 0; i < this->objects.count; i++)
		delete this->objects[i];
}