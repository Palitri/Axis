//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMaths.h"

#include "..\..\Tools\AxMath.h"

#include "..\Primitive\AxVector2.h"
#include "..\Primitive\AxVector3.h"
#include "..\Primitive\AxVector4.h"
#include "..\Primitive\AxMatrix.h"
#include "..\Primitive\AxBoundingVolume.h"

AxMaths::AxMaths(void)
{
}


AxMaths::~AxMaths(void)
{
}

   
void AxMaths::PlaneGeneralFromPointNormal(AxVector4 &plane, AxVector3 &planePoint, AxVector3 &planeNormal)
{
	plane.x = planeNormal.x;
	plane.y = planeNormal.y;
	plane.z = planeNormal.z;
	plane.w = -(planeNormal.x * planePoint.x + planeNormal.y * planePoint.y + planeNormal.z * planePoint.z);
}

void AxMaths::PlanePointNormalFromGeneral(AxVector3 &planePoint, AxVector3 &planeNormal, AxVector4 &plane)
{
	planeNormal.x = plane.x;
	planeNormal.y = plane.y;
	planeNormal.z = plane.z;
	
	AxVector3::Normalize(planeNormal, planeNormal);

	planePoint.x = 0.0f;
	planePoint.y = 0.0f;
	planePoint.z = 0.0f;

	float absNormalX = AxMath::Abs(plane.x);
	float absNormalY = AxMath::Abs(plane.y);
	float absNormalZ = AxMath::Abs(plane.z);

	if (absNormalX > absNormalY)
	{
		if (absNormalX > absNormalZ)
			planePoint.x = -plane.w / plane.x;
		else
			planePoint.z = -plane.w / plane.z;
	}
	else
	{
		if (absNormalY > absNormalZ)
			planePoint.y = -plane.w / plane.y;
		else
			planePoint.z = -plane.w / plane.z;
	}
}

void AxMaths::PlaneFromPoints(AxVector4 &plane, AxVector3 &p0, AxVector3 &p1, AxVector3 &p2)
{
	AxVector3 a, b, normal;
	AxVector3::Subtract(a, p1, p0);
	AxVector3::Subtract(b, p2, p0);
	AxVector3::Cross(normal, a, b);
	plane.x = normal.x;
	plane.y = normal.y;
	plane.z = normal.z;
	plane.w = -AxVector3::Dot(normal, p0);
}

float AxMaths::PointToRayProjection(AxVector3 &projection, AxVector3 &point, AxVector3 &rayPoint, AxVector3 &rayVector)
{
	AxVector3::Subtract(projection, point, rayPoint);
	float projectionDistance = AxVector3::Dot(rayVector, projection);
	AxVector3::Scale(projection, rayVector, projectionDistance);
	AxVector3::Add(projection, rayPoint, projection);
	return projectionDistance;
}

//planeNormal must be normalized
float AxMaths::PointToPlaneDistance(AxVector3 &point, AxVector3 &planePoint, AxVector3 &planeNormal)
{
    return -AxMaths::RayToOrthogonalPlaneDistance(point, planeNormal, planePoint);
}

float AxMaths::PointToPlaneDistance(AxVector3 &point, AxVector4 &plane)
{
	// TODO: sophisticate the straightforward division by plane.z, make conversion functions for plane of Vector3s and of Vector4
	AxVector3 planePoint(0.0f, 0.0f, -plane.w / plane.z);
    AxVector3 planeNormal(plane.x, plane.y, plane.z);

	AxVector3::Normalize(planeNormal, planeNormal);

    return -AxMaths::RayToOrthogonalPlaneDistance(point, planeNormal, planePoint);
}

//planeNormal must be normalized
float AxMaths::PointToPlaneProjection(AxVector3 &projection, AxVector3 &point, AxVector3 &planePoint, AxVector3 &planeNormal)
{
    float distance = AxMaths::PointToPlaneDistance(point, planePoint, planeNormal);
	AxVector3::Scale(projection, planeNormal, -distance);
	AxVector3::Add(projection, point, projection);
	return distance;
}

