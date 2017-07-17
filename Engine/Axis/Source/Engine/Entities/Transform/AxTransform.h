#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxList.h"

#include "..\..\Base\AxResource.h"
#include "..\..\Primitive\AxMatrix.h"

#include "AxTransformLayer.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxTransformLayer>;

class AXDLLCLASS AxTransform
	: public AxResource
{
private:
	static const unsigned int SerializationId_Pivot				= 0x21111002;
	static const unsigned int SerializationId_Transform			= 0x21112002;
	static const unsigned int SerializationId_TransformLayers	= 0x21113002;
	static const unsigned int SerializationId_TransformLayer	= 0x21113102;
	static const unsigned int SerializationId_BindInverse		= 0x21114002;

	AxMatrix oldPivot, pivotInverse, m;

public:
	static const int classId = (AxResourceType_Transform << 16) | 0;

	static const int propertyIndex_Bone			= AxResource::propertyIndex_ChildPropertiesIndex + 0;
	static const int propertyIndex_BoneIndex	= AxResource::propertyIndex_ChildPropertiesIndex + 1;

	AxMatrix boneBindPoseInverse, pivot, transform, localMatrix, worldMatrix, pivotedWorldMatrix;
	AxList<AxTransformLayer> transformLayers;

	AxTransform *parent;

	bool _updateState;


	AxTransform(void);
	virtual ~AxTransform(void);

	void InsertTransformLayer(AxTransformOperation operation, int index);
	int AddTransformLayer(AxTransformOperation operation);
	void RemoveTransformLayer(int index);
	void ClearTransformLayers();
	void Process(AxTransform *parent = 0);

	AxVector3 GetRightVectorExtrinsic();
	AxVector3 GetRightVectorIntrinsic();
	AxVector3 GetUpVectorExtrinsic();
	AxVector3 GetUpVectorIntrinsic();
	AxVector3 GetForwardVectorExtrinsic();
	AxVector3 GetForwardVectorIntrinsic();
	
	AxVector3 GetPositionExtrinsic();
	AxVector3 GetPositionIntrinsic();
	void SetPositionExtrinsic(AxVector3 &translation);
	void SetPositionIntrinsic(AxVector3 &translation);
	AxVector3 GetRotationExtrinsic();
	AxVector3 GetRotationIntrinsic();
	void SetRotationExtrinsic(AxVector3 &rotation);
	void SetRotationIntrinsic(AxVector3 &rotation);
	AxVector3 GetScalingExtrinsic();
	AxVector3 GetScalingIntrinsic();
	void SetScalingExtrinsic(AxVector3 &scaling);
	void SetScalingIntrinsic(AxVector3 &scaling);

	void TranslateExtrinsic(AxVector3 &translation);
	void TranslateIntrinsic(AxVector3 &translation);
	void RotateExtrinsic(AxVector3 &angles);
	void RotateIntrinsic(AxVector3 &angles);
	void RotateAroundExtrinsic(AxVector3 &axis, float angle);
	void RotateAroundIntrinsic(AxVector3 &axis, float angle);
	void RotateAroundRayExtrinsic(AxVector3 &rayPoint, AxVector3 &rayVector, float angle);
	void RotateAroundRayIntrinsic(AxVector3 &rayPoint, AxVector3 &rayVector, float angle);
	void ScaleExtrinsic(AxVector3 &scaling);
	void ScaleIntrinsic(AxVector3 &scaling);

	void RelateToIdentity();
	void RelateTo(AxMatrix &parent);
	void RelateTo(AxTransform *parent);

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

