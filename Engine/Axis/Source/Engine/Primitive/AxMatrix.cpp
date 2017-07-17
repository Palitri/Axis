//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMatrix.h"

#include "..\..\Tools\AxMath.h"

#include "..\Utilities\AxMaths.h"

#include "AxVector3.h"
#include "AxVector4.h"

int AxMatrix::handedness = AxMatrix::handedness_Right;

AxMatrix::AxMatrix(void)
{
	AxMatrix::CreateIdentity(*this);
}

AxMatrix::AxMatrix(AxMatrix &source)
{
	AxMatrix::Copy(*this, source);
}


AxMatrix::~AxMatrix(void)
{
}

float AxMatrix::GetDeterminant()
{
    return this->_11 * this->_22 * this->_33 * this->_44 +
		   this->_11 * this->_23 * this->_34 * this->_42 +
		   this->_11 * this->_24 * this->_32 * this->_43 +

		   this->_12 * this->_21 * this->_34 * this->_43 +
		   this->_12 * this->_23 * this->_31 * this->_44 +
		   this->_12 * this->_24 * this->_33 * this->_41 +

		   this->_13 * this->_21 * this->_32 * this->_44 +
		   this->_13 * this->_22 * this->_34 * this->_41 +
		   this->_13 * this->_24 * this->_31 * this->_42 +

		   this->_14 * this->_21 * this->_33 * this->_42 +
		   this->_14 * this->_22 * this->_31 * this->_43 +
		   this->_14 * this->_23 * this->_32 * this->_41 -

		   this->_11 * this->_22 * this->_34 * this->_43 -
		   this->_11 * this->_23 * this->_32 * this->_44 -
		   this->_11 * this->_24 * this->_33 * this->_42 -

		   this->_12 * this->_21 * this->_33 * this->_44 -
		   this->_12 * this->_23 * this->_34 * this->_41 -
		   this->_12 * this->_24 * this->_31 * this->_43 -

		   this->_13 * this->_21 * this->_34 * this->_42 -
		   this->_13 * this->_22 * this->_31 * this->_44 -
		   this->_13 * this->_24 * this->_32 * this->_41 -

		   this->_14 * this->_21 * this->_32 * this->_43 -
		   this->_14 * this->_22 * this->_33 * this->_41 -
		   this->_14 * this->_23 * this->_31 * this->_42;
}

bool AxMatrix::Equals(AxMatrix &matrix)
{
	return
		(this->_11 == matrix._11) &&
		(this->_12 == matrix._12) &&
		(this->_13 == matrix._13) &&
		(this->_14 == matrix._14) &&
		(this->_21 == matrix._21) &&
		(this->_22 == matrix._22) &&
		(this->_23 == matrix._23) &&
		(this->_24 == matrix._24) &&
		(this->_31 == matrix._31) &&
		(this->_32 == matrix._32) &&
		(this->_33 == matrix._33) &&
		(this->_34 == matrix._34) &&
		(this->_41 == matrix._41) &&
		(this->_42 == matrix._42) &&
		(this->_43 == matrix._43) &&
		(this->_44 == matrix._44);
}

bool AxMatrix::IsIdentity()
{
	return
		(this->_11 == 1.0f) &&
		(this->_12 == 0.0f) &&
		(this->_13 == 0.0f) &&
		(this->_14 == 0.0f) &&
		(this->_21 == 0.0f) &&
		(this->_22 == 1.0f) &&
		(this->_23 == 0.0f) &&
		(this->_24 == 0.0f) &&
		(this->_31 == 0.0f) &&
		(this->_32 == 0.0f) &&
		(this->_33 == 1.0f) &&
		(this->_34 == 0.0f) &&
		(this->_41 == 0.0f) &&
		(this->_42 == 0.0f) &&
		(this->_43 == 0.0f) &&
		(this->_44 == 1.0f);
}



// Static Methods

