//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeImage.h"

#include "..\..\..\Engine\Utilities\Serialization\AxHierarchyStreamReader.h"
#include "..\..\..\Engine\Utilities\Serialization\AxHierarchyStreamWriter.h"
#include "..\..\..\Engine\Utilities\Serialization\AxSerializationUtils.h"

const char *AxNativeImage::formatName = "axi";

AxNativeImage::AxNativeImage(void)
	: AxImage()
{
}

AxNativeImage::AxNativeImage(AxImage &source)
	: AxImage(source)
{
}

AxNativeImage::AxNativeImage(AxImage &source, AxPixelFormat &pixelFormat)
	: AxImage(source, pixelFormat)
{
}


AxNativeImage::~AxNativeImage(void)
{
}

bool AxNativeImage::Load(AxStream &source) 
{
	AxHierarchyStreamReader reader(source);

	long long mainChunkEnd = -1;

    while (reader.ReadChunkHeader())
	{
		switch (reader.chunkId)
		{
			case AxNativeImage::SerializationId_Main:
			{
				mainChunkEnd = reader.stream->position + reader.chunkSize;
				break;
			}

			case AxNativeImage::SerializationId_Size:
			{
				int width = reader.stream->ReadInt32();
				int height = reader.stream->ReadInt32();
				int depth = reader.stream->ReadInt32();

				this->SetSize(width, height, depth);

				break;
			}

			case AxNativeImage::SerializationId_Format:
			{
				this->pixelFormat = AxSerializationUtils::DeserializePixelFormat(source);

				this->SetPixelFormat(this->pixelFormat, false);

				break;
			}

			case AxNativeImage::SerializationId_Data:
			{
				int pixelDataSize = this->width * this->height * this->depth * this->pixelFormat.sizeInBytes;
				if (pixelDataSize > reader.chunkSize)
					pixelDataSize = reader.chunkSize;

				if (pixelDataSize > 0)
					reader.stream->ReadData(this->pixelData, pixelDataSize);

				break;
			}

			default:
			{
				if ((reader.chunkSize < 0) || (reader.stream->position + reader.chunkSize > mainChunkEnd))
					return false;

				reader.SkipChunk();

				break;
			}
		}

		if (reader.stream->position >= mainChunkEnd)
			break;
	}

	return true;
}

bool AxNativeImage::Save(AxStream &destination, const char *format)
{
	if (format != 0)
	{
		AxString encodingCode = AxString(format).Trim('.');
		if ((encodingCode.length != 0) && (!encodingCode.Equals(AxNativeImage::formatName)))
			return false;
	}

	AxHierarchyStreamWriter writer(destination);

	writer.BeginChunk(AxNativeImage::SerializationId_Main);

	writer.BeginChunk(AxNativeImage::SerializationId_Size);
	writer.stream->WriteInt32(this->width);
	writer.stream->WriteInt32(this->height);
	writer.stream->WriteInt32(this->depth);
	writer.EndChunk();

	writer.BeginChunk(AxNativeImage::SerializationId_Format);
	AxSerializationUtils::SerializePixelFormat(destination, this->pixelFormat);
	writer.EndChunk();

	writer.BeginChunk(AxNativeImage::SerializationId_Data);
	writer.stream->WriteData(this->pixelData, this->width * this->height * this->depth * this->pixelFormat.sizeInBytes);
	writer.EndChunk();

	writer.EndChunk();

	return true;
}
