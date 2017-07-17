//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMaterial.h"

#include "..\..\..\Tools\AxMath.h"

#include "..\..\Axis.h"

AxMaterial::AxMaterial(Axis *context)
	: AxResource()
{
	this->context = context;

	this->typeId = AxMaterial::classId;
	this->resourceType = AxResourceType_Material;

	//this->properties.Add(new AxProperty(AxString("Override Materials"), false));
	this->properties.Add(new AxProperty(AxString("Override Materials"), false));

	this->deviceShader = this->context->graphicsDevice->CreateShader();

	this->paramAddressWorldMatrix = AxDeviceShader::invalidParameterAddress;
	this->paramAddressViewMatrix = AxDeviceShader::invalidParameterAddress;
	this->paramAddressProjectionMatrix = AxDeviceShader::invalidParameterAddress;
	this->paramAddressViewPosition = AxDeviceShader::invalidParameterAddress;
	this->paramAddressLightsPos = AxDeviceShader::invalidParameterAddress;
	this->paramAddressLightsColor = AxDeviceShader::invalidParameterAddress;
	this->paramAddressLightsIntensity = AxDeviceShader::invalidParameterAddress;
	this->paramAddressActiveLightsCount = AxDeviceShader::invalidParameterAddress;
	this->paramAddressAmbientLight = AxDeviceShader::invalidParameterAddress;

    this->CommitRenderStates();
}

AxMaterial::~AxMaterial(void)
{
	delete this->deviceShader;
}


AxString AxMaterial::CreatePropertyName(const AxString &desiredName, int propertyNumber)
{
	if (propertyNumber == 0)
		return AxString(desiredName.contents);

	AxString result = AxString(desiredName.contents) + " " + AxString(propertyNumber);
	return AxString(result);
}

AxString AxMaterial::CreateUniquePropertyName(const AxString &desiredName)
{
	AxString result = desiredName.contents;
	int propertyNumber = 0;
	
	while (this->GetProperty(result) != 0)
	{
		propertyNumber++;
		result = AxMaterial::CreatePropertyName(desiredName, propertyNumber);
	}

	return AxString(result);
}

bool AxMaterial::InsertColorOperationProperty(int index, const AxString &name, AxColorOperation colorOp)
{
	switch (colorOp)
	{
		case AxColorOperation_None:
		{
			break;
		}

		case AxColorOperation_Set:
		case AxColorOperation_Blend:
		case AxColorOperation_Add:
		case AxColorOperation_Subtract:
		case AxColorOperation_Modulate:
		case AxColorOperation_Lighter:
		case AxColorOperation_Darker:
		case AxColorOperation_AlphaBlend:
		case AxColorOperation_AlphaAdd:
		case AxColorOperation_AlphaSubtract:
		case AxColorOperation_AlphaModulate:
		{
			this->properties.Insert(index, new AxProperty(name, 1.0f));
			return true;
		}
	}

	return false;
}

void AxMaterial::CommitRenderStates()
{
	int numRenderStates = this->renderStates.count;
	for (int i = 0; i < numRenderStates; i++)
			this->context->graphicsDevice->SetRenderState(this->renderStates[i]);
}


