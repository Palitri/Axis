/**
 * Creates a new heightmap geometry mechanism
 */
function AxHeightmapGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxHeightmapGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Width"), 100.0));
    this.properties.Add(new AxProperty(new AxString("Length"), 100.0));
    this.properties.Add(new AxProperty(new AxString("Height"), 25.0));
    this.properties.Add(new AxProperty(new AxString("Width segments"), 100));
    this.properties.Add(new AxProperty(new AxString("Length segments"), 100));
    this.properties.Add(new AxProperty(new AxString("Color filter"), new AxVector4(1.0, 1.0, 1.0, 0.0)));
    this.properties.Add(new AxProperty(new AxString("Height map"), 0, AxParameterType.ReferenceTexture));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxHeightmapGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxHeightmapGeometryMechanism.classId = AxMechanism.classId | 206;

AxHeightmapGeometryMechanism.propertyIndex_Width		= AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxHeightmapGeometryMechanism.propertyIndex_Length		= AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxHeightmapGeometryMechanism.propertyIndex_Height		= AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxHeightmapGeometryMechanism.propertyIndex_WidthSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxHeightmapGeometryMechanism.propertyIndex_LengthSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxHeightmapGeometryMechanism.propertyIndex_ColorFilter		= AxResource.propertyIndex_ChildPropertiesIndex + 5;
AxHeightmapGeometryMechanism.propertyIndex_HeightMap		= AxResource.propertyIndex_ChildPropertiesIndex + 6;
AxHeightmapGeometryMechanism.propertyIndex_Mesh			= AxResource.propertyIndex_ChildPropertiesIndex + 7;


AxHeightmapGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var width = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_Width).GetFloat();
    var length = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_Length).GetFloat();
    var height = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_Height).GetFloat();
    var widthSegments = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_WidthSegments).GetInt();
    var lengthSegments = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_LengthSegments).GetInt();
    var colorFilter = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_ColorFilter).GetColor();
    var heightMap = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_HeightMap).GetEffectiveValue();
    var mesh = this.properties.Get(AxHeightmapGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();
    

    if ((heightMap === 0) || (mesh === 0))
        return;

    // Tweak for asynchrnously loaded images, which might not yet be loaded in the first few frames
    if ((heightMap.width === 0) || (heightMap.height === 0))
    {
        var oldParam = this.oldProperties.Get(AxHeightmapGeometryMechanism.propertyIndex_HeightMap);
        oldParam.SetValue(0, oldParam.type);
        return;
    }

    // Create a flat plane
    new AxPlaneGeometry(mesh.deviceMesh, widthSegments, lengthSegments, new AxVector3(width, 0, 0), new AxVector3(0, 0, length));
    new AxPlanarTexCoords(mesh.deviceMesh, new AxVector3(width, 0, 0), new AxVector3(0, 0, length));
    new AxNormalsGenerator(mesh.deviceMesh);

    // Heighten the flat plane along its normals
    var heightMapImage = heightMap.ToImage();
    new AxHeightMapGeometry(mesh.deviceMesh, heightMapImage, colorFilter, height);

    mesh.ComputeBounds();
    mesh.ComputeNormals();
    mesh.ComputeTangents();

    mesh.deviceMesh.UpdateVertices(0, mesh.deviceMesh.GetVertexCount());
    mesh.deviceMesh.UpdateIndices(0, mesh.deviceMesh.GetIndexCount());

    mesh.serializeVertices = false;
    mesh.serializeNormals = false;
    mesh.serializeTexCoords = false;
    mesh.serializeTangents = false;
    mesh.serializeBones = false;
};
