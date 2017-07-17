//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGLGraphicsDevice.h"

#include "AxOpenGLTexture2D.h"
#include "AxOpenGLMesh.h"
#include "AxOpenGLShader.h"

#include "..\..\AxisEngine.h"

AxOpenGLGraphicsDevice::AxOpenGLGraphicsDevice()
	:AxGraphicsDevice()
{
	this->windowHandle_HWND = new HWND();
	this->deviceContext_HDC = new HDC();
	this->renderContext_HGLRC = new HGLRC();

	this->width = 0;
	this->height = 0;

    this->currentShader = 0;
	this->currentMesh = 0;
	this->currentRenderTarget = 0;

	this->cullMode = AxRenderState_CullBackFace;

	this->maxBones = 64;
	this->bones = new AxMatrix[this->maxBones];
}

AxOpenGLGraphicsDevice::~AxOpenGLGraphicsDevice(void)
{
	delete[] this->bones;

	delete (HWND*)this->windowHandle_HWND;
	delete (HDC*)this->deviceContext_HDC;
	delete (HGLRC*)this->renderContext_HGLRC;
}

AxDeviceMesh *AxOpenGLGraphicsDevice::CreateMesh()
{
	AxOpenGLMesh *result = new AxOpenGLMesh();
	result->context = this;
	return result;
}

AxDeviceTexture2D *AxOpenGLGraphicsDevice::CreateTexture()
{
	AxOpenGLTexture2D *result = new AxOpenGLTexture2D();
	result->context = this;
	return result;
}

AxDeviceShader *AxOpenGLGraphicsDevice::CreateShader()
{
	AxOpenGLShader *result = new AxOpenGLShader();
	result->context = this;
	return result;
}

bool AxOpenGLGraphicsDevice::SetRenderTargetWindow(void *windowHandle)
{
	if (*(HWND*)this->windowHandle_HWND != *(HWND*)windowHandle)
	{
		PIXELFORMATDESCRIPTOR pixelFormatDescriptor =
		{
			sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
			1,											// Version Number
			PFD_DRAW_TO_WINDOW |						// Format Must Support Window
			PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
			PFD_DOUBLEBUFFER,							// Must Support Double Buffering
			PFD_TYPE_RGBA,								// Request An RGBA Format
			32,											// Select Our Color Depth
			0, 0, 0, 0, 0, 0,							// Color Bits Ignored
			0,											// No Alpha Buffer
			0,											// Shift Bit Ignored
			0,											// No Accumulation Buffer
			0, 0, 0, 0,									// Accumulation Bits Ignored
			24,											// Z-Buffer (Depth Buffer)  
			0,											// No Stencil Buffer
			0,											// No Auxiliary Buffer
			PFD_MAIN_PLANE,								// Main Drawing Layer
			0,											// Reserved
			0, 0, 0										// Layer Masks Ignored
		};

		*(HWND*)this->windowHandle_HWND = *(HWND*)windowHandle;

		*(HDC*)this->deviceContext_HDC = GetDC(*(HWND*)this->windowHandle_HWND);
		if (*(HDC*)this->deviceContext_HDC == 0)
			return false;

		int pixelFormatIndex = ChoosePixelFormat(*(HDC*)this->deviceContext_HDC, &pixelFormatDescriptor);
		if (pixelFormatIndex == 0)
			return false;

		if(!SetPixelFormat(*((HDC*)this->deviceContext_HDC), pixelFormatIndex, &pixelFormatDescriptor))
			return false;

		*(HGLRC*)this->renderContext_HGLRC = wglCreateContext(*(HDC*)this->deviceContext_HDC);
		if (*(HGLRC*)this->renderContext_HGLRC == 0)
			return false;

		if (!wglMakeCurrent(*(HDC*)this->deviceContext_HDC, *(HGLRC*)this->renderContext_HGLRC) != 0)
			return false;

		glewInit();

		glEnable(GL_DEPTH_TEST);
            
        glEnable(GL_CULL_FACE);
		glFrontFace(GL_CW);
        glCullFace(GL_BACK);

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glDisable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		this->pixelFormat = AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdAlpha);

		// Window size
		int windowWidth, windowHeight;
		AxPlatformUtils::GetWindowClientSize(this->windowHandle_HWND, windowWidth, windowHeight);
		this->SetViewport(0, 0, windowWidth, windowHeight);
	}

	else

	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
        
        this->currentRenderTarget = 0;
    
        this->SetViewport(0, 0, this->width, this->height);
	}

	return true;
}

bool AxOpenGLGraphicsDevice::SetRenderTargetTexture(AxDeviceTexture2D *texture)
{
    AxOpenGLTexture2D *glTexture = (AxOpenGLTexture2D*)texture;
    
    if (this->currentRenderTarget == texture)
        return true;
    
    if (!glTexture->isFrameBuffer)
    {
        glTexture->Create(0, glTexture->width, glTexture->height, true);
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, glTexture->glFrameBuffer);
    
    this->currentRenderTarget = glTexture;

    glClear(GL_DEPTH_BUFFER_BIT);

    this->SetViewport(0, 0, glTexture->width, glTexture->height);
    
    return true;
}

AxPixelFormat AxOpenGLGraphicsDevice::GetPixelFormat()
{
	return this->pixelFormat;
}

