//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGLTexture2D.h"


AxOpenGLTexture2D::AxOpenGLTexture2D()
	: AxDeviceTexture2D()
{
    this->context = 0;
    
    this->glTexture = 0;
    this->glDepthBuffer = 0;
    this->glFrameBuffer = 0;
    
    this->width = 0;
    this->height = 0;
    
    this->isFrameBuffer = false;

	this->Create(0, 1, 1, false);
}

void AxOpenGLTexture2D::Create(void *data, int width, int height, bool renderTarget)
{
    if ((this->width != width) || (this->height != height) || (this->isFrameBuffer != renderTarget))
    {
        this->Dispose();
        
        if (renderTarget)
        {
            glGenTextures(1, &this->glTexture);
            glBindTexture(GL_TEXTURE_2D, this->glTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

            glGenRenderbuffers(1, &this->glDepthBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, this->glDepthBuffer);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

            glGenFramebuffers(1, &this->glFrameBuffer);
            glBindFramebuffer(GL_FRAMEBUFFER, this->glFrameBuffer);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->glTexture, 0);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, this->glDepthBuffer);
        }
        else
        {
            glGenTextures(1, &this->glTexture);
            glBindTexture(GL_TEXTURE_2D, this->glTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
        }

    }
    
    this->width = width;
    this->height = height;
    this->isFrameBuffer = renderTarget;
};

void AxOpenGLTexture2D::Dispose()
{
   if (this->glTexture != 0)
    {
        glDeleteTextures(1, &this->glTexture);
        this->glTexture = 0;
    }
    
    if (this->glDepthBuffer != 0)
    {
		glDeleteRenderbuffers(1, &this->glDepthBuffer);
        this->glDepthBuffer = 0;
    }
    
    if (this->glFrameBuffer != 0)
    {
		glDeleteFramebuffers(1, &this->glFrameBuffer);
        this->glFrameBuffer = 0;
    }
    
    this->width = 0;
    this->height = 0;

    this->isFrameBuffer = false;
}

AxOpenGLTexture2D::~AxOpenGLTexture2D(void)
{
	this->Dispose();
}

void AxOpenGLTexture2D::Update(void *data, int width, int height)
{
	this->width = width;
	this->height = height;

	glBindTexture(GL_TEXTURE_2D, this->glTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

void AxOpenGLTexture2D::GetData(void *data)
{
	glBindTexture(GL_TEXTURE_2D, this->glTexture);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}
