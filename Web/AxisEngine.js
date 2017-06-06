var scriptFiles = 
[
    'Tools/AxMath.js',
    'Tools/AxMem.js',
    'Tools/AxUtils.js',
    'Tools/AxList.js',
    'Tools/AxLinkedList.js',
    'Tools/AxDictionary.js',
    'Tools/AxStack.js',
    'Tools/AxString.js',
    'Tools/AxStrings.js',
    
    'Tools/Platform/AxReferenceParameter.js',
    'Tools/Platform/AxAnimationElement.js',
    'Tools/Platform/AxPlatformUtils.js',
    
    'Tools/Streams/AxStream.js',
    'Tools/Streams/AxMemoryStream.js',
    
    'Engine/Base/AxResourceType.js',
    'Engine/Base/AxEntity.js',
    'Engine/Base/AxEntityDispatcher.js',
    'Engine/Base/AxResource.js',
    'Engine/Base/AxModuleDispatcher.js',
    'Engine/Base/AxClassId.js',

    'Engine/Primitive/AxVector2.js',
    'Engine/Primitive/AxVector3.js',
    'Engine/Primitive/AxVector4.js',
    'Engine/Primitive/AxMatrix.js',
    'Engine/Primitive/AxShadingOperation.js',
    'Engine/Primitive/AxColorOperation.js',
    'Engine/Primitive/AxPCMFormat.js',
    'Engine/Primitive/AxPixelFormat.js',
    'Engine/Primitive/AxRenderState.js',
    'Engine/Primitive/AxVertex.js',
    'Engine/Primitive/AxBoundingVolume.js',
    'Engine/Primitive/AxIntersectionInfo.js',

    'Engine/Properties/AxParameterType.js',
    'Engine/Properties/AxParameter.js',
    'Engine/Properties/AxProperty.js',
    
    'Engine/Utilities/AxMaths.js',
    'Engine/Utilities/AxTimer.js',

    'Engine/Utilities/Normals/AxNormalsGenerator.js',

    'Engine/Utilities/Tangents/AxTangentsGenerator.js',

    'Engine/Utilities/Serialization/AxHierarchyStreamReader.js',
    'Engine/Utilities/Serialization/AxHierarchyStreamWriter.js',
    'Engine/Utilities/Serialization/AxSerializationUtils.js',

    'Engine/Utilities/SmoothingGroups/AxSmoothingGroupsPrimitiveTypes.js',
    'Engine/Utilities/SmoothingGroups/AxSmoothingGroups.js',

    'Engine/Entities/Mesh/AxMesh.js',

    'Engine/Entities/Camera/AxCamera.js',

    'Engine/Entities/Transform/AxTransformOperation.js',
    'Engine/Entities/Transform/AxTransformLayer.js',
    'Engine/Entities/Transform/AxTransform.js',

    'Engine/Entities/Textures/AxTexture2D.js',

    'Engine/Entities/EntitySet/AxEntitySet.js',

    'Engine/Entities/Light/AxLight.js',

    'Engine/Entities/Material/AxMaterialShadingLayer.js',
    'Engine/Entities/Material/AxMaterial.js',

    'Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrame.js',
    'Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameRotationAxis.js',
    'Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameRotationEuler.js',
    'Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameScaling.js',
    'Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFramesTrack.js',
    'Engine/Entities/Mechanisms/KeyFrameAnimation/AxKeyFrameTranslation.js',

    'Engine/Entities/Mechanisms/AxMechanism.js',
    'Engine/Entities/Mechanisms/AxFloatValueMechanism.js',
    'Engine/Entities/Mechanisms/AxPolynomialMechanism.js',
    'Engine/Entities/Mechanisms/AxLinearMechanism.js',
    'Engine/Entities/Mechanisms/AxSineMechanism.js',
    'Engine/Entities/Mechanisms/AxLinearInterpolationMechanism.js',
    'Engine/Entities/Mechanisms/AxKeyFrameAnimationMechanism.js',
    'Engine/Entities/Mechanisms/AxTimeRateMechanism.js',

    'Engine/Entities/Settings/AxSettings.js',

    'FileSystem/AxFileInfo.js',
    'FileSystem/AxFileSystem.js',

    'FileSystem/WebFileSystem/AxWebFileSystem.js',
    'FileSystem/WebFileSystem/AxWebFileSystemDispatcher.js',

    'Graphics/AxDeviceMesh.js',
    'Graphics/AxDeviceTexture2D.js',
    'Graphics/AxDeviceShader.js',
    'Graphics/AxGraphicsDevice.js',

    'Graphics/WebGL/DynamicShading/AxDynamicShaderBlockDefinition.js',
    'Graphics/WebGL/DynamicShading/AxDynamicShaderWriter.js',

    'Graphics/WebGL/AxWebGLMesh.js',
    'Graphics/WebGL/AxWebGLTexture2D.js',
    'Graphics/WebGL/AxWebGLShader.js',
    'Graphics/WebGL/AxWebGLGraphicsDevice.js',
    'Graphics/WebGL/AxWebGLGraphicsDeviceDispatcher.js',
    
    'Media/AxMedia.js',

    'Media/Scene/AxScene.js',
    
    'Media/Scene/Native/AxNativeSceneId.js',
    'Media/Scene/Native/AxNativeSceneWriter.js',
    'Media/Scene/Native/AxNativeSceneReader.js',
    'Media/Scene/Native/AxNativeScene.js',
    'Media/Scene/Native/AxNativeSceneDispatcher.js',
    
    'Media/Image/AxImage.js',

    'Media/Image/WebImage/AxWebImage.js',
    'Media/Image/WebImage/AxWebImageDispatcher.js',
    'Media/Image/Native/AxNativeImage.js',
    'Media/Image/Native/AxNativeImageDispatcher.js',
    
    'Engine/AxTraceParameters.js',
    'Engine/AxTraceEvents.js',
    'Engine/AxRenderEvents.js',
    'Engine/AxScreenPickEvents.js',
    'Engine/AxRayIntersectionEvents.js',
    'Engine/AxStandardEntityDispatcher.js',
    'Engine/AxSerializationParameters.js',
    'Engine/Axis.js'
];

