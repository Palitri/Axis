#pragma once

#include "..\..\AxGlobals.h"

class AxVector3;
class AxVector4;

class AXDLLCLASS AxMatrix
{
public:
	static const int handedness_Left	= 0;
	static const int handedness_Right	= 1;

	static int handedness;

	float _11, _12, _13, _14,
          _21, _22, _23, _24,
          _31, _32, _33, _34,
          _41, _42, _43, _44;	
	
	AxMatrix(void);
	AxMatrix(AxMatrix &source);
	~AxMatrix(void);
	
	float GetDeterminant();
	bool Equals(AxMatrix &matrix);
	bool IsIdentity();

	static void Copy(AxMatrix &result, AxMatrix &source);
	static void CreateIdentity(AxMatrix &result);
	static void CreateRotationX(AxMatrix &result, float phi);
	static void CreateRotationY(AxMatrix &result, float phi);
	static void CreateRotationZ(AxMatrix &result, float phi);
	static void CreateRotationAxis(AxMatrix &result, float x, float y, float z, float phi);
	static void CreateRotationAxis(AxMatrix &result, AxVector3 &axis, float phi);
	static void CreateTranslation(AxMatrix &result, float x, float y, float z);
	static void CreateTranslation(AxMatrix &result, AxVector3 &translation);
	static void CreateScaling(AxMatrix &result, float x, float y, float z);
	static void CreateScaling(AxMatrix &result, AxVector3 &scaling);
	static void CreateLookAt(AxMatrix &result, AxVector3 &position, AxVector3 &lookAt, AxVector3 &up);
	static void CreatePerspectiveFieldOfView(AxMatrix &result, float fov, float aspectHorizontal, float aspectVertical, float zn, float zf);
	static void CreatePerspectiveFieldOfViewRH(AxMatrix &result, float fov, float aspectHorizontal, float aspectVertical, float zn, float zf);
	static void CreatePerspectiveFieldOfViewLH(AxMatrix &result, float fov, float aspectHorizontal, float aspectVertical, float zn, float zf);
	static void CreateOrthographic(AxMatrix &result, float width, float height, float zn, float zf);
	static void CreateOrthographicRH(AxMatrix &result, float width, float height, float zn, float zf);
	static void CreateOrthographicLH(AxMatrix &result, float width, float height, float zn, float zf);
	static void CreateOrientation(AxMatrix &result, AxVector3 &orientationVector, AxVector3 &orientationOrigin);
	static void CreateOrientedPosition(AxMatrix &result, AxVector3 &position, AxVector3 &target, AxVector3 &orientationOrigin);
	static void CreateOrientationAxis(AxMatrix &result, AxVector3 &orientationVector, AxVector3 &orientationOrigin, AxVector3 &axis);
	static void CreateOrientationAxis(AxMatrix &result, AxVector3 &position, AxVector3 &target, AxVector3 &orientationOrigin, AxVector3 &axis);
	static void CreateReflectionPlane(AxMatrix &result, AxVector4 &plane);
	static void Multiply(AxMatrix &result, AxMatrix &matrix1, AxMatrix &matrix2);
	static void Multiply3x3(AxMatrix &result, AxMatrix &matrix1, AxMatrix &matrix2);
	static void Transpose(AxMatrix &result, AxMatrix &source);
	static void Transpose3x3(AxMatrix &result, AxMatrix &source);
	static void Invert(AxMatrix &result, AxMatrix &source);
	static void Invert3x3(AxMatrix &result, AxMatrix &source);
	static void InvertTranspose3x3(AxMatrix &result, AxMatrix &source);
	static void GetAxisX(AxVector3 &result, AxMatrix &source);
	static void SetAxisX(AxMatrix &result, AxVector3 &source);
	static void GetAxisY(AxVector3 &result, AxMatrix &source);
	static void SetAxisY(AxMatrix &result, AxVector3 &source);
	static void GetAxisZ(AxVector3 &result, AxMatrix &source);
	static void SetAxisZ(AxMatrix &result, AxVector3 &source);
	static void GetTranslation(AxVector3 &result, AxMatrix &source);
	static void SetTranslation(AxMatrix &result, AxVector3 &translation);
	static void GetScaling(AxVector3 &result, AxMatrix &source);
	static void SetScaling(AxMatrix &result, AxVector3 &scaling);
	static void ExtractEulerRotationXYZ(AxVector3 &result, AxMatrix &source);
};

