#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\Base\AxEntity.h"

#include "..\..\..\Primitive\AxMatrix.h"

#include "..\..\..\..\Tools\Streams\AxStream.h"

class AXDLLCLASS AxKeyFrame :
	public AxEntity
{
public:
	float time;

	AxKeyFrame(void);
	AxKeyFrame(float time);
	virtual ~AxKeyFrame(void);

	// Gets the resulting transform of the key frame
	virtual void Process(AxMatrix &result) = 0;
	// Gets a transform, which is the result of the keyframe, blended with another one, with blending factor given by blend. Normally the another key frame is of the same object and blend is in the range between 0 and 1
	virtual void Process(AxMatrix &result, AxKeyFrame *next, float blend) = 0;

	// Serializes the keyframe data to a stream. See base class for more info.
	virtual void Serialize(AxStream &destination);
	// Deserializes the keyframe from a stream. See base class for more info.
	virtual bool Deserialize(AxStream &source);
};

