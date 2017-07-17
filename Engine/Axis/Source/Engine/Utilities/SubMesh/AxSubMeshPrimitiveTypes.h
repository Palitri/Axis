#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxList.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

struct AxSubMeshFace
{
	AxList<int> indices;
};