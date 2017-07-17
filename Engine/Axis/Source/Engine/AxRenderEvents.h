#pragma once

#include "..\AxGlobals.h"

#include "AxTraceEvents.h"

class AXDLLCLASS AxRenderEvents :
	public AxTraceEvents
{
public:
	int numRenderedObjects;
	bool updateState;

	
	AxRenderEvents(Axis &Axis);
	~AxRenderEvents(void);

	virtual void SetupNewFrame();

	virtual void OnMaterial(AxTraceParameters &parameters);
    virtual void OnMesh(AxTraceParameters &parameters);
    virtual void OnMechanism(AxTraceParameters &parameters);
    virtual void OnTransform(AxTraceParameters &parameters);
    virtual void OnLight(AxTraceParameters &parameters);
	virtual void OnSoundEmitter(AxTraceParameters &parameters);
	virtual void OnSoundReceptor(AxTraceParameters &parameters);
	virtual void OnTexture2D(AxTraceParameters &parameters);
	virtual void OnExitEntitySet(AxTraceParameters &parameters);
};

