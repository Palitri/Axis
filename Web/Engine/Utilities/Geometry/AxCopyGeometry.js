/**
 * Copies the source mesh into the result. 
 * A transform parameter can be given to be applied to the resulting mesh. If omitted, the mesh is copied as is
 * Position, normal and texture coordinates are copied
 * Indices are copied
 * @param {AxDeviceMesh} result The mesh in which to copy
 * @param {AxDeviceMesh} source The mesh to be copied
 * @param {!AxMatrix} transform A transformation to be applied on the copied mesh. If omitted, no transformation is applied.
 */
function AxCopyGeometry(result, source, transform)
{
    if (AxUtils.IsUndefinedOrNull(transform))
    {
        transform = new AxMatrix();
	AxMatrix.CreateIdentity(transform);
    }
    
    AxCopyGeometry.CopyGeometry(result, source, transform);
}

/**
 * Copies the source mesh into the result. 
 * A transform parameter can be given to be applied to the resulting mesh. If omitted, the mesh is copied as is
 * Position, normal and texture coordinates are copied
 * Indices are copied
 * @param {AxDeviceMesh} result The mesh in which to copy
 * @param {AxDeviceMesh} source The mesh to be copied
 * @param {!AxMatrix} transform A transformation to be applied on the copied mesh. If omitted, no transformation is applied.
 */
AxCopyGeometry.CopyGeometry = function(result, source, transform)
{
    if ((result === source) && (transform.IsIdentity()))
        return;

    var numVertices = source.GetVertexCount();
    result.CreateVertexBuffer(numVertices);

    var position = new AxVector3();
    var normal = new AxVector3();
    var texCoords = new AxVector2();

    var normalsTransform = new AxMatrix();
    AxMatrix.Invert3x3(normalsTransform, transform);
    AxMatrix.Transpose(normalsTransform, normalsTransform);

    for (var i = 0; i < numVertices; i++)
    {
        source.GetVertexPosition(i, position);
        source.GetVertexNormal(i, normal);
        source.GetVertexTexCoords(i, texCoords);

        AxVector3.Transform(position, position, transform);
        AxVector3.TransformNormal(normal, normal, normalsTransform);

        result.SetVertexPosition(i, position);
        result.SetVertexNormal(i, normal);
        result.SetVertexTexCoords(i, texCoords);
    }


    var numIndices = source.GetIndexCount();
    result.CreateIndexBuffer(numIndices);

    for (var i = 0; i < numIndices; i++)
    {
        result.SetIndex(i, source.GetIndex(i));
    }
};
