/**
 * Creates a new graphics device mesh.
 * This constructor shouldn't be called explicity, but instead the graphics device should create its respective mesh
 * @constructor
 */
function AxDeviceMesh()
{
}

/**
 * Copies the source graphics device mesh
 * @param {AxGraphicsDeviceMesh} source The graphics device mesh to copy from
 */
AxDeviceMesh.prototype.CopyFrom = function(source)
{
    if (source === this)
        return;

    var numIndices = source.GetIndexCount();
    this.CreateIndexBuffer(numIndices);
    for (var i = 0; i < numIndices; i++)
    {
        this.SetIndex(i, source.GetIndex(i));
    }
    this.UpdateIndices(0, numIndices);

    var numVertices = source.GetVertexCount();
    this.CreateVertexBuffer(numVertices);
    for (var i = 0; i < numVertices; i++)
    {
            var v3 = new AxVector3();
            var v2 = new AxVector2();

            source.GetVertexPosition(i, v3);
            this.GetVertexPosition(i, v3);

            source.GetVertexNormal(i, v3);
            this.GetVertexNormal(i, v3);

            source.GetVertexTexCoords(i, v2);
            this.GetVertexTexCoords(i, v2);
    }
    this.UpdateVertices(0, numVertices);
};



// Virtual methods



/**
 * Creates a vertex buffer for the mesh
 * @param {Integer} numVertices The number of vertices for the vertex buffer to hold
 * @return {Boolean} True if the vertex buffer was created successfully
 */
AxDeviceMesh.prototype.CreateVertexBuffer = function(numVertices) { };
/**
 * Creates an index buffer for the mesh
 * @param {Integer} numIndices The number of indices for the index buffer to hold
 * @return {Boolean} True if the index buffer was created successfully
 */
AxDeviceMesh.prototype.CreateIndexBuffer = function(numIndices) { };



/**
 * Deletes the vertex buffer, freeing the resources it allocates
 * @return {Boolean} True if the vertex buffer was deleted successfully
 */
AxDeviceMesh.prototype.DeleteVertexBuffer = function() { };
/**
 * Deletes the index buffer, freeing the resources it allocates
 * @return {Boolean} True if the index buffer was deleted successfully
 */
AxDeviceMesh.prototype.DeleteIndexBuffer = function() { };



/**
 * Updates the vertices of the graphics device.
 * This method needs to be called when the mesh's vertices have been changed and the graphics device has to be updated with the changes
 * @param {Integer} offset The number of the vertex to start the update from
 * @param {Integer} count The number of vertices to update
 * @returns {Boolean} True if the vertices were successfully updated on the graphics device
 */
AxDeviceMesh.prototype.UpdateVertices = function(offset, count) { };
/**
 * Updates the indices of the graphics device.
 * This method needs to be called when the mesh's indices have been changed and the graphics device has to be updated with the changes
 * @param {Integer} offset The number of the index to start the update from
 * @param {Integer} count The number of indices to update
 * @returns {Boolean} True if the indices were successfully updated on the graphics device
 */
AxDeviceMesh.prototype.UpdateIndices = function(offset, count) { };



/**
 * Gets the number of vertices in the mesh
 * @returns {Integer} The number of vertices in the mesh
 */
AxDeviceMesh.prototype.GetVertexCount = function() { };
/**
 * Gets the number of indices in the mesh
 * @returns {Integer} The number of indices in the mesh
 */
AxDeviceMesh.prototype.GetIndexCount = function() { };
/**
 * Gets the number of primitives in the mesh. 
 * For example, for triangle primitives, that would be the number of indices / 3
 * @returns {Integer} The number of primitives in the mesh
 */
AxDeviceMesh.prototype.GetPrimitiveCount = function() { };



/**
 * Gets the position of a given vertex into a vector
 * @param {Integer} index The index of the vertex to copy
 * @param {AxVector3} result The vector to copy the position to
 */
AxDeviceMesh.prototype.GetVertexPosition = function(index, result) { };
/**
 * Sets the position of a given vertex
 * @param {Integer} index The index of the vertex to set
 * @param {AxVector3} value The vector with the new position value to set
 */
AxDeviceMesh.prototype.SetVertexPosition = function(index, value) { };



/**
 * Gets the normal of a given vertex into a vector
 * @param {Integer} index The index of the vertex to copy
 * @param {AxVector3} result The vector to copy the normal to
 */
AxDeviceMesh.prototype.GetVertexNormal = function(index, result) { };
/**
 * Sets the normal of a given vertex
 * @param {Integer} index The index of the vertex to set
 * @param {AxVector3} value The vector with the new normal value to set
 */
AxDeviceMesh.prototype.SetVertexNormal = function(index, value) { };



/**
 * Gets the texture coordinates of a given vertex into a vector
 * @param {Integer} index The index of the vertex to copy
 * @param {AxVector2} result The vector to copy the texture coordinates to
 */
AxDeviceMesh.prototype.GetVertexTexCoords = function(index, result) { };
/**
 * Sets the texture coordinates of a given vertex
 * @param {Integer} index The index of the vertex to set
 * @param {AxVector2} value The vector with the new texture coordinates to set
 */
AxDeviceMesh.prototype.SetVertexTexCoords = function(index, value) { };



/**
 * Gets the bone data of a given vertex
 * @param {Integer} index The index of the vertex to get bone data from
 * @param {AxVector4} boneIndices The vector in which to copy the indices of the bones affecting the given vertex
 * @param {AxVector4} boneWeights The vector in which to copy the weights of the bones affecting the given vertex
 */
AxDeviceMesh.prototype.GetVertexBones = function(index, boneIndices, boneWeights) { };
/**
 * Sets the bone data of a given vertex
 * @param {Integer} index The index of the vertex to set bone data to
 * @param {AxVector4} boneIndices The vector which holds the new indices of the bones to affect the given vertex
 * @param {AxVector4} boneWeights The vector which holds the new weights of the bones to affect the given vertex
 */
AxDeviceMesh.prototype.SetVertexBones = function(index, boneIndices, boneWeights) { };



/**
 * Gets the tangent vectors of a given vertex. The tangent vectors represent the mapped texture's base axes in model space, tangent to the model's surface
 * @param {Integer} index The index of the vertex for which to get tangent vector
 * @param {AxVector3} tangent The vector in which to copy the tangent vector of the vertex. Corresponds to the texture's X axis, which is the absciss
 * @param {AxVector3} biTangent The vector in which to copy the biTangent vector of the vertex. Corresponds to the texture's Y axis, which is the ordinate
 */
AxDeviceMesh.prototype.GetVertexTangents = function(index, tangent, biTangent) { };
/**
 * Sets the tangent vectors of a given vertex. The tangent vectors represent the mapped texture's base axes in model space, tangent to the model's surface
 * @param {Integer} index The index of the vertex for which to set tangent vector
 * @param {AxVector3} tangent The value of the tangent vector to set. Corresponds to the texture's X axis, which is the absciss
 * @param {AxVector3} biTangent The value of the bitangent vector to set. Corresponds to the texture's Y axis, which is the ordinate
 */
AxDeviceMesh.prototype.SetVertexTangents = function(index, tangent, biTangent) { };



/**
 * Gets the value of an index in the mesh. 
 * @param {Integer} index The index of the vertex to set bone data to
 * @returns {Integer} The value of an index in the mesh. 
 */
AxDeviceMesh.prototype.GetIndex = function(index) { };
/**
 * Sets the value of an index in the mesh. 
 * @param {Integer} index The index of the vertex to set bone data to
 * @param {Integer} value The new value for the index
 */
AxDeviceMesh.prototype.SetIndex = function(index, value) { };