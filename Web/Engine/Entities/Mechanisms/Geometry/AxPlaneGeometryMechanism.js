/**
 * Creates a new plane geometry mechanism
 * @constructor
 */
function AxPlaneGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxPlaneGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Width"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Length"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Width segments"), 1));
    this.properties.Add(new AxProperty(new AxString("Length segments"), 1));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxPlaneGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxPlaneGeometryMechanism.classId = AxMechanism.classId | 201;

AxPlaneGeometryMechanism.propertyIndex_Width		= AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxPlaneGeometryMechanism.propertyIndex_Length		= AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxPlaneGeometryMechanism.propertyIndex_WidthSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxPlaneGeometryMechanism.propertyIndex_LengthSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxPlaneGeometryMechanism.propertyIndex_Mesh		= AxResource.propertyIndex_ChildPropertiesIndex + 4;


AxPlaneGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var width = this.properties.Get(AxPlaneGeometryMechanism.propertyIndex_Width).GetFloat();
    var length = this.properties.Get(AxPlaneGeometryMechanism.propertyIndex_Length).GetFloat();
    var widthSegments = this.properties.Get(AxPlaneGeometryMechanism.propertyIndex_WidthSegments).GetInt();
    var lengthSegments = this.properties.Get(AxPlaneGeometryMechanism.propertyIndex_LengthSegments).GetInt();
    var mesh = this.properties.Get(AxPlaneGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();

    if (mesh === 0)
        return;

    // Create a flat plane
    new AxPlaneGeometry(mesh.deviceMesh, widthSegments, lengthSegments, new AxVector3(width, 0, 0), new AxVector3(0, 0, length));
    new AxPlanarTexCoords(mesh.deviceMesh, new AxVector3(width, 0, 0), new AxVector3(0, 0, length));

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
