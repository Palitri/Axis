function AxActiveLightInfo()
{
    this.lightRef = null;
    this.lightTransform = new AxMatrix();
    this.lightPos = new AxVector3();
    this.distance = 0.0;
}

/**
 * Creates and initializes an instance of the Axis engine
 * @param {Canvas} windowHandle A canvas object onto which to render
 * @constructor
 */
function Axis(windowHandle)
{
    this.resourceID = 0;
    
    this.windowHandle = windowHandle;
    this.speed = 1.0;
    this.timer = new AxTimer();
    this.cameraFOVMode = AxCamera.FOVMode_Vertical;
    this.viewportWidth = windowHandle.width;
    this.viewportHeight = windowHandle.height;

    this.input = new AxInput();
    this.inputModels = new AxList();
    
    this.settings = new AxSettings();
    
    this.clearOnRenderTarget = false;
    
    this.currentCamera = new AxTraceParameters();

    this.serializationParameters = new AxSerializationParameters();

    this.renderEvents = new AxRenderEvents(this);
    this.rayIntersectionEvents = new AxRayIntersectionEvents(this);
    this.screenPickEvents = new AxScreenPickEvents(this);

    // activeLights should be used as the set of lights currently affecting the scene. It is the list of all active lights, collected while rendering the last frame. activeLightsCollecting should be used to collect active lights while rendering the current frame.
    this.activeLights = new AxList();
    this.activeLightsCollecting = new AxList();

    // Filled by the CalculateNearestLights() method, holds pointers to a number of lights from the activeLights member variable, which are the nearest to a certain point in space, sorted ascending by distance to that point
    this.nearestActiveLights = new AxList();

    this.bones = new AxList();
    this.bones.SetSize(64);
    for (var i = 0; i < this.bones.count; i++)
        this.bones.Set(i, new AxMatrix());

    this.imageMediaDispatchers = new AxList();
    this.soundMediaDispatchers = new AxList();
    this.sceneMediaDispatchers = new AxList();
    this.graphicsDeviceDispatchers = new AxList();
    this.audioDeviceDispatchers = new AxList();
    this.fileSystemDispatchers = new AxList();
    this.inputDeviceDispatchers = new AxList();
    
    this.imageMediaDispatchers.Add(new AxNativeImageDispatcher());
    this.imageMediaDispatchers.Add(new AxWebImageDispatcher());
    
    this.sceneMediaDispatchers.Add(new AxNativeSceneDispatcher());
    
    this.graphicsDeviceDispatchers.Add(new AxWebGLGraphicsDeviceDispatcher());
    
    this.inputDeviceDispatchers.Add(new AxHtmlCanvasInputDispatcher());
    this.inputDeviceDispatchers.Add(new AxHtmlGamepadInputDispatcher());
    
    this.fileSystemDispatchers.Add(new AxWebFileSystemDispatcher());
    

    
    this.entityDispatchers = new AxList();
    
    this.entityDispatchers.Add(new AxStandardEntityDispatcher());

    this.graphicsDevice = null;
    if (this.graphicsDeviceDispatchers.count > 0)
    {
        this.graphicsDevice = this.graphicsDeviceDispatchers.Get(0).CreateObject();
        this.graphicsDevice.SetRenderTargetWindow(this.windowHandle);
    }
    
    this.inputDevices = new AxList();
    for (var i = 0; i < this.inputDeviceDispatchers.count; i++)
    {
        var device = this.inputDeviceDispatchers.Get(i).CreateObject();
        device.SetContext(this);
        this.inputDevices.Add(device);
    }
    
    this.fileSystem = null;
    if (this.fileSystemDispatchers.count > 0)
    {
        this.fileSystem = this.fileSystemDispatchers.Get(0).CreateObject();
    }
    
    this.resources = new AxList();
    this.root = null;
}

Axis.SerializationId_ResourceSerialization = 0x21100000;



/**
 * Gets fired when an object has been loaded
 * Used for callback functions, where an object has to be loaded and returned when loading has finished
 * Generally, a caller initiates the loading, by calling its loading method. The loading method accepts the required parameters, along with the callback function itself and a context object, which will be passed to the callback function.
 * The context is provided for convenience and can hold any value, but usually it is the object from whithin which the caller's loading method was invoked
 * Useful for example, when loading textures, images and etc.
 * @callback AxisObjectLoadedCallback
 * @param {*} caller The instance of Axis, which did the loading and caused the callback
 * @param {*} context An object holding the context for the callback. It is the object passed to the Loading method
 * @param {*} object If loading was successful, holds the loaded object. Otherwise has a value of null.
 */


