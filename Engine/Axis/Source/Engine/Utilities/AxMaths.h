#pragma once

#include "..\..\AxGlobals.h"

class AxVector2;
class AxVector3;
class AxVector4;
class AxMatrix;

class AXDLLCLASS AxMaths
{
public:
	AxMaths(void);
	~AxMaths(void);

	static void PlaneGeneralFromPointNormal(AxVector4 &plane, AxVector3 &planePoint, AxVector3 &planeNormal);
	static void PlanePointNormalFromGeneral(AxVector3 &planePoint, AxVector3 &planeNormal, AxVector4 &plane);
	static void PlaneFromPoints(AxVector4 &plane, AxVector3 &p0, AxVector3 &p1, AxVector3 &p2);
	static float PointToRayProjection(AxVector3 &projection, AxVector3 &point, AxVector3 &rayPoint, AxVector3 &rayVector);
	static float PointToPlaneDistance(AxVector3 &point, AxVector3 &planePoint, AxVector3 &planeNormal);
	static float PointToPlaneDistance(AxVector3 &point, AxVector4 &plane);
	static float PointToPlaneProjection(AxVector3 &projection, AxVector3 &point, AxVector3 &planePoint, AxVector3 &planeNormal);
	static float RayToOrthogonalPlaneDistance(AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &planePoint);
	static float RayToOrthogonalPlaneIntersection(AxVector3 &intersection, AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &planePoint);
	static float RayToRayDistance(AxVector3 &resultPoint, AxVector3 &resultRay1Point, AxVector3 &resultRay2Point, AxVector3 &ray1Point, AxVector3 &ray1Direction, AxVector3 &ray2Point, AxVector3 &ray2Direction);
	static bool RayToSphereIntersection(AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &sphereCenter, float sphereRadius);
	static bool RayToSphereIntersection(AxVector2 &resultDistances, AxVector3 &resultFront, AxVector3 &resultBack, AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &sphereCenter, float sphereRadius);
	static float RayToPlaneIntersection(AxVector3 &result, AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &planePoint, AxVector3 &planeNormal);
	static bool RayToTriangleIntersection(AxVector3 &rayPoint, AxVector3 &rayVector, AxVector3 &v0, AxVector3 &v1, AxVector3 &v2, float &d, float &u, float &v);
	static bool SphereConeIntersection(AxVector3 &conePoint, AxVector3 &coneVector, float coneAngle, AxVector3 &sphereCenter, float sphereRadius);
	static bool SphereViewFrustumIntersection(AxMatrix &viewFrustum, AxVector3 &sphereCenter, float sphereRadius);
	static void GetBarycentricCoordinates(AxVector2 &result, AxVector3 &cartesianCoords, AxVector3 &uAxis, AxVector3 &vAxis);
	static void GetNormal(AxVector3 &result, AxVector3 &v1, AxVector3 &v2, AxVector3 &v3);
	static void GetTangents(AxVector3 &tangent, AxVector3 &biTangent, AxVector3 &v1, AxVector3 &v2, AxVector3 &v3, AxVector2 &t1, AxVector2 &t2, AxVector2 &t3);
	static float VectorAngle(AxVector3 &v1, AxVector3 &v2);
	static int GetPowerOf2(int value);
	static int GetDivisibleBy(int value, int factor);
	static float DegreeToRad(float degree);
	static float RadToDegree(float rad);
	static float AngleFromDistanceAndSize(float distance, float size);
	static float DistanceFromSizeAndAngle(float size, float angle);
	static float SizeFromDistanceAndAngle(float distance, float angle);
	static float LensToFOV(float lens, float filmSize = 36.0f);
	static float FOVToLens(float fov, float filmSize = 36.0f);
	static float LerpAngle(float a1, float a2, float factor);
	static void AxMaths::GetIndexBlending(int fromIndex, int toIndex, float factor, int &index1, int &index2, float &blend);
};

