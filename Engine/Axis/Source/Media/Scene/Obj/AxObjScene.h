#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxScene.h"

#include "..\..\..\Engine\Axis.h"

#include "FileObj\AxFileObjMesh.h"
#include "FileObj\AxFileObjMaterial.h"

class AXDLLCLASS AxObjScene :
	public AxScene
{
private:
	AxMesh* LoadMeshResource(AxFileObjMesh* meshObj);
	AxMaterial* LoadMaterialResource(AxFileObjMaterial *materialObj);

public:
	AxObjScene(void);
	~AxObjScene(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