/**
 * Imports a scene from a stream
 * The scene can be in any of the registered scene media formats
 * @param {AxStream} stream The stream, which contains the scene serialized data
 * @returns {Boolean} True if the scene was imported successfully
 */
Axis.prototype.ImportScene_1 = function(stream)
{
    var streamPos = stream.position;

    var success = false;
    for (var i = 0; i < this.sceneMediaDispatchers.count; i++)
    {
        stream.Seek(streamPos);
        var scene = this.sceneMediaDispatchers.Get(i).CreateObject();
        scene.SetContext(this);
        success = scene.Load(stream);
        scene = null;

        if (success)
            break;
    }

    this.timer.Tick();

    return success;
};

/**
 * Imports a scene from a file
 * The scene can be in any of the registered scene media formats
 * @param {String|AxString} fileName Name of the file, which contains the scene serialized data
 * @param {*} callback Callback method, called when importing the scene has finished
 */
Axis.prototype.ImportScene_2 = function(fileName, callback)
{
    fileName = AxString.GetAxString(fileName);
    
    var fullFileName = this.GetFullFileName(this.fileSystem.FromNativePathSyntax(fileName));

    this.serializationParameters.fullSourceName = fullFileName;
    this.serializationParameters.rootDir = AxFileSystem.GetFileDir(fullFileName);
    this.serializationParameters.friendlyName = AxFileSystem.GetFileNameWithoutExtension(fullFileName);

    this.fileSystem.OpenFile(this.fileSystem.ToNativePathSyntax(fullFileName), this,
        function(context, stream, path, fileSystem)
        {
            if (stream === null)
                return false;

            var success = context.ImportScene_1(stream);
            
            if (!AxUtils.IsUndefinedOrNull(callback))
                callback(context, fileName, success);
        }
    );
};

/**
 * Imports a scene from a file or a stream
 * The scene can be in any of the registered scene media formats
 * This method combines methods ImportScene_1() and ImportScene_2()
 * @param {String|AxString|AxStream} source In case of importing from a file, it is the file name. In case of importing from a stream, it is the stream
 * @param {*} callback In case of importing from a file, it is a callback method, called when importing the scene has finished. In case of importing from a stream, this argument is not used.
 */
Axis.prototype.ImportScene = function(source, callback)
{
    if (AxUtils.IsInstanceOf(source, AxStream))
        this.ImportScene_1(source);
    else
        this.ImportScene_2(source, callback);
};

/**
 * Processes a given entity recursively. If the entity is an entity set, all the references in the set get processed recursively
 * The way processing is done is influenced by the current traceEvents
 * @param {AxResource} entity The entity which is to be processed
 * @param {AxTraceParameters} parameters The tracing parameters, relevant to the tracing
 */
