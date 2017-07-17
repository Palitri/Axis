//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGLShader.h"

#include "AxOpenGLGraphicsDevice.h"
#include "AxOpenGLTexture2D.h"

#include <gl\GL.h>

AxOpenGLShader::AxOpenGLShader(void)
	: AxDeviceShader()
{
    this->context = 0;
    
    this->glShaderProgram = 0;

	this->glVertexShader = 0;
	this->glPixelShader = 0;
    
    this->vertexPositionAttribute = -1;
    this->vertexTexCoordsAttribute = -1;
    this->vertexNormalAttribute = -1;
    this->vertexBoneIndicesAttribute = -1;
    this->vertexBoneWeightsAttribute = -1;
    this->vertexTangentsAttribute = -1;
    this->vertexBitangentsAttribute = -1;
    
	this->worldMatrixAddress = AxDeviceShader::invalidParameterAddress;
    this->viewProjectionMatrixAddress = AxDeviceShader::invalidParameterAddress;
    this->lightsPosIntensityAddress = AxDeviceShader::invalidParameterAddress;
    this->lightsColorAddress = AxDeviceShader::invalidParameterAddress;
    
    this->setTextureIndex = 0;
    
    this->bones = new GLfloat[16 * AxOpenGLShader::maxBones];
    this->isSkeletalAnimated = false;
    
    this->lightsPosIntensity = new GLfloat[4 * AxOpenGLShader::maxLights];
    this->lightsColor = new GLfloat[4 * AxOpenGLShader::maxLights];
    
    // Used for general purposes, for conversion between AxMatrix and GL"s matrix format
    this->glMatrix = new GLfloat[16];

	this->shaderWriter = 0;
}

AxOpenGLShader::~AxOpenGLShader(void)
{
	if (this->shaderWriter != 0)
		delete this->shaderWriter;

	delete[] this->glMatrix;
	delete[] this->lightsColor;
	delete[] this->lightsPosIntensity;
	delete[] this->bones;
}

int AxOpenGLShader::GetOperationCount()
{
    return this->shadingOps.count;
};

void AxOpenGLShader::AddOperation(AxShadingOperation shadingOp, AxColorOperation colorOp)
{
    AxOpenGLMaterialShadingOperation shadingOperation;
    shadingOperation.shadingOp = shadingOp;
    shadingOperation.colorOp = colorOp;

    this->shadingOps.Add(shadingOperation);    
};

void AxOpenGLShader::GetOperation(int index, AxShadingOperation &shadingOp, AxColorOperation &colorOp)
{
	shadingOp = this->shadingOps.Get(index).shadingOp;
	colorOp = this->shadingOps.Get(index).colorOp;
};

void AxOpenGLShader::SetOperation(int index, AxShadingOperation shadingOp, AxColorOperation colorOp)
{
    this->shadingOps.Get(index).shadingOp = shadingOp;
    this->shadingOps.Get(index).colorOp = colorOp;
};

void AxOpenGLShader::ClearOperations()
{
    this->shadingOps.Clear();
};


