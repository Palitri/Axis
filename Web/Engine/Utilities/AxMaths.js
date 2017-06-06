function AxMaths()
{
}

/**
 * Converts a plane in point-normal form to general form
 * @param {AxVector4} plane The resulting plane in general form, ax + by + cx + d, where a, b, c and d correspond to the vector's x, y, z, and w components
 * @param {AxVector3} planePoint A point which lies on the plane
 * @param {AxVector3} planeNormal The normal vector for the plane
 */
AxMaths.PlaneGeneralFromPointNormal = function(plane, planePoint, planeNormal)
{
    plane.x = planeNormal.x;
    plane.y = planeNormal.y;
    plane.z = planeNormal.z;
    plane.w = -(planeNormal.x * planePoint.x + planeNormal.y * planePoint.y + planeNormal.z * planePoint.z);
};

/**
 * Converts a plane in general form to point-normal form
 * @param {AxVector3} planePoint The resulting point on the plane. The point is taken such, that it is the closest to the coordinate system's origin, which lies on one of the axes
 * @param {AxVector3} planeNormal The resulting normal vector for the plane, normalized.
 * @param {AxVector4} plane The plane in general form, ax + by + cx + d, where a, b, c and d correspond to the vector's x, y, z, and w components
 */
AxMaths.PlanePointNormalFromGeneral = function(planePoint, planeNormal, plane)
{
    planeNormal.x = plane.x;
    planeNormal.y = plane.y;
    planeNormal.z = plane.z;

    AxVector3.Normalize(planeNormal, planeNormal);

    planePoint.x = 0.0;
    planePoint.y = 0.0;
    planePoint.z = 0.0;

    var absNormalX = AxMath.Abs(plane.x);
    var absNormalY = AxMath.Abs(plane.y);
    var absNormalZ = AxMath.Abs(plane.z);

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
};

/**
 * Produces a plane in general form from three points, lying on it
 * @param {AxVector4} plane The resulting plane in general form, ax + by + cx + d, where a, b, c and d correspond to the vector's x, y, z, and w components
 * @param {AxVector3} p0 The first point on the surface of the plane
 * @param {AxVector3} p1 The second point on the surface of the plane
 * @param {AxVector3} p2 The third point on the surface of the plane
 */
AxMaths.PlaneFromPoints = function(plane, p0, p1, p2)
{
    var a = new AxVector3();
    var b = new AxVector3();
    var normal = new AxVector3();
    AxVector3.Subtract(a, p1, p0);
    AxVector3.Subtract(b, p2, p0);
    AxVector3.Cross(normal, a, b);
    plane.x = normal.x;
    plane.y = normal.y;
    plane.z = normal.z;
    plane.w = -AxVector3.Dot(normal, p0);
};

/**
 * Projects a point orthogonally onto a ray and produces the projected point
 * @param {AxVector3} projection The resulting projected point on the ray
 * @param {AxVector3} point The point, which is to be projected
 * @param {AxVector3} rayPoint The initial (starting) point of the ray
 * @param {AxVector3} rayVector The orientation of the ray
 * @returns {Number} The distance between the initial point of the ray and the projected point
 */
AxMaths.PointToRayProjection = function(projection, point, rayPoint, rayVector)
{
    AxVector3.Subtract(projection, point, rayPoint);
    var projectionDistance = AxVector3.Dot(rayVector, projection);
    AxVector3.Scale(projection, rayVector, projectionDistance);
    AxVector3.Add(projection, rayPoint, projection);
    return projectionDistance;
};

/**
 * Calculates the distance between a point and a plane
 * The plane can be defined either in general form by a 4-vector, or in point and normal form by a 3-vector normal and a 3-vector point on the plane
 * Method has the following forms:
 * 1. PointToPlaneDistance(AxVector3 point, AxVector3 planePoint, AxVector3 planeNormal)
 * 2. PointToPlaneDistance(AxVector3 point, AxVector4 plane)
 * planeNormal must be normalized, if the plane is to be defined in a point and normal form
 * @param {AxVector3} point The point in space whose distance to calculate against the plane
 * @param {AxVector3|AxVector4} plane Either the plane in general form, given by a 4-vector or a point on the plane, defined by a 3-vector
 * @param {!AxVector3} planeNormal For defining the plane in a point and normal form, the normal vector of the plane. Must be normalized
 * @returns {Number} The distance between the point and  the plane. If the point is behind the plane, the result will be negative
 */
