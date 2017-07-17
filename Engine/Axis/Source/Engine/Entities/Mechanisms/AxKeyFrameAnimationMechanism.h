#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxMechanism.h"

#include "..\..\Entities\Transform\AxTransform.h"

#include "KeyFrameAnimation\AxKeyFramesTrack.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxKeyFramesTrack*>;

class Axis;

class AXDLLCLASS AxKeyframeAnimationMechanism
	: public AxMechanism
{
private:
	static const unsigned int SerializationId_KeyFrameTracks		= 0x21111018;
	static const unsigned int SerializationId_KeyFrameTrack			= 0x21111118;
	static const unsigned int SerializationId_KeyFrameTrackFrames	= 0x21111128;

	AxKeyFramesTrack *currentDeserializationTrack;

public:
	static const int classId = AxMechanism::classId | 107;

	static const int propertyIndex_Frame		= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Transform	= AxResource::propertyIndex_ChildPropertiesIndex + 1;

	Axis *context;
	AxList<AxKeyFramesTrack*> keyFramesTracks;

    AxKeyframeAnimationMechanism(Axis *context);
	virtual ~AxKeyframeAnimationMechanism(void);

    bool Process(float deltaTime);

	// Precalculates data, which is needed for a AxKeyFramesTrack composed out of AxKeyFrameRotationAxis to work properly. This is needed for optimization, because the result of each AxKeyFrameRotationAxis keyframe is affected by all previous ones
	void BuildRotationAxisTrack(AxKeyFramesTrack *rotationAxisTrack);
    
	// Serialize key frame data. See base class method for more info
	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	// Deserialize key frame data. See base class method for more info
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};
