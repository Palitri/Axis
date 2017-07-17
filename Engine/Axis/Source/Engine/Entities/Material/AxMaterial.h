#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Tools\AxList.h"

#include "..\..\..\Graphics\AxGraphicsDevice.h"

#include "..\..\Base\AxResource.h"

#include "..\..\Primitive\AxRenderState.h"

#include "AxMaterialShadingLayer.h"

class Axis;

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxMaterialShadingLayer>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxRenderState>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

class AXDLLCLASS AxMaterial
	: public AxResource
{
private:
	static const unsigned int SerializationId_ShadingLayers		= 0x21111003;
	static const unsigned int SerializationId_ShadingLayer		= 0x21111103;
	static const unsigned int SerializationId_RenderStates		= 0x21112003;
	static const unsigned int SerializationId_RenderState		= 0x21112103;

	static AxString CreatePropertyName(const AxString &desiredName, int propertyNumber);

	AxString CreateUniquePropertyName(const AxString &desiredName);

	bool InsertColorOperationProperty(int index, const AxString &name, AxColorOperation colorOp);

	AxList<int> shadingPropertiesAddresses;

	int noCullingPropertyIndex;

public:
	static const int classId = (AxResourceType_Material << 16) | 0;

	static const int propertyIndex_OverrideMaterials = AxResource::propertyIndex_ChildPropertiesIndex + 0;

	Axis *context;

	AxDeviceShader *deviceShader;
    
	AxList<AxMaterialShadingLayer> shadingLayers;
    AxList<AxRenderState> renderStates;
    int lightSourcesCount;
	bool isAnimated;
	bool successfullyCompiled;
	AxString compilationMessage;

    bool updateState;

	int paramAddressWorldMatrix, paramAddressViewMatrix, paramAddressProjectionMatrix,
		paramAddressViewPosition,
		paramAddressLightsPos, paramAddressLightsColor, paramAddressLightsIntensity, paramAddressActiveLightsCount, paramAddressAmbientLight;

	AxMaterial(Axis *context);

	virtual ~AxMaterial(void);

    void CommitRenderStates();
    void InsertShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp, int index);
    int AddShadingLayer(AxShadingOperation shadingOp, AxColorOperation colorOp);
    void RemoveShadingLayer(int index);
    void MoveShadingLayer(int originalIndex, int newIndex);
    void ClearShadingLayers();
	int GetShadingLayersCount(AxShadingOperation shadingOp);
	int GetShadingLayersCount(AxShadingOperation shadingOp, AxColorOperation colorOp);

	bool UpdateShader();

    // Applies to the device shader a parameter of the material
	void ApplyProperty(AxProperty *materialProperty, int shaderAddress);
    // Applies to the device shader all parameters of the material
    void ApplyProperties();
    // Applies to the device shader lights from the context which are nearest to the given location and with respect to the number of light sources in the shader
	void ApplyLights(AxVector3 &location);
    // Applies to the device shader bones from the context and with respect to whether the shader has animation
	void ApplyBones();

	virtual bool Deserialize(AxStream &source);

	virtual void SerializeChunks(AxHierarchyStreamWriter &writer);
	virtual bool DeserializeChunk(AxHierarchyStreamReader &reader);
};

