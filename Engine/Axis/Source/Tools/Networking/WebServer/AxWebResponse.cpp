//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWebResponse.h"


AxWebResponse::AxWebResponse(AxWebRequest *request)
{
	this->request = request;

	this->data = new AxMemoryStream();

	this->newSession = false;
}


AxWebResponse::~AxWebResponse(void)
{
	delete this->data;
}
