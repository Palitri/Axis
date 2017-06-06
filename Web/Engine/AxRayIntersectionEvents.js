/**
 * Creates an object instance of AxRayIntersectionEvents
 * @param {Axis} context The instance of axis, which calls the events
 * @constructor
 */
function AxRayIntersectionEvents(context) 
{
    AxTraceEvents.call(this, context);

    this.rayPoint = new AxVector3();
    this.rayVector = new AxVector3();
    this.intersectionDistance = 0.0;

    this.traceResult = new AxTraceParameters();
    this.intersectionInfo = new AxIntersectionInfo();
    this.intersected = false;
}
	
AxRayIntersectionEvents.prototype = Object.create(AxTraceEvents.prototype);



AxRayIntersectionEvents.prototype.Dispose = function()
{
};


/**
 * Prepares the events for processing a scene and testing for intersecion with the given ray in the scene
 * @param {AxVector3} rayInitialPoint The initial (starting) point of the ray
 * @param {AxVector3} rayOrientation The orientation vector of the ray
 */
AxRayIntersectionEvents.prototype.SetupRay = function(rayInitialPoint, rayOrientation)
{
    AxVector3.Copy(this.rayPoint, rayInitialPoint);
    AxVector3.Normalize(this.rayVector, rayOrientation);
    this.intersectionDistance = AxMath.FloatMax;
    this.intersected = false;
};
    
AxRayIntersectionEvents.prototype.OnMesh = function(parameters)
{
    if (AxUtils.IsUndefinedOrNull(parameters.transformRef))
        return;

    var transform = new AxMatrix();
    var origin = new AxVector3();
    var direction = new AxVector3();
    var intersectionInfo = new AxIntersectionInfo();

    AxMatrix.Invert(transform, parameters.transformRef.pivotedWorldMatrix);
    AxVector3.Transform(origin, this.rayPoint, transform);
    AxVector3.TransformNormal(direction, this.rayVector, transform);
    if (parameters.meshRef.Intersection(origin, direction, intersectionInfo))
    {
        AxVector3.Transform(intersectionInfo.point.position, intersectionInfo.point.position, parameters.transformRef.pivotedWorldMatrix);
        intersectionInfo.distance = AxVector3.Distance(this.rayPoint, intersectionInfo.point.position);
        
        if (intersectionInfo.distance < this.intersectionDistance)
        {
            this.traceResult = parameters;
            this.intersectionInfo = intersectionInfo;
            this.intersectionDistance = intersectionInfo.distance;
            this.intersected = true;
        }
    }
};

AxRayIntersectionEvents.prototype.OnTransform = function(parameters)
{
    parameters.transformRef.Process(parameters.parentTransformRef);
};