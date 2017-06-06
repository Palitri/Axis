/**
 * Represents the bounding volume of a volumetric object
 * @constructor
 */
function AxBoundingVolume()
{
    this.center = new AxVector3();
    this.box = new AxVector3();
    
    this.sphereRadius = 0.0;
}

/**
 * Returns whether the bounding volume is inside the view frustum of the given camera
 * @param {AxMatrix} viewFrustum The world-view-projection transformation to check against
 * @return {Boolean} True if the bounding volume is in the given transormation's view frustum
 */
AxBoundingVolume.prototype.InsideViewFrustum = function(viewFrustum)
{
    return AxMaths.SphereViewFrustumIntersection(viewFrustum, this.center, this.sphereRadius);
};
