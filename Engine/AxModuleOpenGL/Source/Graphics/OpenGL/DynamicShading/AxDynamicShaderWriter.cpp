//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDynamicShaderWriter.h"


AxDynamicShaderWriter::AxDynamicShaderWriter(bool animated, int maxBones)
{
    this->vsInputType = new AxDynamicShaderBlockDefinitions();
	this->vsParams = new AxDynamicShaderBlockDefinitions();
	this->vsCode = new AxDynamicShaderBlockDefinitions();
	this->sIOType = new AxDynamicShaderBlockDefinitions();
	this->psParams = new AxDynamicShaderBlockDefinitions();
	this->psCode = new AxDynamicShaderBlockDefinitions();
	this->psOutputType = new AxDynamicShaderBlockDefinitions();

	this->animated = animated;
    this->maxBones = maxBones;

    this->numLights = 0;
    this->textureSlots = 0;
    this->texCoordSlots = 0;
    
    if (this->animated)
    {
		this->vsParams->ProvideDefinition("bones", AxString("uniform mat4 worldMatrix[") + this->maxBones + "];\r\n", AxParameterType_ReferenceTransform);
    }
    else
    {
		this->vsParams->ProvideDefinition("worldMatrix", "uniform mat4 worldMatrix;\r\n", AxParameterType_ReferenceTransform);
    }

    this->vsParams->ProvideDefinition("", "precision mediump float;");
	this->vsParams->ProvideDefinition("viewProjectionMatrix", "uniform mat4 viewProjectionMatrix;", AxParameterType_ReferenceTransform);
	this->vsParams->ProvideDefinition("ambientLight", "uniform vec4 ambientLight;", AxParameterType_Color);
	this->vsParams->ProvideDefinition("viewPosition", "uniform vec3 viewPosition;", AxParameterType_ReferenceVector3);
    
    this->psParams->ProvideDefinition("", "precision mediump float;");
	this->psParams->ProvideDefinition("viewPosition", "uniform vec3 viewPosition;", AxParameterType_ReferenceVector3);

    this->vsInputType->ProvideDefinition("input_pos", "attribute vec3 input_pos;");
    if (this->animated)
    {
        this->vsInputType->ProvideDefinition("input_boneIndices", "attribute vec4 input_boneIndices;");
        this->vsInputType->ProvideDefinition("input_boneWeights", "attribute vec4 input_boneWeights;");
    }

    this->sIOType->ProvideDefinition("pos", "varying vec4 sio_pos;");

    this->psParams->ProvideDefinition("", "precision mediump float;");
	this->psParams->ProvideDefinition("ambientLight", "uniform vec4 ambientLight;", AxParameterType_Color);
    
    this->psOutputType->ProvideDefinition("PSOutput", "struct PSOutput\r\n{");
    this->psOutputType->ProvideDefinition("color", "\tvec4 color;");

    this->vsCode->ProvideDefinition("", "void main()\r\n{");
    if (this->animated)
    {
        this->vsCode->ProvideDefinition("bonesMatrix", "\tmat4 bonesMatrix;");
        this->vsCode->ProvideDefinition("", AxString("\tbonesMatrix = \r\n") +
            "\t\tworldMatrix[int(input_boneIndices.x)] * input_boneWeights.x +\r\n" +
            "\t\tworldMatrix[int(input_boneIndices.y)] * input_boneWeights.y +\r\n" +
            "\t\tworldMatrix[int(input_boneIndices.z)] * input_boneWeights.z +\r\n" +
            "\t\tworldMatrix[int(input_boneIndices.w)] * input_boneWeights.w;");

        this->vsCode->ProvideDefinition("worldPos", "\tvec4 worldPos = bonesMatrix * vec4(input_pos, 1.0);\r\n");
    }
    else
    {
        this->vsCode->ProvideDefinition("worldPos", "\tvec4 worldPos = worldMatrix * vec4(input_pos, 1.0);\r\n");
    }
    this->vsCode->ProvideDefinition(".pos", "\tsio_pos = viewProjectionMatrix * worldPos;\r\n");

    this->psCode->ProvideDefinition("", "void main()\r\n{");
    this->psCode->ProvideDefinition("result", "\tPSOutput result;\r\n");
}


AxDynamicShaderWriter::~AxDynamicShaderWriter(void)
{
    delete this->vsInputType;
	delete this->vsParams;
	delete this->vsCode;
	delete this->sIOType;
	delete this->psParams;
	delete this->psCode;
	delete this->psOutputType;
}

