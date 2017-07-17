#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxKeyFrame.h"

#include "..\..\..\Primitive\AxVector3.h"

class AXDLLCLASS AxKeyFrameRotationEuler :
	public AxKeyFrame
{
public:
	static const int classId = 0x21111318;

	AxVector3 eulerAngles;
	
	AxKeyFrameRotationEuler(void);
	AxKeyFrameRotationEuler(float time, AxVector3 &eulerAngles);
	virtual ~AxKeyFrameRotationEuler(void);

	virtual void Process(AxMatrix &result);
	virtual void Process(AxMatrix &result, AxKeyFrame *next, float blend);

	virtual void Serialize(AxStream &destination);
	virtual bool Deserialize(AxStream &source);
};