void AxMatrix::Copy(AxMatrix &result, AxMatrix &source)
{
	result._11 = source._11;
	result._12 = source._12;
	result._13 = source._13;
	result._14 = source._14;
	result._21 = source._21;
	result._22 = source._22;
	result._23 = source._23;
	result._24 = source._24;
	result._31 = source._31;
	result._32 = source._32;
	result._33 = source._33;
	result._34 = source._34;
	result._41 = source._41;
	result._42 = source._42;
	result._43 = source._43;
	result._44 = source._44;
}
	
void AxMatrix::CreateIdentity(AxMatrix &result)
{
    result._11 = 1.0f;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = 1.0f;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = 1.0f;
    result._34 = 0.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}
	
void AxMatrix::CreateRotationX(AxMatrix &result, float phi)
{
    float SinPhi = AxMath::Sin(phi);
    float CosPhi = AxMath::Cos(phi);

    result._11 = 1.0f;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = CosPhi;
    result._23 = SinPhi;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = -SinPhi;
    result._33 = CosPhi;
    result._34 = 0.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}

void AxMatrix::CreateRotationY(AxMatrix &result, float phi)
{
    float SinPhi = AxMath::Sin(phi);
    float CosPhi = AxMath::Cos(phi);

    result._11 = CosPhi;
    result._12 = 0.0f;
    result._13 = -SinPhi;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = 1.0f;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = SinPhi;
    result._32 = 0.0f;
    result._33 = CosPhi;
    result._34 = 0.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}

void AxMatrix::CreateRotationZ(AxMatrix &result, float phi)
{
    float SinPhi = AxMath::Sin(phi);
    float CosPhi = AxMath::Cos(phi);

    result._11 = CosPhi;
    result._12 = SinPhi;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = -SinPhi;
    result._22 = CosPhi;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = 1.0f;
    result._34 = 0.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}	

void AxMatrix::CreateRotationAxis(AxMatrix &result, float x, float y, float z, float phi)
{
	float SinPhi = AxMath::Sin(phi);
    float CosPhi = AxMath::Cos(phi);
    float InvCosPhi = 1 - CosPhi;
    float SinPhiX = SinPhi * x;
    float SinPhiY = SinPhi * y;
    float SinPhiZ = SinPhi * z;
    float InvCosPhiXY = InvCosPhi * x * y;
    float InvCosPhiXZ = InvCosPhi * x * z;
    float InvCosPhiYZ = InvCosPhi * y * z;

    result._11 = CosPhi + InvCosPhi * x * x;
    result._12 = InvCosPhiXY - SinPhiZ;
    result._13 = InvCosPhiXZ + SinPhiY;
    result._14 = 0.0f;
    result._21 = InvCosPhiXY + SinPhiZ;
    result._22 = CosPhi + InvCosPhi * y * y;
    result._23 = InvCosPhiYZ - SinPhiX;
    result._24 = 0.0f;
    result._31 = InvCosPhiXZ - SinPhiY;
    result._32 = InvCosPhiYZ + SinPhiX;
    result._33 = CosPhi + InvCosPhi * z * z;
    result._34 = 0.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}

void AxMatrix::CreateRotationAxis(AxMatrix &result, AxVector3 &axis, float phi)
{
	AxMatrix::CreateRotationAxis(result, axis.x, axis.y, axis.z, phi);
}

void AxMatrix::CreateTranslation(AxMatrix &result, float x, float y, float z)
{
    result._11 = 1.0f;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = 1.0f;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = 1.0f;
    result._34 = 0.0f;
    result._41 = x;
    result._42 = y;
    result._43 = z;
    result._44 = 1.0f;
}

void AxMatrix::CreateTranslation(AxMatrix &result, AxVector3 &translation)
{
    AxMatrix::CreateTranslation(result, translation.x, translation.y, translation.z);
}

void AxMatrix::CreateScaling(AxMatrix &result, float x, float y, float z)
{
    result._11 = x;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = y;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = z;
    result._34 = 0.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}

void AxMatrix::CreateScaling(AxMatrix &result, AxVector3 &scaling)
{
    AxMatrix::CreateScaling(result, scaling.x, scaling.y, scaling.z);
}

