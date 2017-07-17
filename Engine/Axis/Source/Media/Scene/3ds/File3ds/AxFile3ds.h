#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "AxFile3dsPrimitiveTypes.h"
#include "AxFile3dsBaseObject.h"
#include "AxFile3dsMesh.h"
#include "AxFile3dsLight.h"
#include "AxFile3dsCamera.h"
#include "AxFile3dsMaterial.h"
#include "AxFile3dsKeyFrames.h"
#include "AxFile3dsHierarchy.h"

#include "..\..\..\..\Tools\Streams\AxStream.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFile3dsBaseObject*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxFile3dsMaterial*>;

enum AxFile3dsMaterialParameter
{
    AxFile3dsMaterialParameter_AmbientColor,
    AxFile3dsMaterialParameter_DiffuseColor,
    AxFile3dsMaterialParameter_SpecularColor,
    AxFile3dsMaterialParameter_TextureMap,
    AxFile3dsMaterialParameter_SpecularMap,
    AxFile3dsMaterialParameter_BumpMap,
    AxFile3dsMaterialParameter_ReflectionMap,
    AxFile3dsMaterialParameter_SpecularLevel,
    AxFile3dsMaterialParameter_Glossiness
};

class AXDLLCLASS AxFile3ds
{
private:
	static const int chunk_Main = 0x4D4D;
	static const int 	chunk_3dEditor = 0x3D3D;
	static const int 		chunk_ObjectBlock = 0x4000;
	static const int 			chunk_TriangularMesh = 0x4100;
	static const int 				chunk_VerticesList = 0x4110;
	static const int 				chunk_FacesList = 0x4120;
	static const int 				chunk_MeshMaterialName = 0x4130;
	static const int 				chunk_MappingCoords = 0x4140;
	static const int 				chunk_SmoothGroups = 0x4150;
	static const int 				chunk_LocalCoordSystem = 0x4160;
	static const int 			chunk_Light = 0x4600;
	static const int 				chunk_LightColor = 0x0010;
	static const int 				chunk_SpotLight = 0x4610;
	static const int 				chunk_LightOff = 0x4620;
	static const int 			chunk_Camera = 0x4700;
	static const int 		chunk_MaterialBlock = 0xAFFF;
	static const int 			chunk_MaterialName = 0xA000;
	static const int 			chunk_AmbientColor = 0xA010;
	static const int 			chunk_DiffuseColor = 0xA020;
	static const int 			chunk_SpecularColor = 0xA030;
	static const int 				chunk_MaterialColor = 0x011;
	static const int 			chunk_Glossiness = 0xA040;
	static const int 			chunk_SpecularLevel = 0xA041;
	static const int 				chunk_AmountOf = 0x0030;
	static const int 			chunk_TextureMap = 0xA200;
	static const int 			chunk_SpecularMap = 0xA204;
	static const int 			chunk_BumpMap = 0xA230;
	static const int 			chunk_ReflectionMap = 0xA220;
	static const int 				chunk_MappingFileName = 0xA300;
	static const int 	chunk_KeyFrameData = 0xB000;
	static const int 		chunk_KeyFrameInfo = 0xB008;
	static const int 		chunk_CurrentFrame = 0xB009;
	static const int 		chunk_KeyFrameObjectNode = 0xB002;
	static const int 		chunk_KeyFrameCameraNode = 0xB003;
	static const int 		chunk_KeyFrameLightNode = 0xB005;
	static const int 			chunk_KFObjectNameAndHierarchyLevel = 0xB010;
	static const int 			chunk_KFObjectInstanceName = 0xB011;
	static const int 			chunk_KFObjectPivot = 0xB013;
	static const int 			chunk_KFObjectPositionTrack = 0xB020;
	static const int 			chunk_KFObjectRotationTrack = 0xB021;
	static const int 			chunk_KFObjectScalingTrack = 0xB022;
	static const int 			chunk_KFObjectID = 0xB030;

	AxStream *dataStream;

	AxFile3dsChunkHeader currentHeader;
	
	AxString currentString;

	AxFile3dsMesh *currentMesh;
	AxFile3dsLight *currentLight;
	AxFile3dsCamera *currentCamera;

	AxFile3dsMaterial *currentMaterial;
	AxFile3dsMaterialParameter currentMaterialParameter;

	AxFile3dsKeyFrameObject *currentKeyFrameObject;

	int hierarchyIndex;

	long long mainChunkEnd;

	bool ReadChunks();

	void ReadChunkObjectBlock();
	void ReadChunkTriangularMesh();
	void ReadChunkVerticesList();
	void ReadChunkFacesList();
	void ReadChunkMeshMaterialName();
	void ReadChunkMappingCoords();
	void ReadChunkSmoothGroups();
	void ReadChunkLocalCoordSystem();
	void ReadChunkLight();
	void ReadChunkLightColor();
	void ReadChunkSpotLight();
	void ReadChunkLightActive();
	void ReadChunkCamera();
	void ReadChunkMaterialBlock();
	void ReadChunkMaterialName();
	void ReadChunkAmbientColor();
	void ReadChunkDiffuseColor();
	void ReadChunkSpecularColor();
	void ReadChunkMaterialColor();
	void ReadChunkSpecularLevel();
	void ReadChunkGlossiness();
	void ReadChunkAmountOf();
	void ReadChunkTextureMap();
	void ReadChunkSpecularMap();
	void ReadChunkBumpMap();
	void ReadChunkReflectionMap();
	void ReadChunkMappingFileName();

	void ReadChunkKeyframesInfo();
	void ReadChunkKeyframesCurrentFrame();
	void ReadChunkKFObjectNameAndHieararchyLevel();
	void ReadChunkKFObjectInstanceName();
	void ReadChunkKFObjectPivot();
	void ReadChunkKFObjectPositionTrack();
	void ReadChunkKFObjectRoatationTrack();
	void ReadChunkKFObjectScalingTrack();
	void ReadChunkKFObjectID();

	AxString ReadString();
	void ReadKeyFrameTrackHeader(AxFile3dsKeyFramesTrack *track);
	void ReadKeyFrameParams(AxFile3dsKeyFrameBase *keyFrame);

	void InverseAllMeshesTransformation();
	AxFile3dsBaseObject *FindObject(AxString name);
	int FindParent(int child);
	void BuildHierarchyFromResources();
	void ArrangeHierarchyObject(int hierarchyLevel, AxFile3dsHierarchy *hierarchy);
	void ArrangeHierarchy();

public:

	//TODO: separate mesh, cameara and light?
	// Contains all the resource objects in the 3ds - meshes, lights, cameras
	AxList<AxFile3dsBaseObject*> objects;
	int numLights;

	// Contains all the material resources in the 3ds
	AxList<AxFile3dsMaterial*> materials;

	// Contains all the keyframe data, where every keyframe node is references a resource object via the object's name.
	AxFile3dsKeyFrames *keyFrames;

	// After ArrangeHierarchy, this holds the hierarchical structure of the 3ds scene, including the references to the resource objects and their keyframes
	AxFile3dsHierarchy *hierarchy;

	AxFile3ds(void);
	~AxFile3ds(void);

	bool Read(AxStream *dataStream);
};

