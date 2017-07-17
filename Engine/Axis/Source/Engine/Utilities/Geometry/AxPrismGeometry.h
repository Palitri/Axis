#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Graphics\AxDeviceMesh.h"

class AXDLLCLASS AxPrismGeometry
{
public:
	// Creates a regular n-gonal right prism with given radii for the top and bottom base
	// If the number of segments for height, base or top are zero, the respective parts of the prism will be absent
	// A cone can be visually represented by a prism with enough radial segments to make it look round and smooth
	// result The mesh object in which to store the resulting prism
	// bottomRadius The radius of the bottom base regular polygon
	// topRadius The radius of the top base regular polygon
	// height The height of the prism
	// radialSegments The number of sides of the base of the prism. Must be 3 or more
	// heightSegments The number of segments along the height of the prism. If zero, the body of the prism will not be created
	// bottomCapSegments The number of segments for the bottom base. If zero, no bottom cap will be created
	// topCapSegments The number of segments for the top base. If zero, no top cap will be created
	AxPrismGeometry(AxDeviceMesh &result, float bottomRadius, float topRadius, float height, int radialSegments, int heightSegments, int baseCapSegments = 0, int topCapSegments = 0);

	~AxPrismGeometry(void);
};