int AxOpenGLShader::GetParameterAddress(AxString parameterName)
{
    if (parameterName.Equals("View matrix"))
        return AxOpenGLShader::viewMatrixAddress;
    if (parameterName.Equals("Projection matrix"))
        return AxOpenGLShader::projectionMatrixAddress;
    if ((this->isSkeletalAnimated) && (parameterName.Equals("World matrix")))
        return AxOpenGLShader::worldMatrixFakeAddress;
    if (parameterName.Equals("Lights pos"))
        return AxOpenGLShader::lightsPosFakeAddress;
    if (parameterName.Equals("Lights intensity"))
        return AxOpenGLShader::lightsIntensityFakeAddress;
    if (parameterName.Equals("Lights color"))
        return AxOpenGLShader::lightsColorFakeAddress;

    AxString parameterInShaderName = AxDynamicShaderWriter::PropertyNameToParameterName(parameterName);
    GLuint parameterInShaderAddress = glGetUniformLocation(this->glShaderProgram, parameterInShaderName.GetCharContents());

	if ((parameterInShaderAddress == 0xffffffff) || (parameterInShaderAddress == GL_INVALID_VALUE) || (parameterInShaderAddress == GL_INVALID_OPERATION))
		return AxDeviceShader::invalidParameterAddress;

	AxParameterType parameterType = AxParameterType_None;

	int parameterDefinitionIndex = this->shaderWriter->vsParams->GetDefinitionIndex(parameterInShaderName);
	if (parameterDefinitionIndex != -1)
	{
		parameterType = (AxParameterType)this->shaderWriter->vsParams->definitions[parameterDefinitionIndex].type;
	}
	else
	{
		parameterDefinitionIndex = this->shaderWriter->psParams->GetDefinitionIndex(parameterInShaderName);
		if (parameterDefinitionIndex != -1)
		{
			parameterType = (AxParameterType)this->shaderWriter->psParams->definitions[parameterDefinitionIndex].type;
		}
	}
    
	return (parameterInShaderAddress << 20) | (parameterType & ((1 << 12) - 1));
}

bool AxOpenGLShader::SetFakeParameter(int address, int index, void *value)
{
    if (!(address < -1))
        return false;
    
    if (address == AxOpenGLShader::viewMatrixAddress)
    {
        AxMatrix::Copy(this->viewMatrix, *(AxMatrix*)value);
        return true;
    }
    
    if (address == AxOpenGLShader::projectionMatrixAddress)
    {
        AxMatrix::Copy(this->projectionMatrix, *(AxMatrix*)value);
        return true;
    }
    
    if ((address == AxOpenGLShader::worldMatrixFakeAddress) && (this->isSkeletalAnimated))
    {
        // Apply this->bones at graphics device"s SetShader
        if (index < AxOpenGLShader::maxBones)
            AxOpenGLShader::AxMatrixToGLUniform(*(AxMatrix*)value, this->bones, index * 16);
        return true;
    }
    
    if (address == AxOpenGLShader::lightsPosFakeAddress)
    {
        if (index < AxOpenGLShader::maxLights)
            AxOpenGLShader::AxVector3ToGLUniform(*(AxVector3*)value, this->lightsPosIntensity, index * 4);
        return true;
    }
    
    if (address == AxOpenGLShader::lightsIntensityFakeAddress)
    {
        if (index < AxOpenGLShader::maxLights)
            this->lightsPosIntensity[index * 4 + 3] = *(float*)value;
        return true;
    }
    
    if (address == AxOpenGLShader::lightsColorFakeAddress)
    {
        if (index < AxOpenGLShader::maxLights)
            AxOpenGLShader::AxVector4ToGLUniform(*(AxVector4*)value, this->lightsColor, index * 4);
        return true;
    }
    
    return false;
};

void AxOpenGLShader::SetParameter(int address, int index, void *value)
{
	if (address == AxDeviceShader::invalidParameterAddress)
		return;
	
	if (this->SetFakeParameter(address, index, value))
        return;

	AxParameterType parameterType = (AxParameterType)(address & ((1 << 12) - 1));
	address >>= 20;
    
    switch (parameterType)
	{
		case AxParameterType_Float:
		{
			float *val = (float*)value;
	        glUniform1f(address, *val);
	        //glUniform1fv(address, 1, (GLfloat*)value);
			break;
		}

		case AxParameterType_Int:
		{
			//int *val = (int*)value;
	  //      glUniform1i(address, *val);
	        glUniform1iv(address, 1, (GLint*)value);
			break;
		}

		case AxParameterType_ReferenceVector3:
		{
			//AxVector3 *val = (AxVector3*)value;
			//glUniform3f(address, val->x, val->y, val->z);
			glUniform3fv(address, 1, (GLfloat*)value);
			break;
		}

		case AxParameterType_Color:
		{
			//AxVector4 *val = (AxVector4*)value;
			//glUniform4f(address, val->x, val->y, val->z, val->w);
			glUniform4fv(address, 1, (GLfloat*)value);
			break;
		}

		case AxParameterType_ReferenceTransform:
		case AxParameterType_ReferenceCamera:
		{
			//AxOpenGLShader::AxMatrixToGLUniform(*(AxMatrix*)value, this->glMatrix, 0);
			//glUniformMatrix4fv(address, 1, false, this->glMatrix);
			glUniformMatrix4fv(address, 1, false, (GLfloat*)value);
			break;
		}

		case AxParameterType_ReferenceTexture:
		{
			AxOpenGLTexture2D *val = (AxOpenGLTexture2D*)value;

			glActiveTexture(GL_TEXTURE0 + this->setTextureIndex);
			glBindTexture(GL_TEXTURE_2D, val->glTexture);
			glUniform1i(address, this->setTextureIndex);
			this->setTextureIndex++;
			break;
		}

		default:
			{
				value = 0;
			return;
			}
	}
}

