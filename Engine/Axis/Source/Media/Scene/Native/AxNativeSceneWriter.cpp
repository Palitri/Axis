//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeSceneWriter.h"

#include "..\..\..\Engine\Axis.h"

#include "AxNativeSceneID.h"

#include "..\..\..\Engine\Utilities\Serialization\AxHierarchyStreamWriter.h"

#include "..\..\Image\Native\AxNativeImage.h"

AxNativeSceneWriter::AxNativeSceneWriter(Axis *context)
{
    this->context = context;
}


AxNativeSceneWriter::~AxNativeSceneWriter(void)
{
}

void AxNativeSceneWriter::Write(AxStream &destination)
{
    AxHierarchyStreamWriter writer(destination);
	
	writer.BeginChunk(AxNativeSceneID::Main);

	writer.BeginChunk(AxNativeSceneID::Header);

	writer.BeginChunk(AxNativeSceneID::Header_String);
	writer.stream->WriteString(AxString("Axis"));
	writer.EndChunk();

	writer.BeginChunk(AxNativeSceneID::Header_Ver);
	writer.stream->WriteUInt32(Axis::VersionSignature);
	writer.EndChunk();

	writer.EndChunk();
	// Header


	writer.BeginChunk(AxNativeSceneID::Resources);

	for (int i = 0; i < this->context->resources.count; i++)
	{
		writer.BeginChunk(AxNativeSceneID::ResourceSerializationWrap);
		this->context->SerializeResource(&destination, this->context->resources[i]);
		writer.EndChunk();
	}

	writer.EndChunk();
	// Resources


	writer.BeginChunk(AxNativeSceneID::RootBranch);
	writer.stream->WriteInt64(this->context->root->id);
	writer.EndChunk();
	// Root Branch


	writer.BeginChunk(AxNativeSceneID::RuntimeParameters);
	writer.BeginChunk(AxNativeSceneID::ResourceSerializationWrap);
	this->context->SerializeResource(&destination, this->context->settings);
	writer.EndChunk();
	writer.EndChunk();
	// RuntimeParameters


	if (context->settings->properties[AxSettings::propertyIndex_Thumbnail]->GetBool())
	{
		int thumbnailSize = context->settings->properties[AxSettings::propertyIndex_ThumbnailSize]->GetInt();

		AxImage *thumbnail = this->context->TakeScreenshot(thumbnailSize, thumbnailSize);
		if (thumbnail != 0)
		{
			AxNativeImage *thumbnailNative = new AxNativeImage(*thumbnail);

			writer.BeginChunk(AxNativeSceneID::Thumbnail);
			thumbnailNative->Save(*writer.stream, AxNativeImage::formatName);
			writer.EndChunk();

			delete thumbnailNative;
			delete thumbnail;
		}
	}
	// Thumbnail

	writer.EndChunk();
	// Main
}