/**
 * Gets fired when a script file Axis web library progresses on loading
 * @callback ScriptFilesLoadProgressCallback
 * @param {Number} Percentage of loading progress
 * @param {string} Url of the file just loaded
 */

/**
 * Gets fired when Axis is fully loaded by the AxisInitialize method
 * @callback ScriptFilesLoadedCallback
 */

/**
 * Initializes Axis web engine by loading all necessary files of the library
 * @param {!ScriptFilesLoadedCallbackType} onLoaded Callback which gets fired once all libraries have been loaded and axis is ready to go
 * @param {!ScriptFilesLoadProgressCallbackType} onProgress Callback which gets fired each time the intialization has progressed. A Number parameter gives the progression percentage
 * @returns {undefined}
 */
function AxisInitialize(onLoaded, onProgress)
{
    LoadScriptFilesSequential(0, onLoaded, onProgress);
}

/**
 * Loads Axis' script files sequentially, starting from the given index
 * For internal use
 * @param {type} fileIndex The index of the script file to start loading from
 * @param {!ScriptFilesLoadedCallback} onLoaded Callback, which gets called when the final file has been loaded
 * @param {!ScriptFilesLoadProgressCallback} onProgress Callback which gets called when a file has been loaded
 */
function LoadScriptFilesSequential(fileIndex, onLoaded, onProgress)
{
    LoadScriptFile(scriptFiles[fileIndex], function(scriptFileUrl)
    {
        fileIndex++;

        if ((typeof onProgress !== 'undefined') && (onProgress !== null))
            onProgress(fileIndex / scriptFiles.length, scriptFileUrl);
        
        if (fileIndex < scriptFiles.length)
            LoadScriptFilesSequential(fileIndex, onLoaded, onProgress);
        else if ((typeof onLoaded !== 'undefined') && (onLoaded !== null))
            onLoaded();
    });
}

/**
 * Gets fired when LoadScriptFile finishes loading a script file
 * @callback ScriptFileLoadedCallback
 * @param {string} The url of the loaded file
 */

/**
 * Loads a javascript file
 * @param {string} scriptFileUrl Url of a script file to be loaded. The url can be of a local or http file
 * @param {ScriptFileLoadedCallback} onLoaded Callback which gets fired after the script file has been loaded
 */
function LoadScriptFile(scriptFileUrl, onLoaded)
{
    var script = document.createElement('script');
    script.type = 'text/javascript';
    script.src = scriptFileUrl;
    // TODO: Supposedly, this should work on IE, wile "onload" should work on others. Test
    ////script.onreadystatechange = function () 
    //{
    //    if (this.readyState == 'complete') // || (this.readyState == 'loaded') but careful not to trigger it twice
    //        onLoaded;
    //}
    script.onload = function()
    {
        if (onLoaded !== null)
            onLoaded(scriptFileUrl);
    };

    var head = document.getElementsByTagName('head')[0];
    head.appendChild(script);
}