Axis.prototype.ProcessEntity = function(entity, parameters)
{
    // For backward/forward compatibility. Entities unknown to the current version will result in null references in the entity set
    if (entity === null)
        return;

    if (!entity.properties.Get(AxResource.propertyIndex_Enabled).GetBool())
        return;

    switch (entity.resourceType)
    {
        case AxResourceType.Texture:
        {
            parameters.textureRef = entity;

            this.traceEvents.OnTexture2D(parameters);
            break;
        }

        case AxResourceType.Camera:
        {
            parameters.cameraBranch = parameters.entitySetRef;
            parameters.cameraIndex = parameters.traceIndex;
            parameters.cameraRef = entity;

            if (!AxUtils.IsUndefinedOrNull(parameters.transformRef))
                parameters.cameraRef.BuildMatrix(parameters.transformRef.worldMatrix);
            
            this.currentCamera = new AxTraceParameters(parameters);

            this.traceEvents.OnCamera(parameters);
            break;
        }

        case AxResourceType.Material:
        {
            var materialRes = entity;

            var hasOld = parameters.materialRef !== null;
            var isOldOverride = hasOld && parameters.materialRef.properties.Get(AxMaterial.propertyIndex_OverrideMaterials).GetBool();
            var isCurrentOverride = materialRes.properties.Get(AxMaterial.propertyIndex_OverrideMaterials).GetBool();
            if (!isOldOverride || isCurrentOverride)
            {
                parameters.materialBranch = parameters.entitySetRef;
                parameters.materialIndex = parameters.traceIndex;
                parameters.materialRef = materialRes;
            }

            this.traceEvents.OnMaterial(parameters);

            break;
        }

        case AxResourceType.Transform:
        {
            parameters.transformBranch = parameters.entitySetRef;
            parameters.transformIndex = parameters.traceIndex;
            parameters.transformRef = entity;

            this.traceEvents.OnTransform(parameters);

            if (parameters.transformRef.properties.Get(AxTransform.propertyIndex_Bone).GetBool())
            {
                var boneIndex = parameters.transformRef.properties.Get(AxTransform.propertyIndex_BoneIndex).GetInt();
                if (boneIndex < this.bones.count)
                {
                    AxMatrix.Multiply(this.bones.Get(boneIndex), parameters.transformRef.boneBindPoseInverse, parameters.transformRef.worldMatrix);
                }
            }
            
            break;
        }

        case AxResourceType.Mesh:
        {
            parameters.meshBranch = parameters.entitySetRef;
            parameters.meshIndex = parameters.traceIndex;
            parameters.meshRef = entity;

            if (!parameters.meshRef.active)
                break;

            this.traceEvents.OnMesh(parameters);
            break;
        }

        case AxResourceType.EntitySet:
        {
            var entitySet = entity;

            var parentEntitySet = parameters.entitySetRef;
            var parentEntitySetIndex = parameters.entitySetIndex;

            var multiPass = entitySet.properties.Get(AxEntitySet.propertyIndex_MultiPass).GetBool();
            var branch = entitySet.properties.Get(AxEntitySet.propertyIndex_Branch).GetBool();

            var doMultiPass = multiPass && (parentEntitySet !== null);

            var traceParams = branch ? new AxTraceParameters(parameters) : parameters;

            traceParams.entitySetBranch = parameters.entitySetRef;
            traceParams.entitySetIndex = parameters.traceIndex;
            traceParams.entitySetRef = entity;

            for (var setRefIndex = 0; setRefIndex < entitySet.references.count; setRefIndex++)
            {
                traceParams.traceIndex = setRefIndex;
                this.ProcessEntity(entitySet.references.Get(setRefIndex), traceParams);

                if (doMultiPass)
                {
                    for (var parentSetRefIndex = parentEntitySetIndex + 1; parentSetRefIndex < parentEntitySet.references.count; parentSetRefIndex++)
                    {
                        traceParams.traceIndex = parentSetRefIndex;
                        this.ProcessEntity(parentEntitySet.references.Get(parentSetRefIndex), traceParams);
                    }
                }
            }

            if (branch)
            {
                this.traceEvents.OnExitEntitySet(parameters);
            }

            break;
        }

        case AxResourceType.Mechanism:
        {
            parameters.mechanismRef = entity;

            this.traceEvents.OnMechanism(parameters);
            break;
        }

        case AxResourceType.Light:
        {
            parameters.lightBranch = parameters.entitySetRef;
            parameters.lightIndex = parameters.traceIndex;
            parameters.lightRef = entity;

            this.traceEvents.OnLight(parameters);
            break;
        }

        case AxResourceType.SoundEmitter:
        {
            parameters.soundEmitterBranch = parameters.entitySetRef;
            parameters.soundEmitterIndex = parameters.traceIndex;
            parameters.soundEmitterRef = entity;

            this.traceEvents.OnSoundEmitter(parameters);
            break;
        }

        case AxResourceType.SoundReceptor:
        {
            parameters.soundReceptorBranch = parameters.entitySetRef;
            parameters.soundReceptorIndex = parameters.traceIndex;
            parameters.soundReceptorRef = entity;

            this.traceEvents.OnSoundReceptor(parameters);
            break;
        }
    }
};

/**
 * Traces the scene using the given trace events
 * @param {AxTraceEvents} traceEvents The trace events to use while traversing the scene
 */
Axis.prototype.ProcessScene = function(traceEvents)
{
    this.traceEvents = AxUtils.IsUndefinedOrNull(traceEvents) ? this.renderEvents : traceEvents;
    this.ProcessEntity(this.root, new AxTraceParameters());
};

/**
 * Renders the whole scene
 * @param {Boolean} presentOnScreen Determines whether the scene should be presented on screen after rendered. If omitted, a value of true is assumed
 */
Axis.prototype.RenderScene = function(presentOnScreen)
{
    if (AxUtils.IsUndefinedOrNull(presentOnScreen))
        presentOnScreen = true;
    
    this.timer.Tick();
    
    this.ProcessInput();

    if (this.settings.properties.Get(AxSettings.propertyIndex_ClearScreen).GetBool())
        this.graphicsDevice.ClearScreen(this.settings.properties.Get(AxSettings.propertyIndex_ClearScreenColor).GetEffectiveValue());
    this.graphicsDevice.ClearDepthBuffer();

    this.renderEvents.SetupNewFrame();
    this.ProcessScene(this.renderEvents);

    if (presentOnScreen)
        this.PresentOnScreen();

    this.SwapLightsLists();
};

