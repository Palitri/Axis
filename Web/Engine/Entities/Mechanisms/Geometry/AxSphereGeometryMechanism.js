/**
 * Creates a new sphere geometry mechanism
 * @constructor
 */
function AxSphereGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxSphereGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Radius"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Radial segments"), 10));
    this.properties.Add(new AxProperty(new AxString("Height segments"), 10));
    this.properties.Add(new AxProperty(new AxString("Twist"), 0.0));
    this.properties.Add(new AxProperty(new AxString("Weld surface"), false));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxSphereGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxSphereGeometryMechanism.classId = AxMechanism.classId | 203;

AxSphereGeometryMechanism.propertyIndex_Radius		= AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxSphereGeometryMechanism.propertyIndex_RadialSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxSphereGeometryMechanism.propertyIndex_HeightSegments	= AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxSphereGeometryMechanism.propertyIndex_Twist		= AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxSphereGeometryMechanism.propertyIndex_WeldSurface	= AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxSphereGeometryMechanism.propertyIndex_Mesh		= AxResource.propertyIndex_ChildPropertiesIndex + 5;


AxSphereGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var radius = this.properties.Get(AxSphereGeometryMechanism.propertyIndex_Radius).GetFloat();
    var radialSegments = this.properties.Get(AxSphereGeometryMechanism.propertyIndex_RadialSegments).GetInt();
    var heightSegments = this.properties.Get(AxSphereGeometryMechanism.propertyIndex_HeightSegments).GetInt();
    var twist = this.properties.Get(AxSphereGeometryMechanism.propertyIndex_Twist).GetFloat();
    var weldSurface = this.properties.Get(AxSphereGeometryMechanism.propertyIndex_WeldSurface).GetBool();
    var mesh = this.properties.Get(AxSphereGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();

    if (mesh === 0)
        return;

    new AxSphereGeometry(mesh.deviceMesh, radius, radialSegments, heightSegments, twist, weldSurface);

    mesh.ComputeBounds();
    mesh.ComputeNormals();
    mesh.ComputeTangents();

    mesh.deviceMesh.UpdateVertices(0, mesh.deviceMesh.GetVertexCount());
    mesh.deviceMesh.UpdateIndices(0, mesh.deviceMesh.GetIndexCount());
};