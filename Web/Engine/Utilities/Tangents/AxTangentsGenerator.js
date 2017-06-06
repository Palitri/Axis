/**
 * Generates tangent vectors for a device mesh based on the orientation of the mesh's texture mapping base vectors, by averaging the tangents of a vertex's shared faces
 * @param {AxDeviceMesh} result Device mesh to generete tangents for. It needs to have its vertices, texture mapping and faces information
 * @constructor
 */
function AxTangentsGenerator(result)
{
    var v0 = new AxVector3();
    var v1 = new AxVector3();
    var v2 = new AxVector3();
    var t0 = new AxVector2();
    var t1 = new AxVector2();
    var t2 = new AxVector2();

    var tangent = new AxVector3();
    var biTangent = new AxVector3();

    var numVertices = result.GetVertexCount();
    var numIndices = result.GetIndexCount();
    var tangents = new Array(numVertices);
    var biTangents = new Array(numVertices);

    for (var i = 0; i < numVertices; i++)
    {
        tangents[i] = new AxVector3();
        tangents[i].Set_1(0.0);

        biTangents[i] = new AxVector3();
        biTangents[i].Set_1(0.0);
    }

    for (var i = 0; i < numIndices; i += 3)
    {
        var index0 = result.GetIndex(i);
        var index1 = result.GetIndex(i + 1);
        var index2 = result.GetIndex(i + 2);

        result.GetVertexPosition(index0, v0); 
        result.GetVertexPosition(index1, v1); 
        result.GetVertexPosition(index2, v2);

        result.GetVertexTexCoords(index0, t0); 
        result.GetVertexTexCoords(index1, t1); 
        result.GetVertexTexCoords(index2, t2); 
        
        AxMaths.GetTangents(tangent, biTangent, v0, v1, v2, t0, t1, t2);

        AxVector3.Add(tangents[index0], tangents[index0], tangent);
        AxVector3.Add(tangents[index1], tangents[index1], tangent);
        AxVector3.Add(tangents[index2], tangents[index2], tangent);

        AxVector3.Add(biTangents[index0], biTangents[index0], biTangent);
        AxVector3.Add(biTangents[index1], biTangents[index1], biTangent);
        AxVector3.Add(biTangents[index2], biTangents[index2], biTangent);
    }

    for (var i = 0; i < numVertices; i++)
    {
        AxVector3.Normalize(tangent, tangents[i]);
        AxVector3.Normalize(biTangent, biTangents[i]);
        result.SetVertexTangents(i, tangent, biTangent);
    }
}
