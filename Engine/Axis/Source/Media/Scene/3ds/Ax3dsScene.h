#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxScene.h"

#include "File3ds\AxFile3ds.h"

#include "..\..\..\Engine\Entities\EntitySet\AxEntitySet.h"
#include "..\..\..\Engine\Entities\Material\AxMaterial.h"
#include "..\..\..\Engine\Entities\Mechanisms\AxLinearMechanism.h"

#include "..\..\..\Tools\AxList.h"

struct Ax3dsSceneNameResourceMapping
{
	AxString *name3ds;
	AxResource *resource;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<Ax3dsSceneNameResourceMapping>;

class AXDLLCLASS Ax3dsScene :
	public AxScene
{
private:
	// Separate resourceMapping and materialMapping, because AxFile3dsMaterial is not a AxFile3dsBase object, which is because the 3ds file structure does not imply it to be. That generally means a 3ds material can have the same name as a 3ds mesh, 3ds light or 3ds camera
	// TODO: If two resou	rces of different types can have the same name, finding a resource by name can be ambiguous
	AxList<Ax3dsSceneNameResourceMapping> resourceMapping, materialMapping;

	AxResource *CreateResourceAndMapping(AxString name3ds, AxResource *resource);
	AxResource *FindResourceMapping(const AxString &name3ds);

	AxResource *CreateMaterialAndMapping(AxString name3ds, AxResource *material);
	AxResource *FindMaterialMapping(const AxString &name3ds);

	void LoadMaterialResource(AxFile3dsMaterial *material3ds);
	void LoadMeshResource(AxFile3dsMesh *mesh3ds);
	void LoadLightResource(AxFile3dsLight *light3ds);
	void LoadCameraResource(AxFile3dsCamera *camera3ds);

	void BuildSceneHierarchyNode(AxEntitySet *branch, AxFile3dsHierarchyNode *hierarchyNode3ds, AxLinearMechanism *animationFrame);
	void BuildSceneHierarchy(AxEntitySet *branch, AxFile3dsHierarchy *hierarchy3ds, AxLinearMechanism *animationFrame = 0);

public:
	Ax3dsScene(void);
	~Ax3dsScene(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

