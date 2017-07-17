//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFile3ds.h"

AxFile3ds::AxFile3ds(void)
{
	this->keyFrames = 0;
	this->hierarchy = 0;
}


AxFile3ds::~AxFile3ds(void)
{
	for (int i = 0; i < this->objects.count; i++)
		delete this->objects[i];

	for (int i = 0; i < this->materials.count; i++)
		delete this->materials[i];

	if (this->keyFrames != 0)
		delete this->keyFrames;

	if (this->hierarchy != 0)
		delete this->hierarchy;
}


bool AxFile3ds::Read(AxStream *dataStream)
{
	this->dataStream = dataStream;
	this->mainChunkEnd = -1;

	if (this->dataStream->length - this->dataStream->position < 6)
		return false;

	if (!this->ReadChunks())
		return false;

	this->InverseAllMeshesTransformation();
	this->ArrangeHierarchy();

	return true;
}

bool AxFile3ds::ReadChunks()
{
    long long chunkStart = this->dataStream->position;
	this->currentHeader.id = this->dataStream->ReadUInt16();
    this->currentHeader.length = this->dataStream->ReadInt32();
	switch (this->currentHeader.id)
    {
        case AxFile3ds::chunk_Main:
			this->mainChunkEnd = chunkStart + this->currentHeader.length;
			break;

        case AxFile3ds::chunk_3dEditor:
            break;

        case AxFile3ds::chunk_ObjectBlock:
            this->ReadChunkObjectBlock();
            break;

        case AxFile3ds::chunk_TriangularMesh:
            this->ReadChunkTriangularMesh();
            break;

        case AxFile3ds::chunk_VerticesList:
            this->ReadChunkVerticesList();
            break;

        case AxFile3ds::chunk_FacesList:
            this->ReadChunkFacesList();
            break;

        case AxFile3ds::chunk_MeshMaterialName:
            this->ReadChunkMeshMaterialName();
            break;

        case AxFile3ds::chunk_MappingCoords:
            this->ReadChunkMappingCoords();
            break;

        case AxFile3ds::chunk_SmoothGroups:
            this->ReadChunkSmoothGroups();
            break;

        case AxFile3ds::chunk_LocalCoordSystem:
            this->ReadChunkLocalCoordSystem();
            break;

        case AxFile3ds::chunk_Light:
            this->ReadChunkLight();
            break;

        case AxFile3ds::chunk_LightColor:
            this->ReadChunkLightColor();
            break;

        case AxFile3ds::chunk_SpotLight:
            this->ReadChunkSpotLight();
            break;

        case AxFile3ds::chunk_LightOff:
            this->ReadChunkLightActive();
            break;

        case AxFile3ds::chunk_Camera:
            this->ReadChunkCamera();
            break;

        case AxFile3ds::chunk_MaterialBlock:
            this->ReadChunkMaterialBlock();
            break;

        case AxFile3ds::chunk_MaterialName:
            this->ReadChunkMaterialName();
            break;

        case AxFile3ds::chunk_AmbientColor:
            this->ReadChunkAmbientColor();
            break;

        case AxFile3ds::chunk_DiffuseColor:
            this->ReadChunkDiffuseColor();
            break;

        case AxFile3ds::chunk_SpecularColor:
            this->ReadChunkSpecularColor();
            break;

        case AxFile3ds::chunk_MaterialColor:
            this->ReadChunkMaterialColor();
            break;

        case AxFile3ds::chunk_SpecularLevel:
            this->ReadChunkSpecularLevel();
            break;

        case AxFile3ds::chunk_Glossiness:
            this->ReadChunkGlossiness();
            break;

        case AxFile3ds::chunk_AmountOf:
            this->ReadChunkAmountOf();
            break;

        case AxFile3ds::chunk_TextureMap:
            this->ReadChunkTextureMap();
            break;

        case AxFile3ds::chunk_SpecularMap:
            this->ReadChunkSpecularMap();
            break;

        case AxFile3ds::chunk_BumpMap:
            this->ReadChunkBumpMap();
            break;

        case AxFile3ds::chunk_ReflectionMap:
            this->ReadChunkReflectionMap();
            break;

        case AxFile3ds::chunk_MappingFileName:
            this->ReadChunkMappingFileName();
            break;

        case AxFile3ds::chunk_KeyFrameData:
            this->keyFrames = new AxFile3dsKeyFrames();
            break;

        case AxFile3ds::chunk_KeyFrameInfo:
            this->ReadChunkKeyframesInfo();
            break;

        case AxFile3ds::chunk_CurrentFrame:
            this->ReadChunkKeyframesCurrentFrame();
            break;

        case AxFile3ds::chunk_KeyFrameObjectNode:
        case AxFile3ds::chunk_KeyFrameCameraNode:
        case AxFile3ds::chunk_KeyFrameLightNode:
            this->currentKeyFrameObject = new AxFile3dsKeyFrameObject();
            this->keyFrames->objects.Add(this->currentKeyFrameObject);
            break;

        case AxFile3ds::chunk_KFObjectNameAndHierarchyLevel:
            this->ReadChunkKFObjectNameAndHieararchyLevel();
            break;

        case AxFile3ds::chunk_KFObjectInstanceName:
            this->ReadChunkKFObjectInstanceName();
            break;

        case AxFile3ds::chunk_KFObjectPivot:
            this->ReadChunkKFObjectPivot();
            break;

        case AxFile3ds::chunk_KFObjectPositionTrack:
            this->ReadChunkKFObjectPositionTrack();
            break;

        case AxFile3ds::chunk_KFObjectRotationTrack:
            this->ReadChunkKFObjectRoatationTrack();
            break;

        case AxFile3ds::chunk_KFObjectScalingTrack:
            this->ReadChunkKFObjectScalingTrack();
            break;

        case AxFile3ds::chunk_KFObjectID:
            this->ReadChunkKFObjectID();
            break;

        default:
			if ((this->currentHeader.length < 0) || (chunkStart + this->currentHeader.length > this->mainChunkEnd))
				return false;

			this->dataStream->Seek(chunkStart + this->currentHeader.length);
            break;
    }

	if (this->mainChunkEnd < 0)
		return false;

	if (this->dataStream->position + 6 < this->mainChunkEnd)
        return this->ReadChunks();

	return true;
}
		
