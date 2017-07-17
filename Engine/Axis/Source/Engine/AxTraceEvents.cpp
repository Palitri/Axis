//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTraceEvents.h"


AxTraceEvents::AxTraceEvents(Axis &context)
{
	this->context = &context;
}

AxTraceEvents::~AxTraceEvents(void)
{
}

// Virtual Methods
void AxTraceEvents::OnMesh(AxTraceParameters &parameters) { }
void AxTraceEvents::OnCamera(AxTraceParameters &parameters) { }
void AxTraceEvents::OnTransform(AxTraceParameters &parameters) { }
void AxTraceEvents::OnRenderTarget(AxTraceParameters &parameters) { }
void AxTraceEvents::OnMaterial(AxTraceParameters &parameters) { }
void AxTraceEvents::OnMechanism(AxTraceParameters &parameters) { }
void AxTraceEvents::OnLight(AxTraceParameters &parameters) { }
void AxTraceEvents::OnSoundEmitter(AxTraceParameters &parameters) { }
void AxTraceEvents::OnSoundReceptor(AxTraceParameters &parameters) { }
void AxTraceEvents::OnTexture2D(AxTraceParameters &parameters) { }

void AxTraceEvents::OnExitEntitySet(AxTraceParameters &parameters) { }
