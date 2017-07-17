//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScene.h"


AxScene::AxScene(void)
{
	this->context = 0;
}


AxScene::~AxScene(void)
{
}

void AxScene::SetContext(Axis *context)
{
	this->context = context;
}