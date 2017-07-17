//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxStandardEntityDispatcher.h"

#include "Entities\Camera\AxCamera.h"
#include "Entities\Material\AxMaterial.h"
#include "Entities\Mechanisms\AxFloatValueMechanism.h"
#include "Entities\Mechanisms\AxPolynomialMechanism.h"
#include "Entities\Mechanisms\AxLinearMechanism.h"
#include "Entities\Mechanisms\AxLinearInterpolationMechanism.h"
#include "Entities\Mechanisms\AxSineMechanism.h"
#include "Entities\Mechanisms\AxOrientationMechanism.h"
#include "Entities\Mechanisms\AxKeyframeAnimationMechanism.h"
#include "Entities\Mechanisms\AxPivotalMechanism.h"
#include "Entities\Mechanisms\AxTimeRateMechanism.h"
#include "Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameRotationAxis.h"
#include "Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameRotationEuler.h"
#include "Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameScaling.h"
#include "Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameTranslation.h"
#include "Entities\Mechanisms\Geometry\AxPlaneGeometryMechanism.h"
#include "Entities\Mechanisms\Geometry\AxBoxGeometryMechanism.h"
#include "Entities\Mechanisms\Geometry\AxConeGeometryMechanism.h"
#include "Entities\Mechanisms\Geometry\AxGeosphereGeometryMechanism.h"
#include "Entities\Mechanisms\Geometry\AxSphereGeometryMechanism.h"
#include "Entities\Mechanisms\Geometry\AxHeightmapGeometryMechanism.h"
#include "Entities\Mechanisms\Geometry\AxTorusGeometryMechanism.h"
#include "Entities\Mechanisms\Texture\AxHueMapTextureMechanism.h"
#include "Entities\Mechanisms\Texture\AxPlasmaFractalTextureMechanism.h"
#include "Entities\Mechanisms\Texture\AxNormalMapTextureMechanism.h"
#include "Entities\Mechanisms\Texture\AxBlurTextureMechanism.h"
#include "Entities\Mechanisms\Texture\AxCheckerTextureMechanism.h"
#include "Entities\Mesh\AxMesh.h"
#include "Entities\EntitySet\AxEntitySet.h"
#include "Entities\Textures\AxTexture2D.h"
#include "Entities\Transform\AxTransform.h"
#include "Entities\Light\AxLight.h"
#include "Entities\Sound\AxSound.h"
#include "Entities\SoundEmitter\AxSoundEmitter.h"
#include "Entities\SoundReceptor\AxSoundReceptor.h"
#include "Entities\Script\AxScript.h"
#include "Entities\Settings\AxSettings.h"

AxStandardEntityDispatcher::AxStandardEntityDispatcher(void)
{
}


AxStandardEntityDispatcher::~AxStandardEntityDispatcher(void)
{
}

AxEntity *AxStandardEntityDispatcher::CreateEntity(Axis *context, int typeId)
{
	switch (typeId)
	{
		case AxMesh::classId:
		{
			return new AxMesh(context);
		}

		case AxTransform::classId:
		{
			return new AxTransform();
		}

		case AxCamera::classId:
		{
			return new AxCamera(context);
		}

		case AxMaterial::classId:
		{
			return new AxMaterial(context);
		}

		case AxEntitySet::classId:
		{
			return new AxEntitySet();
		}

		case AxTexture2D::classId:
		{
			return new AxTexture2D(context);
		}

		case AxSettings::classId:
		{
			return new AxSettings();
		}

		case AxFloatValueMechanism::classId:
		{
			return new AxFloatValueMechanism(0);
		}

		case AxPolynomialMechanism::classId:
		{
			return new AxPolynomialMechanism(0);
		}

		case AxLinearMechanism::classId:
		{
			return new AxLinearMechanism(0.0, 1.0, 1.0, false, false, false);
		}

		case AxSineMechanism::classId:
		{
			return new AxSineMechanism(0.0, 1.0, 0.0, 1.0);
		}

		case AxLinearInterpolationMechanism::classId:
		{
			return new AxLinearInterpolationMechanism(0.0, 1.0, 0.0, 1.0, false, false);
		}

		case AxOrientationMechanism::classId:
		{
			return new AxOrientationMechanism();
		}

		case AxKeyframeAnimationMechanism::classId:
		{
			return new AxKeyframeAnimationMechanism(context);
		}

		case AxPivotalMechanism::classId:
		{
			return new AxPivotalMechanism();
		}


		case AxTimeRateMechanism::classId:
		{
			return new AxTimeRateMechanism(context);
		}

		
		case AxPlaneGeometryMechanism::classId:
		{
			return new AxPlaneGeometryMechanism();
		}

		case AxBoxGeometryMechanism::classId:
		{
			return new AxBoxGeometryMechanism();
		}

		case AxConeGeometryMechanism::classId:
		{
			return new AxConeGeometryMechanism();
		}

		case AxGeosphereGeometryMechanism::classId:
		{
			return new AxGeosphereGeometryMechanism();
		}

		case AxSphereGeometryMechanism::classId:
		{
			return new AxSphereGeometryMechanism();
		}

		case AxHeightmapGeometryMechanism::classId:
		{
			return new AxHeightmapGeometryMechanism();
		}

		case AxTorusGeometryMechanism::classId:
		{
			return new AxTorusGeometryMechanism();
		}


		case AxHueMapTextureMechanism::classId:
		{
			return new AxHueMapTextureMechanism();
		}

		case AxPlasmaFractalTextureMechanism::classId:
		{
			return new AxPlasmaFractalTextureMechanism();
		}

		case AxNormalMapTextureMechanism::classId:
		{
			return new AxNormalMapTextureMechanism();
		}

		case AxBlurTextureMechanism::classId:
		{
			return new AxBlurTextureMechanism();
		}

		case AxCheckerTextureMechanism::classId:
		{
			return new AxCheckerTextureMechanism();
		}

		case AxLight::classId:
		{
			return new AxLight();
		}

		case AxSound::classId:
		{
			return new AxSound(context);
		}

		case AxSoundEmitter::classId:
		{
			return new AxSoundEmitter(context);
		}

		case AxSoundReceptor::classId:
		{
			return new AxSoundReceptor(context);
		}

		case AxKeyFrameTranslation::classId:
		{
			return new AxKeyFrameTranslation();
		}

		case AxScript::classId:
		{
			return new AxScript(context);
		}


		case AxKeyFrameRotationAxis::classId:
		{
			return new AxKeyFrameRotationAxis();
		}

		case AxKeyFrameRotationEuler::classId:
		{
			return new AxKeyFrameRotationEuler();
		}

		case AxKeyFrameScaling::classId:
		{
			return new AxKeyFrameScaling();
		}

	}

	return 0;
}