void AxMatrix::CreateLookAt(AxMatrix &result, AxVector3 &position, AxVector3 &lookAt, AxVector3 &up)
{
	AxVector3 xAxis, yAxis, zAxis;

	AxVector3::Subtract(zAxis, position, lookAt);
	AxVector3::Normalize(zAxis, zAxis);
    AxVector3::Cross(xAxis, up, zAxis);
	AxVector3::Normalize(xAxis, xAxis);
    AxVector3::Cross(yAxis, zAxis, xAxis);

    result._11 = xAxis.x;
    result._12 = yAxis.x;
    result._13 = zAxis.x;
    result._14 = 0.0f;
    result._21 = xAxis.y;
    result._22 = yAxis.y;
    result._23 = zAxis.y;
    result._24 = 0.0f;
    result._31 = xAxis.z;
    result._32 = yAxis.z;
    result._33 = zAxis.z;
    result._34 = 0.0f;
    result._41 = -AxVector3::Dot(xAxis, position);
    result._42 = -AxVector3::Dot(yAxis, position);
    result._43 = -AxVector3::Dot(zAxis, position);
    result._44 = 1.0f;
}

void AxMatrix::CreatePerspectiveFieldOfView(AxMatrix &result, float fov, float aspectHorizontal, float aspectVertical, float zn, float zf)
{
	switch (AxMatrix::handedness)
	{
		case AxMatrix::handedness_Right:
			return AxMatrix::CreatePerspectiveFieldOfViewRH(result, fov, aspectHorizontal, aspectVertical, zn, zf);
		case AxMatrix::handedness_Left:
			return AxMatrix::CreatePerspectiveFieldOfViewLH(result, fov, aspectHorizontal, aspectVertical, zn, zf);
		default:
			return AxMatrix::CreatePerspectiveFieldOfViewRH(result, fov, aspectHorizontal, aspectVertical, zn, zf);
	}
}

void AxMatrix::CreatePerspectiveFieldOfViewRH(AxMatrix &result, float fov, float aspectHorizontal, float aspectVertical, float zn, float zf)
{
    float f = 1.0f / AxMath::Tan(fov * 0.5f);
	float dz = zn - zf;

    result._11 = f / aspectHorizontal;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = f / aspectVertical;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = zf / dz;
    result._34 = -1.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = (zn * zf) / dz;
    result._44 = 0.0f;
}

void AxMatrix::CreatePerspectiveFieldOfViewLH(AxMatrix &result, float fov, float aspectHorizontal, float aspectVertical, float zn, float zf)
{
    float f = 1.0f / AxMath::Tan(fov * 0.5f);
	float dz = zn - zf;

    result._11 = f / aspectHorizontal;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = f / aspectVertical;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = zf / dz;
    result._34 = 1.0f;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = - (zn * zf) / dz;
    result._44 = 0.0f;
}

void AxMatrix::CreateOrthographic(AxMatrix &result, float width, float height, float zn, float zf)
{
	switch (AxMatrix::handedness)
	{
		case AxMatrix::handedness_Right:
			return AxMatrix::CreateOrthographicRH(result, width, height, zn, zf);
		case AxMatrix::handedness_Left:
			return AxMatrix::CreateOrthographicLH(result, width, height, zn, zf);
		default:
			return AxMatrix::CreateOrthographicRH(result, width, height, zn, zf);
	}
}

void AxMatrix::CreateOrthographicRH(AxMatrix &result, float width, float height, float zn, float zf)
{
    float dz = zn - zf;

    result._11 = 2.0f / width;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = 2.0f / height;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = 1.0f / dz;
    result._34 = zn / dz;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}

void AxMatrix::CreateOrthographicLH(AxMatrix &result, float width, float height, float zn, float zf)
{
    float dz = zn - zf;

    result._11 = 2.0f / width;
    result._12 = 0.0f;
    result._13 = 0.0f;
    result._14 = 0.0f;
    result._21 = 0.0f;
    result._22 = 2.0f / height;
    result._23 = 0.0f;
    result._24 = 0.0f;
    result._31 = 0.0f;
    result._32 = 0.0f;
    result._33 = - 1.0f / dz;
    result._34 = zn / dz;
    result._41 = 0.0f;
    result._42 = 0.0f;
    result._43 = 0.0f;
    result._44 = 1.0f;
}
    
