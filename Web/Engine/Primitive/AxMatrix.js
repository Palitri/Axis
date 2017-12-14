/**
 * Creates a matrix object and initializes it as identity
 * @constructor
 */
function AxMatrix()
{
    this._11 = 1.0;
    this._12 = 0.0;
    this._13 = 0.0;
    this._14 = 0.0;
    this._21 = 0.0;
    this._22 = 1.0;
    this._23 = 0.0;
    this._24 = 0.0;
    this._31 = 0.0;
    this._32 = 0.0;
    this._33 = 1.0;
    this._34 = 0.0;
    this._41 = 0.0;
    this._42 = 0.0;
    this._43 = 0.0;
    this._44 = 1.0;
}

AxMatrix.handedness_Left    = 0;
AxMatrix.handedness_Right   = 1;

AxMatrix.handedness = AxMatrix.handedness_Right;

/**
 * Calculates the determinant of the matrix
 * @return {Number} The determinant of the matrix
 */
AxMatrix.prototype.GetDeterminant = function()
{
    return this._11 * this._22 * this._33 * this._44 +
        this._11 * this._23 * this._34 * this._42 +
        this._11 * this._24 * this._32 * this._43 +

        this._12 * this._21 * this._34 * this._43 +
        this._12 * this._23 * this._31 * this._44 +
        this._12 * this._24 * this._33 * this._41 +

        this._13 * this._21 * this._32 * this._44 +
        this._13 * this._22 * this._34 * this._41 +
        this._13 * this._24 * this._31 * this._42 +

        this._14 * this._21 * this._33 * this._42 +
        this._14 * this._22 * this._31 * this._43 +
        this._14 * this._23 * this._32 * this._41 -

        this._11 * this._22 * this._34 * this._43 -
        this._11 * this._23 * this._32 * this._44 -
        this._11 * this._24 * this._33 * this._42 -

        this._12 * this._21 * this._33 * this._44 -
        this._12 * this._23 * this._34 * this._41 -
        this._12 * this._24 * this._31 * this._43 -

        this._13 * this._21 * this._34 * this._42 -
        this._13 * this._22 * this._31 * this._44 -
        this._13 * this._24 * this._32 * this._41 -

        this._14 * this._21 * this._32 * this._43 -
        this._14 * this._22 * this._33 * this._41 -
        this._14 * this._23 * this._31 * this._42;
};

/**
 * Compares the matrix with another one and returns true if both are identical
 * @param {AxMatrix} matrix The matrix to compare with
 * @returns {Boolean} True if identical to the given matrix
 */
AxMatrix.prototype.Equals = function(matrix)
{
    return (this._11 === matrix._11) &&
        (this._12 === matrix._12) &&
        (this._13 === matrix._13) &&
        (this._14 === matrix._14) &&
        (this._21 === matrix._21) &&
        (this._22 === matrix._22) &&
        (this._23 === matrix._23) &&
        (this._24 === matrix._24) &&
        (this._31 === matrix._31) &&
        (this._32 === matrix._32) &&
        (this._33 === matrix._33) &&
        (this._34 === matrix._34) &&
        (this._41 === matrix._41) &&
        (this._42 === matrix._42) &&
        (this._43 === matrix._43) &&
        (this._44 === matrix._44);
};


// Static Methods



/**
 * Copies the source matrix into the result
 * @param {AxMatrix} result The matrix to copy to
 * @param {AxMatrix} source The matrix to copy from
 */
AxMatrix.Copy = function(result, source)
{
	result._11 = source._11;
	result._12 = source._12;
	result._13 = source._13;
	result._14 = source._14;
	result._21 = source._21;
	result._22 = source._22;
	result._23 = source._23;
	result._24 = source._24;
	result._31 = source._31;
	result._32 = source._32;
	result._33 = source._33;
	result._34 = source._34;
	result._41 = source._41;
	result._42 = source._42;
	result._43 = source._43;
	result._44 = source._44;
};
	
/**
 * Copies the source matrix into the result, treating them as 3x3 matrices by operating only on their first three rows and columns, ignoring the fourth row and column
 * The fourth row and column are irrelevant in this operation and are ignored and left unchanged.
 * It can be useful for example, in operations where translation must be disregarded
 * @param {AxMatrix} result The matrix to copy to
 * @param {AxMatrix} source The matrix to copy from
 */
AxMatrix.Copy3x3 = function(result, source)
{
	result._11 = source._11;
	result._12 = source._12;
	result._13 = source._13;
	result._21 = source._21;
	result._22 = source._22;
	result._23 = source._23;
	result._31 = source._31;
	result._32 = source._32;
	result._33 = source._33;
};

/**
 * Creates an identity matrix
 * @param {AxMatrix} result The matrix to create identity into
 */
AxMatrix.CreateIdentity = function(result)
{
    result._11 = 1.0;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = 1.0;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = 1.0;
    result._34 = 0.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};

/**
 * Creates a rotation transformation around the X axis
 * @param {AxMatrix} result The matrix to create the rotation transformation into
 * @param {Number} phi The angle of the rotation around the X axis
 */
