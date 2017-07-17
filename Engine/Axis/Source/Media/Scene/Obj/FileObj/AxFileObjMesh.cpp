//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFileObjMesh.h"


AxFileObjMesh::AxFileObjMesh(void)
{
}


AxFileObjMesh::~AxFileObjMesh(void)
{
	for (int i = 0; i < this->faces.count; i++)
		delete this->faces[i];

	for (int i = 0; i < this->bufferedFaces.count; i++)
		delete this->bufferedFaces[i];
}

void AxFileObjMesh::CreateBuffers()
{
	for (int i = 0; i < this->bufferedFaces.count; i++)
		delete this->bufferedFaces[i];

	this->bufferedFaces.SetSize(this->faces.count);
	this->bufferedVertices.SetSize(0);

	for (int faceIndex = 0; faceIndex < this->faces.count; faceIndex++)
	{
		int numFaceVertices = this->faces[faceIndex]->vertices.count;

		this->bufferedFaces[faceIndex] = new AxFileObjBufferedFace();
		this->bufferedFaces[faceIndex]->bufferedVertexIndices.SetSize(numFaceVertices);

		for (int faceVertexIndex = 0; faceVertexIndex < numFaceVertices; faceVertexIndex++)
		{
			AxFileObjFaceVertex *faceVertex = &(*this->faces[faceIndex]).vertices[faceVertexIndex];

			int bufferedVertexIndex = this->FindEquivalentBufferedVertexIndex(faceVertex);
			if (bufferedVertexIndex == -1)
			{
				this->bufferedVertices.Add(*faceVertex);

				bufferedVertexIndex = this->bufferedVertices.count - 1;
			}

			this->bufferedFaces[faceIndex]->smoothingGroups = this->faces[faceIndex]->smoothingGroups;
			this->bufferedFaces[faceIndex]->bufferedVertexIndices[faceVertexIndex] = bufferedVertexIndex;
		}
	}
}

int AxFileObjMesh::FindEquivalentBufferedVertexIndex(AxFileObjFaceVertex *faceVertex)
{
	for (int i = 0; i < this->bufferedVertices.count; i++)
	{
		AxFileObjFaceVertex *bufferedVertex = &this->bufferedVertices[i];
		if ((faceVertex->vertexIndex == bufferedVertex->vertexIndex) && (faceVertex->texCoordIndex == bufferedVertex->texCoordIndex) && (faceVertex->normalIndex == bufferedVertex->normalIndex))
			return i;
	}
	
	return -1;
}
