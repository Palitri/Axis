#pragma once

#include "Engine\Axis.h"
#include "Engine\AxRayIntersectionEvents.h"
#include "Engine\AxRenderEvents.h"
#include "Engine\AxScreenPickEvents.h"
#include "Engine\AxTraceEvents.h"
#include "Engine\AxTraceParameters.h"
#include "Engine\AxStandardEntityDispatcher.h"

#include "Engine\Base\AxResource.h"
#include "Engine\Base\AxResourceType.h"
#include "Engine\Base\AxEntity.h"
#include "Engine\Base\AxEntityDispatcher.h"
#include "Engine\Base\AxModuleDispatcher.h"
#include "Engine\Base\AxClassId.h"

#include "Engine\Entities\Camera\AxCamera.h"

#include "Engine\Entities\Material\AxMaterial.h"
#include "Engine\Entities\Material\AxMaterialShadingLayer.h"

#include "Engine\Entities\Mechanisms\AxMechanism.h"
#include "Engine\Entities\Mechanisms\AxOrientationMechanism.h"
#include "Engine\Entities\Mechanisms\AxFloatValueMechanism.h"
#include "Engine\Entities\Mechanisms\AxKeyframeAnimationMechanism.h"
#include "Engine\Entities\Mechanisms\AxLinearMechanism.h"
#include "Engine\Entities\Mechanisms\AxLinearInterpolationMechanism.h"
#include "Engine\Entities\Mechanisms\AxPolynomialMechanism.h"
#include "Engine\Entities\Mechanisms\AxPivotalMechanism.h"
#include "Engine\Entities\Mechanisms\AxSineMechanism.h"
#include "Engine\Entities\Mechanisms\AxTimelineAnimationMechanism.h"
#include "Engine\Entities\Mechanisms\AxTimeRateMechanism.h"

#include "Engine\Entities\Mechanisms\Geometry\AxPlaneGeometryMechanism.h"
#include "Engine\Entities\Mechanisms\Geometry\AxBoxGeometryMechanism.h"
#include "Engine\Entities\Mechanisms\Geometry\AxConeGeometryMechanism.h"
#include "Engine\Entities\Mechanisms\Geometry\AxGeosphereGeometryMechanism.h"
#include "Engine\Entities\Mechanisms\Geometry\AxSphereGeometryMechanism.h"
#include "Engine\Entities\Mechanisms\Geometry\AxHeightmapGeometryMechanism.h"
#include "Engine\Entities\Mechanisms\Geometry\AxTorusGeometryMechanism.h"

#include "Engine\Entities\Mechanisms\Texture\AxHueMapTextureMechanism.h"
#include "Engine\Entities\Mechanisms\Texture\AxPlasmaFractalTextureMechanism.h"
#include "Engine\Entities\Mechanisms\Texture\AxNormalMapTextureMechanism.h"
#include "Engine\Entities\Mechanisms\Texture\AxBlurTextureMechanism.h"
#include "Engine\Entities\Mechanisms\Texture\AxCheckerTextureMechanism.h"

#include "Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrame.h"
#include "Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameRotationAxis.h"
#include "Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameRotationEuler.h"
#include "Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameScaling.h"
#include "Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFramesTrack.h"
#include "Engine\Entities\Mechanisms\KeyFrameAnimation\AxKeyFrameTranslation.h"

#include "Engine\Entities\Mesh\AxMesh.h"

#include "Engine\Entities\EntitySet\AxEntitySet.h"

#include "Engine\Entities\Sound\AxSound.h"

#include "Engine\Entities\SoundEmitter\AxSoundEmitter.h"

#include "Engine\Entities\SoundReceptor\AxSoundReceptor.h"

#include "Engine\Entities\Textures\AxTexture2D.h"

#include "Engine\Entities\Script\AxScript.h"

#include "Engine\Entities\Transform\AxTransform.h"
#include "Engine\Entities\Transform\AxTransformOperation.h"
#include "Engine\Entities\Transform\AxTransformLayer.h"

#include "Engine\Entities\Light\AxLight.h"

#include "Engine\Input\AxInput.h"
#include "Engine\Input\AxInputModel.h"
#include "Engine\Input\AxInputControls.h"

#include "Engine\Input\Models\AxWalkInputModel.h"
#include "Engine\Input\Models\AxFlightInputModel.h"
#include "Engine\Input\Models\AxOrbitInputModel.h"
#include "Engine\Input\Models\AxRotationInputModel.h"

#include "Engine\Primitive\AxModule.h"
#include "Engine\Primitive\AxBoundingVolume.h"
#include "Engine\Primitive\AxColorOperation.h"
#include "Engine\Primitive\AxIntersectionInfo.h"
#include "Engine\Primitive\AxMatrix.h"
#include "Engine\Primitive\AxPixelFormat.h"
#include "Engine\Primitive\AxRenderState.h"
#include "Engine\Primitive\AxVector2.h"
#include "Engine\Primitive\AxVector3.h"
#include "Engine\Primitive\AxVector4.h"
#include "Engine\Primitive\AxVertex.h"