AxMatrix.CreateRotationX = function(result, phi)
{
    var sinPhi = AxMath.Sin(phi);
    var cosPhi = AxMath.Cos(phi);

    result._11 = 1.0;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = cosPhi;
    result._23 = sinPhi;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = -sinPhi;
    result._33 = cosPhi;
    result._34 = 0.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};

/**
 * Creates a rotation transformation around the Y axis
 * @param {AxMatrix} result The matrix to create the rotation transformation into
 * @param {Number} phi The angle of the rotation around the Y axis
 */
AxMatrix.CreateRotationY = function(result, phi)
{
    var sinPhi = AxMath.Sin(phi);
    var cosPhi = AxMath.Cos(phi);

    result._11 = cosPhi;
    result._12 = 0.0;
    result._13 = -sinPhi;
    result._21 = 0.0;
    result._14 = 0.0;
    result._22 = 1.0;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = sinPhi;
    result._32 = 0.0;
    result._33 = cosPhi;
    result._34 = 0.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};

/**
 * Creates a rotation transformation around the Z axis
 * @param {AxMatrix} result The matrix to create the rotation transformation into
 * @param {Number} phi The angle of the rotation around the Z axis
 */
AxMatrix.CreateRotationZ = function(result, phi)
{
    var sinPhi = AxMath.Sin(phi);
    var cosPhi = AxMath.Cos(phi);

    result._11 = cosPhi;
    result._12 = sinPhi;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = -sinPhi;
    result._22 = cosPhi;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = 1.0;
    result._34 = 0.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};

/**
 * Creates a rotation transformation around an arbitrary axis
 * Supports two input variants - a vector input and a XYZ input
 * @param {AxMatrix} result The matrix to create the rotation transformation into
 * @param {Number|AxVector3} x For XYZ input - the X component of the axis around which to rotate. For vector input - the vector around which to rotate
 * @param {Number} y For XYZ input - the Y component of the axis around which to rotate. For vector input - the angle of rotation
 * @param {!Number} z For XYZ input - the Z component of the axis around which to rotate. Omitted for vector input
 * @param {!Number} phi For XYZ input - the angle of rotation around the given axis. Omitted for vector input
 */
AxMatrix.CreateRotationAxis = function(result, x, y, z, phi)
{
    if (!AxUtils.IsUndefinedOrNull(x.x))
    {
        AxMatrix.CreateRotationAxis_2(result, x, y);
        return;
    }
    
    var sinPhi = AxMath.Sin(phi);
    var cosPhi = AxMath.Cos(phi);
    var invCosPhi = 1 - cosPhi;
    var sinPhiX = sinPhi * x;
    var sinPhiY = sinPhi * y;
    var sinPhiZ = sinPhi * z;
    var invCosPhiXY = invCosPhi * x * y;
    var invCosPhiXZ = invCosPhi * x * z;
    var invCosPhiYZ = invCosPhi * y * z;

    result._11 = cosPhi + invCosPhi * x * x;
    result._12 = invCosPhiXY - sinPhiZ;
    result._13 = invCosPhiXZ + sinPhiY;
    result._14 = 0.0;
    result._21 = invCosPhiXY + sinPhiZ;
    result._22 = cosPhi + invCosPhi * y * y;
    result._23 = invCosPhiYZ - sinPhiX;
    result._24 = 0.0;
    result._31 = invCosPhiXZ - sinPhiY;
    result._32 = invCosPhiYZ + sinPhiX;
    result._33 = cosPhi + invCosPhi * z * z;
    result._34 = 0.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};


/**
 * Creates a rotation transformation around an arbitrary axis
 * @param {AxMatrix} result The matrix to create the rotation transformation into
 * @param {AxVector3} axis A vector representing the axis around which to rotate
 * @param {Number} phi The angle of the rotation around the given axis
 */
AxMatrix.CreateRotationAxis_2 = function(result, axis, phi)
{
    AxMatrix.CreateRotationAxis(result, axis.x, axis.y, axis.z, phi);
};

/**
 * Creates a translation transformation
 * Supports two input variants - a vector input and a XYZ input
 * @param {AxMatrix} result The matrix to create the translation transformation into
 * @param {Number|AxVector3} x For XYZ input - the X component of the translation. For vector input - the translation vector
 * @param {!Number} y For XYZ input - the Y component of the translation. Omitted for vector input
 * @param {!Number} z For XYZ input - the Z component of the translation. Omitted for vector input
 */
AxMatrix.CreateTranslation = function(result, x, y, z)
{
    if (!AxUtils.IsUndefinedOrNull(x.x))
    {
        AxMatrix.CreateTranslation_2(result, x, y);
        return;
    }
    
    result._11 = 1.0;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = 1.0;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = 1.0;
    result._34 = 0.0;
    result._41 = x;
    result._42 = y;
    result._43 = z;
    result._44 = 1.0;
};

/**
 * Creates a translation transformation
 * @param {AxMatrix} result The matrix to create the translation transformation into
 * @param {AxVector3} translation The translation vector
 */
AxMatrix.CreateTranslation_2 = function(result, translation)
{
    AxMatrix.CreateTranslation(result, translation.x, translation.y, translation.z);
};

