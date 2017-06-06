/**
 * Creates a new dispatcher for axis native scene media
 * @constructor
 */
function AxNativeSceneDispatcher()
{
    AxModuleDispatcher.call(this);
}

AxNativeSceneDispatcher.prototype = Object.create(AxModuleDispatcher.prototype);



/**
 * Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
 * @param {Integer} infoId Specifies what information is requested
 * @param {Integer} infoTag An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
 * @param {AxString} info A string in which the requested information will be returned
 * @return {Boolean} True if the requested information is available and returned in the info parameter, false otherwise
 */
AxNativeSceneDispatcher.prototype.GetInfo = function(infoId, infoTag, info)
{
    switch (infoId)
    {
        case AxModuleDispatcher.infoId_TypeId:
            info.Set(AxModuleDispatcher.typeId_Scene);
            return true;

        case AxModuleDispatcher.infoId_Name:
            info.Set('Axis');
            return true;

        case AxModuleDispatcher.infoId_Description:
            info = "Axis native scene format. Serializes and deserializes scene in axis' native format";
            return true;

        case AxModuleDispatcher.infoId_MediaSerializationFormatExtension:
            if (infoTag === 0)
            {
                info = AxNativeScene.formatName;
                return true;
            }
            break;

        case AxModuleDispatcher.infoId_MediaSerializationFormatDescription:
            if (infoTag === 0)
            {
                info = "Axis";
                return true;
            }
            break;

        case AxModuleDispatcher.infoId_MediaDeserializationFormatExtension:
            if (infoTag === 0)
            {
                info = AxNativeScene.formatName;
                return true;
            }
            break;

        case AxModuleDispatcher.infoId_MediaDeserializationFormatDescription:
            if (infoTag === 0)
            {
                info = "Axis";
                return true;
            }
            break;
    }

    return false;
};

/**
 * Creates an instance of AxNativeScene
 * @return {AxNativeScene} An instance of a native axis scene media
 */
AxNativeSceneDispatcher.prototype.CreateObject = function()
{
    return new AxNativeScene();
};