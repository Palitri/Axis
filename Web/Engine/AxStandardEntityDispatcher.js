/**
 * Creates a new entity dispatcher for the standard entities in Axis
 * The entity dispatcher creates instances of all the AxResource descendant classes by providing the respective classId
 * @constructor
 */
function AxStandardEntityDispatcher()
{
}

/**
 * Creates an entity of the given typeId
 * @param {Axis} context Context to use for the creation of the entity
 * @param {Integer} typeId Identificator for the entity type to create
 * @return {AxEntity} The created entity object
 */
AxStandardEntityDispatcher.prototype.CreateEntity = function(context, typeId)
{
    switch (typeId)
    {
        case AxMesh.classId:
        {
            return new AxMesh(context);
        }

        case AxTransform.classId:
        {
            return new AxTransform();
        }

        case AxCamera.classId:
        {
            return new AxCamera(context);
        }

        case AxMaterial.classId:
        {
            return new AxMaterial(context);
        }

        case AxEntitySet.classId:
        {
            return new AxEntitySet();
        }

        case AxTexture2D.classId:
        {
            return new AxTexture2D(context);
        }

        case AxSettings.classId:
        {
            return new AxSettings();
        }

        case AxFloatValueMechanism.classId:
        {
            return new AxFloatValueMechanism(0.0);
        }

        case AxPolynomialMechanism.classId:
        {
            return new AxPolynomialMechanism(0.0);
        }
        
        case AxLinearMechanism.classId:
        {
            return new AxLinearMechanism(0.0, 1.0, 1.0, false);
        }

        case AxSineMechanism.classId:
        {
            return new AxSineMechanism(0.0, 1.0, 0.0, 1.0);
        }

        case AxLinearInterpolationMechanism.classId:
        {
            return new AxLinearInterpolationMechanism(0.0, 1.0, 0.0, 1.0, false);
        }

        case AxKeyframeAnimationMechanism.classId:
        {
            return new AxKeyframeAnimationMechanism(context);
        }

        case AxTimelineAnimationMechanism.classId:
        {
            return new AxTimelineAnimationMechanism();
        }

        case AxTimeRateMechanism.classId:
        {
            return new AxTimeRateMechanism(context);
        }
        
        
        case AxPlaneGeometryMechanism.classId:
        {
            return new AxPlaneGeometryMechanism(context);
        }
        
        case AxBoxGeometryMechanism.classId:
        {
            return new AxBoxGeometryMechanism(context);
        }
        
        case AxConeGeometryMechanism.classId:
        {
            return new AxConeGeometryMechanism(context);
        }
        
        case AxGeosphereGeometryMechanism.classId:
        {
            return new AxGeosphereGeometryMechanism(context);
        }
        
        case AxSphereGeometryMechanism.classId:
        {
            return new AxSphereGeometryMechanism(context);
        }
        
        case AxHeightmapGeometryMechanism.classId:
        {
            return new AxHeightmapGeometryMechanism(context);
        }
        
        case AxTorusGeometryMechanism.classId:
        {
            return new AxTorusGeometryMechanism(context);
        }
        
        
        /*
        case (AxResourceType.Mechanism << 16) | 8:
        {
            return new AxPivotalMechanism();
        }

        case (AxResourceType.Mechanism << 16) | 10:
        {
            return new AxTimeRateMechanism(context);
        }
        */

        case AxLight.classId:
        {
            return new AxLight();
        }

        /*
        case AxSound.classId:
        {
            return new AxSound(context);
        }

        case AxSoundEmitter.classId:
        {
            return new AxSoundEmitter(context);
        }

        case AxSoundReceptor.classId:
        {
            return new AxSoundReceptor(context);
        }
        */

        case AxKeyFrameTranslation.classId:
        {
            return new AxKeyFrameTranslation();
        }

        /*
        case AxScript.classId:
        {
            return new AxScript(context);
        }
        */

        case AxKeyFrameRotationAxis.classId:
        {
            return new AxKeyFrameRotationAxis();
        }

        case AxKeyFrameRotationEuler.classId:
        {
            return new AxKeyFrameRotationEuler();
        }

        case AxKeyFrameScaling.classId:
        {
            return new AxKeyFrameScaling();
        }

    }

    return null;
};