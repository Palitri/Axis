//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxHierarchyStreamWriter.h"


AxHierarchyStreamWriter::AxHierarchyStreamWriter(AxStream &stream)
{
    this->stream = &stream;
}


AxHierarchyStreamWriter::~AxHierarchyStreamWriter(void)
{
}

void AxHierarchyStreamWriter::BeginChunk(unsigned int chunkId)
{
	this->stream->WriteUInt32(chunkId);
	this->chunkStack.Push(this->stream->position);
	this->stream->WriteUInt64(0);
}

void AxHierarchyStreamWriter::EndChunk()
{
	long long currentPos = this->stream->position;
	this->stream->Seek(this->chunkStack.Pop());
	this->stream->WriteUInt64(currentPos - this->stream->position - 8);
	this->stream->Seek(currentPos);
}