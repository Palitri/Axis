/**
 * Generates normals for a device mesh by averaging the orientation of a vertex's shared faces
 * @param {AxDeviceMesh} result Device mesh to generete normals for. It needs to have its vertices and faces information
 * @constructor
 */
function AxNormalsGenerator(result)
{
    var v0 = new AxVector3();
    var v1 = new AxVector3();
    var v2 = new AxVector3();

    var n = new AxVector3();

    var numVertices = result.GetVertexCount();
    var numIndices = result.GetIndexCount();
    var normals = new Array(numVertices);

    for (var i = 0; i < numVertices; i++)
    {
        normals[i] = new AxVector3();
        normals[i].Set_1(0.0);
    }

    for (var i = 0; i < numIndices; i += 3)
    {
        var index0 = result.GetIndex(i);
        var index1 = result.GetIndex(i + 1);
        var index2 = result.GetIndex(i + 2);

        result.GetVertexPosition(index0, v0); 
        result.GetVertexPosition(index1, v1); 
        result.GetVertexPosition(index2, v2);
        
        AxMaths.GetNormal(n, v0, v1, v2);

        AxVector3.Add(normals[index0], normals[index0], n);
        AxVector3.Add(normals[index1], normals[index1], n);
        AxVector3.Add(normals[index2], normals[index2], n);
    }

    for (var i = 0; i < numVertices; i++)
    {
        AxVector3.Normalize(v0, normals[i]);
        result.SetVertexNormal(i, v0);
    }
}