void AxFile3ds::ReadChunkObjectBlock()
{
    this->currentString = this->ReadString();
}

void AxFile3ds::ReadChunkTriangularMesh()
{
    this->currentMesh = new AxFile3dsMesh();
    this->objects.Add(this->currentMesh);
    this->currentMesh->name = currentString;
}

void AxFile3ds::ReadChunkVerticesList()
{
    int numVertices = this->dataStream->ReadUInt16();
    if (this->currentMesh->numVertices == 0)
    {
        this->currentMesh->numVertices = numVertices;
        this->currentMesh->vertices = new AxFile3dsMeshVertex[this->currentMesh->numVertices];
    }

    for (int i = 0; i < this->currentMesh->numVertices; i++)
    {
        this->currentMesh->vertices[i].x = this->dataStream->ReadFloat32();
        this->currentMesh->vertices[i].y = this->dataStream->ReadFloat32();
        this->currentMesh->vertices[i].z = this->dataStream->ReadFloat32();
    }
}

void AxFile3ds::ReadChunkFacesList()
{
    this->currentMesh->numFaces = this->dataStream->ReadUInt16();
    this->currentMesh->faces = new AxFile3dsMeshFace[this->currentMesh->numFaces];

    for (int i = 0; i < this->currentMesh->numFaces; i++)
    {
        this->currentMesh->faces[i].a = this->dataStream->ReadUInt16();
        this->currentMesh->faces[i].b = this->dataStream->ReadUInt16();
        this->currentMesh->faces[i].c = this->dataStream->ReadUInt16();
        this->currentMesh->faces[i].info = this->dataStream->ReadUInt16();
    }
}