/**
 * Creates a scaling transformation
 * Supports two input variants - a vector input and a XYZ input
 * @param {AxMatrix} result The matrix to create the scaling transformation into
 * @param {Number|AxVector3} x For XYZ input - the X component of the scaling. For vector input - The scaling vector
 * @param {!Number} y For XYZ input - the Y component of the scaling. Omitted for vector input
 * @param {!Number} z For XYZ input - the Z component of the scaling. Omitted for vector input
 */
AxMatrix.CreateScaling = function(result, x, y, z)
{
    if (!AxUtils.IsUndefinedOrNull(x.x))
    {
        AxMatrix.CreateScaling_2(result, x, y);
        return;
    }

    result._11 = x;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = y;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = z;
    result._34 = 0.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};

/**
 * Creates a scaling transformation
 * @param {AxMatrix} result The matrix to create the scaling transformation into
 * @param {AxVector3} scaling The scaling vector
 */
AxMatrix.CreateScaling_2 = function(result, scaling)
{
    AxMatrix.CreateScaling(result, scaling.x, scaling.y, scaling.z);
};

/**
 * Creates an observer's view transformation
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {AxVector3} position The location of the observer
 * @param {AxVector3} lookAt The location at which the abserver is looking at
 * @param {AxVector3} up The observer's up direction. It represents how the observer is rolled.
 */
AxMatrix.CreateLookAt = function(result, position, lookAt, up)
{
    var xAxis = new AxVector3();
    var yAxis = new AxVector3();
    var zAxis = new AxVector3();

    AxVector3.Subtract(zAxis, position, lookAt);
    AxVector3.Normalize(zAxis, zAxis);
    AxVector3.Cross(xAxis, up, zAxis);
    AxVector3.Normalize(xAxis, xAxis);
    AxVector3.Cross(yAxis, zAxis, xAxis);

    result._11 = xAxis.x;
    result._12 = yAxis.x;
    result._13 = zAxis.x;
    result._14 = 0.0;
    result._21 = xAxis.y;
    result._22 = yAxis.y;
    result._23 = zAxis.y;
    result._24 = 0.0;
    result._31 = xAxis.z;
    result._32 = yAxis.z;
    result._33 = zAxis.z;
    result._34 = 0.0;
    result._41 = -AxVector3.Dot(xAxis, position);
    result._42 = -AxVector3.Dot(yAxis, position);
    result._43 = -AxVector3.Dot(zAxis, position);
    result._44 = 1.0;
};

/**
 * Creates a perspective projection transformation
 * The handedness of the projection is determined by AxMatrix.handedness 
 * The aspectHorizontal and aspectVertical parameters can be used to correct the field of view of the projection, usually either horizontally or vertically, according to the dimensions of the projection plane.
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {Number} fov The field of view angle of the projectin in radians
 * @param {Number} aspectHorizontal Horizontal aspect ratio factor. Value of 1.0 results in no change
 * @param {Number} aspectVertical Vertical aspect ratio factor. Value of 1.0 results in no change
 * @param {Number} zn The projection's near clip plane distance
 * @param {Number} zf The projection's far clip plane distance
 */
AxMatrix.CreatePerspectiveFieldOfView = function(result, fov, aspectHorizontal, aspectVertical, zn, zf)
{
    switch (AxMatrix.handedness)
    {
        case AxMatrix.handedness_Right:
            return AxMatrix.CreatePerspectiveFieldOfViewRH(result, fov, aspectHorizontal, aspectVertical, zn, zf);
        case AxMatrix.handedness_Left:
            return AxMatrix.CreatePerspectiveFieldOfViewLH(result, fov, aspectHorizontal, aspectVertical, zn, zf);
        default:
            return AxMatrix.CreatePerspectiveFieldOfViewRH(result, fov, aspectHorizontal, aspectVertical, zn, zf);
    }
};


/**
 * Creates a right handed perspective projection transformation
 * The aspectHorizontal and aspectVertical parameters can be used to correct the field of view of the projection, usually either horizontally or vertically, according to the dimensions of the projection plane.
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {Number} fov The field of view angle of the projectin in radians
 * @param {Number} aspectHorizontal Horizontal aspect ratio factor. Value of 1.0 results in no change
 * @param {Number} aspectVertical Vertical aspect ratio factor. Value of 1.0 results in no change
 * @param {Number} zn The projection's near clip plane distance
 * @param {Number} zf The projection's far clip plane distance
 */
AxMatrix.CreatePerspectiveFieldOfViewRH = function(result, fov, aspectHorizontal, aspectVertical, zn, zf)
{
    var f = 1.0 / AxMath.Tan(fov * 0.5);
    var dz = zn - zf;

    result._11 = f / aspectHorizontal;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = f / aspectVertical;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = zf / dz;
    result._34 = -1.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = (zn * zf) / dz;
    result._44 = 0.0;
};
/**
 * Creates a left handed perspective projection transformation
 * The aspectHorizontal and aspectVertical parameters can be used to correct the field of view of the projection, usually either horizontally or vertically, according to the dimensions of the projection plane.
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {Number} fov The field of view angle of the projectin in radians
 * @param {Number} aspectHorizontal Horizontal aspect ratio factor. Value of 1.0 results in no change
 * @param {Number} aspectVertical Vertical aspect ratio factor. Value of 1.0 results in no change
 * @param {Number} zn The projection's near clip plane distance
 * @param {Number} zf The projection's far clip plane distance
 */
