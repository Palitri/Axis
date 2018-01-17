#pragma once

#include "..\AxGlobals.h"

#include "Base\AxEntityDispatcher.h"
#include "Base\AxModuleDispatcher.h"

#include "AxTraceEvents.h"
#include "AxTraceParameters.h"

#include "AxRenderEvents.h"
#include "AxScreenPickEvents.h"
#include "AxRayIntersectionEvents.h"

#include "Primitive\AxModule.h"

#include "Utilities\AxTimer.h"
#include "Utilities\AxLogging.h"

#include "Input\AxInput.h"
#include "Input\AxInputModel.h"

#include "Entities\Light\AxLight.h"
#include "Entities\Sound\AxSound.h"
#include "Entities\Settings\AxSettings.h"

#include "..\Media\Image\AxImage.h"
#include "..\Media\Sound\AxSoundMedia.h"

#include "..\FileSystem\AxFileSystem.h"

#include "..\Graphics\AxGraphicsDevice.h"
#include "..\Sound\AxAudioDevice.h"

#include "..\Input\AxInputDevice.h"

#include "..\Script\Parsing\AxCodeBuilder.h"
#include "..\Script\Processing\AxCodeProcessor.h"

#include "AxSerializationParameters.h"

struct AxActiveLightInfo
{
	AxLight* lightRef;
	AxMatrix lightTransform;
	AxVector3 lightPos;
	float distance;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxModule*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxResource*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxEntityDispatcher*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxModuleDispatcher*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<void*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxActiveLightInfo>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxActiveLightInfo*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxMatrix>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxInputModel*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxInputDevice*>;

class AXDLLCLASS Axis
{
private:
	static const unsigned int SerializationId_ResourceSerialization = 0x21100000;

	long long resourceID;

	// Each of activeLights1 and activeLights2 are alternating each frame to be the list of the lights currently being collected and the list of all the lights, collected in the previous scene
	AxList<AxActiveLightInfo> activeLights1, activeLights2;


	void ProcessInput();

	void SwapLightsLists();

	void RemoveResourceFromLightsList(AxResource *resource);


public:
	static const unsigned int VersionSignature = 0x09000000;

	AxList<AxModule*> loadedModules;

	void *windowHandle;
	AxTraceEvents *traceEvents;
    AxRenderEvents *renderEvents;
    AxRayIntersectionEvents *rayIntersectionEvents;
    AxScreenPickEvents *screenPickEvents;

    int viewportWidth, viewportHeight;
	//AxVector4 ambientLighting;
    double speed;
	int cameraFOVMode;

	AxSettings *settings;

	AxTexture2D *currentRenderTarget;
	AxTraceParameters currentCamera;

	AxLogging logging;

	AxSerializationParameters serializationParameters;
	bool clearOnRenderTarget;

    AxList<AxResource*> resources;
    AxEntitySet *root;
    
	AxTimer *timer;

	AxInput *input;
	AxList<AxInputModel*> inputModels;
    
	AxFileSystem *fileSystem;

	AxGraphicsDevice *graphicsDevice;

	AxAudioDevice *audioDevice;

	AxList<AxInputDevice*> inputDevices;

	AxList<AxEntityDispatcher*> entityDispatchers;

	AxList<AxModuleDispatcher*> imageMediaDispatchers, soundMediaDispatchers, sceneMediaDispatchers, graphicsDeviceDispatchers, audioDeviceDispatchers, fileSystemDispatchers, inputDeviceDispatchers;

	// activeLights should be used as the set of lights currently affecting the scene. It is the list of all active lights, collected while rendering the last frame. activeLightsCollecting should be used to collect active lights while rendering the current frame.
	AxList<AxActiveLightInfo> *activeLights, *activeLightsCollecting;
	// Filled by the CalculateNearestLights() method, holds pointers to a number of lights from the activeLights member variable, which are the nearest to a certain point in space, sorted ascending by distance to that point
	AxList<AxActiveLightInfo*> nearestActiveLights;

	AxList<AxMatrix> bones;

	AxCodeBuilder scriptBuilder;
	AxCodeProcessor scriptProcessor;

	Axis(void *windowHandle);
	virtual ~Axis(void);
    
	// Loads a module and adds its dispatchers to the appropriate collections and the module itself to the collection of loaded modules.
	// Returns the loaded module. If loading was not successful, return 0
	virtual AxModule *Axis::LoadModule(AxString moduleFileName);
	// Unloads a module and removes its dispatchers and itself from the respective collections
	virtual void UnloadModule(AxModule *module);

	// Loads all the compatible modules from the given path
	virtual void LoadModules(AxString modulesDir = "");
	// Unloads all loaded modules and their dispatchers. Internal dispatchers are not affected
	virtual void UnloadModules();

	virtual void SetGraphicsDevice(AxGraphicsDevice *graphicsDevice);
	virtual int SetGraphicsDevice(const AxString &graphicsDeviceName);

	virtual void SetAudioDevice(AxAudioDevice *audioDevice);
	virtual int SetAudioDevice(const AxString &audioDeviceName);
    
