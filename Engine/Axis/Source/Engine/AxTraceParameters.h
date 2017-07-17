#pragma once

#include "..\AxGlobals.h"

#include "Primitive\AxMatrix.h"

#include "Entities\Mesh\AxMesh.h"
#include "Entities\Camera\AxCamera.h"
#include "Entities\Transform\AxTransform.h"
#include "Entities\Material\AxMaterial.h"
#include "Entities\EntitySet\AxEntitySet.h"
#include "Entities\Mechanisms\AxMechanism.h"
#include "Entities\Textures\AxTexture2D.h"
#include "Entities\Light\AxLight.h"
#include "Entities\SoundEmitter\AxSoundEmitter.h"
#include "Entities\SoundReceptor\AxSoundReceptor.h"

class AXDLLCLASS AxTraceParameters
{
public:
	    AxMesh *meshRef;
        AxEntitySet *meshBranch;
		int meshIndex;

		AxCamera *cameraRef;
        AxEntitySet *cameraBranch;
		int cameraIndex;
        
		AxMaterial *materialRef;
        AxEntitySet *materialBranch;
		int materialIndex;

		AxTransform *transformRef, *parentTransformRef;
        AxEntitySet *transformBranch;
		int transformIndex;

		AxLight *lightRef;
        AxEntitySet *lightBranch;
		int lightIndex;

		AxSoundEmitter *soundEmitterRef;
        AxEntitySet *soundEmitterBranch;
		int soundEmitterIndex;

		AxSoundReceptor *soundReceptorRef;
        AxEntitySet *soundReceptorBranch;
		int soundReceptorIndex;

		AxMechanism *mechanismRef;

		AxTexture2D *textureRef, *renderTargetRef;

		AxEntitySet *entitySetRef;
		AxEntitySet *entitySetBranch;
		int entitySetIndex;

		int traceIndex;

        AxTraceParameters(void);
        AxTraceParameters(AxTraceParameters &parent);
		~AxTraceParameters(void);
};

