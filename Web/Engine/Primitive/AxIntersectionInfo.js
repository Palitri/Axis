/**
 * Represents information on intersection
 * Used when testing for intersection
 * @constructor
 */
function AxIntersectionInfo()
{
    this.point = new AxVertex();
    this.distance = 0.0;
    this.primitiveIndex = 0;
    this.hasIntersected = false;
}