AxMaths.PointToPlaneDistance = function(point, plane, planeNormal)
{
    if (AxUtils.IsUndefined(planeNormal) && !AxUtils.IsUndefined(plane.w))
    {
        var planePoint = new AxVector3();
        var planeNormal = new AxVector3();
        AxMaths.PlanePointNormalFromGeneral(planePoint, planeNormal, plane);

        return -AxMaths.RayToOrthogonalPlaneDistance(point, planeNormal, planePoint);
    }
    else
        return -AxMaths.RayToOrthogonalPlaneDistance(point, planeNormal, plane);
};

/**
 * Calculates the orthogonal projection point of a point onto a plane. That is, the point is projected along the normal of the plane
 * planeNormal must be normalized
 * @param {AxVector3} projection The resulting projected point, which lies on the plane
 * @param {AxVector3} point The point in space which is to be projected onto the plane
 * @param {AxVector3} planePoint A point which lies on the plane, used for defining the plane
 * @param {AxVector3} planeNormal The normal vector of the plane, used for defining the plane. Must be normalized
 * @returns {Number} The distance between the point and its projection on the plane. If the point is behind the plane, the result will be negative
 */
AxMaths.PointToPlaneProjection = function(projection, point, planePoint, planeNormal)
{
    var distance = AxMaths.PointToPlaneDistance(point, planePoint, planeNormal);
    AxVector3.Scale(projection, planeNormal, -distance);
    AxVector3.Add(projection, point, projection);
    return distance;
};

/**
 * Calculates the distance between a ray, which starts at a given point and a plane which is orthogonal to the ray
 * The plane is assumed orthogonal to the ray and its orientation derives from the ray's, so that it faces the ray perpendicularly
 * rayVector must be normalized
 * @param {AxVector3} rayPoint The location of the initial (starting) point of the ray
 * @param {AxVector3} rayVector The orientation vector of the ray. Must be normalized.
 * @param {AxVector3} planePoint A point which lies on the plane
 * @returns {Number} The distance between the ray initial point and the intersection point
 */
AxMaths.RayToOrthogonalPlaneDistance = function(rayPoint, rayVector, planePoint)
{
    var rayDir = new AxVector3();
    AxVector3.Subtract(rayDir, planePoint, rayPoint);
    return AxVector3.Dot(rayVector, rayDir);
};
    
/**
 * Returns the intersection point between a ray and a plane orthogonal to the plane
 * The plane is assumed orthogonal to the ray, so its orientation is derived from the ray
 * rayVector must be normalized
 * @param {AxVector3} intersection The resulting intersection point
 * @param {AxVector3} rayPoint The location of the initial (starting) point of the ray
 * @param {AxVector3} rayVector The orientation vector of the ray. Must be normalized.
 * @param {AxVector3} planePoint A point which lies on the plane
 * @returns {Number} The distance between the ray initial point and the intersection point
 */
AxMaths.RayToOrthogonalPlaneIntersection = function(intersection, rayPoint, rayVector, planePoint)
{
    var distance = AxMaths.RayToOrthogonalPlaneDistance(rayPoint, rayVector, planePoint);
    AxVector3.Scale(intersection, rayVector, distance);
    AxVector3.Add(intersection, rayPoint, intersection);
    return distance;
};

/**
 * Calculates the smallest distance between two rays and returns the point, which is mutually closest to both rays and the point on each ray, which is closest to the other
 * ray1Direction and ray1Direction must be normalized
 * @param {AxVector3} resultPoint The resulting point, which is mutually closest to both rays
 * @param {AxVector3} resultRay1Point The resulting point, which lies on the first ray and is closest to the second ray
 * @param {AxVector3} resultRay2Point The resulting point, which lies on the second ray and is closest to the first ray
 * @param {AxVector3} ray1Point The initial (starting) point of the first ray
 * @param {AxVector3} ray1Direction The orientation of the first ray
 * @param {AxVector3} ray2Point The initial (starting) point of the first ray
 * @param {AxVector3} ray2Direction The orientation of the second ray
 * @returns {Number} The smallest distance between the rays
 */