AxMatrix.CreatePerspectiveFieldOfViewLH = function(result, fov, aspectHorizontal, aspectVertical, zn, zf)
{
    var f = 1.0 / AxMath.Tan(fov * 0.5);
    var dz = zn - zf;

    result._11 = f / aspectHorizontal;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = f / aspectVertical;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = zf / dz;
    result._34 = 1.0;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = - (zn * zf) / dz;
    result._44 = 0.0;
};

/**
 * Creates an orthogonal projection transformation
 * The handedness of the projection is determined by AxMatrix.handedness 
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {Number} width The projection's width
 * @param {Number} height The projection's height
 * @param {Number} zn The projection's near clip plane distance
 * @param {Number} zf The projection's far clip plane distance
 */
AxMatrix.CreateOrthographic = function(result, width, height, zn, zf)
{
    switch (AxMatrix.handedness)
    {
        case AxMatrix.handedness_Right:
            return AxMatrix.CreateOrthographicRH(result, width, height, zn, zf);
        case AxMatrix.handedness_Left:
            return AxMatrix.CreateOrthographicLH(result, width, height, zn, zf);
        default:
            return AxMatrix.CreateOrthographicRH(result, width, height, zn, zf);
    }
};

/**
 * Creates a right handed orthogonal projection transformation
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {Number} width The projection's width
 * @param {Number} height The projection's height
 * @param {Number} zn The projection's near clip plane distance
 * @param {Number} zf The projection's far clip plane distance
 */
AxMatrix.CreateOrthographicRH = function(result, width, height, zn, zf)
{
    var dz = zn - zf;

    result._11 = 2.0 / width;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = 2.0 / height;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = 1.0 / dz;
    result._34 = zn / dz;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};

/**
 * Creates a left handed orthogonal projection transformation
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {Number} width The projection's width
 * @param {Number} height The projection's height
 * @param {Number} zn The projection's near clip plane distance
 * @param {Number} zf The projection's far clip plane distance
 */
AxMatrix.CreateOrthographicLH = function(result, width, height, zn, zf)
{
    var dz = zn - zf;

    result._11 = 2.0 / width;
    result._12 = 0.0;
    result._13 = 0.0;
    result._14 = 0.0;
    result._21 = 0.0;
    result._22 = 2.0 / height;
    result._23 = 0.0;
    result._24 = 0.0;
    result._31 = 0.0;
    result._32 = 0.0;
    result._33 = - 1.0 / dz;
    result._34 = zn / dz;
    result._41 = 0.0;
    result._42 = 0.0;
    result._43 = 0.0;
    result._44 = 1.0;
};
    
/**
 * Creates a matrix tranformation oriented towards the given vector
 * @param {AxMatrix} result The matrix to create the transformation into
 * @param {AxVector3} orientationVector The vector holding the orientation
 * @param {AxVector3} orientationOrigin The vector holding the orientation origin
 */
AxMatrix.CreateOrientation = function(result, orientationVector, orientationOrigin)
{
	var rotationAxis = new AxVector3();
	
	var phi = AxMaths.VectorAngle(orientationOrigin, orientationVector);
	AxVector3.Cross(rotationAxis, orientationOrigin, orientationVector);
	AxVector3.Normalize(rotationAxis, rotationAxis);
	AxMatrix.CreateRotationAxis(result, rotationAxis, -phi);
};

// Creates a matrix tranformation with the given position, oriented towards the given target
AxMatrix.CreateOrientedPosition = function(result, position, target, orientationOrigin)
{
	var orientationVector = new AxVector3();
	AxVector3.Subtract(orientationVector, target, position);

	AxMatrix.CreateOrientation(result, orientationVector, orientationOrigin);
	AxMatrix.SetTranslation(result, position);
	//AxMatrix translation;
	//AxMatrix.CreateTranslation(translation, position);
	//AxMatrix.Multiply(result, result, translation);
};

// Creates a matrix tranformation with the given position, oriented as possible towards the given target, with the given axis to rotate around
AxMatrix.CreateOrientationAxis = function(result, orientationVector, orientationOrigin, axis)
{
	var actualTarget = new AxVector3();
	var origin = new AxVector3();
        origin.Set(0.0, 0.0, 0.0);

	AxMaths.PointToPlaneProjection(actualTarget, orientationVector, origin, axis);

	AxMatrix.CreateOrientation(result, actualTarget, orientationOrigin);
};

// Creates a matrix tranformation with the given position, oriented as possible towards the given target, with the given axis to rotate around
AxMatrix.CreateOrientationAxis = function(result, position, target, orientationOrigin, axis)
{
	var actualTarget = new AxVector3();

	AxMaths.PointToPlaneProjection(actualTarget, target, position, axis);

	AxMatrix.CreateOrientedPosition(result, position, actualTarget, orientationOrigin);
};

/**
 * Multiplies two matrices
 * @param {AxMatrix} result The matrix to hold the result of the multiplication
 * @param {AxMatrix} matrix1 The first matrix to be multiplied
 * @param {AxMatrix} matrix2 The second matrix to be multiplied
 */
