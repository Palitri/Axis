#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsPrimitiveTypes.h"

#include "AxFile3dsBaseObject.h"
#include "AxFile3dsKeyFrames.h"

class AxFile3dsHierarchyNode;

class AXDLLCLASS AxFile3dsHierarchy : 
	public AxList<AxFile3dsHierarchyNode*>
{
public:
	AxFile3dsHierarchyNode *parent;

	AxFile3dsHierarchy(AxFile3dsHierarchyNode *parent);
	~AxFile3dsHierarchy(void);
};

class AXDLLCLASS AxFile3dsHierarchyNode
{

public:
	AxFile3dsBaseObject *obj;
	AxFile3dsKeyFrameObject *keyFrames;
	AxFile3dsHierarchy *children;
	bool isDummy;

	AxFile3dsHierarchyNode(AxFile3dsBaseObject *obj, AxFile3dsKeyFrameObject *keyFrames);
	~AxFile3dsHierarchyNode(void);
};