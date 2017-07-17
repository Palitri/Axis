#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxKeyFrame.h"

#include "..\..\..\..\Tools\AxList.h"

class AXDLLCLASS AxKeyFramesTrack :
	public AxList<AxKeyFrame*>
{
public:
	AxKeyFramesTrack(void);
	virtual ~AxKeyFramesTrack(void);

	virtual int GetTransform(AxMatrix &result, float time);
};

