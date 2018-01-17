/**
 * Creates an object instance of AxScreenPickEvents
 * @param {Axis} context The instance of axis, which calls the events
 * @constructor
 */
function AxScreenPickEvents(context) 
{
    AxTraceEvents.call(this, context);

    this.screenCoords = new AxVector3();

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
    this.screenCoords.Set(screenCoords, 0.0);
    this.intersectionInfo.hasIntersected = false;
    this.intersectionInfo.distance = AxMath.FloatMax;
};
	
AxScreenPickEvents.prototype.OnMesh = function(parameters)
{
    if ((parameters.cameraRef === null) || (parameters.transformRef === null))
        return;

    var transform = new AxMatrix();
    AxMatrix.Multiply(transform, parameters.transformRef.pivotedWorldMatrix, parameters.cameraRef.viewProjection);
    if (!parameters.meshRef.bounds.InsideViewFrustum(transform))
        return;

    var origin = new AxVector3();
    var direction = new AxVector3();
    AxMaths.ScreenSpaceToVolumetricRay(origin, direction, this.screenCoords, transform);

    var intersectionInfo = new AxIntersectionInfo();
    if (parameters.meshRef.Intersection(origin, direction, intersectionInfo))
    {
        var screenSpacePosition = new AxVector3();
        AxVector3.Transform(screenSpacePosition, intersectionInfo.point.position, transform);
        intersectionInfo.distance = AxVector3.Distance(this.screenCoords, screenSpacePosition);

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