/**
 * Merges two meshes into a new one, incorporating the given transformations to the source meshes
 * If the resulting mesh is also used as a source, its original geometry will be put into the final result
 * Position, normals and texture coordinates are copied
 * Indices are copied
 * @param {AxDeviceMesh} result The mesh to hold the merged geometry
 * @param {AxDeviceMesh} geometry1 The first mesh to merge
 * @param {AxDeviceMesh} geometry2 The second mesh to merge
 * @param {!AxMatrix} transform1 A transformation to apply to the first mesh in the merged geometry. If omitted, no transformation is applied
 * @param {!AxMatrix} transform2 A transformation to apply to the second mesh in the merged geometry. If omitted, no transformation is applied
 */
function AxMergeGeometry(result, geometry1, geometry2, transform1, transform2)
{
    if (AxUtils.IsUndefinedOrNull(transform1))
    {
        transform1 = new AxMatrix();
        AxMatrix.CreateIdentity(transform1)
    }
    if (AxUtils.IsUndefinedOrNull(transform2))
    {
        transform2 = new AxMatrix();
        AxMatrix.CreateIdentity(transform2)
    }
    
    this.CreateGeometry(result, geometry1, geometry2, transform1, transform2);
}

/**
 * Merges two meshes into a new one, incorporating the given transformations to the source meshes
 * If the resulting mesh is also used as a source, its original geometry will be put into the final result
 * Position, normals and texture coordinates are copied
 * Indices are copied
 * @param {AxDeviceMesh} result The mesh to hold the merged geometry
 * @param {AxDeviceMesh} geometry1 The first mesh to merge
 * @param {AxDeviceMesh} geometry2 The second mesh to merge
 * @param {AxMatrix} transform1 A transformation to apply to the first mesh in the merged geometry
 * @param {AxMatrix} transform2 A transformation to apply to the second mesh in the merged geometry
 */
AxMergeGeometry.prototype.CreateGeometry = function(result, geometry1, geometry2, transform1, transform2)
{
    var finalResult;

    var buffered = (result === geometry1) || (result === geometry2);
    if (buffered)
        finalResult = new AxDeviceIndependentMesh();
    else
        finalResult = result;

    finalResult.CreateVertexBuffer(geometry1.GetVertexCount() + geometry2.GetVertexCount());

    var position = new AxVector3();
    var normal = new AxVector3();
    var texCoords = new AxVector2();

    var finalVertices = finalResult.GetVertexCount();
    for (var i = 0; i < finalVertices; i++)
    {
        var i2 = i - geometry1.GetVertexCount();
        var source = i2 < 0 ? geometry1 : geometry2;
        var sourceIndex = i2 < 0 ? i : i2;
        var transform = i2 < 0 ? transform1 : transform2;

        source.GetVertexPosition(sourceIndex, position);
        source.GetVertexNormal(sourceIndex, normal);
        source.GetVertexTexCoords(sourceIndex, texCoords);

        AxVector3.Transform(position, position, transform);
        AxVector3.TransformNormal(normal, normal, transform);

        finalResult.SetVertexPosition(i, position);
        finalResult.SetVertexNormal(i, normal);
        finalResult.SetVertexTexCoords(i, texCoords);
    }


    finalResult.CreateIndexBuffer(geometry1.GetIndexCount() + geometry2.GetIndexCount());

    var finalIndices = finalResult.GetIndexCount();
    for (var i = 0; i < finalIndices; i++)
    {
        var i2 = i - geometry1.GetIndexCount();
        var source = i2 < 0 ? geometry1 : geometry2;
        var sourceIndex = i2 < 0 ? i : i2;
        var destIndex = i2 < 0 ? source.GetIndex(sourceIndex) : source.GetIndex(sourceIndex) + geometry1.GetVertexCount();

        finalResult.SetIndex(i, destIndex);
    }

    if (buffered)
    {
        AxCopyGeometry(result, finalResult);
    }
};