void AxOpenGLGraphicsDevice::ClearScreen(AxVector4 &color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void AxOpenGLGraphicsDevice::ClearDepthBuffer()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void AxOpenGLGraphicsDevice::BeginScene()
{
}

void AxOpenGLGraphicsDevice::EndScene()
{
}

void AxOpenGLGraphicsDevice::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
    
	if (this->currentRenderTarget == 0)
    {
        this->width = width;
        this->height = height;
    }
}

void AxOpenGLGraphicsDevice::SetMesh(AxDeviceMesh &mesh)
{
	this->currentMesh = (AxOpenGLMesh*)&mesh;
}

void AxOpenGLGraphicsDevice::SetShader(AxDeviceShader &shader)
{
	this->currentShader = (AxOpenGLShader*)&shader;
    
    this->currentShader->setTextureIndex = 0;
    
    glUseProgram(this->currentShader->glShaderProgram);
    
    AxMatrix viewProjectionMatrix;
    AxMatrix::Multiply(viewProjectionMatrix, this->currentShader->viewMatrix, this->currentShader->projectionMatrix);
    this->currentShader->SetParameter(this->currentShader->viewProjectionMatrixAddress, 0, &viewProjectionMatrix);
    
    if (this->currentShader->isSkeletalAnimated)
        glUniformMatrix4fv(this->currentShader->worldMatrixAddress, AxOpenGLShader::maxBones, false, this->currentShader->bones);

	if (this->currentShader->lightsPosIntensityAddress != AxDeviceShader::invalidParameterAddress)
        glUniform4fv(this->currentShader->lightsPosIntensityAddress, 1, this->currentShader->lightsPosIntensity);
    
    if (this->currentShader->lightsColorAddress != AxDeviceShader::invalidParameterAddress)
        glUniform4fv(this->currentShader->lightsColorAddress, 1, this->currentShader->lightsColor);
}

void AxOpenGLGraphicsDevice::SetRenderState(AxRenderState renderState)
{
    switch (renderState)
    {
        case AxRenderState_CullBackFace:
        {
            if (this->cullMode != renderState)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);
                
                this->cullMode = renderState;
            }

            break;
        }

        case AxRenderState_CullFrontFace:
        {
            if (this->cullMode != renderState)
            {
                glEnable(GL_CULL_FACE);
                glCullFace(GL_FRONT);

                this->cullMode = renderState;
            }

            break;
        }

        case AxRenderState_CullNone:
        {
            if (this->cullMode != renderState)
            {
                glDisable(GL_CULL_FACE);

                this->cullMode = renderState;
            }

            break;
        }
    }
}

void AxOpenGLGraphicsDevice::RenderMesh()
{
    if (this->currentShader->vertexPositionAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexPositionAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesPositionsBuffer);
        glVertexAttribPointer(this->currentShader->vertexPositionAttribute, this->currentMesh->vertexPositionElementsCount, GL_FLOAT, false, 0, 0);
    }

    if (this->currentShader->vertexTexCoordsAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexTexCoordsAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesTexCoordsBuffer);
        glVertexAttribPointer(this->currentShader->vertexTexCoordsAttribute, this->currentMesh->vertexTexCoordsElementsCount, GL_FLOAT, false, 0, 0);
    }

    if (this->currentShader->vertexNormalAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexNormalAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesNormalsBuffer);
        glVertexAttribPointer(this->currentShader->vertexNormalAttribute, this->currentMesh->vertexNormalElementsCount, GL_FLOAT, false, 0, 0);
    }
    
    if (this->currentShader->vertexBoneIndicesAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexBoneIndicesAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesBoneIndicesBuffer);
        glVertexAttribPointer(this->currentShader->vertexBoneIndicesAttribute, this->currentMesh->vertexBoneIndicesElementsCount, GL_FLOAT, false, 0, 0);
    }
    
    if (this->currentShader->vertexBoneWeightsAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexBoneWeightsAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesBoneWeightsBuffer);
        glVertexAttribPointer(this->currentShader->vertexBoneWeightsAttribute, this->currentMesh->vertexBoneWeightsElementsCount, GL_FLOAT, false, 0, 0);
    }

    if (this->currentShader->vertexTangentsAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexTangentsAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesTangentBuffer);
        glVertexAttribPointer(this->currentShader->vertexTangentsAttribute, this->currentMesh->vertexTangentElementsCount, GL_FLOAT, false, 0, 0);
    }
    
    if (this->currentShader->vertexBitangentsAttribute != -1)
    {
        glEnableVertexAttribArray(this->currentShader->vertexBitangentsAttribute);
        glBindBuffer(GL_ARRAY_BUFFER, this->currentMesh->verticesBitangentBuffer);
        glVertexAttribPointer(this->currentShader->vertexBitangentsAttribute, this->currentMesh->vertexBitangentElementsCount, GL_FLOAT, false, 0, 0);
    }
   
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->currentMesh->indicesBuffer);

    glDrawElements(GL_TRIANGLES, this->currentMesh->indicesCount, GL_UNSIGNED_INT, 0);
}

bool AxOpenGLGraphicsDevice::Present()
{
	return SwapBuffers(*(HDC*)this->deviceContext_HDC) != 0;
}
