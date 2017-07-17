//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGL1_1GraphicsDevice.h"

#include "AxOpenGL1_1Texture2D.h"
#include "AxOpenGL1_1Mesh.h"
#include "AxOpenGL1_1Shader.h"

#include "..\..\AxisEngine.h"

AxOpenGL1_1GraphicsDevice::AxOpenGL1_1GraphicsDevice()
	:AxGraphicsDevice()
{
	this->windowHandle_HWND = new HWND();
	this->deviceContext_HDC = new HDC();
	this->renderContext_HGLRC = new HGLRC();

	this->animatedMesh = (AxOpenGL1_1Mesh*)this->CreateMesh();
	this->animatedMesh->CreateVertexBuffer(65536);
	
	this->maxBones = 64;
	this->bones = new AxMatrix[this->maxBones];
}

AxOpenGL1_1GraphicsDevice::~AxOpenGL1_1GraphicsDevice(void)
{
	delete[] this->bones;
	delete this->animatedMesh;

	delete (HWND*)this->windowHandle_HWND;
	delete (HDC*)this->deviceContext_HDC;
	delete (HGLRC*)this->renderContext_HGLRC;
}

bool AxOpenGL1_1GraphicsDevice::SetRenderTargetWindow(void *windowHandle)
{
	if (*(HWND*)this->windowHandle_HWND == *(HWND*)windowHandle)
		return true;

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

	this->pixelFormatIndex = ChoosePixelFormat(*(HDC*)this->deviceContext_HDC, &pixelFormatDescriptor);
	if (this->pixelFormatIndex == 0)
		return false;

	if(!SetPixelFormat(*((HDC*)this->deviceContext_HDC), this->pixelFormatIndex, &pixelFormatDescriptor))
		return false;

	*(HGLRC*)this->renderContext_HGLRC = wglCreateContext(*(HDC*)this->deviceContext_HDC);
	if (*(HGLRC*)this->renderContext_HGLRC == 0)
		return false;

	if (!wglMakeCurrent(*(HDC*)this->deviceContext_HDC, *(HGLRC*)this->renderContext_HGLRC) != 0)
		return false;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	//glFrontFace(GL_CCW);
	
	glEnable(GL_DEPTH_TEST);
	   
	glEnable(GL_TEXTURE_2D);
	//glDisable(GL_TEXTURE_2D);
	
	glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glDisable(GL_LIGHTING);
	
	//glLightfv(GL_LIGHT0, GL_AMBIENT, this.floatGL);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, this.floatGL);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, this.floatGL);
	
	glDisable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_ADD);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	this->pixelFormat = AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdAlpha);

	return true;
}

bool AxOpenGL1_1GraphicsDevice::SetRenderTargetTexture(AxDeviceTexture2D *texture)
{
	return false;
}

AxDeviceMesh *AxOpenGL1_1GraphicsDevice::CreateMesh()
{
	return new AxOpenGL1_1Mesh();
}

AxDeviceTexture2D *AxOpenGL1_1GraphicsDevice::CreateTexture()
{
	return new AxOpenGL1_1Texture2D();
}

AxDeviceShader *AxOpenGL1_1GraphicsDevice::CreateShader()
{
	AxOpenGL1_1Shader *shader = new AxOpenGL1_1Shader();
	shader->context = this;
	return shader;
}

AxPixelFormat AxOpenGL1_1GraphicsDevice::GetPixelFormat()
{
	return this->pixelFormat;
}

void AxOpenGL1_1GraphicsDevice::ClearScreen(AxVector4 &color)
{
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void AxOpenGL1_1GraphicsDevice::ClearDepthBuffer()
{
    glClear(GL_DEPTH_BUFFER_BIT);
}

void AxOpenGL1_1GraphicsDevice::BeginScene()
{
}

void AxOpenGL1_1GraphicsDevice::EndScene()
{
}

void AxOpenGL1_1GraphicsDevice::SetViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
}

void AxOpenGL1_1GraphicsDevice::SetMesh(AxDeviceMesh &mesh)
{
	this->currentMesh = (AxOpenGL1_1Mesh*)&mesh;
}

void AxOpenGL1_1GraphicsDevice::SetShader(AxDeviceShader &shader)
{
	this->currentShader = (AxOpenGL1_1Shader*)&shader;
}

void AxOpenGL1_1GraphicsDevice::SetRenderState(AxRenderState renderState)
{
}

void AxOpenGL1_1GraphicsDevice::RenderMesh()
{
	if (this->currentShader->isAnimated)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)&this->currentShader->viewTransform);

		this->CreateAnimatedMesh(this->currentMesh);

		glVertexPointer(3, GL_FLOAT, 0, this->animatedMesh->positions);
		glNormalPointer(GL_FLOAT, 0, this->animatedMesh->normals);
		glTexCoordPointer(2, GL_FLOAT, 0, this->currentMesh->texCoords);
    
		glDrawElements(GL_TRIANGLES, this->currentMesh->numIndices, GL_UNSIGNED_SHORT, this->currentMesh->indices);
	}
	else
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)&this->currentShader->modelViewTransform);

		glVertexPointer(3, GL_FLOAT, 0, this->currentMesh->positions);
		glNormalPointer(GL_FLOAT, 0, this->currentMesh->normals);
		glTexCoordPointer(2, GL_FLOAT, 0, this->currentMesh->texCoords);
    
		glDrawElements(GL_TRIANGLES, this->currentMesh->numIndices, GL_UNSIGNED_SHORT, this->currentMesh->indices);
	}
}

