//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTangentsGenerator.h"

#include "..\AxMaths.h"

AxTangentsGenerator::AxTangentsGenerator(AxDeviceMesh &result)
{
	AxVector3 v0, v1, v2;
	AxVector2 t0, t1, t2;
	AxVector3 tangent, biTangent;

	int numVertices = result.GetVertexCount();
	int numIndices = result.GetIndexCount();
	AxVector3 *tangents = new AxVector3[numVertices];
	AxVector3 *biTangents = new AxVector3[numVertices];
    int i;

    for (i = 0; i < numVertices; i++)
    {
		tangents[i].Set(0.0f);
		biTangents[i].Set(0.0f);
    }

    for (i = 0; i < numIndices; i += 3)
    {
		int index0 = result.GetIndex(i);
		int index1 = result.GetIndex(i + 1);
		int index2 = result.GetIndex(i + 2);

		result.GetVertexPosition(index0, v0); 
		result.GetVertexPosition(index1, v1); 
		result.GetVertexPosition(index2, v2); 
        	
		result.GetVertexTexCoords(index0, t0); 
		result.GetVertexTexCoords(index1, t1); 
		result.GetVertexTexCoords(index2, t2); 

		AxMaths::GetTangents(tangent, biTangent, v0, v1, v2, t0, t1, t2);

		AxVector3::Add(tangents[index0], tangents[index0], tangent);
		AxVector3::Add(tangents[index1], tangents[index1], tangent);
		AxVector3::Add(tangents[index2], tangents[index2], tangent);

		AxVector3::Add(biTangents[index0], biTangents[index0], biTangent);
		AxVector3::Add(biTangents[index1], biTangents[index1], biTangent);
		AxVector3::Add(biTangents[index2], biTangents[index2], biTangent);
    }

    for (i = 0; i < numVertices; i++)
    {
		AxVector3::Normalize(tangent, tangents[i]);
		AxVector3::Normalize(biTangent, biTangents[i]);
		result.SetVertexTangents(i, tangent, biTangent);
    }

	delete[] tangents;
	delete[] biTangents;
}


AxTangentsGenerator::~AxTangentsGenerator(void)
{
}