void AxMaterial::InsertShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp, int index)
{
	int numProperties = this->properties.count;

	int propertyIndex = index < this->shadingLayers.count ? this->shadingLayers[index].propertyFirst : numProperties;
	int propertyFirst = propertyIndex;

	AxTexture2D *defaultTexture = 0;

	int countRecurringOps = this->GetShadingLayersCount(shadingOp);

	switch (shadingOp)
	{
		case AxShadingOperation_BasicShading:
			{
				break;
			}

		case AxShadingOperation_FlatColor:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Flat color", countRecurringOps), AxVector4(1.0f, 1.0f, 1.0f, 1.0f)));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Flat color amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_Tint:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Tint color", countRecurringOps), AxVector4(1.0f, 1.0f, 1.0f, 1.0f)));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Tint amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_AmbientLighting:
			{
				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Ambient amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_LightSource:
			{
				break;
			}

		case AxShadingOperation_VertexLighting:
			{
				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Vertex lighting amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_PixelLighting:
			{
				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Pixel lighting amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_ColorMap:
			{
				//this->properties.Insert(propertyIndex++, new AxBoundProperty(AxMaterial::CreatePropertyName("Reflection map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				//this->properties.Insert(propertyIndex++, new AxBoundProperty(AxMaterial::CreatePropertyName("Test Color map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Color map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Color map amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_ReflectionMap:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Reflection map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Reflection map factor", countRecurringOps), 1.0f));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Reflection map amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}


		case AxShadingOperation_RefractionMap:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Refraction map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Refraction map factor", countRecurringOps), 0.5f));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Refraction map amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_NormalMap:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Normal map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Normal map factor", countRecurringOps), 1.0f));

				break;
			}

		case AxShadingOperation_BlurMap:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Blur map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Blur map factor", countRecurringOps), 0.0f));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Blur map amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_HBlur:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("HBlur map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("HBlur map factor", countRecurringOps), 0.0f));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("HBlur map amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_VBlur:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("VBlur map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("VBlur map factor", countRecurringOps), 0.0f));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("VBlur map amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_AlignScreenQuad:
			{
				break;
			}

		case AxShadingOperation_ProjectTexture:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Projection Map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Projection Cam", countRecurringOps), 0, AxParameterType_ReferenceCamera));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Project texture amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_ProjectPicture:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Picture Map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Picture Cam", countRecurringOps), 0, AxParameterType_ReferenceCamera));

				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Project picture amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		case AxShadingOperation_ShadowMapWriter:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Shadow map depth", countRecurringOps), 0.0f));

				break;
			}

		case AxShadingOperation_ShadowMap:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("ShadowMap Cam", countRecurringOps), 0, AxParameterType_ReferenceCamera));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Shadow Map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Light Map", countRecurringOps), defaultTexture, AxParameterType_ReferenceTexture));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Bias", countRecurringOps), 0.0f));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Shadow map depth", countRecurringOps), 0.0f));

				break;
			}

		case AxShadingOperation_TextureTransform:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Texture Transform", countRecurringOps), 0, AxParameterType_ReferenceTransform));

				break;
			}

		// TODO: Consider removing this operation. Currently, it only provides the properties without actually doing anything
		case AxShadingOperation_Material:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Ambient", countRecurringOps), AxVector4(0.2f, 0.2f, 0.2f, 1.0f)));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Diffuse", countRecurringOps), AxVector4(0.5f, 0.5f, 0.5f, 1.0f)));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Specular", countRecurringOps), AxVector4(1.0f, 1.0f, 1.0f, 1.0f)));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Gloss", countRecurringOps), 10.0f));

				break;
			}

		case AxShadingOperation_Fog:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Fog color", countRecurringOps), AxVector4(0.8f, 0.8f, 0.8f, 1.0f)));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Fog start", countRecurringOps), 100.0f));
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("Fog density", countRecurringOps), 100.0f));
	
				if (this->InsertColorOperationProperty(propertyIndex, AxMaterial::CreatePropertyName("Fog amount", countRecurringOps), colorOp))
					propertyIndex++;

				break;
			}

		// TODO: consider moving this to a "Parameters" resource entity, dedicated to changing various parameters of the engine alont traversing the scene
		case AxShadingOperation_ChangeCulling:
			{
				this->properties.Insert(propertyIndex++, new AxProperty(AxMaterial::CreatePropertyName("No culling", countRecurringOps), false));
				
				break;
			}
	}

	int numNewProperties = this->properties.count - numProperties;
	this->shadingLayers.Insert(index, AxMaterialShadingLayer(shadingOp, colorOp, propertyFirst, numNewProperties));

	int count = this->shadingLayers.count;
	for (int i = index + 1; i < count; i++)
		this->shadingLayers[i].propertyFirst += numNewProperties;
}

int AxMaterial::AddShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp)
{
	this->InsertShadingLayer(shadingOp, colorOp, shadingLayers.count);
	return this->shadingLayers.count - 1;
}

void AxMaterial::RemoveShadingLayer(int index)
{
	int count = this->shadingLayers.count;
	for (int i = index + 1; i < count; i++)
		this->shadingLayers[i].propertyFirst -= this->shadingLayers[index].propertyCount;

	int propertyFirst = this->shadingLayers[index].propertyFirst;
	int propertyCount = this->shadingLayers[index].propertyCount;
	for (int i = 0; i < propertyCount; i++)
		delete this->properties[propertyFirst + i];

	this->properties.RemoveRange(propertyFirst, propertyCount);
	this->shadingLayers.RemoveAt(index);
}

void AxMaterial::MoveShadingLayer(int originalIndex, int newIndex)
{
	int originalIndexPropertiesStart = this->shadingLayers[originalIndex].propertyFirst;
	int originalIndexPropertiesCount = this->shadingLayers[originalIndex].propertyCount;

	int start = AxMath::Min(originalIndex, newIndex);
	int propertyFirst = this->shadingLayers[start].propertyFirst;

	this->shadingLayers.Move(originalIndex, newIndex, 1);

	int count = this->shadingLayers.count;
	for (int i = start; i < count; i++)
	{
		this->shadingLayers[i].propertyFirst = propertyFirst;
		propertyFirst += this->shadingLayers[i].propertyCount;
	}

	this->properties.Move(originalIndexPropertiesStart, this->shadingLayers[newIndex].propertyFirst, originalIndexPropertiesCount);
}

