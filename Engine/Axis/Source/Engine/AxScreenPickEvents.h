#pragma once

#include "..\AxGlobals.h"

#include "AxTraceEvents.h"


class AXDLLCLASS AxScreenPickEvents :
	public AxTraceEvents
{
private:
	AxVector3 rayPoint1, rayPoint2;

public:
    AxIntersectionInfo intersectionInfo;
    AxTraceParameters entityInfo;

	AxScreenPickEvents(Axis &Axis);
	~AxScreenPickEvents(void);

    void SetupScreenPick(AxVector2 &screenCoords);
	
	void OnMesh(AxTraceParameters &parameters);
    void OnTransform(AxTraceParameters &parameters);
};

