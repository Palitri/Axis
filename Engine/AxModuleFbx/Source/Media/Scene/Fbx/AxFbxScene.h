#pragma once

#include "..\..\..\AxisEngine.h"

#include <fbxsdk.h>

// Important notice about the AxFbxScene is that it CAN ONLY LOAD FILES FROM THE PHYSICAL FILE SYSTEM.
// The reason for this restriction is that it uses the Autodesk FBX SDK, which on the other hand does not support loading from memory.
// As consequence, AxFbxScene uses the serialization parameters of the context to get the name of the file to load. The source stream is, in fact, not used.

class AxFbxScene :
	public AxScene
{
private:
	FbxManager *fbxManager;
	FbxScene *fbxScene;

	AxLinearMechanism *animationFrame;
	
	AxDictionary<void*, AxResource*> resourceMap;

	AxVector2 FbxToAxVector2(FbxDouble2 fbxVector);
	AxVector3 FbxToAxVector3(FbxDouble3 fbxVector);
	AxVector3 FbxToAxVector3(FbxDouble4 fbxVector);
	void FbxToAxMatrix(AxMatrix &result, FbxAMatrix *fbxMatrix);
	void FbxCreateRotationMatrix(AxMatrix &result, AxVector3 eulerAngles, EFbxRotationOrder rotationOrder);

	void GetNodeGeometryTransform(AxMatrix &transform, FbxNode *fbxNode);

	void LoadNodeDefaultTransform(AxTransform *transform, FbxNode *fbxNode);
	bool LoadNodeKeyframeAnimation(AxEntitySet *entitySet, FbxNode* fbxNode, AxTransform *transformToAnimate);

	bool GetMaterialPropertyTextureName(const FbxProperty &fbxProperty, AxString &fileName);
	AxVector4 GetMaterialPropertyColor(const FbxProperty &fbxProperty);
	float GetMaterialPropertyColorFactor(const FbxProperty &fbxProperty);
	AxMaterial *AcquireNodeMaterial(FbxSurfaceMaterial *fbxMaterial);


	void LoadMesh_Vertices(AxDeviceMesh *deviceMesh, FbxMesh *fbxMesh, AxMatrix &geometryTransform);
	AxTexCoordChannels *LoadMesh_TextureCoords(AxDeviceMesh *deviceMesh, FbxMesh *fbxMesh);
	bool LoadMesh_Animation(AxDeviceMesh *deviceMesh, FbxMesh *fbxMesh);
	AxSmoothingGroups* LoadMesh_CreateSmoothGrouping(FbxMesh *fbxMesh, AxTexCoordChannels *texCoordChannels);
	void LoadMesh(AxEntitySet *entitySet, FbxMesh *fbxMesh, AxMatrix &geometryTransform);

	void LoadNode(AxEntitySet *entitySet, FbxNode* fbxNode, FbxNodeAttribute::EType loadType);

public:
	AxFbxScene(void);
	~AxFbxScene(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