// Creates a matrix tranformation oriented towards the given vector
void AxMatrix::CreateOrientation(AxMatrix &result, AxVector3 &orientationVector, AxVector3 &orientationOrigin)
{
	AxVector3 rotationAxis;
	
	float phi = AxMaths::VectorAngle(orientationOrigin, orientationVector);
	AxVector3::Cross(rotationAxis, orientationOrigin, orientationVector);
	AxVector3::Normalize(rotationAxis, rotationAxis);
	AxMatrix::CreateRotationAxis(result, rotationAxis, -phi);
}

// Creates a matrix tranformation with the given position, oriented towards the given target
void AxMatrix::CreateOrientedPosition(AxMatrix &result, AxVector3 &position, AxVector3 &target, AxVector3 &orientationOrigin)
{
	AxVector3 orientationVector;
	AxVector3::Subtract(orientationVector, target, position);

	AxMatrix::CreateOrientation(result, orientationVector, orientationOrigin);
	AxMatrix::SetTranslation(result, position);
	//AxMatrix translation;
	//AxMatrix::CreateTranslation(translation, position);
	//AxMatrix::Multiply(result, result, translation);
}

// Creates a matrix tranformation with the given position, oriented as possible towards the given target, with the given axis to rotate around
void AxMatrix::CreateOrientationAxis(AxMatrix &result, AxVector3 &orientationVector, AxVector3 &orientationOrigin, AxVector3 &axis)
{
	AxVector3 actualTarget;
	AxVector3 origin = AxVector3(0.0f, 0.0f, 0.0f);

	AxMaths::PointToPlaneProjection(actualTarget, orientationVector, origin, axis);

	AxMatrix::CreateOrientation(result, actualTarget, orientationOrigin);
}

// Creates a matrix tranformation with the given position, oriented as possible towards the given target, with the given axis to rotate around
void AxMatrix::CreateOrientationAxis(AxMatrix &result, AxVector3 &position, AxVector3 &target, AxVector3 &orientationOrigin, AxVector3 &axis)
{
	AxVector3 actualTarget;

	AxMaths::PointToPlaneProjection(actualTarget, target, position, axis);

	AxMatrix::CreateOrientedPosition(result, position, actualTarget, orientationOrigin);
}

void AxMatrix::CreateReflectionPlane(AxMatrix &result, AxVector4 &plane)
{
	result._11 = - 2.0f * plane.x * plane.x + 1.0f;
	result._12 = - 2.0f * plane.x * plane.y;
	result._13 = - 2.0f * plane.x * plane.z;
	result._14 = 0.0f;
	result._21 = - 2.0f * plane.y * plane.x;
	result._22 = - 2.0f * plane.y * plane.y + 1.0f;
	result._23 = - 2.0f * plane.y * plane.z;
	result._24 = 0.0f;
	result._31 = - 2.0f * plane.z * plane.x;
	result._32 = - 2.0f * plane.z * plane.y;
	result._33 = - 2.0f * plane.z * plane.z + 1.0f;
	result._34 = 0.0f;
	result._41 = - 2.0f * plane.w * plane.x;
	result._42 = - 2.0f * plane.w * plane.y;
	result._43 = - 2.0f * plane.w * plane.z;
	result._44 = 1.0f;
}

