#pragma once

#include "..\AxGlobals.h"

#include "AxTraceParameters.h"

class Axis;

class AXDLLCLASS AxTraceEvents
{
public:
    Axis *context;

	AxTraceEvents(Axis &context);
	virtual ~AxTraceEvents(void);

	virtual void OnMesh(AxTraceParameters &parameters);
	virtual void OnCamera(AxTraceParameters &parameters);
	virtual void OnTransform(AxTraceParameters &parameters);
	virtual void OnRenderTarget(AxTraceParameters &parameters);
	virtual void OnMaterial(AxTraceParameters &parameters);
	virtual void OnMechanism(AxTraceParameters &parameters);
	virtual void OnLight(AxTraceParameters &parameters);
	virtual void OnSoundEmitter(AxTraceParameters &parameters);
	virtual void OnSoundReceptor(AxTraceParameters &parameters);
	virtual void OnTexture2D(AxTraceParameters &parameters);

	virtual void OnExitEntitySet(AxTraceParameters &parameters);
};

