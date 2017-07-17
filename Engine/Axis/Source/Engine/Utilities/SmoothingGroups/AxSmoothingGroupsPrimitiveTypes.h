#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxList.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

// Holds all the indices that derive from a given original vertex and all their smooth groups accumulated
struct AxSmoothingGroupsVertexInfo
{
    unsigned long long accumulatedSmoothingGroups;
    AxList<int> indices;
};

// Holds the smoothing groups of a face and the indices of its vertices
struct AxSmoothingGroupsFace
{
    unsigned long long smoothingGroups;
	AxList<int> indices;
};

// Holds the smooth groups of a vetex
struct AxSmoothingGroupsVertex
{
    unsigned long long accumulatedSmoothingGroups;
    int index;
};