void AxFile3ds::ReadChunkMeshMaterialName()
{
    this->currentMesh->materialName = this->ReadString();
    int numFaces = this->dataStream->ReadUInt16();
    for (int i = 0; i < numFaces; i++)
        this->dataStream->ReadInt16();
}

void AxFile3ds::ReadChunkMappingCoords()
{
    int numVertices = this->dataStream->ReadUInt16();
    if (this->currentMesh->numVertices == 0)
    {
        this->currentMesh->numVertices = numVertices;
        this->currentMesh->vertices = new AxFile3dsMeshVertex[this->currentMesh->numVertices];
    }

    for (int i = 0; i < this->currentMesh->numVertices; i++)
    {
        this->currentMesh->vertices[i].tu = this->dataStream->ReadFloat32();
        this->currentMesh->vertices[i].tv = this->dataStream->ReadFloat32();
    }
}

void AxFile3ds::ReadChunkSmoothGroups()
{
	int num = (this->currentHeader.length - 6) / 4;//this->currentMesh->numFaces;
    for (int i = 0; i < num; i++)
        this->currentMesh->faces[i].smoothGroups = this->dataStream->ReadUInt32();
}

void AxFile3ds::ReadChunkLocalCoordSystem()
{
    this->currentMesh->localCoordSystem._11 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._12 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._13 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._14 = 0.0f;
    this->currentMesh->localCoordSystem._21 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._22 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._23 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._24 = 0.0f;
    this->currentMesh->localCoordSystem._31 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._32 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._33 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._34 = 0.0f;
    this->currentMesh->localCoordSystem._41 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._42 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._43 = this->dataStream->ReadFloat32();
    this->currentMesh->localCoordSystem._44 = 1.0f;
}

void AxFile3ds::ReadChunkLight()
{
    this->currentLight = new AxFile3dsLight();
    this->objects.Add(this->currentLight);
    this->currentLight->name = this->currentString;

    this->currentLight->position.x = this->dataStream->ReadFloat32();
    this->currentLight->position.y = this->dataStream->ReadFloat32();
    this->currentLight->position.z = this->dataStream->ReadFloat32();

    numLights++;
}

void AxFile3ds::ReadChunkLightColor()
{
    this->currentLight->color.r = this->dataStream->ReadFloat32();
    this->currentLight->color.g = this->dataStream->ReadFloat32();
    this->currentLight->color.b = this->dataStream->ReadFloat32();
}

void AxFile3ds::ReadChunkSpotLight()
{
    this->currentLight->spotlight_Target.x = this->dataStream->ReadFloat32();
    this->currentLight->spotlight_Target.y = this->dataStream->ReadFloat32();
    this->currentLight->spotlight_Target.z = this->dataStream->ReadFloat32();
    this->currentLight->spotlight_Hotspot = this->dataStream->ReadFloat32();
    this->currentLight->spotlight_Falloff = this->dataStream->ReadFloat32();
    this->currentLight->spotLight = true;
}

void AxFile3ds::ReadChunkLightActive()
{
	this->currentLight->active = this->dataStream->ReadBool();
}

void AxFile3ds::ReadChunkCamera()
{
    this->currentCamera = new AxFile3dsCamera();
    this->objects.Add(this->currentCamera);
    this->currentCamera->name = currentString;

    this->currentCamera->position.x = this->dataStream->ReadFloat32();
    this->currentCamera->position.y = this->dataStream->ReadFloat32();
    this->currentCamera->position.z = this->dataStream->ReadFloat32();
    this->currentCamera->target.x = this->dataStream->ReadFloat32();
    this->currentCamera->target.y = this->dataStream->ReadFloat32();
    this->currentCamera->target.z = this->dataStream->ReadFloat32();
    this->currentCamera->bank = this->dataStream->ReadFloat32();
    this->currentCamera->lens = this->dataStream->ReadFloat32();
}

void AxFile3ds::ReadChunkMaterialBlock()
{
    this->currentMaterial = new AxFile3dsMaterial();
    this->materials.Add(this->currentMaterial);
}

