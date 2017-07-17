/**
 * Creates a new geosphere geometry mechanism
 */
 function AxGeosphereGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxGeosphereGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Radius"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Subdivisions"), 2));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxGeosphereGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxGeosphereGeometryMechanism.classId = AxMechanism.classId | 204;

AxGeosphereGeometryMechanism.propertyIndex_Radius           = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxGeosphereGeometryMechanism.propertyIndex_Subdivisions     = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxGeosphereGeometryMechanism.propertyIndex_Mesh             = AxResource.propertyIndex_ChildPropertiesIndex + 2;


AxGeosphereGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var radius = this.properties.Get(AxGeosphereGeometryMechanism.propertyIndex_Radius).GetFloat();
    var subDivisions = this.properties.Get(AxGeosphereGeometryMechanism.propertyIndex_Subdivisions).GetInt();
    var mesh = this.properties.Get(AxGeosphereGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();

    if (mesh === 0)
        return;

    new AxGeosphereGeometry(mesh.deviceMesh, radius, subDivisions);
    new AxSphericalTexCoords(mesh.deviceMesh);

    mesh.ComputeBounds();
    mesh.ComputeNormals();
    mesh.ComputeTangents();

    mesh.deviceMesh.UpdateVertices(0, mesh.deviceMesh.GetVertexCount());
    mesh.deviceMesh.UpdateIndices(0, mesh.deviceMesh.GetIndexCount());
};
