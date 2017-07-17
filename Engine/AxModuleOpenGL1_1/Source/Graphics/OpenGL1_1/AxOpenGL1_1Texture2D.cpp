//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGL1_1Texture2D.h"

#include <windows.h>
#include <gl\GL.h>

AxOpenGL1_1Texture2D::AxOpenGL1_1Texture2D()
	: AxDeviceTexture2D()
{
	glGenTextures(1, &this->textureId);

	glBindTexture(GL_TEXTURE_2D, this->textureId);				
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


AxOpenGL1_1Texture2D::~AxOpenGL1_1Texture2D(void)
{
	glDeleteTextures(1, &this->textureId);
}

void AxOpenGL1_1Texture2D::Update(void *data, int width, int height)
{
	if (data != 0)
	{
		glBindTexture(GL_TEXTURE_2D, this->textureId);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	}
}

void AxOpenGL1_1Texture2D::GetData(void *data)
{
	glBindTexture(GL_TEXTURE_2D, this->textureId);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
