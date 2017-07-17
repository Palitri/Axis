/**
 * Performs a height map modification to the given mesh, based on its normals and the given map
 * The given mesh must have vertices with their position and normal set
 * The position of the mesh's vertices is modified
 * @param {AxDeviceMesh} result The mesh in which to create the heightmap
 * @param {AxImage} map The image from which to sample the height values
 * @param {AxVector4} colorFilter The color filter to apply when sampling the height values.
 * @param {Number} heightScaling The height scaling factor
 */
function AxHeightMapGeometry(result, map, colorFilter, heightScaling)
{
    var numVertices = result.GetVertexCount();
    for (var i = 0; i < numVertices; i++)
    {
        var position = new AxVector3();
        var normal = new AxVector3();
        var texCoords = new AxVector2();

        result.GetVertexPosition(i, position);
        result.GetVertexNormal(i, normal);
        result.GetVertexTexCoords(i, texCoords);

        var rgba = new Float32Array(4);
        map.pixelFormat.GetRGBAUnitValues(rgba,  map.GetSample(texCoords.x, texCoords.y, 0));
        //float heightFactor = (rgba[3] * colorFilter.x + rgba[2] * colorFilter.y + rgba[1] * colorFilter.z + rgba[0] * colorFilter.w) / (colorFilter.x + colorFilter.y + colorFilter.z + colorFilter.w);
        var heightFactor = (rgba[0] * colorFilter.x + rgba[1] * colorFilter.y + rgba[2] * colorFilter.z + rgba[3] * colorFilter.w) / (colorFilter.x + colorFilter.y + colorFilter.z + colorFilter.w);

        AxVector3.Scale(normal, normal, heightScaling * heightFactor);
        AxVector3.Add(position, position, normal);

        result.SetVertexPosition(i, position);
    }
}
