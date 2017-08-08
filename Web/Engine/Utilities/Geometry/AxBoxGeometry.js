/**
 * Creates a box with the given number of segments
 * New mesh vertices are created with their position filled
 * New mesh indices are created
 * @param {AxDeviceMesh} result The mesh to hold the box
 * @param {Integer} xSegments The number of segments for the left and right sides of the box
 * @param {Integer} ySegments The number of segments for the top and bottom sides of the box
 * @param {Integer} zSegments The number of segments for the front and back sides of the box
 * @param {Boolean} texture3x2 Denotes whether to create a texture mapping for a single texture holding the six sides in a 3x2 pattern, where the top 3 parts of the texture from left to right represent respectively the left, front and right sides of the box, and the bottom 3 parts of the texture from left to right represent respectively the top, back and bottom  sides of the box. If false, each side of the box is texturedto fit a full single texture
 * @param {!AxVector3} xAxis The orientation of the box's X axis. If omitted, the arbitrary X axis is assumed.
 * @param {!AxVector3} yAxis The orientation of the box's Y axis. If omitted, the arbitrary Y axis is assumed.
 * @param {!AxVector3} zAxis The orientation of the box's Z axis. If omitted, the arbitrary Z axis is assumed.
 * @constructor
 */
function AxBoxGeometry(result, xSegments, ySegments, zSegments, texture3x2, xAxis, yAxis, zAxis)
{
    if (AxUtils.IsUndefinedOrNull(xAxis))
        xAxis = new AxVector3(1.0, 0.0, 0.0);
    if (AxUtils.IsUndefinedOrNull(yAxis))
        yAxis = new AxVector3(0.0, 1.0, 0.0);
    if (AxUtils.IsUndefinedOrNull(zAxis))
        zAxis = new AxVector3(0.0, 0.0, 1.0);
    
    this.CreateGeometry(result, xSegments, ySegments, zSegments, texture3x2, xAxis, yAxis, zAxis);
}

/**
 * Creates a box with the given number of segments
 * New mesh vertices are created with their position filled
 * New mesh indices are created
 * @param {AxDeviceMesh} result The mesh to hold the box
 * @param {Integer} xSegments The number of segments for the left and right sides of the box
 * @param {Integer} ySegments The number of segments for the top and bottom sides of the box
 * @param {Integer} zSegments The number of segments for the front and back sides of the box
 * @param {Boolean} texture3x2 Denotes whether to create a texture mapping for a single texture holding the six sides in a 3x2 pattern, where the top 3 parts of the texture from left to right represent respectively the left, front and right sides of the box, and the bottom 3 parts of the texture from left to right represent respectively the top, back and bottom  sides of the box. If false, each side of the box is texturedto fit a full single texture
 * @param {AxVector3} xAxis The orientation of the box's X axis
 * @param {AxVector3} yAxis The orientation of the box's Y axis
 * @param {AxVector3} zAxis The orientation of the box's Z axis
 */
AxBoxGeometry.prototype.CreateGeometry = function(result, xSegments, ySegments, zSegments, texture3x2, xAxis, yAxis, zAxis)
{
    var plane = new AxDeviceIndependentMesh();
    var transform = new AxMatrix();
    var textureScaling = new AxMatrix();
    var identity = new AxMatrix();
    var x = new AxVector3();
    var y = new AxVector3();
    var up = new AxVector3();

    var threshold = 0.01;
    var thresholdOffset = threshold / 2.0;

    AxMatrix.CreateIdentity(identity);
    if (texture3x2)
        AxMatrix.CreateScaling(textureScaling, 1.0 / 3.0, 1.0 / 2.0 - threshold, 1.0);

    // Left
    x = zAxis.Invert();
    y = yAxis;
    up = xAxis.Scale(-0.5);
    new AxPlaneGeometry(plane, zSegments, ySegments, x, y);
    if (texture3x2)
    {
        AxMatrix.CreateTranslation(transform, 0.0 / 3.0, 1.0 / 2.0 + thresholdOffset, 0.0);
        AxMatrix.Multiply(transform, textureScaling, transform);
        new AxTransformTexCoords(plane, transform);
    }
    AxMatrix.CreateTranslation(transform, up);
    new AxCopyGeometry(result, plane, transform);

    // Back
    x = xAxis;
    y = yAxis;
    up = zAxis.Scale(-0.5);
    new AxPlaneGeometry(plane, xSegments, ySegments, x, y);
    if (texture3x2)
    {
        AxMatrix.CreateTranslation(transform, 1.0 / 3.0, 1.0 / 2.0 + thresholdOffset, 0.0);
        AxMatrix.Multiply(transform, textureScaling, transform);
        new AxTransformTexCoords(plane, transform);
    }
    AxMatrix.CreateTranslation(transform, up);
    new AxMergeGeometry(result, result, plane, identity, transform);

    // Right
    x = zAxis;
    y = yAxis;
    up = xAxis.Scale(0.5);
    new AxPlaneGeometry(plane, zSegments, ySegments, x, y);
    if (texture3x2)
    {
        AxMatrix.CreateTranslation(transform, 2.0 / 3.0, 1.0 / 2.0 + thresholdOffset, 0.0);
        AxMatrix.Multiply(transform, textureScaling, transform);
        new AxTransformTexCoords(plane, transform);
    }
    AxMatrix.CreateTranslation(transform, up);
    new AxMergeGeometry(result, result, plane, identity, transform);


    //AxMatrix.CreateScaling(textureScaling, - 1.0 / 3.0, 1.0 / 2.0, 1.0);

    // Top
    x = zAxis;
    y = xAxis.Invert();
    up = yAxis.Scale(0.5);
    new AxPlaneGeometry(plane, xSegments, zSegments, x, y);
    if (texture3x2)
    {
        AxMatrix.CreateTranslation(transform, 0.0 / 3.0, 0.0 + thresholdOffset, 0.0);
        AxMatrix.Multiply(transform, textureScaling, transform);
        new AxTransformTexCoords(plane, transform);
    }
    AxMatrix.CreateTranslation(transform, up);
    new AxMergeGeometry(result, result, plane, identity, transform);

    // Front
    x = yAxis.Invert();
    y = xAxis.Invert();
    up = zAxis.Scale(0.5);
    new AxPlaneGeometry(plane, xSegments, ySegments, x, y);
    if (texture3x2)
    {
        AxMatrix.CreateTranslation(transform, 1.0 / 3.0, 0.0 + thresholdOffset, 0.0);
        AxMatrix.Multiply(transform, textureScaling, transform);
        new AxTransformTexCoords(plane, transform);
    }
    AxMatrix.CreateTranslation(transform, up);
    new AxMergeGeometry(result, result, plane, identity, transform);

    // Bottom
    x = zAxis.Invert();
    y = xAxis.Invert();
    up = yAxis.Scale(-0.5);
    new AxPlaneGeometry(plane, xSegments, zSegments, x, y);
    if (texture3x2)
    {
        AxMatrix.CreateTranslation(transform, 2.0 / 3.0, 0.0 + thresholdOffset, 0.0);
        AxMatrix.Multiply(transform, textureScaling, transform);
        new AxTransformTexCoords(plane, transform);
    }
    AxMatrix.CreateTranslation(transform, up);
    new AxMergeGeometry(result, result, plane, identity, transform);
};