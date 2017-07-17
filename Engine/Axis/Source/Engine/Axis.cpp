//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "..\FileSystem\PhysicalFileSystem\AxPhysicalFileSystem.h"

#include "..\Media\Scene\Native\AxNativeSceneDispatcher.h"
#include "..\Media\Scene\3ds\Ax3dsSceneDispatcher.h"
#include "..\Media\Scene\Obj\AxObjSceneDispatcher.h"

#include "..\Media\Image\Native\AxNativeImageDispatcher.h"

#include "..\Media\Sound\Wav\AxWavSoundMediaDispatcher.h"

#include "..\Graphics\Soras\AxSorasGraphicsDeviceDispatcher.h"

#include "..\Input\WindowsForms\AxWindowsFormsInputDeviceDispatcher.h"

#include "..\FileSystem\PhysicalFileSystem\AxPhysicalFileSystemDispatcher.h"

#include "..\Media\Scene\AxScene.h"

#include "..\Media\Scene\Native\AxNativeScene.h"

#include "Utilities\Serialization\AxHierarchyStreamWriter.h"
#include "Utilities\Serialization\AxHierarchyStreamReader.h"

#include "AxStandardEntityDispatcher.h"

#include "..\Tools\Streams\AxMemoryStream.h"

#include "..\Tools\Platform\AxPlatformUtils.h"

#include "Utilities\Imaging\AxGenerateNormalMapImage.h"

#include "Utilities\AxLogging.h"

#include "Axis.h"

typedef unsigned int (*AxModuleFunctionGetSignature) ();
typedef void *(*AxModuleFunctionCreateDispatcher) (int index);

Axis::Axis(void *windowHandle)
{
    this->windowHandle = windowHandle;
	this->speed = 1.0;
    this->timer = new AxTimer();
	this->cameraFOVMode = AxCamera::FOVMode_Vertical;
	AxPlatformUtils::GetWindowClientSize(windowHandle, this->viewportWidth, this->viewportHeight);
	this->input = new AxInput();

	this->clearOnRenderTarget = false;

	this->settings = new AxSettings();

	this->renderEvents = new AxRenderEvents(*this);
	this->rayIntersectionEvents = new AxRayIntersectionEvents(*this);
	this->screenPickEvents = new AxScreenPickEvents(*this);

	this->activeLights = &this->activeLights2;
	this->activeLightsCollecting = &this->activeLights1;

	this->bones.SetSize(64);	

	this->sceneMediaDispatchers.Add(new AxNativeSceneDispatcher());
	this->sceneMediaDispatchers.Add(new Ax3dsSceneDispatcher());
	this->sceneMediaDispatchers.Add(new AxObjSceneDispatcher());

	this->imageMediaDispatchers.Add(new AxNativeImageDispatcher());

	this->soundMediaDispatchers.Add(new AxWavSoundMediaDispatcher());

	this->graphicsDeviceDispatchers.Add(new AxSorasGraphicsDeviceDispatcher());

	this->inputDeviceDispatchers.Add(new AxWindowsFormsInputDeviceDispatcher());

	this->entityDispatchers.Add(new AxStandardEntityDispatcher());

	this->fileSystemDispatchers.Add(new AxPhysicalFileSystemDispatcher());

	this->LoadModules();

	if (this->fileSystemDispatchers.count > 0)
	{
		this->fileSystem = (AxFileSystem*)this->fileSystemDispatchers[0]->CreateObject();
	}
    else
        this->fileSystem = 0;


	if (this->graphicsDeviceDispatchers.count > 0)
	{
		this->graphicsDevice = (AxGraphicsDevice*)this->graphicsDeviceDispatchers[0]->CreateObject();
		this->graphicsDevice->SetRenderTargetWindow(this->windowHandle);
		//this->SetGraphicsDevice((AxGraphicsDevice*)this->graphicsDeviceDispatchers[0]->CreateObject());
	}
    else
        this->graphicsDevice = 0;

	if (this->audioDeviceDispatchers.count > 0)
	{
		this->audioDevice = (AxAudioDevice*)this->audioDeviceDispatchers[0]->CreateObject();
	}
    else
        this->audioDevice = 0;

	for (int i = 0; i < this->inputDeviceDispatchers.count; i++)
	{
		AxInputDevice *device = (AxInputDevice*)this->inputDeviceDispatchers[i]->CreateObject();
		device->SetContext(this);
		this->inputDevices.Add(device);
	}

	this->scriptBuilder.SetProcessor(&this->scriptProcessor);

	this->Reset();
}