void AxMatrix::Multiply(AxMatrix &result, AxMatrix &matrix1, AxMatrix &matrix2)
{
    float _11 = 	matrix1._11 * matrix2._11 +
	                matrix1._12 * matrix2._21 +
	                matrix1._13 * matrix2._31 +
	                matrix1._14 * matrix2._41;
    float _12 = 	matrix1._11 * matrix2._12 +
	                matrix1._12 * matrix2._22 +
	                matrix1._13 * matrix2._32 +
	                matrix1._14 * matrix2._42;
    float _13 = 	matrix1._11 * matrix2._13 +
	                matrix1._12 * matrix2._23 +
	                matrix1._13 * matrix2._33 +
	                matrix1._14 * matrix2._43;
    float _14 = 	matrix1._11 * matrix2._14 +
	                matrix1._12 * matrix2._24 +
	                matrix1._13 * matrix2._34 +
	                matrix1._14 * matrix2._44;
    float _21 = 	matrix1._21 * matrix2._11 +
	                matrix1._22 * matrix2._21 +
	                matrix1._23 * matrix2._31 +
	                matrix1._24 * matrix2._41;
    float _22 = 	matrix1._21 * matrix2._12 +
	                matrix1._22 * matrix2._22 +
	                matrix1._23 * matrix2._32 +
	                matrix1._24 * matrix2._42;
    float _23 = 	matrix1._21 * matrix2._13 +
	                matrix1._22 * matrix2._23 +
	                matrix1._23 * matrix2._33 +
	                matrix1._24 * matrix2._43;
    float _24 = 	matrix1._21 * matrix2._14 +
	                matrix1._22 * matrix2._24 +
	                matrix1._23 * matrix2._34 +
	                matrix1._24 * matrix2._44;
    float _31 = 	matrix1._31 * matrix2._11 +
	                matrix1._32 * matrix2._21 +
	                matrix1._33 * matrix2._31 +
	                matrix1._34 * matrix2._41;
    float _32 = 	matrix1._31 * matrix2._12 +
	                matrix1._32 * matrix2._22 +
	                matrix1._33 * matrix2._32 +
	                matrix1._34 * matrix2._42;
    float _33 = 	matrix1._31 * matrix2._13 +
	                matrix1._32 * matrix2._23 +
	                matrix1._33 * matrix2._33 +
	                matrix1._34 * matrix2._43;
    float _34 = 	matrix1._31 * matrix2._14 +
	                matrix1._32 * matrix2._24 +
	                matrix1._33 * matrix2._34 +
	                matrix1._34 * matrix2._44;
    float _41 = 	matrix1._41 * matrix2._11 +
	                matrix1._42 * matrix2._21 +
	                matrix1._43 * matrix2._31 +
	                matrix1._44 * matrix2._41;
    float _42 = 	matrix1._41 * matrix2._12 +
	                matrix1._42 * matrix2._22 +
	                matrix1._43 * matrix2._32 +
	                matrix1._44 * matrix2._42;
    float _43 = 	matrix1._41 * matrix2._13 +
	                matrix1._42 * matrix2._23 +
	                matrix1._43 * matrix2._33 +
	                matrix1._44 * matrix2._43;
    float _44 = 	matrix1._41 * matrix2._14 +
	                matrix1._42 * matrix2._24 +
	                matrix1._43 * matrix2._34 +
	                matrix1._44 * matrix2._44;

    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._14 = _14;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._24 = _24;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
    result._34 = _34;
    result._41 = _41;
    result._42 = _42;
    result._43 = _43;
    result._44 = _44;
}

void AxMatrix::Multiply3x3(AxMatrix &result, AxMatrix &matrix1, AxMatrix &matrix2)
{
    float _11 = 	matrix1._11 * matrix2._11 +
	                matrix1._12 * matrix2._21 +
	                matrix1._13 * matrix2._31;
    float _12 = 	matrix1._11 * matrix2._12 +
	                matrix1._12 * matrix2._22 +
	                matrix1._13 * matrix2._32;
    float _13 = 	matrix1._11 * matrix2._13 +
	                matrix1._12 * matrix2._23 +
	                matrix1._13 * matrix2._33;
    float _21 = 	matrix1._21 * matrix2._11 +
	                matrix1._22 * matrix2._21 +
	                matrix1._23 * matrix2._31;
    float _22 = 	matrix1._21 * matrix2._12 +
	                matrix1._22 * matrix2._22 +
	                matrix1._23 * matrix2._32;
    float _23 = 	matrix1._21 * matrix2._13 +
	                matrix1._22 * matrix2._23 +
	                matrix1._23 * matrix2._33;
    float _31 = 	matrix1._31 * matrix2._11 +
	                matrix1._32 * matrix2._21 +
	                matrix1._33 * matrix2._31;
    float _32 = 	matrix1._31 * matrix2._12 +
	                matrix1._32 * matrix2._22 +
	                matrix1._33 * matrix2._32;
    float _33 = 	matrix1._31 * matrix2._13 +
	                matrix1._32 * matrix2._23 +
	                matrix1._33 * matrix2._33;

    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
}

