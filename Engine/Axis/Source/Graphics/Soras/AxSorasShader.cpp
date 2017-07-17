//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSorasShader.h"

#include "AxSorasGraphicsDevice.h"
#include "AxSorasTexture2D.h"

AxSorasShader::AxSorasShader(void)
	: AxDeviceIndependentShader()
{
}


AxSorasShader::~AxSorasShader(void)
{
}

void AxSorasShader::AxToSrARGBInt(SrARGBInt &result, AxVector4 &source)
{
	result.r = (int)(source.x * 255.0f);
	result.g = (int)(source.y * 255.0f);
	result.b = (int)(source.z * 255.0f);
	result.a = (int)(source.w * 255.0f);
}

void AxSorasShader::AxToSrARGB(SrARGB &result, AxVector4 &source)
{
	result.r = (int)(source.x * 255.0f);
	result.g = (int)(source.y * 255.0f);
	result.b = (int)(source.z * 255.0f);
	result.a = (int)(source.w * 255.0f);
}

int AxSorasShader::GetParameterAddress(AxString parameterName)
{
	if (parameterName.Equals("Ambient"))
		return AxSorasShader::paramAddress_Ambient;
	else if (parameterName.Equals("Diffuse"))
		return AxSorasShader::paramAddress_Diffuse;
	else if (parameterName.Equals("Specular"))
		return AxSorasShader::paramAddress_Specular;
	else if (parameterName.Equals("Gloss"))
		return AxSorasShader::paramAddress_Gloss;
	else if (parameterName.Equals("Color map"))
		return AxSorasShader::paramAddress_ColorMap;
	else if (parameterName.Equals("Reflection map"))
		return AxSorasShader::paramAddress_ReflectionMap;
	else if (parameterName.Equals("Normal map"))
		return AxSorasShader::paramAddress_NormalMap;
	else if (parameterName.Equals("Refraction map"))
		return AxSorasShader::paramAddress_RefractionMap;
	else if (parameterName.Equals("Active lights count"))
		return AxSorasShader::paramAddress_ActiveLightsCount;
	else if (parameterName.Equals("View matrix"))
		return AxSorasShader::paramAddress_ViewMatrix;
	else if (parameterName.Equals("Projection matrix"))
		return AxSorasShader::paramAddress_ProjectionMatrix;
	else if (parameterName.Equals("Fog color"))
		return AxSorasShader::paramAddress_FogColor;
	else if (parameterName.Equals("Fog start"))
		return AxSorasShader::paramAddress_FogStart;
	else if (parameterName.Equals("Fog density"))
		return AxSorasShader::paramAddress_FogDensity;
	else if (parameterName.Equals("Texture Transform"))
		return AxSorasShader::paramAddress_TextureTransform;
	
	else if (parameterName.Equals("World matrix"))
		return AxSorasShader::paramAddress_WorldMatrix;
	else if (parameterName.Equals("Lights pos"))
		return AxSorasShader::paramAddress_LightsPos;
	else if (parameterName.Equals("Lights color"))
		return AxSorasShader::paramAddress_LightsColor;
	else if (parameterName.Equals("Lights intensity"))
		return AxSorasShader::paramAddress_LightsIntensity;
	else if (parameterName.Equals("Ambient light"))
		return AxSorasShader::paramAddress_AmbientLight;

	return AxDeviceShader::invalidParameterAddress;
}