/**
 * Presents the rendered sceen on the screen
 */
Axis.prototype.PresentOnScreen = function()
{
    this.graphicsDevice.Present();
};

/**
 * Returns a set of 2D coordinates of unit size for the given coordinates of a pixel on the screen
 * The input pixel coordinates are in the range [0, width - 1] horizontally from left to right and [height - 1, 0] vertically from bottom to top.
 * The returned unit coordinates are in the range [-1, 1] horizontally from left to right and [-1, 1] vertically from bottom to top
 * @param {AxVector2} pixelCoords The pixel coordinates, which are to be converted to unit size
 * @param {!AxVector2} screenPixelSize The size of the screen in pixels. If omitted, the size of the viewport is assumed.
 * @returns {AxVector2} The coordinates in unit size
 */
Axis.prototype.ScreenCoordsPixelToUnit = function(pixelCoords, screenPixelSize)
{
    if (AxUtils.IsUndefinedOrNull(screenPixelSize))
        screenPixelSize = new AxVector2(this.viewportWidth, this.viewportHeight);

    return new AxVector2(2.0 * pixelCoords.x / screenPixelSize.x - 1.0, 1.0 - 2.0 * pixelCoords.y / screenPixelSize.y);
};

/**
 * Returns a set of 2D coordinates of pixel size for the given coordinates of a pixel on the screen
 * The input unit coordinates are in the range [-1, 1] horizontally from left to right and [-1, 1] vertically from bottom to top
 * The returned pixel coordinates are in the range [0, width - 1] horizontally from left to right and [height - 1, 0] vertically from bottom to top.
 * @param {AxVector2} unitCoords The unit coordinates, which are to be converted to pixel size
 * @param {!AxVector2} screenPixelSize The size of the screen in pixels. If omitted, the size of the viewport is assumed.
 * @returns {AxVector2} The coordinates in unit size
 */
Axis.prototype.ScreenCoordsUnitToPixel = function(unitCoords, screenPixelSize)
{
    if (AxUtils.IsUndefinedOrNull(screenPixelSize))
        screenPixelSize = new AxVector2(this.viewportWidth, this.viewportHeight);

    return AxVector2(screenPixelSize.x * (unitCoords.x + 1.0) / 2.0, screenPixelSize.y * (1.0 - unitCoords.x) / 2.0);
};


/**
 * Provides information about the intersection of a given ray and the objects on the scene
 * The ray is defined by an initial point and orientation
 * The returned info is for the closest intersected object
 * @param {AxVector3} rayInitialPoint The initial (starting) point of the ray
 * @param {AxVector3} rayOrientation The orientation vector of the ray
 * @param {AxIntersectionInfo} intersectionInfo After the picking is performed, provides information about the intersection. Must be a valid AxIntersectionInfo object
 * @param {AxTraceParameters} entityInfo After the picking is performed, provides information about the intersected object. Must be a valid AxTraceParameters object
 * @return {Boolean} True if the ray has intersected with an object
 */
Axis.prototype.PickByRayPointAndOrientation = function(rayInitialPoint, rayOrientation, intersectionInfo, entityInfo)
{
    this.rayIntersectionEvents.SetupRay(rayInitialPoint, rayOrientation);
    this.ProcessScene(this.rayIntersectionEvents);
      
    if (this.rayIntersectionEvents.intersected)
    {
        intersectionInfo = this.rayIntersectionEvents.intersectionInfo;
        entityInfo = this.rayIntersectionEvents.traceResult;
        return true;
    }
    else
        return false;
};
    
/**
 * Provides information about the intersection of a given ray and the objects on the scene
 * The ray is defined by two points
 * The returned info is for the closest intersected object
 * @param {AxVector3} rayInitialPoint The initial (starting) point of the ray
 * @param {AxVector3} raySecondPoint The second point along the ray
 * @param {AxIntersectionInfo} intersectionInfo After the picking is performed, provides information about the intersection. Must be a valid AxIntersectionInfo object
 * @param {AxTraceParameters} entityInfo After the picking is performed, provides information about the intersected object. Must be a valid AxTraceParameters object
 * @return {Boolean} True if the ray has intersected with an object
 */
Axis.prototype.PickByRayTwoPoints = function(rayInitialPoint, raySecondPoint, intersectionInfo, entityInfo)
{
    var rayOrientation = new AxVector3();
    AxVector3.Subtract(rayOrientation, raySecondPoint, rayInitialPoint);

    return this.PickByRayPointAndOrientation(rayInitialPoint, rayOrientation, intersectionInfo, entityInfo);
};

