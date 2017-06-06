/**
 * Creates a new dispatcher for AxWebImage
 * @constructor
 */
function AxWebImageDispatcher()
{
    AxModuleDispatcher.call(this);
}

AxWebImageDispatcher.prototype = Object.create(AxModuleDispatcher.prototype);



/**
 * Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
 * @param {Integer} infoId Specifies what information is requested
 * @param {Integer} infoTag An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
 * @param {AxString} info A string in which the requested information will be returned
 * @return {Boolean} True if the requested information is available and returned in the info parameter, false otherwise
 */
AxWebImageDispatcher.prototype.GetInfo = function(infoId, infoTag, info)
{
    var numDeserializationFormats = 5;
    var deserializationFormats = [ 'jpg', 'jpeg', 'gif', 'png', 'tga' ];
    var deserializationFormatsDescription = [ 'Jpeg', 'Jpeg', 'Graphics Interchange Format', 'Portable Network Graphics' ];
	
    var numSerializationFormats = 0;
    var serializationFormats = [ ];
    var serializationFormatsDescription = [ ];

    switch (infoId)
    {
        case AxModuleDispatcher.infoId_TypeId:
            info = AxString(AxModuleDispatcher.typeId_Image);
            return true;

        case AxModuleDispatcher.infoId_Name:
            info = 'Web Image';
            return true;

        case AxModuleDispatcher.infoId_Description:
            info = 'Serializes and deserializes images using the web browser\'s capabilities';
            return true;

        case AxModuleDispatcher.infoId_MediaDeserializationFormatExtension:
            if ((infoTag >= 0) && (infoTag < numDeserializationFormats))
            {
                info = deserializationFormats[infoTag];
                return true;
            }
            break;

        case AxModuleDispatcher.infoId_MediaDeserializationFormatDescription:
            if ((infoTag >= 0) && (infoTag < numDeserializationFormats))
            {
                info = deserializationFormatsDescription[infoTag];
                return true;
            }
            break;

        case AxModuleDispatcher.infoId_MediaSerializationFormatExtension:
            if ((infoTag >= 0) && (infoTag < numSerializationFormats))
            {
                info = serializationFormats[infoTag];
                return true;
            }
            break;

        case AxModuleDispatcher.infoId_MediaSerializationFormatDescription:
            if ((infoTag >= 0) && (infoTag < numSerializationFormats))
            {
                info = serializationFormatsDescription[infoTag];
                return true;
            }
            break;	
    }

    return false;
};

/**
 * Creates an instance of the dispatched object.
 * @return {AxWebImage}
 */
AxWebImageDispatcher.prototype.CreateObject = function()
{
	return new AxWebImage();
};