//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTraceParameters.h"


AxTraceParameters::AxTraceParameters(void)
{
	this->meshRef = 0;
	this->cameraRef = 0;
	this->parentTransformRef = 0;
	this->transformRef = 0;
	this->renderTargetRef = 0;
	this->materialRef = 0;
	this->mechanismRef = 0;
	this->lightRef = 0;
	this->soundEmitterRef = 0;
	this->soundReceptorRef = 0;
	this->textureRef = 0;
	this->entitySetRef = 0;

	this->meshBranch = 0;
	this->cameraBranch = 0;
	this->transformBranch = 0;
	this->materialBranch = 0;
	this->lightBranch = 0;
	this->soundEmitterBranch = 0;
	this->soundReceptorBranch = 0;
	this->entitySetBranch = 0;

	this->meshIndex = -1;
	this->cameraIndex = -1;
	this->transformIndex = -1;
	this->materialIndex = -1;
	this->lightIndex = -1;
	this->soundEmitterIndex = -1;
	this->soundReceptorIndex = -1;
	this->entitySetIndex = -1;

	this->traceIndex = -1;
}

AxTraceParameters::AxTraceParameters(AxTraceParameters &parent)
{
	this->meshRef = parent.meshRef;
	this->cameraRef = parent.cameraRef;
	this->parentTransformRef = parent.transformRef;
	this->transformRef = parent.transformRef;
	this->renderTargetRef = parent.renderTargetRef;
	this->materialRef = parent.materialRef;
	this->mechanismRef = parent.mechanismRef;
	this->lightRef = parent.lightRef;
	this->soundEmitterRef = parent.soundEmitterRef;
	this->soundReceptorRef = parent.soundReceptorRef;
	this->textureRef = parent.textureRef;
	this->entitySetRef = parent.entitySetRef;

	this->meshBranch = parent.meshBranch;
	this->cameraBranch = parent.cameraBranch;
	this->transformBranch = parent.transformBranch;
	this->materialBranch = parent.materialBranch;
	this->lightBranch = parent.lightBranch;
	this->soundEmitterBranch = parent.soundEmitterBranch;
	this->soundReceptorBranch = parent.soundReceptorBranch;
	this->entitySetBranch = parent.entitySetBranch;

	this->meshIndex = parent.meshIndex;
	this->cameraIndex = parent.cameraIndex;
	this->transformIndex = parent.transformIndex;
	this->materialIndex = parent.materialIndex;
	this->lightIndex = parent.lightIndex;
	this->soundEmitterIndex = parent.soundEmitterIndex;
	this->soundReceptorIndex = parent.soundReceptorIndex;
	this->entitySetIndex = parent.entitySetIndex;

	this->traceIndex = parent.traceIndex;
}

AxTraceParameters::~AxTraceParameters(void)
{
}

