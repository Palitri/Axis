//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "SrShaderOperations.h"

#include "SrUnit.h"

#include "..\SrRenderThread.h"
#include "..\Soras.h"

#include "..\..\..\..\Tools\AxMath.h"

void SrShaderOperations::Lighting(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	*reg_ColorArg = renderThread->soras->materialAmbient;
	//reg_ColorArg->a = 0x00;
 //   reg_ColorArg->r = 0x00;
 //   reg_ColorArg->g = 0x00;
 //   reg_ColorArg->b = 0x00;

	SrMaterial *material = &renderThread->soras->material;
	SrVector3 lightVector;
	for (int i = 0; i < renderThread->soras->numLights; i++)
    {
        SrLight light = renderThread->soras->lights[i];

		lightVector.x = light.position.x - reg_Point->position.x;
        lightVector.y = light.position.y - reg_Point->position.y;
        lightVector.z = light.position.z - reg_Point->position.z;

        float dp = reg_Point->normal.x * lightVector.x +
                   reg_Point->normal.y * lightVector.y +
                   reg_Point->normal.z * lightVector.z;

        if (dp > 0.0f)
        {
            float distance = AxMath::Sqrt(lightVector.x * lightVector.x +
                                          lightVector.y * lightVector.y +
                                          lightVector.z * lightVector.z);
            if (distance != 0.0f)
            {
                dp /= distance;

				float specular = (float)AxMath::Power(dp, material->gloss);
                int intensity = (int)(specular * 0xff);
                reg_ColorArg->r += (light.color.r * material->specular.r * intensity) >> 16;
                reg_ColorArg->g += (light.color.g * material->specular.g * intensity) >> 16;
                reg_ColorArg->b += (light.color.b * material->specular.b * intensity) >> 16;

                if (distance < light.range)
                {
                    intensity = (int)((1.0f - distance / light.range) * dp * 0xff);
                    //intensity = (int)(dp * 0xff);
                    reg_ColorArg->r += (light.color.r * material->diffuse.r * intensity) >> 16;
                    reg_ColorArg->g += (light.color.g * material->diffuse.g * intensity) >> 16;
                    reg_ColorArg->b += (light.color.b * material->diffuse.b * intensity) >> 16;
                }
            }
        }
    }
}
      
void SrShaderOperations::BasicLighting(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_ColorArg->a = 0xff;
	Soras *soras = renderThread->soras;

    if (reg_Point->normal.z < 0.0f)
    {
		int intensity = (int)((-reg_Point->normal.z + AxMath::Power(-reg_Point->normal.z, soras->material.gloss)) * 0xff);
        reg_ColorArg->r = (soras->shadingParameters.basicLightColor.r * soras->material.diffuse.r * intensity) >> 16;
        reg_ColorArg->g = (soras->shadingParameters.basicLightColor.g * soras->material.diffuse.g * intensity) >> 16;
        reg_ColorArg->b = (soras->shadingParameters.basicLightColor.b * soras->material.diffuse.b * intensity) >> 16;
    }
    else
    {
        reg_ColorArg->r = 0x00;
        reg_ColorArg->g = 0x00;
        reg_ColorArg->b = 0x00;
    }
}

