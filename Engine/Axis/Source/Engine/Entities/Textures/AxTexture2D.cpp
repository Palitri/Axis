//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxTexture2D.h"

#include "..\..\..\Tools\AxMem.h"

#include "..\..\..\Tools\Streams\AxSubStream.h"

#include "..\..\..\Media\Image\Native\AxNativeImage.h"

#include "..\..\Axis.h"

AxTexture2D::AxTexture2D(Axis *context)
	: AxResource()
{
	this->context = context;

	this->typeId = AxTexture2D::classId;

	this->resourceType = AxResourceType_Texture;

	this->deviceTexture = this->context->graphicsDevice->CreateTexture();

	this->width = 0;
	this->height = 0;

	this->properties.Add(new AxProperty(AxString("Width"), this->width));
	this->properties.Add(new AxProperty(AxString("Height"), this->height));
}

AxTexture2D::~AxTexture2D(void)
{
	delete this->deviceTexture;
}

void AxTexture2D::Load(void *data, int width, int height)
{
	this->width = width;
	this->height = height;

	this->properties[AxTexture2D::propertyIndex_Width]->GetEffectiveParameter()->SetInt(width);
	this->properties[AxTexture2D::propertyIndex_Height]->GetEffectiveParameter()->SetInt(height);

	this->deviceTexture->Update(data, width, height);
}
	
void AxTexture2D::Load(AxImage &sourceImage)
{
	if (this->context->graphicsDevice->GetPixelFormat().Equals(sourceImage.pixelFormat))
		this->Load(sourceImage.pixelData, sourceImage.width, sourceImage.height);
	else
	{
		AxImage deviceFormatImage(sourceImage, this->context->graphicsDevice->GetPixelFormat());
		this->Load(deviceFormatImage);
	}
}

bool AxTexture2D::Load(AxStream &source)
{
	AxImage *image = this->context->LoadImageX(source);
	if (image != 0)
	{
		image->SetPixelFormat(this->context->graphicsDevice->GetPixelFormat());
		this->Load(*image);
		delete image;

		return true;
	}

	return false;
}

bool AxTexture2D::Load(AxString &fileName)
{
	AxStream *file = this->context->fileSystem->OpenFile(fileName);

	if (file != 0)
	{
		bool result = this->Load(*file);
		delete file;

		if (result)
		{
			this->fileName = fileName;
			return true;
		}
	}

	return false;
}

AxImage *AxTexture2D::ToImage()
{
	AxPixelFormat pixelFormat = this->context->graphicsDevice->GetPixelFormat();

	AxImage *result = new AxImage();
	result->SetPixelFormat(pixelFormat);
	result->SetSize(this->width, this->height, 1);
	this->deviceTexture->GetData(result->pixelData);

	return result;
}

AxImage *AxTexture2D::Read()
{
	AxImage *result = new AxImage();
	result->pixelFormat = this->context->graphicsDevice->GetPixelFormat();
	result->SetSize(this->properties[AxTexture2D::propertyIndex_Width]->GetEffectiveParameter()->GetInt(), this->properties[AxTexture2D::propertyIndex_Height]->GetEffectiveParameter()->GetInt(), 1);
	this->deviceTexture->GetData(result->pixelData);

	return result;
}

void AxTexture2D::UpdateToProperties()
{
	int propertyWidth = this->properties[AxTexture2D::propertyIndex_Width]->GetEffectiveParameter()->GetInt();
	int propertyHeight = this->properties[AxTexture2D::propertyIndex_Height]->GetEffectiveParameter()->GetInt();
	if ((this->width != propertyWidth) || (this->height != propertyHeight))
	{
		this->width = propertyWidth;
		this->height = propertyHeight;

		this->deviceTexture->Update(0, this->width, this->height);
	}
}

void AxTexture2D::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxResource::SerializeChunks(writer);

	if (this->fileName.IsEmpty())
	{
		if (this->context->settings->properties[AxSettings::propertyIndex_EmbedGeneratedTextures]->GetBool())
		{
			AxImage *image = this->ToImage();
			AxNativeImage *nativeImage = new AxNativeImage(*image);
			
			writer.BeginChunk(AxTexture2D::SerializationId_TextureData);
			nativeImage->Save(*writer.stream);
			writer.EndChunk();

			delete nativeImage;
			delete image;
		}
	}
	else
	{
		if (this->context->settings->properties[AxSettings::propertyIndex_EmbedImportedTextures]->GetBool())
		{
			AxStream *file = this->context->fileSystem->OpenFile(this->fileName);

			if (this->context->settings->properties[AxSettings::propertyIndex_EmbeddedTexturesOriginalEncoding]->GetBool())
			{
				writer.BeginChunk(AxTexture2D::SerializationId_TextureData);
				writer.stream->WriteStreamData(*file);
				writer.EndChunk();
			}
			else
			{
				AxImage *image = this->context->LoadImageX(*file);
				AxNativeImage *nativeImage = new AxNativeImage(*image);
			
				writer.BeginChunk(AxTexture2D::SerializationId_TextureData);
				nativeImage->Save(*writer.stream);
				writer.EndChunk();

				delete nativeImage;
				delete image;
			}

			delete file;
		}
		else
		{
			writer.BeginChunk(AxTexture2D::SerializationId_FileName);
			writer.stream->WriteString(AxFileSystem::GetRelativeFilePath(this->context->serializationParameters.rootDir, this->fileName));
			writer.EndChunk();
		}
	}
}

bool AxTexture2D::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxTexture2D::SerializationId_FileName:
		{
			AxString fileName = reader.stream->ReadString();
			this->Load(AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, fileName));
			break;
		}

		case AxTexture2D::SerializationId_TextureData:
		{
			AxSubStream *imageStream = new AxSubStream(*reader.stream, reader.chunkSize);
			bool success = this->Load(*imageStream);
			delete imageStream;

			if (!success)
				return false;

			reader.stream->Seek(reader.chunkSize, StreamSeekMode_Relative);
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
