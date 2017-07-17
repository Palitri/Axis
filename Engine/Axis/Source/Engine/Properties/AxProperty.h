#pragma once

#include "..\..\AxGlobals.h"

#include "AxParameter.h"
#include "..\..\Tools\AxString.h"

class AXDLLCLASS AxProperty
	: public AxParameter
{
public:
	AxString name;

	AxProperty(const AxString &name, int value);
	AxProperty(const AxString &name, float value);
	AxProperty(const AxString &name, bool value);
	AxProperty(const AxString &name, const AxVector4 &value);
	AxProperty(const AxString &name, void *value, AxParameterType type);
	virtual ~AxProperty(void);
};