Axis::~Axis(void)
{
	this->Reset();

	for (int i = 0; i < this->graphicsDeviceDispatchers.count; i++)
		delete this->graphicsDeviceDispatchers[i];

	for (int i = 0; i < this->audioDeviceDispatchers.count; i++)
		delete this->audioDeviceDispatchers[i];

	for (int i = 0; i < this->entityDispatchers.count; i++)
		delete this->entityDispatchers[i];
	
	for (int i = 0; i < this->imageMediaDispatchers.count; i++)
		delete this->imageMediaDispatchers[i];

	for (int i = 0; i < this->soundMediaDispatchers.count; i++)
		delete this->soundMediaDispatchers[i];

	for (int i = 0; i < this->sceneMediaDispatchers.count; i++)
		delete this->sceneMediaDispatchers[i];

	for (int i = 0; i < this->fileSystemDispatchers.count; i++)
		delete this->fileSystemDispatchers[i];

	for (int i = 0; i < this->inputDeviceDispatchers.count; i++)
		delete this->inputDeviceDispatchers[i];
	
	for (int i = 0; i < this->inputDevices.count; i++)
		delete this->inputDevices[i];
	if (this->graphicsDevice != 0)
		delete this->graphicsDevice;
	if (this->audioDevice != 0)
		delete this->audioDevice;
	if (this->fileSystem != 0)
		delete this->fileSystem;

	delete this->input;
	delete this->timer;
	delete this->root;

	delete this->renderEvents;
	delete this->rayIntersectionEvents;
	delete this->screenPickEvents;

	delete this->settings;

	for (int i = 0; i < this->inputModels.count; i++)
		delete this->inputModels[i];

	this->UnloadModules();
}
    
AxModule *Axis::LoadModule(AxString moduleFileName)
{
	AxLogging::AddMessage(AxString("Module probing: ") + moduleFileName, AxLogging::MessageType_Info);

	AxModule *module = new AxModule();
	module->name = moduleFileName;
	module->handle = AxPlatformUtils::LoadModule(module->name);
	if (module->handle != 0)
	{
		AxModuleFunctionGetSignature signatureFunction = (AxModuleFunctionGetSignature)AxPlatformUtils::GetModuleFunction(module->handle, AxString("AxGetSignature"));
		if (signatureFunction != 0)
		{
			unsigned int signature = signatureFunction();
			if (signature == Axis::VersionSignature)
			{
				AxLogging::AddMessage(AxString("Module ") + module->name + " loaded, signature: " + AxString(signature), AxLogging::MessageType_Info);

				AxModuleFunctionCreateDispatcher createDispatcherFunction = (AxModuleFunctionCreateDispatcher)AxPlatformUtils::GetModuleFunction(module->handle, AxString("AxCreateDispatcher"));

				for (int dispatcherIndex = 0; AxModuleDispatcher *dispatcher = (AxModuleDispatcher*)createDispatcherFunction(dispatcherIndex); dispatcherIndex++)
				{
					AxString dispatcherName;
					if (!dispatcher->GetInfo(AxModuleDispatcher::infoId_Name, 0, dispatcherName))
						AxLogging::AddMessage(AxString("Module ") + module->name + " dispatcher at index " + AxString(dispatcherIndex) + " could not provide name", AxLogging::MessageType_Warning);

					AxString typeId;
					if (dispatcher->GetInfo(AxModuleDispatcher::infoId_TypeId, 0, typeId))
					{
						unsigned int uintTypeId;
						if (typeId.ToUInt(uintTypeId))
						{
							switch (uintTypeId)
							{
								case AxModuleDispatcher::typeId_Image:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded image dispatcher: " + dispatcherName, AxLogging::MessageType_Info);

									module->dispatchersReferences.Add(dispatcher);
									this->imageMediaDispatchers.Add(dispatcher);
									break;
								}

								case AxModuleDispatcher::typeId_Sound:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded sound dispatcher: " + dispatcherName, AxLogging::MessageType_Info);

									module->dispatchersReferences.Add(dispatcher);
									this->soundMediaDispatchers.Add(dispatcher);
									break;
								}

								case AxModuleDispatcher::typeId_Scene:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded scene dispatcher: " + dispatcherName, AxLogging::MessageType_Info);

									module->dispatchersReferences.Add(dispatcher);
									this->sceneMediaDispatchers.Add(dispatcher);
									break;
								}

								case AxModuleDispatcher::typeId_GraphicsDevice:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded graphics device dispatcher: " + dispatcherName, AxLogging::MessageType_Info);

									module->dispatchersReferences.Add(dispatcher);
									this->graphicsDeviceDispatchers.Add(dispatcher);
									break;
								}

								case AxModuleDispatcher::typeId_AudioDevice:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded audio device dispatcher: " + dispatcherName, AxLogging::MessageType_Info);
											
									module->dispatchersReferences.Add(dispatcher);
									this->audioDeviceDispatchers.Add(dispatcher);
									break;
								}

								case AxModuleDispatcher::typeId_FileSystem:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded file system dispatcher: " + dispatcherName, AxLogging::MessageType_Info);

									module->dispatchersReferences.Add(dispatcher);
									this->fileSystemDispatchers.Add(dispatcher);
									break;
								}

								case AxModuleDispatcher::typeId_InputDevice:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " loaded file system dispatcher: " + dispatcherName, AxLogging::MessageType_Info);

									module->dispatchersReferences.Add(dispatcher);
									this->inputDeviceDispatchers.Add(dispatcher);
									break;
								}

								default:
								{
									AxLogging::AddMessage(AxString("Module ") + module->name + " has dispatcher " + dispatcherName + " of unknown type: " + AxString(typeId), AxLogging::MessageType_Warning);
									delete dispatcher;
									break;
								}
							}
						}
						else
						{
							AxLogging::AddMessage(AxString("Module ") + module->name + " has dispatcher " + dispatcherName + " of unexpected type: " + typeId, AxLogging::MessageType_Warning);
							delete dispatcher;
						}
					}
					else 
					{
						AxLogging::AddMessage(AxString("Module ") + module->name + " dispatcher " + dispatcherName + " is unable to provide type info", AxLogging::MessageType_Warning);
						delete dispatcher;
					}
				}
			}
			else
				AxLogging::AddMessage(AxString("Module ") + module->name + " not loaded - signature mismatch: Expected " + AxString(Axis::VersionSignature) + " but found "+ AxString(signature), AxLogging::MessageType_Warning);
		} // AxGetSignature() is not available, the file is not a Axis module
	}
	else
		AxLogging::AddMessage(AxString("Module probing: Error loading library ") + module->name, AxLogging::MessageType_Warning);


	if (module->dispatchersReferences.count > 0)
	{
		this->loadedModules.Add(module);
	}
	else
	{
		if (module->handle != 0)
			AxPlatformUtils::UnloadModule(&module->handle);

		delete module;
		module = 0;
	}

	return module;
}

