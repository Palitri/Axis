//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxVector3.h"

#include "..\..\Tools\AxMath.h"

#include "AxMatrix.h"
#include "AxVector2.h"

AxVector3::AxVector3(void)
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

AxVector3::AxVector3(float value)
{
	this->x = value;
	this->y = value;
	this->z = value;
}

AxVector3::AxVector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
	
	
AxVector3::AxVector3(AxVector2 &source, float z)
{
	this->x = source.x;
	this->y = source.y;
	this->z = z;
}

AxVector3::AxVector3(AxVector3 &source)
{
	this->x = source.x;
	this->y = source.y;
	this->z = source.z;
}

AxVector3::~AxVector3(void)
{
}

void AxVector3::Set(float value)
{
	this->x = value;
	this->y = value;
	this->z = value;
}

void AxVector3::Set(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}
	
void AxVector3::Set(AxVector2 vector, float z)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = z;
}

void AxVector3::Set(AxVector3 vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
}

bool AxVector3::Equals(AxVector3 &vector)
{
	return
		(this->x == vector.x) &&
		(this->y == vector.y) &&
		(this->z == vector.z);
}

float AxVector3::GetLength()
{
    return AxMath::Sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
}

AxVector3 AxVector3::OfLength(float length)
{
	AxVector3 result;
	AxVector3::SetLength(result, *this, length);
	return result;
}

AxVector3 AxVector3::Normalize()
{
	AxVector3 result;
	AxVector3::Normalize(result, *this);
	return result;
}

AxVector3 AxVector3::Invert()
{
	AxVector3 result;
	AxVector3::Invert(result, *this);
	return result;
}

float AxVector3::DistanceTo(AxVector3 &v)
{
	return AxVector3::Distance(*this, v);
}

AxVector3 AxVector3::Lerp(AxVector3 &v, float factor)
{
	AxVector3 result;
	AxVector3::Lerp(result, *this, v, factor);
	return result;
}

AxVector3 AxVector3::Subtract(AxVector3 &v)
{
	AxVector3 result;
	AxVector3::Subtract(result, *this, v);
	return result;
}

AxVector3 AxVector3::Add(AxVector3 &v)
{
	AxVector3 result;
	AxVector3::Add(result, *this, v);
	return result;
}

float AxVector3::Dot(AxVector3 &v)
{
	return AxVector3::Dot(*this, v);
}

AxVector3 AxVector3::Cross(AxVector3 &v)
{
	AxVector3 result;
	AxVector3::Cross(result, *this, v);
	return result;
}

AxVector3 AxVector3::Scale(AxVector3 &v)
{
	AxVector3 result;
	AxVector3::Scale(result, *this, v);
	return result;
}

AxVector3 AxVector3::Scale(float factor)
{
	AxVector3 result;
	AxVector3::Scale(result, *this, factor);
	return result;
}

AxVector3 AxVector3::Min(AxVector3 &v)
{
	AxVector3 result;
	AxVector3::Min(result, *this, v);
	return result;
}

AxVector3 AxVector3::Max(AxVector3 &v)
{
	AxVector3 result;
	AxVector3::Max(result, *this, v);
	return result;
}

AxVector3 AxVector3::Transform(AxMatrix &transformation)
{
	AxVector3 result;
	AxVector3::Transform(result, *this, transformation);
	return result;
}

AxVector3 AxVector3::CartesianToSpherical()
{
	AxVector3 result;
	AxVector3::CartesianToSpherical(result, *this);
	return result;
}

AxVector3 AxVector3::SphericalToCartesian()
{
	AxVector3 result;
	AxVector3::SphericalToCartesian(result, *this);
	return result;
}

AxVector3 AxVector3::LerpSherical(AxVector3 &s, float factor)
{
	AxVector3 result;
	AxVector3::LerpSherical(result, *this, s, factor);
	return result;
}



// Static Methods



void AxVector3::Copy(AxVector3 &result, AxVector3 &source)
{
    result.x = source.x;
    result.y = source.y;
    result.z = source.z;
}

void AxVector3::SetLength(AxVector3 &result, AxVector3 &v, float length)
{
    float k = AxMath::Sqrt(v.x * v.x + v.y * v.y +	v.z * v.z);
    if (k != 0.0f)
    {
        k = length / k;

        result.x = v.x * k;
        result.y = v.y * k;
        result.z = v.z * k;
    }
}

void AxVector3::Normalize(AxVector3 &result, AxVector3 &v)
{
	AxVector3::SetLength(result, v, 1.0f);
}
	
void AxVector3::Invert(AxVector3 &result, AxVector3 &v)
{
	result.x = -v.x;
	result.y = -v.y;
	result.z = -v.z;
}

float AxVector3::Distance(AxVector3 &v1, AxVector3 &v2)
{
    float x = v1.x - v2.x;
    float y = v1.y - v2.y;
    float z = v1.z - v2.z;

    return (float)(AxMath::Sqrt(x * x + y * y + z * z));
}
	
void AxVector3::Lerp(AxVector3 &result, AxVector3 &v1, AxVector3 &v2, float factor)
{
    result.x = v1.x + (v2.x - v1.x) * factor;
    result.y = v1.y + (v2.y - v1.y) * factor;
    result.z = v1.z + (v2.z - v1.z) * factor;
}