bool AxOpenGLShader::Update(AxString &compilationMessage)
{
    this->Release();
    
    this->isSkeletalAnimated = this->IndexOf(AxShadingOperation_Skinning) != -1;

	if (this->shaderWriter != 0)
		delete this->shaderWriter;

	this->shaderWriter = new AxDynamicShaderWriter(this->isSkeletalAnimated, AxOpenGLShader::maxBones);

    for (int i = 0; i < this->shadingOps.count; i++)
        this->shaderWriter->AddShadingLayer(this->shadingOps.Get(i).shadingOp, this->shadingOps.Get(i).colorOp);

    AxString vertexShader = this->shaderWriter->GetVSSourceCode();
    AxString pixelShader = this->shaderWriter->GetPSSourceCode();
    
    if (!this->CompileShader(vertexShader, pixelShader))
        return false;
    
    return true;
}

bool AxOpenGLShader::CompileShader(AxString &vertexShaderCode, AxString &pixelShaderCode)
{
	if (this->glVertexShader == 0)
		this->glVertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char *code = vertexShaderCode.GetCharContents();
	GLint codeLength = vertexShaderCode.length;
	glShaderSource(this->glVertexShader, 1, &code, &codeLength);
    glCompileShader(this->glVertexShader);
	if (!AxOpenGLShader::GetShaderCompileStatus(this->glVertexShader, this->compilationMessage))
    {
        this->compilationMessage = AxString("Vertex shader error:\r\n") + this->compilationMessage;
        return false;
    }
    
    
    
    if (this->glPixelShader == 0)
		this->glPixelShader = glCreateShader(GL_FRAGMENT_SHADER);
	code = pixelShaderCode.GetCharContents();
	codeLength = pixelShaderCode.length;
	glShaderSource(this->glPixelShader, 1, &code, &codeLength);
    glCompileShader(this->glPixelShader);
	if (!AxOpenGLShader::GetShaderCompileStatus(this->glPixelShader, this->compilationMessage))
    {
        this->compilationMessage = AxString("Pixel shader error:\r\n") + this->compilationMessage;
        return false;
    }
    
    
    
    if (this->glShaderProgram == 0)
		this->glShaderProgram = glCreateProgram();
    glAttachShader(this->glShaderProgram, this->glVertexShader);
    glAttachShader(this->glShaderProgram, this->glPixelShader);
    glLinkProgram(this->glShaderProgram);
	if (!AxOpenGLShader::GetShaderProgramCompileStatus(this->glShaderProgram, this->compilationMessage)) 
    {
        this->compilationMessage = AxString("Linking shader error:\r\n") + this->compilationMessage;
        return false;
    }
    
    
    
    this->vertexPositionAttribute = glGetAttribLocation(this->glShaderProgram, "input_pos");
    this->vertexTexCoordsAttribute = glGetAttribLocation(this->glShaderProgram, "input_texCoords");
    this->vertexNormalAttribute = glGetAttribLocation(this->glShaderProgram, "input_normal");
    this->vertexBoneIndicesAttribute = glGetAttribLocation(this->glShaderProgram, "input_boneIndices");
    this->vertexBoneWeightsAttribute = glGetAttribLocation(this->glShaderProgram, "input_boneWeights");
    this->vertexTangentsAttribute = glGetAttribLocation(this->glShaderProgram, "input_tangent");
    this->vertexBitangentsAttribute = glGetAttribLocation(this->glShaderProgram, "input_biTangent");
    
    // The viewProjectionMatrix parameter is actually real, but needs special treatment, because Axis sets view and projection separately - for optimization
	this->viewProjectionMatrixAddress = this->GetParameterAddress(AxString("View projection matrix"));
    // The following parameters are used internally and GetParameterAddress returns fake addresses for them
    this->worldMatrixAddress = glGetUniformLocation(this->glShaderProgram, "worldMatrix");
    this->lightsPosIntensityAddress = glGetUniformLocation(this->glShaderProgram, "lightsPosIntensity");
    this->lightsColorAddress = glGetUniformLocation(this->glShaderProgram, "lightsColor");

    return true;
};