void Axis::UnloadModule(AxModule *module)
{
	for (int i = 0; i < module->dispatchersReferences.count; i++)
	{
		AxModuleDispatcher *dispatcher = module->dispatchersReferences[i];
		this->imageMediaDispatchers.Remove(dispatcher);
		this->soundMediaDispatchers.Remove(dispatcher);
		this->sceneMediaDispatchers.Remove(dispatcher);
		this->graphicsDeviceDispatchers.Remove(dispatcher);
		this->audioDeviceDispatchers.Remove(dispatcher);
		this->fileSystemDispatchers.Remove(dispatcher);
	}

	AxPlatformUtils::UnloadModule(&module->handle);
}

void Axis::LoadModules(AxString modulesDir)
{
	AxList<AxFileInfo> files;

	if (modulesDir.IsEmpty())
		modulesDir = AxPhysicalFileSystem::GetFileDir(AxPlatformUtils::GetApplicationName());

	AxPhysicalFileSystem fs;
	fs.GetDirectoryContents(modulesDir, files);

	for (int fileIndex = 0; fileIndex < files.count; fileIndex++)
	{
		if (files[fileIndex].fileName.ToLower().StartsWith("ax") && AxFileSystem::GetFileExtension(files[fileIndex].fileName).ToLower().Equals("dll"))
		{
			AxString moduleFilePath = fs.MergePaths(modulesDir, files[fileIndex].fileName);

			this->LoadModule(moduleFilePath);
		}
	}
}

void Axis::UnloadModules()
{
	for (int i = 0; i < this->loadedModules.count; i++)
	{
		this->UnloadModule(this->loadedModules[i]);
		delete this->loadedModules[i];
	}

	this->loadedModules.Clear();
}

// TODO: remove this method, or implement it properly : resources have to be loaded anew and not from the already loaded ones, so what about textures loaded from streams, changing from lower to higher color depth and etc.
// Should the old graphics device be deleted here? That doesn't seem intuitive. Rename to SetNEWGraphicsDevice?
void Axis::SetGraphicsDevice(AxGraphicsDevice *graphicsDevice)
{
	if (this->graphicsDevice == graphicsDevice)
		return;

	AxMemoryStream save;
	bool saveSceneThumbnail = this->settings->properties[AxSettings::propertyIndex_Thumbnail]->GetBool();
	this->settings->properties[AxSettings::propertyIndex_Thumbnail]->SetBool(false);
	this->ExportScene(save);
	this->settings->properties[AxSettings::propertyIndex_Thumbnail]->SetBool(saveSceneThumbnail);

	this->Reset();
	
	if (this->graphicsDevice != 0)
		delete this->graphicsDevice;

	this->graphicsDevice = graphicsDevice;
	this->graphicsDevice->SetRenderTargetWindow(this->windowHandle);

	save.Seek(0);
	this->ImportScene(save);
}

int Axis::SetGraphicsDevice(const AxString &graphicsDeviceName)
{
	for (int i = 0; i < this->graphicsDeviceDispatchers.count; i++)
	{
		AxString name;
		if (this->graphicsDeviceDispatchers[i]->GetInfo(AxModuleDispatcher::infoId_Name, 0, name))
		{
			if(name.Equals(graphicsDeviceName))
			{
				this->SetGraphicsDevice((AxGraphicsDevice*)this->graphicsDeviceDispatchers[i]->CreateObject());
				return i;
			}
		}
	}

	return -1;
}

void Axis::SetAudioDevice(AxAudioDevice *audioDevice)
{
	if (this->audioDevice == audioDevice)
		return;

	AxMemoryStream save;
	this->ExportScene(save);

	this->Reset();
	
	if (this->audioDevice != 0)
		delete this->audioDevice;

	this->audioDevice = audioDevice;

	save.Seek(0);
	this->ImportScene(save);
}

int Axis::SetAudioDevice(const AxString &audioDeviceName)
{
	for (int i = 0; i < this->audioDeviceDispatchers.count; i++)
	{
		AxString name;
		if (this->audioDeviceDispatchers[i]->GetInfo(AxModuleDispatcher::infoId_Name, 0, name))
		{
			if(name.Equals(audioDeviceName))
			{
				this->SetAudioDevice((AxAudioDevice*)this->audioDeviceDispatchers[i]->CreateObject());
				return i;
			}
		}
	}

	return -1;
}

