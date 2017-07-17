//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxObjScene.h"

#include "FileObj\AxFileObj.h"

#include "..\..\..\Engine\Utilities\SmoothingGroups\AxSmoothingGroups.h"

AxObjScene::AxObjScene(void)
{
}


AxObjScene::~AxObjScene(void)
{
}

bool AxObjScene::Load(AxStream &source)
{
	AxFileObj fileObj;

	AxDictionary<AxString, AxResource*> resourceMapping;

	if (!fileObj.Read(&source))
		return false;

	for (int i = 0; i < fileObj.materialFiles.count; i++)
	{
		AxStream *materialFile = this->context->fileSystem->OpenFile(AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, fileObj.materialFiles[i]));
		if (materialFile != 0)
			fileObj.Read(materialFile);
		delete materialFile;
	}

	for (int materialIndex = 0; materialIndex < fileObj.materials.count; materialIndex++)
	{
		AxMaterial *material = this->LoadMaterialResource(fileObj.materials[materialIndex]);
		resourceMapping.Add(fileObj.materials[materialIndex]->name, material);
	}

	if (fileObj.meshes.count > 0)
	{
		AxEntitySet *parentSet = (AxEntitySet*)((AxEntitySet*)this->context->root)->references.Add(this->context->AddResource(new AxEntitySet()));
		parentSet->name = this->context->AcquireResourceName(this->context->serializationParameters.friendlyName, parentSet->resourceType);

		AxResource *lastMeshMaterial = 0;
		for (int meshIndex = 0; meshIndex < fileObj.meshes.count; meshIndex++)
		{
			AxMesh *mesh = this->LoadMeshResource(fileObj.meshes[meshIndex]);
	
			AxResource *meshMaterial;
			if (resourceMapping.GetValue(fileObj.meshes[meshIndex]->materialName, meshMaterial))
			{
				if (meshMaterial != lastMeshMaterial)
				{
					parentSet->references.Add(meshMaterial);
					lastMeshMaterial = meshMaterial;
				}
			}

			parentSet->references.Add(mesh);
		}
	}

	return true;
}

bool AxObjScene::Save(AxStream &destination, const char *format)
{
	return false;
}

AxMesh *AxObjScene::LoadMeshResource(AxFileObjMesh* meshObj)
{
	AxMesh *mesh = (AxMesh*)this->context->AddResource(new AxMesh(this->context));
	mesh->name = this->context->AcquireResourceName(meshObj->name, mesh->resourceType);

	meshObj->CreateBuffers();

	AxSmoothingGroups smoothGrouping(meshObj->bufferedFaces.count, meshObj->bufferedVertices.count);
	for (int faceIndex = 0; faceIndex < meshObj->faces.count; faceIndex++)
	{
		smoothGrouping.SetFaceSmoothingGroups(faceIndex, meshObj->bufferedFaces.Get(faceIndex)->smoothingGroups);
		for (int faceVertexIndex = 0; faceVertexIndex < meshObj->faces[faceIndex]->vertices.count; faceVertexIndex++)
			smoothGrouping.AddFaceVertex(faceIndex, meshObj->bufferedFaces.Get(faceIndex)->bufferedVertexIndices.Get(faceVertexIndex));
	}

	int numVertices = smoothGrouping.vertexIndices.count;
	mesh->deviceMesh->CreateVertexBuffer(numVertices);
	for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		
		AxFileObjFaceVertex *vertexObj = &meshObj->bufferedVertices[smoothGrouping.vertexIndices[vertexIndex].index];

		if (vertexObj->vertexIndex != -1)
			mesh->deviceMesh->SetVertexPosition(vertexIndex, AxVector3(meshObj->vertices[vertexObj->vertexIndex].x, meshObj->vertices[vertexObj->vertexIndex].y, meshObj->vertices[vertexObj->vertexIndex].z));

		if (vertexObj->texCoordIndex != -1)
			mesh->deviceMesh->SetVertexTexCoords(vertexIndex, AxVector2(meshObj->texCoords[vertexObj->texCoordIndex].x, meshObj->texCoords[vertexObj->texCoordIndex].y));

		if (vertexObj->normalIndex != -1)
			mesh->deviceMesh->SetVertexNormal(vertexIndex, AxVector3(meshObj->normals[vertexObj->normalIndex].x, meshObj->normals[vertexObj->normalIndex].y, meshObj->normals[vertexObj->normalIndex].z));
	}


	// Calculate the total number of triangles, given each face consists of (number of face vertices - 2) triangles, that is 1 for triangle, 2 for rectangle, 3 for pentagon and so on
	int numTriangles = 0;
	for (int i = 0; i < smoothGrouping.faces.count; i++)
		numTriangles += smoothGrouping.faces[i].indices.count - 2;

	mesh->deviceMesh->CreateIndexBuffer(numTriangles * 3);
	int index = 0;
	for (int faceIndex = 0; faceIndex < smoothGrouping.faces.count; faceIndex++)
	{
		// In case of convex polygons with any number of sides, use the first vertex to form a triangle fan with the rest vertices
		
		int zeroVertexIndex = smoothGrouping.faces[faceIndex].indices[0];
		for (int faceVertexIndex = 2; faceVertexIndex < smoothGrouping.faces[faceIndex].indices.count; faceVertexIndex++)
		{
			mesh->deviceMesh->SetIndex(index++, zeroVertexIndex);
			mesh->deviceMesh->SetIndex(index++, smoothGrouping.faces[faceIndex].indices[faceVertexIndex]);
			mesh->deviceMesh->SetIndex(index++, smoothGrouping.faces[faceIndex].indices[faceVertexIndex - 1]);
		}
	}

	mesh->ComputeNormals();
	mesh->ComputeBounds();

	mesh->deviceMesh->UpdateVertices(0, numVertices);
	mesh->deviceMesh->UpdateIndices(0, numTriangles * 3);

	return mesh;
}

