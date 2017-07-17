/**
 * Creates a new module dispatcher. 
 * This is a virtual prototype meant to be inherited by overrideing its methods
 * @constructor
 */
function AxModuleDispatcher()
{
    
}


// Request of the typeId of the dispatched object
AxModuleDispatcher.infoId_TypeId				= 0x00000000;
// Request of a user friendly name of the dispatched object
AxModuleDispatcher.infoId_Name					= 0x10000000;
// Request of a user friendly description of the dispatched object
AxModuleDispatcher.infoId_Description				= 0x20000000;
// Media serialization and deserialization. As the dispatched object might support number of formats for serialization and different number for deserialization, info on each can be accessed by providing an index to the infoTag parameter in the AxMediaDispatcher::GetInfoType method. The method returning false means the index and all consequent indices are invalid
// Request the format extension of a type supported for serialization by the dispatched object. This is effectively a file type extension. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
AxModuleDispatcher.infoId_MediaSerializationFormatExtension	= 0x30000000;
// Request the format description of a type supported for serialization by the dispatched object. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
AxModuleDispatcher.infoId_MediaSerializationFormatDescription	= 0x40000000;
// Request the format extension of a type supported for deserialization by the dispatched object. This is effectively a file type extension. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
AxModuleDispatcher.infoId_MediaDeserializationFormatExtension	= 0x50000000;
// Request the format description of a type supported for deserialization by the dispatched object. Index of type is fed to the infoTag parameter of the AxMediaDispatcher::GetInfoType method
AxModuleDispatcher.infoId_MediaDeserializationFormatDescription	= 0x60000000;


AxModuleDispatcher.typeId_Image             = 0;
AxModuleDispatcher.typeId_Sound             = 1;
AxModuleDispatcher.typeId_Scene             = 2;
AxModuleDispatcher.typeId_GraphicsDevice    = 3;
AxModuleDispatcher.typeId_AudioDevice       = 4;
AxModuleDispatcher.typeId_FileSystem        = 5;
AxModuleDispatcher.typeId_InputDevice       = 6;



/**
 * Checks whether a given feature is supported, by the checked feature's id and the name of the feature
 * For example, to check if a given serialization format is supported, the method should be provided with the infoId_MediaSerializationFormatExtension as featureInfoId and the name of the format's extension as featureName. In this example, the ignoreCase parameter would best be set to true
 * @param {Integer} featureInfoId The id of the type of feature which is to be checked
 * @param {AxString} featureName The name of the feature which is to be checked
 * @param {!Boolean} ignoreCase Denotes whether to compare the featureName case-insesitively
 * @returns {Boolean} True if the feature is supported
 */
AxModuleDispatcher.prototype.IsFeatureSupported = function(featureInfoId, featureName, ignoreCase)
{
    var testedFeatureTagIndex = 0;
    var testedFeatureName = new AxString();
    var featureNameNormalized = ignoreCase ? featureName.ToLower() : featureName;
    while (this.GetInfo(featureInfoId, testedFeatureTagIndex, testedFeatureName))
        if (featureNameNormalized.Equals(ignoreCase ? testedFeatureName.ToLower() : testedFeatureName))
            return true;
        else
            testedFeatureTagIndex++;

    return false;
};

/**
 * Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
 * @param {Integer} infoId Specifies what information is requested
 * @param {Integer} infoTag An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
 * @param {AxString} info A string in which the requested information will be returned
 * @return {Boolean} True if the requested information is available and returned in the info parameter, false otherwise
 */
AxModuleDispatcher.prototype.GetInfo = function(infoId, infoTag, info) { };

/**
 * Creates an instance of the dispatched object.
 */
AxModuleDispatcher.prototype.CreateObject = function() { };