void Axis::Reset()
{
	this->activeLights1.Clear();
	this->activeLights2.Clear();
	this->nearestActiveLights.Clear();

	for (int i = 0; i < this->resources.count; i++)
	{
		AxResource *r = this->resources[i];
		delete this->resources[i];
	}
	this->resources.Clear();
	this->resourceID = 0;

    this->root = (AxEntitySet*)this->AddResource(new AxEntitySet());
	this->root->name = "Default Root";

	this->currentCamera = AxTraceParameters();

//	delete this->settings;
//	this->settings = new AxSettings();
}

bool Axis::ImportScene(AxStream &stream)
{
	long long streamPos = stream.position;

	bool success = false;
	for (int i = 0; i < this->sceneMediaDispatchers.count; i++)
	{
		stream.Seek(streamPos);
		AxScene *scene = (AxScene*)this->sceneMediaDispatchers[i]->CreateObject();
		scene->SetContext(this);
		success = scene->Load(stream);
		delete scene;

		if (success)
			break;
	}

	this->timer->Tick();

	return success;
}

bool Axis::ImportScene(AxString fileName)
{
	AxString fullFileName = this->GetFullFileName(fileName);

	this->serializationParameters.fullSourceName = fullFileName;
	this->serializationParameters.rootDir = AxFileSystem::GetFileDir(fullFileName);
	this->serializationParameters.friendlyName = AxFileSystem::GetFileNameWithoutExtension(fullFileName);
	
	AxStream *stream = this->fileSystem->OpenFile(fullFileName);
	if (stream == 0)
		return false;

	bool success = this->ImportScene(*stream);

	delete stream;

	return success;
}

bool Axis::ExportScene(AxStream &stream)
{
	AxNativeScene scene;
	scene.SetContext(this);

	return scene.Save(stream, 0);
}

bool Axis::ExportScene(AxString fileName)
{
	AxString fullFileName = this->GetFullFileName(fileName);

	this->serializationParameters.fullSourceName = fileName;
	this->serializationParameters.rootDir = AxFileSystem::GetFileDir(fileName);
	this->serializationParameters.friendlyName = AxFileSystem::GetFileNameWithoutExtension(fileName);

	AxStream *stream = this->fileSystem->CreateNewFile(fileName);
	if (stream == 0)
		return false;

	bool success = this->ExportScene(*stream);

	delete stream;

	return success;
}

void Axis::SetViewport(int width, int height)
{
	this->graphicsDevice->SetViewport(0, 0, width, height);
	this->viewportWidth = width;
	this->viewportHeight = height;
}

void Axis::ProcessEntity(AxResource *entity, AxTraceParameters &parameters)
{
	// For backward/forward compatibility. Entities unknown to the current version will result in null references in the entity set
	if (entity == 0)
		return;

	if (!entity->properties[AxResource::propertyIndex_Enabled]->GetBool())
		return;

	switch (entity->resourceType)
    {
		case AxResourceType_Texture:
			{
				parameters.textureRef = (AxTexture2D*)entity;

				this->traceEvents->OnTexture2D(parameters);
				break;
			}

        case AxResourceType_Camera:
			{
				parameters.cameraBranch = parameters.entitySetRef;
				parameters.cameraIndex = parameters.traceIndex;
				parameters.cameraRef = (AxCamera*)entity;

				if (parameters.transformRef != 0)
					parameters.cameraRef->BuildMatrix(parameters.transformRef->worldMatrix);

				this->currentCamera = parameters;
                    
				this->traceEvents->OnCamera(parameters);
				break;
			}
                
        case AxResourceType_Material:
			{
				AxMaterial *materialRes = (AxMaterial*)entity;

				bool hasOld = parameters.materialRef != 0;
				bool isOldOverride = hasOld && parameters.materialRef->properties[AxMaterial::propertyIndex_OverrideMaterials]->GetBool();
				bool isCurrentOverride = materialRes->properties[AxMaterial::propertyIndex_OverrideMaterials]->GetBool();
				if (!isOldOverride || isCurrentOverride)
				{
					parameters.materialBranch = parameters.entitySetRef;
					parameters.materialIndex = parameters.traceIndex;
					parameters.materialRef = materialRes;
				}

				this->traceEvents->OnMaterial(parameters);

				break;
			}

        case AxResourceType_Transform:
			{
				parameters.transformBranch = parameters.entitySetRef;
				parameters.transformIndex = parameters.traceIndex;
				parameters.transformRef = (AxTransform*)entity;

                this->traceEvents->OnTransform(parameters);

				if (parameters.transformRef->properties[AxTransform::propertyIndex_Bone]->GetBool())
				{
					int boneIndex = parameters.transformRef->properties[AxTransform::propertyIndex_BoneIndex]->GetInt();
					if (boneIndex < this->bones.count)
					{
						AxMatrix::Multiply(this->bones[boneIndex], parameters.transformRef->boneBindPoseInverse, parameters.transformRef->worldMatrix);
					}
				}
				break;
			}

        case AxResourceType_Mesh:
			{
				parameters.meshBranch = parameters.entitySetRef;
				parameters.meshIndex = parameters.traceIndex;
				parameters.meshRef = (AxMesh*)entity;

				if (!parameters.meshRef->active)
					break;

				this->traceEvents->OnMesh(parameters);
				break;
			}

		case AxResourceType_EntitySet:
			{
				AxEntitySet *entitySet = (AxEntitySet*)entity;

				AxEntitySet *parentEntitySet = parameters.entitySetRef;

				bool multiPass = entitySet->properties[AxEntitySet::propertyIndex_MultiPass]->GetBool();
				bool branch = entitySet->properties[AxEntitySet::propertyIndex_Branch]->GetBool();

				bool doMultiPass = multiPass && (parentEntitySet != 0);

				AxTraceParameters *traceParams = branch ? new AxTraceParameters(parameters) : &parameters;

				traceParams->entitySetBranch = parameters.entitySetRef;
				traceParams->entitySetIndex = parameters.traceIndex;
				traceParams->entitySetRef = (AxEntitySet*)entity;

				for (int setRefIndex = 0; setRefIndex < entitySet->references.count; setRefIndex++)
				{
					traceParams->traceIndex = setRefIndex;
					this->ProcessEntity(entitySet->references[setRefIndex], *traceParams);

					if (doMultiPass)
					{
						for (int parentSetRefIndex = parameters.traceIndex + 1; parentSetRefIndex < parentEntitySet->references.count; parentSetRefIndex++)
						{
							traceParams->traceIndex = parentSetRefIndex;
							this->ProcessEntity(parentEntitySet->references[parentSetRefIndex], *traceParams);
						}
					}
				}

				if (branch)
				{
					delete traceParams;
					this->traceEvents->OnExitEntitySet(parameters);
				}
                	
				break;
			}

        case AxResourceType_Mechanism:
			{
				parameters.mechanismRef = (AxMechanism*)entity;

				this->traceEvents->OnMechanism(parameters);
				break;
			}

		case AxResourceType_Light:
			{
				parameters.lightBranch = parameters.entitySetRef;
				parameters.lightIndex = parameters.traceIndex;
				parameters.lightRef = (AxLight*)entity;

				this->traceEvents->OnLight(parameters);
				break;
			}

		case AxResourceType_SoundEmitter:
			{
				parameters.soundEmitterBranch = parameters.entitySetRef;
				parameters.soundEmitterIndex = parameters.traceIndex;
				parameters.soundEmitterRef = (AxSoundEmitter*)entity;

				this->traceEvents->OnSoundEmitter(parameters);
				break;
			}
		case AxResourceType_SoundReceptor:
			{
				parameters.soundReceptorBranch = parameters.entitySetRef;
				parameters.soundReceptorIndex = parameters.traceIndex;
				parameters.soundReceptorRef = (AxSoundReceptor*)entity;

				this->traceEvents->OnSoundReceptor(parameters);
				break;
			}
    }
}