void AxMatrix::Transpose(AxMatrix &result, AxMatrix &source)
{
    float _11 = source._11;
    float _12 = source._21;
    float _13 = source._31;
    float _14 = source._41;
    float _21 = source._12;
    float _22 = source._22;
    float _23 = source._32;
    float _24 = source._42;
    float _31 = source._13;
    float _32 = source._23;
    float _33 = source._33;
    float _34 = source._43;
    float _41 = source._14;
    float _42 = source._24;
    float _43 = source._34;
    float _44 = source._44;
        
    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._14 = _14;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._24 = _24;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
    result._34 = _34;
    result._41 = _41;
    result._42 = _42;
    result._43 = _43;
    result._44 = _44;
}
    
void AxMatrix::Transpose3x3(AxMatrix &result, AxMatrix &source)
{
    float _11 = source._11;
    float _12 = source._21;
    float _13 = source._31;
    float _21 = source._12;
    float _22 = source._22;
    float _23 = source._32;
    float _31 = source._13;
    float _32 = source._23;
    float _33 = source._33;
        
    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
}
    
void AxMatrix::Invert(AxMatrix &result, AxMatrix &source)
{
	float det = source.GetDeterminant();
    
	if (det == 0.0f)
        return;

    det = 1.0f / det;

    float _11 = det * (source._22 * source._33 * source._44 +
                       source._23 * source._34 * source._42 +
                       source._24 * source._32 * source._43 -
                       source._22 * source._34 * source._43 -
                       source._23 * source._32 * source._44 -
                       source._24 * source._33 * source._42);

	float _12 = det * (source._12 * source._34 * source._43 +
                       source._13 * source._32 * source._44 +
                       source._14 * source._33 * source._42 -
                       source._12 * source._33 * source._44 -
                       source._13 * source._34 * source._42 -
                       source._14 * source._32 * source._43);

    float _13 = det * (source._12 * source._23 * source._44 +
                       source._13 * source._24 * source._42 +
                       source._14 * source._22 * source._43 -
                       source._12 * source._24 * source._43 -
                       source._13 * source._22 * source._44 -
                       source._14 * source._23 * source._42);
	
	float _14 = det * (source._12 * source._24 * source._33 +
                       source._13 * source._22 * source._34 +
                       source._14 * source._23 * source._32 -
                       source._12 * source._23 * source._34 -
                       source._13 * source._24 * source._32 -
                       source._14 * source._22 * source._33);
	
	float _21 = det * (source._21 * source._34 * source._43 +
                       source._23 * source._31 * source._44 +
                       source._24 * source._33 * source._41 -
                       source._21 * source._33 * source._44 -
                       source._23 * source._34 * source._41 -
                       source._24 * source._31 * source._43);
	
	float _22 = det * (source._11 * source._33 * source._44 +
                       source._13 * source._34 * source._41 +
                       source._14 * source._31 * source._43 -
                       source._11 * source._34 * source._43 -
                       source._13 * source._31 * source._44 -
                       source._14 * source._33 * source._41);
	
	float _23 = det * (source._11 * source._24 * source._43 +
                       source._13 * source._21 * source._44 +
                       source._14 * source._23 * source._41 -
                       source._11 * source._23 * source._44 -
                       source._13 * source._24 * source._41 -
                       source._14 * source._21 * source._43);
	
	float _24 = det * (source._11 * source._23 * source._34 +
                       source._13 * source._24 * source._31 +
                       source._14 * source._21 * source._33 -
                       source._11 * source._24 * source._33 -
                       source._13 * source._21 * source._34 -
                       source._14 * source._23 * source._31);
	
	float _31 = det * (source._21 * source._32 * source._44 +
                       source._22 * source._34 * source._41 +
                       source._24 * source._31 * source._42 -
                       source._21 * source._34 * source._42 -
                       source._22 * source._31 * source._44 -
                       source._24 * source._32 * source._41);
	
	float _32 = det * (source._11 * source._34 * source._42 +
                       source._12 * source._31 * source._44 +
                       source._14 * source._32 * source._41 -
                       source._11 * source._32 * source._44 -
                       source._12 * source._34 * source._41 -
                       source._14 * source._31 * source._42);
	
	float _33 = det * (source._11 * source._22 * source._44 +
                       source._12 * source._24 * source._41 +
                       source._14 * source._21 * source._42 -
                       source._11 * source._24 * source._42 -
                       source._12 * source._21 * source._44 -
                       source._14 * source._22 * source._41);
	
	float _34 = det * (source._11 * source._24 * source._32 +
                       source._12 * source._21 * source._34 +
                       source._14 * source._22 * source._31 -
                       source._11 * source._22 * source._34 -
                       source._12 * source._24 * source._31 -
                       source._14 * source._21 * source._32);
	
	float _41 = det * (source._21 * source._33 * source._42 +
                       source._22 * source._31 * source._43 +
                       source._23 * source._32 * source._41 -
                       source._21 * source._32 * source._43 -
                       source._22 * source._33 * source._41 -
                       source._23 * source._31 * source._42);
	
	float _42 = det * (source._11 * source._32 * source._43 +
                       source._12 * source._33 * source._41 +
                       source._13 * source._31 * source._42 -
                       source._11 * source._33 * source._42 -
                       source._12 * source._31 * source._43 -
                       source._13 * source._32 * source._41);
	
	float _43 = det * (source._11 * source._23 * source._42 +
                       source._12 * source._21 * source._43 +
                       source._13 * source._22 * source._41 -
                       source._11 * source._22 * source._43 -
                       source._12 * source._23 * source._41 -
                       source._13 * source._21 * source._42);
	
	float _44 = det * (source._11 * source._22 * source._33 +
	                   source._12 * source._23 * source._31 +
	                   source._13 * source._21 * source._32 -
	                   source._11 * source._23 * source._32 -
	                   source._12 * source._21 * source._33 -
	                   source._13 * source._22 * source._31);
        
    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._14 = _14;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._24 = _24;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
    result._34 = _34;
    result._41 = _41;
    result._42 = _42;
    result._43 = _43;
    result._44 = _44;
}

