/**
 * Creates a new cone geometry mechanism
 * @constructor
 */
function AxConeGeometryMechanism()
{
    AxPropertyChangeNotifiedMechanism.call(this);

    this.typeId = AxConeGeometryMechanism.classId;

    this.properties.Add(new AxProperty(new AxString("Base radius"), 10.0));
    this.properties.Add(new AxProperty(new AxString("Top radius"), 5.0));
    this.properties.Add(new AxProperty(new AxString("Height"), 20.0));
    this.properties.Add(new AxProperty(new AxString("Radial segments"), 12));
    this.properties.Add(new AxProperty(new AxString("Height segments"), 1));
    this.properties.Add(new AxProperty(new AxString("Base cap segments"), 0));
    this.properties.Add(new AxProperty(new AxString("Top cap segments"), 0));
    this.properties.Add(new AxProperty(new AxString("Mesh"), 0, AxParameterType.ReferenceMesh));
}

AxConeGeometryMechanism.prototype = Object.create(AxPropertyChangeNotifiedMechanism.prototype);


AxConeGeometryMechanism.classId = AxMechanism.classId | 205;

AxConeGeometryMechanism.propertyIndex_BottmRadius           = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxConeGeometryMechanism.propertyIndex_TopRadius             = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxConeGeometryMechanism.propertyIndex_Height                = AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxConeGeometryMechanism.propertyIndex_RadialSegments        = AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxConeGeometryMechanism.propertyIndex_HeightSegments        = AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxConeGeometryMechanism.propertyIndex_BottomCapSegments     = AxResource.propertyIndex_ChildPropertiesIndex + 5;
AxConeGeometryMechanism.propertyIndex_TopCapSegments        = AxResource.propertyIndex_ChildPropertiesIndex + 6;
AxConeGeometryMechanism.propertyIndex_Mesh                  = AxResource.propertyIndex_ChildPropertiesIndex + 7;


AxConeGeometryMechanism.prototype.OnPropertiesChanged = function()
{
    var baseRadius = this.properties.Get(AxConeGeometryMechanism.propertyIndex_BottmRadius).GetFloat();
    var topRadius = this.properties.Get(AxConeGeometryMechanism.propertyIndex_TopRadius).GetFloat();
    var height = this.properties.Get(AxConeGeometryMechanism.propertyIndex_Height).GetFloat();
    var radialSegments = this.properties.Get(AxConeGeometryMechanism.propertyIndex_RadialSegments).GetInt();
    var heightSegments = this.properties.Get(AxConeGeometryMechanism.propertyIndex_HeightSegments).GetInt();
    var baseCapSegments = this.properties.Get(AxConeGeometryMechanism.propertyIndex_BottomCapSegments).GetInt();
    var topCapSegments = this.properties.Get(AxConeGeometryMechanism.propertyIndex_TopCapSegments).GetInt();
    var mesh = this.properties.Get(AxConeGeometryMechanism.propertyIndex_Mesh).GetEffectiveValue();

    if (mesh === 0)
        return;

    new AxPrismGeometry(mesh.deviceMesh, baseRadius, topRadius, height, radialSegments, heightSegments, baseCapSegments, topCapSegments);

    mesh.ComputeBounds();
    mesh.ComputeNormals();
    mesh.ComputeTangents();

    mesh.deviceMesh.UpdateVertices(0, mesh.deviceMesh.GetVertexCount());
    mesh.deviceMesh.UpdateIndices(0, mesh.deviceMesh.GetIndexCount());
};