void Axis::ProcessScene(AxTraceEvents *traceEvents)
{
	this->traceEvents = traceEvents != 0 ? traceEvents : this->renderEvents;
	this->ProcessEntity(this->root, AxTraceParameters());
}

void Axis::RenderScene(bool presentOnScreen)
{
	this->timer->Tick();

	this->ProcessInput();

	if (this->settings->properties[AxSettings::propertyIndex_ClearScreen]->GetBool())
		this->graphicsDevice->ClearScreen(*(AxVector4*)this->settings->properties[AxSettings::propertyIndex_ClearScreenColor]->GetEffectiveValue());
	this->graphicsDevice->ClearDepthBuffer();

	this->renderEvents->SetupNewFrame();
	this->ProcessScene(this->renderEvents);

	if (presentOnScreen)
		this->PresentOnScreen();

	this->SwapLightsLists();
}

void Axis::PresentOnScreen()
{
	this->graphicsDevice->Present();
}

    
AxVector2 Axis::ScreenCoordsPixelToUnit(AxVector2 &pixelCoords, AxVector2 &screenPixelSize)
{
	return AxVector2(2.0f * pixelCoords.x / screenPixelSize.x - 1.0f, 1.0f - 2.0f * pixelCoords.y / screenPixelSize.y);
}

AxVector2 Axis::ScreenCoordsPixelToUnit(AxVector2 &pixelCoords)
{
	return this->ScreenCoordsPixelToUnit(pixelCoords, AxVector2((float)this->viewportWidth, (float)this->viewportHeight));
}

AxVector2 Axis::ScreenCoordsUnitToPixel(AxVector2 &unitCoords, AxVector2 &screenPixelSize)
{
	return AxVector2(screenPixelSize.x * (unitCoords.x + 1.0f) / 2.0f, screenPixelSize.y * (1.0f - unitCoords.x) / 2.0f);
}

AxVector2 Axis::ScreenCoordsUnitToPixel(AxVector2 &unitCoords)
{
	return this->ScreenCoordsUnitToPixel(unitCoords, AxVector2((float)this->viewportWidth, (float)this->viewportHeight));
}

bool Axis::PickByRayPointAndOrientation(AxVector3 &rayInitialPoint, AxVector3 &rayOrientation, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo)
{
    this->rayIntersectionEvents->SetupRay(rayInitialPoint, rayOrientation);
    this->ProcessScene(this->rayIntersectionEvents);
      
    if (this->rayIntersectionEvents->intersected)
    {
        intersectionInfo = this->rayIntersectionEvents->intersectionInfo;
        entityInfo = this->rayIntersectionEvents->traceResult;
        return true;
    }
    else
        return false;
}
    
