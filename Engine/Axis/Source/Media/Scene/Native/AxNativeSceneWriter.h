#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxString.h"
#include "..\..\..\Tools\Streams\AxStream.h"

class Axis;

class AXDLLCLASS AxNativeSceneWriter
{
public:
	Axis *context;

	AxNativeSceneWriter(Axis *context);
	~AxNativeSceneWriter(void);

	void Write(AxStream &destination);
};

