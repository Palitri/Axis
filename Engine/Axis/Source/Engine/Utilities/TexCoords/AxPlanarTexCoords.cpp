//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxPlanarTexCoords.h"

#include "..\AxMaths.h"

AxPlanarTexCoords::AxPlanarTexCoords(AxDeviceMesh &result, AxVector3 &planeX, AxVector3 &planeY)
{
	AxVector2 centerCorrection(0.5f, 0.5f);

	int numVertices = result.GetVertexCount();
	for (int i = 0; i < numVertices; i++)
	{
		AxVector3 vPos;
		result.GetVertexPosition(i, vPos);

		AxVector2 texCoords;
		AxMaths::GetBarycentricCoordinates(texCoords, vPos, planeX, planeY);
				
		AxVector2::Add(texCoords, texCoords, centerCorrection);
		result.SetVertexTexCoords(i, texCoords);
	}
}


AxPlanarTexCoords::~AxPlanarTexCoords(void)
{
}
