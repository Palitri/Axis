#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\Base\AxResource.h"

#include "..\..\..\Tools\Streams\AxMemoryStream.h"

class Axis;

class AXDLLCLASS AxScript :
	public AxResource
{
private:
	static const unsigned int SerializationId_SourceCode	= 0x2111100d;
	static const unsigned int SerializationId_CompiledCode	= 0x2111200d;

public:
	static const int classId = (AxResourceType_Script << 16) | 0;

	Axis *context;

	long long entryPoint;
	AxString errorCode;
	AxString sourceCode;
	AxMemoryStream compiledCode;

	bool successfullyCompiled;

	AxScript(Axis *context);
	virtual ~AxScript(void);

	bool CompileCode();
	bool RunCode();

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

