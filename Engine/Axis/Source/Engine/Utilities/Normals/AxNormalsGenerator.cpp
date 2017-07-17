//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNormalsGenerator.h"

#include "..\AxMaths.h"


AxNormalsGenerator::AxNormalsGenerator(AxDeviceMesh &result)
{
	AxVector3 v0, v1, v2,
			  n;

	int numVertices = result.GetVertexCount();
	int numIndices = result.GetIndexCount();
	AxVector3 *normals = new AxVector3[numVertices];
    int i;

    for (i = 0; i < numVertices; i++)
    {
		normals[i].Set(0.0f);
    }

    for (i = 0; i < numIndices; i += 3)
    {
		int index0 = result.GetIndex(i);
		int index1 = result.GetIndex(i + 1);
		int index2 = result.GetIndex(i + 2);

		result.GetVertexPosition(index0, v0); 
		result.GetVertexPosition(index1, v1); 
		result.GetVertexPosition(index2, v2); 
        	
		AxMaths::GetNormal(n, v0, v1, v2);

		AxVector3::Add(normals[index0], normals[index0], n);
        AxVector3::Add(normals[index1], normals[index1], n);
        AxVector3::Add(normals[index2], normals[index2], n);
    }

    for (i = 0; i < numVertices; i++)
    {
        AxVector3::Normalize(v0, normals[i]);
        result.SetVertexNormal(i, v0);
    }

	delete[] normals;
}


AxNormalsGenerator::~AxNormalsGenerator(void)
{
}
