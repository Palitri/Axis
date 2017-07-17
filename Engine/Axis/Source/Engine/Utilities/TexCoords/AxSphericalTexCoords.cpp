//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxSphericalTexCoords.h"

#include "..\..\..\Tools\AxMath.h"

AxSphericalTexCoords::AxSphericalTexCoords(AxDeviceMesh &result)
{
	int numVertices = result.GetVertexCount();
	for (int i = 0; i < numVertices; i++)
	{
		AxVector3 vPos, vSphere;
		result.GetVertexPosition(i, vPos);
		
		AxVector3::CartesianToSpherical(vSphere, vPos);
		//AxVector3::SphereToCartesian(vSphere, vSphere.x, vSphere.y, 1.0f);

		result.SetVertexTexCoords(i, AxVector2(vSphere.x / AxMath::Pi, (1.0f - vSphere.y / AxMath::PiHalf) / 2.0f));
	}
}


AxSphericalTexCoords::~AxSphericalTexCoords(void)
{
}
