#pragma once

#include "..\..\..\AxisEngine.h"

struct AxDynamicShaderConstantInfo
{
	AxString name;
	int offset, elementSize, elementCount, elementPitch;

	int GetAddress(int index);
};

class AxDynamicShaderConstantBufferInfo : 
	public AxList<AxDynamicShaderConstantInfo>
{
public:
	// Feature level 10 an higher can create buffers of sizes less or equal to D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT (currently, 4096)
	static const int MaxSize = 4096;
	// Constants reside in a vector of size 16 (a float4)
	static const int VectorSize = 16;

	int totalSize;

	AxDynamicShaderConstantBufferInfo(void);
	~AxDynamicShaderConstantBufferInfo(void);

	// Adds info for a constant or an array of constants. Can be used to create info on constants, which share the same vector in the GPU shader. For example, a float4 can hold up to 4 semantically different constants. In such a case, the first 3 should be created with a respective non-zero offset - 4, 8 or 12, and the last one with offset 0, which will also add to the total buffer size
	bool Add(AxString name, int elementSize, int elementCount = 1, int offset = 0, bool isArray = false);
	// Clears the info added so far.
	void Clear();
};

