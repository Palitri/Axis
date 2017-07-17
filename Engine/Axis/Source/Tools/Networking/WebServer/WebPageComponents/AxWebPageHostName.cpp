//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWebPageHostName.h"

AxWebPageHostName::AxWebPageHostName(void)
{
}


AxWebPageHostName::~AxWebPageHostName(void)
{
}

AxString AxWebPageHostName::Output(AxWebRequest *request)
{
	return AxString(AxString("http://") + request->hostName);
}
