#pragma once

#include "..\..\..\AxisEngine.h"

struct AxFbxKeyframe
{
	float time	;
	AxVector3 value;
};

enum AxFbxKeyFrameValueComponent
{
	AxFbxKeyFrameValueComponentX,
	AxFbxKeyFrameValueComponentY,
	AxFbxKeyFrameValueComponentZ
};