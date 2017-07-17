#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsPrimitiveTypes.h"

#include "..\..\..\..\Tools\AxString.h"
#include "..\..\..\..\Tools\AxList.h"


class AXDLLCLASS AxFile3dsKeyFrameBase
{
public:
    int frameNumber;
    int flags;
    float tension, continuity, bias, easeTo, easeFrom;

	AxFile3dsKeyFrameBase::AxFile3dsKeyFrameBase(void);
	virtual AxFile3dsKeyFrameBase::~AxFile3dsKeyFrameBase(void);
};

class AXDLLCLASS AxFile3dsKeyFramePosition : 
	public AxFile3dsKeyFrameBase
{
public:
    AxFile3dsVector position;
};

class AXDLLCLASS AxFile3dsKeyFrameRotation : 
	public AxFile3dsKeyFrameBase
{
public:
    float angle;
    AxFile3dsVector axis;
};

class AXDLLCLASS AxFile3dsKeyFrameScaling : 
	public AxFile3dsKeyFrameBase
{
public:
    AxFile3dsVector scaling;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFile3dsKeyFrameBase*>;

class AXDLLCLASS AxFile3dsKeyFramesTrack
{
public:
    int flags, keys;
    AxList<AxFile3dsKeyFrameBase*> data;

	AxFile3dsKeyFramesTrack(void);
	~AxFile3dsKeyFramesTrack(void);
};
	
class AXDLLCLASS AxFile3dsKeyFrameObject
{
public:
    AxString name, instanceName;
    int hierarchyLevel, id;
    AxFile3dsVector pivot;
    AxFile3dsKeyFramesTrack *position, *rotation, *scaling;

	AxFile3dsKeyFrameObject(void);
	~AxFile3dsKeyFrameObject(void);
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFile3dsKeyFrameObject*>;

class AXDLLCLASS AxFile3dsKeyFrames
{
public:
	int start, end, current;
	AxList<AxFile3dsKeyFrameObject*> objects;

	AxFile3dsKeyFrames(void);
	~AxFile3dsKeyFrames(void);
};
