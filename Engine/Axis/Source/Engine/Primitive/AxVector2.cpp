//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxVector2.h"

#include "..\..\Tools\AxMath.h"

#include "AxMatrix.h"

AxVector2::AxVector2(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
}

AxVector2::AxVector2(float value)
{
	this->x = value;
	this->y = value;
}

AxVector2::AxVector2(float x, float y)
{
	this->x = x;
	this->y = y;
}
	
AxVector2::AxVector2(AxVector2 &source)
{
	this->x = source.x;
	this->y = source.y;
}
	
AxVector2::~AxVector2(void)
{
}

void AxVector2::Set(float value)
{
	this->x = value;
	this->y = value;
}

void AxVector2::Set(float x, float y)
{
	this->x = x;
	this->y = y;
}
	
void AxVector2::Set(AxVector2 &vector)
{
	this->x = vector.x;
	this->y = vector.y;
}

bool AxVector2::Equals(AxVector2 &vector)
{
	return
		(this->x == vector.x) &&
		(this->y == vector.y);
}

float AxVector2::GetLength()
{
    return AxMath::Sqrt(this->x * this->x + this->y * this->y);
}

AxVector2 AxVector2::OfLength(float length)
{
	AxVector2 result;
	AxVector2::SetLength(result, *this, length);
	return result;
}

AxVector2 AxVector2::Normalize()
{
	AxVector2 result;
	AxVector2::Normalize(result, *this);
	return result;
}

AxVector2 AxVector2::Invert()
{
	AxVector2 result;
	AxVector2::Invert(result, *this);
	return result;
}

float AxVector2::DistanceTo(AxVector2 &v)
{
	return AxVector2::Distance(*this, v);
}

AxVector2 AxVector2::Lerp(AxVector2 &v, float factor)
{
	AxVector2 result;
	AxVector2::Lerp(result, *this, v, factor);
	return result;
}

AxVector2 AxVector2::Subtract(AxVector2 &v)
{
	AxVector2 result;
	AxVector2::Subtract(result, *this, v);
	return result;
}

AxVector2 AxVector2::Add(AxVector2 &v)
{
	AxVector2 result;
	AxVector2::Add(result, *this, v);
	return result;
}

float AxVector2::Dot(AxVector2 &v)
{
	return AxVector2::Dot(*this, v);
}

AxVector2 AxVector2::Cross()
{
	AxVector2 result;
	AxVector2::Cross(result, *this);
	return result;
}

AxVector2 AxVector2::Scale(AxVector2 &v)
{
	AxVector2 result;
	AxVector2::Scale(result, *this, v);
	return result;
}

AxVector2 AxVector2::Scale(float factor)
{
	AxVector2 result;
	AxVector2::Scale(result, *this, factor);
	return result;
}

AxVector2 AxVector2::Min(AxVector2 &v)
{
	AxVector2 result;
	AxVector2::Min(result, *this, v);
	return result;
}

AxVector2 AxVector2::Max(AxVector2 &v)
{
	AxVector2 result;
	AxVector2::Max(result, *this, v);
	return result;
}

AxVector2 AxVector2::Transform(AxMatrix &transformation)
{
	AxVector2 result;
	AxVector2::Transform(result, *this, transformation);
	return result;
}



// Static Methods



void AxVector2::Copy(AxVector2 &result, AxVector2 &source)
{
    result.x = source.x;
    result.y = source.y;
}

void AxVector2::SetLength(AxVector2 &result, AxVector2 &v, float length)
{
    float k = AxMath::Sqrt(v.x * v.x + v.y * v.y);

    if (k != 0.0f)
    {
        k = length / k;

        result.x = v.x * k;
        result.y = v.y * k;
    }
}

void AxVector2::Normalize(AxVector2 &result, AxVector2 &v)
{
	AxVector2::SetLength(result, v, 1.0f);
}

void AxVector2::Invert(AxVector2 &result, AxVector2 &v)
{
	result.x = -v.x;
	result.y = -v.y;
}

float AxVector2::Distance(AxVector2 &v1, AxVector2 &v2)
{
    float x = v1.x - v2.x;
    float y = v1.y - v2.y;

    return (float)(AxMath::Sqrt(x * x + y * y));
}
	
void AxVector2::Lerp(AxVector2 &result, AxVector2 &v1, AxVector2 &v2, float factor)
{
    result.x = v1.x + (v2.x - v1.x) * factor;
    result.y = v1.y + (v2.y - v1.y) * factor;
}

void AxVector2::Subtract(AxVector2 &result, AxVector2 &v1, AxVector2 &v2)
{
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
}

void AxVector2::Add(AxVector2 &result, AxVector2 &v1, AxVector2 &v2)
{
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
}

float AxVector2::Dot(AxVector2 &v1, AxVector2 &v2)
{
    return (v1.x * v2.x +
            v1.y * v2.y);
}

void AxVector2::Cross(AxVector2 &result, AxVector2 &v1)
{
    result.x = -v1.y;
    result.y = v1.x;
}
    
void AxVector2::Scale(AxVector2 &result, AxVector2 &v1, AxVector2 &v2)
{
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
}

void AxVector2::Scale(AxVector2 &result, AxVector2 &v1, float factor)
{
    result.x = v1.x * factor;
    result.y = v1.y * factor;
}
    
void AxVector2::Min(AxVector2 &result, AxVector2 &v1, AxVector2 &v2)
{
    result.x = v1.x < v2.x ? v1.x : v2.x;
    result.y = v1.y < v2.y ? v1.y : v2.y;
}
    
void AxVector2::Max(AxVector2 &result, AxVector2 &v1, AxVector2 &v2)
{
    result.x = v1.x > v2.x ? v1.x : v2.x;
    result.y = v1.y > v2.y ? v1.y : v2.y;
}
    
void AxVector2::Transform(AxVector2 &result, AxVector2 &v, AxMatrix &transformation)
{
    float x = 	v.x * transformation._11 +
    		    v.y * transformation._21 +
					   transformation._31 +
					   transformation._41;
    float y = 	v.x * transformation._12 +
        		v.y * transformation._22 +
        			   transformation._32 +
        			   transformation._42;
        
    result.x = x;
    result.y = y;
}

void AxVector2::TransformNormal(AxVector2 &result, AxVector2 &v, AxMatrix &transformation)
{
    float x =	v.x * transformation._11 +
			    v.y * transformation._21;
	float y =	v.x * transformation._12 +
			    v.y * transformation._22;

	result.x = x;
    result.y = y;
}