void SrShaderOperations::CellLighting(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_ColorArg->a = 0x00;
    reg_ColorArg->r = 0x00;
    reg_ColorArg->g = 0x00;
    reg_ColorArg->b = 0x00;


	Soras *soras = renderThread->soras;
    
	SrVector3 lightVector;
    for (int i = 0; i < soras->numLights; i++)
    {
        SrLight light = soras->lights[i];

		lightVector.x = light.position.x - reg_Point->position.x;
        lightVector.y = light.position.y - reg_Point->position.y;
        lightVector.z = light.position.z - reg_Point->position.z;

        float dp = reg_Point->normal.x * lightVector.x +
                    reg_Point->normal.y * lightVector.y +
                    reg_Point->normal.z * lightVector.z;

        if (dp > 0.0f)
        {
            float distance = AxMath::Sqrt(lightVector.x * lightVector.x +
                                            lightVector.y * lightVector.y +
                                            lightVector.z * lightVector.z);
            if (distance != 0.0f)
            {
                dp /= distance;

				float specular = AxMath::Power(dp, soras->material.gloss);
                int intensity = (int)(specular * 0xff);
                intensity = (intensity / soras->shadingParameters.cellSpecularSegmentation) * soras->shadingParameters.cellSpecularSegmentation;
                reg_ColorArg->r += (light.color.r * soras->material.specular.r * intensity) >> 16;
                reg_ColorArg->g += (light.color.g * soras->material.specular.g * intensity) >> 16;
                reg_ColorArg->b += (light.color.b * soras->material.specular.b * intensity) >> 16;

                if (distance < light.range)
                {
                    intensity = (int)((1.0f - distance / light.range) * dp * 0xff);
                    //intensity = (int)(dp * 0xff);
                    intensity = (intensity / soras->shadingParameters.cellDiffuseSegmentation) * soras->shadingParameters.cellDiffuseSegmentation;
                    reg_ColorArg->r += (light.color.r * soras->material.diffuse.r * intensity) >> 16;
                    reg_ColorArg->g += (light.color.g * soras->material.diffuse.g * intensity) >> 16;
                    reg_ColorArg->b += (light.color.b * soras->material.diffuse.b * intensity) >> 16;
                }
            }
        }
    }
}

void SrShaderOperations::CellBasicLighting(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_ColorArg->a = 0xff;

	Soras *soras = renderThread->soras;
    if (reg_Point->normal.z < 0.0f)
    {
        int intensity = (int)((-reg_Point->normal.z + AxMath::Power(-reg_Point->normal.z, soras->material.gloss)) * 0xff);
        intensity = (intensity / soras->shadingParameters.cellDiffuseSegmentation) * soras->shadingParameters.cellDiffuseSegmentation;
        reg_ColorArg->r = (soras->shadingParameters.basicLightColor.r * soras->material.diffuse.r * intensity) >> 16;
        reg_ColorArg->g = (soras->shadingParameters.basicLightColor.g * soras->material.diffuse.g * intensity) >> 16;
        reg_ColorArg->b = (soras->shadingParameters.basicLightColor.b * soras->material.diffuse.b * intensity) >> 16;
    }
    else
    {
        reg_ColorArg->r = 0x00;
        reg_ColorArg->g = 0x00;
        reg_ColorArg->b = 0x00;
    }
}

void SrShaderOperations::Fog(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    int fogRange = soras->shadingParameters.fogEnd - soras->shadingParameters.fogStart;
    float z = ((float)(1 << 18) + reg_Point->positionIntScreen.z) / (float)(1 << 4) - soras->shadingParameters.fogStart;
    reg_ColorArg->a = (0xff * AxMath::Min((int)z, fogRange)) / fogRange;
    reg_ColorArg->r = soras->shadingParameters.fogColor.r;
    reg_ColorArg->g = soras->shadingParameters.fogColor.g;
    reg_ColorArg->b = soras->shadingParameters.fogColor.b;
}

void SrShaderOperations::NormalOut(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_ColorArg->r = (int)((reg_Point->normal.x + 1.0) * 0x80);
    reg_ColorArg->g = (int)((reg_Point->normal.y + 1.0) * 0x80);
    reg_ColorArg->b = (int)((reg_Point->normal.z + 1.0) * 0x80);
}

void SrShaderOperations::VectorNoise(SrRenderThread *renderThread)
{
 	SrVertex *reg_Point = renderThread->shaderReg_Point;

	reg_Point->position.x += AxMath::Random(5) - 2;
    reg_Point->position.y += AxMath::Random(5) - 2;
    reg_Point->position.z += AxMath::Random(5) - 2;
}

void SrShaderOperations::Noise(SrRenderThread *renderThread)
{
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_ColorArg->r = AxMath::Random(0x80);
    reg_ColorArg->g = reg_ColorArg->r;
    reg_ColorArg->b = reg_ColorArg->r;
}

void SrShaderOperations::Grayscale(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	int channel = (reg_Point->color.r + reg_Point->color.g + reg_Point->color.b) / 3;
    reg_ColorArg->r = channel;
    reg_ColorArg->g = channel;
    reg_ColorArg->b = channel;
    reg_ColorArg->a = 0xff;
}

