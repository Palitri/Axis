//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxHierarchyStreamReader.h"


AxHierarchyStreamReader::AxHierarchyStreamReader(AxStream &stream)
{
    this->stream = &stream;
}


AxHierarchyStreamReader::~AxHierarchyStreamReader(void)
{
}

bool AxHierarchyStreamReader::ReadChunkHeader()
{
    while (this->chunkStack.count > 0)
        if (this->chunkStack.Peek() <= this->stream->position)
			this->stream->Seek(this->chunkStack.Pop());
        else
            break;

    if (this->stream->length - this->stream->position < 4 + 8)
        return false;

	this->chunkId = this->stream->ReadUInt32();
	this->chunkSize = this->stream->ReadUInt64();
    this->chunkStack.Push(this->stream->position + this->chunkSize);

    return true;
}

void AxHierarchyStreamReader::SkipChunk()
{
	while (this->chunkStack.count > 0)
        if (this->chunkStack.Peek() < this->stream->position)
            this->stream->Seek(this->chunkStack.Pop());
        else
            break;

	if (this->chunkStack.count > 0)
        this->stream->Seek(this->chunkStack.Pop());
}