void AxDynamicShaderWriter::AddShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp)
{
    switch (shadingOp)
    {
    //*
        case AxShadingOperation_BasicShading:
        {
            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");

            this->sIOType->ProvideDefinition("color", "varying vec4 sio_color;");

            if (this->animated)
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
            else
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");

            this->vsCode->ProvideDefinition("", "\tfloat intensity = clamp(-normal.z, 0.0, 1.0);\r\n");
            this->vsCode->ProvideDefinition("", "\tsio_color = vec4(intensity, intensity, intensity, 1.0);\r\n");

            this->psCode->ProvideDefinition("", "\tresult.color = sio_color;\r\n");
            //this->ProvideColorOperationCode(shadingOp, colorOp, "", this->psCode, "\t", "result.color", "sio_color", colorOp);
            break;
        }

        case AxShadingOperation_FlatColor:
        {
            AxString pFlatColor = this->ProvideParameterDefinition(this->psParams, shadingOp, "flatColor", AxParameterType_Color);
            
            this->ProvideColorOperationCode(shadingOp, colorOp, "flatColorAmount", this->psCode, "\t", "result.color", pFlatColor);
            
            break;
        }

        case AxShadingOperation_Tint:
        {
            AxString pTintColor = this->ProvideParameterDefinition(this->psParams, shadingOp, "tintColor", AxParameterType_Color);

            this->psCode->ProvideDefinition("tintFactor", "float tintFactor;");
            this->psCode->ProvideDefinition("", "\ttintFactor = (result.color.x + result.color.y + result.color.z) / 3.0;");

            this->ProvideColorOperationCode(shadingOp, colorOp, "tintAmount", this->psCode, "\t", "result.color", pTintColor + " * vec4(tintFactor, tintFactor, tintFactor, 1.0)");

            break;
        }

        case AxShadingOperation_AmbientLighting:
        {
            this->ProvideColorOperationCode(shadingOp, colorOp, "ambientAmount", this->psCode, "\t", "result.color", "ambient * ambientLight");
            
            break;
        }

        case AxShadingOperation_LightSource:
        {
            this->numLights++;
            break;
        }

        case AxShadingOperation_VertexLighting:
        {
            if (this->numLights == 0)
                break;

            if (this->shadingOps.Count(AxShadingOperation_VertexLighting) > 0)
                break;

            this->ProvideParameterDefinition(this->vsParams, shadingOp, "activeLightsCount", AxParameterType_Float);

            
            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");

            this->sIOType->ProvideDefinition("color", "varying vec4 sio_color;");

            if (this->animated)
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
            else
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");
            
            this->vsCode->ProvideDefinition("viewVector", "\tvec3 viewVector = normalize(viewPosition - worldPos.xyz);");
            this->vsCode->ProvideDefinition("lightVector", "\tvec3 lightVector;");
            this->vsCode->ProvideDefinition("lightReflectionVector", "\tvec3 lightReflectionVector;");
            this->vsCode->ProvideDefinition("lightAttenuation", "\tfloat lightAttenuation;");
            this->vsCode->ProvideDefinition("lightDiffuse", "\tfloat lightDiffuse;");
            this->vsCode->ProvideDefinition("lightSpecular", "\tfloat lightSpecular;");
            this->vsCode->ProvideDefinition("color", "\tvec4 color;");
            this->vsCode->ProvideDefinition("", 
                AxString("\tcolor = vec4(0.0, 0.0, 0.0, 0.0);\r\n") +
                "\tint iActiveLightsCount = int(activeLightsCount);\r\n" +
                "\tfor (int lightIndex = 0; lightIndex < maxLights; lightIndex++)\r\n" +
                "\t{\r\n" +
                "\t\tlightVector = lightsPosIntensity[lightIndex].xyz - worldPos.xyz;\r\n" +
                "\t\tlightAttenuation = 1.0 / length(lightVector);\r\n" +
                "\t\tlightVector *= lightAttenuation;\r\n" +
                "\t\tlightAttenuation *= lightAttenuation;\r\n" +
                "\t\tlightDiffuse = clamp(dot(normal, lightVector), 0.0, 1.0) * lightsPosIntensity[lightIndex].w * lightAttenuation;\r\n" +
                "\t\tlightReflectionVector = normalize(lightVector + viewVector);\r\n" +
                "\t\tlightSpecular = pow(clamp(dot(normal, lightReflectionVector), 0.0, 1.0), gloss);\r\n" +
                "\t\tcolor += lightsColor[lightIndex] * (lightDiffuse * diffuse + lightSpecular * specular);\r\n" +
                "\t\tiActiveLightsCount--;\r\n" +
                "\t\tif (iActiveLightsCount == 0)\r\n" +
                "\t\t\t break;\r\n" +
                "\t}\r\n"
                );
            this->vsCode->ProvideDefinition("", "\tsio_color = color;");

            this->ProvideColorOperationCode(shadingOp, colorOp, "vertexLightingAmount", this->psCode, "\t", "result.color", "sio_color");
            break;
        }


        case AxShadingOperation_PixelLighting:
        {
            if (this->numLights == 0)
                break;

            if (this->shadingOps.Count(AxShadingOperation_PixelLighting) > 0)
                break;

            this->ProvideParameterDefinition(this->psParams, shadingOp, "activeLightsCount", AxParameterType_Int);

            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");

            this->sIOType->ProvideDefinition("normal", "varying vec3 sio_normal;");
            this->sIOType->ProvideDefinition("worldPos", "varying vec4 sio_worldPos;");

            if (this->animated)
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
            else
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");
            this->vsCode->ProvideDefinition(".normal", "\tsio_normal = normal;");
            this->vsCode->ProvideDefinition(".worldPos", "\tsio_worldPos = worldPos;");

            this->psCode->ProvideDefinition("sio2_normal", "\tvec3 sio2_normal = sio_normal;");

            this->psCode->ProvideDefinition("viewVector", "\tvec3 viewVector = normalize(viewPosition - sio_worldPos.xyz);");
            this->psCode->ProvideDefinition("lightVector", "\tvec3 lightVector;");
            this->psCode->ProvideDefinition("lightReflectionVector", "\tvec3 lightReflectionVector;");
            this->psCode->ProvideDefinition("lightAttenuation", "\tfloat lightAttenuation;");
            this->psCode->ProvideDefinition("lightDiffuse", "\tfloat lightDiffuse;");
            this->psCode->ProvideDefinition("lightSpecular", "\tfloat lightSpecular;");
            this->psCode->ProvideDefinition("color", "\tvec4 color;");
            this->psCode->ProvideDefinition("", 
                AxString("\tcolor = vec4(0.0, 0.0, 0.0, 0.0);\r\n") +
                "\tint iActiveLightsCount = int(activeLightsCount);\r\n" +
                "\tfor (int lightIndex = 0; lightIndex < maxLights; lightIndex++)\r\n" +
                "\t{\r\n" +
                "\t\tlightVector = lightsPosIntensity[lightIndex].xyz - sio_worldPos.xyz;\r\n" +
                "\t\tlightAttenuation = 1.0 / length(lightVector);\r\n" +
                "\t\tlightVector *= lightAttenuation;\r\n" +
                "\t\tlightAttenuation *= lightAttenuation;\r\n" +
                "\t\tlightDiffuse = clamp(dot(sio2_normal, lightVector), 0.0, 1.0) * lightsPosIntensity[lightIndex].w * lightAttenuation;\r\n" +
                "\t\tlightReflectionVector = normalize(lightVector + viewVector);\r\n" +
                "\t\tlightSpecular = pow(clamp(dot(sio2_normal, lightReflectionVector), 0.0, 1.0), gloss);\r\n" +
                "\t\tcolor += lightsColor[lightIndex] * (lightDiffuse * diffuse + lightSpecular * specular);\r\n" +
                "\t\tiActiveLightsCount--;\r\n" +
                "\t\tif (iActiveLightsCount == 0)\r\n" +
                "\t\t\t break;\r\n" +
                "\t}\r\n"
                );

            this->ProvideColorOperationCode(shadingOp, colorOp, "pixelLightingAmount", this->psCode, "\t", "result.color", "color");
            break;
        }
                
        case AxShadingOperation_ColorMap:
        {
            AxString pColorMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "colorMap", AxParameterType_ReferenceTexture);

            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");

            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");

            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = input_texCoords;");

            this->psCode->ProvideDefinition("sio2_texCoords", "vec2 sio2_texCoords = sio_texCoords;");

            this->ProvideColorOperationCode(shadingOp, colorOp, "colorMapAmount", this->psCode, "\t", "result.color", AxDynamicShaderWriter::TextureSampling(pColorMap, "sio2_texCoords"));

            break;
        }

        case AxShadingOperation_ReflectionMap:
        {
            AxString pReflectionMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "reflectionMap", AxParameterType_ReferenceTexture);
            AxString pReflectionFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "reflectionMapFactor", AxParameterType_Float);

            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");

            this->sIOType->ProvideDefinition("normal", "varying vec3 sio_normal;");
            this->sIOType->ProvideDefinition("screenSpaceNormal", "varying vec3 sio_screenSpaceNormal;");
            this->sIOType->ProvideDefinition("screenSpacePos", "varying vec3 sio_screenSpacePos;");

            if (this->animated)
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
            else
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");
            this->vsCode->ProvideDefinition("wvpNormal", "\tvec3 wvpNormal = (viewProjectionMatrix * vec4(normal, 0.0)).xyz;");
            this->vsCode->ProvideDefinition(".normal", "\tsio_normal = normal;");
            this->vsCode->ProvideDefinition(".screenSpaceNormal", "\tsio_screenSpaceNormal = wvpNormal;");
            this->vsCode->ProvideDefinition(".screenSpacePos", 
                "\tsio_screenSpacePos = ((sio_pos.xyz / sio_pos.w) - 1.0) / 2.0;\r\n");

            this->psCode->ProvideDefinition("sio2_normal", "\tvec3 sio2_normal = sio_normal;");
            this->psCode->ProvideDefinition("sampleTexCoords", "\tvec2 sampleTexCoords;");
            this->psCode->ProvideDefinition("", AxString("\tsampleTexCoords = sio_screenSpacePos.xy - sio2_normal.xy * ") + pReflectionFactor + ";");
            
            this->ProvideColorOperationCode(shadingOp, colorOp, "reflectionMapAmount", this->psCode, "\t", "result.color", AxDynamicShaderWriter::TextureSampling(pReflectionMap, "sampleTexCoords"));
            break;
        }

        case AxShadingOperation_RefractionMap:
        {
            AxString pRefractionMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "refractionMap", AxParameterType_ReferenceTexture);
            AxString pRefractionMapFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "refractionMapFactor", AxParameterType_Float);

            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");

            this->sIOType->ProvideDefinition("normal", "varying vec3 sio_normal;");
            this->sIOType->ProvideDefinition("screenSpaceNormal", "varying vec3 sio_screenSpaceNormal;");
            this->sIOType->ProvideDefinition("screenSpacePos", "varying vec3 sio_screenSpacePos;");

            if (this->animated)
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
            else
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");
            this->vsCode->ProvideDefinition("wvpNormal", "\tvec3 wvpNormal = (viewProjectionMatrix * vec4(normal, 0.0)).xyz;");
            this->vsCode->ProvideDefinition(".normal", "\tsio_normal = normal;");
            this->vsCode->ProvideDefinition(".screenSpaceNormal", "\tsio_screenSpaceNormal = wvpNormal;");
            this->vsCode->ProvideDefinition(".screenSpacePos", 
                "\tsio_screenSpacePos = ((sio_pos.xyz / sio_pos.w) - 1.0) / 2.0;\r\n");

            this->psCode->ProvideDefinition("sio2_normal", "\tvec3 sio2_normal = sio_normal;");
            this->psCode->ProvideDefinition("sampleTexCoords", "\tvec2 sampleTexCoords;");
            this->psCode->ProvideDefinition("", AxString("\tsampleTexCoords = sio_screenSpacePos.xy - sio2_normal.xy * ") + pRefractionMapFactor + ";");

            this->ProvideColorOperationCode(shadingOp, colorOp, "refractionMapAmount", this->psCode, "\t", "result.color", AxDynamicShaderWriter::TextureSampling(pRefractionMap, "sampleTexCoords"));

            break;
        }
        
        case AxShadingOperation_NormalMap:
        {
            AxString pNormalMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "normalMap", AxParameterType_ReferenceTexture);
            AxString pNormalMapFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "normalMapFactor", AxParameterType_Float);

            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");
            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");
            this->vsInputType->ProvideDefinition("tangent", "attribute vec3 input_tangent;");
            this->vsInputType->ProvideDefinition("biTangent", "attribute vec3 input_biTangent;");

            this->sIOType->ProvideDefinition("normal", "varying vec3 sio_normal;");
            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");
            this->sIOType->ProvideDefinition("tangent", "varying vec3 sio_tangent;");
            this->sIOType->ProvideDefinition("biTangent", "varying vec3 sio_biTangent;");

            if (this->animated)
            {
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
                this->vsCode->ProvideDefinition("tangent", "\tvec3 tangent = normalize(bonesMatrix * vec4(input_tangent, 0.0)).xyz;");
                this->vsCode->ProvideDefinition("biTangent", "\tvec3 biTangent = normalize(bonesMatrix * vec4(input_biTangent, 0.0)).xyz;");
            }
            else
            {
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");
                this->vsCode->ProvideDefinition("tangent", "\tvec3 tangent = normalize(worldMatrix * vec4(input_tangent, 0.0)).xyz;");
                this->vsCode->ProvideDefinition("biTangent", "\tvec3 biTangent = normalize(worldMatrix * vec4(input_biTangent, 0.0)).xyz;");
            }

            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = input_texCoords;");
            this->vsCode->ProvideDefinition(".normal", "\tsio_normal = normal;");
            this->vsCode->ProvideDefinition(".tangent", "\tsio_tangent = tangent;");
            this->vsCode->ProvideDefinition(".biTangent", "\tsio_biTangent = biTangent;");

            this->psCode->ProvideDefinition("sio2_texCoords", "\tvec2 sio2_texCoords = sio_texCoords;");
            this->psCode->ProvideDefinition("sio2_normal", "\tvec3 sio2_normal;");
            this->psCode->ProvideDefinition("",  
                AxString("\tvec3 mapNormal = (") + AxDynamicShaderWriter::TextureSampling(pNormalMap, "sio2_texCoords") + ".xyz * 2.0 - 1.0) * " + pNormalMapFactor + ";\r\n" +
                "\tsio2_normal = mapNormal.x * sio_tangent + mapNormal.y * sio_biTangent + mapNormal.z * sio_normal;\r\n");

            break;
        }
            
            
            
            
        case AxShadingOperation_BumpMap:
        {
            AxString pBumpMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "bumpMap", AxParameterType_ReferenceTexture);
            AxString pBumpMapFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "bumpMapFactor", AxParameterType_Float);
            
            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");
            this->vsInputType->ProvideDefinition("normal", "attribute vec3 input_normal;");

            this->sIOType->ProvideDefinition("normal", "varying vec3 sio_normal;");
            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");

            if (this->animated)
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(bonesMatrix * vec4(input_normal, 0.0)).xyz;");
            else
                this->vsCode->ProvideDefinition("normal", "\tvec3 normal = normalize(worldMatrix * vec4(input_normal, 0.0)).xyz;");
            this->vsCode->ProvideDefinition(".normal", "\tsio_normal = normal;");
            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = input_texCoords;");

            this->psCode->ProvideDefinition("sio2_texCoords", "\tvec2 sio2_texCoords = sio_texCoords;");
            this->psCode->ProvideDefinition("sio2_normal", "\tvec3 sio2_normal = sio_normal;");
            this->psCode->ProvideDefinition("", 
                AxString("\tvec3 bumpMapNormal = normalize(") + AxDynamicShaderWriter::TextureSampling(pBumpMap, "sio2_texCoords") + ").xyz * " + pBumpMapFactor + " * 2.0 - 1.0;\r\n" +
                "\tsio2_normal += bumpMapNormal;\r\n");


            break;
        }

        
        case AxShadingOperation_BlurMap:
        {
            AxString pBlurMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "blurMap", AxParameterType_ReferenceTexture);
            AxString pBlurMapFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "blurMapFactor", AxParameterType_Float);

            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");

            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");

            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = input_texCoords;");

            this->psCode->ProvideDefinition("sio2_texCoords", "\tvec2 sio2_texCoords = sio_texCoords;");

            this->psCode->ProvideDefinition("blurSum", "\tvec4 blurSum;");

            this->psCode->ProvideDefinition("", 
                AxString("\tblurSum = ") + AxDynamicShaderWriter::TextureSampling(pBlurMap, "sio2_texCoords") + ";\r\n"  +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2(-0.326212, -0.405805) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2(-0.840144, -0.073580) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2(-0.695914,  0.457137) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2(-0.203345,  0.620716) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2( 0.962340, -0.194983) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2( 0.473434, -0.480026) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2( 0.519456,  0.767022) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2( 0.185461, -0.893124) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2( 0.507431,  0.064425) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2( 0.896420,  0.412458) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2(-0.321940, -0.932615) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pBlurMap, AxString("sio2_texCoords + vec2(-0.791559, -0.597705) * ") + pBlurMapFactor) + ";\r\n" +
                "\tblurSum /= 13.0;\r\n");
           
            this->ProvideColorOperationCode(shadingOp, colorOp, "blurMapAmount", this->psCode, "\t", "result.color", "blurSum");
            break;
        }

        case AxShadingOperation_HBlur:
        {
            AxString pHBlurMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "hBlurMap", AxParameterType_ReferenceTexture);
            AxString pHBlurMapFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "hBlurMapFactor", AxParameterType_Float);

            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");

            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");

            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = input_texCoords;");

            this->psCode->ProvideDefinition("sio2_texCoords", "\tvec2 sio2_texCoords = sio_texCoords;");

            this->psCode->ProvideDefinition("blurSum", "\tvec4 blurSum;");

            this->psCode->ProvideDefinition("", 
                AxString("\tblurSum = ") + AxDynamicShaderWriter::TextureSampling(pHBlurMap, "sio2_texCoords") + ";\r\n"  +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pHBlurMap, AxString("sio2_texCoords + vec2(-0.33, 0.0) * ") + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pHBlurMap, AxString("sio2_texCoords + vec2(-0.66, 0.0) * ") + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pHBlurMap, AxString("sio2_texCoords + vec2(-0.99, 0.0) * ") + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pHBlurMap, AxString("sio2_texCoords + vec2(0.33, 0.0) * ") + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pHBlurMap, AxString("sio2_texCoords + vec2(0.66, 0.0) * ") + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pHBlurMap, AxString("sio2_texCoords + vec2(0.99, 0.0) * ") + pHBlurMapFactor) + ";\r\n" +
                "\tblurSum /= 7.0;");

            this->ProvideColorOperationCode(shadingOp, colorOp, "hBlurMapAmount", this->psCode, "\t", "result.color", "blurSum");
            break;
        }

        case AxShadingOperation_VBlur:
        {
            AxString pVBlurMap = this->ProvideParameterDefinition(this->psParams, shadingOp, "vBlurMap", AxParameterType_ReferenceTexture);
            AxString pVBlurMapFactor = this->ProvideParameterDefinition(this->psParams, shadingOp, "vBlurMapFactor", AxParameterType_Float);

            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");

            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");

            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = input_texCoords;");

            this->psCode->ProvideDefinition("sio2_texCoords", "\tvec2 sio2_texCoords = sio_texCoords;");

            this->psCode->ProvideDefinition("blurSum", "\tvec4 blurSum;");

            this->psCode->ProvideDefinition("", 
                AxString("\tblurSum = ") + AxDynamicShaderWriter::TextureSampling(pVBlurMap, "sio2_texCoords") + ";\r\n"  +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pVBlurMap, AxString("sio2_texCoords + vec2(0.0, -0.33) * ") + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pVBlurMap, AxString("sio2_texCoords + vec2(0.0, -0.66) * ") + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pVBlurMap, AxString("sio2_texCoords + vec2(0.0, -0.99) * ") + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pVBlurMap, AxString("sio2_texCoords + vec2(0.0, 0.33) * ") + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pVBlurMap, AxString("sio2_texCoords + vec2(0.0, 0.66) * ") + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum += " + AxDynamicShaderWriter::TextureSampling(pVBlurMap, AxString("sio2_texCoords + vec2(0.0, 0.99) * ") + pVBlurMapFactor) + ";\r\n" +
                "\tblurSum /= 7.0;");

            this->ProvideColorOperationCode(shadingOp, colorOp, "vBlurMapAmount", this->psCode, "\t", "result.color", "blurSum");
            break;
        }

        case AxShadingOperation_AlignScreenQuad:
        {
            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");

            this->vsCode->ProvideDefinition("screenPos", "\tvec2 screenPos;");
            this->vsCode->ProvideDefinition("",
                    AxString("\tscreenPos = sign(input_pos.xy);\r\n") +
                    "\tsio_pos = vec4(screenPos, 0.999, 1.0);\r\n");// + // 0.999 for zbuffer
                    //"\tsio_pos = vec4(screenPos, 0.001, 1.0);\r\n" +
                    //"\tsio_pos.y = -sio_pos.y; // remove this, instead change vertex order");

            this->vsCode->ProvideDefinition(".texCoords", "\tsio_texCoords = (screenPos + 1.0) / 2.0;");

            break;
        }
        /*



        case AxShadingOperation_ProjectTexture:
        {
                if (this->various.ProvideDefinition("projectionMap", 
                        AxString("Texture2D tProjectionMap: register(t") + AxString(this->textureBuffersInfo.count) + ");\r\n"
                        "sampler sProjectionMap: register(s" + AxString(this->textureBuffersInfo.count) + ");\r\n"))
                {
                        this->textureBuffersInfo.Add("projectionMap");
                }

                if (this->parameters.ProvideDefinition("projectionCamera", "\tmat4 projectionCamera;"))
                {
                        this->constantBuffersInfo.Get(0).Add("projectionCamera", 64);
                }

                if (this->sIOType->ProvideDefinition("projectionTexCoords", AxString("\tvec2 projectionTexCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
                        this->texCoordSlots++;

                this->vsCode->ProvideDefinition("projectionCoords", "\tvec4 projectionCoords;");
                this->vsCode->ProvideDefinition("", "\tprojectionCoords = mul(projectionCamera, worldPos);");
                this->vsCode->ProvideDefinition(".projectionTexCoords", "\toutput.projectionTexCoords = ((projectionCoords.xy / projectionCoords.w) + 1.0) / 2.0;");

                this->psCode->ProvideDefinition("", AxString("\t") + this->GetColorOperationCode("output.color", "tProjectionMap.Sample(sProjectionMap, input_projectionTexCoords)", colorOp));

                break;
        }

        case AxShadingOperation_ProjectPicture:
        {
                if (this->various.ProvideDefinition("projectionMap", 
                        AxString("Texture2D tProjectionMap: register(t") + AxString(this->textureBuffersInfo.count) + ");\r\n"
                        "sampler sProjectionMap: register(s" + AxString(this->textureBuffersInfo.count) + ");\r\n"))
                {
                        this->textureBuffersInfo.Add("projectionMap");
                }

                if (this->parameters.ProvideDefinition("projectionCamera", "\tmat4 projectionCamera;"))
                {
                        this->constantBuffersInfo.Get(0).Add("projectionCamera", 64);
                }

                if (this->sIOType->ProvideDefinition("projectionTexCoords", AxString("\tvec2 projectionTexCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
                        this->texCoordSlots++;

                this->vsCode->ProvideDefinition("projectionCoords", "\tvec4 projectionCoords;");
                this->vsCode->ProvideDefinition("", "\tprojectionCoords = mul(projectionCamera, worldPos);");
                this->vsCode->ProvideDefinition(".projectionTexCoords", "\toutput.projectionTexCoords = ((projectionCoords.xy / projectionCoords.w) + 1.0) / 2.0;");

                this->psCode->ProvideDefinition("", 
                        AxString("\tif ((input_projectionTexCoords.x > 0.0) && (input_projectionTexCoords.x < 1.0) && (input_projectionTexCoords.y > 0.0) && (input_projectionTexCoords.y < 1.0))\r\n") +
                        "\t\t" + this->GetColorOperationCode("output.color", "tProjectionMap.Sample(sProjectionMap, input_projectionTexCoords)", colorOp));

                break;
        }

    //*/

        case AxShadingOperation_TextureTransform:
        {
            AxString pTextureTransform = this->ProvideParameterDefinition(this->psParams, shadingOp, "textureTransform", AxParameterType_ReferenceTransform);

            this->vsInputType->ProvideDefinition("texCoords", "attribute vec2 input_texCoords;");

            this->sIOType->ProvideDefinition("texCoords", "varying vec2 sio_texCoords;");
            
            this->psCode->ProvideDefinition("sio2_texCoords", "vec2 sio2_texCoords;");
            this->psCode->ProvideDefinition("", AxString("\tsio2_texCoords = (") + pTextureTransform + " * vec4(sio_texCoords, 0, 1)).xy;");
            break;
        }
        
        case AxShadingOperation_Skinning:
        {
            break;
        }

        case AxShadingOperation_Material:
        {
            this->ProvideParameterDefinition(this->vsParams, shadingOp, "ambient", AxParameterType_Color);
            this->ProvideParameterDefinition(this->vsParams, shadingOp, "diffuse", AxParameterType_Color);
            this->ProvideParameterDefinition(this->vsParams, shadingOp, "specular", AxParameterType_Color);
            this->ProvideParameterDefinition(this->vsParams, shadingOp, "gloss", AxParameterType_Float);

            this->ProvideParameterDefinition(this->psParams, shadingOp, "ambient", AxParameterType_Color);
            this->ProvideParameterDefinition(this->psParams, shadingOp, "diffuse", AxParameterType_Color);
            this->ProvideParameterDefinition(this->psParams, shadingOp, "specular", AxParameterType_Color);
            this->ProvideParameterDefinition(this->psParams, shadingOp, "gloss", AxParameterType_Float);
        }
        case AxShadingOperation_Fog:
        {
            AxString pFogColor = this->ProvideParameterDefinition(this->psParams, shadingOp, "fogColor", AxParameterType_Color);
            AxString pFogStart = this->ProvideParameterDefinition(this->vsParams, shadingOp, "fogStart", AxParameterType_Float);
            AxString pFogDensity = this->ProvideParameterDefinition(this->vsParams, shadingOp, "fogDensity", AxParameterType_Float);

            this->sIOType->ProvideDefinition("fogImmersion", "varying float sio_fogImmersion;");

            this->vsCode->ProvideDefinition("distance", "\tfloat distance = length(viewPosition - worldPos.xyz);");
            this->vsCode->ProvideDefinition(".fogImmersion", AxString("\tsio_fogImmersion = clamp((distance - ") + pFogStart + ") / " + pFogDensity + ", 0.0, 1.0);");

            this->psCode->ProvideDefinition("fogColorAlpha", 
                AxString("\tvec4 fogColorAlpha = ") + pFogColor + ";\r\n" +
                "\tfogColorAlpha.w = sio_fogImmersion;");

            this->ProvideColorOperationCode(shadingOp, colorOp, "fogAmount", this->psCode, "\t", "result.color", "fogColorAlpha");

            break;
        }
    }

    this->shadingOps.Add(shadingOp);
};