void AxMatrix::Invert3x3(AxMatrix &result, AxMatrix &source)
{
    float det = source._12 * source._23 * source._31 +
                source._13 * source._21 * source._32 -
                source._11 * source._23 * source._32 -
                source._12 * source._21 * source._33 -
                source._13 * source._22 * source._31;

    if (det == 0.0f)
        return;

    det = 1.0f / det;

    float _11 = det * (source._22 * source._33 - source._23 * source._32);
    float _12 = det * (source._13 * source._32 - source._12 * source._33);
    float _13 = det * (source._12 * source._23 - source._13 * source._22);
    float _21 = det * (source._23 * source._31 - source._21 * source._33);
    float _22 = det * (source._11 * source._33 - source._13 * source._31);
    float _23 = det * (source._13 * source._21 - source._11 * source._23);
    float _31 = det * (source._21 * source._32 - source._22 * source._31);
    float _32 = det * (source._12 * source._31 - source._11 * source._32);
    float _33 = det * (source._11 * source._22 - source._12 * source._21);

	result._11 = _11;
	result._12 = _12;
	result._13 = _13;
	result._21 = _21;
	result._22 = _22;
	result._23 = _23;
	result._31 = _31;
	result._32 = _32;
	result._33 = _33;
}

void AxMatrix::InvertTranspose3x3(AxMatrix &result, AxMatrix &source)
{
    float det = source._12 * source._23 * source._31 +
                source._13 * source._21 * source._32 -
                source._11 * source._23 * source._32 -
                source._12 * source._21 * source._33 -
                source._13 * source._22 * source._31;

    if (det == 0.0f)
        return;

    det = 1.0f / det;

    float _11 = det * (source._22 * source._33 - source._23 * source._32);
    float _12 = det * (source._13 * source._32 - source._12 * source._33);
    float _13 = det * (source._12 * source._23 - source._13 * source._22);
    float _21 = det * (source._23 * source._31 - source._21 * source._33);
    float _22 = det * (source._11 * source._33 - source._13 * source._31);
    float _23 = det * (source._13 * source._21 - source._11 * source._23);
    float _31 = det * (source._21 * source._32 - source._22 * source._31);
    float _32 = det * (source._12 * source._31 - source._11 * source._32);
    float _33 = det * (source._11 * source._22 - source._12 * source._21);

	result._11 = _11;
	result._12 = _21;
	result._13 = _31;
	result._21 = _12;
	result._22 = _22;
	result._23 = _32;
	result._31 = _13;
	result._32 = _23;
	result._33 = _33;
}

