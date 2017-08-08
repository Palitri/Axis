/**
 * Creates a torus with a variety of options, which may extend the geometry to a spiral, helix, spring
 * @param {AxDeviceMesh} result The mesh to hold the sphere
 * @param {Integer} radialSegments The number of segments along the circumference of the torus
 * @param {Integer} ringSegments The number of segments along the circumference of the tube which makes up the torus
 * @param {Integer} twists The number of twists along the length of the tube, which makes up the torus
 * @param {Number} radius The radius of the torus, measured from its central point to the center of the tube
 * @param {Number} ringRadius The radius of the tube which makes up the torus
 * @param {Number} spiralSize The difference between the torus radius at start an end. Can be used to produce a spiral shape. A value of zero results in no spiral shaping
 * @param {Number} helicalHeight The difference between the torus center at start and end. Can be used to produce a helical spring-like shape. A value of zero results in no helical shaping
 * @param {Number} turns The number turns made by the tube of the torus
 * @param {Boolean} weldEnds Denotes whether to weld the ends of the torus tube
 * @param {Boolean} weldSurface Denotes whether to weld the surface, which makes up the torus tube
 * @constructor
 */
function AxTorusGeometry(result, radialSegments, ringSegments, twists, radius, ringRadius, spiralSize, helicalHeight, turns, weldEnds, weldSurface)
{
    var radialRingsCount = weldEnds ? radialSegments : radialSegments + 1;
    var ringVertexCount = weldSurface ? ringSegments : ringSegments + 1;

    result.CreateVertexBuffer(radialRingsCount * ringVertexCount);

    var index = 0;
    for (var radialIndex = 0; radialIndex < radialRingsCount; radialIndex++)
    {
        var u = radialIndex / radialSegments;
        var uUnit = radialIndex / (radialRingsCount + 1);

        var radialAngle = turns * AxMath.Pi2 * u;

        var radialVector = new AxVector3(AxMath.Cos(radialAngle), 0.0, AxMath.Sin(radialAngle));
        var ringPos = new AxVector3();
        AxVector3.Scale(ringPos, radialVector, radius + (spiralSize * uUnit));
        ringPos.y = uUnit * helicalHeight;

        var twistOffsetAngle = AxMath.Pi2 * twists * radialIndex / (radialRingsCount - 1);
        for (var ringIndex = 0; ringIndex < ringVertexCount; ringIndex++)
        {
            var v = ringIndex / ringSegments;

            var ringAngle = twistOffsetAngle + AxMath.Pi2 * v;

            var ringX = AxMath.Cos(ringAngle);
            var ringY = AxMath.Sin(ringAngle);

            var ringVector = new AxVector3(ringX * radialVector.x, ringY, ringX * radialVector.z);
            AxVector3.Scale(ringVector, ringVector, ringRadius);
            AxVector3.Add(ringVector, ringVector, ringPos);

            result.SetVertexPosition(index, ringVector);
            result.SetVertexTexCoords(index, new AxVector2(u, v));

            index++;
        }
    }

    result.CreateIndexBuffer(radialSegments * ringSegments * 2 * 3);

    index = 0;
    for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
    {
        var radialIndex0 = radialIndex * ringVertexCount;
        var radialIndex1 = ((radialIndex + 1) % radialRingsCount) * ringVertexCount;
        for (var ringIndex = 0; ringIndex < ringSegments; ringIndex++)
        {
            var ringIndex1 = (ringIndex + 1) % ringVertexCount;

            var ring00 = radialIndex0 + ringIndex;
            var ring01 = radialIndex0 + ringIndex1;
            var ring10 = radialIndex1 + ringIndex;
            var ring11 = radialIndex1 + ringIndex1;

            result.SetIndex(index++, ring00);
            result.SetIndex(index++, ring10);
            result.SetIndex(index++, ring01);

            result.SetIndex(index++, ring10);
            result.SetIndex(index++, ring11);
            result.SetIndex(index++, ring01);
        }
    }
}
