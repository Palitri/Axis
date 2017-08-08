/**
 * Creates planar mapped texture coordinates for a mesh
 * @param {AxDeviceMesh} result The mesh to map texture coordinates on to
 * @param {AxVector3} planeX The orientation and size of the mapping plane's X axis
 * @param {AxVector3} planeY The orientation and size of the mapping plane's Y axis
 * @constructor
 */
function AxPlanarTexCoords(result, planeX, planeY)
{
    var centerCorrection = new AxVector2(0.5, 0.5);

    var numVertices = result.GetVertexCount();
    for (var i = 0; i < numVertices; i++)
    {
        var vPos = new AxVector3();
        result.GetVertexPosition(i, vPos);

        var texCoords = new AxVector2();
        AxMaths.GetBarycentricCoordinates(texCoords, vPos, planeX, planeY);

        AxVector2.Add(texCoords, texCoords, centerCorrection);
        result.SetVertexTexCoords(i, texCoords);
    }
}