#include "Engine\Properties\AxParameter.h"
#include "Engine\Properties\AxParameterType.h"
#include "Engine\Properties\AxProperty.h"

#include "Engine\Utilities\AxMaths.h"
#include "Engine\Utilities\AxTimer.h"
#include "Engine\Utilities\AxLogging.h"

#include "Engine\Utilities\Serialization\AxHierarchyStreamReader.h"
#include "Engine\Utilities\Serialization\AxHierarchyStreamWriter.h"
#include "Engine\Utilities\Serialization\AxSerializationUtils.h"

#include "Engine\Utilities\Geometry\AxBoxGeometry.h"
#include "Engine\Utilities\Geometry\AxConvexHullGeometry.h"
#include "Engine\Utilities\Geometry\AxCopyGeometry.h"
#include "Engine\Utilities\Geometry\AxHeightMapGeometry.h"
#include "Engine\Utilities\Geometry\AxIcosahedronGeometry.h"
#include "Engine\Utilities\Geometry\AxMergeGeometry.h"
#include "Engine\Utilities\Geometry\AxPlaneGeometry.h"
#include "Engine\Utilities\Geometry\AxGeosphereGeometry.h"
#include "Engine\Utilities\Geometry\AxSphereGeometry.h"
#include "Engine\Utilities\Geometry\AxPrismGeometry.h"
#include "Engine\Utilities\Geometry\AxTorusGeometry.h"

#include "Engine\Utilities\Normals\AxNormalsGenerator.h"

#include "Engine\Utilities\Tangents\AxTangentsGenerator.h"

#include "Engine\Utilities\TexCoords\AxTransformTexCoords.h"
#include "Engine\Utilities\TexCoords\AxPlanarTexCoords.h"
#include "Engine\Utilities\TexCoords\AxSphericalTexCoords.h"
#include "Engine\Utilities\TexCoords\AxTexCoordChannels.h"

#include "Engine\Utilities\SmoothingGroups\AxSmoothingGroups.h"
#include "Engine\Utilities\SmoothingGroups\AxSmoothingGroupsPrimitiveTypes.h"

#include "Engine\Utilities\SubMesh\AxSubMesh.h"
#include "Engine\Utilities\SubMesh\AxSubMeshPrimitiveTypes.h"

#include "Engine\Utilities\Imaging\AxGenerateNormalMapImage.h"
#include "Engine\Utilities\Imaging\AxHueMapImage.h"
#include "Engine\Utilities\Imaging\AxPlasmaFractalImage.h"
#include "Engine\Utilities\Imaging\AxCheckerPatternImage.h"
#include "Engine\Utilities\Imaging\AxGaussianBlurImage.h"


#include "FileSystem\AxFileInfo.h"
#include "FileSystem\AxFileSystem.h"

#include "FileSystem\PhysicalFileSystem\AxPhysicalFileSystem.h"
#include "FileSystem\PhysicalFileSystem\AxPhysicalFileSystemDispatcher.h"

#include "Graphics\AxDeviceMesh.h"
#include "Graphics\AxDeviceTexture2D.h"
#include "Graphics\AxDeviceShader.h"
#include "Graphics\AxGraphicsDevice.h"

#include "Graphics\DeviceIndependent\AxDeviceIndependentMesh.h"
#include "Graphics\DeviceIndependent\AxDeviceIndependentShader.h"

#include "Graphics\Soras\AxSorasGraphicsDeviceDispatcher.h"
#include "Graphics\Soras\AxSorasGraphicsDevice.h"
#include "Graphics\Soras\AxSorasMesh.h"
#include "Graphics\Soras\AxSorasShader.h"
#include "Graphics\Soras\AxSorasTexture2D.h"

#include "Graphics\Soras\Engine\Soras.h"
#include "Graphics\Soras\Engine\SrMesh.h"
#include "Graphics\Soras\Engine\SrRenderThread.h"
#include "Graphics\Soras\Engine\SrTexture.h"

#include "Graphics\Soras\Engine\Primitive\SrARGB.h"
#include "Graphics\Soras\Engine\Primitive\SrARGBInt.h"
#include "Graphics\Soras\Engine\Primitive\SrCullModes.h"
#include "Graphics\Soras\Engine\Primitive\SrLight.h"
#include "Graphics\Soras\Engine\Primitive\SrMaterial.h"
#include "Graphics\Soras\Engine\Primitive\SrMatrix.h"
#include "Graphics\Soras\Engine\Primitive\SrShaderOperations.h"
#include "Graphics\Soras\Engine\Primitive\SrShadingParameters.h"
#include "Graphics\Soras\Engine\Primitive\SrTextureSamplers.h"
#include "Graphics\Soras\Engine\Primitive\SrUnit.h"
#include "Graphics\Soras\Engine\Primitive\SrVector2Int.h"
#include "Graphics\Soras\Engine\Primitive\SrVector3.h"
#include "Graphics\Soras\Engine\Primitive\SrVector3Int.h"
#include "Graphics\Soras\Engine\Primitive\SrVector4.h"
#include "Graphics\Soras\Engine\Primitive\SrVector4Int.h"
#include "Graphics\Soras\Engine\Primitive\SrVertex.h"