//rayVector must be normalized
float AxMaths::RayToOrthogonalPlaneDistance(AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &planePoint)
{
    AxVector3 rayDir;
	AxVector3::Subtract(rayDir, planePoint, rayPoint);
    return AxVector3::Dot(rayVector, rayDir);
}
    
//planeNormal must be normalized
float AxMaths::RayToOrthogonalPlaneIntersection(AxVector3 &intersection, AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &planePoint)
{
	float distance = AxMaths::RayToOrthogonalPlaneDistance(rayPoint, rayVector, planePoint);
	AxVector3::Scale(intersection, rayVector, distance);
	AxVector3::Add(intersection, rayPoint, intersection);
    return distance;
}

float AxMaths::RayToRayDistance(AxVector3 &resultPoint, AxVector3 &resultRay1Point, AxVector3 &resultRay2Point, AxVector3 &ray1Point, AxVector3 &ray1Direction, AxVector3 &ray2Point, AxVector3 &ray2Direction)
{
    AxVector3 a = AxVector3(ray1Point);
    AxVector3 b = AxVector3(ray2Point);
    AxVector3 c;
    AxVector3::Subtract(c, ray2Point, ray1Point);

    float p = AxVector3::Dot(a, b);
    float q = AxVector3::Dot(a, c);
    float r = AxVector3::Dot(b, c);
    float s = AxVector3::Dot(a, a);
    float t = AxVector3::Dot(b, b);

    float p2 = p * p;
    float ptsp2 = p / (t * s - p2);
    float vd = - r * ptsp2 + q / (s - p2 / t);	// = - r * p / (t * s - p * p) + q / (s - p * p / t);
    float ve = q * ptsp2 - r / (t - p2 / s);	// = q * p / (t * s - p * p) - r / (t - p * p / s);

    AxVector3::Scale(a, a, vd);
    AxVector3::Scale(b, b, ve);

    AxVector3::Add(resultRay1Point, ray1Point, a);
    AxVector3::Add(resultRay2Point, ray2Point, b);

    AxVector3::Lerp(resultPoint, resultRay1Point, resultRay2Point, 0.5);
    
    return AxVector3::Distance(resultRay1Point, resultRay2Point);
}

//rayVector must be normalized
bool AxMaths::RayToSphereIntersection(AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &sphereCenter, float sphereRadius)
{
    AxVector3 b;
	AxMaths::RayToOrthogonalPlaneIntersection(b, rayPoint, rayVector, sphereCenter);
	float ob = AxVector3::Distance(sphereCenter, b);
    return ob <= sphereRadius;
}

//rayVector must be normalized
bool AxMaths::RayToSphereIntersection(AxVector2 &resultDistances, AxVector3 &resultFront, AxVector3 &resultBack, AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &sphereCenter, float sphereRadius)
{
	AxVector3 b;
	float ab = AxMaths::RayToOrthogonalPlaneIntersection(b, rayPoint, rayVector, sphereCenter);
	float ob = AxVector3::Distance(sphereCenter, b);
	if (ob > sphereRadius)
		return false;
	
	float s = AxMath::Sqrt(sphereRadius * sphereRadius - ob * ob);

	resultDistances.x = ab - s;
	resultDistances.y = ab + s;

	AxVector3::Scale(resultFront, rayVector, resultDistances.x);
	AxVector3::Scale(resultBack, rayVector, resultDistances.y);

	return true;
}


// rayVector and planeNormal must be normalized
float AxMaths::RayToPlaneIntersection(AxVector3 &result, AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &planePoint, AxVector3 &planeNormal)
{
	AxVector3 ra;
    AxVector3::Subtract(ra, planePoint, rayPoint);
	
	float d = AxVector3::Dot(planeNormal, ra) / AxVector3::Dot(planeNormal, rayVector);

	AxVector3::Scale(result, rayVector, d);
	AxVector3::Add(result, result, rayPoint);

	return d;
}

