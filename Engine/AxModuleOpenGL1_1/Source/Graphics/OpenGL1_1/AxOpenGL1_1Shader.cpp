//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxOpenGL1_1Shader.h"

#include "AxOpenGL1_1GraphicsDevice.h"
#include "AxOpenGL1_1Texture2D.h"

#include <gl\GL.h>

AxOpenGL1_1Shader::AxOpenGL1_1Shader(void)
	: AxDeviceIndependentShader()
{
}


AxOpenGL1_1Shader::~AxOpenGL1_1Shader(void)
{
}

int AxOpenGL1_1Shader::GetParameterAddress(AxString parameterName)
{
	if (parameterName.Equals("Ambient"))
		return 0;
	else if (parameterName.Equals("Diffuse"))
		return 1;
	else if (parameterName.Equals("Specular"))
		return 2;
	else if (parameterName.Equals("Gloss"))
		return 3;
	else if (parameterName.Equals("Color map"))
		return 4;
	else if (parameterName.Equals("Reflection map"))
		return 5;
	else if (parameterName.Equals("Normal map"))
		return 6;
	else if (parameterName.Equals("Refraction map"))
		return 7;
	else if (parameterName.Equals("Active lights count"))
		return 8;
	else if (parameterName.Equals("View matrix"))
		return 9;
	else if (parameterName.Equals("Projection matrix"))
		return 10;

	else if (parameterName.Equals("World matrix"))
		return 11;
	
	else if (parameterName.Equals("Lights pos"))
		return 12;
	else if (parameterName.Equals("Lights color"))
		return 13;
	else if (parameterName.Equals("Lights intensity"))
		return 14;
	else if (parameterName.Equals("Ambient light"))
		return 15;

	return -1;
}

void AxOpenGL1_1Shader::SetParameter(int address, int index, void *value)
{
	switch (address)
	{
		case 0:
		{
	    	glMaterialfv(GL_FRONT, GL_AMBIENT, (GLfloat*)((AxVector4*)value));
	    	glColor4f(((float*)value)[0], ((float*)value)[1], ((float*)value)[2], ((float*)value)[3]);
			break;
		}
		case 1:
		{
			glMaterialfv(GL_FRONT, GL_DIFFUSE, (GLfloat*)((AxVector4*)value));
			break;
		}
		case 2:
		{
	    	glMaterialfv(GL_FRONT, GL_SPECULAR, (GLfloat*)((AxVector4*)value));
			break;
		}
		case 3:
		{
			glMaterialfv(GL_FRONT, GL_SHININESS, (GLfloat*)((AxVector4*)value));
			break;
		}
		case 4:
		{
			if (value != 0)
			{
				AxOpenGL1_1Texture2D *colorMap = (AxOpenGL1_1Texture2D*)value;
				glDisable(GL_TEXTURE_GEN_S);
				glDisable(GL_TEXTURE_GEN_T);
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, colorMap->textureId);
				glEnable(GL_TEXTURE_2D);
			}
			break;
		}
		case 5:
		{
			if (value != 0)
			{
				AxOpenGL1_1Texture2D *envMap = (AxOpenGL1_1Texture2D*)value;

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, envMap->textureId);
				glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
				glEnable(GL_TEXTURE_GEN_S);
				glEnable(GL_TEXTURE_GEN_T);
			}
			break;
		}
		case 6:
		{
			break;
		}
		case 7:
		{
			break;
		}
		case 8:
		{
			if (value != 0)
			{
				int numLights = *(int*)value;
				for (int i = 0; i < 8; i++)
					if (i < numLights)
						glEnable(GL_LIGHT0 + i);
					else
						glDisable(GL_LIGHT0 + i);
			}
			break;
		}
		case 9:
		{
			if (value != 0)
			{
				AxMatrix::Copy(this->viewTransform, *(AxMatrix*)value);
				AxMatrix::Multiply(this->modelViewTransform, this->modelTransform, this->viewTransform);

				glMatrixMode(GL_MODELVIEW);
				glLoadMatrixf((GLfloat*)&this->modelViewTransform);
			}
			break;
		}
		case 10:
		{
			if (value != 0)
			{
				AxMatrix::Copy(this->projectionTransform, *(AxMatrix*)value);

				glMatrixMode(GL_PROJECTION);
				glLoadMatrixf((GLfloat*)&this->projectionTransform);
			}
			break;
		}

		case 11:
		{
			if (value != 0)
			{
				if (index < this->context->maxBones)
				{
					this->isAnimated |= index > 0;
					this->context->bones[index] = *(AxMatrix*)value;

					if (!this->isAnimated)
					{
						AxMatrix::Copy(this->modelTransform, *(AxMatrix*)value);
						AxMatrix::Multiply(this->modelViewTransform, this->modelTransform, this->viewTransform);
			
						glMatrixMode(GL_MODELVIEW);
						glLoadMatrixf((GLfloat*)&this->modelViewTransform);
					}
				}
			}
			break;
		}
		case 12:
		{
			if (value != 0)
			{
				if (index < 8)
				{
					glLightfv(GL_LIGHT0 + index, GL_POSITION, (GLfloat*)&AxVector4(*(AxVector3*)value, 1.0f));
				}
			}
			break;
		}
		case 13:
		{
			if (value != 0)
			{
				if (index < 8)
				{
					glLightfv(GL_LIGHT0 + index, GL_DIFFUSE, (GLfloat*)((AxVector4*)value));
					glLightfv(GL_LIGHT0 + index, GL_SPECULAR, (GLfloat*)((AxVector4*)value));
				}
			}
			break;
		}
		case 14:
		{
			// No intensity setting
			break;
		}
		// Global ambient light
		case 15:
		{
			glLightfv(GL_LIGHT0, GL_AMBIENT, (GLfloat*)((AxVector4*)value));
			break;
		}
	}
}

bool AxOpenGL1_1Shader::Update(AxString &compilationMessage)
{
	this->isAnimated = false;
	for (int i = 0; i < this->shadingOps.count; i++)
	{
		if (this->shadingOps[i].shadingOp == AxShadingOperation_Skinning)
		{
			this->isAnimated = true;
			break;
		}
	}

	return true;
}