void SrShaderOperations::Tint(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    int channel = (reg_Point->color.r + reg_Point->color.g + reg_Point->color.b) / 3;
    reg_ColorArg->r = (channel * soras->shadingParameters.tintColor.r) >> 8;
    reg_ColorArg->g = (channel * soras->shadingParameters.tintColor.g) >> 8;
    reg_ColorArg->b = (channel * soras->shadingParameters.tintColor.b) >> 8;
    reg_ColorArg->a = soras->shadingParameters.tintColor.a;
}

void SrShaderOperations::ColorMap(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    renderThread->shaderReg_Texture = soras->shaderTextures[renderThread->shaderTextureIndex++];
    soras->textureSampler(renderThread, reg_Point->texCoords.x, reg_Point->texCoords.y, renderThread->shaderReg_ColorArg);
}

void SrShaderOperations::EnvMap(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    renderThread->shaderReg_Texture = soras->shaderTextures[renderThread->shaderTextureIndex++];
    soras->textureSampler(renderThread, 
						 (SrUnit::high - (int)(reg_Point->normal.x * SrUnit::high)) >> 1,
						 (SrUnit::high - (int)(reg_Point->normal.y * SrUnit::high)) >> 1,
						 renderThread->shaderReg_ColorArg);
}

void SrShaderOperations::CellEnvMap(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    renderThread->shaderReg_Texture = soras->shaderTextures[renderThread->shaderTextureIndex++];
    int tu = (SrUnit::high - (int)(reg_Point->normal.x * SrUnit::high)) >> 1;
    int tv = (SrUnit::high - (int)(reg_Point->normal.y * SrUnit::high)) >> 1;
    tu = (tu / soras->shadingParameters.cellEnvMapSegmentation) * soras->shadingParameters.cellEnvMapSegmentation;
    tv = (tv / soras->shadingParameters.cellEnvMapSegmentation) * soras->shadingParameters.cellEnvMapSegmentation;
    soras->textureSampler(renderThread, tu, tv, renderThread->shaderReg_ColorArg);
}

void SrShaderOperations::NormalMap(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;

	renderThread->shaderReg_Texture = soras->shaderTextures[renderThread->shaderTextureIndex++];

    SrARGBInt sample;
    soras->textureSampler(renderThread, reg_Point->texCoords.x, reg_Point->texCoords.y, sample);

    float k = 0.5f / (float)0x80;
    reg_Point->normal.x += (sample.r - 0x80) * k;
    reg_Point->normal.y += (sample.g - 0x80) * k;
    reg_Point->normal.z += (sample.b - 0x80) * k;

    k = AxMath::Sqrt(reg_Point->normal.x * reg_Point->normal.x +
                    reg_Point->normal.y * reg_Point->normal.y +
                    reg_Point->normal.z * reg_Point->normal.z);
    if (k != 0.0f)
    {
        k = 1.0f / k;
        reg_Point->normal.x *= k;
        reg_Point->normal.y *= k;
        reg_Point->normal.z *= k;
    }
}

void SrShaderOperations::FlatMap(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    renderThread->shaderReg_Texture = soras->shaderTextures[renderThread->shaderTextureIndex++];
    soras->textureSampler(renderThread,
						  (int)(reg_Point->position.x * SrUnit::high) / renderThread->shaderReg_Texture->width,
						  (int)(reg_Point->position.y * SrUnit::high) / renderThread->shaderReg_Texture->height,
						  renderThread->shaderReg_ColorArg);
}

void SrShaderOperations::RefractionMap(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	Soras *soras = renderThread->soras;
    renderThread->shaderReg_Texture = soras->shaderTextures[renderThread->shaderTextureIndex++];
    soras->textureSampler(renderThread,
						  (int)((reg_Point->positionIntScreen.x - reg_Point->normal.x * soras->shadingParameters.refractionFactor) * SrUnit::high) / soras->renderSurface->width,
						  (int)((reg_Point->positionIntScreen.y - reg_Point->normal.y * soras->shadingParameters.refractionFactor) * SrUnit::high) / soras->renderSurface->height,
						  renderThread->shaderReg_ColorArg);
}

