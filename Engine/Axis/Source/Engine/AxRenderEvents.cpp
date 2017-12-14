//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxRenderEvents.h"

#include "Axis.h"


AxRenderEvents::AxRenderEvents(Axis &Axis) 
	: AxTraceEvents(Axis)
{
}
	
AxRenderEvents::~AxRenderEvents(void)
{
}

void AxRenderEvents::SetupNewFrame()
{
	this->numRenderedObjects = 0;
	this->updateState = !this->updateState;
}

void AxRenderEvents::OnMaterial(AxTraceParameters &parameters)
{
	if (!parameters.materialRef->successfullyCompiled)
		return;

	this->context->graphicsDevice->SetShader(*parameters.materialRef->deviceShader);

	parameters.materialRef->deviceShader->SetParameter(parameters.materialRef->paramAddressAmbientLight, 0, parameters.materialRef->context->settings->properties[AxSettings::propertyIndex_AmbientLight]->GetEffectiveValue());

	parameters.materialRef->ApplyProperties();
}
    
   
void AxRenderEvents::OnMesh(AxTraceParameters &parameters)
{
	if ((parameters.materialRef == 0) || (parameters.cameraRef == 0) || (parameters.transformRef == 0))
		return;

	if (!parameters.materialRef->successfullyCompiled)
		return;

	parameters.meshRef->ApplyBlendChannels();

	AxMatrix transform;
	AxMatrix::Multiply(transform, parameters.transformRef->pivotedWorldMatrix, parameters.cameraRef->viewProjection);
	if (parameters.meshRef->properties[AxMesh::propertyIndex_Cull]->GetBool())
		if (!parameters.meshRef->bounds.InsideViewFrustum(transform))
	        return;

	parameters.materialRef->deviceShader->SetParameter(parameters.materialRef->paramAddressViewMatrix, 0, &parameters.cameraRef->view);
	parameters.materialRef->deviceShader->SetParameter(parameters.materialRef->paramAddressProjectionMatrix, 0, &parameters.cameraRef->projection);
	parameters.materialRef->deviceShader->SetParameter(parameters.materialRef->paramAddressViewPosition, 0, &parameters.cameraRef->position);
	
	if (parameters.materialRef->isAnimated)
		parameters.materialRef->ApplyBones();
	else
		parameters.materialRef->deviceShader->SetParameter(parameters.materialRef->paramAddressWorldMatrix, 0, &parameters.transformRef->pivotedWorldMatrix);

	AxVector3 lightsReferenceLocation;
	AxMatrix::GetTranslation(lightsReferenceLocation, parameters.transformRef->pivotedWorldMatrix);
	parameters.materialRef->ApplyLights(lightsReferenceLocation);


	// Moved to OnMaterial()
	this->context->graphicsDevice->SetShader(*parameters.materialRef->deviceShader);

	this->context->graphicsDevice->SetMesh(*parameters.meshRef->deviceMesh);

	this->context->graphicsDevice->RenderMesh();
        
	this->numRenderedObjects++;
}
    
void AxRenderEvents::OnMechanism(AxTraceParameters &parameters)
{
    //if (parameters.mechanismRef->_updateState != this->updateState)
    {
        parameters.mechanismRef->Process(this->context->timer->time);
        parameters.mechanismRef->_updateState = this->updateState;
    }
}

void AxRenderEvents::OnTransform(AxTraceParameters &parameters)
{
	//if (parameters.transformRef->_updateState != this->updateState)
    {
		parameters.transformRef->Process(parameters.parentTransformRef);
        parameters.transformRef->_updateState = this->updateState;
    }
}

void AxRenderEvents::OnLight(AxTraceParameters &parameters)
{
	if (parameters.transformRef == 0)
		return;

	this->context->activeLightsCollecting->SetSize(this->context->activeLightsCollecting->count + 1);
	AxActiveLightInfo *lightInfo = &this->context->activeLightsCollecting->Get(this->context->activeLightsCollecting->count - 1);

	lightInfo->lightRef = parameters.lightRef;
	lightInfo->lightTransform = parameters.transformRef->worldMatrix;
	AxMatrix::GetTranslation(lightInfo->lightPos, lightInfo->lightTransform);
}

void AxRenderEvents::OnSoundEmitter(AxTraceParameters &parameters)
{
	if (parameters.transformRef != 0)
		parameters.soundEmitterRef->ApplyWorldLocation(&parameters.transformRef->worldMatrix);

	parameters.soundEmitterRef->ApplyProperties();
}

void AxRenderEvents::OnSoundReceptor(AxTraceParameters &parameters)
{
	if (parameters.transformRef != 0)
		parameters.soundReceptorRef->ApplyWorldLocation(&parameters.transformRef->worldMatrix);
}

void AxRenderEvents::OnTexture2D(AxTraceParameters &parameters)
{
	parameters.textureRef->UpdateToProperties();

    this->context->graphicsDevice->SetRenderTargetTexture(parameters.textureRef->deviceTexture);

	this->context->currentRenderTarget = parameters.textureRef;

	if (this->context->clearOnRenderTarget)
	{
		this->context->graphicsDevice->ClearScreen(AxVector4());
		this->context->graphicsDevice->ClearDepthBuffer();
	}
}

void AxRenderEvents::OnExitEntitySet(AxTraceParameters &parameters)
{
	if (this->context->currentRenderTarget != parameters.textureRef)
	{
		if (parameters.textureRef == 0)
			this->context->graphicsDevice->SetRenderTargetWindow(this->context->windowHandle);
		else
			this->context->graphicsDevice->SetRenderTargetTexture(parameters.textureRef->deviceTexture);

		this->context->currentRenderTarget = parameters.textureRef;
	}
}
