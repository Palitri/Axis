#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxKeyFrame.h"

#include "..\..\..\Primitive\AxVector3.h"

class AXDLLCLASS AxKeyFrameTranslation :
	public AxKeyFrame
{
public:
	static const int classId = 0x21111518;

	AxVector3 position;

	AxKeyFrameTranslation(void);
	AxKeyFrameTranslation(float time, AxVector3 &position);
	virtual ~AxKeyFrameTranslation(void);

	virtual void Process(AxMatrix &result);
	virtual void Process(AxMatrix &result, AxKeyFrame *next, float blend);

	virtual void Serialize(AxStream &destination);
	virtual bool Deserialize(AxStream &source);
};