AxString AxDynamicShaderWriter::GetVSSourceCode()
{
    if (this->numLights > 0)
    {
        this->vsParams->ProvideDefinition("", AxString("#define maxLights ") + this->numLights);
        this->vsParams->ProvideDefinition("", "uniform vec4 lightsPosIntensity[maxLights];");
        this->vsParams->ProvideDefinition("lightsColor", "uniform vec4 lightsColor[maxLights];\r\n", AxParameterType_Color);
        
		this->psParams->ProvideDefinition("", AxString("#define maxLights ") + this->numLights);
		this->psParams->ProvideDefinition("", "uniform vec4 lightsPosIntensity[maxLights];");
        this->psParams->ProvideDefinition("lightsColor", "uniform vec4 lightsColor[maxLights];\r\n", AxParameterType_Color);
    }

    return AxString(AxString("// Generated by Axis GLSL dynamic shader writer\r\n\r\n") +

        this->vsParams->GetCode().GetContents() + "\r\n\r\n" +

        this->vsInputType->GetCode().GetContents() + "\r\n\r\n" +

        this->sIOType->GetCode().GetContents() + "\r\n\r\n" +

        this->vsCode->GetCode().GetContents() + "\r\n\tgl_Position = sio_pos;\r\n}");
};

AxString AxDynamicShaderWriter::GetPSSourceCode()
{
    return AxString(AxString("// Generated by Axis GLSL dynamic shader writer\r\n\r\n") +

        this->psParams->GetCode().GetContents() + "\r\n\r\n" +

        this->sIOType->GetCode().GetContents() + "\r\n\r\n" +

        this->psOutputType->GetCode().GetContents() + "};\r\n\r\n" +

        this->psCode->GetCode().GetContents() + 
            "\r\n" +
            "\tgl_FragColor = result.color;\r\n" +
            "\tgl_FragColor.w = 1.0;\r\n}");
};


