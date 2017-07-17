//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxVector4.h"

#include "..\..\Tools\AxMath.h"

#include "AxMatrix.h"
#include "AxVector3.h"
#include "AxVector2.h"

AxVector4::AxVector4(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}
	
AxVector4::AxVector4(float value)
{
	this->x = value;
	this->y = value;
	this->z = value;
	this->w = value;
}

AxVector4::AxVector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

AxVector4::AxVector4(AxVector2 &source, float z, float w)
{
	this->x = source.x;
	this->y = source.y;
	this->z = z;
	this->w = w;
}

AxVector4::AxVector4(AxVector3 &source, float w)
{
	this->x = source.x;
	this->y = source.y;
	this->z = source.z;
	this->w = w;
}

AxVector4::AxVector4(AxVector4 &source)
{
	this->x = source.x;
	this->y = source.y;
	this->z = source.z;
	this->w = source.w;
}

AxVector4::~AxVector4(void)
{
}

void AxVector4::Set(float value)
{
	this->x = value;
	this->y = value;
	this->z = value;
	this->w = value;
}

void AxVector4::Set(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

void AxVector4::Set(AxVector2 vector, float z, float w)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = z;
	this->w = w;
}

void AxVector4::Set(AxVector3 vector, float w)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	this->w = w;
}

void AxVector4::Set(AxVector4 vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	this->w = vector.w;
}

bool AxVector4::Equals(AxVector4 &vector)
{
	return
		(this->x == vector.x) &&
		(this->y == vector.y) &&
		(this->z == vector.z) &&
		(this->w == vector.w);
}

float AxVector4::GetLength()
{
    return (float)(AxMath::Sqrt(this->x * this->x + this->y * this->y + this->z * this->z + this->w * this->w));
}
	
AxVector4 AxVector4::OfLength(float length)
{
	AxVector4 result;
	AxVector4::SetLength(result, *this, length);
	return result;
}

AxVector4 AxVector4::Normalize()
{
	AxVector4 result;
	AxVector4::Normalize(result, *this);
	return result;
}

AxVector4 AxVector4::Invert()
{
	AxVector4 result;
	AxVector4::Invert(result, *this);
	return result;
}

float AxVector4::DistanceTo(AxVector4 &v)
{
	return AxVector4::Distance(*this, v);
}

AxVector4 AxVector4::Lerp(AxVector4 &v, float factor)
{
	AxVector4 result;
	AxVector4::Lerp(result, *this, v, factor);
	return result;
}

AxVector4 AxVector4::Subtract(AxVector4 &v)
{
	AxVector4 result;
	AxVector4::Subtract(result, *this, v);
	return result;
}

AxVector4 AxVector4::Add(AxVector4 &v)
{
	AxVector4 result;
	AxVector4::Add(result, *this, v);
	return result;
}

float AxVector4::Dot(AxVector4 &v)
{
	return AxVector4::Dot(*this, v);
}

AxVector4 AxVector4::Cross(AxVector4 &v1, AxVector4 &v2)
{
	AxVector4 result;
	AxVector4::Cross(result, *this, v1, v2);
	return result;
}

AxVector4 AxVector4::Scale(AxVector4 &v)
{
	AxVector4 result;
	AxVector4::Scale(result, *this, v);
	return result;
}

AxVector4 AxVector4::Scale(float factor)
{
	AxVector4 result;
	AxVector4::Scale(result, *this, factor);
	return result;
}

AxVector4 AxVector4::Min(AxVector4 &v)
{
	AxVector4 result;
	AxVector4::Min(result, *this, v);
	return result;
}

AxVector4 AxVector4::Max(AxVector4 &v)
{
	AxVector4 result;
	AxVector4::Max(result, *this, v);
	return result;
}

AxVector4 AxVector4::Transform(AxMatrix &transformation)
{
	AxVector4 result;
	AxVector4::Transform(result, *this, transformation);
	return result;
}




// Stataic Methods



void AxVector4::Copy(AxVector4 &result, AxVector4 &source)
{
    result.x = source.x;
    result.y = source.y;
    result.z = source.z;
    result.w = source.w;
}

void AxVector4::SetLength(AxVector4 &result, AxVector4 &v, float length)
{
    float k = AxMath::Sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
    if (k != 0.0f)
    {
        k = length / k;

        result.x = v.x * k;
        result.y = v.y * k;
        result.z = v.z * k;
        result.w = v.w * k;
    }
}