void AxFile3ds::ReadChunkMaterialName()
{
    this->currentMaterial->name = this->ReadString();
}

void AxFile3ds::ReadChunkAmbientColor()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_AmbientColor;
    this->currentMaterial->hasAmbient = true;
}

void AxFile3ds::ReadChunkDiffuseColor()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_DiffuseColor;
    this->currentMaterial->hasDiffuse = true;
}

void AxFile3ds::ReadChunkSpecularColor()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_SpecularColor;
    this->currentMaterial->hasSpecular = true;
}

void AxFile3ds::ReadChunkMaterialColor()
{
    AxFile3dsColorByte color;
    color.r = this->dataStream->ReadUInt8();
    color.g = this->dataStream->ReadUInt8();
    color.b = this->dataStream->ReadUInt8();

    switch (this->currentMaterialParameter)
    {
        case AxFile3dsMaterialParameter_AmbientColor:
            this->currentMaterial->ambient = color;
            break;

        case AxFile3dsMaterialParameter_DiffuseColor:
            this->currentMaterial->diffuse = color;
            break;

        case AxFile3dsMaterialParameter_SpecularColor:
            this->currentMaterial->specular = color;
            break;
    }
}

void AxFile3ds::ReadChunkSpecularLevel()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_SpecularLevel;
}

void AxFile3ds::ReadChunkGlossiness()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_Glossiness;
}

void AxFile3ds::ReadChunkAmountOf()
{
    int amountOf;
    amountOf = this->dataStream->ReadInt16();

    switch (this->currentMaterialParameter)
    {
        case AxFile3dsMaterialParameter_SpecularLevel:
            this->currentMaterial->specularLevel = amountOf / 100.0f;
            break;

        case AxFile3dsMaterialParameter_Glossiness:
            this->currentMaterial->glossiness = (float)amountOf;
            break;
    }
}

void AxFile3ds::ReadChunkTextureMap()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_TextureMap;
}

void AxFile3ds::ReadChunkSpecularMap()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_SpecularMap;
}

void AxFile3ds::ReadChunkBumpMap()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_BumpMap;
}

void AxFile3ds::ReadChunkReflectionMap()
{
    this->currentMaterialParameter = AxFile3dsMaterialParameter_ReflectionMap;
}

void AxFile3ds::ReadChunkMappingFileName()
{
    switch (this->currentMaterialParameter)
    {
        case AxFile3dsMaterialParameter_TextureMap:
            this->currentMaterial->colorMap = this->ReadString();
            break;

        case AxFile3dsMaterialParameter_SpecularMap:
            this->currentMaterial->specularMap = this->ReadString();
            break;

        case AxFile3dsMaterialParameter_BumpMap:
            this->currentMaterial->bumpMap = this->ReadString();
            break;

        case AxFile3dsMaterialParameter_ReflectionMap:
            this->currentMaterial->reflectionMap = this->ReadString();
            break;

        default:
            this->ReadString();
            break;
    }
}



void AxFile3ds::ReadChunkKeyframesInfo()
{
    this->keyFrames->start = this->dataStream->ReadInt32();
    this->keyFrames->end = this->dataStream->ReadInt32();
}

void AxFile3ds::ReadChunkKeyframesCurrentFrame()
{
    this->keyFrames->current = this->dataStream->ReadInt32() / 160;
}

void AxFile3ds::ReadChunkKFObjectNameAndHieararchyLevel()
{
    this->currentKeyFrameObject->name = ReadString();
    this->currentKeyFrameObject->instanceName = this->currentKeyFrameObject->name;
	this->dataStream->Seek(2 * 2,  StreamSeekMode_Relative);
    this->currentKeyFrameObject->hierarchyLevel = this->dataStream->ReadInt16();
}

void AxFile3ds::ReadChunkKFObjectInstanceName()
{
    this->currentKeyFrameObject->instanceName = ReadString();
}

void AxFile3ds::ReadChunkKFObjectPivot()
{
    this->currentKeyFrameObject->pivot.x = this->dataStream->ReadFloat32();
    this->currentKeyFrameObject->pivot.y = this->dataStream->ReadFloat32();
    this->currentKeyFrameObject->pivot.z = this->dataStream->ReadFloat32();
}

