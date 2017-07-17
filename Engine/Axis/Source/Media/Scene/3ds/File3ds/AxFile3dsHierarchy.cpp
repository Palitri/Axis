//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3dsHierarchy.h"


AxFile3dsHierarchy::AxFile3dsHierarchy(AxFile3dsHierarchyNode *parent)
{
	this->parent = parent;
}


AxFile3dsHierarchy::~AxFile3dsHierarchy(void)
{
	for (int i = 0; i < this->count; i++)
		delete (*this)[i];
}

AxFile3dsHierarchyNode::AxFile3dsHierarchyNode(AxFile3dsBaseObject *obj, AxFile3dsKeyFrameObject *keyFrames)
{
	this->obj = obj;
	this->keyFrames = keyFrames;
	if (this->keyFrames != 0)
		this->isDummy = this->keyFrames->name.Equals("$$$DUMMY");
	else
		this->isDummy = false;

	this->children = new AxFile3dsHierarchy(this);
}

AxFile3dsHierarchyNode::~AxFile3dsHierarchyNode(void)
{
	delete this->children;
}

