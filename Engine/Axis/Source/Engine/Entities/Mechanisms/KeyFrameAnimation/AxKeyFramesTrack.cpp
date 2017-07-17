//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxKeyFramesTrack.h"

#include "..\..\..\Base\AxClassId.h"


AxKeyFramesTrack::AxKeyFramesTrack(void)
	: AxList<AxKeyFrame*>()
{
}


AxKeyFramesTrack::~AxKeyFramesTrack(void)
{
	for (int i = 0; i < this->count; i++)
		delete this->Get(i);
}

int AxKeyFramesTrack::GetTransform(AxMatrix &result, float time)
{
	int count = this->count;
	
	// No keyframes
	if (count == 0)
	{
		AxMatrix::CreateIdentity(result);
		return AxClassId::None;
	}

	// Only one keyframe
	else if (count == 1)
	{
		this->Get(0)->Process(result);
		return this->Get(0)->typeId;
	}

	// Time outside of left boundary
	else if (time <= this->Get(0)->time)
	{
		this->Get(0)->Process(result);
		return this->Get(0)->typeId;
	}

	// Time outside of right boundary
	else if (time >= this->Get(count - 1)->time)
	{
		this->Get(count - 1)->Process(result);
		return this->Get(count - 1)->typeId;
	}

	// Search - a minimum of 2 key frames are required
	else
	{
		int left = 0;
		int right = count - 2;

		int index;
		float startTime, endTime;

		do
		{
			index = left + (right - left) / 2;
			startTime = this->Get(index)->time;
			endTime = this->Get(index + 1)->time;

			if (time > endTime)
				left = index + 1;
			else if (time < startTime)
				right = index - 1;
			else
				break;
		}
		while (left <= right);

		float blend = (time - startTime) / (float)(endTime - startTime);

		this->Get(index)->Process(result, this->Get(index + 1), blend);

		return this->Get(index)->typeId;
	}
}