AxMatrix.Multiply = function(result, matrix1, matrix2)
{
    var _11 = 	matrix1._11 * matrix2._11 +
                matrix1._12 * matrix2._21 +
                matrix1._13 * matrix2._31 +
                matrix1._14 * matrix2._41;
    var _12 = 	matrix1._11 * matrix2._12 +
                matrix1._12 * matrix2._22 +
                matrix1._13 * matrix2._32 +
                matrix1._14 * matrix2._42;
    var _13 = 	matrix1._11 * matrix2._13 +
                matrix1._12 * matrix2._23 +
                matrix1._13 * matrix2._33 +
                matrix1._14 * matrix2._43;
    var _14 = 	matrix1._11 * matrix2._14 +
                matrix1._12 * matrix2._24 +
                matrix1._13 * matrix2._34 +
                matrix1._14 * matrix2._44;
    var _21 = 	matrix1._21 * matrix2._11 +
                matrix1._22 * matrix2._21 +
                matrix1._23 * matrix2._31 +
                matrix1._24 * matrix2._41;
    var _22 = 	matrix1._21 * matrix2._12 +
                matrix1._22 * matrix2._22 +
                matrix1._23 * matrix2._32 +
                matrix1._24 * matrix2._42;
    var _23 = 	matrix1._21 * matrix2._13 +
                matrix1._22 * matrix2._23 +
                matrix1._23 * matrix2._33 +
                matrix1._24 * matrix2._43;
    var _24 = 	matrix1._21 * matrix2._14 +
                matrix1._22 * matrix2._24 +
                matrix1._23 * matrix2._34 +
                matrix1._24 * matrix2._44;
    var _31 = 	matrix1._31 * matrix2._11 +
                matrix1._32 * matrix2._21 +
                matrix1._33 * matrix2._31 +
                matrix1._34 * matrix2._41;
    var _32 = 	matrix1._31 * matrix2._12 +
                matrix1._32 * matrix2._22 +
                matrix1._33 * matrix2._32 +
                matrix1._34 * matrix2._42;
    var _33 = 	matrix1._31 * matrix2._13 +
                matrix1._32 * matrix2._23 +
                matrix1._33 * matrix2._33 +
                matrix1._34 * matrix2._43;
    var _34 = 	matrix1._31 * matrix2._14 +
                matrix1._32 * matrix2._24 +
                matrix1._33 * matrix2._34 +
                matrix1._34 * matrix2._44;
    var _41 = 	matrix1._41 * matrix2._11 +
                matrix1._42 * matrix2._21 +
                matrix1._43 * matrix2._31 +
                matrix1._44 * matrix2._41;
    var _42 = 	matrix1._41 * matrix2._12 +
                matrix1._42 * matrix2._22 +
                matrix1._43 * matrix2._32 +
                matrix1._44 * matrix2._42;
    var _43 = 	matrix1._41 * matrix2._13 +
                matrix1._42 * matrix2._23 +
                matrix1._43 * matrix2._33 +
                matrix1._44 * matrix2._43;
    var _44 = 	matrix1._41 * matrix2._14 +
                matrix1._42 * matrix2._24 +
                matrix1._43 * matrix2._34 +
                matrix1._44 * matrix2._44;

    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._14 = _14;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._24 = _24;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
    result._34 = _34;
    result._41 = _41;
    result._42 = _42;
    result._43 = _43;
    result._44 = _44;
};

/**
 * Multiplies two matrices, treating them as 3x3 matrices by operating only on their first three rows and columns, ignoring the fourth row and column
 * The fourth row and column are irrelevant in this operation and are ignored and left unchanged.
 * It can be useful for example, in operations where translation must be disregarded
 * @param {AxMatrix} result The matrix to hold the result of the multiplication
 * @param {AxMatrix} matrix1 The first matrix to be multiplied
 * @param {AxMatrix} matrix2 The second matrix to be multiplied
 */
AxMatrix.Multiply3x3 = function(result, matrix1, matrix2)
{
    var _11 = 	matrix1._11 * matrix2._11 +
                matrix1._12 * matrix2._21 +
                matrix1._13 * matrix2._31 +
                matrix1._14 * matrix2._41;
    var _12 = 	matrix1._11 * matrix2._12 +
                matrix1._12 * matrix2._22 +
                matrix1._13 * matrix2._32 +
                matrix1._14 * matrix2._42;
    var _13 = 	matrix1._11 * matrix2._13 +
                matrix1._12 * matrix2._23 +
                matrix1._13 * matrix2._33 +
                matrix1._14 * matrix2._43;
    var _21 = 	matrix1._21 * matrix2._11 +
                matrix1._22 * matrix2._21 +
                matrix1._23 * matrix2._31 +
                matrix1._24 * matrix2._41;
    var _22 = 	matrix1._21 * matrix2._12 +
                matrix1._22 * matrix2._22 +
                matrix1._23 * matrix2._32 +
                matrix1._24 * matrix2._42;
    var _23 = 	matrix1._21 * matrix2._13 +
                matrix1._22 * matrix2._23 +
                matrix1._23 * matrix2._33 +
                matrix1._24 * matrix2._43;
    var _31 = 	matrix1._31 * matrix2._11 +
                matrix1._32 * matrix2._21 +
                matrix1._33 * matrix2._31 +
                matrix1._34 * matrix2._41;
    var _32 = 	matrix1._31 * matrix2._12 +
                matrix1._32 * matrix2._22 +
                matrix1._33 * matrix2._32 +
                matrix1._34 * matrix2._42;
    var _33 = 	matrix1._31 * matrix2._13 +
                matrix1._32 * matrix2._23 +
                matrix1._33 * matrix2._33 +
                matrix1._34 * matrix2._43;

    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
};

