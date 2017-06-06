/**
 * Creates an object instance of AxScreenPickEvents
 * @param {Axis} context The instance of axis, which calls the events
 * @constructor
 */
function AxScreenPickEvents(context) 
{
    AxTraceEvents.call(this, context);

    this.rayPoint1 = new AxVector3();
    this.rayPoint2 = new AxVector3();

    this.entityInfo = new AxTraceParameters();
    this.intersectionInfo = new AxIntersectionInfo();
}
	
AxScreenPickEvents.prototype = Object.create(AxTraceEvents.prototype);


AxScreenPickEvents.prototype.Dispose = function()
{
};


/**
 * Prepares the events for processing a scene and testing for intersecion with the given point on the screen.
 * The sceen coordinates are in screen unit space in the interval [-1, 1] from left to right and [-1, 1] from bottom to top
 * @param {AxVector2} screenCoords The unit coordinates on the screen, at which to perform the picking
 */
AxScreenPickEvents.prototype.SetupScreenPick = function(screenCoords)
{
    this.rayPoint1 = new AxVector3(screenCoords, -1.0);
    this.rayPoint2 = new AxVector3(screenCoords, 1.0);
    this.intersectionInfo.hasIntersected = false;
    this.intersectionInfo.distance = AxMath.FloatMax;
};
	
AxScreenPickEvents.prototype.OnMesh = function(parameters)
{
    var transform = new AxMatrix();
    var invertTransform = new AxMatrix();
    var origin4 = new AxVector4();
    var direction4 = new AxVector4();
    var origin = new AxVector3();
    var direction = new AxVector3();
    var intersectionInfo = new AxIntersectionInfo();

    if ((parameters.cameraRef === null) || (parameters.transformRef === null))
        return;

    AxMatrix.Multiply(transform, parameters.transformRef.pivotedWorldMatrix, parameters.cameraRef.viewProjection);

    if (!parameters.meshRef.bounds.InsideViewFrustum(transform))
        return;

    AxMatrix.Invert(invertTransform, transform);
    AxVector4.Transform(origin4, this.rayPoint1, invertTransform);
    AxVector4.Transform(direction4, this.rayPoint2, invertTransform);
    origin.x = origin4.x / origin4.w; 
    origin.y = origin4.y / origin4.w; 
    origin.z = origin4.z / origin4.w;
    direction.x = direction4.x / direction4.w;
    direction.y = direction4.y / direction4.w;
    direction.z = direction4.z / direction4.w;
    AxVector3.Subtract(direction, direction, origin);
    AxVector3.Normalize(direction, direction);
    if (parameters.meshRef.Intersection(origin, direction, intersectionInfo))
    {
        var cameraSpacePosition = new AxVector3();
        AxVector3.Transform(cameraSpacePosition, intersectionInfo.point.position, transform);
        intersectionInfo.distance = AxVector3.Distance(this.rayPoint1, cameraSpacePosition);

        if (intersectionInfo.distance < this.intersectionInfo.distance)
        {
            this.entityInfo = parameters;
            this.intersectionInfo = intersectionInfo;
        }
    }
};

AxScreenPickEvents.prototype.OnTransform = function(parameters)
{
    parameters.transformRef.Process(parameters.parentTransformRef);
};