bool Axis::PickByRayTwoPoints(AxVector3 &rayInitialPoint, AxVector3 &raySecondPoint, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo)
{
	AxVector3 rayOrientation;
	AxVector3::Subtract(rayOrientation, raySecondPoint, rayInitialPoint);

	return this->PickByRayPointAndOrientation(rayInitialPoint, rayOrientation, intersectionInfo, entityInfo);
}

bool Axis::PickByScreenCoordsUnit(AxVector2 &screenUnitCoords, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo)
{
    this->screenPickEvents->SetupScreenPick(screenUnitCoords);
    this->ProcessScene(this->screenPickEvents);

    intersectionInfo = this->screenPickEvents->intersectionInfo;
    entityInfo = this->screenPickEvents->entityInfo;
	return this->screenPickEvents->intersectionInfo.hasIntersected;
}

bool Axis::PickByScreenCoordsPixel(AxVector2 &screenPixelCoords, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo)
{
	return this->PickByScreenCoordsPixel(screenPixelCoords, AxVector2((float)this->viewportWidth, (float)this->viewportHeight), intersectionInfo, entityInfo);
}

bool Axis::PickByScreenCoordsPixel(AxVector2 &screenPixelCoords, AxVector2 screenPixelSize, AxIntersectionInfo &intersectionInfo, AxTraceParameters &entityInfo)
{
	return this->PickByScreenCoordsUnit(this->ScreenCoordsPixelToUnit(screenPixelCoords, screenPixelSize), intersectionInfo, entityInfo);
}

    
void Axis::CalculateNearestActiveLights(int maxCount, AxVector3 &position)
{
	this->nearestActiveLights.SetSize(this->activeLights->count);
	for (int i = 0; i < this->activeLights->count; i++)
	{
		AxActiveLightInfo *lightInfo = &this->activeLights->Get(i);
		lightInfo->distance = AxVector3::Distance(position, lightInfo->lightPos);
		this->nearestActiveLights.Set(i, lightInfo);
	}

	int nearestCount = AxMath::Min(this->activeLights->count, maxCount);

	for (int i = 0; i < nearestCount; i++)
	{
		int nearestIndex = i;
		for (int j = i + 1; j < this->nearestActiveLights.count; j++)
		{
			if (this->nearestActiveLights[j]->distance < this->nearestActiveLights[nearestIndex]->distance)
				nearestIndex = j;
		}

		if (nearestIndex != i)
		{
			AxActiveLightInfo *lightInfo = this->nearestActiveLights[i];
			this->nearestActiveLights[i] = this->nearestActiveLights[nearestIndex];
			this->nearestActiveLights[nearestIndex] = lightInfo;
		}
	}

	this->nearestActiveLights.SetSize(nearestCount);
}

AxResource *Axis::AddResource(AxResource *resource)
{
    this->resourceID++;

    resource->id = resourceID;
	
	this->resources.Add(resource);

    return resource;
}

void Axis::RemoveResourceFromSet(AxEntitySet *resourceSet, AxResource *resource)
{
	for (int i = 0; i < resourceSet->references.count; i++)
	{
		AxResource *entitySetResource = resourceSet->references[i];

		if (entitySetResource == resource)
		{
			resourceSet->references.RemoveAt(i);
			i--;
		}

		else if (entitySetResource->resourceType == AxResourceType_EntitySet)
			this->RemoveResourceFromSet((AxEntitySet*)entitySetResource, resource);
	}

}

void Axis::RemoveResourceReferences(AxResource *resource)
{
	for (int resourceIndex = 0; resourceIndex < this->resources.count; resourceIndex++)
	{
		AxResource *res = this->resources[resourceIndex];
		for (int propertyIndex = 0; propertyIndex < res->properties.count; propertyIndex++)
		{
			AxProperty *prop = res->properties[propertyIndex];
			if (prop->IsReference())
				if (prop->value == resource)
					prop->value = 0;
		}
	}
}

void Axis::RemoveResourceFromLightsList(AxResource *resource)
{
	if (resource->resourceType != AxResourceType_Light)
		return;

	for (int i = 0; i < this->activeLights1.count; i++)
		if (this->activeLights1[i].lightRef == resource)
			this->activeLights1.RemoveAt(i);

	for (int i = 0; i < this->activeLights2.count; i++)
		if (this->activeLights2[i].lightRef == resource)
			this->activeLights2.RemoveAt(i);
}

void Axis::RemoveResource(AxResource *resource)
{
	this->RemoveResourceReferences(resource);
	this->RemoveResourceFromSet(this->root, resource);
	this->RemoveResourceFromLightsList(resource);
	
	for (int i = 0; i < this->resources.count; i++)
	{
		if (this->resources[i] == resource)
		{
			delete this->resources[i];
			this->resources.RemoveAt(i);
			i--;
		}
	}
}

int Axis::GetResourceIndex(AxResource &resource)
{
    int count = this->resources.count;
    for (int i = 0; i < count; i++)
        if (this->resources[i] == &resource)
            return i;
    return -1;
}

AxResource *Axis::FindResourceX(const AxString &resourceName, AxResourceType resourceType)
{
    int count = this->resources.count;
    for (int i = 0; i < count; i++)
    {
    	AxResource *resource = this->resources[i];
        if (resource->resourceType == resourceType)
			if (resource->name.Equals(resourceName))
                return resource;
    }

    return 0;
}

