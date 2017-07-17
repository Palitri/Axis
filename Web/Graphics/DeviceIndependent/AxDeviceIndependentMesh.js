/**
 * Creates a new graphics device mesh.
 * This constructor shouldn't be called explicity, but instead the graphics device should create its respective mesh
 * @constructor
 */
function AxDeviceIndependentMesh()
{
    AxDeviceMesh.call(this);
    
    this.verticesCount = 0;
    
    this.vertexPositionElementsCount = 3;
    this.verticesPositions = null;
    
    this.vertexNormalElementsCount = 3;
    this.verticesNormals = null;
    
    this.vertexTexCoordsElementsCount = 2;
    this.verticesTexCoords = null;

    this.vertexBoneIndicesElementsCount = 4;
    this.verticesBoneIndices = null;
    
    this.vertexBoneWeightsElementsCount = 4;
    this.verticesBoneWeights = null;
    
    this.vertexTangentElementsCount = 3;
    this.verticesTangents = null;

    this.vertexBitangentElementsCount = 3;
    this.verticesBitangents = null;
    
    this.indicesCount = 0;

    this.indices = 0;
}

AxDeviceIndependentMesh.prototype = Object.create(AxDeviceMesh.prototype);


/**
 * Creates a vertex buffer for the mesh
 * @param {Integer} numVertices The number of vertices for the vertex buffer to hold
 * @return {Boolean} True if the vertex buffer was created successfully
 */
AxDeviceIndependentMesh.prototype.CreateVertexBuffer = function(numVertices) 
{ 
    this.verticesCount = numVertices;
    
    this.verticesPositions = new Float32Array(this.verticesCount * this.vertexPositionElementsCount);
    this.verticesNormals = new Float32Array(this.verticesCount * this.vertexNormalElementsCount);
    this.verticesTexCoords = new Float32Array(this.verticesCount * this.vertexTexCoordsElementsCount);
    this.verticesBoneIndices = new Float32Array(this.verticesCount * this.vertexBoneIndicesElementsCount);
    this.verticesBoneWeights = new Float32Array(this.verticesCount * this.vertexBoneWeightsElementsCount);
    this.verticesTangents = new Float32Array(this.verticesCount * this.vertexTangentElementsCount);
    this.verticesBitangents = new Float32Array(this.verticesCount * this.vertexBitangentElementsCount);
};
/**
 * Creates an index buffer for the mesh
 * @param {Integer} numIndices The number of indices for the index buffer to hold
 * @return {Boolean} True if the index buffer was created successfully
 */
AxDeviceIndependentMesh.prototype.CreateIndexBuffer = function(numIndices)
{
    this.indicesCount = numIndices;
    
    this.indices = new Uint16Array(this.indicesCount);
};



/**
 * Deletes the vertex buffer, freeing the resources it allocates
 * @return {Boolean} True if the vertex buffer was deleted successfully
 */
AxDeviceIndependentMesh.prototype.DeleteVertexBuffer = function() { };
/**
 * Deletes the index buffer, freeing the resources it allocates
 * @return {Boolean} True if the index buffer was deleted successfully
 */
AxDeviceIndependentMesh.prototype.DeleteIndexBuffer = function() { };



/**
 * Updates the vertices of the graphics device.
 * This method needs to be called when the mesh's vertices have been changed and the graphics device has to be updated with the changes
 * @param {Integer} offset The number of the vertex to start the update from
 * @param {Integer} count The number of vertices to update
 * @returns {Boolean} True if the vertices were successfully updated on the graphics device
 */
AxDeviceIndependentMesh.prototype.UpdateVertices = function(offset, count) { return true; };

/**
 * Updates the indices of the graphics device.
 * This method needs to be called when the mesh's indices have been changed and the graphics device has to be updated with the changes
 * @param {Integer} offset The number of the index to start the update from
 * @param {Integer} count The number of indices to update
 * @returns {Boolean} True if the indices were successfully updated on the graphics device
 */
AxDeviceIndependentMesh.prototype.UpdateIndices = function(offset, count) { return true; };



