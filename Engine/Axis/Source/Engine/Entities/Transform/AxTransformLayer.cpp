//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTransformLayer.h"


AxTransformLayer::AxTransformLayer(void)
{
}

AxTransformLayer::AxTransformLayer(AxTransformOperation operation, int propertyFirst, int propertyCount)
{
	this->operation = operation;
	this->propertyFirst = propertyFirst;
	this->propertyCount = propertyCount;
}


AxTransformLayer::~AxTransformLayer(void)
{
}
