/**
 * Transforms the texture coordinates of a mesh
 * @param {AxDeviceMesh} result The mesh to have its texture coordinates transformed
 * @param {AxMatrix} transformation The transformation to apply to the texture coordinates of the mesh
 */
function AxTransformTexCoords(result, transformation)
{
    var vertexCount = result.GetVertexCount();
    for (var i = 0; i < vertexCount; i++)
    {
        var texCoords = new AxVector2();
        result.GetVertexTexCoords(i, texCoords);
        AxVector2.Transform(texCoords, texCoords, transformation);
        result.SetVertexTexCoords(i, texCoords);
    }
}
