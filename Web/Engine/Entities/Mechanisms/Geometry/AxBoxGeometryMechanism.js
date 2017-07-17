/**
 * Creates a new box geometry mechanism
 */
function AxBoxGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxBoxGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Width"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Length"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Height"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Width segments"), 1));
    this.properties.Add(new AxProperty(new AxString("Length segments"), 1));
    this.properties.Add(new AxProperty(new AxString("Height segments"), 1));
    this.properties.Add(new AxProperty(new AxString("Texture 2x3"), false));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxBoxGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxBoxGeometryMechanism.classId = AxMechanism.classId | 202;

AxBoxGeometryMechanism.propertyIndex_Width		= AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxBoxGeometryMechanism.propertyIndex_Length		= AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxBoxGeometryMechanism.propertyIndex_Height		= AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxBoxGeometryMechanism.propertyIndex_WidthSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxBoxGeometryMechanism.propertyIndex_LengthSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxBoxGeometryMechanism.propertyIndex_HeightSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 5;
AxBoxGeometryMechanism.propertyIndex_Texture2x3		= AxResource.propertyIndex_ChildPropertiesIndex + 6;
AxBoxGeometryMechanism.propertyIndex_Mesh		= AxResource.propertyIndex_ChildPropertiesIndex + 7;


AxBoxGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var width = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_Width).GetFloat();
    var length = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_Length).GetFloat();
    var height = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_Height).GetFloat();
    var widthSegments = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_WidthSegments).GetInt();
    var lengthSegments = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_LengthSegments).GetInt();
    var heightSegments = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_HeightSegments).GetInt();
    var texture2x3 = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_Texture2x3).GetBool();
    var mesh = this.properties.Get(AxBoxGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();

    if (mesh === 0)
        return;

    new AxBoxGeometry(mesh.deviceMesh, widthSegments, heightSegments, lengthSegments, texture2x3, new AxVector3(width, 0.0, 0.0), new AxVector3(0.0, height, 0.0), new AxVector3(0.0, 0.0, length));

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
