//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxCamera.h"

#include "..\..\Axis.h"

AxCamera::AxCamera(Axis *context)
	: AxResource()
{
	this->context = context;

	this->typeId = AxCamera::classId;
	this->resourceType = AxResourceType_Camera;
    this->view = AxMatrix();
    this->projection = AxMatrix();
    this->viewProjection = AxMatrix();
        
    this->properties.Add(new AxProperty(AxString("Aspect"), 1.0f));
    this->properties.Add(new AxProperty(AxString("FOV"), AxMath::Pi / 4.0f));
    this->properties.Add(new AxProperty(AxString("Ortho Zoom"), 1.0f));
    this->properties.Add(new AxProperty(AxString("Orthographic"), false));
    //this->properties.Add(new AxProperty(AxString("Horizontal Scope"), false));
    //this->properties.Add(new AxProperty(AxString("Vertical Scope"), true));
}


AxCamera::~AxCamera(void)
{
}

void AxCamera::BuildMatrix(AxMatrix &transform)
{
	AxMatrix::GetAxisX(this->xAxis, transform);
	AxMatrix::GetAxisY(this->yAxis, transform);
	AxMatrix::GetAxisZ(this->zAxis, transform);
	AxMatrix::GetTranslation(this->position, transform);
	AxVector3::Add(this->lookAt, this->position, this->zAxis);

	float aspect = *(float*)this->properties[AxCamera::propertyIndex_Aspect]->GetEffectiveValue();
	bool orthographic = *(bool*)this->properties[AxCamera::propertyIndex_Orthographic]->GetEffectiveValue();

	AxMatrix::CreateLookAt(this->view, this->position, this->lookAt, this->yAxis);

	float screenAspect = (float)this->context->viewportWidth / (float)this->context->viewportHeight;
	bool isWidthGreater = this->context->viewportWidth > this->context->viewportHeight;
	bool horizontalFOV = 
		(this->context->cameraFOVMode == AxCamera::FOVMode_Horizontal) || 
		((this->context->cameraFOVMode == AxCamera::FOVMode_GreaterScreenDimension) && (isWidthGreater)) ||
		((this->context->cameraFOVMode == AxCamera::FOVMode_LesserScreenDimension) && (!isWidthGreater));

	if (orthographic)
	{
		float orthoZoom = *(float*)this->properties[AxCamera::propertyIndex_OrthoZoom]->GetEffectiveValue();
		if (horizontalFOV)
		{
			AxMatrix::CreateOrthographic(this->projection, orthoZoom, orthoZoom * aspect / screenAspect, 1.0f, 10000.0f);
		}
		else
		{
			AxMatrix::CreateOrthographic(this->projection, orthoZoom * screenAspect / aspect, orthoZoom, 1.0f, 10000.0f);
		}
	}
    else
	{
		float fov = *(float*)this->properties[AxCamera::propertyIndex_FOV]->GetEffectiveValue();
		if (horizontalFOV)
		{
			AxMatrix::CreatePerspectiveFieldOfView(this->projection, fov, 1.0f, aspect / screenAspect, 1.0f, 10000.0f);
		}
		else
		{
			AxMatrix::CreatePerspectiveFieldOfView(this->projection, fov, screenAspect / aspect, 1.0f, 1.0f, 10000.0f);
		}
	}
        
    AxMatrix::Multiply(this->viewProjection, this->view, this->projection);
}

AxVector3 AxCamera::CastVector(AxVector2 &screenCoords)
{
	AxMatrix inverseVP;
	AxMatrix::Invert(inverseVP, this->viewProjection);

    AxVector4 result4;
	AxVector4::Transform(result4, AxVector3(screenCoords, 1.0f), inverseVP);

    AxVector3 result;
	result.x = result4.x / result4.w;
    result.y = result4.y / result4.w;
    result.z = result4.z / result4.w;
	AxVector3::Subtract(result, result, this->position);
    AxVector3::Normalize(result, result);

	return result;
}

AxVector3 AxCamera::ProjectVector(AxVector3 vector)
{
	AxVector4 result4;
	AxVector4::Transform(result4, vector, this->viewProjection);

    AxVector3 result;
	result.x = result4.x / result4.w;
    result.y = result4.y / result4.w;
    result.z = result4.z / result4.w;

	return result;
}