void AxFile3ds::ReadChunkKFObjectPositionTrack()
{
    this->currentKeyFrameObject->position = new AxFile3dsKeyFramesTrack();

    ReadKeyFrameTrackHeader(this->currentKeyFrameObject->position);

	for (int i = 0; i < this->currentKeyFrameObject->position->keys; i++)
    {
        AxFile3dsKeyFramePosition *keyFrame = new AxFile3dsKeyFramePosition();
        ReadKeyFrameParams(keyFrame);

        keyFrame->position.x = this->dataStream->ReadFloat32();
        keyFrame->position.y = this->dataStream->ReadFloat32();
        keyFrame->position.z = this->dataStream->ReadFloat32();

        this->currentKeyFrameObject->position->data.Add(keyFrame);
    }
}

void AxFile3ds::ReadChunkKFObjectRoatationTrack()
{
    this->currentKeyFrameObject->rotation = new AxFile3dsKeyFramesTrack();

    ReadKeyFrameTrackHeader(this->currentKeyFrameObject->rotation);

    for (int i = 0; i < this->currentKeyFrameObject->rotation->keys; i++)
    {
        AxFile3dsKeyFrameRotation *keyFrame = new AxFile3dsKeyFrameRotation();
        ReadKeyFrameParams(keyFrame);

        keyFrame->angle = this->dataStream->ReadFloat32();
        keyFrame->axis.x = this->dataStream->ReadFloat32();
        keyFrame->axis.y = this->dataStream->ReadFloat32();
        keyFrame->axis.z = this->dataStream->ReadFloat32();

        this->currentKeyFrameObject->rotation->data.Add(keyFrame);
    }
}

void AxFile3ds::ReadChunkKFObjectScalingTrack()
{
    this->currentKeyFrameObject->scaling = new AxFile3dsKeyFramesTrack();

    ReadKeyFrameTrackHeader(this->currentKeyFrameObject->scaling);

    for (int i = 0; i < this->currentKeyFrameObject->scaling->keys; i++)
    {
        AxFile3dsKeyFrameScaling *keyFrame = new AxFile3dsKeyFrameScaling();
        ReadKeyFrameParams(keyFrame);

        keyFrame->scaling.x = this->dataStream->ReadFloat32();
        keyFrame->scaling.y = this->dataStream->ReadFloat32();
        keyFrame->scaling.z = this->dataStream->ReadFloat32();

		this->currentKeyFrameObject->scaling->data.Add(keyFrame);
    }
}

void AxFile3ds::ReadChunkKFObjectID()
{
    this->currentKeyFrameObject->id = this->dataStream->ReadUInt16();
}


// Helper methods

AxString AxFile3ds::ReadString()
{
    AxString result;
    char c;
	for (;;)
	{
		c = this->dataStream->ReadUInt8();

        if (c != 0)
			result += c;
		else
			break;
    }

    return AxString(result);
}

void AxFile3ds::ReadKeyFrameTrackHeader(AxFile3dsKeyFramesTrack *track)
{
	track->flags = this->dataStream->ReadUInt16();
	this->dataStream->Seek(2 * 4, StreamSeekMode_Relative);
    track->keys = this->dataStream->ReadInt32();
}

void AxFile3ds::ReadKeyFrameParams(AxFile3dsKeyFrameBase *keyFrame)
{
    keyFrame->frameNumber = this->dataStream->ReadInt32();

    keyFrame->flags = this->dataStream->ReadUInt16();

    if ((keyFrame->flags & 0x00000001) != 0)
        keyFrame->tension = this->dataStream->ReadFloat32();
    if ((keyFrame->flags & 0x00000002) != 0)
        keyFrame->continuity = this->dataStream->ReadFloat32();
    if ((keyFrame->flags & 0x00000004) != 0)
        keyFrame->bias = this->dataStream->ReadFloat32();
    if ((keyFrame->flags & 0x00000008) != 0)
        keyFrame->easeTo = this->dataStream->ReadFloat32();
    if ((keyFrame->flags & 0x00000010) != 0)
        keyFrame->easeFrom = this->dataStream->ReadFloat32();
}