bool AxOpenGL1_1GraphicsDevice::Present()
{
	return SwapBuffers(*(HDC*)this->deviceContext_HDC) != 0;
}



void AxOpenGL1_1GraphicsDevice::CreateAnimatedMesh(AxOpenGL1_1Mesh *glMesh)
{
	if (glMesh->numVertices > this->animatedMesh->numVertices)
		this->animatedMesh->CreateVertexBuffer(glMesh->numVertices);

	float *inputPos = glMesh->positions;
	float *inputNormal = glMesh->normals;
	float *outputPos = this->animatedMesh->positions;
	float *outputNormal = this->animatedMesh->normals;
	AxVector4 boneIndices, boneWeights;
	AxMatrix bone, transform;
	for (int vertexIndex = 0; vertexIndex < glMesh->numVertices; vertexIndex++)
	{
		glMesh->GetVertexBones(vertexIndex, boneIndices, boneWeights);

		bone = this->bones[(int)boneIndices.x];
		transform._11 = bone._11 * boneWeights.x;
		transform._12 = bone._12 * boneWeights.x;
		transform._13 = bone._13 * boneWeights.x;
		transform._21 = bone._21 * boneWeights.x;
		transform._22 = bone._22 * boneWeights.x;
		transform._23 = bone._23 * boneWeights.x;
		transform._31 = bone._31 * boneWeights.x;
		transform._32 = bone._32 * boneWeights.x;
		transform._33 = bone._33 * boneWeights.x;
		transform._41 = bone._41 * boneWeights.x;
		transform._42 = bone._42 * boneWeights.x;
		transform._43 = bone._43 * boneWeights.x;
		if (boneWeights.y > 0.0f)
		{
			bone = this->bones[(int)boneIndices.y];
			transform._11 += bone._11 * boneWeights.y;
			transform._12 += bone._12 * boneWeights.y;
			transform._13 += bone._13 * boneWeights.y;
			transform._21 += bone._21 * boneWeights.y;
			transform._22 += bone._22 * boneWeights.y;
			transform._23 += bone._23 * boneWeights.y;
			transform._31 += bone._31 * boneWeights.y;
			transform._32 += bone._32 * boneWeights.y;
			transform._33 += bone._33 * boneWeights.y;
			transform._41 += bone._41 * boneWeights.y;
			transform._42 += bone._42 * boneWeights.y;
			transform._43 += bone._43 * boneWeights.y;
			if (boneWeights.z > 0.0f)
			{
				bone = this->bones[(int)boneIndices.z];
				transform._11 += bone._11 * boneWeights.z;
				transform._12 += bone._12 * boneWeights.z;
				transform._13 += bone._13 * boneWeights.z;
				transform._21 += bone._21 * boneWeights.z;
				transform._22 += bone._22 * boneWeights.z;
				transform._23 += bone._23 * boneWeights.z;
				transform._31 += bone._31 * boneWeights.z;
				transform._32 += bone._32 * boneWeights.z;
				transform._33 += bone._33 * boneWeights.z;
				transform._41 += bone._41 * boneWeights.z;
				transform._42 += bone._42 * boneWeights.z;
				transform._43 += bone._43 * boneWeights.z;
				if (boneWeights.w > 0.0f)
				{
					bone = this->bones[(int)boneIndices.w];
					transform._11 += bone._11 * boneWeights.w;
					transform._12 += bone._12 * boneWeights.w;
					transform._13 += bone._13 * boneWeights.w;
					transform._21 += bone._21 * boneWeights.w;
					transform._22 += bone._22 * boneWeights.w;
					transform._23 += bone._23 * boneWeights.w;
					transform._31 += bone._31 * boneWeights.w;
					transform._32 += bone._32 * boneWeights.w;
					transform._33 += bone._33 * boneWeights.w;
					transform._41 += bone._41 * boneWeights.w;
					transform._42 += bone._42 * boneWeights.w;
					transform._43 += bone._43 * boneWeights.w;
				}
			}
		}
	
		float inputX = *inputPos;
		inputPos++;
		float inputY = *inputPos;
		inputPos++;
		float inputZ = *inputPos;
		inputPos++;

		*outputPos = inputX * transform._11 +
						inputY * transform._21 +
						inputZ * transform._31 +
									transform._41;
		outputPos++;

		*outputPos = inputX * transform._12 +
						inputY * transform._22 +
						inputZ * transform._32 +
									transform._42;
		outputPos++;

		*outputPos = inputX * transform._13 +
						inputY * transform._23 +
						inputZ * transform._33 +
									transform._43;
		outputPos++;


		// Normals should be calculated with the invert transpose of transform in case of a non-uniform scaling. This is omitted for performance reasons
		inputX = *inputNormal;
		inputNormal++;
		inputY = *inputNormal;
		inputNormal++;
		inputZ = *inputNormal;
		inputNormal++;

		*outputNormal =	inputX * transform._11 +
						inputY * transform._21 +
						inputZ * transform._31;
		outputNormal++;

		*outputNormal =	inputX * transform._12 +
						inputY * transform._22 +
						inputZ * transform._32;
		outputNormal++;

		*outputNormal =	inputX * transform._13 +
						inputY * transform._23 +
						inputZ * transform._33;
		outputNormal++;
	}
}