void AxSorasShader::SetParameter(int address, int index, void *value)
{
	switch (address)
	{
		case AxDeviceShader::invalidParameterAddress:
		{
			break;
		}
		case AxSorasShader::paramAddress_Ambient:
		{
			this->AxToSrARGBInt(this->context->soras->material.ambient, *(AxVector4*)value);
			break;
		}
		case AxSorasShader::paramAddress_Diffuse:
		{
			this->AxToSrARGBInt(this->context->soras->material.diffuse, *(AxVector4*)value);
			break;
		}
		case AxSorasShader::paramAddress_Specular:
		{
			this->AxToSrARGBInt(this->context->soras->material.specular, *(AxVector4*)value);
			break;
		}
		case AxSorasShader::paramAddress_Gloss:
		{
			this->context->soras->material.gloss = *(float*)value;
			break;
		}
		case AxSorasShader::paramAddress_ColorMap:
		{
			if (this->colorMapIndex != -1)
				if (value != 0)
					this->pixelOpsTextures[this->colorMapIndex] = ((AxSorasTexture2D*)value)->sorasTexture;
			break;
		}
		case AxSorasShader::paramAddress_ReflectionMap:
		{
			if (this->envMapIndex != -1)
				if (value != 0)
					this->pixelOpsTextures[this->envMapIndex] = ((AxSorasTexture2D*)value)->sorasTexture;
			break;
		}
		case AxSorasShader::paramAddress_NormalMap:
		{
			if (this->normalMapIndex != -1)
				if (value != 0)
					this->pixelOpsTextures[this->normalMapIndex] = ((AxSorasTexture2D*)value)->sorasTexture;
			break;
		}
		case AxSorasShader::paramAddress_RefractionMap:
		{
			if (this->refractionMapIndex != -1)
				if (value != 0)
					this->pixelOpsTextures[this->refractionMapIndex] = ((AxSorasTexture2D*)value)->sorasTexture;
			break;
		}
		case AxSorasShader::paramAddress_ActiveLightsCount:
		{
			if (value != 0)
				this->context->soras->SetLighsCount(*(int*)value);
			break;
		}
		case AxSorasShader::paramAddress_ViewMatrix:
		{
			if (value != 0)
				this->viewMatrix = *(AxMatrix*)value;
			break;
		}
		case AxSorasShader::paramAddress_ProjectionMatrix:
		{
			if (value != 0)
				this->projectionMatrix = *(AxMatrix*)value;
			break;
		}

		case AxSorasShader::paramAddress_WorldMatrix:
		{
			if (value != 0)
			{
				if (index < this->context->soras->maxBones)
				{
					this->isAnimated |= index > 0;
					this->context->soras->bones[index] = *(SrMatrix*)((AxMatrix*)value);
					if (!this->isAnimated)
						this->context->soras->worldMatrix = *(SrMatrix*)((AxMatrix*)value);
				}
			}

			break;
		}

		case AxSorasShader::paramAddress_LightsPos:
		{
			if (value != 0)
			{
				if (index < this->context->soras->numLights)
				{
					this->context->soras->lights[index].position = *(SrVector3*)((AxVector3*)value);
				}
			}

			break;
		}

		case AxSorasShader::paramAddress_LightsColor:
		{
			if (value != 0)
			{
				if (index < this->context->soras->numLights)
				{
					this->AxToSrARGBInt(this->context->soras->lights[index].color, *(AxVector4*)value);
				}
			}

			break;
		}
		
		case AxSorasShader::paramAddress_LightsIntensity:
		{
			if (value != 0)
			{
				if (index < this->context->soras->numLights)
				{
					this->context->soras->lights[index].range = *(float*)value;
				}
			}

			break;
		}

		case AxSorasShader::paramAddress_AmbientLight:
		{
			if (value != 0)
			{
				this->AxToSrARGBInt(this->context->soras->shadingParameters.globalAmbient, *(AxVector4*)value);
			}

			break;
		}

		case AxSorasShader::paramAddress_FogColor:
		{
			if (value != 0)
			{
				this->AxToSrARGB(this->context->soras->shadingParameters.fogColor, *(AxVector4*)value);
			}

			break;
		}

		case AxSorasShader::paramAddress_FogStart:
		{
			if (value != 0)
			{
				this->context->soras->shadingParameters.fogStart = (int)(*(float*)value);
				this->context->soras->shadingParameters.fogEnd = this->context->soras->shadingParameters.fogStart + (int)this->fogDensity;
			}

			break;
		}

		case AxSorasShader::paramAddress_FogDensity:
		{
			if (value != 0)
			{
				this->fogDensity = *(float*)value;
				this->context->soras->shadingParameters.fogEnd = this->context->soras->shadingParameters.fogStart + (int)this->fogDensity;
			}

			break;
		}

		case AxSorasShader::paramAddress_TextureTransform:
		{
			if (value != 0)
			{
				AxMatrix *transform = (AxMatrix*)value;
				this->context->soras->shadingParameters.texTransformVector.x = SrUnit::FromFloat(transform->_41);
				this->context->soras->shadingParameters.texTransformVector.y = SrUnit::FromFloat(transform->_42);
			}

			break;
		}

	}
}

