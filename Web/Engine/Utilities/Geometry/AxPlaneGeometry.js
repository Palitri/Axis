/**
 * Creates a plane with the given number of segments, along the given vectors
 * New mesh vertices are created with their position filled
 * New mesh indices are created
 * @param {AxDeviceMesh} result The mesh to hold the plane geometry
 * @param {Integer} xSegments The number of segments along the plane's X axis
 * @param {Integer} ySegments The number of segments along the plane's Y axis
 * @param {!AxVector3} xAxis The orientation of the plane's X axis. If omitted, the arbitraty X axis is assumed
 * @param {!AxVector3} yAxis The orientation of the plane's Y axis. If omitted, the arbitraty Y axis is assumed
 */
function AxPlaneGeometry(result, xSegments, ySegments, xAxis, yAxis)
{
    if (AxUtils.IsUndefinedOrNull(xAxis))
        xAxis = new AxVector3(1.0, 0.0, 0.0);
    if (AxUtils.IsUndefinedOrNull(yAxis))
        yAxis = new AxVector3(0.0, 1.0, 0.0);
    
    this.CreateGeometry(result, xSegments, ySegments, xAxis, yAxis);
}


/**
 * Creates a plane with the given number of segments, along the given vectors
 * New mesh vertices are created with their position filled
 * New mesh indices are created
 * @param {AxDeviceMesh} result The mesh to hold the plane geometry
 * @param {Integer} xSegments The number of segments along the plane's X axis
 * @param {Integer} ySegments The number of segments along the plane's Y axis
 * @param {AxVector3} xAxis The orientation of the plane's X axis
 * @param {AxVector3} yAxis The orientation of the plane's Y axis
 */
AxPlaneGeometry.prototype.CreateGeometry = function(result, xSegments, ySegments, xAxis, yAxis)
{
    result.CreateVertexBuffer((xSegments + 1) * (ySegments + 1));

    var pos = new AxVector3();
    var xPos = new AxVector3();
    var yPos = new AxVector3();
    var index = 0;
    for (var y = 0; y <= ySegments; y++)
    {
        var v = y / ySegments;
        AxVector3.Scale(yPos, yAxis, v - 0.5);

        for (var x = 0; x <= xSegments; x++)
        {
            var u = x / xSegments;
            AxVector3.Scale(xPos, xAxis, u - 0.5);

            AxVector3.Add(pos, xPos, yPos);

            result.SetVertexTexCoords(index, new AxVector2(u, v));
            result.SetVertexPosition(index++, pos);
        }
    }


    result.CreateIndexBuffer(xSegments * ySegments * 2 * 3);

    index = 0;
    for (var y = 0; y < ySegments; y++)
    {
        for (var x = 0; x < xSegments; x++)
        {
            var line0 = y * (xSegments + 1) + x;
            var line1 = (y + 1) * (xSegments + 1) + x;

            result.SetIndex(index++, line0);
            result.SetIndex(index++, line0 + 1);
            result.SetIndex(index++, line1);

            result.SetIndex(index++, line0 + 1);
            result.SetIndex(index++, line1 + 1);
            result.SetIndex(index++, line1);
        }
    }
};