void AxFile3ds::InverseAllMeshesTransformation()
{
    int count = this->objects.count;
    for (int i = 0; i < count; i++)
		if (this->objects[i]->objectType == AxFile3dsObjectType_Mesh)
			((AxFile3dsMesh*)this->objects[i])->InverseTransformation();
}

AxFile3dsBaseObject *AxFile3ds::FindObject(AxString name)
{
	int count = this->objects.count;
    for (int i = 0; i < count; i++)
		if (this->objects[i]->name.Equals(name))
            return objects[i];

    return 0;
}

int AxFile3ds::FindParent(int child)
{
    for (int i = child; i >= 0; i--)
		if (this->keyFrames->objects[i]->hierarchyLevel < this->keyFrames->objects[child]->hierarchyLevel)
            return i;
    return -1;
}

void AxFile3ds::BuildHierarchyFromResources()
{
    for (int i = 0; i < this->objects.count; i++)
	{
		AxFile3dsBaseObject *fileObject = this->objects[i];
		if (fileObject->objectType == AxFile3dsObjectType_Mesh)
		{
			AxFile3dsKeyFrameObject *keyFrame = new AxFile3dsKeyFrameObject();
			keyFrame->name = fileObject->name;
			keyFrame->id = 0;
			keyFrame->hierarchyLevel = -1;
			keyFrame->position = new AxFile3dsKeyFramesTrack();
			keyFrame->rotation = new AxFile3dsKeyFramesTrack();
			keyFrame->scaling = new AxFile3dsKeyFramesTrack();
			this->hierarchy->Add(new AxFile3dsHierarchyNode(fileObject, keyFrame));
		}
	}
}

void AxFile3ds::ArrangeHierarchyObject(int hierarchyLevel, AxFile3dsHierarchy *hierarchy)
{
    if (this->hierarchyIndex < this->keyFrames->objects.count)
        if (this->keyFrames->objects[this->hierarchyIndex]->hierarchyLevel < hierarchyLevel)
            return;

    if (this->hierarchyIndex < this->keyFrames->objects.count)
        if (this->keyFrames->objects[this->hierarchyIndex]->hierarchyLevel > hierarchyLevel)
        {
            AxFile3dsHierarchyNode *child = new AxFile3dsHierarchyNode(this->FindObject(this->keyFrames->objects[this->hierarchyIndex]->name), this->keyFrames->objects[this->hierarchyIndex]);
            (*hierarchy)[hierarchy->count - 1]->children->Add(child);

            this->hierarchyIndex++;
            this->ArrangeHierarchyObject(this->keyFrames->objects[this->hierarchyIndex - 1]->hierarchyLevel, (*hierarchy)[hierarchy->count - 1]->children);
        }

    if (this->hierarchyIndex < this->keyFrames->objects.count)
        if (this->keyFrames->objects[this->hierarchyIndex]->hierarchyLevel == hierarchyLevel)
        {
            AxFile3dsHierarchyNode *child = new AxFile3dsHierarchyNode(this->FindObject(this->keyFrames->objects[this->hierarchyIndex]->name), this->keyFrames->objects[this->hierarchyIndex]);
            hierarchy->Add(child);

            this->hierarchyIndex++;
            this->ArrangeHierarchyObject(this->keyFrames->objects[this->hierarchyIndex - 1]->hierarchyLevel, hierarchy);
        }
}

void AxFile3ds::ArrangeHierarchy()
{
    this->hierarchy = new AxFile3dsHierarchy(0);

    if (this->keyFrames == 0)
        this->BuildHierarchyFromResources();
    else
        if (this->keyFrames->objects.count == 0)
            this->BuildHierarchyFromResources();
        else
        {
            this->hierarchyIndex = 0;
            this->ArrangeHierarchyObject(-1, this->hierarchy);
        }
}