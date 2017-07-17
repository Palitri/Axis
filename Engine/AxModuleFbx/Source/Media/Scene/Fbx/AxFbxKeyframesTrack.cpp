//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFbxKeyframesTrack.h"


AxFbxKeyframesTrack::AxFbxKeyframesTrack(void)
{
	this->valid = true;
}


AxFbxKeyframesTrack::~AxFbxKeyframesTrack(void)
{
}

bool AxFbxKeyframesTrack::FindFrameKey(int &index, float time)
{
	index = 0;

	//int left = 0;
    //int right = this->keyframes.count - 1;

    //while (left <= right)
    //{
    //    index = left + (right - left) / 2;

    //    if (this->keyframes[index].time == time)
    //        return true;

    //    if (this->keyframes[index].time < time)
    //        left = index + 1;
    //    else
    //        right = index - 1;
    //}

    //return false;

	while (index < this->keyframes.count)
	{
		if (this->keyframes[index].time == time)
			return true;

		if (this->keyframes[index].time > time)
			return false;
		
		index++;
	}

	return false;
}


void AxFbxKeyframesTrack::ProcessFbxCurve(FbxAnimCurve* curve, AxFbxKeyFrameValueComponent valueComponent, float defaultValue)
{
	//if ((curve == 0) || (curve->KeyGetCount() == 0))
	//{
	//	this->keyframes.Clear();
	//	this->valid = true;
	//}

	//if (!this->valid)
	//	return;
	
	if (curve == 0)
		return;

	int numKeys = curve->KeyGetCount();
	for (int keyIndex = 0; keyIndex < numKeys; keyIndex++)
	{
		float keyTime = (float)curve->KeyGetTime(keyIndex).GetSecondDouble();
		//float keyTime = curve->KeyGetTime(keyIndex).GetFrameCount();
		//float keyValueComponent = curve->EvaluateIndex(keyIndex); 
		float keyValueComponent = curve->KeyGetValue(keyIndex);

		bool isConstant = (curve->KeyGetInterpolation(keyIndex) & FbxAnimCurveDef::eInterpolationConstant) != 0;
		bool isLinear = (curve->KeyGetInterpolation(keyIndex) & FbxAnimCurveDef::eInterpolationLinear) != 0;
		bool isCubic = (curve->KeyGetInterpolation(keyIndex) & FbxAnimCurveDef::eInterpolationCubic) != 0;
		
		int trackKey = 0;
		if (!this->FindFrameKey(trackKey, keyTime))
		{
			//if (trackKey != this->keyframes.count)
			//	return;

			AxFbxKeyframe newKeyframe;
			newKeyframe.time = keyTime;
			newKeyframe.value.Set(defaultValue);
			this->keyframes.Insert(trackKey, newKeyframe);
			trackKey = this->keyframes.count - 1;
		}

		switch (valueComponent)
		{
			case AxFbxKeyFrameValueComponentX:
				this->keyframes[trackKey].value.x = keyValueComponent;
				break;

			case AxFbxKeyFrameValueComponentY:
				this->keyframes[trackKey].value.y = keyValueComponent;
				break;

			case AxFbxKeyFrameValueComponentZ:
				this->keyframes[trackKey].value.z = keyValueComponent;
				break;
		}
	}
}