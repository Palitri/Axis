//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "Ax3dsScene.h"

#include "..\..\..\Engine\Axis.h"

#include "..\..\..\Engine\Utilities\AxMaths.h"
#include "..\..\..\Engine\Utilities\SmoothingGroups\AxSmoothingGroups.h"
#include "..\..\..\Tools\AxMath.h"

#include "..\..\..\Engine\Entities\Mechanisms\AxKeyframeAnimationMechanism.h"
#include "..\..\..\Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameScaling.h"
#include "..\..\..\Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameRotationAxis.h"
#include "..\..\..\Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameTranslation.h"

Ax3dsScene::Ax3dsScene(void)
{
}


Ax3dsScene::~Ax3dsScene(void)
{
	for (int i = 0; i < this->materialMapping.count; i++)
		delete this->materialMapping[i].name3ds;

	for (int i = 0; i < this->resourceMapping.count; i++)
		delete this->resourceMapping[i].name3ds;
}

bool Ax3dsScene::Load(AxStream &source)
{
	AxFile3ds file3ds;
	if (!file3ds.Read(&source))
		return false;

    for (int materialIndex = 0; materialIndex < file3ds.materials.count; materialIndex++)
    {
		this->LoadMaterialResource(file3ds.materials[materialIndex]);
	}

	for (int objectIndex = 0; objectIndex < file3ds.objects.count; objectIndex++)
	{
		if (file3ds.objects[objectIndex]->objectType == AxFile3dsObjectType_Mesh)
			this->LoadMeshResource((AxFile3dsMesh*)file3ds.objects[objectIndex]);

		if (file3ds.objects[objectIndex]->objectType == AxFile3dsObjectType_Camera)
			this->LoadCameraResource((AxFile3dsCamera*)file3ds.objects[objectIndex]);

		if (file3ds.objects[objectIndex]->objectType == AxFile3dsObjectType_Light)
			this->LoadLightResource((AxFile3dsLight*)file3ds.objects[objectIndex]);
	}

	this->BuildSceneHierarchy(this->context->root, file3ds.hierarchy);

	for (int i = 0; i < this->context->resources.count; i++)
	{
		AxResource *resource = this->context->resources[i];
		if (dynamic_cast<AxMaterial*>(resource) != 0)
        {
            ((AxMaterial*)resource)->CommitRenderStates();
        }
	}


	return true;
}

bool Ax3dsScene::Save(AxStream &destination, const char *format)
{
	return false;
}

AxResource *Ax3dsScene::CreateResourceAndMapping(AxString name3ds, AxResource *resource)
{
	this->context->AddResource(resource);

	resource->name = this->context->AcquireResourceName(name3ds, resource->resourceType);

	Ax3dsSceneNameResourceMapping mapping;
	mapping.name3ds = new AxString(name3ds);
	mapping.resource = resource;
	this->resourceMapping.Add(mapping);

	return resource;
}

AxResource *Ax3dsScene::FindResourceMapping(const AxString &name3ds)
{
	for (int i = 0; i < this->resourceMapping.count; i++)
		if (this->resourceMapping[i].name3ds->Equals(name3ds))
			return this->resourceMapping[i].resource;

	return 0;
}


AxResource *Ax3dsScene::CreateMaterialAndMapping(AxString name3ds, AxResource *material)
{
	this->context->AddResource(material);

	material->name = this->context->AcquireResourceName(name3ds, material->resourceType);

	Ax3dsSceneNameResourceMapping mapping;
	mapping.name3ds = new AxString(name3ds);
	mapping.resource = material;
	this->materialMapping.Add(mapping);

	return material;
}

AxResource *Ax3dsScene::FindMaterialMapping(const AxString &name3ds)
{
	for (int i = 0; i < this->materialMapping.count; i++)
		if (this->materialMapping[i].name3ds->Equals(name3ds))
			return this->materialMapping[i].resource;

	return 0;
}