void AxVector3::Subtract(AxVector3 &result, AxVector3 &v1, AxVector3 &v2)
{
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
}

void AxVector3::Add(AxVector3 &result, AxVector3 &v1, AxVector3 &v2)
{
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
}

float AxVector3::Dot(AxVector3 &v1, AxVector3 &v2)
{
    return (v1.x * v2.x +
            v1.y * v2.y +
            v1.z * v2.z);
}

void AxVector3::Cross(AxVector3 &result, AxVector3 &v1, AxVector3 &v2)
{
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
}
    
void AxVector3::Scale(AxVector3 &result, AxVector3 &v1, AxVector3 &v2)
{
    result.x = v1.x * v2.x;
    result.y = v1.y * v2.y;
    result.z = v1.z * v2.z;
}

void AxVector3::Scale(AxVector3 &result, AxVector3 &v1, float factor)
{
    result.x = v1.x * factor;
    result.y = v1.y * factor;
    result.z = v1.z * factor;
}
    
void AxVector3::Min(AxVector3 &result, AxVector3 &v1, AxVector3 &v2)
{
    result.x = v1.x < v2.x ? v1.x : v2.x;
    result.y = v1.y < v2.y ? v1.y : v2.y;
    result.z = v1.z < v2.z ? v1.z : v2.z;
}
    
void AxVector3::Max(AxVector3 &result, AxVector3 &v1, AxVector3 &v2)
{
    result.x = v1.x > v2.x ? v1.x : v2.x;
    result.y = v1.y > v2.y ? v1.y : v2.y;
    result.z = v1.z > v2.z ? v1.z : v2.z;
}
    
void AxVector3::Transform(AxVector3 &result, AxVector3 &v, AxMatrix &transformation)
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
        
    result.x = x;
    result.y = y;
    result.z = z;
}

void AxVector3::TransformNormal(AxVector3 &result, AxVector3 &v, AxMatrix &transformation)
{
    float x =	v.x * transformation._11 +
			    v.y * transformation._21 +
			    v.z * transformation._31;
	float y =	v.x * transformation._12 +
			    v.y * transformation._22 +
			    v.z * transformation._32;
	float z = 	v.x * transformation._13 +
			    v.y * transformation._23 +
			    v.z * transformation._33;

	result.x = x;
    result.y = y;
    result.z = z;
}

// Converts the given Cartesian coordinates into Spherical coordinates
// result.x: azimuth, range 0 to 2Pi, initial direction is negative Z (east), increasing towards full revolution around positive Y
// result.y: elevation, range -Pi/2 to Pi/2, initial direction is negative Y (down), increasing towards positive Y (up)
// result.z: distance
void AxVector3::CartesianToSpherical(AxVector3 &result, float x, float y, float z)
{
    result.z = AxMath::Sqrt(x * x + y * y + z * z);
    if (result.z == 0.0f)
        return;

    result.y = AxMath::ArcSin(y / result.z);

    if (x == 0)
        if (z > 0)
			result.x = AxMath::Pi / 2.0f;
        else
			result.x = -AxMath::Pi / 2.0f;
    else
        result.x = AxMath::ArcTan(z / x);

    if (x < 0)
		result.x += AxMath::Pi;

    if (result.x < 0)
		result.x += AxMath::Pi * 2;
}

void AxVector3::CartesianToSpherical(AxVector3 &result, AxVector3 &cartesian)
{
    AxVector3::CartesianToSpherical(result, cartesian.x, cartesian.y, cartesian.z);
}

void AxVector3::SphericalToCartesian(AxVector3 &result, float azimuth, float elevation, float radius)
{
    float eCos = AxMath::Cos(elevation) * radius;
    result.x = AxMath::Cos(azimuth) * eCos;
    result.y = AxMath::Sin(elevation) * radius;
    result.z = AxMath::Sin(azimuth) * eCos;
}

void AxVector3::SphericalToCartesian(AxVector3 &result, AxVector3 &spherical)
{
    AxVector3::SphericalToCartesian(result, spherical.x, spherical.y, spherical.z);
}
    
void AxVector3::LerpSherical(AxVector3 &result, AxVector3 &v1, AxVector3 &v2, float factor)
{
    float delta = v2.x - v1.x;
	if (AxMath::Abs(delta) < AxMath::Pi)
        result.x = v1.x + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath::Pi2;
            result.x = v1.x + delta * factor;
            if (result.x < 0)
                result.x += AxMath::Pi2;
        }
        else
        {
            delta += AxMath::Pi * 2;
            result.x = v1.x + delta * factor;
            if (result.x > AxMath::Pi2)
                result.x -= AxMath::Pi2;
        }
    }

    delta = v2.y - v1.y;
	if (AxMath::Abs(delta) < AxMath::Pi)
        result.y = v1.y + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath::Pi2;
            result.y = v1.y + delta * factor;
            if (result.y < 0)
				result.y += AxMath::Pi2;
        }
        else
        {
			delta += AxMath::Pi2;
            result.y = v1.y + delta * factor;
            if (result.y > AxMath::Pi2)
                result.y -= AxMath::Pi2;
        }
    }

    result.z = v1.z + (v2.z - v1.z) * factor;
} 