/**
 * Gets the number of vertices in the mesh
 * @returns {Integer} The number of vertices in the mesh
 */
AxDeviceIndependentMesh.prototype.GetVertexCount = function() 
{
    return this.verticesCount;
};
/**
 * Gets the number of indices in the mesh
 * @returns {Integer} The number of indices in the mesh
 */
AxDeviceIndependentMesh.prototype.GetIndexCount = function() 
{ 
    return this.indicesCount;
};
/**
 * Gets the number of primitives in the mesh. 
 * For example, for triangle primitives, that would be the number of indices / 3
 * @returns {Integer} The number of primitives in the mesh
 */
AxDeviceIndependentMesh.prototype.GetPrimitiveCount = function() 
{ 
    return this.indicesCount / 3;
};



/**
 * Gets the position of a given vertex into a vector
 * @param {Integer} index The index of the vertex to copy
 * @param {AxVector3} result The vector to copy the position to
 */
AxDeviceIndependentMesh.prototype.GetVertexPosition = function(index, result) 
{
    var i = index * this.vertexPositionElementsCount;
    result.x = this.verticesPositions[i++];
    result.y = this.verticesPositions[i++];
    result.z = this.verticesPositions[i];
};
/**
 * Sets the position of a given vertex
 * @param {Integer} index The index of the vertex to set
 * @param {AxVector3} value The vector with the new position value to set
 */
AxDeviceIndependentMesh.prototype.SetVertexPosition = function(index, value) 
{ 
    var i = index * this.vertexPositionElementsCount;
    this.verticesPositions[i++] = value.x;
    this.verticesPositions[i++] = value.y;
    this.verticesPositions[i] = value.z;
};



/**
 * Gets the normal of a given vertex into a vector
 * @param {Integer} index The index of the vertex to copy
 * @param {AxVector3} result The vector to copy the normal to
 */
AxDeviceIndependentMesh.prototype.GetVertexNormal = function(index, result) 
{
    var i = index * this.vertexNormalElementsCount;
    result.x = this.verticesNormals[i++];
    result.y = this.verticesNormals[i++];
    result.z = this.verticesNormals[i];
};
/**
 * Sets the normal of a given vertex
 * @param {Integer} index The index of the vertex to set
 * @param {AxVector3} value The vector with the new normal value to set
 */
AxDeviceIndependentMesh.prototype.SetVertexNormal = function(index, value) 
{
    var i = index * this.vertexNormalElementsCount;
    this.verticesNormals[i++] = value.x;
    this.verticesNormals[i++] = value.y;
    this.verticesNormals[i] = value.z;
};



/**
 * Gets the texture coordinates of a given vertex into a vector
 * @param {Integer} index The index of the vertex to copy
 * @param {AxVector2} result The vector to copy the texture coordinates to
 */
AxDeviceIndependentMesh.prototype.GetVertexTexCoords = function(index, result) 
{ 
    var i = index * this.vertexTexCoordsElementsCount;
    result.x = this.verticesTexCoords[i++];
    result.y = this.verticesTexCoords[i];
};
/**
 * Sets the texture coordinates of a given vertex
 * @param {Integer} index The index of the vertex to set
 * @param {AxVector2} value The vector with the new texture coordinates to set
 */
AxDeviceIndependentMesh.prototype.SetVertexTexCoords = function(index, value) 
{
    var i = index * this.vertexTexCoordsElementsCount;
    this.verticesTexCoords[i++] = value.x;
    this.verticesTexCoords[i] = value.y;
};



/**
 * Gets the bone data of a given vertex
 * @param {Integer} index The index of the vertex to get bone data from
 * @param {AxVector4} boneIndices The vector in which to copy the indices of the bones affecting the given vertex
 * @param {AxVector4} boneWeights The vector in which to copy the weights of the bones affecting the given vertex
 */
