/**
 * Creates a sphere, segmented along its latitude and longitude
 * @param {AxDeviceMesh} result The mesh to hold the sphere
 * @param {Number} radius The radius of the sphere
 * @param {Integer} radialSegments The number of segments along the latitudal circumference of the sphere
 * @param {Integer} heightSegments The number of segments along the longitude of the sphere
 * @param {Number} twist The number of twists along the height of the sphere
 * @param {Boolean} weldSurface Denotes whether to weld the edges of the surface which makes up the sphere. Welded sphere has no "stitched" edge, but can't have a fully proper texture mapped spherically, because it shares the same vertices with both textrure coordintas 0 and 1
 * @constructor
 */
function AxSphereGeometry(result, radius, radialSegments, heightSegments, twist, weldSurface)
{
    var heightLinesCount = heightSegments + 1;
    var radialLinesCount = weldSurface ? radialSegments : radialSegments + 1;

    if ((radialSegments < 2) || (heightSegments < 2))
        return;

    result.CreateVertexBuffer(radialLinesCount * heightLinesCount);

    var index = 0;
    for (var heightLineIndex = 0; heightLineIndex < heightLinesCount; heightLineIndex++)
    {
        var v = heightLineIndex / heightSegments;

        var heightAngle = (v - 0.5) * AxMath.Pi;

        var heightCircleHeight = AxMath.Sin(heightAngle) * radius;
        var heightCircleRadius = AxMath.Cos(heightAngle) * radius;

        for (var radialLineIndex = 0; radialLineIndex < radialLinesCount; radialLineIndex++)
        {
            var u = radialLineIndex / radialSegments;

            var radialAngle = (u + twist * v) * AxMath.Pi2;

            result.SetVertexPosition(index, new AxVector3(AxMath.Cos(radialAngle) * heightCircleRadius, heightCircleHeight, AxMath.Sin(radialAngle) * heightCircleRadius));
            result.SetVertexTexCoords(index, new AxVector2(u, v));

            index++;
        }
    }


    result.CreateIndexBuffer(heightSegments * radialSegments * 2 * 3);

    index = 0;
    for (var heightIndex = 0; heightIndex < heightSegments; heightIndex++)
    {
        for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
        {
            var line00 = heightIndex * radialLinesCount + radialIndex;
            var line01 = heightIndex * radialLinesCount + (radialIndex + 1) % radialLinesCount;
            var line10 = (heightIndex + 1) * radialLinesCount + radialIndex;
            var line11 = (heightIndex + 1) * radialLinesCount + (radialIndex + 1) % radialLinesCount;

            result.SetIndex(index++, line00);
            result.SetIndex(index++, line01);
            result.SetIndex(index++, line10);

            result.SetIndex(index++, line01);
            result.SetIndex(index++, line11);
            result.SetIndex(index++, line10);
        }
    }
}
