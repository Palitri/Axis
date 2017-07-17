//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxBoundingVolume.h"

#include "AxMatrix.h"
#include "AxVector4.h"

#include "..\Utilities\AxMaths.h"

AxBoundingVolume::AxBoundingVolume(void)
{
}


AxBoundingVolume::~AxBoundingVolume(void)
{
}

bool AxBoundingVolume::InsideViewFrustum(AxMatrix &viewFrustum)
{
	return AxMaths::SphereViewFrustumIntersection(viewFrustum, this->center, this->sphereRadius);
}