AxMaths.RayToRayDistance = function(resultPoint, resultRay1Point, resultRay2Point, ray1Point, ray1Direction, ray2Point, ray2Direction)
{
    var a = new AxVector3(ray1Point);
    var b = new AxVector3(ray2Point);
    var c = new AxVector3();
    AxVector3.Subtract(c, ray2Point, ray1Point);

    var p = AxVector3.Dot(a, b);
    var q = AxVector3.Dot(a, c);
    var r = AxVector3.Dot(b, c);
    var s = AxVector3.Dot(a, a);
    var t = AxVector3.Dot(b, b);

    var p2 = p * p;
    var ptsp2 = p / (t * s - p2);
    var vd = - r * ptsp2 + q / (s - p2 / t);	// = - r * p / (t * s - p * p) + q / (s - p * p / t);
    var ve = q * ptsp2 - r / (t - p2 / s);	// = q * p / (t * s - p * p) - r / (t - p * p / s);

    AxVector3.Scale(a, a, vd);
    AxVector3.Scale(b, b, ve);

    AxVector3.Add(resultRay1Point, ray1Point, a);
    AxVector3.Add(resultRay2Point, ray2Point, b);

    AxVector3.Lerp(resultPoint, resultRay1Point, resultRay2Point, 0.5);
    
    return AxVector3.Distance(resultRay1Point, resultRay2Point);
};

/**
 * Checks whether a ray intersects a sphere
 * rayVector must be normalized
 * @param {AxVector3} rayPoint The location of the initial (starting) point of the ray
 * @param {AxVector3} rayVector The orientation vector of the ray. Must be normalized.
 * @param {AxVector3} sphereCenter The location of the center point of the sphere
 * @param {Number} sphereRadius The radius of the sphere
 * @returns {Boolean} True if the ray intersects the sphere
 */
AxMaths.RayToSphereIntersection = function(rayPoint, rayVector, sphereCenter, sphereRadius)
{
    var b = new AxVector3();
    AxMaths.RayToOrthogonalPlaneIntersection(b, rayPoint, rayVector, sphereCenter);
    var ob = AxVector3.Distance(sphereCenter, b);
    return ob <= sphereRadius;
};

/**
 * Calculates the intersection point between a ray and a plane and returns the distance between the ray initial point to the intersection point
 * rayVector and planeNormal must be normalized
 * @param {AxVector3} result The resulting intersection point
 * @param {AxVector3} rayPoint The initial (starting) point of the ray
 * @param {AxVector3} rayVector The orientation vector of the ray. Must be normalized
 * @param {AxVector3} planePoint A point which lies on the surface of the plane
 * @param {AxVector3} planeNormal The normal of the plane. Must be normalized
 */
AxMaths.RayToPlaneIntersection = function(result, rayPoint, rayVector, planePoint, planeNormal)
{
    var ra = new AxVector3();
    AxVector3.Subtract(ra, planePoint, rayPoint);
	
    var d = AxVector3.Dot(planeNormal, ra) / AxVector3.Dot(planeNormal, rayVector);

    AxVector3.Scale(result, rayVector, d);
    AxVector3.Add(result, result, rayPoint);

    return d;
};

/**
 * Checks whether a ray intersects a triangle and calculates the intersection point
 * rayVector must be normalized
 * @param {AxVector3} rayPoint The initial point of the ray
 * @param {AxVector3} rayVector The orientation vector of the plane. Must be normalized
 * @param {AxVector3} v0 The first vertex point of the triangle
 * @param {AxVector3} v1 The second vertex point of the triangle
 * @param {AxVector3} v2 The third vertex point of the triangle
 * @param {AxVector3} result A vector which holds the barycentric coordinates of the intersection in its x and y components and the distance between the ray initial point and the intersection point in its z component
 * @returns {Boolean} True if the ray intersects with the triangle
 */
