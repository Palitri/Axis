/**
 * Creates a new torus geometry mechanism
 * @constructor
 */
function AxTorusGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxTorusGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Radius"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Ring radius"), 1.0));
    this.properties.Add(new AxProperty(new AxString("Spiral size"), 0.0));
    this.properties.Add(new AxProperty(new AxString("Helical height"), 0.0));
    this.properties.Add(new AxProperty(new AxString("Turns"), 1.0));
    this.properties.Add(new AxProperty(new AxString("Radial segments"), 16));
    this.properties.Add(new AxProperty(new AxString("Ring segments"), 8));
    this.properties.Add(new AxProperty(new AxString("Start cap segments"), 0));
    this.properties.Add(new AxProperty(new AxString("End cap segments"), 0));
    this.properties.Add(new AxProperty(new AxString("Twists"), 0));
    this.properties.Add(new AxProperty(new AxString("Weld ends"), true));
    this.properties.Add(new AxProperty(new AxString("Weld surface"), true));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxTorusGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxTorusGeometryMechanism.classId = AxMechanism.classId | 207;

AxTorusGeometryMechanism.propertyIndex_Radius               = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxTorusGeometryMechanism.propertyIndex_RingRadius           = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxTorusGeometryMechanism.propertyIndex_SpiralSize           = AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxTorusGeometryMechanism.propertyIndex_HelicalHeight        = AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxTorusGeometryMechanism.propertyIndex_Turns                = AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxTorusGeometryMechanism.propertyIndex_RadialSegments       = AxResource.propertyIndex_ChildPropertiesIndex + 5;
AxTorusGeometryMechanism.propertyIndex_RingSegments         = AxResource.propertyIndex_ChildPropertiesIndex + 6;
AxTorusGeometryMechanism.propertyIndex_StartCapSegments     = AxResource.propertyIndex_ChildPropertiesIndex + 7;
AxTorusGeometryMechanism.propertyIndex_EndCapSegments       = AxResource.propertyIndex_ChildPropertiesIndex + 8;
AxTorusGeometryMechanism.propertyIndex_Twists               = AxResource.propertyIndex_ChildPropertiesIndex + 9;
AxTorusGeometryMechanism.propertyIndex_WeldEnds             = AxResource.propertyIndex_ChildPropertiesIndex + 10;
AxTorusGeometryMechanism.propertyIndex_WeldSurface          = AxResource.propertyIndex_ChildPropertiesIndex + 11;
AxTorusGeometryMechanism.propertyIndex_Mesh                 = AxResource.propertyIndex_ChildPropertiesIndex + 12;


AxTorusGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var radius = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_Radius).GetFloat();
    var ringRadius = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_RingRadius).GetFloat();
    var spiralSize = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_SpiralSize).GetFloat();
    var helicalHeight = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_HelicalHeight).GetFloat();
    var turns = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_Turns).GetFloat();
    var radialSegments = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_RadialSegments).GetInt();
    var ringSegments = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_RingSegments).GetInt();
    var startCapSegments = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_StartCapSegments).GetInt();
    var endCapSegments = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_EndCapSegments).GetInt();
    var twists = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_Twists).GetInt();
    var weldEnds = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_WeldEnds).GetBool();
    var weldSurface = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_WeldSurface).GetBool();
    var mesh = this.properties.Get(AxTorusGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();

    if (mesh === 0)
        return;

    new AxTorusGeometry(mesh.deviceMesh, radialSegments, ringSegments, twists, radius, ringRadius, spiralSize, helicalHeight, turns, weldEnds, weldSurface);

    mesh.ComputeBounds();
    mesh.ComputeNormals();
    mesh.ComputeTangents();

    mesh.deviceMesh.UpdateVertices(0, mesh.deviceMesh.GetVertexCount());
    mesh.deviceMesh.UpdateIndices(0, mesh.deviceMesh.GetIndexCount());
};