AxResource *Axis::FindResourceX(long long id)
{
    int left = 0;
    int right = this->resources.count - 1;

    while (left <= right)
    {
        int index = left + (right - left) / 2;
        AxResource *resource = this->resources[index];

        if (resource->id == id)
            return resource;

        if (resource->id < id)
            left = index + 1;
        else
            right = index - 1;
    }

    return 0;
}
    
AxString Axis::AcquireResourceName(AxString &resourceName, AxResourceType resourceType)
{
    int nameIndex = 0;
    AxString result = resourceName;

    while (FindResourceX(result, resourceType) != 0)
    {
        nameIndex++;
        result = resourceName + AxString(nameIndex);
    }

    return AxString(result);
}

AxResource *Axis::FindEntity(AxResourceType resourceType, AxEntitySet *entitySet)
{
	if (entitySet == 0)
		entitySet = this->root;

	for (int i = 0; i < entitySet->references.count; i++)
	{
		AxResource *reference = entitySet->references[i];
		
		if (reference->resourceType == resourceType)
			return reference;
		
		if (reference->resourceType == AxResourceType_EntitySet)
			return this->FindEntity(resourceType, (AxEntitySet*)reference);
	}

	return 0;
}

AxResource *Axis::FindEntity(const AxString &entityName, AxResourceType resourceType, AxEntitySet *entitySet)
{
	if (entitySet == 0)
		entitySet = this->root;

	for (int i = 0; i < entitySet->references.count; i++)
	{
		AxResource *reference = entitySet->references[i];
		
		if ((reference->resourceType == resourceType) && (reference->name.Equals(entityName)))
			return reference;
		
		if (reference->resourceType == AxResourceType_EntitySet)
			return this->FindEntity(entityName, resourceType, (AxEntitySet*)reference);
	}

	return 0;
}

AxImage *Axis::TakeScreenshot()
{
	return this->TakeScreenshot(this->viewportWidth, this->viewportHeight);
}

AxImage *Axis::TakeScreenshot(int width, int height)
{
	bool originalClearOnRenderTarget = this->clearOnRenderTarget;
	int originalViewportWidth = this->viewportWidth;
	int originalViewportHeight = this->viewportHeight;

	this->clearOnRenderTarget = true;
	this->SetViewport(width, height);

	AxTexture2D *renderTarget = new AxTexture2D(this);
	renderTarget->properties[AxTexture2D::propertyIndex_Width]->SetInt(width);
	renderTarget->properties[AxTexture2D::propertyIndex_Height]->SetInt(height);
	AxEntitySet *newRoot = new AxEntitySet();
	newRoot->references.Add(renderTarget);
	newRoot->references.Add(this->root);
	this->root = newRoot;
	this->RenderScene(false);
	this->root = (AxEntitySet*)newRoot->references[1];
	delete newRoot;

	AxImage *result = renderTarget->ToImage();
	delete renderTarget;

	this->SetViewport(originalViewportWidth, originalViewportHeight);
	this->clearOnRenderTarget = originalClearOnRenderTarget;

	return result;
}

AxImage *Axis::LoadImageX(AxStream &stream)
{
	long long streamInitialPos = stream.position;

	for (int i = 0; i < this->imageMediaDispatchers.count; i++)
	{
		AxImage *image = (AxImage*)this->imageMediaDispatchers[i]->CreateObject();
		if (image->Load(stream))
			return image;

		delete image;

		stream.Seek(streamInitialPos);
	}

	return 0;
}

AxImage *Axis::LoadImageX(AxString fileName)
{
	AxStream *stream = this->fileSystem->OpenFile(this->GetFullFileName(fileName));
	if (stream == 0)
		return 0;

	AxImage *result = this->LoadImageX(*stream);

	delete stream;

	return result;
}

bool Axis::SaveImage(AxImage *image, AxString fileName, AxStream &stream)
{
	long long streamInitialPos = stream.position;

	AxString formatExtension = AxFileSystem::GetFileExtension(fileName);

	for (int i = 0; i < this->imageMediaDispatchers.count; i++)
	{
		if (this->imageMediaDispatchers[i]->IsFeatureSupported(AxModuleDispatcher::infoId_MediaSerializationFormatExtension, formatExtension, true))
		{
			AxImage *imageSaver = (AxImage*)this->imageMediaDispatchers[i]->CreateObject();
			imageSaver->Copy(*image);
			
			bool result = imageSaver->Save(stream, formatExtension.GetCharContents());
			delete imageSaver;

			if (result)
				return true;

			stream.Seek(streamInitialPos);
		}
	}

	return false;
}

bool Axis::SaveImage(AxImage *image, AxString fileName)
{
	AxStream *stream = this->fileSystem->CreateNewFile(fileName);
	if (stream == 0)
		return false;

	bool result = this->SaveImage(image, fileName, *stream);

	delete stream;

	return result;
}


AxTexture2D *Axis::AcquireTexture(AxString fileName)
{
	for (int i = 0; i < this->resources.count; i++)
	{
		AxResource *resource = this->resources[i];
		if (resource->resourceType == AxResourceType_Texture)
			if (((AxTexture2D*)resource)->fileName.Equals(fileName))
				return (AxTexture2D*)resource;
	}
	
	AxTexture2D *result = new AxTexture2D(this);
	
	if (!result->Load(fileName))
	{
		delete result;
		return 0;
	}
	

	result->name = this->AcquireResourceName(AxFileSystem::GetFileNameWithoutExtension(fileName), AxResourceType_Texture);
	this->AddResource(result);

	return result;
}