void AxMaterial::ClearShadingLayers()
{
	for (int i = AxMaterial::propertyIndex_OverrideMaterials + 1; i < this->properties.count; i++)
		delete this->properties[i];

	this->properties.Clear();
	
	this->shadingLayers.Clear();
}

int AxMaterial::GetShadingLayersCount(AxShadingOperation shadingOp)
{
	int result = 0;
	for (int i = 0; i < this->shadingLayers.count; i++)
		if (this->shadingLayers.Get(i).shadingOp == shadingOp)
			result++;

	return result;
}

int AxMaterial::GetShadingLayersCount(AxShadingOperation shadingOp, AxColorOperation colorOp)
{
	int result = 0;
	for (int i = 0; i < this->shadingLayers.count; i++)
	{
		AxMaterialShadingLayer shadingLayer = this->shadingLayers.Get(i);
		if ((shadingLayer.shadingOp == shadingOp) && (shadingLayer.colorOp == colorOp))
			result++;
	}

	return result;
}


bool AxMaterial::UpdateShader()
{
	this->successfullyCompiled = false;

	// Setup material
	this->deviceShader->ClearOperations();
	for (int i = 0; i < this->shadingLayers.count; i++)
		this->deviceShader->AddOperation(this->shadingLayers[i].shadingOp, this->shadingLayers[i].colorOp);
	
	// Update shader
	if (!this->deviceShader->Update(this->compilationMessage))
		return false;

	// Setup parameters
	this->shadingPropertiesAddresses.SetSize(this->properties.count);
	for (int i = 0; i < this->properties.count; i++)
	{
		this->shadingPropertiesAddresses[i] = this->deviceShader->GetParameterAddress(this->properties[i]->name);
	}

	this->noCullingPropertyIndex = this->properties.IndexOf(this->GetProperty("No culling"));
		
	this->paramAddressWorldMatrix = this->deviceShader->GetParameterAddress("World matrix");
	this->paramAddressViewMatrix = this->deviceShader->GetParameterAddress("View matrix");
	this->paramAddressProjectionMatrix = this->deviceShader->GetParameterAddress("Projection matrix");
	this->paramAddressViewPosition = this->deviceShader->GetParameterAddress("View position");
	this->paramAddressLightsPos = this->deviceShader->GetParameterAddress("Lights pos");
	this->paramAddressLightsColor = this->deviceShader->GetParameterAddress("Lights color");
	this->paramAddressLightsIntensity = this->deviceShader->GetParameterAddress("Lights intensity");
	this->paramAddressActiveLightsCount = this->deviceShader->GetParameterAddress("Active lights count");
	this->paramAddressAmbientLight = this->deviceShader->GetParameterAddress("Ambient light");

	// Get some common parameters
	this->isAnimated = false;
	this->lightSourcesCount = 0;
	for (int i = 0; i < this->shadingLayers.count; i++)
	{
		if (this->shadingLayers[i].shadingOp == AxShadingOperation_LightSource)
			this->lightSourcesCount++;
		if (this->shadingLayers[i].shadingOp == AxShadingOperation_Skinning)
			this->isAnimated = true;
	}

	this->successfullyCompiled = true;
	return true;
}

void AxMaterial::ApplyProperty(AxProperty *materialProperty, int shaderAddress)
{
	if (!this->successfullyCompiled)
		return;

	switch (materialProperty->type)
	{
		case AxParameterType_ReferenceTexture:
		{
			AxTexture2D *texture = (AxTexture2D*)materialProperty->GetEffectiveValue();
			if (texture != 0)
				this->deviceShader->SetParameter(shaderAddress, 0, texture->deviceTexture);
			break;
		}

		case AxParameterType_ReferenceCamera:
		{
			AxCamera *camera = (AxCamera*)materialProperty->GetEffectiveValue();
			if (camera != 0)
				this->deviceShader->SetParameter(shaderAddress, 0, &camera->viewProjection);
			break;
		}

		case AxParameterType_ReferenceTransform:
		{
			AxTransform *transform = (AxTransform*)materialProperty->GetEffectiveValue();
			if (transform != 0)
				this->deviceShader->SetParameter(shaderAddress, 0, &transform->worldMatrix);
			break;
		}

		case AxParameterType_ReferenceVector3:
		{
			AxTransform *transform = (AxTransform*)materialProperty->GetEffectiveValue();
			if (transform != 0)
			{
				this->deviceShader->SetParameter(shaderAddress, 0, &transform->worldMatrix._41);
			}
			break;
		}

		default:
		{
			this->deviceShader->SetParameter(shaderAddress, 0, materialProperty->GetEffectiveValue());
			break;
		}
	}
}

