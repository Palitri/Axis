#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

#include "..\..\..\Graphics\AxDeviceTexture2D.h"

#include "..\..\..\Media\Image\AxImage.h"
#include "..\..\..\Tools\Streams\AxStream.h"
#include "..\..\..\Tools\AxString.h"

class Axis;

class AXDLLCLASS AxTexture2D :
	public AxResource
{
private:
	static const unsigned int SerializationId_FileName		= 0x21111006;
	static const unsigned int SerializationId_TextureData	= 0x21112006;

	// held only for checking if the respective properties have changed value, in which case texture should be resized
	int width, height;

public:
	static const int classId = (AxResourceType_Texture << 16) | 0;

	static const int propertyIndex_Width	= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_Height	= AxResource::propertyIndex_ChildPropertiesIndex + 1;

	Axis *context;
	AxDeviceTexture2D *deviceTexture;
	AxString fileName;

	AxTexture2D(Axis *context);
	virtual ~AxTexture2D(void);

	void Load(void *data, int width, int height);
	void Load(AxImage &sourceImage);
	bool Load(AxStream &source);
	bool Load(AxString &fileName);

	AxImage *ToImage();

	AxImage *Read();

	void UpdateToProperties();

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