AxSoundMedia *Axis::CreateSoundMedia(AxStream &stream)
{
	long long streamInitialPos = stream.position;

	for (int i = 0; i < this->soundMediaDispatchers.count; i++)
	{
		AxSoundMedia *soundMedia = (AxSoundMedia*)this->soundMediaDispatchers[i]->CreateObject();
		if (soundMedia->Load(stream))
			return soundMedia;

		delete soundMedia;

		stream.Seek(streamInitialPos);
	}

	return 0;
}

AxSoundMedia *Axis::CreateSoundMedia(AxString fileName, AxStream **sourceStream)
{
	*sourceStream = this->fileSystem->OpenFile(this->GetFullFileName(fileName));
	if (sourceStream == 0)
		return 0;

	AxSoundMedia *result = this->CreateSoundMedia(**sourceStream);

	if (result == 0)
	{
		delete *sourceStream;
		*sourceStream = 0;
	}

	return result;
}

AxSound *Axis::AcquireSound(AxString fileName)
{
	for (int i = 0; i < this->resources.count; i++)
		if (this->resources[i]->resourceType == AxResourceType_Sound)
			if (((AxSound*)this->resources[i])->fileName.Equals(fileName))
				return (AxSound*)this->resources[i];
	
	AxSound *result = new AxSound(this);
	if (!result->Load(fileName))
	{
		delete result;
		return 0;
	}

	result->name = AxFileSystem::GetFileName(fileName);
	this->AddResource(result);

	return result;
}

AxEntity *Axis::CreateEntity(int typeId)
{
	for (int i = 0; i < this->entityDispatchers.count; i++)
	{
		AxEntity *result = this->entityDispatchers[i]->CreateEntity(this, typeId);
		if (result != 0)
			return result;
	}

	return 0;
}

void Axis::SerializeResource(AxStream *destination, AxResource *resource)
{
	AxHierarchyStreamWriter writer(*destination);

	writer.BeginChunk(Axis::SerializationId_ResourceSerialization);
	writer.stream->WriteInt32(resource->typeId);
	resource->Serialize(*destination);
	writer.EndChunk();
}

AxResource *Axis::DeserializeResource(AxStream *source)
{
	AxHierarchyStreamReader reader(*source);

	if (reader.ReadChunkHeader())
	{
		if (reader.chunkId == Axis::SerializationId_ResourceSerialization)
		{
			int typeId = reader.stream->ReadInt32();

			AxResource *result = (AxResource*)this->CreateEntity(typeId);
			if (result != 0)
			{
				if (result->Deserialize(*source))
					return result;
				else
					delete result;
			}
		}
	}

	return 0;
}


void Axis::ProcessInput()
{
	for (int i = 0; i < this->inputModels.count; i++)
		this->inputModels[i]->Process();

	for (int i = 0; i < this->inputDevices.count; i++)
		this->inputDevices[i]->Update();
}

void Axis::SwapLightsLists()
{
	AxList<AxActiveLightInfo> *temp = this->activeLights;
	this->activeLights = this->activeLightsCollecting;
	this->activeLightsCollecting = temp;

	this->activeLightsCollecting->SetSize(0);
}

AxString Axis::GetFullFileName(AxString fileName)
{
	if (AxFileSystem::IsAbsolutePath(fileName))
		return AxString(fileName);
	else
		return AxString(AxFileSystem::MergePaths(AxPlatformUtils::GetApplicationPath(), fileName));
}

void Axis::Attach(AxEntitySet *sourceParentSet, int sourceIndex, AxEntitySet *destinationParentSet, int destinationIndex, AxTransform *parentTransform)
{
	if (destinationParentSet == 0)
		destinationParentSet = this->root;

	if (destinationIndex == -1)
		destinationIndex = destinationParentSet->references.count;

	AxResource *movedResource = sourceParentSet->references[sourceIndex];

	switch (movedResource->resourceType)
	{
		case AxResourceType_EntitySet:
		{
			AxEntitySet *movedEntitySet = (AxEntitySet*)movedResource;

			if (movedEntitySet->HierarchyContains(destinationParentSet) || (movedEntitySet == destinationParentSet))
				return;

			int refCount = movedEntitySet->references.count;
			for (int i = 0; i < refCount; i++)
			{
				AxResource *ref = movedEntitySet->references[i];
				if (ref->resourceType == AxResourceType_Transform)
					((AxTransform*)ref)->RelateTo(parentTransform);
			}

			break;
		}

		case AxResourceType_Transform:
		{
			((AxTransform*)movedResource)->RelateTo(parentTransform);

			break;
		}
	}

	sourceParentSet->references.RemoveAt(sourceIndex);
	destinationParentSet->references.Insert(destinationIndex, movedResource);
}

void Axis::Detach(AxEntitySet *sourceParentSet, int sourceIndex)
{
	this->Attach(sourceParentSet, sourceIndex, 0, -1, 0);
}

void *Axis::GetRenderingWindow()
{
	return this->windowHandle;
}