/**
 * Transposes a matrix
 * @param {AxMatrix} result The matrix to hold the result of the transposing
 * @param {AxMatrix} source The source matrix of the transposing
 */
AxMatrix.Transpose = function(result, source)
{
    var _11 = source._11;
    var _12 = source._21;
    var _13 = source._31;
    var _14 = source._41;
    var _21 = source._12;
    var _22 = source._22;
    var _23 = source._32;
    var _24 = source._42;
    var _31 = source._13;
    var _32 = source._23;
    var _33 = source._33;
    var _34 = source._43;
    var _41 = source._14;
    var _42 = source._24;
    var _43 = source._34;
    var _44 = source._44;
        
    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._14 = _14;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._24 = _24;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
    result._34 = _34;
    result._41 = _41;
    result._42 = _42;
    result._43 = _43;
    result._44 = _44;
};
    

/**
 * Transposes a matrix, treating it as a 3x3 matrix by operating only on the first three rows and columns, ignoring the fourth row and column
 * The fourth row and column are irrelevant in this operation and are ignored and left unchanged.
 * It can be useful for example, in operations where translation must be disregarded
 * @param {AxMatrix} result The matrix to hold the result of the transposing
 * @param {AxMatrix} source The source matrix of the transposing
 */
AxMatrix.Transpose3x3 = function(result, source)
{
    var _11 = source._11;
    var _12 = source._21;
    var _13 = source._31;
    var _21 = source._12;
    var _22 = source._22;
    var _23 = source._32;
    var _31 = source._13;
    var _32 = source._23;
    var _33 = source._33;
        
    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
};

/**
 * Inverts a matrix
 * @param {AxMatrix} result The matrix to hold the result of the inversion
 * @param {AxMatrix} source The source matrix of the inversion
 */
AxMatrix.Invert = function(result, source)
{
    var det = source.GetDeterminant();

    if (det === 0.0)
        return;

    det = 1.0 / det;

    var _11 = det * (source._22 * source._33 * source._44 +
                    source._23 * source._34 * source._42 +
                    source._24 * source._32 * source._43 -
                    source._22 * source._34 * source._43 -
                    source._23 * source._32 * source._44 -
                    source._24 * source._33 * source._42);

    var _12 = det * (source._12 * source._34 * source._43 +
                    source._13 * source._32 * source._44 +
                    source._14 * source._33 * source._42 -
                    source._12 * source._33 * source._44 -
                    source._13 * source._34 * source._42 -
                    source._14 * source._32 * source._43);

    var _13 = det * (source._12 * source._23 * source._44 +
                    source._13 * source._24 * source._42 +
                    source._14 * source._22 * source._43 -
                    source._12 * source._24 * source._43 -
                    source._13 * source._22 * source._44 -
                    source._14 * source._23 * source._42);

    var _14 = det * (source._12 * source._24 * source._33 +
                    source._13 * source._22 * source._34 +
                    source._14 * source._23 * source._32 -
                    source._12 * source._23 * source._34 -
                    source._13 * source._24 * source._32 -
                    source._14 * source._22 * source._33);

    var _21 = det * (source._21 * source._34 * source._43 +
                    source._23 * source._31 * source._44 +
                    source._24 * source._33 * source._41 -
                    source._21 * source._33 * source._44 -
                    source._23 * source._34 * source._41 -
                    source._24 * source._31 * source._43);

    var _22 = det * (source._11 * source._33 * source._44 +
                    source._13 * source._34 * source._41 +
                    source._14 * source._31 * source._43 -
                    source._11 * source._34 * source._43 -
                    source._13 * source._31 * source._44 -
                    source._14 * source._33 * source._41);

    var _23 = det * (source._11 * source._24 * source._43 +
                    source._13 * source._21 * source._44 +
                    source._14 * source._23 * source._41 -
                    source._11 * source._23 * source._44 -
                    source._13 * source._24 * source._41 -
                    source._14 * source._21 * source._43);

    var _24 = det * (source._11 * source._23 * source._34 +
                    source._13 * source._24 * source._31 +
                    source._14 * source._21 * source._33 -
                    source._11 * source._24 * source._33 -
                    source._13 * source._21 * source._34 -
                    source._14 * source._23 * source._31);

    var _31 = det * (source._21 * source._32 * source._44 +
                    source._22 * source._34 * source._41 +
                    source._24 * source._31 * source._42 -
                    source._21 * source._34 * source._42 -
                    source._22 * source._31 * source._44 -
                    source._24 * source._32 * source._41);

    var _32 = det * (source._11 * source._34 * source._42 +
                    source._12 * source._31 * source._44 +
                    source._14 * source._32 * source._41 -
                    source._11 * source._32 * source._44 -
                    source._12 * source._34 * source._41 -
                    source._14 * source._31 * source._42);

    var _33 = det * (source._11 * source._22 * source._44 +
                    source._12 * source._24 * source._41 +
                    source._14 * source._21 * source._42 -
                    source._11 * source._24 * source._42 -
                    source._12 * source._21 * source._44 -
                    source._14 * source._22 * source._41);

    var _34 = det * (source._11 * source._24 * source._32 +
                    source._12 * source._21 * source._34 +
                    source._14 * source._22 * source._31 -
                    source._11 * source._22 * source._34 -
                    source._12 * source._24 * source._31 -
                    source._14 * source._21 * source._32);

    var _41 = det * (source._21 * source._33 * source._42 +
                    source._22 * source._31 * source._43 +
                    source._23 * source._32 * source._41 -
                    source._21 * source._32 * source._43 -
                    source._22 * source._33 * source._41 -
                    source._23 * source._31 * source._42);

    var _42 = det * (source._11 * source._32 * source._43 +
                    source._12 * source._33 * source._41 +
                    source._13 * source._31 * source._42 -
                    source._11 * source._33 * source._42 -
                    source._12 * source._31 * source._43 -
                    source._13 * source._32 * source._41);

    var _43 = det * (source._11 * source._23 * source._42 +
                    source._12 * source._21 * source._43 +
                    source._13 * source._22 * source._41 -
                    source._11 * source._22 * source._43 -
                    source._12 * source._23 * source._41 -
                    source._13 * source._21 * source._42);

    var _44 = det * (source._11 * source._22 * source._33 +
                    source._12 * source._23 * source._31 +
                    source._13 * source._21 * source._32 -
                    source._11 * source._23 * source._32 -
                    source._12 * source._21 * source._33 -
                    source._13 * source._22 * source._31);
        
    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._14 = _14;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._24 = _24;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
    result._34 = _34;
    result._41 = _41;
    result._42 = _42;
    result._43 = _43;
    result._44 = _44;
};