AxDeviceIndependentMesh.prototype.GetVertexBones = function(index, boneIndices, boneWeights)
{ 
    var i = index * this.vertexBoneIndicesElementsCount;
    boneIndices.x = this.verticesBoneIndices[i];
    boneWeights.x = this.verticesBoneWeights[i++];

    boneIndices.y = this.verticesBoneIndices[i];
    boneWeights.y = this.verticesBoneWeights[i++];
    
    boneIndices.z = this.verticesBoneIndices[i];
    boneWeights.z = this.verticesBoneWeights[i++];

    boneIndices.w = this.verticesBoneIndices[i];
    boneWeights.w = this.verticesBoneWeights[i];
};
/**
 * Sets the bone data of a given vertex
 * @param {Integer} index The index of the vertex to set bone data to
 * @param {AxVector4} boneIndices The vector which holds the new indices of the bones to affect the given vertex
 * @param {AxVector4} boneWeights The vector which holds the new weights of the bones to affect the given vertex
 */
AxDeviceIndependentMesh.prototype.SetVertexBones = function(index, boneIndices, boneWeights) 
{ 
    var i = index * this.vertexBoneIndicesElementsCount;
    this.verticesBoneIndices[i] = boneIndices.x;
    this.verticesBoneWeights[i++] = boneWeights.x;

    this.verticesBoneIndices[i] = boneIndices.y;
    this.verticesBoneWeights[i++] = boneWeights.y;
    
    this.verticesBoneIndices[i] = boneIndices.z;
    this.verticesBoneWeights[i++] = boneWeights.z;

    this.verticesBoneIndices[i] = boneIndices.w;
    this.verticesBoneWeights[i] = boneWeights.w;
};



/**
 * Gets the tangent vectors of a given vertex. The tangent vectors represent the mapped texture's base axes in model space, tangent to the model's surface
 * @param {Integer} index The index of the vertex for which to get tangent vector
 * @param {AxVector3} tangent The vector in which to copy the tangent vector of the vertex. Corresponds to the texture's X axis, which is the absciss
 * @param {AxVector3} biTangent The vector in which to copy the biTangent vector of the vertex. Corresponds to the texture's Y axis, which is the ordinate
 */
AxDeviceIndependentMesh.prototype.GetVertexTangents = function(index, tangent, biTangent)
{
    var i = index * this.vertexTangentElementsCount;
    tangent.x = this.verticesTangents[i];
    biTangent.x = this.verticesBitangents[i++];

    tangent.y = this.verticesTangents[i];
    biTangent.y = this.verticesBitangents[i++];

    tangent.z = this.verticesTangents[i];
    biTangent.z = this.verticesBitangents[i];
};
/**
 * Sets the tangent vectors of a given vertex. The tangent vectors represent the mapped texture's base axes in model space, tangent to the model's surface
 * @param {Integer} index The index of the vertex for which to set tangent vector
 * @param {AxVector3} tangent The value of the tangent vector to set. Corresponds to the texture's X axis, which is the absciss
 * @param {AxVector3} biTangent The value of the bitangent vector to set. Corresponds to the texture's Y axis, which is the ordinate
 */
AxDeviceIndependentMesh.prototype.SetVertexTangents = function(index, tangent, biTangent)
{
    var i = index * this.vertexTangentElementsCount;
    this.verticesTangents[i] = tangent.x;
    this.verticesBitangents[i++] = biTangent.x;

    this.verticesTangents[i] = tangent.y;
    this.verticesBitangents[i++] = biTangent.y;

    this.verticesTangents[i] = tangent.z;
    this.verticesBitangents[i] = biTangent.z;
};



/**
 * Gets the value of an index in the mesh. 
 * @param {Integer} index The index of the vertex to set bone data to
 * @returns {Integer} The value of an index in the mesh. 
 */
AxDeviceIndependentMesh.prototype.GetIndex = function(index) 
{ 
    return this.indices[index];
};
/**
 * Sets the value of an index in the mesh. 
 * @param {Integer} index The index of the vertex to set bone data to
 * @param {Integer} value The new value for the index
 */
AxDeviceIndependentMesh.prototype.SetIndex = function(index, value) 
{
    this.indices[index] = value;
};