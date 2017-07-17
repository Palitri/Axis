#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\AxString.h"

enum AXDLLCLASS AxFile3dsObjectType
{
	AxFile3dsObjectType_Mesh,
	AxFile3dsObjectType_Light,
	AxFile3dsObjectType_Camera
};

class AXDLLCLASS AxFile3dsBaseObject
{
public:
	AxFile3dsObjectType objectType;

	AxString name;

	AxFile3dsBaseObject(void);
	virtual ~AxFile3dsBaseObject(void);
};