void Ax3dsScene::LoadMaterialResource(AxFile3dsMaterial *material3ds)
{
	AxMaterial *material = (AxMaterial*)this->CreateMaterialAndMapping(material3ds->name, new AxMaterial(this->context));

    material->AddShadingLayer(AxShadingOperation_Material, AxColorOperation_None);

	material->GetProperty("Ambient")->SetColor(AxVector4((float)material3ds->ambient.r / (float)0xff,
														 (float)material3ds->ambient.g / (float)0xff,
														 (float)material3ds->ambient.b / (float)0xff,
														 1.0f));
    material->GetProperty("Diffuse")->SetColor(AxVector4((float)material3ds->diffuse.r / (float)0xff,
														 (float)material3ds->diffuse.g / (float)0xff,
														 (float)material3ds->diffuse.b / (float)0xff,
														 1.0f));
	material->GetProperty("Specular")->SetColor(AxVector4((float)material3ds->specular.r / (float)0xff,
														  (float)material3ds->specular.g / (float)0xff,
														  (float)material3ds->specular.b / (float)0xff,
														  1.0f));
	material->GetProperty("Gloss")->SetFloat(material3ds->glossiness);

    //for (ii = 0; ii < objectsCount; ii++)
    //    if (file3ds.objects[ii] is FileTypes.file3ds.Light)
    //        material->AddShadingLayer(AxShadingOperation.LightSource, AxColorOperation.None);

	material->AddShadingLayer(AxShadingOperation_AmbientLighting, AxColorOperation_Set);

	material->AddShadingLayer(AxShadingOperation_LightSource, AxColorOperation_None);
	material->AddShadingLayer(AxShadingOperation_LightSource, AxColorOperation_None);

	if (material3ds->bumpMap.length != 0)
    {
		AxTexture2D *texture = this->context->AcquireTexture(AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, material3ds->bumpMap));

		material->AddShadingLayer(AxShadingOperation_NormalMap, AxColorOperation_None);
		material->AddShadingLayer(AxShadingOperation_PixelLighting, AxColorOperation_Add);

		material->GetProperty("Normal map")->SetValue(texture, AxParameterType_ReferenceTexture);
    }
    else
        material->AddShadingLayer(AxShadingOperation_VertexLighting, AxColorOperation_Add);


    if (material3ds->colorMap.length != 0)
    {
		AxTexture2D *texture = this->context->AcquireTexture(AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, material3ds->colorMap));

        material->AddShadingLayer(AxShadingOperation_ColorMap, AxColorOperation_Modulate);

        material->GetProperty("Color map")->SetValue(texture, AxParameterType_ReferenceTexture);
    }

	if (material3ds->reflectionMap.length != 0)
    {
		AxTexture2D *texture = this->context->AcquireTexture(AxFileSystem::MergePaths(this->context->serializationParameters.rootDir, material3ds->reflectionMap));

        material->AddShadingLayer(AxShadingOperation_ReflectionMap, AxColorOperation_Add);

        material->GetProperty("Reflection map")->SetValue(texture, AxParameterType_ReferenceTexture);
    }

    if (material3ds->specularMap.length != 0)
    {
        /*
        AxTexture texture = AddTextureResource(filePath + file3ds.materials[i].specularMap);
        texture.name = file3ds.materials[i].specularMap;

        material->parametersList.Add(new AxMaterialParameter("Normal map", "normalMap", AxMaterialParameterType.Texture, texture));

        dShader.AddShadingLayer(AxShadingOperation.ColorMap, AxColorOperation.Modulate);
        */
    }

	material->UpdateShader();
}