void SrShaderOperations::TransformTexCoord(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;

	reg_Point->texCoords.x += renderThread->soras->shadingParameters.texTransformVector.x;
	reg_Point->texCoords.y += renderThread->soras->shadingParameters.texTransformVector.y;
}

void SrShaderOperations::Color_Set(SrRenderThread *renderThread)
{
	renderThread->shaderReg_Point->color = renderThread->shaderReg_ColorArg;
}

void SrShaderOperations::Color_Add(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.a += reg_ColorArg->a;
    reg_Point->color.r += reg_ColorArg->r;
    reg_Point->color.g += reg_ColorArg->g;
    reg_Point->color.b += reg_ColorArg->b;
}

void SrShaderOperations::Color_AlphaAdd(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.a += reg_ColorArg->a;
    reg_Point->color.r += (reg_ColorArg->r * reg_ColorArg->a) >> 8;
    reg_Point->color.g += (reg_ColorArg->g * reg_ColorArg->a) >> 8;
    reg_Point->color.b += (reg_ColorArg->b * reg_ColorArg->a) >> 8;
}

void SrShaderOperations::Color_Subtract(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.a -= reg_ColorArg->a;
    reg_Point->color.r -= reg_ColorArg->r;
    reg_Point->color.g -= reg_ColorArg->g;
    reg_Point->color.b -= reg_ColorArg->b;
}

void SrShaderOperations::Color_AlphaSubtract(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.a -= reg_ColorArg->a;
    reg_Point->color.r -= (reg_ColorArg->r * reg_ColorArg->a) >> 8;
    reg_Point->color.g -= (reg_ColorArg->g * reg_ColorArg->a) >> 8;
    reg_Point->color.b -= (reg_ColorArg->b * reg_ColorArg->a) >> 8;
}

void SrShaderOperations::Color_Modulate(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.a = (reg_Point->color.a * reg_ColorArg->a) >> 8;
    reg_Point->color.r = (reg_Point->color.r * reg_ColorArg->r) >> 8;
    reg_Point->color.g = (reg_Point->color.g * reg_ColorArg->g) >> 8;
    reg_Point->color.b = (reg_Point->color.b * reg_ColorArg->b) >> 8;
}

void SrShaderOperations::Color_AlphaModulate(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.r *= (reg_ColorArg->r * reg_ColorArg->a) >> 8;
    reg_Point->color.g *= (reg_ColorArg->g * reg_ColorArg->a) >> 8;
    reg_Point->color.b *= (reg_ColorArg->b * reg_ColorArg->a) >> 8;
}

void SrShaderOperations::Color_AlphaBlend(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	reg_Point->color.r += ((reg_ColorArg->r - reg_Point->color.r) * reg_ColorArg->a) >> 8;
    reg_Point->color.g += ((reg_ColorArg->g - reg_Point->color.g) * reg_ColorArg->a) >> 8;
    reg_Point->color.b += ((reg_ColorArg->b - reg_Point->color.b) * reg_ColorArg->a) >> 8;
}

void SrShaderOperations::Color_Lighter(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	if (reg_ColorArg->r > reg_Point->color.r)
		reg_Point->color.r = reg_ColorArg->r;
	if (reg_ColorArg->g > reg_Point->color.g)
		reg_Point->color.g = reg_ColorArg->g;
	if (reg_ColorArg->b > reg_Point->color.b)
		reg_Point->color.b = reg_ColorArg->b;
}

void SrShaderOperations::Color_Darker(SrRenderThread *renderThread)
{
	SrVertex *reg_Point = renderThread->shaderReg_Point;
	SrARGBInt *reg_ColorArg = &renderThread->shaderReg_ColorArg;

	if (reg_ColorArg->r < reg_Point->color.r)
		reg_Point->color.r = reg_ColorArg->r;
	if (reg_ColorArg->g < reg_Point->color.g)
		reg_Point->color.g = reg_ColorArg->g;
	if (reg_ColorArg->b < reg_Point->color.b)
		reg_Point->color.b = reg_ColorArg->b;
}