void AxMaterial::ApplyProperties()
{
	if (!this->successfullyCompiled)
		return;

	if (this->properties.count != this->shadingPropertiesAddresses.count)
		return;

	int count = this->properties.count;
	for (int i = 0; i < count; i++)
		this->ApplyProperty(this->properties[i], this->shadingPropertiesAddresses[i]);

	if (this->noCullingPropertyIndex != -1)
		if (this->properties[this->noCullingPropertyIndex]->GetBool())
			this->context->graphicsDevice->SetRenderState(AxRenderState_CullNone);
		else
			this->context->graphicsDevice->SetRenderState(AxRenderState_CullFrontFace);
	else
		this->context->graphicsDevice->SetRenderState(AxRenderState_CullBackFace);
}

void AxMaterial::ApplyLights(AxVector3 &location)
{
	if (this->lightSourcesCount > 0)
	{
		this->context->CalculateNearestActiveLights(this->lightSourcesCount, location);
		int lightsCount = this->context->nearestActiveLights.count;
		this->deviceShader->SetParameter(this->paramAddressActiveLightsCount, 0, &lightsCount);
		if (lightsCount > 0)
		{
			if ((this->paramAddressLightsPos != AxDeviceShader::invalidParameterAddress) && (this->paramAddressLightsColor != AxDeviceShader::invalidParameterAddress) && (this->paramAddressLightsIntensity != AxDeviceShader::invalidParameterAddress))
			{
				for (int i = 0; i < lightsCount; i++)
				{
					this->deviceShader->SetParameter(this->paramAddressLightsPos, i, &this->context->nearestActiveLights[i]->lightPos);
					this->deviceShader->SetParameter(this->paramAddressLightsColor, i , this->context->nearestActiveLights[i]->lightRef->properties[AxLight::propertyIndex_Color]->GetEffectiveValue());
					this->deviceShader->SetParameter(this->paramAddressLightsIntensity, i , this->context->nearestActiveLights[i]->lightRef->properties[AxLight::propertyIndex_Intensity]->GetEffectiveValue());
				}
			}
		}
	}
	else
		this->deviceShader->SetParameter(this->paramAddressActiveLightsCount, 0, &this->lightSourcesCount);
}

void AxMaterial::ApplyBones()
{
	// TODO: addresses of parameters should be precalculated, or GetParameterAddress should accept not a parameter name, but an id, so it can map fast and directly to the address
	if (this->isAnimated)
	{
		if (this->paramAddressWorldMatrix != AxDeviceShader::invalidParameterAddress)
		{
			for (int i = 0; i < this->context->bones.count; i++)
			{
				this->deviceShader->SetParameter(this->paramAddressWorldMatrix ,i, &this->context->bones[i]);
			}
		}
	}
}


bool AxMaterial::Deserialize(AxStream &source)
{
	if (!AxResource::Deserialize(source))
		return false;

	return this->UpdateShader();
}

void AxMaterial::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	writer.BeginChunk(AxMaterial::SerializationId_ShadingLayers);

	for (int i = 0; i < this->shadingLayers.count; i++)
	{
		writer.BeginChunk(AxMaterial::SerializationId_ShadingLayer);
		writer.stream->WriteInt16(this->shadingLayers[i].shadingOp);
		writer.stream->WriteInt16(this->shadingLayers[i].colorOp);
		writer.EndChunk();
	}

	writer.EndChunk();

	writer.BeginChunk(AxMaterial::SerializationId_RenderStates);

	for (int i = 0; i < this->shadingLayers.count; i++)
	{
		writer.BeginChunk(AxMaterial::SerializationId_RenderState);
		writer.stream->WriteInt16(this->renderStates[i]);
		writer.EndChunk();
	}

	writer.EndChunk();

	// Make base serialization at end, because shader data creates properties, which need to be filled afterwards
	AxResource::SerializeChunks(writer);
}

bool AxMaterial::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxMaterial::SerializationId_ShadingLayers:
		{
			break;
		}

		case AxMaterial::SerializationId_ShadingLayer:
		{
			AxShadingOperation shadingOp = (AxShadingOperation)(reader.stream->ReadInt16());
			AxColorOperation colorOp = (AxColorOperation)(reader.stream->ReadInt16());
			this->AddShadingLayer(shadingOp, colorOp);
			break;
		}

		case AxMaterial::SerializationId_RenderStates:
		{
			break;
		}

		case AxMaterial::SerializationId_RenderState:
		{
			this->renderStates.Add((AxRenderState)reader.stream->ReadInt16());
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