void Ax3dsScene::LoadMeshResource(AxFile3dsMesh *mesh3ds)
{
	AxMesh *mesh = (AxMesh*)this->CreateResourceAndMapping(mesh3ds->name, new AxMesh(this->context));

	AxSmoothingGroups smoothGrouping(mesh3ds->numFaces, mesh3ds->numVertices);
	for (int i = 0; i < mesh3ds->numFaces; i++)
	{
		smoothGrouping.SetFaceSmoothingGroups(i, mesh3ds->faces[i].smoothGroups);
		smoothGrouping.AddFaceVertex(i, mesh3ds->faces[i].a);
		smoothGrouping.AddFaceVertex(i, mesh3ds->faces[i].b);
		smoothGrouping.AddFaceVertex(i, mesh3ds->faces[i].c);
	}


	int numVertices = smoothGrouping.vertexIndices.count;
	mesh->deviceMesh->CreateVertexBuffer(numVertices);
	for (int vertexIndex = 0; vertexIndex < numVertices; vertexIndex++)
	{
		int index = smoothGrouping.vertexIndices[vertexIndex].index;

		mesh->deviceMesh->SetVertexPosition(vertexIndex, AxVector3(-mesh3ds->vertices[index].x, mesh3ds->vertices[index].z, mesh3ds->vertices[index].y));
		mesh->deviceMesh->SetVertexTexCoords(vertexIndex, AxVector2(mesh3ds->vertices[index].tu, mesh3ds->vertices[index].tv));
	}

	int numTriangles = smoothGrouping.faces.count;
	mesh->deviceMesh->CreateIndexBuffer(numTriangles * 3);
	int index = 0;
	for (int faceIndex = 0; faceIndex < numTriangles; faceIndex++)
	{
		mesh->deviceMesh->SetIndex(index++, smoothGrouping.faces[faceIndex].indices[1]);
		mesh->deviceMesh->SetIndex(index++, smoothGrouping.faces[faceIndex].indices[0]);
		mesh->deviceMesh->SetIndex(index++, smoothGrouping.faces[faceIndex].indices[2]);
	}

	mesh->ComputeNormals();
	mesh->ComputeBounds();

	mesh->deviceMesh->UpdateVertices(0, numVertices);
	mesh->deviceMesh->UpdateIndices(0, numTriangles * 3);
}

void Ax3dsScene::LoadLightResource(AxFile3dsLight *light3ds)
{
	AxLight *light = (AxLight*)this->CreateResourceAndMapping(light3ds->name, new AxLight());
	light->name = this->context->AcquireResourceName(light3ds->name, AxResourceType_Light);
	light->properties[AxResource::propertyIndex_Enabled]->SetBool(light3ds->active);
	light->properties[AxLight::propertyIndex_Color]->SetColor(AxVector4(light3ds->color.r, light3ds->color.g, light3ds->color.b, 1.0f));
	light->properties[AxLight::propertyIndex_Intensity]->SetFloat(500.0f);
}

void Ax3dsScene::LoadCameraResource(AxFile3dsCamera *camera3ds)
{
	AxCamera *camera = (AxCamera*)this->CreateResourceAndMapping(camera3ds->name, new AxCamera(this->context));

	camera->name = this->context->AcquireResourceName(camera3ds->name, AxResourceType_Camera);
	camera->properties[AxCamera::propertyIndex_FOV]->SetFloat(AxMaths::LensToFOV(camera3ds->lens));
	//camera->orthoZoom = (float)this->context->
}

