//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScript.h"

#include "..\..\Axis.h"

AxScript::AxScript(Axis *context)
	: AxResource()
{
	this->typeId = AxMesh::classId;
	this->resourceType = AxResourceType_Script;

	this->context = context;
        
	this->successfullyCompiled = false;
}


AxScript::~AxScript(void)
{
}

bool AxScript::CompileCode()
{
	this->successfullyCompiled = this->context->scriptBuilder.Build(&this->compiledCode, this->sourceCode);
	
	if (this->successfullyCompiled)
		this->entryPoint = this->compiledCode.position;
	else
		this->errorCode = this->context->scriptBuilder.tokens[this->context->scriptBuilder.tokenIndex].name;

	return this->successfullyCompiled;
}

bool AxScript::RunCode()
{
	if (!this->successfullyCompiled)
		return false;
	
	this->compiledCode.Seek(this->entryPoint);
	this->context->scriptProcessor.SetMemory(&this->compiledCode);
	this->context->scriptProcessor.Process();

	return true;
}

void AxScript::SerializeChunks(AxHierarchyStreamWriter &writer)
{
	AxResource::SerializeChunks(writer);

	writer.BeginChunk(AxScript::SerializationId_SourceCode);
	writer.stream->WriteString(this->sourceCode);
	writer.EndChunk();

	if (this->successfullyCompiled)
	{
		writer.BeginChunk(AxScript::SerializationId_CompiledCode);
		writer.stream->WriteInt64(this->compiledCode.length);
		long long compiledCodePos = this->compiledCode.position;
		this->compiledCode.Seek(0);
		writer.stream->WriteStreamData(this->compiledCode);
		this->compiledCode.Seek(compiledCodePos);
		writer.stream->WriteInt64(this->entryPoint);
		writer.EndChunk();
	}
}

bool AxScript::DeserializeChunk(AxHierarchyStreamReader &reader)
{
	if (AxResource::DeserializeChunk(reader))
		return true;

	switch (reader.chunkId)
	{
		case AxScript::SerializationId_SourceCode:
		{
			this->sourceCode = reader.stream->ReadString();
			
			if (!this->successfullyCompiled)
				this->CompileCode();

			break;
		}

		case AxScript::SerializationId_CompiledCode:
		{
			long long compiledCodeLength = reader.stream->ReadInt64();
			this->compiledCode.Resize(0);
			this->compiledCode.WriteStreamData(*reader.stream, compiledCodeLength);
			this->entryPoint = reader.stream->ReadInt64();
			this->successfullyCompiled = true;
			break;
		}

		default:
		{
			return false;
		}
	}

	return true;
}