	virtual void Reset();

	virtual bool ImportScene(AxStream &stream);
	virtual bool ImportScene(AxString fileName);
	virtual bool ExportScene(AxStream &stream, AxString fileName);
	virtual bool ExportScene(AxString fileName);

	virtual void SetViewport(int width, int height);

	virtual void ProcessEntity(AxResource *entity, AxTraceParameters &parameters);
	virtual void ProcessScene(AxTraceEvents *traceEvents = 0);

	virtual void RenderScene(bool presentOnScreen = true);

	virtual void PresentOnScreen();

	virtual AxVector2 PixelToScreenSpace(AxVector2 &pixelCoords, AxVector2 &screenPixelSize);
	virtual AxVector2 PixelToScreenSpace(AxVector2 &pixelCoords);
	virtual AxVector2 ScreenToPixelSpace(AxVector2 &screenCoords, AxVector2 &screenPixelSize);
	virtual AxVector2 ScreenToPixelSpace(AxVector2 &screenCoords);
    
	// Picks a scene object, which is the first to intersect a ray defined by an initial point and an orientation
    virtual bool PickByRayPointAndOrientation(AxVector3 &rayInitialPoint, AxVector3 &rayOrientation, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo);
	// Picks a scene object, which is the first to intersect a ray defined by an initial point and a secondary point
    virtual bool PickByRayTwoPoints(AxVector3 &rayInitialPoint, AxVector3 &raySecondPoint, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo);
	// Picks a scene object, which is located at the given 2D coordinates on the screen.
	// The coordinates are of unit size, ranging in the interval [-1, 1], where the X coordinate ranges from left to right and the Y coordinate ranges from bottom to top
	virtual bool PickByScreenSpaceCoords(AxVector2 &screenUnitCoords, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo);
	// Picks a scene object, which is located at the given 2D coordinates on the screen.
	// The coordinates are in screen pixels, the X coordinate ranging in the interval [0, viewportWidth] from left to right and the Y coordinate ranging in the interval [0, viewportHeight] from top to bottom
    virtual bool PickByPixelSpaceCoords(AxVector2 &screenPixelCoords, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo);
	// Picks a scene object, which is located at the given 2D coordinates on a screen of given size.
	// The coordinates are in screen pixels, the X coordinate ranging in the interval [0, screenWidth] from left to right and the Y coordinate ranging in the interval [0, screenHeight] from top to bottom
    virtual bool PickByPixelSpaceCoords(AxVector2 &screenPixelCoords, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo, AxVector2 &screenPixelSize);

	virtual void CalculateNearestActiveLights(int maxCount, AxVector3 &position);

    virtual AxResource *AddResource(AxResource *resource);
	// Removes a resource from the engine and releases
	virtual void RemoveResource(AxResource *resource);
	//void RemoveResource(long long id);
	// Recursively removes each encounter of a resource from the contents of a resource set
	virtual void RemoveResourceFromSet(AxEntitySet *resourceSet, AxResource *resource);
	// Removes the references to a resource from all the properties of the loaded resources
	virtual void RemoveResourceReferences(AxResource *resource);
    // Returns the index, at which the given resource is located in the resources set
	virtual int GetResourceIndex(AxResource &resource);
    virtual AxResource *FindResourceX(const AxString &resourceName, AxResourceType resourceType);
    virtual AxResource *FindResourceX(long long id);
    virtual AxString AcquireResourceName(AxString &resourceName, AxResourceType resourceType);
	virtual AxResource *FindEntity(AxResourceType resourceType, AxEntitySet *entitySet = 0);
	virtual AxResource *FindEntity(const AxString &entityName, AxResourceType resourceType, AxEntitySet *entitySet = 0);

	virtual AxImage *TakeScreenshot();
	virtual AxImage *TakeScreenshot(int width, int height);

	virtual AxImage *LoadImageX(AxStream &stream);
	virtual AxImage *LoadImageX(AxString fileName);

	virtual bool SaveImage(AxImage *image, AxString fileName, AxStream &stream);
	virtual bool SaveImage(AxImage *image, AxString fileName);

	virtual AxTexture2D *AcquireTexture(AxString fileName);

	// Creates a sound media from a stream
	virtual AxSoundMedia *CreateSoundMedia(AxStream &stream);
	// Creates a sound media from a given file and outputs a stream of the file to be used for streaming sound
	virtual AxSoundMedia *CreateSoundMedia(AxString fileName, AxStream **sourceStream);

	virtual AxSound *AcquireSound(AxString fileName);

	virtual AxEntity *CreateEntity(int typeId);
	
	virtual void SerializeResource(AxStream *destination, AxResource *resource);
	virtual AxResource *DeserializeResource(AxStream *source);

	virtual AxString GetFullFileName(AxString fileName);

	virtual void Attach(AxEntitySet *sourceParentSet, int sourceIndex, AxEntitySet *destinationParentSet, int destinationIndex, AxTransform *parentTransform);
	virtual void Detach(AxEntitySet *sourceParentSet, int sourceIndex);

	virtual void *GetRenderingWindow();
};
