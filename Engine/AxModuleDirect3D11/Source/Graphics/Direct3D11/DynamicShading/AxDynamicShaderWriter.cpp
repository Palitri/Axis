//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxDynamicShaderWriter.h"


AxDynamicShaderWriter::AxDynamicShaderWriter(bool animated, AxList<AxDynamicShaderConstantBufferInfo*> *constantBuffersInfo, AxStrings *textureBuffersInfo)
{
	this->animated = animated;
	
	this->numLights = 0;
	this->textureSlots = 0;
	this->texCoordSlots = 0;

	this->constantBuffersInfo = constantBuffersInfo;
	this->textureBuffersInfo = textureBuffersInfo;
	
	this->constantBuffersInfo->Add(new AxDynamicShaderConstantBufferInfo());

	if (this->animated)
	{
		this->parameters.ProvideDefinition("bones", 
			"cbuffer Bones : register(b1)\r\n{\r\n"
			"\tfloat4x4 worldMatrix[64];\r\n};\r\n");

		this->constantBuffersInfo->Add(new AxDynamicShaderConstantBufferInfo());
		this->constantBuffersInfo->Get(1)->Add("worldMatrix", 64, 64, 0, true);
	}

	this->parameters.ProvideDefinition("", "cbuffer Parameters : register(b0)\r\n{");

	if (!this->animated)
	{
		this->parameters.ProvideDefinition("worldMatrix", "\tfloat4x4 worldMatrix : World;");
		this->constantBuffersInfo->Get(0)->Add("worldMatrix", 64);
	}

	this->parameters.ProvideDefinition("viewProjectionMatrix", "\tfloat4x4 viewProjectionMatrix : ViewProjection;");//
	this->parameters.ProvideDefinition("ambientLight", "\tfloat4 ambientLight;");
	this->parameters.ProvideDefinition("viewPosition", "\tfloat3 viewPosition;");
	this->constantBuffersInfo->Get(0)->Add("viewProjectionMatrix", 64);
	this->constantBuffersInfo->Get(0)->Add("ambientLight", 16);
	this->constantBuffersInfo->Get(0)->Add("viewPosition", 12);

	this->vsInputType.ProvideDefinition("", "struct VSInput\r\n{");
	this->vsInputType.ProvideDefinition("pos", "\tfloat3 pos: POSITION;");
	if (this->animated)
	{
		this->vsInputType.ProvideDefinition("boneIndices", "\tfloat4 boneIndices: BLENDINDICES;");
		this->vsInputType.ProvideDefinition("boneWeights", "\tfloat4 boneWeights: BLENDWEIGHT;");
	}

	this->sIOType.ProvideDefinition("", "struct SIO\r\n{");
	this->sIOType.ProvideDefinition("pos", "\tfloat4 pos: SV_POSITION;");

	this->psOutputType.ProvideDefinition("PSOutput", "struct PSOutput\r\n{");
	this->psOutputType.ProvideDefinition("color", "\tfloat4 color: SV_TARGET;");

	this->vsCode.ProvideDefinition("", "SIO VSMain(VSInput input)\r\n{");
	this->vsCode.ProvideDefinition("output", "\tSIO output;\r\n");
	if (this->animated)
	{
		this->vsCode.ProvideDefinition("\tbonesMatrix", "\tfloat4x4 bonesMatrix;");
		this->vsCode.ProvideDefinition("", "\tbonesMatrix = \r\n"
			"\t\tworldMatrix[input.boneIndices.x] * input.boneWeights.x +\r\n"
			"\t\tworldMatrix[input.boneIndices.y] * input.boneWeights.y +\r\n"
			"\t\tworldMatrix[input.boneIndices.z] * input.boneWeights.z +\r\n"
			"\t\tworldMatrix[input.boneIndices.w] * input.boneWeights.w;");

        this->vsCode.ProvideDefinition("worldPos", "\tfloat4 worldPos = mul(bonesMatrix, float4(input.pos, 1.0));\r\n");
	}
	else
	{
        this->vsCode.ProvideDefinition("worldPos", "\tfloat4 worldPos = mul(worldMatrix, float4(input.pos, 1.0));\r\n");
	}
	this->vsCode.ProvideDefinition(".pos", "\toutput.pos = mul(viewProjectionMatrix, worldPos);\r\n");

	this->psCode.ProvideDefinition("", "PSOutput PSMain(SIO input)\r\n{");
	this->psCode.ProvideDefinition("output", "\tPSOutput output;\r\n");
}


AxDynamicShaderWriter::~AxDynamicShaderWriter(void)
{
}