/**
 * Inverts a matrix, treating it as a 3x3 matrix by operating only on the first three rows and columns, ignoring the fourth row and column
 * The fourth row and column are irrelevant in this operation and are ignored and left unchanged.
 * It is much faster than inverting the whole matrix and can be useful for example, in operations where translation must be disregarded
 * @param {AxMatrix} result The matrix to hold the result of the inversion
 * @param {AxMatrix} source The source matrix of the inversion
 */
AxMatrix.Invert3x3 = function(result, source)
{
    var det =   source._12 * source._23 * source._31 +
                source._13 * source._21 * source._32 -
                source._11 * source._23 * source._32 -
                source._12 * source._21 * source._33 -
                source._13 * source._22 * source._31;

    if (det === 0.0)
        return;

    det = 1.0 / det;

    var _11 = det * (source._22 * source._33 - source._23 * source._32);
    var _12 = det * (source._13 * source._32 - source._12 * source._33);
    var _13 = det * (source._12 * source._23 - source._13 * source._22);
    var _21 = det * (source._23 * source._31 - source._21 * source._33);
    var _22 = det * (source._11 * source._33 - source._13 * source._31);
    var _23 = det * (source._13 * source._21 - source._11 * source._23);
    var _31 = det * (source._21 * source._32 - source._22 * source._31);
    var _32 = det * (source._12 * source._31 - source._11 * source._32);
    var _33 = det * (source._11 * source._22 - source._12 * source._21);

    result._11 = _11;
    result._12 = _12;
    result._13 = _13;
    result._21 = _21;
    result._22 = _22;
    result._23 = _23;
    result._31 = _31;
    result._32 = _32;
    result._33 = _33;
};

/**
 * Inverts and transposes a matrix, treating it as a 3x3 matrix by operating only on the first three rows and columns, ignoring the fourth row and column
 * The fourth row and column are irrelevant in this operation and are ignored and left unchanged.
 * It is much faster than inverting the whole matrix and can be useful in some cases, particularly for generating a transformation to apply on normals, when a non-uniform scaling is in effect
 * @param {AxMatrix} result The matrix to hold the result of the inversion
 * @param {AxMatrix} source The source matrix of the inversion
 */
AxMatrix.InvertTranspose3x3 = function(result, source)
{
    var det =   source._12 * source._23 * source._31 +
                source._13 * source._21 * source._32 -
                source._11 * source._23 * source._32 -
                source._12 * source._21 * source._33 -
                source._13 * source._22 * source._31;

    if (det === 0.0)
        return;

    det = 1.0 / det;

    var _11 = det * (source._22 * source._33 - source._23 * source._32);
    var _12 = det * (source._13 * source._32 - source._12 * source._33);
    var _13 = det * (source._12 * source._23 - source._13 * source._22);
    var _21 = det * (source._23 * source._31 - source._21 * source._33);
    var _22 = det * (source._11 * source._33 - source._13 * source._31);
    var _23 = det * (source._13 * source._21 - source._11 * source._23);
    var _31 = det * (source._21 * source._32 - source._22 * source._31);
    var _32 = det * (source._12 * source._31 - source._11 * source._32);
    var _33 = det * (source._11 * source._22 - source._12 * source._21);

    result._11 = _11;
    result._12 = _21;
    result._13 = _31;
    result._21 = _12;
    result._22 = _22;
    result._23 = _32;
    result._31 = _13;
    result._32 = _23;
    result._33 = _33;
};

