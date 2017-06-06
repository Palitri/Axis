/**
 * The class assumes a mesh built of polygon faces, where each face is formed by indices to a vertex array.
 * Each face may belong to a number of smoothing groups, which are denoted by the bits of a value.
 * The input data is loaded through the SetFaceSmoothingGroup() and AddFaceVertex() methods, by which the original geometry of a mesh is input.
 * While using the SetFaceSmoothingGroup() and AddFaceVertex() methods, a set of vertex indices and faces are being created, in which faces with no matching smoothing groups no longer share the same vertices. This is achieved by creating a new vertex whenever an original vertex is shared between faces whose smoothing groups do not match.
 * The vertices of the newly build mesh can be accessed by the vertexIndices member variable, where each element represents a vertex and the value of each element is an index to the originally input vertex
 * The faces of the newly build mesh can be accessed by the faces member variable, where each element represents a face with vertices represented by indices that point to the vertexIndices member variable. For example, the index to the original vertex of the n-th new face's m-th vertex would be vertexIndices[faces[n].indices[m]]
 * The algorithm simply accumulates for each vertex all the smoothing groups of each face the vertex has been used by, and when a face's smoothing groups do not match any of the accumulated smoothing groups of a vertex it uses, then a new vertex is created and the face is fed with the index of that new vertex.
 * The new mesh data may have more vertices than the original and will always have the same number of faces.
 * @param {Integer} originalFacesCount Number of faces in the original mesh
 * @param {Integer} originalVertexCount Number of vertices in the original mesh
 * @constructor
 */
function AxSmoothingGroups(originalFacesCount, originalVertexCount)
{
    // AxSmoothingGroupsVertexInfo
    this.originalVerticesInfo = new AxList();

    // AxSmoothingGroupsVertex
    this.vertexIndices = new AxList();
    // AxSmoothingGroupsFace
    this.faces = new AxList();

    this.originalVerticesInfo.SetSize(originalVertexCount);
    for (var i = 0; i < this.originalVerticesInfo.count; i++)
    {
        this.originalVerticesInfo.Set(i, new AxSmoothingGroupsVertexInfo());
        this.originalVerticesInfo.Get(i).accumulatedSmoothingGroups = 0;
        this.originalVerticesInfo.Get(i).indices.EnsureCapacity(4);
    }

    this.vertexIndices.EnsureCapacity(originalVertexCount);

    this.faces.SetSize(originalFacesCount);
    for (var i = 0; i < this.faces.count; i++)
    {
        this.faces.Set(i, new AxSmoothingGroupsFace());
        this.faces.Get(i).smoothingGroups = 0;
    }
}


/**
 * Sets a face's smoothing groups
 * @param {Integer} faceIndex Index of the face to set the smoothing groups to
 * @param {Integer} smoothingGroupFlags Its bits denote which smoothing groups the face belongs to. Being a 64-bit value means that there are a maximum 64 different smoothing groups possible
 */
AxSmoothingGroups.prototype.SetFaceSmoothingGroups = function(faceIndex, smoothingGroupFlags)
{
    this.faces.Get(faceIndex).smoothingGroups = smoothingGroupFlags;
};

/**
 * Adds a vertex to a face.
 * @param {Integer} faceIndex Index of face to add the vertex to
 * @param {Integer} originalVertexIndex Index of the vertex in the original geometry
 * @return {Integer} The index of the added vertex in the vertexIndices member variable
 */
AxSmoothingGroups.prototype.AddFaceVertex = function(faceIndex, originalVertexIndex)
{
    var face = this.faces.Get(faceIndex);

    var vertex = new AxSmoothingGroupsVertex();
    vertex.accumulatedSmoothingGroups = face.smoothingGroups;

    if ((vertex.accumulatedSmoothingGroups & this.originalVerticesInfo.Get(originalVertexIndex).accumulatedSmoothingGroups) === 0)
    {
        var newVertexIndex = this.vertexIndices.count;
        vertex.index = originalVertexIndex;
        face.indices.Add(newVertexIndex);
        this.vertexIndices.Add(vertex);
        this.originalVerticesInfo.Get(originalVertexIndex).indices.Add(newVertexIndex);
    }
    else
    {
        var indicesCount = this.originalVerticesInfo.Get(originalVertexIndex).indices.count;
        for (var i = 0; i < indicesCount; i++)
        {
            vertex.index = this.originalVerticesInfo.Get(originalVertexIndex).indices.Get(i);
            if ((vertex.accumulatedSmoothingGroups & this.vertexIndices.Get(vertex.index).accumulatedSmoothingGroups) !== 0)
            {
                this.vertexIndices.Get(vertex.index).accumulatedSmoothingGroups |= vertex.accumulatedSmoothingGroups;
                face.indices.Add(vertex.index);
                break;
            }
        }
    }

    this.originalVerticesInfo.Get(originalVertexIndex).accumulatedSmoothingGroups |= vertex.accumulatedSmoothingGroups;

    return vertex.index;
};