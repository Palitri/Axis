#pragma once

#include "..\AxGlobals.h"

#include "AxTraceEvents.h"

class AXDLLCLASS AxRayIntersectionEvents :
	public AxTraceEvents
{
private:
	AxVector3 rayPoint, rayVector;
    float intersectionDistance;

public:
    AxTraceParameters traceResult;
    AxIntersectionInfo intersectionInfo;
    bool intersected;
	
	AxRayIntersectionEvents(Axis &context);
	~AxRayIntersectionEvents(void);

	void SetupRay(AxVector3 &rayInitialPoint, AxVector3 &rayOrientation);

	void OnMesh(AxTraceParameters &parameters);
    void OnTransform(AxTraceParameters &parameters);
};

