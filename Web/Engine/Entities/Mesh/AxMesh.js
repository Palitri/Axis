/**
 * Creates a new mesh
 * @param {Axis} context The context to use for the mesh
 * @constructor
 */
function AxMesh(context)
{
    AxResource.call(this);
    
    this.typeId = AxMesh.classId;
    this.resourceType = AxResourceType.Mesh;
    this.active = true;

    this.bounds = new AxBoundingVolume();
        
    this.properties.Add(new AxProperty(new AxString('Cull'), true));
    
    this.deviceMesh = context.graphicsDevice.CreateMesh();

    this.serializeVertices = true;
    this.serializeTexCoords = true;
    this.serializeBones = true;
    this.serializeNormals = false;
    this.serializeTangents = false;
}

AxMesh.prototype = Object.create(AxResource.prototype);



AxMesh.classId = (AxResourceType.Mesh << 16) | 0;

AxMesh.propertyIndex_Cull = AxResource.propertyIndex_ChildPropertiesIndex + 0;

AxMesh.SerializationId_Vertices 	= 0x21111004;
AxMesh.SerializationId_Normals          = 0x21112004;
AxMesh.SerializationId_TexCoords	= 0x21113004;
AxMesh.SerializationId_Triangles	= 0x21114004;
AxMesh.SerializationId_Bones		= 0x21115004;



AxMesh.prototype.Dispose = function()
{
    //this.deviceMesh.Dispose();
};

/**
 * Computes normals for the mesh based on averaging the orientation of shared faces
 */
AxMesh.prototype.ComputeNormals = function()
{
    new AxNormalsGenerator(this.deviceMesh);	

    this.deviceMesh.UpdateVertices(0, -1);
};

/**
 * Computes tangent vectors for the mesh, based on the orientation of the mesh's texture mapping base vectors, by averaging the tangents of a vertex's shared faces
 */
AxMesh.prototype.ComputeTangents = function()
{
    new AxTangentsGenerator(this.deviceMesh);	

    this.deviceMesh.UpdateVertices(0, -1);
};

/**
 * Computes the bounding volume of the mesh
 */
AxMesh.prototype.ComputeBounds = function()
{
    v = new AxVector3();
    min = new AxVector3();
    max = new AxVector3();

    min.Set_1(AxMath.FloatMax);
    max.Set_1(AxMath.FloatMin);
    var numVertices = this.deviceMesh.GetVertexCount();
    for (var i = 0; i < numVertices; i++)
    {
        this.deviceMesh.GetVertexPosition(i, v);
        AxVector3.Min(min, min, v);
        AxVector3.Max(max, max, v);
    }

    AxVector3.Add(this.bounds.center, min, max);
    AxVector3.Scale(this.bounds.center, this.bounds.center, 1.0 / 2.0);

    AxVector3.Subtract(this.bounds.box, max, min);
    AxVector3.Scale(this.bounds.box, this.bounds.box, 1.0 / 2.0);

    this.bounds.sphereRadius = 0.0;
    for (var i = 0; i < numVertices; i++)
    {
        this.deviceMesh.GetVertexPosition(i, v);
        this.bounds.sphereRadius = AxMath.Max(this.bounds.sphereRadius, AxVector3.Distance(this.bounds.center, v));
    }
};

    
/**
 * Tests whether the mesh gets intersected by a given ray and gives details on the intersectoin
 * @param {AxVector3} rayPoint Starting point of the ray
 * @param {AxVector3} rayDir Orientation of the ray
 * @param {AxIntersectionInfo} intersectionInfo Outputs details on the intersection
 * @return {Boolean} True if the mesh gets intersected by the given ray
 */
