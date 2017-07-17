#pragma once

#include "..\..\AxGlobals.h"

class AxMatrix;
class AxVector3;
class AxVector2;

class AXDLLCLASS AxVector4
{
public:
	float x, y, z, w;

	AxVector4(void);
	AxVector4(float value);
	AxVector4(float x, float y, float z, float w);
	AxVector4(AxVector2 &source, float z, float w);
	AxVector4(AxVector3 &source, float w);
	AxVector4(AxVector4 &source);
	~AxVector4(void);

	void Set(float value);
	void Set(float x, float y, float z, float w);
	void Set(AxVector2 vector, float z, float w);
	void Set(AxVector3 vector, float w);
	void Set(AxVector4 vector);
	bool Equals(AxVector4 &vector);
	float GetLength();
	AxVector4 OfLength(float length);
	AxVector4 Normalize();
	AxVector4 Invert();
	float DistanceTo(AxVector4 &v);
	AxVector4 Lerp(AxVector4 &v, float factor);
	AxVector4 Subtract(AxVector4 &v);
	AxVector4 Add(AxVector4 &v);
	float Dot(AxVector4 &v);
	AxVector4 Cross(AxVector4 &v1, AxVector4 &v2);
	AxVector4 Scale(AxVector4 &v);
	AxVector4 Scale(float factor);
	AxVector4 Min(AxVector4 &v);
	AxVector4 Max(AxVector4 &v);
	AxVector4 Transform(AxMatrix &transformation);

	static void Copy(AxVector4 &result, AxVector4 &source);
	static void SetLength(AxVector4 &result, AxVector4 &v, float length);
	static void Normalize(AxVector4 &result, AxVector4 &v);
	static void Invert(AxVector4 &result, AxVector4 &v);
	static float Distance(AxVector4 &v1, AxVector4 &v2);
	static void Lerp(AxVector4 &result, AxVector4 &v1, AxVector4 &v2, float factor);
	static void Subtract(AxVector4 &result, AxVector4 &v1, AxVector4 &v2);
	static void Add(AxVector4 &result, AxVector4 &v1, AxVector4 &v2);
	static float Dot(AxVector4 &v1, AxVector4 &v2);
	static void Cross(AxVector4 &result, AxVector4 &v1, AxVector4 &v2, AxVector4 &v3);
	static void Scale(AxVector4 &result, AxVector4 &v1, AxVector4 &v2);
	static void Scale(AxVector4 &result, AxVector4 &v1, float factor);
	static void Min(AxVector4 &result, AxVector4 &v1, AxVector4 &v2);
	static void Max(AxVector4 &result, AxVector4 &v1, AxVector4 &v2);
	static void Transform(AxVector4 &result, AxVector4 &v, AxMatrix &transformation);
	static void Transform(AxVector4 &result, AxVector3 &v, AxMatrix &transformation);
};

