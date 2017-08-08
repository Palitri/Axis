/**
 * Constructs a prism, optionally truncated, with height along the Y-Axis
 * In terms of polygonal geomery in general, a multi-sided prism serves also as a cone
 * @param {AxDeviceMesh} result The mesh to hold the prism geometry
 * @param {Number} bottomRadius The radius of the bottom base of the prism
 * @param {Number} topRadius The radius of the top base of the prism
 * @param {Number} height The height of the prism
 * @param {Integer} radialSegments The number of sides of the prism, that is the number of segments along the circumference of the prism. Must be greater than 3
 * @param {Integer} heightSegments The number of segments along the height of the prism
 * @param {!Integer} bottomCapSegments The number of segments of the bottom base of the prism. If omitted or zero, no bottom base is created
 * @param {!Integer} topCapSegments The number of segments of the top base of the prism. If omitted or zero, no top base is created
 * @constructor
 */
function AxPrismGeometry(result, bottomRadius, topRadius, height, radialSegments, heightSegments, bottomCapSegments, topCapSegments)
{
    if (AxUtils.IsUndefinedOrNull(bottomCapSegments))
        bottomCapSegments = 0;

    if (AxUtils.IsUndefinedOrNull(topCapSegments))
        topCapSegments = 0;
    
    if (radialSegments < 3)
        return;

    var vertexCount = radialSegments * (heightSegments + 1);
    if (bottomCapSegments > 0)
        vertexCount += radialSegments * bottomCapSegments + 1;
    if (topCapSegments > 0)
        vertexCount += radialSegments * topCapSegments + 1;


    var facesCount = radialSegments * heightSegments * 2;
    if (bottomCapSegments > 0)
        facesCount += radialSegments * ((bottomCapSegments - 1) * 2 + 1);
    if (topCapSegments > 0)
        facesCount += radialSegments * ((topCapSegments - 1) * 2 + 1);

    result.CreateVertexBuffer(vertexCount);
    result.CreateIndexBuffer(facesCount * 3);


    var vertexOffset = 0;
    var vertexIndex = 0;

    var indexIndex = 0;

    // Cone body
    // Vertices
    for (var segmentIndex = 0; segmentIndex <= heightSegments; segmentIndex ++)
    {
        var heightUnit = segmentIndex / heightSegments;
        var segmentRadius = bottomRadius + (topRadius - bottomRadius) * heightUnit;
        var segmentHeight = height * (heightUnit - 0.5);

        for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
        {
            var angle = AxMath.Pi2 * radialIndex / radialSegments;

            var pos = new AxVector3(AxMath.Cos(angle) * segmentRadius, segmentHeight, AxMath.Sin(angle) * segmentRadius);

            result.SetVertexPosition(vertexIndex++, pos);
        }
    }

    // Faces
    for (var segmentIndex = 0; segmentIndex < heightSegments; segmentIndex++)
    {
        for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
        {
            result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + radialIndex);
            result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
            result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);

            result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
            result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + (radialIndex + 1) % radialSegments);
            result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);
        }
    }

    vertexOffset = vertexIndex;

    // Cone bottom cap
    if (bottomCapSegments > 0)
    {
        // Vertices
        for (var segmentIndex = 0; segmentIndex < bottomCapSegments; segmentIndex++)
        {
            var radiusUnit = 1.0 - segmentIndex / bottomCapSegments;
            var segmentRadius = bottomRadius * radiusUnit;
            var segmentHeight = - height / 2.0;
            for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
            {
                var angle = AxMath.Pi2 * radialIndex / radialSegments;

                var pos = new AxVector3(AxMath.Cos(angle) * segmentRadius, segmentHeight, AxMath.Sin(angle) * segmentRadius);

                result.SetVertexPosition(vertexIndex++, pos);
            }
        }
        var pos = new AxVector3(0.0, - height / 2.0, 0.0);
        result.SetVertexPosition(vertexIndex++, pos);

        // Faces
        for (var segmentIndex = 0; segmentIndex < bottomCapSegments - 1; segmentIndex++)
        {
            for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
            {
                result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
                result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + radialIndex);
                result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);

                result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + (radialIndex + 1) % radialSegments);
                result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
                result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);
            }
        }

        for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
        {
            result.SetIndex(indexIndex++, vertexOffset + (bottomCapSegments - 1) * radialSegments + (radialIndex + 1) % radialSegments);
            result.SetIndex(indexIndex++, vertexOffset + (bottomCapSegments - 1) * radialSegments + radialIndex);
            result.SetIndex(indexIndex++, vertexOffset + bottomCapSegments * radialSegments);
        }

    }


    vertexOffset = vertexIndex;

    // Cone top cap
    if (topCapSegments > 0)
    {
        // Vertices
        for (var segmentIndex = 0; segmentIndex < topCapSegments; segmentIndex++)
        {
            var radiusUnit = 1.0 - segmentIndex / topCapSegments;
            var segmentRadius = topRadius * radiusUnit;
            var segmentHeight = height / 2.0;
            for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
            {
                var angle = AxMath.Pi2 * radialIndex / radialSegments;

                var pos = new AxVector3(AxMath.Cos(angle) * segmentRadius, segmentHeight, AxMath.Sin(angle) * segmentRadius);

                result.SetVertexPosition(vertexIndex++, pos);
            }

        }
        var pos = new AxVector3(0.0, height / 2.0, 0.0);
        result.SetVertexPosition(vertexIndex++, pos);

        // Faces
        for (var segmentIndex = 0; segmentIndex < topCapSegments - 1; segmentIndex++)
        {
            for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
            {
                result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + radialIndex);
                result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
                result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);

                result.SetIndex(indexIndex++, vertexOffset + segmentIndex * radialSegments + (radialIndex + 1) % radialSegments);
                result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + (radialIndex + 1) % radialSegments);
                result.SetIndex(indexIndex++, vertexOffset + (segmentIndex + 1) * radialSegments + radialIndex);
            }
        }

        for (var radialIndex = 0; radialIndex < radialSegments; radialIndex++)
        {
            result.SetIndex(indexIndex++, vertexOffset + (topCapSegments - 1) * radialSegments + radialIndex);
            result.SetIndex(indexIndex++, vertexOffset + (topCapSegments - 1) * radialSegments + (radialIndex + 1) % radialSegments);
            result.SetIndex(indexIndex++, vertexOffset + topCapSegments * radialSegments);
        }

    }
}