AxMaths.RayToTriangleIntersection = function(rayPoint, rayVector, v0, v1, v2, result)
{
    var edge1 = new AxVector3();
    var edge2 = new AxVector3();
    var perp = new AxVector3();
    var tvec = new AxVector3();
    var qvec = new AxVector3();

    AxVector3.Subtract(edge1, v1, v0);
    AxVector3.Subtract(edge2, v2, v0);

    AxVector3.Cross(perp, rayVector, edge2);
        
    var det = AxVector3.Dot(edge1, perp);
    if (det <= 0.0)
        return false;

    AxVector3.Subtract(tvec, rayPoint, v0);
    result.x = AxVector3.Dot(tvec, perp);
    if ((result.x < 0.0) || (result.x > det))
        return false;

    AxVector3.Cross(qvec, tvec, edge1);
    result.y = AxVector3.Dot(rayVector, qvec);
    if ((result.y < 0.0) || (result.x + result.y > det))
        return false;

    result.z = AxVector3.Dot(edge2, qvec);
    var fInvDet = 1.0 / det;
    result.z *= fInvDet;
    result.x *= fInvDet;
    result.y *= fInvDet;

    return true;
};

/**
 * Checks whether a sphere and a cone intersect
 * coneVector must be normalized
 * @param {AxVector3} coneApex The location of the apex point of the cone
 * @param {AxVector3} coneVector The orientation vector of the cone, such that it points from the apex point toward the center of the base of the cone. Must be normalized
 * @param {Number} coneAngle The angle at the apex of the cone
 * @param {AxVector3} sphereCenter The location of the center of the sphere
 * @param {Number} sphereRadius The radius of the sphere
 * @returns {Boolean} True if the sphera and the cone intersect
 */
AxMaths.SphereConeIntersection = function(coneApex, coneVector, coneAngle, sphereCenter, sphereRadius)
{
    coneAngle /= 2.0;
    var d = RayToOrthogonalPlaneDistance(coneApex, coneVector, sphereCenter);
    var maxSphereToAxisDistance = (sphereRadius + d * AxMath.Sin(coneAngle)) / AxMath.ArcCos(coneAngle);
        
    var intersectionVector = new AxVector3();
    var intersectionPoint = new AxVector3();
    AxVector3.Scale(intersectionVector, coneVector, d);
    AxVector3.Add(intersectionPoint, coneApex, intersectionVector);
        
    var sphereToAxisDistance = AxVector3.Distance(sphereCenter, intersectionPoint);
    return sphereToAxisDistance <= maxSphereToAxisDistance;
};

/**
 * Checks whether a sphere with given center and radius intersects with a given view frustum, given by the frustum's projection marix
 * The view frustum matrix is a typical camera projection transform
 * @param {AxMatrix} viewFrustum The projection matrix which defines the view frustum. Generally, it's a typical transform of a camera projection
 * @param {AxVector3} sphereCenter The location of the sphere's center
 * @param {Number} sphereRadius The radius of the sphere
 * @returns {Boolean} True if the sphere intersects with the view frustum
 */