/**
 * Picks a scene object, which is located at the given 2D coordinates on the screen.
 * The coordinates are measured in unit coordinates, the X coordinate ranging in the interval [-1, 1] from left to right and the Y coordinate ranging in the interval [-1, 1] bottom to top
 * @param {AxVector2} screenUnitCoords The coordinates, in unit space, of the picking point on the screen
 * @param {AxIntersectionInfo} intersectionInfo After the picking is performed, provides information about the intersection. Must be a valid AxIntersectionInfo object
 * @param {AxTraceParameters} entityInfo After the picking is performed, provides information about the intersected object. Must be a valid AxTraceParameters object
 * @return {Boolean} True if an object was picked
 */
Axis.prototype.PickByScreenCoordsUnit = function(screenUnitCoords, intersectionInfo, entityInfo)
{
    this.screenPickEvents.SetupScreenPick(screenUnitCoords);
    this.ProcessScene(this.screenPickEvents);

    intersectionInfo = this.screenPickEvents.intersectionInfo;
    entityInfo = this.screenPickEvents.entityInfo;
    return this.screenPickEvents.intersectionInfo.hasIntersected;
};

/**
 * Picks a scene object, which is located at the given 2D coordinates on the screen.
 * The coordinates are measured in screen pixels, the X coordinate ranging in the interval [0, viewportWidth] from left to right and the Y coordinate ranging in the interval [0, viewportHeight] from top to bottom
 * @param {AxVector2} screenPixelCoords The coordinates, in pixels, of the picking point on the screen
 * @param {AxIntersectionInfo} intersectionInfo After the picking is performed, provides information about the intersection. Must be a valid AxIntersectionInfo object
 * @param {AxTraceParameters} entityInfo After the picking is performed, provides information about the intersected object. Must be a valid AxTraceParameters object
 * @return {Boolean} True if an object was picked
 */
Axis.prototype.PickByScreenCoordsPixel = function(screenPixelCoords, intersectionInfo, entityInfo)
{
    return this.PickByScreenCoordsUnit(this.ScreenCoordsPixelToUnit(screenPixelCoords, new AxVector2(this.viewportWidth, this.viewportHeight)), intersectionInfo, entityInfo);
};


/**
 * Fills the nearestActiveLights collection with up to maxCount lights, which are nearest to a given location
 * @param {Integer} maxCount Maximum number of lights to fill. If the whole scene has less lights than this number, all the lights will be included
 * @param {AxVector3} position The position in relation to which to pick nearest lights
 */
Axis.prototype.CalculateNearestActiveLights = function(maxCount, position)
{
    this.nearestActiveLights.SetSize(this.activeLights.count);
    for (var i = 0; i < this.activeLights.count; i++)
    {
        var lightInfo = this.activeLights.Get(i);
        lightInfo.distance = AxVector3.Distance(position, lightInfo.lightPos);
        this.nearestActiveLights.Set(i, lightInfo);
    }

    var nearestCount = AxMath.Min(this.activeLights.count, maxCount);

    for (var i = 0; i < nearestCount; i++)
    {
        var nearestIndex = i;
        for (var j = i + 1; j < this.nearestActiveLights.count; j++)
        {
            if (this.nearestActiveLights.Get(j).distance < this.nearestActiveLights.Get(nearestIndex).distance)
                nearestIndex = j;
        }

        if (nearestIndex !== i)
        {
            var lightInfo = this.nearestActiveLights.Get(i);
            this.nearestActiveLights.Set(i, this.nearestActiveLights.Get(nearestIndex));
            this.nearestActiveLights.Set(nearestIndex, lightInfo);
        }
    }

    this.nearestActiveLights.SetSize(nearestCount);
};

/**
 * Adds a resource to the resources set
 * @param {AxResource} resource The resource to add
 * @returns {AxResource} The added resource. Provided for syntax ease
 */
Axis.prototype.AddResource = function(resource)
{
    this.resourceID++;

    resource.id = this.resourceID;
	
    this.resources.Add(resource);

    return resource;
};

/**
 * Searches for a resource with a given id
 * If no such resource is found, returns null
 * @param {Integer} id Id of the resource to search for
 * @return {AxResource} The resource with the given id. If none such was found, returns null
 */
Axis.prototype.FindResource_1 = function(id)
{
    var left = 0;
    var right = this.resources.count - 1;

    while (left <= right)
    {
        var index = left + AxMath.Floor((right - left) / 2);
        var resource = this.resources.Get(index);

        if (resource.id === id)
            return resource;

        if (resource.id < id)
            left = index + 1;
        else
            right = index - 1;
    }

    return null;
};

