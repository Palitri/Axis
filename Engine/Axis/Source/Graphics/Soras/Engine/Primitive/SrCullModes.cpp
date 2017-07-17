//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "SrCullModes.h"

#include "..\Soras.h"

bool SrCullModes::None(Soras *soras)
{
    return true;
}
		
bool SrCullModes::CounterClockWise(Soras *soras)
{
    int *index = &soras->mesh->indices[soras->cull_PrimitiveIndex];

	SrVertex *v0 = &soras->mesh->transformedVertices[*index++];
	SrVertex *v1 = &soras->mesh->transformedVertices[*index++];
	SrVertex *v2 = &soras->mesh->transformedVertices[*index];

    return ((v1->positionIntScreen.x - v0->positionIntScreen.x) *
            (v2->positionIntScreen.y - v0->positionIntScreen.y) -
            (v2->positionIntScreen.x - v0->positionIntScreen.x) *
            (v1->positionIntScreen.y - v0->positionIntScreen.y)) < 0;
}

bool SrCullModes::ClockWise(Soras *soras)
{
    int *index = &soras->mesh->indices[soras->cull_PrimitiveIndex];

	SrVertex *v0 = &soras->mesh->transformedVertices[*index++];
	SrVertex *v1 = &soras->mesh->transformedVertices[*index++];
	SrVertex *v2 = &soras->mesh->transformedVertices[*index];

    return ((v1->positionIntScreen.x - v0->positionIntScreen.x) *
            (v2->positionIntScreen.y - v0->positionIntScreen.y) -
            (v2->positionIntScreen.x - v0->positionIntScreen.x) *
            (v1->positionIntScreen.y - v0->positionIntScreen.y)) > 0;
}