/**
 * Gets the X axis of a matrix transformation
 * @param {AxVector3} result The vector to hold the resulting axis
 * @param {AxMatrix} source The matrix which holds the whole transformation
 */
AxMatrix.GetAxisX = function(result, source)
{
    result.x = source._11;
    result.y = source._12;
    result.z = source._13;
};

/**
 * Sets the X axis of a matrix transformation
 * @param {AxMatrix} result The matrix to have its X axis set
 * @param {AxVector3} source The vector to be set as X axis in the given matrix
 */
AxMatrix.SetAxisX = function(result, source)
{
    result._11 = source.x;
    result._12 = source.y;
    result._13 = source.z;
};

/**
 * Gets the Y axis of a matrix transformation
 * @param {AxVector3} result The vector to hold the resulting axis
 * @param {AxMatrix} source The matrix which holds the whole transformation
 */
AxMatrix.GetAxisY = function(result, source)
{
    result.x = source._21;
    result.y = source._22;
    result.z = source._23;
};

/**
 * Sets the Y axis of a matrix transformation
 * @param {AxMatrix} result The matrix to have its Y axis set
 * @param {AxVector3} source The vector to be set as Y axis in the given matrix
 */
AxMatrix.SetAxisY = function(result, source)
{
    result._21 = source.x;
    result._22 = source.y;
    result._23 = source.z;
};

/**
 * Gets the Z axis of a matrix transformation
 * @param {AxVector3} result The vector to hold the resulting axis
 * @param {AxMatrix} source The matrix which holds the whole transformation
 */
AxMatrix.GetAxisZ = function(result, source)
{
    result.x = source._31;
    result.y = source._32;
    result.z = source._33;
};

/**
 * Sets the Z axis of a matrix transformation
 * @param {AxMatrix} result The matrix to have its Z axis set
 * @param {AxVector3} source The vector to be set as Z axis in the given matrix
 */
AxMatrix.SetAxisZ = function(result, source)
{
    result._31 = source.x;
    result._32 = source.y;
    result._33 = source.z;
};

/**
 * Gets the translation vector of a matrix transformation
 * @param {AxVector3} result The vector to hold the resulting translation
 * @param {AxMatrix} source The matrix which holds the whole transformation
 */
AxMatrix.GetTranslation = function(result, source)
{
    result.x = source._41;
    result.y = source._42;
    result.z = source._43;
};

/**
 * Sets the translation of a matrix transformation
 * @param {AxMatrix} result The matrix to have its translation set
 * @param {AxVector3} translation The vector to be set as translation in the given matrix
 */
AxMatrix.SetTranslation = function(result, translation)
{
    result._41 = translation.x;
    result._42 = translation.y;
    result._43 = translation.z;
};

/**
 * Extracts the scaling vector of a matrix transformation
 * @param {AxVector3} result The vector to hold the resulting scaling
 * @param {AxMatrix} source The matrix which holds the whole transformationx
 */
AxMatrix.GetScaling = function(result, source)
{
    result.x = AxMath.Sqrt(source._11 * source._11 + source._12 * source._12 + source._13 * source._13);
    result.y = AxMath.Sqrt(source._21 * source._21 + source._22 * source._22 + source._23 * source._23);
    result.z = AxMath.Sqrt(source._31 * source._31 + source._32 * source._32 + source._33 * source._33);
};

/**
 * Sets the scaling of a matrix transformation
 * @param {AxMatrix} result The matrix to have its scaling set
 * @param {AxVector3} scaling The vector to be set as scaling in the given matrix
 */
AxMatrix.SetScaling = function(result, scaling)
{
    var factor;

    factor = AxMath.Sqrt(result._11 * result._11 + result._12 * result._12 + result._13 * result._13);
    if (factor !== 0)
    {
        factor = scaling.x / factor;
        result._11 *= factor;
        result._12 *= factor;
        result._13 *= factor;
    }

    factor = AxMath.Sqrt(result._21 * result._21 + result._22 * result._22 + result._23 * result._23);
    if (factor !== 0)
    {
        factor = scaling.y / factor;
        result._21 *= factor;
        result._22 *= factor;
        result._23 *= factor;
    }

    factor = AxMath.Sqrt(result._31 * result._31 + result._32 * result._32 + result._33 * result._33);
    if (factor !== 0)
    {
        factor = scaling.z / factor;
        result._31 *= factor;
        result._32 *= factor;
        result._33 *= factor;
    }
};

/**
 * Extracts the Euler rotation angles in XYZ sequence of a matrix transformation
 * 
 * @param {AxVector3} result The vector to hold the resulting Euler rotation
 * @param {AxMatrix} source The matrix which holds the whole transformationx
 */
AxMatrix.ExtractEulerRotationXYZ = function(result, source)
{
    result.x = AxMath.ArcTan2(source._23, source._33);

    var cosY = AxMath.Sqrt(source._11 * source._11 + source._12 * source._12);
    result.y = AxMath.ArcTan2(-source._13, cosY);

    var sinX = AxMath.Sin(result.x);
    var cosX = AxMath.Cos(result.x);
    result.z = AxMath.ArcTan2(sinX * source._31 - cosX * source._21, cosX * source._22 - sinX * source._32);
};