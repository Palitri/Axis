#pragma once

#include "..\..\AxGlobals.h"

enum AxResourceType
{
	AxResourceType_None = 0,
	AxResourceType_Camera = 1,
    AxResourceType_Transform = 2,
    AxResourceType_Material = 3,
    AxResourceType_Mesh = 4,
    AxResourceType_EntitySet = 5,
    AxResourceType_Texture = 6, 
    AxResourceType_RenderTarget = 7,
    AxResourceType_Mechanism = 8,
    AxResourceType_Light = 9,
	AxResourceType_Sound = 10,
	AxResourceType_SoundEmitter = 11,
	AxResourceType_SoundReceptor = 12,
	AxResourceType_Script = 13,
	AxResourceType_Settings = 14,
};
