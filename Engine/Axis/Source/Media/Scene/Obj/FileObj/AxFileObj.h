#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\Streams\AxStream.h"
#include "..\..\..\..\Tools\AxList.h"
#include "..\..\..\..\Tools\AxStrings.h"
#include "..\..\..\..\Tools\AxDictionary.h"

#include "AxFileObjMesh.h"
#include "AxFileObjMaterial.h"


struct AxFileObjArgument
{
	int start, end;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjArgument>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjMesh*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFileObjMaterial*>;

class AXDLLCLASS AxFileObj
{
private:
	// To avoid slow string processing, read all the file contents into one data buffer and process each separate file token directly from this buffer
	char* data;
	int dataSize, dataPos;
	AxList<AxFileObjArgument> arguments;
	int argumentsCount;

	bool ParseLine();

	bool ArgEquals(int argIndex, const char *str);
	bool ArgEquals(int arg1Index, AxFileObjArgument *arg2);
	bool DataEquals(int start, int end, const char *str);
	bool DataToInt(int pos, int &value);
	bool DataToFloat(int pos, float &value);
	int DataCharIndex(int start, int end, const char character);
	AxString DataCreateString(int startArgIndex, int endArgIndex);

	bool ReadTextureMap(AxFileObjTextureMap * texture);

public:
	AxList<AxFileObjMesh*> meshes;
	AxList<AxFileObjMaterial*> materials;
	AxStrings materialFiles;

	AxFileObj(void);
	~AxFileObj(void);

	bool Read(AxStream *dataStream);
};
