#include <iostream>
#include <fstream>
#include <string>

const int jsFilesCount = 148;
const char *jsFiles[jsFilesCount] = {
    "Supplemental/AxisLicense-full.js",
    "Supplemental/Supplemental.js",

	"Tools/AxMath.js",
    "Tools/AxMem.js",
    "Tools/AxUtils.js",
    "Tools/AxList.js",
    "Tools/AxLinkedList.js",
    "Tools/AxDictionary.js",
    "Tools/AxStack.js",
    "Tools/AxString.js",
    "Tools/AxStrings.js",
    
	"Tools/Networking/WebSockets/AxWebSocketClient.js",
    "Tools/Networking/Exchange/AxExchangeClient.js",

	"Tools/Platform/AxReferenceParameter.js",
    "Tools/Platform/AxAnimationElement.js",
    "Tools/Platform/AxPlatformUtils.js",
	"Tools/Platform/AxViewport.js",
    
    "Tools/Streams/AxStream.js",
    "Tools/Streams/AxMemoryStream.js",
    "Tools/Streams/AxSubStream.js",
    
    "Engine/Base/AxResourceType.js",
    "Engine/Base/AxEntity.js",
    "Engine/Base/AxEntityDispatcher.js",
    "Engine/Base/AxResource.js",
    "Engine/Base/AxModuleDispatcher.js",
    "Engine/Base/AxClassId.js",

    "Engine/Primitive/AxVector2.js",
    "Engine/Primitive/AxVector3.js",
    "Engine/Primitive/AxVector4.js",
    "Engine/Primitive/AxMatrix.js",
    "Engine/Primitive/AxShadingOperation.js",
    "Engine/Primitive/AxColorOperation.js",
    "Engine/Primitive/AxPCMFormat.js",
    "Engine/Primitive/AxPixelFormat.js",
    "Engine/Primitive/AxRenderState.js",
    "Engine/Primitive/AxVertex.js",
    "Engine/Primitive/AxBoundingVolume.js",
    "Engine/Primitive/AxIntersectionInfo.js",

    "Engine/Properties/AxParameterType.js",
    "Engine/Properties/AxParameter.js",
    "Engine/Properties/AxProperty.js",
    
    "Engine/Utilities/AxMaths.js",
    "Engine/Utilities/AxTimer.js",

    "Engine/Utilities/Geometry/AxBoxGeometry.js",
    "Engine/Utilities/Geometry/AxConvexHullGeometry.js",
    "Engine/Utilities/Geometry/AxCopyGeometry.js",
    "Engine/Utilities/Geometry/AxHeightMapGeometry.js",
    "Engine/Utilities/Geometry/AxIcosahedronGeometry.js",
    "Engine/Utilities/Geometry/AxMergeGeometry.js",
    "Engine/Utilities/Geometry/AxPlaneGeometry.js",
    "Engine/Utilities/Geometry/AxGeosphereGeometry.js",
    "Engine/Utilities/Geometry/AxSphereGeometry.js",
    "Engine/Utilities/Geometry/AxPrismGeometry.js",
    "Engine/Utilities/Geometry/AxTorusGeometry.js",
	
	"Engine/Utilities/Normals/AxNormalsGenerator.js",

    "Engine/Utilities/Tangents/AxTangentsGenerator.js",

    "Engine/Utilities/TexCoords/AxTransformTexCoords.js",
    "Engine/Utilities/TexCoords/AxPlanarTexCoords.js",
    "Engine/Utilities/TexCoords/AxSphericalTexCoords.js",

	"Engine/Utilities/Serialization/AxHierarchyStreamReader.js",
    "Engine/Utilities/Serialization/AxHierarchyStreamWriter.js",
    "Engine/Utilities/Serialization/AxSerializationUtils.js",

    "Engine/Utilities/SmoothingGroups/AxSmoothingGroupsPrimitiveTypes.js",
    "Engine/Utilities/SmoothingGroups/AxSmoothingGroups.js",

    "Engine/Entities/Mesh/AxMesh.js",

    "Engine/Entities/Camera/AxCamera.js",

    "Engine/Entities/Transform/AxTransformOperation.js",
    "Engine/Entities/Transform/AxTransformLayer.js",
    "Engine/Entities/Transform/AxTransform.js",

    "Engine/Entities/Textures/AxTexture2D.js",

    "Engine/Entities/EntitySet/AxEntitySet.js",

    "Engine/Entities/Light/AxLight.js",

    "Engine/Entities/Material/AxMaterialShadingLayer.js",
    "Engine/Entities/Material/AxMaterial.js",

	"Engine/Entities/Mechanisms/AxMechanism.js",
	"Engine/Entities/Mechanisms/AxPropertyChangeNotifiedMechanism.js",
    "Engine/Entities/Mechanisms/AxFloatValueMechanism.js",
    "Engine/Entities/Mechanisms/AxPolynomialMechanism.js",
    "Engine/Entities/Mechanisms/AxLinearMechanism.js",
    "Engine/Entities/Mechanisms/AxSineMechanism.js",
    "Engine/Entities/Mechanisms/AxLinearInterpolationMechanism.js",
    "Engine/Entities/Mechanisms/AxKeyFrameAnimationMechanism.js",
    "Engine/Entities/Mechanisms/AxTimelineAnimationMechanism.js",
    "Engine/Entities/Mechanisms/AxTimeRateMechanism.js",

    "Engine/Entities/Mechanisms/Geometry/AxPlaneGeometryMechanism.js",
    "Engine/Entities/Mechanisms/Geometry/AxBoxGeometryMechanism.js",
    "Engine/Entities/Mechanisms/Geometry/AxConeGeometryMechanism.js",
    "Engine/Entities/Mechanisms/Geometry/AxGeosphereGeometryMechanism.js",
    "Engine/Entities/Mechanisms/Geometry/AxSphereGeometryMechanism.js",
    "Engine/Entities/Mechanisms/Geometry/AxHeightmapGeometryMechanism.js",
    "Engine/Entities/Mechanisms/Geometry/AxTorusGeometryMechanism.js",

    "Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrame.js",
    "Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameRotationAxis.js",
    "Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameRotationEuler.js",
    "Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameScaling.js",
    "Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFramesTrack.js",
    "Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameTranslation.js",

	"Engine/Entities/Settings/AxSettings.js",

	"Engine/Input/AxInput.js",
	"Engine/Input/AxInputControls.js",
	"Engine/Input/AxInputModel.js",

	"Engine/Input/Models/AxFlightInputModel.js",
	"Engine/Input/Models/AxOrbitInputModel.js",
	"Engine/Input/Models/AxRotationInputModel.js",
	"Engine/Input/Models/AxWalkInputModel.js",
    "Engine/Input/Models/AxPropertyLinkInputModel.js",

    "FileSystem/AxFileInfo.js",
    "FileSystem/AxFileSystem.js",

    "FileSystem/WebFileSystem/AxWebFileSystem.js",
    "FileSystem/WebFileSystem/AxWebFileSystemDispatcher.js",

    "Graphics/AxDeviceMesh.js",
    "Graphics/AxDeviceTexture2D.js",
    "Graphics/AxDeviceShader.js",
    "Graphics/AxGraphicsDevice.js",

    "Graphics/DeviceIndependent/AxDeviceIndependentMesh.js",

	"Graphics/WebGL/DynamicShading/AxDynamicShaderBlockDefinition.js",
    "Graphics/WebGL/DynamicShading/AxDynamicShaderWriter.js",

    "Graphics/WebGL/AxWebGLMesh.js",
    "Graphics/WebGL/AxWebGLTexture2D.js",
    "Graphics/WebGL/AxWebGLShader.js",
    "Graphics/WebGL/AxWebGLGraphicsDevice.js",
    "Graphics/WebGL/AxWebGLGraphicsDeviceDispatcher.js",

	"Input/AxInputDevice.js",

	"Input/HtmlCanvas/AxHtmlCanvasInputDevice.js",
	"Input/HtmlCanvas/AxHtmlCanvasInputDispatcher.js",
    "Input/HtmlGamepad/AxHtmlGamepadInputDevice.js",
    "Input/HtmlGamepad/AxHtmlGamepadInputDispatcher.js",
    
    "Media/AxMedia.js",

    "Media/Scene/AxScene.js",
    
    "Media/Scene/Native/AxNativeSceneId.js",
    "Media/Scene/Native/AxNativeSceneWriter.js",
    "Media/Scene/Native/AxNativeSceneReader.js",
    "Media/Scene/Native/AxNativeScene.js",
    "Media/Scene/Native/AxNativeSceneDispatcher.js",
    
    "Media/Image/AxImage.js",

    "Media/Image/WebImage/AxWebImage.js",
    "Media/Image/WebImage/AxWebImageDispatcher.js",
    "Media/Image/Native/AxNativeImage.js",
    "Media/Image/Native/AxNativeImageDispatcher.js",
    
    "Engine/AxTraceParameters.js",
    "Engine/AxTraceEvents.js",
    "Engine/AxRenderEvents.js",
    "Engine/AxScreenPickEvents.js",
    "Engine/AxRayIntersectionEvents.js",
    "Engine/AxStandardEntityDispatcher.js",
    "Engine/AxSerializationParameters.js",
	"Engine/Axis.js",

	"Tools/Platform/HTMLAxisViewportElement.js"
};