// Private methods


AxString AxDynamicShaderWriter::ProvideParameterDefinition(AxDynamicShaderBlockDefinitions *definitions, AxShadingOperation shadingOp, AxString desiredName, AxParameterType type, int elementsCount, bool isArray)
{
    int shadingOpCount = this->shadingOps.Count(shadingOp);
    AxString name = shadingOpCount == 0 ? AxString(desiredName) : (desiredName + AxString(shadingOpCount));
    isArray = isArray || (elementsCount > 1);
    
    AxString typeName;
    switch (type)
    {
        case AxParameterType_Int: 
        { 
            typeName = "int";
            break; 
        }
        case AxParameterType_Float:
        { 
            typeName = "float";
            break; 
        }
        case AxParameterType_Bool:
        { 
            typeName = "bool";
            break; 
        }
        case AxParameterType_Color:
        { 
            typeName = "vec4";
            break; 
        }
        case AxParameterType_ReferenceVector3:
        { 
            typeName = "vec3";
            break; 
        }
        case AxParameterType_ReferenceCamera:
        { 
            typeName = "mat4";
            break; 
        }
        case AxParameterType_ReferenceTransform:
        { 
            typeName = "mat4";
            break; 
        }
        case AxParameterType_ReferenceTexture:
        { 
            typeName = "sampler2D";
            break; 
        }
    }
    
    definitions->ProvideDefinition(name, AxString("uniform") + " " + typeName + " " + name + ";", type);
    
    return name;
};

