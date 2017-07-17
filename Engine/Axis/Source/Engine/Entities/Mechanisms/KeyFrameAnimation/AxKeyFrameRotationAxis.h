#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxKeyFrame.h"

#include "..\..\..\Primitive\AxVector3.h"
#include "..\..\..\Primitive\AxMatrix.h"

class AXDLLCLASS AxKeyFrameRotationAxis :
	public AxKeyFrame
{
public:
	static const int classId = 0x21111418;

	AxMatrix previousTransform;
    AxVector3 axis;
    float angle;

	AxKeyFrameRotationAxis(void);
	AxKeyFrameRotationAxis(float time, float angle, AxVector3 &axis);
	virtual ~AxKeyFrameRotationAxis(void);

	virtual void Process(AxMatrix &result);
	virtual void Process(AxMatrix &result, AxKeyFrame *next, float blend);

	virtual void Serialize(AxStream &destination);
	virtual bool Deserialize(AxStream &source);
};

