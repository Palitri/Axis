//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptIdentifiers.h"


AxScriptIdentifiers::AxScriptIdentifiers(void)
{
	this->size = 0;
}


AxScriptIdentifiers::~AxScriptIdentifiers(void)
{
	this->Clear();
}

AxScriptIdentifier *AxScriptIdentifiers::Find(AxString name)
{
	for (int i = 0; i < this->list.count; i++)
		if (this->list[i]->name.Equals(name))
			return this->list[i];

	return 0;
}

AxScriptIdentifier *AxScriptIdentifiers::Add(AxString name, AxScriptDataType *dataType, int numElements)
{
	AxScriptIdentifier *identifier = new AxScriptIdentifier(name, dataType, this->size);
	this->size += dataType->size * numElements;
	return this->list.Add(identifier);
}

void AxScriptIdentifiers::Clear()
{
	for (int i = 0; i < this->list.count; i++)
		delete this->list[i];
	this->list.Clear();
}