AxMesh.prototype.Intersection = function(rayPoint, rayDir, intersectionInfo)
{
    intersectionInfo.hasIntersected = false;

    if (!AxMaths.RayToSphereIntersection(rayPoint, rayDir, this.bounds.center, this.bounds.sphereRadius))
        return false;

    var v0 = new AxVector3();
    var v1 = new AxVector3();
    var v2 = new AxVector3();
    var intersection = new AxVector3();

    intersectionInfo.distance = AxMath.FloatMax;
    var numIndices = this.deviceMesh.GetIndexCount();
    for (var i = 0; i < numIndices; i += 3)
    {
        var index0 = this.deviceMesh.GetIndex(i + 2);
        var index1 = this.deviceMesh.GetIndex(i + 1);
        var index2 = this.deviceMesh.GetIndex(i);

        this.deviceMesh.GetVertexPosition(index0, v0);
        this.deviceMesh.GetVertexPosition(index1, v1);
        this.deviceMesh.GetVertexPosition(index2, v2);
            
        if (AxMaths.RayToTriangleIntersection(rayPoint, rayDir, v0, v1, v2, intersection))
        {
            if ((intersection.z > 0) && (intersection.z < intersectionInfo.distance))
            {
                var delta1 = new AxVertex(), delta2 = new AxVertex();
                var n0 = new AxVector3(), n1 = new AxVector3(), n2 = new AxVector3();
                var texCoord0 = new AxVector2(), texCoord1 = new AxVector2(), texCoord2 = new AxVector2();

                this.deviceMesh.GetVertexNormal(index0, n0);
                this.deviceMesh.GetVertexNormal(index1, n1);
                this.deviceMesh.GetVertexNormal(index2, n2);

                this.deviceMesh.GetVertexTexCoords(index0, texCoord0);
                this.deviceMesh.GetVertexTexCoords(index1, texCoord1);
                this.deviceMesh.GetVertexTexCoords(index2, texCoord2);

                AxVector3.Subtract(delta1.position, v1, v0);
                AxVector3.Subtract(delta1.normal, n1, n0);
                AxVector2.Subtract(delta1.texCoords, texCoord1, texCoord0);

                AxVector3.Subtract(delta2.position, v2, v0);
                AxVector3.Subtract(delta2.normal, n2, n0);
                AxVector2.Subtract(delta2.texCoords, texCoord2, texCoord0);


                intersectionInfo.point.position.x = v0.x + delta1.position.x * intersection.x + delta2.position.x * intersection.y;
                intersectionInfo.point.position.y = v0.y + delta1.position.y * intersection.x + delta2.position.y * intersection.y;
                intersectionInfo.point.position.z = v0.z + delta1.position.z * intersection.x + delta2.position.z * intersection.y;
                intersectionInfo.point.normal.x = n0.x + delta1.normal.x * intersection.x + delta2.normal.x * intersection.y;
                intersectionInfo.point.normal.y = n0.y + delta1.normal.y * intersection.x + delta2.normal.y * intersection.y;
                intersectionInfo.point.normal.z = n0.z + delta1.normal.z * intersection.x + delta2.normal.x * intersection.y;
                intersectionInfo.point.texCoords.x = texCoord0.x + delta1.texCoords.x * intersection.x + delta2.texCoords.x * intersection.y;
                intersectionInfo.point.texCoords.y = texCoord0.y + delta1.texCoords.y * intersection.x + delta2.texCoords.y * intersection.y;

                intersectionInfo.distance = intersection.z;
                intersectionInfo.primitiveIndex = i;
            }
            intersectionInfo.hasIntersected = true;
        }
    }
    return intersectionInfo.hasIntersected;
};

/**
 * Copies another mesh
 * @param {AxMesh} source The mesh to be copied
 */
AxMesh.prototype.CopyFrom = function(source)
{
    this.deviceMesh.CopyFrom(source.deviceMesh);
};

/**
 * Deserializes the mesh from a given stream
 * @param {AxStream} source The stream holding the serialized data
 * @return {Boolean} True if the deserialization was successfull
 */
AxMesh.prototype.Deserialize = function(source)
{
    this.serializeVertices = false;
    this.serializeNormals = false;
    this.serializeTexCoords = false;
    this.serializeTangents = false;
    this.serializeBones = false;
    
    if (!AxResource.prototype.Deserialize.call(this, source))
        return false;
    
    if (!this.serializeNormals)
        this.ComputeNormals();

    if (!this.serializeTangents)
        this.ComputeTangents();
    

    this.deviceMesh.UpdateIndices(0, -1);
    this.deviceMesh.UpdateVertices(0, -1);
    this.ComputeBounds();

    return true;
};

/**
 * Writes chunks for all the data which is needed to serialize the mesh. 
 * @param {AxHierarchyStreamWriter} writer Writer to use for writing the serialization data
 */