void AxMatrix::GetAxisX(AxVector3 &result, AxMatrix &source)
{
	result.x = source._11;
	result.y = source._12;
	result.z = source._13;
}

void AxMatrix::SetAxisX(AxMatrix &result, AxVector3 &source)
{
	result._11 = source.x;
	result._12 = source.y;
	result._13 = source.z;
}

void AxMatrix::GetAxisY(AxVector3 &result, AxMatrix &source)
{
	result.x = source._21;
	result.y = source._22;
	result.z = source._23;
}

void AxMatrix::SetAxisY(AxMatrix &result, AxVector3 &source)
{
	result._21 = source.x;
	result._22 = source.y;
	result._23 = source.z;
}

void AxMatrix::GetAxisZ(AxVector3 &result, AxMatrix &source)
{
	result.x = source._31;
	result.y = source._32;
	result.z = source._33;
}

void AxMatrix::SetAxisZ(AxMatrix &result, AxVector3 &source)
{
	result._31 = source.x;
	result._32 = source.y;
	result._33 = source.z;
}

void AxMatrix::GetTranslation(AxVector3 &result, AxMatrix &source)
{
	result.x = source._41;
	result.y = source._42;
	result.z = source._43;
}

void AxMatrix::SetTranslation(AxMatrix &result, AxVector3 &translation)
{
	result._41 = translation.x;
	result._42 = translation.y;
	result._43 = translation.z;
}

void AxMatrix::GetScaling(AxVector3 &result, AxMatrix &source)
{
	result.x = AxMath::Sqrt(source._11 * source._11 + source._12 * source._12 + source._13 * source._13);
	result.y = AxMath::Sqrt(source._21 * source._21 + source._22 * source._22 + source._23 * source._23);
	result.z = AxMath::Sqrt(source._31 * source._31 + source._32 * source._32 + source._33 * source._33);
}

void AxMatrix::SetScaling(AxMatrix &result, AxVector3 &scaling)
{
	float factor;

	factor = AxMath::Sqrt(result._11 * result._11 + result._12 * result._12 + result._13 * result._13);
	if (factor != 0)
	{
		factor = scaling.x / factor;
		result._11 *= factor;
		result._12 *= factor;
		result._13 *= factor;
	}

	factor = AxMath::Sqrt(result._21 * result._21 + result._22 * result._22 + result._23 * result._23);
	if (factor != 0)
	{
		factor = scaling.y / factor;
		result._21 *= factor;
		result._22 *= factor;
		result._23 *= factor;
	}

	factor = AxMath::Sqrt(result._31 * result._31 + result._32 * result._32 + result._33 * result._33);
	if (factor != 0)
	{
		factor = scaling.z / factor;
		result._31 *= factor;
		result._32 *= factor;
		result._33 *= factor;
	}
}

void AxMatrix::ExtractEulerRotationXYZ(AxVector3 &result, AxMatrix &source)
{
	result.x = AxMath::ArcTan2(source._23, source._33);
	
	float cosY = AxMath::Sqrt(source._11 * source._11 + source._12 * source._12);
	result.y = AxMath::ArcTan2(-source._13, cosY);
	
	float sinX = AxMath::Sin(result.x);
	float cosX = AxMath::Cos(result.x);
	result.z = AxMath::ArcTan2(sinX * source._31 - cosX * source._21, cosX * source._22 - sinX * source._32);
}