/**
 * Searches for a resource with a given name and of specific type and returns the result
 * If no such resource is found, returns null
 * @param {String|AxString} resourceName Name of the resource to search for
 * @param {AxResourceType} resourceType Type of the resource to search for
 * @returns {AxResource} The resource with the given name and of the given type. If none such was found, returns null
 */
Axis.prototype.FindResource_2 = function(resourceName, resourceType)
{
    resourceName = AxString.GetAxString(resourceName);
    
    var count = this.resources.count;
    for (var i = 0; i < count; i++)
    {
    	var resource = this.resources.Get(i);
        if (resource.resourceType === resourceType)
            if (resource.name.Equals(resourceName))
                return resource;
    }

    return null;
};

/**
 * Searches for a resource with a given name and of specific type and returns the result
 * If no such resource is found, returns null
 * Can search either by resource id or by resource name
 * This method combines methods FindResource_1 and FindResource_2
 * @param {String|AxString|Integer} resourceIdentifier In case of searching by id, it is the id of the searched resource. In case of searching by name, it is the name of the resources.
 * @param {AxResourceType} resourceType In case of searching by name, it is the type of the resource to search for.
 * @returns {AxResource} The resource with the given name and of the given type. If none such was found, returns null
 */
Axis.prototype.FindResource = function(resourceIdentifier, resourceType)
{
    if (AxUtils.IsInteger(resourceIdentifier))
        return this.FindResource_1(resourceIdentifier);
    else
        return this.FindResource_2(resourceIdentifier, resourceType);
};


/**
 * Creates a unique name for the given type of resource
 * If the given resourceName is already unique, then it gets returned unaltered
 * @param {AxString} resourceName Original name to use for generating the unique name
 * @param {AxResourceType} resourceType Type of the resource for which to acquire the name
 * @returns {AxString} A name, which is unique for the given type of resources
 */
Axis.prototype.AcquireResourceName = function(resourceName, resourceType)
{
    var nameIndex = 0;
    var result = new AxString(resourceName);

    while (this.FindResource_2(result, resourceType) !== null)
    {
        nameIndex++;
        result = resourceName.Insert(new AxString(nameIndex));
    }

    return result;
};


/* 
 * Makes a screenshot of the scene and returns it as an image in the pixel format of the graphics device
 * The image has the size of the currently used viewport
 * @returns {AxImage} An image containing a screenshot of the scene
 */
Axis.prototype.TakeScreenshot = function()
{
    return this.TakeScreenshot(this.viewportWidth, this.viewportHeight);
};

/* 
 * Makes a screenshot of the scene and returns it as an image in the pixel format of the graphics device
 * @param {Integer} width The width of the screenshot in pixels
 * @param {Integer} height The height of the screenshot in pixels
 * @returns {AxImage} An image containing a screenshot of the scene
 */
Axis.prototype.TakeScreenshot = function(width, height)
{
    var originalClearOnRenderTarget = this.clearOnRenderTarget;
    var originalViewportWidth = this.viewportWidth;
    var originalViewportHeight = this.viewportHeight;

    this.clearOnRenderTarget = true;
    this.SetViewport(width, height);

    var renderTarget = new AxTexture2D(this);
    renderTarget.properties.Get(AxTexture2D.propertyIndex_Width).SetInt(width);
    renderTarget.properties.Get(AxTexture2D.propertyIndex_Height).SetInt(height);
    var newRoot = new AxEntitySet();
    newRoot.references.Add(renderTarget);
    newRoot.references.Add(this.root);
    this.root = newRoot;
    this.RenderScene(false);
    this.root = newRoot.references.Get(1);

    var result = renderTarget.ToImage();

    this.SetViewport(originalViewportWidth, originalViewportHeight);
    this.clearOnRenderTarget = originalClearOnRenderTarget;

    return result;
};

///**
// * Creates an image by loading it from the given stream, using the registered image dispatchers
// * If none of the registered dispatchers is able to load an image from the stream, null is returned and the stream remains at the original position.
// * @param {AxStream} stream Stream which contains the image data
// * @return {AxImage} The newly created image. If loading was not successfull due to unsupported format or an error, null is returned
// */
//Axis.prototype.LoadImage = function(stream)
//{
//    var streamInitialPos = stream.position;
//
//    for (var i = 0; i < this.imageMediaDispatchers.count; i++)
//    {
//        var image = this.imageMediaDispatchers.Get(i).CreateObject();
//        if (image.Load(stream))
//            return image;
//
//        image = null;
//
//        stream.Seek(streamInitialPos);
//    }
//
//    return null;
//};