void AxOpenGLShader::Release()
{
};

int AxOpenGLShader::IndexOf(AxShadingOperation shadingOp)
{
    for (int i = 0; i < this->shadingOps.count; i++)
        if (this->shadingOps.Get(i).shadingOp == shadingOp)
            return i;

    return -1;
};

void AxOpenGLShader::AxMatrixToGLUniform(AxMatrix &axMatrix, float *glUniform, int offset)
{
    glUniform[offset + 0] = axMatrix._11;
    glUniform[offset + 1] = axMatrix._12;
    glUniform[offset + 2] = axMatrix._13;
    glUniform[offset + 3] = axMatrix._14;
    glUniform[offset + 4] = axMatrix._21;
    glUniform[offset + 5] = axMatrix._22;
    glUniform[offset + 6] = axMatrix._23;
    glUniform[offset + 7] = axMatrix._24;
    glUniform[offset + 8] = axMatrix._31;
    glUniform[offset + 9] = axMatrix._32;
    glUniform[offset + 10] = axMatrix._33;
    glUniform[offset + 11] = axMatrix._34;
    glUniform[offset + 12] = axMatrix._41;
    glUniform[offset + 13] = axMatrix._42;
    glUniform[offset + 14] = axMatrix._43;
    glUniform[offset + 15] = axMatrix._44;    
}

void AxOpenGLShader::AxVector3ToGLUniform(AxVector3 &axVector3, float *glUniform, int offset)
{
    glUniform[offset + 0] = axVector3.x;
    glUniform[offset + 1] = axVector3.y;
    glUniform[offset + 2] = axVector3.z;
}

void AxOpenGLShader::AxVector4ToGLUniform(AxVector4 &axVector4, float *glUniform, int offset)
{
    glUniform[offset + 0] = axVector4.x;
    glUniform[offset + 1] = axVector4.y;
    glUniform[offset + 2] = axVector4.z;
    glUniform[offset + 3] = axVector4.w;
}

bool AxOpenGLShader::GetShaderCompileStatus(GLuint shader, AxString &statusMessage)
{
	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (compileStatus == GL_TRUE)
		return true;

	GLint infoBufferLength;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoBufferLength);
	infoBufferLength = 1024;

	GLint infoLength; // size of character string (without the null termination char)
	GLchar *info = new GLchar[infoBufferLength];
	glGetShaderInfoLog(shader, infoBufferLength, &infoLength, info);

	statusMessage = info;
	delete[] info;

	return false;
}

bool AxOpenGLShader::GetShaderProgramCompileStatus(GLuint shaderProgram, AxString &statusMessage)
{
	GLint linkStatus;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_TRUE)
		return true;

	GLint infoBufferLength;
	glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoBufferLength);

	GLint infoLength; // size of character string (without the null termination char)
	GLchar *info = new GLchar[infoBufferLength];
	glGetProgramInfoLog(shaderProgram, infoBufferLength, &infoLength, info);

	statusMessage = info;
	delete[] info;

	return false;
}
