#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxKeyFrame.h"

#include "..\..\..\Primitive\AxVector3.h"

class AXDLLCLASS AxKeyFrameScaling :
	public AxKeyFrame
{
public:
	static const int classId = 0x21111218;

	AxVector3 scaling;

	AxKeyFrameScaling(void);
	AxKeyFrameScaling(float time, AxVector3 &scaling);
	virtual ~AxKeyFrameScaling(void);

	virtual void Process(AxMatrix &result);
	virtual void Process(AxMatrix &result, AxKeyFrame *next, float blend);

	virtual void Serialize(AxStream &destination);
	virtual bool Deserialize(AxStream &source);
};

