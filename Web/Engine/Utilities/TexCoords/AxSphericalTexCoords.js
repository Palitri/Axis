/**
 * Creates spherical mapped texture coordinates for a mesh
 * @param {AxDeviceMesh} result The mesh to map texture coordinates on to
 */
function AxSphericalTexCoords(result)
{
    var numVertices = result.GetVertexCount();
    for (var i = 0; i < numVertices; i++)
    {
        var vPos = new AxVector3();
        var vSphere = new AxVector3();
        result.GetVertexPosition(i, vPos);

        AxVector3.CartesianToSpherical(vSphere, vPos);
        //AxVector3.SphereToCartesian(vSphere, vSphere.x, vSphere.y, 1.0f);

        result.SetVertexTexCoords(i, new AxVector2(vSphere.x / AxMath.Pi, (1.0 - vSphere.y / AxMath.PiHalf) / 2.0));
    }
}
