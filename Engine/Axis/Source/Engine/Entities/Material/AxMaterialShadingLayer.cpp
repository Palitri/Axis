//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMaterialShadingLayer.h"


AxMaterialShadingLayer::AxMaterialShadingLayer(void)
{
	this->shadingOp = AxShadingOperation_None;
	this->colorOp = AxColorOperation_None;;
    this->propertyFirst = 0;
    this->propertyCount = 0;
}

AxMaterialShadingLayer::AxMaterialShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp, int propertyFirst, int propertyCount)
{
    this->shadingOp = shadingOp;
    this->colorOp = colorOp;
    this->propertyFirst = propertyFirst;
    this->propertyCount = propertyCount;
}


AxMaterialShadingLayer::~AxMaterialShadingLayer(void)
{
}
