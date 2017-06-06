/**
 * Creates a new material shading layer
 * @param {AxShadingOperation} shadingOp The shading operation for the layer
 * @param {AxColorOperation} colorOp The color operation for the layer
 * @param {Integer} propertyFirst The index in the material shading layers collection, at which the first property of this shading layer can be found
 * @param {Integer} propertyCount The number of properties which this shading layer has in the material's shading layers collection
 * @constructor
 */
function AxMaterialShadingLayer(shadingOp, colorOp, propertyFirst, propertyCount)
{
    this.shadingOp = AxShadingOperation.None;
    this.colorOp = AxColorOperation.None;;
    this.propertyFirst = 0;
    this.propertyCount = 0;

    if (!AxUtils.IsUndefinedOrNull(shadingOp))
        this.shadingOp = shadingOp;
    if (!AxUtils.IsUndefinedOrNull(colorOp))
        this.colorOp = colorOp;
    if (!AxUtils.IsUndefinedOrNull(propertyFirst))
        this.propertyFirst = propertyFirst;
    if (!AxUtils.IsUndefinedOrNull(propertyCount))
        this.propertyCount = propertyCount;
}
