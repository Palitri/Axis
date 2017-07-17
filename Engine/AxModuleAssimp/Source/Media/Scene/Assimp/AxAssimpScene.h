#pragma once

#include "..\..\..\AxisEngine.h"

#include <Assimp\Importer.hpp>
#include <Assimp\scene.h>
#include <Assimp\mesh.h>
#include <Assimp\postprocess.h>

class AxAssimpScene :
	public AxScene
{
private:
	static void AiToAxMatrix(AxMatrix &result, aiMatrix4x4 &aiMatrix);
	static AxString AiToAxString(aiString &str);

	AxDictionary<AxString, void*> aiMeshesMap;

	Assimp::Importer *aiImporter;
	const aiScene *aiScene;

	void AxAssimpScene::LoadMeshData(aiMesh *aiMesh);
	void AxAssimpScene::LoadNode(aiNode *aiNode);

public:
	AxAssimpScene(void);
	~AxAssimpScene(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