void AxDynamicShaderWriter::AddShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp)
{
	switch (shadingOp)
	{
		case AxShadingOperation_BasicShading:
		{
			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");

			this->sIOType.ProvideDefinition("color", "\tfloat4 color: COLOR;");

			if (this->animated)
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
			else
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");
			this->vsCode.ProvideDefinition("", "\toutput.color = saturate(-normal.z);\r\n");

			this->psCode.ProvideDefinition("", "\toutput.color = input.color;");
			//this->ProvideColorOperationCode(shadingOp, colorOp, "basicShadingAmount", this->psCode, "\t", "output.color", "input.color");
			break;
		}

		case AxShadingOperation_FlatColor:
		{
			AxString pFlatColor = this->ProvideParameterDefinition(shadingOp, "flatColor", AxParameterType_Color);
			this->ProvideColorOperationCode(shadingOp, colorOp, "flatColorAmount", this->psCode, "\t", "output.color", pFlatColor);
			break;
		}

		case AxShadingOperation_Tint:
		{
			AxString pTintColor = this->ProvideParameterDefinition(shadingOp, "tintColor", AxParameterType_Color);

			this->psCode.ProvideDefinition("tintFactor", "float tintFactor;");
			this->psCode.ProvideDefinition("", "\ttintFactor = (output.color.x + output.color.y + output.color.z) / 3.0;");

			this->ProvideColorOperationCode(shadingOp, colorOp, "tintAmount", this->psCode, "\t", "output.color", pTintColor + " * float4(tintFactor, tintFactor, tintFactor, 1.0)");
			break;
		}

		case AxShadingOperation_AmbientLighting:
		{
			//AxString pAmbientColor = this->ProvideParameterDefinition(shadingOp, "ambient", AxParameterType_Color);
			AxString pAmbientColor = "ambient";
			this->ProvideColorOperationCode(shadingOp, colorOp, "ambientAmount", this->psCode, "\t", "output.color", pAmbientColor + " * ambientLight");
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

			this->ProvideParameterDefinition(shadingOp, "activeLightsCount", AxParameterType_Int);
			//this->ProvideParameterDefinition(shadingOp, "diffuse", AxParameterType_Color);
			//this->ProvideParameterDefinition(shadingOp, "specular", AxParameterType_Color);
			//this->ProvideParameterDefinition(shadingOp, "gloss", AxParameterType_Float);

			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");

			this->sIOType.ProvideDefinition("color", "\tfloat4 color: COLOR;");

			if (this->animated)
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
			else
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");

			//Pseudo-lighting:
			//this->vsCode.ProvideDefinition("lightDP", "\tfloat lightDP;");
			//this->vsCode.ProvideDefinition("lightDF", "\tfloat lightDF;");
			//this->vsCode.ProvideDefinition("color", "\tfloat4 color;");
			//this->vsCode.ProvideDefinition("", 
			//	"\tcolor = materialAmbient;\r\n"
			//	"\tfor (int lightIndex = 0; lightIndex < activeLightsCount.x; lightIndex++)\r\n"
			//	"\t{\r\n"
			//	"\t\tlightDP = dot(input.normal, normalize(lightsPosIntensity[lightIndex].xyz - worldPos));\r\n"
			//	"\t\tlightDF = 1.0 - distance(worldPos, lightsPosIntensity[lightIndex].xyz) / lightsPosIntensity[lightIndex].w;\r\n"
			//	"\t\tcolor += lightsColor[lightIndex] * (lightDP * lightDF * materialDiffuse + pow(lightDP, materialGloss.x) * materialSpecular);\r\n"
			//	"\t}\r\n"
			//	);

			this->vsCode.ProvideDefinition("viewVector", "\tfloat3 viewVector = normalize(viewPosition - worldPos.xyz);");
			this->vsCode.ProvideDefinition("lightVector", "\tfloat3 lightVector;");
			this->vsCode.ProvideDefinition("lightReflectionVector", "\tfloat3 lightReflectionVector;");
			this->vsCode.ProvideDefinition("lightAttenuation", "\tfloat lightAttenuation;");
			this->vsCode.ProvideDefinition("lightDiffuse", "\tfloat lightDiffuse;");
			this->vsCode.ProvideDefinition("lightSpecular", "\tfloat lightSpecular;");
			this->vsCode.ProvideDefinition("color", "\tfloat4 color;");
			this->vsCode.ProvideDefinition("", 
				"\tcolor = float4(0.0, 0.0, 0.0, 0.0);\r\n"
				"\tfor (int lightIndex = 0; lightIndex < activeLightsCount.x; lightIndex++)\r\n"
				"\t{\r\n"
				"\t\tlightVector = lightsPosIntensity[lightIndex].xyz - worldPos.xyz;\r\n"
				"\t\tlightAttenuation = 1.0 / length(lightVector);\r\n"
				"\t\tlightVector *= lightAttenuation;\r\n"
				"\t\tlightAttenuation *= lightAttenuation;\r\n"
				"\t\tlightDiffuse = saturate(dot(normal, lightVector)) * lightsPosIntensity[lightIndex].w * lightAttenuation;\r\n"
				"\t\tlightReflectionVector = normalize(lightVector + viewVector);\r\n"
				"\t\tlightSpecular = pow(saturate(dot(normal, lightReflectionVector)), gloss);\r\n"
				"\t\tcolor += lightsColor[lightIndex] * (lightDiffuse * diffuse + lightSpecular * specular);\r\n"
				"\t}\r\n"
				);

			this->vsCode.ProvideDefinition("", "\toutput.color = color;");

			this->ProvideColorOperationCode(shadingOp, colorOp, "vertexLightingAmount", this->psCode, "\t", "output.color", "input.color");
			break;
		}


		case AxShadingOperation_PixelLighting:
		{
			if (this->numLights == 0)
				break;

			if (this->shadingOps.Count(AxShadingOperation_PixelLighting) > 0)
				break;

			this->ProvideParameterDefinition(shadingOp, "activeLightsCount", AxParameterType_Int);
			//this->ProvideParameterDefinition(shadingOp, "diffuse", AxParameterType_Color);
			//this->ProvideParameterDefinition(shadingOp, "specular", AxParameterType_Color);
			//this->ProvideParameterDefinition(shadingOp, "gloss", AxParameterType_Float);

			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");

			this->sIOType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");
			this->sIOType.ProvideDefinition("worldPos", "\tfloat4 worldPos: POSITION;");

			if (this->animated)
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
			else
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");
			this->vsCode.ProvideDefinition(".normal", "\toutput.normal = normal;");
			this->vsCode.ProvideDefinition(".worldPos", "\toutput.worldPos = worldPos;");

			this->psCode.ProvideDefinition("viewVector", "\tfloat3 viewVector = normalize(viewPosition - input.worldPos.xyz);");
			this->psCode.ProvideDefinition("lightVector", "\tfloat3 lightVector;");
			this->psCode.ProvideDefinition("lightReflectionVector", "\tfloat3 lightReflectionVector;");
			this->psCode.ProvideDefinition("lightAttenuation", "\tfloat lightAttenuation;");
			this->psCode.ProvideDefinition("lightDiffuse", "\tfloat lightDiffuse;");
			this->psCode.ProvideDefinition("lightSpecular", "\tfloat lightSpecular;");
			this->psCode.ProvideDefinition("color", "\tfloat4 color;");
			this->psCode.ProvideDefinition("", 
				"\tcolor = float4(0.0, 0.0, 0.0, 0.0);\r\n"
				"\tfor (int lightIndex = 0; lightIndex < activeLightsCount.x; lightIndex++)\r\n"
				"\t{\r\n"
				"\t\tlightVector = lightsPosIntensity[lightIndex].xyz - input.worldPos.xyz;\r\n"
				"\t\tlightAttenuation = 1.0 / length(lightVector);\r\n"
				"\t\tlightVector *= lightAttenuation;\r\n"
				"\t\tlightAttenuation *= lightAttenuation;\r\n"
				"\t\tlightDiffuse = saturate(dot(input.normal, lightVector)) * lightsPosIntensity[lightIndex].w * lightAttenuation;\r\n"
				"\t\tlightReflectionVector = normalize(lightVector + viewVector);\r\n"
				"\t\tlightSpecular = pow(saturate(dot(input.normal, lightReflectionVector)), gloss);\r\n"
				"\t\tcolor += lightsColor[lightIndex] * (lightDiffuse * diffuse + lightSpecular * specular);\r\n"
				"\t}\r\n"
				);

			this->ProvideColorOperationCode(shadingOp, colorOp, "pixelLightingAmount", this->psCode, "\t", "output.color", "color");
			break;
		}

		case AxShadingOperation_ColorMap:
		{
			AxString pColorMap = this->ProvideParameterDefinition(shadingOp, "colorMap", AxParameterType_ReferenceTexture);

			this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");
			
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = input.texCoords;");

			this->ProvideColorOperationCode(shadingOp, colorOp, "colorMapAmount", this->psCode, "\t", "output.color", AxString("t") + pColorMap + ".Sample(s" + pColorMap + ", input.texCoords)");
			break;
		}

		case AxShadingOperation_ReflectionMap:
		{
			AxString pReflectionMap = this->ProvideParameterDefinition(shadingOp, "reflectionMap", AxParameterType_ReferenceTexture);
			AxString pReflectionMapFactor = this->ProvideParameterDefinition(shadingOp, "reflectionMapFactor", AxParameterType_Float);

			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");

			this->sIOType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");
			if (this->sIOType.ProvideDefinition("screenSpaceNormal", AxString("\tfloat3 screenSpaceNormal: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;
			if (this->sIOType.ProvideDefinition("screenSpacePos", AxString("\tfloat3 screenSpacePos: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			if (this->animated)
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
			else
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");
			this->vsCode.ProvideDefinition("wvpNormal", "\tfloat3 wvpNormal = mul(viewProjectionMatrix, float4(normal, 0.0)).xyz;");
			this->vsCode.ProvideDefinition(".normal", "\toutput.normal = normal;");
			this->vsCode.ProvideDefinition(".screenSpaceNormal", "\toutput.screenSpaceNormal = wvpNormal;");
			this->vsCode.ProvideDefinition(".screenSpacePos",
				"\toutput.screenSpacePos = (output.pos.xyz / output.pos.w) * float3(0.5, -0.5, 0.5) + float3(0.5, 0.5, 0.5);\r\n");

			this->ProvideColorOperationCode(shadingOp, colorOp, "reflectionMapAmount", this->psCode, "\t", "output.color", AxString("t") + pReflectionMap + ".Sample(s" + pReflectionMap + ", input.screenSpacePos.xy - input.normal.xy * " + pReflectionMapFactor + ")");
			break;
		}

		case AxShadingOperation_RefractionMap:
		{
			AxString pRefractionMap = this->ProvideParameterDefinition(shadingOp, "refractionMap", AxParameterType_ReferenceTexture);
			AxString pRefractionMapFactor = this->ProvideParameterDefinition(shadingOp, "refractionMapFactor", AxParameterType_Float);

			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");

			this->sIOType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");
			if (this->sIOType.ProvideDefinition("screenSpaceNormal", AxString("\tfloat3 screenSpaceNormal: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;
			if (this->sIOType.ProvideDefinition("screenSpacePos", AxString("\tfloat3 screenSpacePos: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;


			if (this->animated)
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
			else
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");
			this->vsCode.ProvideDefinition("wvpNormal", "\tfloat3 wvpNormal = mul(viewProjectionMatrix, float4(normal, 0.0));");
			this->vsCode.ProvideDefinition(".normal", "\toutput.normal = normal;");
			this->vsCode.ProvideDefinition(".screenSpaceNormal", "\toutput.screenSpaceNormal = wvpNormal;");
			this->vsCode.ProvideDefinition(".screenSpacePos",
				"\toutput.screenSpacePos = (output.pos.xyz / output.pos.w) * float3(0.5, -0.5, 0.5) + float3(0.5, 0.5, 0.5);\r\n");

			//this->psCode.ProvideDefinition("", AxString("\t") + this->GetColorOperationCode("output.color", "tRefractionMap.Sample(sRefractionMap, input.screenSpacePos.xy - input.screenSpaceNormal.xy * refractionFactor)", colorOp));
			//this->psCode.ProvideDefinition("", AxString("\t") + this->GetColorOperationCode("output.color", "tRefractionMap.Sample(sRefractionMap, input.screenSpacePos.xy - input.normal.xy * refractionFactor)", colorOp));
			this->ProvideColorOperationCode(shadingOp, colorOp, "refractionMapAmount", this->psCode, "\t", "output.color", AxString("t") + pRefractionMap + ".Sample(s" + pRefractionMap + ", input.screenSpacePos.xy - input.normal.xy * " + pRefractionMapFactor + ")");
			break;
		}

		case AxShadingOperation_NormalMap:
		{
			AxString pNormalMap = this->ProvideParameterDefinition(shadingOp, "normalMap", AxParameterType_ReferenceTexture);
			AxString pNormalMapFactor = this->ProvideParameterDefinition(shadingOp, "normalMapFactor", AxParameterType_Float);

			this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");
			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");
			this->vsInputType.ProvideDefinition("tangent", "\tfloat3 tangent: TANGENT;");
			this->vsInputType.ProvideDefinition("biTangent", "\tfloat3 biTangent: BINORMAL;");
			
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;
			this->sIOType.ProvideDefinition("normal", AxString("\tfloat3 normal: NORMAL;"));
			this->sIOType.ProvideDefinition("tangent", AxString("\tfloat3 tangent: TANGENT;"));
			this->sIOType.ProvideDefinition("biTangent", AxString("\tfloat3 biTangent: BINORMAL;"));

			if (this->animated)
			{
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
				this->vsCode.ProvideDefinition("tangent", "\tfloat3 tangent = normalize(mul(bonesMatrix, float4(input.tangent, 0.0)).xyz);");
				this->vsCode.ProvideDefinition("biTangent", "\tfloat3 biTangent = normalize(mul(bonesMatrix, float4(input.biTangent, 0.0)).xyz);");
			}
			else
			{
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");
				this->vsCode.ProvideDefinition("tangent", "\tfloat3 tangent = normalize(mul(worldMatrix, float4(input.tangent, 0.0)).xyz);");
				this->vsCode.ProvideDefinition("biTangent", "\tfloat3 biTangent = normalize(mul(worldMatrix, float4(input.biTangent, 0.0)).xyz);");
			}
			
			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = input.texCoords;");
			this->vsCode.ProvideDefinition(".normal", "\toutput.normal = normal;");
			this->vsCode.ProvideDefinition(".tangent", "\toutput.tangent = tangent;");
			this->vsCode.ProvideDefinition(".biTangent", "\toutput.biTangent = biTangent;");
			
			this->psCode.ProvideDefinition("mapNormal",  "\tfloat3 mapNormal;");
			this->psCode.ProvideDefinition("",  
				AxString("\tmapNormal = (t") + pNormalMap + ".Sample(s" + pNormalMap + ", input.texCoords).xyz * 2.0 - 1.0) * " + pNormalMapFactor + ";\r\n"
				"\tinput.normal = mapNormal.x * input.tangent + mapNormal.y * input.biTangent + mapNormal.z * input.normal;\r\n");

			break;
		}

		case AxShadingOperation_BumpMap:
		{
			AxString pBumpMap = this->ProvideParameterDefinition(shadingOp, "bumpMap", AxParameterType_ReferenceTexture);
			AxString pBumpMapFactor = this->ProvideParameterDefinition(shadingOp, "bumpMapFactor", AxParameterType_Float);

			this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");
			this->vsInputType.ProvideDefinition("normal", "\tfloat3 normal: NORMAL;");

			this->sIOType.ProvideDefinition("normal", AxString("\tfloat3 normal: NORMAL;"));
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			if (this->animated)
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(bonesMatrix, float4(input.normal, 0.0)).xyz);");
			else
				this->vsCode.ProvideDefinition("normal", "\tfloat3 normal = normalize(mul(worldMatrix, float4(input.normal, 0.0)).xyz);");
			this->vsCode.ProvideDefinition(".normal", "\toutput.normal = normal;");
			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = input.texCoords;");
			
			this->psCode.ProvideDefinition("bumpMapNormal",  "\tfloat3 bumpMapNormal;");
			this->psCode.ProvideDefinition("",  
				AxString("\tbumpMapNormal = normalize(t") + pBumpMap + ".Sample(s" + pBumpMap + ", input.texCoords)).xyz * " + pBumpMapFactor + " * 2.0 - 1.0;\r\n"
				"\tinput.normal += bumpMapNormal;\r\n");
				//"\t" + this->GetColorOperationCode("input.normal", "mapNormal", colorOp) + "\r\n");

			break;
		}

		case AxShadingOperation_BlurMap:
		{
			AxString pBlurMap = this->ProvideParameterDefinition(shadingOp, "blurMap", AxParameterType_ReferenceTexture);
			AxString pBlurMapFactor = this->ProvideParameterDefinition(shadingOp, "blurMapFactor", AxParameterType_Float);

            this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");
			
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = input.texCoords;");

            this->psCode.ProvideDefinition("blurSum", "\tfloat4 blurSum;");
            this->psCode.ProvideDefinition("", 
				AxString("\tblurSum = t") + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords);\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2(-0.326212, -0.405805) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2(-0.840144, -0.073580) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2(-0.695914,  0.457137) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2(-0.203345,  0.620716) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2( 0.962340, -0.194983) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2( 0.473434, -0.480026) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2( 0.519456,  0.767022) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2( 0.185461, -0.893124) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2( 0.507431,  0.064425) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2( 0.896420,  0.412458) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2(-0.321940, -0.932615) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pBlurMap + ".Sample(s" + pBlurMap + ", input.texCoords + float2(-0.791559, -0.597705) * " + pBlurMapFactor + ");\r\n"
				"\tblurSum /= 13.0;\r\n");
			this->ProvideColorOperationCode(shadingOp, colorOp, "blurMapAmount", this->psCode, "\t", "output.color", "blurSum");
			break;
		}

		case AxShadingOperation_HBlur:
		{
			AxString pHBlurMap = this->ProvideParameterDefinition(shadingOp, "hBlurMap", AxParameterType_ReferenceTexture);
			AxString pHBlurMapFactor = this->ProvideParameterDefinition(shadingOp, "hBlurMapFactor", AxParameterType_Float);

            this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");
			
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = input.texCoords;");

            this->psCode.ProvideDefinition("blurSum", "\tfloat4 blurSum;");

			this->psCode.ProvideDefinition("", 
				AxString("\tblurSum = t") + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords);\r\n"
				"\tblurSum += t" + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords + float2(-0.33, 0.0) * " + pHBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords + float2(-0.66, 0.0) * " + pHBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords + float2(-0.99, 0.0) * " + pHBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords + float2(0.33, 0.0) * " + pHBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords + float2(0.66, 0.0) * " + pHBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pHBlurMap + ".Sample(s" + pHBlurMap + ", input.texCoords + float2(0.99, 0.0) * " + pHBlurMapFactor + ");\r\n"
				"\tblurSum /= 7.0;");

			this->ProvideColorOperationCode(shadingOp, colorOp, "hBlurMapAmount", this->psCode, "\t", "output.color", "blurSum");
			break;
		}

		case AxShadingOperation_VBlur:
		{
			AxString pVBlurMap = this->ProvideParameterDefinition(shadingOp, "vBlurMap", AxParameterType_ReferenceTexture);
			AxString pVBlurMapFactor = this->ProvideParameterDefinition(shadingOp, "vBlurMapFactor", AxParameterType_Float);

            this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");
			
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = input.texCoords;");

            this->psCode.ProvideDefinition("blurSum", "\tfloat4 blurSum;");

			this->psCode.ProvideDefinition("", 
				AxString("\tblurSum = t") + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords);\r\n"
				"\tblurSum += t" + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords + float2(0.0, -0.33) * " + pVBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords + float2(0.0, -0.66) * " + pVBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords + float2(0.0, -0.99) * " + pVBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords + float2(0.0, 0.33) * " + pVBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords + float2(0.0, 0.66) * " + pVBlurMapFactor + ");\r\n"
				"\tblurSum += t" + pVBlurMap + ".Sample(s" + pVBlurMap + ", input.texCoords + float2(0.0, 0.99) * " + pVBlurMapFactor + ");\r\n"
				"\tblurSum /= 7.0;");

			this->ProvideColorOperationCode(shadingOp, colorOp, "vBlurMapAmount", this->psCode, "\t", "output.color", "blurSum");
			break;
		}

		case AxShadingOperation_AlignScreenQuad:
		{
			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition("screenPos", "\tfloat2 screenPos;");
			this->vsCode.ProvideDefinition("",
				"\tscreenPos = sign(input.pos.xy);\r\n"
				"\toutput.pos = float4(screenPos, 0.999, 1.0);\r\n");
				//"\toutput.pos = float4(screenPos, 0.001, 1.0);\r\n"
				//"\toutput.pos.y = -output.pos.y; // remove this, instead change vertex order");
               
			this->vsCode.ProvideDefinition(".texCoords", "\toutput.texCoords = screenPos * float2(0.5, -0.5) + float2(0.5, 0.5);");

			break;
		}
		/*

		case AxShadingOperation_ProjectTexture:
		{
			if (this->various.ProvideDefinition("projectionMap", 
				AxString("Texture2D tProjectionMap: register(t") + AxString(this->textureBuffersInfo->count) + ");\r\n"
				"sampler sProjectionMap: register(s" + AxString(this->textureBuffersInfo->count) + ");\r\n"))
			{
				this->textureBuffersInfo->Add("projectionMap");
			}

			if (this->parameters.ProvideDefinition("projectionCamera", "\tfloat4x4 projectionCamera;"))
			{
				this->constantBuffersInfo->Get(0)->Add("projectionCamera", 64);
			}

			if (this->sIOType.ProvideDefinition("projectionTexCoords", AxString("\tfloat2 projectionTexCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition("projectionCoords", "\tfloat4 projectionCoords;");
			this->vsCode.ProvideDefinition("", "\tprojectionCoords = mul(projectionCamera, worldPos);");
			this->vsCode.ProvideDefinition(".projectionTexCoords", "\toutput.projectionTexCoords = ((projectionCoords.xy / projectionCoords.w) + 1.0) / 2.0;");

			this->ProvideColorOperationCode(this->psCode, "\t", "output.color", "tProjectionMap.Sample(sProjectionMap, input.projectionTexCoords)", "textureProjectionAmount", colorOp);
			break;
		}

		case AxShadingOperation_ProjectPicture:
		{
			if (this->various.ProvideDefinition("projectionMap", 
				AxString("Texture2D tProjectionMap: register(t") + AxString(this->textureBuffersInfo->count) + ");\r\n"
				"sampler sProjectionMap: register(s" + AxString(this->textureBuffersInfo->count) + ");\r\n"))
			{
				this->textureBuffersInfo->Add("projectionMap");
			}

			if (this->parameters.ProvideDefinition("projectionCamera", "\tfloat4x4 projectionCamera;"))
			{
				this->constantBuffersInfo->Get(0)->Add("projectionCamera", 64);
			}

			if (this->sIOType.ProvideDefinition("projectionTexCoords", AxString("\tfloat2 projectionTexCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition("projectionCoords", "\tfloat4 projectionCoords;");
			this->vsCode.ProvideDefinition("", "\tprojectionCoords = mul(projectionCamera, worldPos);");
			this->vsCode.ProvideDefinition(".projectionTexCoords", "\toutput.projectionTexCoords = ((projectionCoords.xy / projectionCoords.w) + 1.0) / 2.0;");

			this->psCode.ProvideDefinition("", "\tif ((input.projectionTexCoords.x > 0.0) && (input.projectionTexCoords.x < 1.0) && (input.projectionTexCoords.y > 0.0) && (input.projectionTexCoords.y < 1.0))\r\n\t{");
			this->ProvideColorOperationCode(this->psCode, "\t\t", "output.color", "tProjectionMap.Sample(sProjectionMap, input.projectionTexCoords)", "pictureProjectionAmount", colorOp);
			this->psCode.ProvideDefinition("", "\t}\r\n");
			break;
		}
		*/

		case AxShadingOperation_TextureTransform:
		{
			AxString pTextureTransform = this->ProvideParameterDefinition(shadingOp, "textureTransform", AxParameterType_ReferenceTransform);

            this->vsInputType.ProvideDefinition("texCoords", "\tfloat2 texCoords: TEXCOORD0;");

			if (this->sIOType.ProvideDefinition("texCoords", AxString("\tfloat2 texCoords: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->psCode.ProvideDefinition("", AxString("\tinput.texCoords = mul(") + pTextureTransform + ", float4(input.texCoords, 0, 1)).xy;");
			break;
		}

		case AxShadingOperation_Skinning:
		{
			break;
		}

		case AxShadingOperation_Material:
		{
			this->ProvideParameterDefinition(shadingOp, "ambient", AxParameterType_Color);
			this->ProvideParameterDefinition(shadingOp, "diffuse", AxParameterType_Color);
			this->ProvideParameterDefinition(shadingOp, "specular", AxParameterType_Color);
			this->ProvideParameterDefinition(shadingOp, "gloss", AxParameterType_Float);

			break;
		}

		case AxShadingOperation_Fog:
		{
			AxString pFogColor = this->ProvideParameterDefinition(shadingOp, "fogColor", AxParameterType_Color);
			AxString pFogStart = this->ProvideParameterDefinition(shadingOp, "fogStart", AxParameterType_Float);
			AxString pFogDensity = this->ProvideParameterDefinition(shadingOp, "fogDensity", AxParameterType_Float);

			if (this->sIOType.ProvideDefinition("fogImmersion", AxString("\tfloat fogImmersion: TEXCOORD") + AxString(this->texCoordSlots) + ";"))
				this->texCoordSlots++;

			this->vsCode.ProvideDefinition("distance", "\tfloat distance = length(viewPosition - worldPos.xyz);");
			this->vsCode.ProvideDefinition(".fogImmersion", AxString("\toutput.fogImmersion = saturate((distance - ") + pFogStart + ") / " + pFogDensity + ");");

			this->psCode.ProvideDefinition("fogColorAlpha", 
				AxString("\tfloat4 fogColorAlpha = ") + pFogColor + ";\r\n"
				"\tfogColorAlpha.w = input.fogImmersion;");

			this->ProvideColorOperationCode(shadingOp, colorOp, "fogAmount", this->psCode, "\t", "output.color", "fogColorAlpha");

			break;
		}

	}

	this->shadingOps.Add(shadingOp);
}

AxString AxDynamicShaderWriter::GetSourceCode()
{
	if (this->numLights > 0)
	{
		AxString numLightsStr(this->numLights);
		this->parameters.ProvideDefinition("",AxString(
			"\tfloat4 lightsPosIntensity[") + numLightsStr + "];\r\n" +
			"\tfloat4 lightsColor[" + numLightsStr + "];\r\n");
		// lightsPos and lightsIntensity share the same shader constant - lightsPosIntensity.
		this->constantBuffersInfo->Get(0)->Add("lightsIntensity", 4, this->numLights, 12, true);
		this->constantBuffersInfo->Get(0)->Add("lightsPos", 12, this->numLights, 0, true);
		this->constantBuffersInfo->Get(0)->Add("lightsColor", 16, this->numLights, 0, true);
	}

	return AxString(
		AxString("// Generated by Axis HLSL dynamic shader writer\r\n\r\n") +

		this->various.GetCode() + "\r\n" +

		this->parameters.GetCode() + "};\r\n\r\n" +

		this->vsInputType.GetCode() + "};\r\n\r\n" +

		this->sIOType.GetCode() + "};\r\n\r\n" +

		this->psOutputType.GetCode() + "};\r\n\r\n" +

		this->vsCode.GetCode() + "\r\n\treturn output;\r\n}\r\n\r\n" +

		this->psCode.GetCode() + "\r\n\treturn output;\r\n}");
}



// Private methods



AxString AxDynamicShaderWriter::ProvideParameterDefinition(AxShadingOperation shadingOp, const AxString &desiredName, AxParameterType type, int elementsCount, bool isArray, int offset)
{
	int shadingOpsCount = this->shadingOps.Count(shadingOp);
	AxString name = shadingOpsCount == 0 ? AxString(desiredName.contents) : AxString(AxString(desiredName.contents) + AxString(shadingOpsCount));

	isArray = isArray || (elementsCount > 1);

	if (type == AxParameterType_ReferenceTexture)
	{
		if (this->various.ProvideDefinition(name.contents, 
			AxString("Texture2D t") + name + ": register(t" + AxString(this->textureBuffersInfo->count) + ");\r\n"
			"sampler s" + name + ": register(s" + AxString(this->textureBuffersInfo->count) + ");\r\n"))
		{
			this->textureBuffersInfo->Add(name.contents);
		}

		return AxString(name);
	}

	AxString typeName;
	int typeSize = 0;
	switch (type)
	{
		case AxParameterType_Int: 
		{ 
			typeName = "int";
			typeSize = 4;
			break; 
		}
		case AxParameterType_Float:
		{ 
			typeName = "float";
			typeSize = 4;
			break; 
		}
		case AxParameterType_Bool:
		{ 
			typeName = "bool";
			typeSize = 1;
			break; 
		}
		case AxParameterType_Color:
		{ 
			typeName = "float4";
			typeSize = 16;
			break; 
		}
		case AxParameterType_ReferenceVector3:
		{ 
			typeName = "float3";
			typeSize = 12;
			break; 
		}
		case AxParameterType_ReferenceCamera:
		{ 
			typeName = "float4x4";
			typeSize = 64;
			break; 
		}
		case AxParameterType_ReferenceTransform:
		{ 
			typeName = "float4x4";
			typeSize = 64;
			break; 
		}
	}

	if (this->parameters.ProvideDefinition(name.contents, AxString("\t") + typeName + " " + name + ";"))
	{
		this->constantBuffersInfo->Get(0)->Add(name.contents, typeSize, elementsCount, offset, isArray);
	}

	return AxString(name);
}

void AxDynamicShaderWriter::ProvideColorOperationCode(AxShadingOperation shadingOp, AxColorOperation colorOp, const AxString &desiredFactorName, AxDynamicShaderBlockDefinitions &definitions, AxString linePrefix, AxString destArg, AxString sourceArg)
{
	switch (colorOp)
	{
		case AxColorOperation_None:
			break;

		case AxColorOperation_Set:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("", linePrefix + destArg + " = " + sourceArg + " * " + factor + ";");

			break;
		}

		case AxColorOperation_Blend:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			//definitions.ProvideDefinition("", linePrefix + destArg + " += ((" + sourceArg + ") - " + destArg + ") * " + factor + ";");
			definitions.ProvideDefinition("", linePrefix + destArg + " = lerp(" + destArg + ", (" + sourceArg + "), " + factor + ");");

			break;
		}

		case AxColorOperation_Add:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("", linePrefix + destArg + " += (" + sourceArg + ") * " + factor + ";");

			break;
		}

		case AxColorOperation_Subtract:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("", linePrefix + destArg + " -= (" + sourceArg + ") * " + factor + ";");

			break;
		}

		case AxColorOperation_Modulate:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("", linePrefix + destArg + " *= (" + sourceArg + ") * " + factor + ";");

			break;
		}

		case AxColorOperation_Lighter:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("", linePrefix + destArg + " = max(" + destArg + ", " + sourceArg + " * " + factor + ");");

			break;
		}

		case AxColorOperation_Darker:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("", linePrefix + destArg + " = min(" + destArg + ", " + sourceArg + " * " + factor + ");");

			break;
		}

		case AxColorOperation_AlphaBlend:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("colorOpArg", linePrefix + "float4 colorOpArg;\r\n");
			definitions.ProvideDefinition("", 
				linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
				linePrefix + destArg + " = lerp(" + destArg + ", colorOpArg, colorOpArg.w * " + factor + ");");

			break;
		}

		case AxColorOperation_AlphaAdd:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("colorOpArg", linePrefix + "float4 colorOpArg;\r\n");
			definitions.ProvideDefinition("", 
				linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
				linePrefix + destArg + " += " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");

			break;
		}

		case AxColorOperation_AlphaSubtract:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("colorOpArg", linePrefix + "float4 colorOpArg;\r\n");
			definitions.ProvideDefinition("", 
				linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
				linePrefix + destArg + " -= " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");

			break;
		}

		case AxColorOperation_AlphaModulate:
		{
			AxString factor = this->ProvideParameterDefinition(shadingOp, desiredFactorName, AxParameterType_Float);
			definitions.ProvideDefinition("colorOpArg", linePrefix + "float4 colorOpArg;\r\n");
			definitions.ProvideDefinition("", 
				linePrefix + "colorOpArg = " + sourceArg + ";\r\n" +
				linePrefix + destArg + " *= " + sourceArg + " * " + sourceArg + ".w * " + factor + ";");

			break;
		}

		default:
		{
			definitions.ProvideDefinition("", linePrefix + destArg + " = 0;");

			break;
		}
	}
}



// Static methods



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