void AxVector4::Normalize(AxVector4 &result, AxVector4 &v)
{
	AxVector4::SetLength(result, v, 1.0f);
}
	
void AxVector4::Invert(AxVector4 &result, AxVector4 &v)
{
	result.x = -v.x;
	result.y = -v.y;
	result.z = -v.z;
	result.w = -v.w;
}

float AxVector4::Distance(AxVector4 &v1, AxVector4 &v2)
{
    float x = v1.x - v2.x;
    float y = v1.y - v2.y;
    float z = v1.z - v2.z;
    float w = v1.w - v2.w;

    return AxMath::Sqrt(x * x + y * y + z * z + w * w);
}
	
void AxVector4::Lerp(AxVector4 &result, AxVector4 &v1, AxVector4 &v2, float factor)
{
    result.x = v1.x + (v2.x - v1.x) * factor;
    result.y = v1.y + (v2.y - v1.y) * factor;
    result.z = v1.z + (v2.z - v1.z) * factor;
    result.w = v1.w + (v2.w - v1.w) * factor;
}

void AxVector4::Subtract(AxVector4 &result, AxVector4 &v1, AxVector4 &v2)
{
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    result.w = v1.w - v2.w;
}

void AxVector4::Add(AxVector4 &result, AxVector4 &v1, AxVector4 &v2)
{
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    result.w = v1.w + v2.w;
}

float AxVector4::Dot(AxVector4 &v1, AxVector4 &v2)
{
    return (v1.x * v2.x +
            v1.y * v2.y +
            v1.z * v2.z + 
            v1.w * v2.w);
}

void AxVector4::Cross(AxVector4 &result, AxVector4 &v1, AxVector4 &v2, AxVector4 &v3)
{
    // not implemented
}
    
void AxVector4::Scale(AxVector4 &result, AxVector4 &v1, AxVector4 &v2)
{
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
    result.w = v1.w * v2.w;
}

void AxVector4::Scale(AxVector4 &result, AxVector4 &v1, float factor)
{
    result.x = v1.x * factor;
    result.y = v1.y * factor;
    result.z = v1.z * factor;
    result.w = v1.w * factor;
}
    
void AxVector4::Min(AxVector4 &result, AxVector4 &v1, AxVector4 &v2)
{
    result.x = v1.x < v2.x ? v1.x : v2.x;
    result.y = v1.y < v2.y ? v1.y : v2.y;
    result.z = v1.z < v2.z ? v1.z : v2.z;
    result.w = v1.w < v2.w ? v1.w : v2.w;
}

void AxVector4::Max(AxVector4 &result, AxVector4 &v1, AxVector4 &v2)
{
    result.x = v1.x > v2.x ? v1.x : v2.x;
    result.y = v1.y > v2.y ? v1.y : v2.y;
    result.z = v1.z > v2.z ? v1.z : v2.z;
    result.w = v1.w > v2.w ? v1.w : v2.w;
}
    
void AxVector4::Transform(AxVector4 &result, AxVector4 &v, AxMatrix &transformation)
{
    float x = 	v.x * transformation._11 +
    		    v.y * transformation._21 +
			    v.z * transformation._31 +
			    v.w * transformation._41;
    float y = 	v.x * transformation._12 +
        		v.y * transformation._22 +
        		v.z * transformation._32 +
        		v.w * transformation._42;
    float z = 	v.x * transformation._13 +
			    v.y * transformation._23 +
			    v.z * transformation._33 +
			    v.w + transformation._43;
    float w = 	v.x * transformation._14 +
			    v.y * transformation._24 +
			    v.z * transformation._34 +
			    v.w * transformation._44;
        
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
}

void AxVector4::Transform(AxVector4 &result, AxVector3 &v, AxMatrix &transformation)
{
    float x = 	v.x * transformation._11 +
    		    v.y * transformation._21 +
			    v.z * transformation._31 +
			    transformation._41;
    float y = 	v.x * transformation._12 +
        		v.y * transformation._22 +
        		v.z * transformation._32 +
        		transformation._42;
    float z = 	v.x * transformation._13 +
			    v.y * transformation._23 +
			    v.z * transformation._33 +
			    transformation._43;
    float w = 	v.x * transformation._14 +
			    v.y * transformation._24 +
			    v.z * transformation._34 +
			    transformation._44;
        
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
}