AxMaterial *AxObjScene::LoadMaterialResource(AxFileObjMaterial *materialObj)
{
	AxMaterial *material = (AxMaterial*)this->context->AddResource(new AxMaterial(this->context));
	material->name = this->context->AcquireResourceName(materialObj->name, material->resourceType);

    material->AddShadingLayer(AxShadingOperation_Material, AxColorOperation_None);

	material->GetProperty("Ambient")->SetColor(AxVector4(materialObj->ambientColor.x,
														 materialObj->ambientColor.y,
														 materialObj->ambientColor.z,
														 1.0f));
    material->GetProperty("Diffuse")->SetColor(AxVector4(materialObj->diffuseColor.x,
														 materialObj->diffuseColor.y,
														 materialObj->diffuseColor.z,
														 1.0f));
	material->GetProperty("Specular")->SetColor(AxVector4(materialObj->specularColor.x,
														  materialObj->specularColor.y,
														  materialObj->specularColor.z,
														  1.0f));
	material->GetProperty("Gloss")->SetFloat(materialObj->specularFactor);

	material->AddShadingLayer(AxShadingOperation_AmbientLighting, AxColorOperation_Set);

	if (materialObj->bumpMap != 0)
    {
		AxString fileName;
		if (materialObj->bumpMap->fileName.IndexOf(':') != -1)
			fileName = materialObj->bumpMap->fileName;
		else
			fileName = AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, materialObj->bumpMap->fileName);

		AxTexture2D *texture = this->context->AcquireTexture(fileName);

        material->AddShadingLayer(AxShadingOperation_NormalMap, AxColorOperation_None);
		material->AddShadingLayer(AxShadingOperation_PixelLighting, AxColorOperation_Add);

		material->GetProperty("Normal map")->SetValue(texture, AxParameterType_ReferenceTexture);
    }
    else
        material->AddShadingLayer(AxShadingOperation_VertexLighting, AxColorOperation_Add);


	if (materialObj->diffuseMap != 0)
    {
		AxString fileName;
		if (materialObj->diffuseMap->fileName.IndexOf(':') != -1)
			fileName = materialObj->diffuseMap->fileName;
		else
			fileName = AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, materialObj->diffuseMap->fileName);

		AxTexture2D *texture = this->context->AcquireTexture(fileName);

        material->AddShadingLayer(AxShadingOperation_ColorMap, AxColorOperation_Modulate);

        material->GetProperty("Color map")->SetValue(texture, AxParameterType_ReferenceTexture);
    }

	if (materialObj->reflectionMap != 0)
    {
		AxString fileName;
		if (materialObj->reflectionMap->fileName.IndexOf(':') != -1)
			fileName = materialObj->reflectionMap->fileName;
		else
			fileName = AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, materialObj->reflectionMap->fileName);

		AxTexture2D *texture = this->context->AcquireTexture(fileName);

        material->AddShadingLayer(AxShadingOperation_ReflectionMap, AxColorOperation_Add);

        material->GetProperty("Reflection map")->SetValue(texture, AxParameterType_ReferenceTexture);
    }

	material->UpdateShader();

	return material;
}
