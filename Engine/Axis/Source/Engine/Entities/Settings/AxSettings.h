#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

class AXDLLCLASS AxSettings :
	public AxResource
{
public:
	static const int classId = (AxResourceType_Settings << 16) | 0;

	static const int propertyIndex_ClearScreen						= 0;
	static const int propertyIndex_ClearScreenColor					= 1;
	static const int propertyIndex_AmbientLight						= 2;
	static const int propertyIndex_Thumbnail						= 3;
	static const int propertyIndex_ThumbnailSize					= 4;
	static const int propertyIndex_EmbedGeneratedMeshes				= 5;
	static const int propertyIndex_EmbedGeneratedTextures			= 6;
	static const int propertyIndex_EmbedImportedTextures			= 7;
	static const int propertyIndex_EmbeddedTexturesOriginalEncoding	= 8;

	AxSettings(void);
	virtual ~AxSettings(void);
};