// rayVector must be normalized
bool AxMaths::RayToTriangleIntersection(AxVector3 &rayPoint, AxVector3 &rayVector,
	AxVector3 &v0, AxVector3 &v1, AxVector3 &v2,
	float &d, float &u, float &v)
{
	AxVector3 edge1, edge2,
			perp, tvec, qvec;

	AxVector3::Subtract(edge1, v1, v0);
    AxVector3::Subtract(edge2, v2, v0);

    AxVector3::Cross(perp, rayVector, edge2);
        
    float det = AxVector3::Dot(edge1, perp);
    if (det <= 0.0f)
        return false;

    AxVector3::Subtract(tvec, rayPoint, v0);
    u = AxVector3::Dot(tvec, perp);
    if ((u < 0.0f) || (u > det))
        return false;

    AxVector3::Cross(qvec, tvec, edge1);
    v = AxVector3::Dot(rayVector, qvec);
    if ((v < 0.0f) || (u + v > det))
        return false;

    d = AxVector3::Dot(edge2, qvec);
    float fInvDet = 1.0f / det;
    d *= fInvDet;
    u *= fInvDet;
    v *= fInvDet;

    return true;
}

//coneVector must be normalized
bool AxMaths::SphereConeIntersection(AxVector3 &coneApex, AxVector3 &coneVector, float coneAngle, AxVector3 &sphereCenter, float sphereRadius)
{
	coneAngle /= 2.0f;
    float d = RayToOrthogonalPlaneDistance(coneApex, coneVector, sphereCenter);
    float maxSphereToAxisDistance = (sphereRadius + d * AxMath::Sin(coneAngle)) / AxMath::ArcCos(coneAngle);
        
    AxVector3 intersectionVector, intersectionPoint;
    AxVector3::Scale(intersectionVector, coneVector, d);
    AxVector3::Add(intersectionPoint, coneApex, intersectionVector);
        
    float sphereToAxisDistance = AxVector3::Distance(sphereCenter, intersectionPoint);
    return sphereToAxisDistance <= maxSphereToAxisDistance;
}