AxMesh.prototype.SerializeChunks = function(writer)
{
    AxResource.prototype.SerializeChunks.call(this, writer);

    var embedMeshData = this.context.settings.properties.Get(AxSettings.propertyIndex_EmbedGeneratedMeshes).GetBool();

    var numVertices = this.deviceMesh.GetVertexCount();
    var numIndices = this.deviceMesh.GetIndexCount();
    var v3 = new AxVector3();
    var v2 = new AxVector2();
    var v41 = new AxVector4();
    var v42 = new AxVector4();

    if (this.serializeVertices || embedMeshData)
    {
        writer.BeginChunk(AxMesh.SerializationId_Vertices);
        writer.stream.WriteInt32(numVertices);
        for (var i = 0; i < numVertices; i++)
        {
            this.deviceMesh.GetVertexPosition(i, v3);
            AxSerializationUtils.SerializeVector3(writer.stream, v3);
        }
        writer.EndChunk();
    }

    if (this.serializeNormals)
    {
        writer.BeginChunk(AxMesh.SerializationId_Normals);
        writer.stream.WriteInt32(numVertices);
        for (var i = 0; i < numVertices; i++)
        {
            this.deviceMesh.GetVertexNormal(i, v3);
            AxSerializationUtils.SerializeVector3(writer.stream, v3);
        }
        writer.EndChunk();
    }

    if (this.serializeTexCoords || embedMeshData)
    {
        writer.BeginChunk(AxMesh.SerializationId_TexCoords);
        writer.stream.WriteInt32(numVertices);
        for (var i = 0; i < numVertices; i++)
        {
            this.deviceMesh.GetVertexTexCoords(i, v2);
            AxSerializationUtils.SerializeVector2(writer.stream, v2);
        }
        writer.EndChunk();
    }

    if (this.serializeVertices || embedMeshData)
    {
        writer.BeginChunk(AxMesh.SerializationId_Triangles);
        writer.stream.WriteInt32(numIndices);
        for (var i = 0; i < numIndices; i++)
        {
            writer.stream.WriteUInt32(this.deviceMesh.GetIndex(i));
        }
        writer.EndChunk();
    }

    if (this.serializeBones)
    {
        writer.BeginChunk(AxMesh.SerializationId_Bones);
        writer.stream.WriteInt32(numVertices);
        for (var i = 0; i < numVertices; i++)
        {
            this.deviceMesh.GetVertexBones(i, v41, v42);
            AxSerializationUtils.Serializeector4(writer.stream, v41);
            AxSerializationUtils.Serializeector4(writer.stream, v42);
        }
        writer.EndChunk();
    }
};

/**
 * Reads the data of a chunk. The chunk header is already read by the reader and this method deserializes the contents of the chunk. Called continuously for each of the mesh's chunks
 * @param {AxHierarchyStreamReader} reader Reader to use for reading the serialized data.
 * @return {Boolean} True if a chunk was successfully deserialized
 */
AxMesh.prototype.DeserializeChunk = function(reader)
{
    if (AxResource.prototype.DeserializeChunk.call(this, reader))
        return true;

    switch (reader.chunkId)
    {
        case AxMesh.SerializationId_Vertices:
        {
            var numVertices = reader.stream.ReadInt32();
            if (numVertices !== this.deviceMesh.GetVertexCount())
                this.deviceMesh.CreateVertexBuffer(numVertices);

            for (var i = 0; i < numVertices; i++)
                this.deviceMesh.SetVertexPosition(i, AxSerializationUtils.DeserializeVector3(reader.stream));
            
            this.serializeVertices = true;

            break;
        }

        case AxMesh.SerializationId_Normals:
        {
            var numVertices = reader.stream.ReadInt32();
            if (numVertices !== this.deviceMesh.GetVertexCount())
                this.deviceMesh.CreateVertexBuffer(numVertices);

            for (var i = 0; i < numVertices; i++)
                this.deviceMesh.SetVertexNormal(i, AxSerializationUtils.DeserializeVector3(reader.stream));
            
            this.deserializedNormals = true;
            
            this.serializeNormals = true;

            break;
        }

        case AxMesh.SerializationId_TexCoords:
        {
            var numVertices = reader.stream.ReadInt32();
            if (numVertices !== this.deviceMesh.GetVertexCount())
                this.deviceMesh.CreateVertexBuffer(numVertices);

            for (var i = 0; i < numVertices; i++)
                this.deviceMesh.SetVertexTexCoords(i, AxSerializationUtils.DeserializeVector2(reader.stream));

            this.serializeTexCoords = true;
            
            break;
        }

        case AxMesh.SerializationId_Triangles:
        {
            var numIndices = reader.stream.ReadInt32();
            this.deviceMesh.CreateIndexBuffer(numIndices);

            for (var i = 0; i < numIndices; i++)
                this.deviceMesh.SetIndex(i, reader.stream.ReadUInt32());
            
            break;
        }

        case AxMesh.SerializationId_Bones:
        {
            var numVertices = reader.stream.ReadInt32();
            if (numVertices !== this.deviceMesh.GetVertexCount())
                this.deviceMesh.CreateVertexBuffer(numVertices);

            var boneIndices = new AxVector4();
            var boneWeights = new AxVector4();
            for (var i = 0; i < numVertices; i++)
            {
                boneIndices = AxSerializationUtils.DeserializeVector4(reader.stream);
                boneWeights = AxSerializationUtils.DeserializeVector4(reader.stream);
                this.deviceMesh.SetVertexBones(i, boneIndices, boneWeights);
            }
            
            this.serializeBones = true;

            break;
        }

        default:
        {
            return false;
        }
    }

    return true;
};