Axis.prototype.LoadImageSequential = function(stream, startDispatcherIndex, callbackContext, callback)
{
    if (startDispatcherIndex >= this.imageMediaDispatchers.count)
    {
        if (!AxUtils.IsUndefinedOrNull(callback))
            callback(this, callbackContext, null);
        return;
    }
    
    var image = this.imageMediaDispatchers.Get(startDispatcherIndex).CreateObject();
    var streamInitialPos = stream.position;
    image.Load(stream, this,
        function(sender, context, succeeded)
        {
            if (succeeded)
            {
                if (!AxUtils.IsUndefinedOrNull(callback))
                    callback(context, callbackContext, image);
            }
            else
            {
                stream.Seek(streamInitialPos);
    
                if (!AxUtils.IsUndefinedOrNull(callback))
                    context.LoadImageSequential(stream, startDispatcherIndex + 1, callbackContext, callback);
            }
        });
};

/**
 * Creates an image by loading it from the given stream, using the registered image dispatchers
 * If none of the registered dispatchers is able to load an image from the stream, null is returned and the stream remains at the original position.
 * @param {AxStream} stream Stream which contains the image data
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxisObjectLoadedCallback} callback A callback method which is to be called when loading has finished
 */
Axis.prototype.LoadImage = function(stream, callbackContext, callback)
{
    this.LoadImageSequential(stream, 0, callbackContext, callback);
};

/**
 * Creates an image by loading it from the given file name, using the registered file systems to access the file and the registered image to decode the image
 * If none of the registered file systems can access the file or none of the registered image dispatchers is able to load an image from the file, null is returned
 * @param {AxString} fileName Name of the file containing the image to be loaded
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxisObjectLoadedCallback} callback A callback method which is to be called when loading has finished
 */
Axis.prototype.LoadImage_2 = function(fileName, callbackContext, callback)
{
    this.fileSystem.OpenFile(this.GetFullFileName(fileName), this,
        function(context, stream, path, fileSystem)
        {
            if (stream !== 0)
            {
                context.LoadImage(stream, callbackContext, callback);
            }
            else
            {
                if (!AxUtils.IsUndefinedOrNull(callback))
                    callback(context, callbackContext, null);
            }
        });
};

/**
 * Acquires a texture from the given file name and returns the texture object
 * If the texture is not already in the loaded resources, it gets loaded.
 * @param {AxString} fileName Name of the file containing the image to used as a texture
 * @param {*} callbackContext An object which is to be passed to the callback. It can be used to serve as a context for the callback or just to pass any relevant data
 * @param {AxisObjectLoadedCallback} callback A callback method which is to be called when acquiring has finished
 */
Axis.prototype.AcquireTexture = function(fileName, callbackContext, callback)
{
    fileName = this.fileSystem.FromNativePathSyntax(fileName);
    
    for (var i = 0; i < this.resources.count; i++)
    {
        var resource = this.resources.Get(i);
        if (resource.resourceType === AxResourceType.Texture)
            if (resource.fileName.Equals(fileName))
            {
                if (!AxUtils.IsUndefinedOrNull(callback))
                    callback(this, callbackContext, resource);
        
                return;
            }
    }

    var result = new AxTexture2D(this);

    result.Load_4(fileName, this,
        function(sender, context, succeeded)
        {
            if (succeeded)
            {
                result.name = context.AcquireResourceName(AxFileSystem.GetFileNameWithoutExtension(fileName));
                context.AddResource(result);
            }
            else
            {
                result = null;
            }

            if (!AxUtils.IsUndefinedOrNull(callback))
                callback(sender, callbackContext, result);
        });
};


/**
 * Creates an entity by its typeId, using the registered entity dispatchers
 * @param {Integer} typeId The identifier for the type of entity, which is to be created
 * @returns {AxEntity} The newly created entity. If the entity couldn't be created, for example because the typeId was invalid or was not supported by the registered entity dispatchers, the return value is null
 */
Axis.prototype.CreateEntity = function(typeId)
{
    for (var i = 0; i < this.entityDispatchers.count; i++)
    {
        var result = this.entityDispatchers.Get(i).CreateEntity(this, typeId);
        if (result !== null)
            return result;
    }

    return null;
};

/**
 * Serializes a resource into a stream
 * @param {AxStream} destination Destination stream in which the resource's serialized data will be written
 * @param {AxResource} resource The resource to serialize
 */
Axis.prototype.SerializeResource = function(destination, resource)
{
    var writer = new AxHierarchyStreamWriter(destination);

    writer.BeginChunk(Axis.SerializationId_ResourceSerialization);
    writer.stream.WriteInt32(resource.typeId);
    resource.Serialize(destination);
    writer.EndChunk();
    
    writer = null;
};

