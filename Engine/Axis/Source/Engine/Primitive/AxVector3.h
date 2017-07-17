#pragma once

#include "..\..\AxGlobals.h"

class AxMatrix;
class AxVector2;

class AXDLLCLASS AxVector3
{
public:
	float x, y, z;

	AxVector3(void);
	AxVector3(float value);
	AxVector3(float x, float y, float z);
	AxVector3(AxVector2 &source, float z);
	AxVector3(AxVector3 &source);
	~AxVector3(void);

	void Set(float value);
	void Set(float x, float y, float z);
	void Set(AxVector2 vector, float z);
	void Set(AxVector3 vector);
	bool Equals(AxVector3 &vector);
	float GetLength();
	AxVector3 OfLength(float length);
	AxVector3 Normalize();
	AxVector3 Invert();
	float DistanceTo(AxVector3 &v);
	AxVector3 Lerp(AxVector3 &v, float factor);
	AxVector3 Subtract(AxVector3 &v);
	AxVector3 Add(AxVector3 &v);
	float Dot(AxVector3 &v);
	AxVector3 Cross(AxVector3 &v);
	AxVector3 Scale(AxVector3 &v);
	AxVector3 Scale(float factor);
	AxVector3 Min(AxVector3 &v);
	AxVector3 Max(AxVector3 &v);
	AxVector3 Transform(AxMatrix &transformation);
	AxVector3 CartesianToSpherical();
	AxVector3 SphericalToCartesian();
	AxVector3 LerpSherical(AxVector3 &s, float factor);
	
	static void Copy(AxVector3 &result, AxVector3 &source);
	static void SetLength(AxVector3 &result, AxVector3 &v, float length);
	static void Normalize(AxVector3 &result, AxVector3 &v);
	static void Invert(AxVector3 &result, AxVector3 &v);
	static float Distance(AxVector3 &v1, AxVector3 &v2);
	static void Lerp(AxVector3 &result, AxVector3 &v1, AxVector3 &v2, float factor);
	static void Subtract(AxVector3 &result, AxVector3 &v1, AxVector3 &v2);
	static void Add(AxVector3 &result, AxVector3 &v1, AxVector3 &v2);
	static float Dot(AxVector3 &v1, AxVector3 &v2);
	static void Cross(AxVector3 &result, AxVector3 &v1, AxVector3 &v2);
	static void Scale(AxVector3 &result, AxVector3 &v1, AxVector3 &v2);
	static void Scale(AxVector3 &result, AxVector3 &v1, float factor);
	static void Min(AxVector3 &result, AxVector3 &v1, AxVector3 &v2);
	static void Max(AxVector3 &result, AxVector3 &v1, AxVector3 &v2);
	static void Transform(AxVector3 &result, AxVector3 &v, AxMatrix &transformation);
	static void TransformNormal(AxVector3 &result, AxVector3 &v, AxMatrix &transformation);

	static void CartesianToSpherical(AxVector3 &result, float x, float y, float z);
	static void CartesianToSpherical(AxVector3 &result, AxVector3 &cartesian);
	static void SphericalToCartesian(AxVector3 &result, float azimuth, float elevation, float radius);
	static void SphericalToCartesian(AxVector3 &result, AxVector3 &spherical);
	static void LerpSherical(AxVector3 &result, AxVector3 &v1, AxVector3 &v2, float factor);
};

