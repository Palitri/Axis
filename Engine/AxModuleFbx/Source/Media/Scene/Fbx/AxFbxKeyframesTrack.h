#pragma once

#include "..\..\..\AxisEngine.h"

#include "AxFbxPrimitiveTypes.h"

#include <fbxsdk.h>

class AxFbxKeyframesTrack
{
public:
	bool valid;
	AxList<AxFbxKeyframe> keyframes;

	AxFbxKeyframesTrack(void);
	~AxFbxKeyframesTrack(void);

	bool FindFrameKey(int &index, float time);
	void ProcessFbxCurve(FbxAnimCurve* curve, AxFbxKeyFrameValueComponent valueComponent, float defaultValue = 0.0f);
};

