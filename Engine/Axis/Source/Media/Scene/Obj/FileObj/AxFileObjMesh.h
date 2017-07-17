#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\AxList.h"
#include "..\..\..\..\Tools\AxString.h"

#include "AxFileObjPrimitiveTypes.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjFace*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjVector4>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjVector3>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjFaceVertex>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjBufferedFace*>;

class AXDLLCLASS AxFileObjMesh
{
private:
	int FindEquivalentBufferedVertexIndex(AxFileObjFaceVertex *faceVertex);

public:
	AxString name, materialName;

	// The raw faces data from the Obj file
	AxList<AxFileObjFace*> faces;
	// The raw vertices data from the Obj file
	AxList<AxFileObjVector4> vertices;
	// The raw nomals and texCoords data from the Obj file
	AxList<AxFileObjVector3> normals, texCoords;

	// Filled by CreateBuffers(), a set of all used unique combinations of vertex position/texCoords/normal
	AxList<AxFileObjFaceVertex> bufferedVertices;
	// Filled by CreateBuffers(), a set of all primitives, such as to correctly reference the vertices in vertexBuffer
	AxList<AxFileObjBufferedFace*> bufferedFaces;
	
	AxFileObjMesh(void);
	~AxFileObjMesh(void);

	// After data from the file has been filled, this method creates a vertex buffer of vertices with unique combinations of position/texCoords/normal and a respective primitive (index) buffer.
	// Required because although Obj file uses indexing, it can have face vertices referencing the same vertex position, but in combination with different texCoords or normal
	void CreateBuffers();
};