/**
 * Deserializes a resource from a stream
 * @param {AxStream} source Source stream, from which the resource's serialized data will be read
 * @returns {AxResource} The resource, deserialized from the stream. If deserialization was not successfull, returns null
 */
Axis.prototype.DeserializeResource = function(source)
{
    var reader = new AxHierarchyStreamReader(source);

    if (reader.ReadChunkHeader())
    {
        if (reader.chunkId === Axis.SerializationId_ResourceSerialization)
        {
            var typeId = reader.stream.ReadInt32();

            var result = this.CreateEntity(typeId);
            if (result !== null)
            {
                if (result.Deserialize(source))
                    return result;
                else
                    result = null;
            }
        }
    }

    return null;
};

/**
 * Performs the input models routines and the input devices routines
 */
Axis.prototype.ProcessInput = function()
{
    for (var i = 0; i < this.inputModels.count; i++)
        this.inputModels.Get(i).Process();

    for (var i = 0; i < this.inputDevices.count; i++)
        this.inputDevices.Get(i).Update();
};


/**
 * Swaps the list of light which was collected during the last traversing of the world, with the list of lights which is used for lighting by the shaders
 */
Axis.prototype.SwapLightsLists = function()
{
    var temp = this.activeLights;
    this.activeLights = this.activeLightsCollecting;
    this.activeLightsCollecting = temp;

    this.activeLightsCollecting.SetSize(0);
};

/**
 * Gets the full file name with its absolute path
 * @param {AxString} fileName File name to get absolute file name for. If the file name is already an absolute one, it is returned unaltered
 * @return {AxString} The full file name with its absolute path
 */
Axis.prototype.GetFullFileName = function(fileName)
{
    if (AxFileSystem.IsAbsolutePath(fileName))
        return fileName;
    else
        return AxFileSystem.MergePaths(this.fileSystem.FromNativePathSyntax(AxPlatformUtils.GetApplicationPath()), fileName);
};

/**
 * Attaches an entity to an entity set by moving it from its original 'source' set into another and, when applicable, performs transformations, so that it retains its situation in the scene
 * @param {AxEntitySet} sourceParentSet The entity set, which contains the entity to be moved / attached
 * @param {Integer} sourceIndex Index of the entity in the references of the source parent set.
 * @param {AxEntitySet} destinationParentSet The destination entity set, where the entity is to be placed / attached to. If omitted or null is passed, the root of the scene is assumed.
 * @param {Integer} destinationIndex The index at which the entity is to placed in the references of te destination parent set. If omitted or -1 is passed, an index at the end of the parent set is assumed.
 * @param {AxTransform} parentTransform The transform, which is to be used as a parent transform of the entity. If omitted or null is passed, an identity transform is assumed
 */
Axis.prototype.Attach = function(sourceParentSet, sourceIndex, destinationParentSet, destinationIndex, parentTransform)
{
    if (AxUtils.IsUndefinedOrNull(destinationParentSet))
        destinationParentSet = this.root;

    if (AxUtils.IsUndefinedOrNull(destinationIndex) || (destinationIndex === -1))
        destinationIndex = destinationParentSet.references.count;

    var movedResource = sourceParentSet.references.Get(sourceIndex);

    switch (movedResource.resourceType)
    {
        case AxResourceType.EntitySet:
        {
            var movedEntitySet = movedResource;

            if (movedEntitySet.HierarchyContains(destinationParentSet) || (movedEntitySet === destinationParentSet))
                return;

            var refCount = movedEntitySet.references.count;
            for (var i = 0; i < refCount; i++)
            {
                var ref = movedEntitySet.references.Get(i);
                if (ref.resourceType === AxResourceType.Transform)
                    ref.RelateTo(parentTransform);
            }

            break;
        }

        case AxResourceType.Transform:
        {
            movedResource.RelateTo(parentTransform);

            break;
        }
    }

    sourceParentSet.references.RemoveAt(sourceIndex);
    destinationParentSet.references.Insert(destinationIndex, movedResource);
};

/**
 * Detaches an entity its original entity set and, when applicable, performs transformations, so that it retains its situation in the scene
 * Effectively, attaches the entity to the root entity set
 * @param {AxEntitySet} sourceParentSet The entity set, which contains the entity to be moved / attached
 * @param {Integer} sourceIndex Index of the entity in the references of the source parent set.
 */
Axis.prototype.Detach = function(sourceParentSet, sourceIndex)
{
    this.Attach(sourceParentSet, sourceIndex, null, -1, null);
};

/**
 * Returns the id of the canvas used for rendering
 * @returns {*} The id of the canvas used for rendering
 */
Axis.prototype.GetRenderingWindow = function()
{
    return this.windowHandle;
};