bool AxSorasShader::Update(AxString &compilationMessage)
{
	this->isAnimated = false;

	this->vertexShaderOps.Clear();
	this->vertexColorOps.Clear();
	this->pixelShaderOps.Clear();
	this->pixelColorOps.Clear();
	this->pixelOpsTextures.Clear();

	this->colorMapIndex = -1;
	this->envMapIndex = -1;
	this->normalMapIndex = -1;
	this->refractionMapIndex = -1;

	for (int i = 0; i < this->shadingOps.count; i++)
	{
		AxColorOperation colorOp = this->shadingOps[i].colorOp;

		SrShaderOperation srColorOp = 0;
		switch (colorOp)
		{
			case AxColorOperation_None:
			{
				srColorOp = 0;
				break;
			}

			case AxColorOperation_Set:
			{
				srColorOp = SrShaderOperations::Color_Set;
				break;
			}

			case AxColorOperation_Add:
			case AxColorOperation_Blend:
			{
				srColorOp = SrShaderOperations::Color_Add;
				break;
			}

			case AxColorOperation_Subtract:
			{
				srColorOp = SrShaderOperations::Color_Subtract;
				break;
			}

			case AxColorOperation_Modulate:
			{
				srColorOp = SrShaderOperations::Color_Modulate;
				break;
			}

			case AxColorOperation_Lighter:
			{
				srColorOp = SrShaderOperations::Color_Lighter;
				break;
			}

			case AxColorOperation_Darker:
			{
				srColorOp = SrShaderOperations::Color_Darker;
				break;
			}

			case AxColorOperation_AlphaBlend:
			{
				srColorOp = SrShaderOperations::Color_AlphaBlend;
				break;
			}

			case AxColorOperation_AlphaAdd:
			{
				srColorOp = SrShaderOperations::Color_AlphaAdd;
				break;
			}

			case AxColorOperation_AlphaSubtract:
			{
				srColorOp = SrShaderOperations::Color_AlphaSubtract;
				break;
			}

			case AxColorOperation_AlphaModulate:
			{
				srColorOp = SrShaderOperations::Color_AlphaModulate;
				break;
			}
		}

		AxShadingOperation shadingOp = this->shadingOps[i].shadingOp;

		switch (shadingOp)
		{
			case AxShadingOperation_BasicShading:
			{
				this->vertexShaderOps.Add(SrShaderOperations::BasicLighting);
				this->vertexColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_LightSource:
			{
				break;
			}

			case AxShadingOperation_VertexLighting:
			{
				this->vertexShaderOps.Add(SrShaderOperations::Lighting);
				this->vertexColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_PixelLighting:
			{
				this->pixelShaderOps.Add(SrShaderOperations::Lighting);
				this->pixelColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_ColorMap:
			{
				if (this->pixelShaderOps.IndexOf(SrShaderOperations::ColorMap) != -1)
					break;

				this->colorMapIndex = this->pixelShaderOps.count;
				this->pixelShaderOps.Add(SrShaderOperations::ColorMap);
				this->pixelColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_ReflectionMap:
			{
				if (this->pixelShaderOps.IndexOf(SrShaderOperations::EnvMap) != -1)
					break;

				this->envMapIndex = this->pixelShaderOps.count;
				this->pixelShaderOps.Add(SrShaderOperations::EnvMap);
				this->pixelColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_NormalMap:
			{
				if (this->pixelShaderOps.IndexOf(SrShaderOperations::NormalMap) != -1)
					break;

				this->normalMapIndex = this->pixelShaderOps.count;
				this->pixelShaderOps.Add(SrShaderOperations::NormalMap);
				this->pixelColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_RefractionMap:
			{
				if (this->pixelShaderOps.IndexOf(SrShaderOperations::RefractionMap) != -1)
					break;

				this->refractionMapIndex = this->pixelShaderOps.count;
				this->pixelShaderOps.Add(SrShaderOperations::RefractionMap);
				this->pixelColorOps.Add(srColorOp);
				break;
			}

			case AxShadingOperation_Fog:
			{
				this->pixelShaderOps.Add(SrShaderOperations::Fog);
				this->pixelColorOps.Add(srColorOp);
				
				break;
			}

			case AxShadingOperation_TextureTransform:
			{
				this->pixelShaderOps.Add(SrShaderOperations::TransformTexCoord);
				this->pixelColorOps.Add(0);
				
				break;
			}

		}
	}

	this->pixelOpsTextures.SetSize(this->pixelShaderOps.count);
	for (int i = 0; i < this->pixelOpsTextures.count; i++)
		this->pixelOpsTextures[i] = 0;

	return true;
}