bool AxMaths::SphereViewFrustumIntersection(AxMatrix &viewFrustum, AxVector3 &sphereCenter, float sphereRadius)
{
    AxVector4 plane;
	
	//Left Plane
    plane.x = viewFrustum._14 + viewFrustum._11;
    plane.y = viewFrustum._24 + viewFrustum._21;
    plane.z = viewFrustum._34 + viewFrustum._31;
    plane.w = viewFrustum._44 + viewFrustum._41;
    if (-AxMaths::PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Right Plane
    plane.x = viewFrustum._14 - viewFrustum._11;
    plane.y = viewFrustum._24 - viewFrustum._21;
    plane.z = viewFrustum._34 - viewFrustum._31;
    plane.w = viewFrustum._44 - viewFrustum._41;
    if (-AxMaths::PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Bottom Plane
    plane.x = viewFrustum._14 + viewFrustum._12;
    plane.y = viewFrustum._24 + viewFrustum._22;
    plane.z = viewFrustum._34 + viewFrustum._32;
    plane.w = viewFrustum._44 + viewFrustum._42;
    if (-AxMaths::PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Top Plane
    plane.x = viewFrustum._14 - viewFrustum._12;
    plane.y = viewFrustum._24 - viewFrustum._22;
    plane.z = viewFrustum._34 - viewFrustum._32;
    plane.w = viewFrustum._44 - viewFrustum._42;
    if (-AxMaths::PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Near Plane
    plane.x = viewFrustum._13;
    plane.y = viewFrustum._23;
    plane.z = viewFrustum._33;
    plane.w = viewFrustum._43;
    if (-AxMaths::PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Far Plane
    plane.x = viewFrustum._14 - viewFrustum._13;
    plane.y = viewFrustum._24 - viewFrustum._23;
    plane.z = viewFrustum._34 - viewFrustum._33;
    plane.w = viewFrustum._44 - viewFrustum._43;
    if (-AxMaths::PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;
        
    return true;
}

void AxMaths::GetBarycentricCoordinates(AxVector2 &result, AxVector3 &cartesianCoords, AxVector3 &uAxis, AxVector3 &vAxis)
{
    float p = AxVector3::Dot(uAxis, vAxis);
    float q = AxVector3::Dot(uAxis, cartesianCoords);
    float r = AxVector3::Dot(vAxis, cartesianCoords);
    float s = AxVector3::Dot(uAxis, uAxis);
    float t = AxVector3::Dot(vAxis, vAxis);

	float invD = 1.0f / (s * t - p * p);

	result.x = (q * t - p * r) * invD;
	result.y = (r * s - p * q) * invD;
}

void AxMaths::GetNormal(AxVector3 &result, AxVector3 &v1, AxVector3 &v2, AxVector3 &v3)
{
    AxVector3 dv1, dv2;

	AxVector3::Subtract(dv1, v2, v1);
    AxVector3::Subtract(dv2, v3, v1);

    AxVector3::Cross(result, dv2, dv1);
    AxVector3::Normalize(result, result);
}

void AxMaths::GetTangents(AxVector3 &tangent, AxVector3 &biTangent, AxVector3 &v1, AxVector3 &v2, AxVector3 &v3, AxVector2 &t1, AxVector2 &t2, AxVector2 &t3)
{
    AxVector3 dv1, dv2;
	AxVector2 dt1, dt2;

	AxVector3::Subtract(dv1, v2, v1);
    AxVector3::Subtract(dv2, v3, v1);

	AxVector2::Subtract(dt1, t2, t1);
    AxVector2::Subtract(dt2, t3, t1);

	AxVector3 a, b;

	AxVector3::Scale(a, dv2, dt1.y);
	AxVector3::Scale(b, dv1, dt2.y);
	AxVector3::Subtract(tangent, a, b);
	AxVector3::Scale(tangent, tangent, 1.0f / (dt1.y * dt2.x - dt2.y * dt1.x));
	AxVector3::Normalize(tangent, tangent);

	AxVector3::Scale(a, dv2, dt1.x);
	AxVector3::Scale(b, dv1, dt2.x);
	AxVector3::Subtract(biTangent, a, b);
	AxVector3::Scale(biTangent, biTangent, 1.0f / (dt1.x * dt2.y - dt2.x * dt1.y));
	AxVector3::Normalize(biTangent, biTangent);
}

float AxMaths::VectorAngle(AxVector3 &v1, AxVector3 &v2)
{
	float l = v1.GetLength() * v2.GetLength();
	return l == 0.0f ? 0.0f : AxMath::ArcCos(AxVector3::Dot(v1, v2) / l);
}

int AxMaths::GetPowerOf2(int value)
{
    int result = 1;
    while (result < value)
        result = result << 1;

    return result;
}

int AxMaths::GetDivisibleBy(int value, int divisor)
{
	int mod = value % divisor;
	if (mod == 0)
		return value;
	else
		return value + divisor - mod;
}

/*
int AxMaths::GetDivisibleBy(int value, int divisor)
{
	int sign = AxMath::Sign(value);
	return ((value - sign) / divisor + sign) * divisor;
}
*/

float AxMaths::DegreeToRad(float degree)
{
    return AxMath::Pi * degree / 180.0f;
}

float AxMaths::RadToDegree(float rad)
{
    return 180.0f * rad / AxMath::Pi;
}

float AxMaths::AngleFromDistanceAndSize(float distance, float size)
{
    return 2.0f * AxMath::ArcTan(size / (2.0f * distance));
}

float AxMaths::DistanceFromSizeAndAngle(float size, float angle)
{
	return size / (2.0f * AxMath::Tan(angle / 2.0f));
}

float AxMaths::SizeFromDistanceAndAngle(float distance, float angle)
{
	return 2.0f * distance * AxMath::Tan(angle / 2.0f);
}

float AxMaths::LensToFOV(float lens, float filmSize)
{
    return AxMaths::AngleFromDistanceAndSize(lens, filmSize);
}

float AxMaths::FOVToLens(float fov, float filmSize)
{
	return AxMaths::DistanceFromSizeAndAngle(fov, filmSize);
}

float AxMaths::LerpAngle(float a1, float a2, float factor)
{
    float result;
	float delta = a2 - a1;
	if (AxMath::Abs(delta) < AxMath::Pi)
        result = a1 + delta * factor;
    else
    {
        if (delta > 0)
        {
            delta -= AxMath::Pi2;
            result = a1 + delta * factor;
            if (result < 0)
				result += AxMath::Pi2;
        }
        else
        {
			delta += AxMath::Pi2;
            result = a1 + delta * factor;
            if (result > AxMath::Pi2)
                result -= AxMath::Pi2;
        }
    }

	return result;
}

void AxMaths::GetIndexBlending(int fromIndex, int toIndex, float factor, int &index1, int &index2, float &blend)
{
	int max = toIndex - fromIndex;
	float realIndex = fromIndex + factor * max;
	index1 = AxMath::Min((int)AxMath::Floor(realIndex), toIndex);
	index2 = AxMath::Min(index1 + 1, toIndex);
	blend = realIndex - index1;
};


void AxMaths::VolumetricToScreenSpace(AxVector3 &result, AxVector3 &volumetricCoords, AxMatrix &transform)
{
    AxVector4 screenSpace;
    AxVector4::Transform(screenSpace, volumetricCoords, transform);

    result.x = screenSpace.x / screenSpace.w;
	result.y = screenSpace.y / screenSpace.w;
	result.z = screenSpace.z / screenSpace.w;
};

void AxMaths::VolumetricToScreenSpace(AxVector2 &result, AxVector3 &volumetricCoords, AxMatrix &transform)
{
	AxVector3 result3;
	AxMaths::VolumetricToScreenSpace(result3, volumetricCoords, transform);
	result.x = result3.x;
	result.y = result3.y;
}

void AxMaths::ScreenSpaceToVolumetricRay(AxVector3 &resultRayOrigin, AxVector3 &resultRayOrientation, AxVector3 &screenCoords, AxMatrix &transform)
{
    AxMatrix inverseTransform;
    AxMatrix::Invert(inverseTransform, transform);

    AxVector4 origin4;
    AxVector4::Transform(origin4, screenCoords, inverseTransform);
    resultRayOrigin.x = origin4.x / origin4.w; 
    resultRayOrigin.y = origin4.y / origin4.w; 
    resultRayOrigin.z = origin4.z / origin4.w;

    AxVector4 direction4;
    AxVector4::Transform(direction4, AxVector3(screenCoords.x, screenCoords.y, 1.0), inverseTransform);
    resultRayOrientation.x = direction4.x / direction4.w;
    resultRayOrientation.y = direction4.y / direction4.w;
    resultRayOrientation.z = direction4.z / direction4.w;

    AxVector3::Subtract(resultRayOrientation, resultRayOrientation, resultRayOrigin);
    AxVector3::Normalize(resultRayOrientation, resultRayOrientation);
};

void AxMaths::ScreenSpaceToVolumetricRay(AxVector3 &resultRayOrigin, AxVector3 &resultRayOrientation, AxVector2 &screenCoords, AxMatrix &transform)
{
	AxVector3 screenCoords3(screenCoords, 0.0f);
	AxMaths::ScreenSpaceToVolumetricRay(resultRayOrigin, resultRayOrientation, screenCoords3, transform);
}

void AxMaths::ScreenToPixelSpace(AxVector2 &result, AxVector2 &screenCoords, float screenWidth, float screenHeight)
{
    result.x = screenWidth * (screenCoords.x + 1.0f) / 2.0f;
	result.y = screenHeight * (1.0f - screenCoords.y) / 2.0f;
};

void AxMaths::PixelToScreenSpace(AxVector2 &result, AxVector2 pixelCoords, float screenWidth, float screenHeight)
{
    result.x = 2.0f * pixelCoords.x / screenWidth - 1.0f;
	result.y = 1.0f - 2.0f * pixelCoords.y / screenHeight;
};

