#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxString.h"

// TODO: Consider removing FormatExtension and the info tag parameter and passing the extensions as one string, extensions divided by '/'
// TODO: Consider renaming AxModuleDispather to AxComponentDispatcher, because it does not actually dispatch modules, rather then dispatch the components of a module
class AXDLLCLASS AxModuleDispatcher
{
public:
	// Request the typeId of the dispatched object
	static const unsigned int infoId_TypeId									= 0x00000000;
	// Request a user friendly name of the dispatched object
	static const unsigned int infoId_Name									= 0x10000000;
	// Request a user friendly description of the dispatched object
	static const unsigned int infoId_Description							= 0x20000000;
	// Media serialization and deserialization. As the dispatched object might support number of formats for serialization and different number for deserialization, info on each can be accessed by providing an index to the infoTag parameter in the AxMediaDispatcher::GetInfoType method. The method returning false means the index and all consequent indices are invalid
	// Request the format extension of a type supported for serialization by the dispatched object. This is effectively a file type extension. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
	static const unsigned int infoId_MediaSerializationFormatExtension		= 0x30000000;
	// Request the format description of a type supported for serialization by the dispatched object. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
	static const unsigned int infoId_MediaSerializationFormatDescription	= 0x40000000;
	// Request the format extension of a type supported for deserialization by the dispatched object. This is effectively a file type extension. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
	static const unsigned int infoId_MediaDeserializationFormatExtension	= 0x50000000;
	// Request the format description of a type supported for deserialization by the dispatched object. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
	static const unsigned int infoId_MediaDeserializationFormatDescription	= 0x60000000;

	static const unsigned int typeId_Image			= 0;
	static const unsigned int typeId_Sound			= 1;
	static const unsigned int typeId_Scene			= 2;
	static const unsigned int typeId_GraphicsDevice	= 3;
	static const unsigned int typeId_AudioDevice	= 4;
	static const unsigned int typeId_FileSystem		= 5;
	static const unsigned int typeId_InputDevice	= 6;

	AxModuleDispatcher(void);
	virtual ~AxModuleDispatcher(void);

	// Checks whether a given feature is supported, by the checked feature's id and the name of the feature
	// For example, to check if a given serialization format is supported, the method should be provided with the infoId_MediaSerializationFormatExtension as featureInfoId and the name of the format's extension as featureName. In this example, the ignoreCase parameter would best be set to true
	// featureInfoId The id of the type of feature which is to be checked
	// featureName The name of the feature which is to be checked
	// Returns true if the feature is supported
	bool IsFeatureSupported(unsigned int featureInfoId, AxString &featureName, bool ignoreCase = false);

	// Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
	// infoId - Specifies what information is requested
	// infoTag - An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
	virtual bool GetInfo(unsigned int infoId, int infoTag, AxString &info) = 0;
	// Creates an instance of the dispatched object.
	virtual void *CreateObject() = 0;
};

