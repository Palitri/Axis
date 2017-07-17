#pragma once

#include "..\..\AxGlobals.h"

class AxMatrix;

class AXDLLCLASS AxVector2
{
public:
	float x, y;

	AxVector2(void);
	AxVector2(float value);
	AxVector2(float x, float y);
	AxVector2(AxVector2 &source);
	~AxVector2(void);

	void Set(float value);
	void Set(float x, float y);
	void Set(AxVector2 &vector);
	bool Equals(AxVector2 &vector);
	float GetLength();
	AxVector2 OfLength(float length);
	AxVector2 Normalize();
	AxVector2 Invert();
	float DistanceTo(AxVector2 &v);
	AxVector2 Lerp(AxVector2 &v, float factor);
	AxVector2 Subtract(AxVector2 &v);
	AxVector2 Add(AxVector2 &v);
	float Dot(AxVector2 &v);
	AxVector2 Cross();
	AxVector2 Scale(AxVector2 &v);
	AxVector2 Scale(float factor);
	AxVector2 Min(AxVector2 &v);
	AxVector2 Max(AxVector2 &v);
	AxVector2 Transform(AxMatrix &transformation);

	static void Copy(AxVector2 &result, AxVector2 &source);
	static void SetLength(AxVector2 &result, AxVector2 &v, float length);
	static void Normalize(AxVector2 &result, AxVector2 &v);
	static void Invert(AxVector2 &result, AxVector2 &v);
	static float Distance(AxVector2 &v1, AxVector2 &v2);
	static void Lerp(AxVector2 &result, AxVector2 &v1, AxVector2 &v2, float factor);
	static void Subtract(AxVector2 &result, AxVector2 &v1, AxVector2 &v2);
	static void Add(AxVector2 &result, AxVector2 &v1, AxVector2 &v2);
	static float Dot(AxVector2 &v1, AxVector2 &v2);
	static void Cross(AxVector2 &result, AxVector2 &v1);
	static void Scale(AxVector2 &result, AxVector2 &v1, AxVector2 &v2);
	static void Scale(AxVector2 &result, AxVector2 &v1, float factor);
	static void Min(AxVector2 &result, AxVector2 &v1, AxVector2 &v2);
	static void Max(AxVector2 &result, AxVector2 &v1, AxVector2 &v2);
	static void Transform(AxVector2 &result, AxVector2 &v, AxMatrix &transformation);
	static void TransformNormal(AxVector2 &result, AxVector2 &v, AxMatrix &transformation);
};

