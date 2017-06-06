/**
 * Creates a new transform layer
 * Transform layers are used to define transformation operations on a transform
 * @param {!AxTransformOperation} operation The transform layer's operation
 * @param {!Integer} propertyFirst Index of the first property in the transform, which this operation owns
 * @param {!Integer} propertyCount Number of the properteis in the transform, which this operation owns
 * @constructor
 */
function AxTransformLayer(operation, propertyFirst, propertyCount)
{
    this.operation = AxUtils.IsUndefined(operation) ? AxTransformOperation.Translate : operation;
    this.propertyFirst = AxUtils.IsUndefined(propertyFirst) ? 0 : propertyFirst;
    this.propertyCount = AxUtils.IsUndefined(propertyCount) ? 0 : propertyCount;
}


/**
 * @param {AxTransformOperation} operation The transform layer's operation
 * @param {Integer} propertyFirst Index of the first property in the transform, which this operation owns
 * @param {Integer} propertyCount Number of the properteis in the transform, which this operation owns
 */
AxTransformLayer.prototype.Set = function(operation, propertyFirst, propertyCount)
{
    this.operation = operation;
    this.propertyFirst = propertyFirst;
    this.propertyCount = propertyCount;
};