char* defaultProjectRootDir = "";
char* defaultMergedFileName = "AxisEngine-full.js";

std::string projectRootDir;
std::string mergedFileName;
std::string minificationCommand;

bool silent = false;


void ParseInputParams(int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "/?") == 0)
		{
			std::cout << 
				"Axis engine js merge application\r\n"
				"-merged <File Name> - Optional, sets the merged file name without path"
				"-root <Dir> - Optional, sets the root directory of the engine";
		}

		if (strcmp(argv[i], "-silent") == 0)
		{
			silent = true;
		}

		if ((strcmp(argv[i], "-merged") == 0) && (i < argc - 1))
		{
			i++;
			mergedFileName = argv[i];
		}

		if ((strcmp(argv[i], "-root") == 0) && (i < argc - 1))
		{
			i++;
			projectRootDir = argv[i];
		}
	}
}

int main(int argc, char *argv[])
{
	projectRootDir = defaultProjectRootDir;
	mergedFileName = defaultMergedFileName;

	ParseInputParams(argc, argv);

	std::string outputFileFullName = projectRootDir + "\\" + mergedFileName;

	std::ofstream outputFile(outputFileFullName, std::ios::binary);

	for (int i = 0; i < jsFilesCount; i++)
	{
		if (!silent)
		{
			std::cout << jsFiles[i] << "\r\n";
		}

		std::string inputFileName = projectRootDir + "\\" + jsFiles[i];
		std::ifstream  jsFile(inputFileName, std::ios::binary);
		outputFile << jsFile.rdbuf() << "\r\n";
	}

	return 0;
}