void AxDynamicShaderWriter::ProvideColorOperationCode(AxShadingOperation shadingOp, AxColorOperation colorOp, AxString desiredFactorName, AxDynamicShaderBlockDefinitions *definitions, AxString linePrefix, AxString destArg, AxString sourceArg)
{
    AxDynamicShaderBlockDefinitions *paramsDefinitions = definitions == this->psCode ? this->psParams : this->vsParams;
    
    switch (colorOp)
    {
        case AxColorOperation_None:
            break;

        case AxColorOperation_Set:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " = " + sourceArg + " * " + factor + ";");
            
            break;
        }

        case AxColorOperation_Blend:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " = mix(" + destArg + ", (" + sourceArg + "), " + factor + ");");
            
            break;
        }
        
        case AxColorOperation_Add:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " += (" + sourceArg + ") * " + factor + ";");
            
            break;
        }

        case AxColorOperation_Subtract:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " -= (" + sourceArg + ") * " + factor + ";");
            
            break;
        }

        case AxColorOperation_Modulate:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " *= (" + sourceArg + ") * " + factor + ";");
            
            break;
        }

        case AxColorOperation_Lighter:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " = max(" + destArg + ", " + sourceArg + " * " + factor + ");");
            
            break;
        }

        case AxColorOperation_Darker:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("", linePrefix + destArg + " = min(" + destArg + ", " + sourceArg + " * " + factor + ");");
            
            break;
        }

        case AxColorOperation_AlphaBlend:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions->ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " = mix(" + destArg + ", colorOpArg, colorOpArg.w * " + factor + ");");
            
            break;
        }

        case AxColorOperation_AlphaAdd:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions->ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " += " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");
            
            break;
        }

        case AxColorOperation_AlphaSubtract:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions->ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " -= " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");
            
            break;
        }

        case AxColorOperation_AlphaModulate:
        {
            AxString factor = this->ProvideParameterDefinition(paramsDefinitions, shadingOp, desiredFactorName, AxParameterType_Float);
            definitions->ProvideDefinition("colorOpArg", linePrefix + "vec4 colorOpArg;\r\n");
            definitions->ProvideDefinition("", 
                linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
                linePrefix + destArg + " *= " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");
            
            break;
        }

        default:
        {
            definitions->ProvideDefinition("", linePrefix + destArg + " = 0;");

            break;
        }    
    }
};

AxString AxDynamicShaderWriter::PropertyNameToParameterName(AxString propertyName)
{
	AxString result;

	bool isFirstCharacter = true;
	AxString character, lastCharacter;
	for (int i = 0; i < propertyName.length; i++)
	{
		character = propertyName.contents[i];

		if (character != " ")
		{
			if (isFirstCharacter)
			{
				result += AxString(character).ToLower();
				isFirstCharacter = false;
			}
			else
			{
				if (lastCharacter ==  " ")
					result +=  AxString(character).ToUpper();
				else
					result += character;
			}
			
		}

		lastCharacter = character;
	}

	return AxString(result);
}

AxString AxDynamicShaderWriter::TextureSampling(AxString texture, AxString texCoords)
{
    return AxString(AxString("texture2D(") + texture + ", " + texCoords + ")");
};

AxString AxDynamicShaderWriter::TextureSampling(AxString texture, AxString texCoords, AxString texCoords1)
{
    return AxString(AxString("texture2D(") + texture + ", vec2(" + texCoords + ", " + texCoords1 + "))");
};