AxMaths.SphereViewFrustumIntersection = function(viewFrustum, sphereCenter, sphereRadius)
{
    var plane = new AxVector4();
	
	//Left Plane
    plane.x = viewFrustum._14 + viewFrustum._11;
    plane.y = viewFrustum._24 + viewFrustum._21;
    plane.z = viewFrustum._34 + viewFrustum._31;
    plane.w = viewFrustum._44 + viewFrustum._41;
    if (-AxMaths.PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Right Plane
    plane.x = viewFrustum._14 - viewFrustum._11;
    plane.y = viewFrustum._24 - viewFrustum._21;
    plane.z = viewFrustum._34 - viewFrustum._31;
    plane.w = viewFrustum._44 - viewFrustum._41;
    if (-AxMaths.PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Bottom Plane
    plane.x = viewFrustum._14 + viewFrustum._12;
    plane.y = viewFrustum._24 + viewFrustum._22;
    plane.z = viewFrustum._34 + viewFrustum._32;
    plane.w = viewFrustum._44 + viewFrustum._42;
    if (-AxMaths.PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Top Plane
    plane.x = viewFrustum._14 - viewFrustum._12;
    plane.y = viewFrustum._24 - viewFrustum._22;
    plane.z = viewFrustum._34 - viewFrustum._32;
    plane.w = viewFrustum._44 - viewFrustum._42;
    if (-AxMaths.PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Near Plane
    plane.x = viewFrustum._13;
    plane.y = viewFrustum._23;
    plane.z = viewFrustum._33;
    plane.w = viewFrustum._43;
    if (-AxMaths.PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;

    //Far Plane
    plane.x = viewFrustum._14 - viewFrustum._13;
    plane.y = viewFrustum._24 - viewFrustum._23;
    plane.z = viewFrustum._34 - viewFrustum._33;
    plane.w = viewFrustum._44 - viewFrustum._43;
    if (-AxMaths.PointToPlaneDistance(sphereCenter, plane) > sphereRadius)
        return false;
        
    return true;
};

/**
 * Calculates the barycentric coordinates of a 3d point onto the coordinate system defined by two axes
 * @param {AxVector2} result The resulting vector, which holds the barycentric coordinates of the given point
 * @param {AxVector3} cartesianCoords The 3d point in cartesian coordinates, for which to calculate barycentric coordinates
 * @param {AxVector3} uAxis The vector of the first axis to define the coordinate system. Corresponds to the X component of the result
 * @param {AxVector3} vAxis The vector of the second axis to define the coordinate system. Corresponds to the Y component of the result
 */
AxMaths.GetBarycentricCoordinates = function(result, cartesianCoords, uAxis, vAxis)
{
    var p = AxVector3.Dot(uAxis, vAxis);
    var q = AxVector3.Dot(uAxis, cartesianCoords);
    var r = AxVector3.Dot(vAxis, cartesianCoords);
    var s = AxVector3.Dot(uAxis, uAxis);
    var t = AxVector3.Dot(vAxis, vAxis);

    var invD = 1.0 / (s * t - p * p);

    result.x = (q * t - p * r) * invD;
    result.y = (r * s - p * q) * invD;
};

/**
 * Calculates the normal of the surfce of a triangle
 * @param {AxVector3} result The vector in which to store the resulting normal
 * @param {AxVector3} v1 The vector holding the position of the first vertex of the triangle
 * @param {AxVector3} v2 The vector holding the position of the second vertex of the triangle
 * @param {AxVector3} v3 The vector holding the position of the third vertex of the triangle
 */
AxMaths.GetNormal = function(result, v1, v2, v3)
{
    var dv1 = new AxVector3();
    var dv2 = new AxVector3();

    AxVector3.Subtract(dv1, v2, v1);
    AxVector3.Subtract(dv2, v3, v1);

    AxVector3.Cross(result, dv2, dv1);
    AxVector3.Normalize(result, result);
};

/**
 * Calculates the tangent and bitangent vectors on the surfce of a triangle, which are oriented along the basis vectors of the texture mapping
 * The tangent vector is oriented along the texture mapping's X axis (the absciss) and the bitangent vector is oriented along the texture mapping's Y axis (the ordinate)
 * @param {AxVector3} tangent The vector in which to store the resulting tangent
 * @param {AxVector3} biTangent The vector in which to store the resulting bitangent
 * @param {AxVector3} v1 The vector holding the position of the first vertex of the triangle
 * @param {AxVector3} v2 The vector holding the position of the second vertex of the triangle
 * @param {AxVector3} v3 The vector holding the position of the third vertex of the triangle
 * @param {AxVector2} t1 The vector holding the texture coordinates of the first vertex of the triangle
 * @param {AxVector2} t2 The vector holding the texture coordinates of the second vertex of the triangle
 * @param {AxVector2} t3 The vector holding the texture coordinates of the third vertex of the triangle
 */
AxMaths.GetTangents = function(tangent, biTangent, v1, v2, v3, t1, t2, t3)
{
    var dv1 = new AxVector3();
    var dv2 = new AxVector3();
    var dt1 = new AxVector2();
    var dt2 = new AxVector2();

    AxVector3.Subtract(dv1, v2, v1);
    AxVector3.Subtract(dv2, v3, v1);

    AxVector2.Subtract(dt1, t2, t1);
    AxVector2.Subtract(dt2, t3, t1);

    var a = new AxVector3();
    var b = new AxVector3();

    AxVector3.Scale(a, dv2, dt1.y);
    AxVector3.Scale(b, dv1, dt2.y);
    AxVector3.Subtract(tangent, a, b);
    AxVector3.Scale(tangent, tangent, 1.0 / (dt1.y * dt2.x - dt2.y * dt1.x));
    AxVector3.Normalize(tangent, tangent);

    AxVector3.Scale(a, dv2, dt1.x);
    AxVector3.Scale(b, dv1, dt2.x);
    AxVector3.Subtract(biTangent, a, b);
    AxVector3.Scale(biTangent, biTangent, 1.0 / (dt1.x * dt2.y - dt2.x * dt1.y));
    AxVector3.Normalize(biTangent, biTangent);
};

/**
 * Calculates the angle between two vectors
 * @param {AxVector3} v1 The first vector
 * @param {AxVector3} v2 The second vector
 * @returns {Number} The angle between the vectors
 */
AxMaths.VectorAngle = function(v1, v2)
{
    return AxMath.ArcCos(AxVector3.Dot(v1, v2) / (v1.GetLength() * v2.GetLength()));
};

/**
 * Calculates a the smallest number, which is a power of two and is equal or greater than the given value
 * @param {Integer} value The number, for which to get a wrapping power of two
 * @returns {Integer} The smallest power of two number, which wraps the given value
 */
AxMaths.GetPowerOf2 = function(value)
{
    var result = 1;
    while (result < value)
        result = result << 1;
    
    return result;
};

/**
 * Gets the closest number, equal to or greater than a given value, which is divisible by a given divisor
 * @param {Integer} value The number, for which to get a divisible value
 * @param {Integer} divisor The divisor for the value to get
 * @returns {Integer} The closest number, equal to or greater than the given value, which is divisible by the given divisor
 */
AxMaths.GetDivisibleBy = function(value, divisor)
{
    var mod = value % divisor;
    if (mod === 0)
        return value;
    else
        return value + divisor - mod;
};

/**
 * Converts an angular value in degrees to radians
 * @param {Number} degree Angular value in degrees
 * @returns {Number} The angular value in radians
 */
AxMaths.DegreeToRad = function(degree)
{
    return AxMath.Pi * degree / 180.0;
};

/**
 * Converts an angular value in radians to degrees
 * @param {Number} rad Angular value in radians
 * @returns {Number} The angular value in degrees
 */
AxMaths.RadToDegree = function(rad)
{
    return 180.0 * rad / AxMath.Pi;
};

/**
 * Calculates the angle under which an object of a given size is seen at a given distance
 * @param {Number} distance Object distance
 * @param {Number} size Object size
 * @returns {Number} Viewed angle in radians
 */
AxMaths.AngleFromDistanceAndSize = function(distance, size)
{
    return 2.0 * AxMath.ArcTan(size / (2.0 * distance));
};

/**
 * Calculates the distance of an object of a given size, which is seen under a given angle
 * @param {Number} size Object size
 * @param {Number} angle Viewed angle in radians
 * @returns {Number} Distance to the object
 */
AxMaths.DistanceFromSizeAndAngle = function(size, angle)
{
    return size / (2.0 * AxMath.Tan(angle / 2.0));
};

/**
 * Calculates the size of an object at a given distance, which is seen under a given angle
 * @param {Number} distance Distance to object
 * @param {Number} angle Viewed angle in radians
 * @returns {Number} Size of the object
 */
AxMaths.SizeFromDistanceAndAngle = function(distance, angle)
{
    return 2.0 * distance * AxMath.Tan(angle / 2.0);
};

/**
 * Converts lens focal distance to a Field of view angle
 * @param {Number} lens The lens' focal distance. Must be in the same measurement units as the filmSize (for example, millimeters)
 * @param {!Number} filmSize The size of the lens' projection film. Must be in the same measurement units as the focal distance (for example, millimeters). If omitted, a typical film size of 36mm is assumed
 * @returns {Number} Field of view angle in radians of the lens
 */
AxMaths.LensToFOV = function(lens, filmSize)
{
    if (AxUtils.IsUndefinedOrNull(filmSize))
        filmSize = 36.0;

    return AxMaths.AngleFromDistanceAndSize(lens, filmSize);
};

/**
 * Converts a Field of view angle to lens focal distance
 * @param {Number} fov The field of view angle of the lens in radians
 * @param {!Number} filmSize The size of the lens' projection film. Must be in the same measurement units as the focal distance (for example, millimeters). If omitted, a typical film size of 36mm is assumed
 * @returns {Number} Lens focal distance
 */
AxMaths.FOVToLens = function(fov, filmSize)
{
    if (AxUtils.IsUndefinedOrNull(filmSize))
        filmSize = 36.0;
    
    return AxMaths.DistanceFromSizeAndAngle(fov, filmSize);
};