#include "Input\AxInputDevice.h"

#include "Input\WindowsForms\AxWindowsFormsInputDevice.h"
#include "Input\WindowsForms\AxWindowsFormsInputDeviceDispatcher.h"

#include "Media\AxMedia.h"

#include "Media\Image\AxImage.h"

#include "Media\Scene\AxScene.h"

#include "Media\Sound\AxSoundMedia.h"

#include "Media\Scene\Native\AxNativeSceneId.h"
#include "Media\Scene\Native\AxNativeSceneReader.h"
#include "Media\Scene\Native\AxNativeSceneWriter.h"
#include "Media\Scene\Native\AxNativeScene.h"
#include "Media\Scene\Native\AxNativeSceneDispatcher.h"

#include "Media\Scene\NativeJS\AxNativeJSScene.h"
#include "Media\Scene\NativeJS\AxNativeJSSceneDispatcher.h"

#include "Media\Scene\3ds\Ax3dsScene.h"
#include "Media\Scene\3ds\Ax3dsSceneDispatcher.h"

#include "Media\Scene\3ds\File3ds\AxFile3ds.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsBaseObject.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsCamera.h"
#include "Media\Scene\3ds\File3ds\AxFile3ds.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsHierarchy.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsKeyFrames.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsLight.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsMaterial.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsMesh.h"
#include "Media\Scene\3ds\File3ds\AxFile3dsPrimitiveTypes.h"

#include "Media\Scene\Obj\AxObjScene.h"
#include "Media\Scene\Obj\AxObjSceneDispatcher.h"

#include "Media\Scene\Obj\FileObj\AxFileObj.h"
#include "Media\Scene\Obj\FileObj\AxFileObjMaterial.h"
#include "Media\Scene\Obj\FileObj\AxFileObjMesh.h"
#include "Media\Scene\Obj\FileObj\AxFileObjPrimitiveTypes.h"

#include "Media\Sound\Wav\AxWavSoundMedia.h"
#include "Media\Sound\Wav\AxWavSoundMediaDispatcher.h"

#include "Media\Sound\Wav\WavFile\AxWavFile.h"

#include "Media\Image\Native\AxNativeImage.h"
#include "Media\Image\Native\AxNativeImageDispatcher.h"

#include "Script\Parsing\AxCodeBuilder.h"
#include "Script\Parsing\AxScriptToken.h"
#include "Script\Parsing\AxScriptTokens.h"

#include "Script\Processing\AxCodeProcessor.h"

#include "Sound\AxAudioDevice.h"
#include "Sound\AxAudioDeviceSound.h"
#include "Sound\AxAudioDeviceSoundEmitter.h"
#include "Sound\AxAudioDeviceSoundReceptor.h"

#include "Tools\AxDictionary.h"
#include "Tools\AxIntegralQuotientDistribution.h"
#include "Tools\AxLinkedList.h"
#include "Tools\AxSortedLinkedList.h"	
#include "Tools\AxList.h"
#include "Tools\AxMath.h"
#include "Tools\AxMem.h"
#include "Tools\AxRandomPool.h"
#include "Tools\AxStack.h"
#include "Tools\AxString.h"
#include "Tools\AxStrings.h"

#include "Tools\Networking\TCPIP\AxTcpIpConnection.h"
#include "Tools\Networking\TCPIP\AxTcpIpListener.h"
#include "Tools\Networking\TCPIP\AxTcpIpClient.h"
#include "Tools\Networking\TCPIP\AxTcpIpServer.h"

#include "Tools\Networking\WebServer\AxWebRequest.h"
#include "Tools\Networking\WebServer\AxWebResponse.h"
#include "Tools\Networking\WebServer\AxWebServer.h"

#include "Tools\Networking\WebServer\WebPageComponents\AxWebPageComponent.h"
#include "Tools\Networking\WebServer\WebPageComponents\AxWebPageFileListing.h"
#include "Tools\Networking\WebServer\WebPageComponents\AxWebPageHostName.h"

#include "Tools\Platform\AxDrawSurface.h"
#include "Tools\Platform\AxPlatformUtils.h"
#include "Tools\Platform\AxWindowsForm.h"

#include "Tools\Streams\AxStream.h"
#include "Tools\Streams\AxFileStream.h"
#include "Tools\Streams\AxMemoryStream.h"
#include "Tools\Streams\AxSubStream.h"

#include "Tools\Threading\AxMutex.h"
#include "Tools\Threading\AxThread.h"
#include "Tools\Threading\AxThreadsPool.h"
#include "Tools\Threading\AxThreadsPoolThread.h"
