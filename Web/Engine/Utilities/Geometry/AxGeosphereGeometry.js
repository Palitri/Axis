/**
 * Creates a sphere (geosphere) with the given radius and number of subdivisions, starting from an icosahedron
 * New mesh vertices are created with their position filled
 * New mesh indices are created
 * @param {AxDeviceMesh} result The mesh in which to create the sphere
 * @param {Number} radius The radius of the sphere
 * @param {Integer} subDivisions The number of subdivisions to apply to the polygons of the sphere
 * @constructor
 */
function AxGeosphereGeometry(result, radius, subDivisions)
{
    this.result = result;
    this.radius = radius;

    this.edges = new AxList();

    var icosa = new AxDeviceIndependentMesh();
    new AxIcosahedronGeometry(icosa, radius);

    this.result.CreateIndexBuffer(icosa.GetIndexCount() * AxMath.Power(4, subDivisions));
    this.result.CreateVertexBuffer(this.result.GetIndexCount() / 6 + 2);

    this.currentVertexIndex = icosa.GetVertexCount();
    this.currentIndex = 0;

    var position = new AxVector3();
    var normal = new AxVector3();
    var texCoords = new AxVector2();

    var numVertices = icosa.GetVertexCount();
    for (var i = 0; i < numVertices; i++)
    {
        icosa.GetVertexPosition(i, position);
        icosa.GetVertexNormal(i, normal);
        icosa.GetVertexTexCoords(i, texCoords);

        this.result.SetVertexPosition(i, position);
        this.result.SetVertexNormal(i, normal);
        this.result.SetVertexTexCoords(i, texCoords);
    }


    var numTriangles = icosa.GetPrimitiveCount();
    var index = 0;
    for (var triangleIndex = 0; triangleIndex < numTriangles; triangleIndex++)
    {
        this.result.SetIndex(this.currentIndex++, icosa.GetIndex(index++));
        this.result.SetIndex(this.currentIndex++, icosa.GetIndex(index++));
        this.result.SetIndex(this.currentIndex++, icosa.GetIndex(index++));

        this.SubDivideTriangle(this.currentIndex - 3, subDivisions);
    }
}

/**
 * Subdivides a triangle recursively
 * @param {Number} triangleIndex The index of the triangle to subdivide
 * @param {Number} subLevel The number of subdivisions
 */
AxGeosphereGeometry.prototype.SubDivideTriangle = function(triangleIndex, subLevel)
{
    if (subLevel === 0)
        return;

    var i0 = this.result.GetIndex(triangleIndex);
    var i1 = this.result.GetIndex(triangleIndex + 1);
    var i2 = this.result.GetIndex(triangleIndex + 2);

    var s0 = this.GetEdgeSubIndex(i0, i1);
    var s1 = this.GetEdgeSubIndex(i1, i2);
    var s2 = this.GetEdgeSubIndex(i2, i0);

    this.result.SetIndex(triangleIndex, i0);
    this.result.SetIndex(triangleIndex + 1, s0);
    this.result.SetIndex(triangleIndex + 2, s2);
    this.SubDivideTriangle(triangleIndex, subLevel - 1);

    this.result.SetIndex(this.currentIndex++, i1);
    this.result.SetIndex(this.currentIndex++, s1);
    this.result.SetIndex(this.currentIndex++, s0);
    this.SubDivideTriangle(this.currentIndex - 3, subLevel - 1);

    this.result.SetIndex(this.currentIndex++, i2);
    this.result.SetIndex(this.currentIndex++, s2);
    this.result.SetIndex(this.currentIndex++, s1);
    this.SubDivideTriangle(this.currentIndex - 3, subLevel - 1);

    this.result.SetIndex(this.currentIndex++, s0);
    this.result.SetIndex(this.currentIndex++, s1);
    this.result.SetIndex(this.currentIndex++, s2);
    this.SubDivideTriangle(this.currentIndex - 3, subLevel - 1);
};

/**
 * Gets the third index of a triangle polygon from two given indices of that polygon
 * @param {Integer} edgeIndex1 The first index of the triangle
 * @param {Integer} edgeIndex2 The second index of the triangle
 * @return {Integer} The third index of the triangle
 */
AxGeosphereGeometry.prototype.GetEdgeSubIndex = function(edgeIndex1, edgeIndex2)
{
    // Look for existing subvertex
    for (var i = 0; i < this.edges.count; i++)
        if (((this.edges.Get(i).index1 === edgeIndex1) && (this.edges.Get(i).index2 === edgeIndex2)) ||
            ((this.edges.Get(i).index1 === edgeIndex2) && (this.edges.Get(i).index2 === edgeIndex1)))
            return this.edges.Get(i).subIndex;

    // Generate subvertex
    var v1 = new AxVector3();
    var v2 = new AxVector3();
    var vsub = new AxVector3();
    this.result.GetVertexPosition(edgeIndex1, v1);
    this.result.GetVertexPosition(edgeIndex2, v2);
    AxVector3.Subtract(vsub, v2, v1);
    AxVector3.Scale(vsub, vsub, 0.5);
    AxVector3.Add(vsub, v1, vsub);
    AxVector3.SetLength(vsub, vsub, this.radius);
    this.result.SetVertexPosition(this.currentVertexIndex, vsub);

    // Add the edge's subvertex to the list
    var newEdge = new AxEdge();
    newEdge.index1 = edgeIndex1;
    newEdge.index2 = edgeIndex2;
    newEdge.subIndex = this.currentVertexIndex;
    this.edges.Add(newEdge);

    this.currentVertexIndex++;

    return newEdge.subIndex;
};

function AxEdge()
{
    this.index1 = 0;
    this.index2 = 0;
    this.subIndex = 0;
}