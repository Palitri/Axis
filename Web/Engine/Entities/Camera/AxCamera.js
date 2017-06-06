/**
 * Creates a new camera
 * @param {Axis} context The context to use for the camera
 * @constructor
 */
function AxCamera(context)
{
    AxResource.call(this);

    this.context = context;

    this.typeId = AxCamera.classId;
    this.resourceType = AxResourceType.Camera;
    this.view = new AxMatrix();
    this.projection = new AxMatrix();
    this.viewProjection = new AxMatrix();
    
    this.position = new AxVector3();
    this.lookAt = new AxVector3();
    this.xAxis = new AxVector3();
    this.yAxis = new AxVector3();
    this.zAxis = new AxVector3();
    
        
    this.properties.Add(new AxProperty(new AxString('Aspect'), 1.0));
    this.properties.Add(new AxProperty(new AxString('FOV'), AxMath.Pi / 4.0));
    this.properties.Add(new AxProperty(new AxString('Ortho Zoom'), 1.0));
    this.properties.Add(new AxProperty(new AxString('Orthographic'), false));
};

AxCamera.prototype = Object.create(AxResource.prototype);

AxCamera.classId = (AxResourceType.Camera << 16) | 0;

AxCamera.propertyIndex_Aspect       = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxCamera.propertyIndex_FOV          = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxCamera.propertyIndex_OrthoZoom    = AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxCamera.propertyIndex_Orthographic = AxResource.propertyIndex_ChildPropertiesIndex + 3;

AxCamera.FOVMode_Vertical = 0;
AxCamera.FOVMode_Horizontal = 1;
AxCamera.FOVMode_LesserScreenDimension = 2;
AxCamera.FOVMode_GreaterScreenDimension = 3;
        
AxCamera.prototype.Dispose = function()
{
};

/**
 * Builds the camera transformation matrix, based on a parent transform
 * @param {AxMatrix} transform The parent transform on which to build the camera's own transform.
 */
AxCamera.prototype.BuildMatrix = function(transform)
{
    AxMatrix.GetAxisX(this.xAxis, transform);
    AxMatrix.GetAxisY(this.yAxis, transform);
    AxMatrix.GetAxisZ(this.zAxis, transform);
    AxMatrix.GetTranslation(this.position, transform);
    AxVector3.Add(this.lookAt, this.position, this.zAxis);

    var aspect = this.properties.Get(AxCamera.propertyIndex_Aspect).GetEffectiveValue();
    var orthographic = this.properties.Get(AxCamera.propertyIndex_Orthographic).GetEffectiveValue();

    AxMatrix.CreateLookAt(this.view, this.position, this.lookAt, this.yAxis);
    var screenAspect = this.context.viewportWidth / this.context.viewportHeight;
    var isWidthGreater = this.context.viewportWidth > this.context.viewportHeight;
    var horizontalFOV = 
            (this.context.cameraFOVMode === AxCamera.FOVMode_Horizontal) || 
            ((this.context.cameraFOVMode === AxCamera.FOVMode_GreaterScreenDimension) && (isWidthGreater)) ||
            ((this.context.cameraFOVMode === AxCamera.FOVMode_LesserScreenDimension) && (!isWidthGreater));
    if (orthographic)
    {
        var orthoZoom = this.properties.Get(AxCamera.propertyIndex_OrthoZoom).GetEffectiveValue();
        if (horizontalFOV)
        {
            AxMatrix.CreateOrthographic(this.projection, orthoZoom, orthoZoom * aspect / screenAspect, 1.0, 10000.0);
        }
        else
        {
            AxMatrix.CreateOrthographic(this.projection, orthoZoom * screenAspect / aspect, orthoZoom, 1.0, 10000.0);
        }
    }
    else
    {
        var fov = this.properties.Get(AxCamera.propertyIndex_FOV).GetEffectiveValue();
        if (horizontalFOV)
        {
            AxMatrix.CreatePerspectiveFieldOfView(this.projection, fov, 1.0, aspect / screenAspect, 1.0, 10000.0);
        }
        else
        {
            AxMatrix.CreatePerspectiveFieldOfView(this.projection, fov, screenAspect / aspect, 1.0, 1.0, 10000.0);
        }
    }
        
    AxMatrix.Multiply(this.viewProjection, this.view, this.projection);
};


/**
 * Returns a vector which projects from given coordinates on the camera screen into scene space.
 * The returned vector is normalized
 * Camera must have its matrices built in order for this method to work. This should normally not be an issue, since matrices of the camera are built by the BuildMatrix method, which is routinely called. If needed, call BuildMatrix() explicitly before calling this method.
 * @param {AxVecto2} screenCoords The coordinates of the point on the screen, from which to project the ray. The coordinates must be in the range [-1, 1] from left to right and from bottom to top of the screen
 * @returns {AxVector3} A normalized vector, oriented along a point on the camera screen
 */
AxCamera.prototype.CastVector = function(screenCoords)
{
    var inverseVP = new AxMatrix();
    AxMatrix.Invert(inverseVP, this.viewProjection);

    var result4 = new AxVector4();
    AxVector4.Transform(result4, new AxVector3(screenCoords, 1.0), inverseVP);

    var result = new AxVector3();
    result.x = result4.x / result4.w; 
    result.y = result4.y / result4.w; 
    result.z = result4.z / result4.w;
    AxVector3.Subtract(result, result, this.position);
    AxVector3.Normalize(result, result);

    return result;
};

/**
 * Returns a vector which is projected onto the camera screen from given point in scene space
 * Camera must have its matrices built in order for this method to work. This should normally not be an issue, since matrices of the camera are built by the BuildMatrix method, which is routinely called. If needed, call BuildMatrix() explicitly before calling this method.
 * @param {AxVector3} vector The coordinates of a vector in scene space
 * @returns {AxVector3} The screen space coordinates of the vector, projected onto the camera screen. Coordinates are between the range [-1, 1] from left to right, bottom to top and back to forward
 */
AxCamera.prototype.ProjectVector = function(vector)
{
    var result4 = new AxVector4();
    AxVector4.Transform(result4, vector, this.viewProjection);

    var result = new AxVector3();
    result.x = result4.x / result4.w;
    result.y = result4.y / result4.w;
    result.z = result4.z / result4.w;

    return result;
};