//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxAssimpScene.h"

void AxAssimpScene::AiToAxMatrix(AxMatrix &result, aiMatrix4x4 &aiMatrix)
{
	result._11 = aiMatrix.a1;
	result._12 = aiMatrix.a2;
	result._13 = aiMatrix.a3;
	result._14 = aiMatrix.a4;
	result._21 = aiMatrix.b1;
	result._22 = aiMatrix.b2;
	result._23 = aiMatrix.b3;
	result._24 = aiMatrix.b4;
	result._31 = aiMatrix.c1;
	result._32 = aiMatrix.c2;
	result._33 = aiMatrix.c3;
	result._34 = aiMatrix.c4;
	result._41 = aiMatrix.d1;
	result._42 = aiMatrix.d2;
	result._43 = aiMatrix.d3;
	result._44 = aiMatrix.d4;
}

AxString AxAssimpScene::AiToAxString(aiString &str)
{
	return AxString(str.data);
}

AxAssimpScene::AxAssimpScene(void)
{
}


AxAssimpScene::~AxAssimpScene(void)
{
}

void AxAssimpScene::LoadMeshData(aiMesh *aiMesh)
{
	AxMesh *mesh = new AxMesh(this->context);
	AxDeviceMesh *deviceMesh = mesh->deviceMesh;

	deviceMesh->CreateIndexBuffer(aiMesh->mNumFaces * 3);
	deviceMesh->CreateVertexBuffer(aiMesh->mNumVertices);

	int index = 0;
	for (unsigned int faceIndex = 0; faceIndex < aiMesh->mNumFaces; faceIndex++)
	{
		deviceMesh->SetIndex(index++, aiMesh->mFaces[faceIndex].mIndices[0]);
		deviceMesh->SetIndex(index++, aiMesh->mFaces[faceIndex].mIndices[1]);
		deviceMesh->SetIndex(index++, aiMesh->mFaces[faceIndex].mIndices[2]);
	}

	if (aiMesh->mVertices != 0)
	{
		for (unsigned int vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; vertexIndex++)
		{
			aiVector3D *aiPos = &aiMesh->mVertices[vertexIndex];
			deviceMesh->SetVertexPosition(vertexIndex, AxVector3(aiPos->x, aiPos->y, aiPos->z));
		}

		if (aiMesh->mTextureCoords != 0)
		{
			for (unsigned int vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; vertexIndex++)
			{
				aiVector3D *aiTex = &aiMesh->mTextureCoords[0][vertexIndex];
				deviceMesh->SetVertexTexCoords(vertexIndex, AxVector2(aiTex->x, aiTex->y));
			}
		}

		if (aiMesh->mNormals != 0)
		{
			for (unsigned int vertexIndex = 0; vertexIndex < aiMesh->mNumVertices; vertexIndex++)
			{
				aiVector3D *aiNorm = &aiMesh->mNormals[vertexIndex];
				deviceMesh->SetVertexNormal(vertexIndex, AxVector3(aiNorm->x, aiNorm->y, aiNorm->z));
			}
		}
	}

	for (unsigned int boneIndex = 0; boneIndex < aiMesh->mNumBones; boneIndex++)
	{
		aiBone *aiBone = aiMesh->mBones[boneIndex];

		for (unsigned int weightIndex = 0; weightIndex < aiBone->mNumWeights; weightIndex++)
		{
			int vertexIndex = aiBone->mWeights[weightIndex].mVertexId;
			float boneWeight = aiBone->mWeights[weightIndex].mWeight;

			AxVector4 boneIndices, boneWeights;
			deviceMesh->GetVertexBones(vertexIndex, boneIndices, boneWeights);
				
			if (boneWeights.x == 0)
			{
				boneIndices.x = (float)vertexIndex;
				boneWeights.x = boneWeight;
			}
			else if (boneWeights.y == 0)
			{
				boneIndices.y = (float)vertexIndex;
				boneWeights.y = boneWeight;
			}
			else if (boneWeights.z == 0)
			{
				boneIndices.z = (float)vertexIndex;
				boneWeights.z = boneWeight;
			}
			else if (boneWeights.w == 0)
			{
				boneIndices.w = (float)vertexIndex;
				boneWeights.w = boneWeight;
			}

			deviceMesh->SetVertexBones(vertexIndex, boneIndices, boneWeights);
		}

		//AxTransform *bone = (AxTransform*)this->context->AddResource(*new AxTransform());
		//bone->name = aiBone->mName.data;
		//AxAssimpScene::AiToAxMatrix(bone->manual, aiBone->mOffsetMatrix);

		//aiMesh->mName
		//aiMesh->mMaterialIndex
	}

	deviceMesh->UpdateVertices(0, deviceMesh->GetVertexCount());
	deviceMesh->UpdateIndices(0, deviceMesh->GetIndexCount());

	this->aiMeshesMap.Add(AxAssimpScene::AiToAxString(aiMesh->mName), mesh);
}

void AxAssimpScene::LoadNode(aiNode *aiNode)
{
	AxEntitySet *entitySet = (AxEntitySet*)this->context->AddResource(new AxEntitySet());

	AxTransform *transform = (AxTransform*)this->context->AddResource(new AxTransform());
	entitySet->references.Add(transform);

	void *meshRef;
	if (this->aiMeshesMap.GetValue(AxAssimpScene::AiToAxString(aiNode->mName), meshRef))
	{
		entitySet->references.Add((AxMesh*)meshRef);
	}

	if (aiNode->mChildren != 0)
	{
		for (unsigned int childIndex = 0; childIndex < aiNode->mNumChildren; childIndex++)
			this->LoadNode(aiNode->mChildren[childIndex]);
	}
}

bool AxAssimpScene::Load(AxStream &source)
{
	this->aiMeshesMap.Clear();

	this->aiImporter = new Assimp::Importer();

	char *fileContents = new char[(unsigned int)source.length];
	source.ReadData(fileContents, source.length);

	this->aiScene = this->aiImporter->ReadFileFromMemory(fileContents, (unsigned int)source.length, aiProcess_Triangulate| aiProcess_GenSmoothNormals | aiProcess_FlipUVs);

	if (this->aiScene == 0) 
	{
		delete this->aiImporter;
		return false;
	}

	// Load mesh resources
	for (unsigned int meshIndex = 0; meshIndex < this->aiScene->mNumMeshes; meshIndex++)
	{
		this->LoadMeshData(this->aiScene->mMeshes[meshIndex]);
	}

	// Load scene structure
	this->LoadNode(this->aiScene->mRootNode);

	return false;
}

bool AxAssimpScene::Save(AxStream &destination, const char *format)
{
	return false;
}