void Ax3dsScene::BuildSceneHierarchyNode(AxEntitySet *branch, AxFile3dsHierarchyNode *hierarchyNode3ds, AxLinearMechanism *animationFrame)
{
	branch->name = this->context->AcquireResourceName(hierarchyNode3ds->keyFrames->instanceName, AxResourceType_EntitySet);

	AxTransform *transformation = (AxTransform*)this->context->AddResource(new AxTransform());
	transformation->name = this->context->AcquireResourceName(hierarchyNode3ds->keyFrames->instanceName, AxResourceType_Transform);
    branch->references.Add(transformation);

	if (!hierarchyNode3ds->isDummy)
    {
		if (hierarchyNode3ds->obj->objectType == AxFile3dsObjectType_Mesh)
        {
			AxFile3dsMesh *mesh3ds = (AxFile3dsMesh*)hierarchyNode3ds->obj;

			AxResource *material = this->FindMaterialMapping(mesh3ds->materialName);
            if (material != 0)
                branch->references.Add(material);

			AxResource *mesh = this->FindResourceMapping(mesh3ds->name);
            if (mesh != 0)
                branch->references.Add(mesh);
        }

		if (hierarchyNode3ds->obj->objectType == AxFile3dsObjectType_Light)
		{
			AxFile3dsLight *light3ds = (AxFile3dsLight*)hierarchyNode3ds->obj;

			AxResource *light = this->FindResourceMapping(light3ds->name);
            if (light != 0)
                branch->references.Add(light);
		}

		if (hierarchyNode3ds->obj->objectType == AxFile3dsObjectType_Camera)
        {
			AxFile3dsCamera *camera3ds = (AxFile3dsCamera*)hierarchyNode3ds->obj;
			AxResource *camera = this->FindResourceMapping(camera3ds->name);
			if (camera != 0)
			{
				// Use position and target coordinates to set up camera's orientation, and if there are no keyframes - its position. AxCamera looks along its local positive Z direction (its orientation origin)
				AxVector3 position(-camera3ds->position.x, camera3ds->position.z, camera3ds->position.y);
				AxVector3 target(-camera3ds->target.x, camera3ds->target.z, camera3ds->target.y);

				if ((hierarchyNode3ds->keyFrames != 0) && (hierarchyNode3ds->keyFrames->position != 0))
				{
					AxVector3 orientationVector;
					AxVector3::Subtract(orientationVector, target, position);
					AxMatrix::CreateOrientation(transformation->transform, orientationVector, AxVector3(0.0f, 0.0f, 1.0f)); // Get only orientation from position and target
				}
				else
					AxMatrix::CreateOrientedPosition(transformation->transform, position, target, AxVector3(0.0f, 0.0f, 1.0f)); // Get both position and orientation

				//AxVector3 orientationVector;
				//AxVector3::Subtract(orientationVector, target, position);
				//AxMatrix::CreateOrientation(transformation->rotation, orientationVector, AxVector3(0.0f, 0.0f, 1.0f)); // Get only orientation from position and target
				//if ((hierarchyNode3ds->keyFrames == 0) || (hierarchyNode3ds->keyFrames->position == 0))
				//{
				//	AxMatrix::CreateTranslation(transformation->translation, position);
				//}

				
				AxTransform *identityTransform = (AxTransform*)this->context->FindResourceX("Identity Transform", AxResourceType_Transform);
				if (identityTransform == 0)
				{
					identityTransform = (AxTransform*)this->context->AddResource(new AxTransform());
					identityTransform->name = "Identity Transform";
				}

				branch->references.Add(camera);

				branch->references.Add(identityTransform);
			}
        }
    }


    if (hierarchyNode3ds->keyFrames != 0)
    {
        transformation->pivot._41 = -hierarchyNode3ds->keyFrames->pivot.x;
        transformation->pivot._42 = hierarchyNode3ds->keyFrames->pivot.z;
        transformation->pivot._43 = hierarchyNode3ds->keyFrames->pivot.y;

        bool useKeyframes = false;
        if (hierarchyNode3ds->keyFrames->scaling != 0)
            if (hierarchyNode3ds->keyFrames->scaling->keys > 1)
                useKeyframes = true;

        if (hierarchyNode3ds->keyFrames->rotation != 0)
            if (hierarchyNode3ds->keyFrames->rotation->keys > 1)
                useKeyframes = true;

        if (hierarchyNode3ds->keyFrames->position != 0)
            if (hierarchyNode3ds->keyFrames->position->keys > 1)
                useKeyframes = true;

        if (!useKeyframes)
        {
            if (hierarchyNode3ds->keyFrames->scaling != 0)
			{
				AxMatrix scaling;
				AxMatrix::CreateScaling(scaling, 
					((AxFile3dsKeyFrameScaling*)hierarchyNode3ds->keyFrames->scaling->data[0])->scaling.x,
					((AxFile3dsKeyFrameScaling*)hierarchyNode3ds->keyFrames->scaling->data[0])->scaling.z,
					((AxFile3dsKeyFrameScaling*)hierarchyNode3ds->keyFrames->scaling->data[0])->scaling.y);
				AxMatrix::Multiply(transformation->transform, transformation->transform, scaling);
			}
            if (hierarchyNode3ds->keyFrames->rotation != 0)
			{
				AxMatrix rotation;
				AxMatrix::CreateRotationAxis(rotation,
					-((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[0])->axis.x,
					((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[0])->axis.z,
					((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[0])->axis.y,
					((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[0])->angle);
				AxMatrix::Multiply(transformation->transform, transformation->transform, rotation);
			}
            if (hierarchyNode3ds->keyFrames->position != 0)
			{
				AxMatrix translation;
				AxMatrix::CreateTranslation(translation,
					-((AxFile3dsKeyFramePosition*)hierarchyNode3ds->keyFrames->position->data[0])->position.x,
                    ((AxFile3dsKeyFramePosition*)hierarchyNode3ds->keyFrames->position->data[0])->position.z,
                    ((AxFile3dsKeyFramePosition*)hierarchyNode3ds->keyFrames->position->data[0])->position.y);
				AxMatrix::Multiply(transformation->transform, transformation->transform, translation);
			}
        }

        else
        {
			AxKeyframeAnimationMechanism *keyFrames = (AxKeyframeAnimationMechanism*)this->context->AddResource(new AxKeyframeAnimationMechanism(this->context));
			keyFrames->name = this->context->AcquireResourceName(hierarchyNode3ds->keyFrames->instanceName + " animation", AxResourceType_Mechanism);
            branch->references.Add(keyFrames);
			keyFrames->properties[AxKeyframeAnimationMechanism::propertyIndex_Frame]->SetValue(animationFrame, AxParameterType_ReferenceMechanism);
			keyFrames->properties[AxKeyframeAnimationMechanism::propertyIndex_Transform]->SetValue(transformation, AxParameterType_ReferenceTransform);

            if (hierarchyNode3ds->keyFrames->scaling != 0)
            {
				AxKeyFramesTrack *scalingTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());

                *(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Min]->value = AxMath::Min(*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Min]->value, (float)hierarchyNode3ds->keyFrames->scaling->data[0]->frameNumber);
				*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->value = AxMath::Max(*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->value, (float)hierarchyNode3ds->keyFrames->scaling->data[hierarchyNode3ds->keyFrames->scaling->keys - 1]->frameNumber);

                for (int ii = 0; ii < hierarchyNode3ds->keyFrames->scaling->keys; ii++)
                    scalingTrack->Add(new AxKeyFrameScaling(
						(float)hierarchyNode3ds->keyFrames->scaling->data[ii]->frameNumber, AxVector3(						
							((AxFile3dsKeyFrameScaling*)hierarchyNode3ds->keyFrames->scaling->data[ii])->scaling.x,
                            ((AxFile3dsKeyFrameScaling*)hierarchyNode3ds->keyFrames->scaling->data[ii])->scaling.z,
                            ((AxFile3dsKeyFrameScaling*)hierarchyNode3ds->keyFrames->scaling->data[ii])->scaling.y)));
            }

            if (hierarchyNode3ds->keyFrames->rotation != 0)
            {
				AxKeyFramesTrack *rotationTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());

				*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Min]->value = AxMath::Min(*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Min]->value, (float)hierarchyNode3ds->keyFrames->rotation->data[0]->frameNumber);
                *(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->value = AxMath::Max(*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->value, (float)hierarchyNode3ds->keyFrames->rotation->data[hierarchyNode3ds->keyFrames->rotation->keys - 1]->frameNumber);

                for (int ii = 0; ii < hierarchyNode3ds->keyFrames->rotation->keys; ii++)
                    rotationTrack->Add(new AxKeyFrameRotationAxis(
						(float)hierarchyNode3ds->keyFrames->rotation->data[ii]->frameNumber,
						((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[ii])->angle, AxVector3(
							-((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[ii])->axis.x,
							((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[ii])->axis.z,
							((AxFile3dsKeyFrameRotation*)hierarchyNode3ds->keyFrames->rotation->data[ii])->axis.y)));

				keyFrames->BuildRotationAxisTrack(rotationTrack);
            }

            if (hierarchyNode3ds->keyFrames->position != 0)
            {
				AxKeyFramesTrack *translationTrack = keyFrames->keyFramesTracks.Add(new AxKeyFramesTrack());

				*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Min]->value = AxMath::Min(*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Min]->value, (float)hierarchyNode3ds->keyFrames->position->data[0]->frameNumber);
				*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->value = AxMath::Max(*(float*)animationFrame->properties[AxLinearMechanism::propertyIndex_Max]->value, (float)hierarchyNode3ds->keyFrames->position->data[hierarchyNode3ds->keyFrames->position->keys - 1]->frameNumber);

                for (int ii = 0; ii < hierarchyNode3ds->keyFrames->position->keys; ii++)
                    translationTrack->Add(new AxKeyFrameTranslation(
						(float)hierarchyNode3ds->keyFrames->position->data[ii]->frameNumber, AxVector3(
							-((AxFile3dsKeyFramePosition*)hierarchyNode3ds->keyFrames->position->data[ii])->position.x,
							((AxFile3dsKeyFramePosition*)hierarchyNode3ds->keyFrames->position->data[ii])->position.z,
							((AxFile3dsKeyFramePosition*)hierarchyNode3ds->keyFrames->position->data[ii])->position.y)));
            }
        }
    }
}

void Ax3dsScene::BuildSceneHierarchy(AxEntitySet *branch, AxFile3dsHierarchy *hierarchy3ds, AxLinearMechanism *animationFrame)
{
	// First cameras in the given branch
	for (int i = 0; i < hierarchy3ds->count; i++)
    {
		AxFile3dsHierarchyNode *hierarchyNode3ds = (*hierarchy3ds)[i];

		if (animationFrame == 0)
		{
			animationFrame = (AxLinearMechanism*)this->context->AddResource(new AxLinearMechanism(0, 100, 30, false, false, false));
			animationFrame->name = this->context->AcquireResourceName(hierarchyNode3ds->keyFrames->instanceName + " frame", AxResourceType_Mechanism);
			branch->references.Add(animationFrame);
		}

		bool isCamera = (hierarchyNode3ds->obj != 0) && (hierarchyNode3ds->obj->objectType == AxFile3dsObjectType_Camera);
		if (isCamera)
		{
			this->BuildSceneHierarchyNode(branch, hierarchyNode3ds, animationFrame);
		}
    }

	// Next, other objects, each in its sub-branch
	for (int i = 0; i < hierarchy3ds->count; i++)
    {
		AxFile3dsHierarchyNode *hierarchyNode3ds = (*hierarchy3ds)[i];

		bool isCamera = (hierarchyNode3ds->obj != 0) && (hierarchyNode3ds->obj->objectType == AxFile3dsObjectType_Camera);
		if (!isCamera)
		{
			AxEntitySet *subBranch = (AxEntitySet*)this->context->AddResource(new AxEntitySet());

			this->BuildSceneHierarchyNode(subBranch, hierarchyNode3ds, animationFrame);

			if (hierarchyNode3ds->children->count > 0)
				this->BuildSceneHierarchy(subBranch, hierarchyNode3ds->children, animationFrame);

			branch->references.Add(subBranch);
		}

    }
}