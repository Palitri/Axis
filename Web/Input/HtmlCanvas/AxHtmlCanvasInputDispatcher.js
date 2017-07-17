/**
 * Creates a new dispatcher for AxHtmlCanvasInputDevice
 * @constructor
 */
function AxHtmlCanvasInputDispatcher()
{
    AxModuleDispatcher.call(this);
}

AxHtmlCanvasInputDispatcher.prototype = Object.create(AxModuleDispatcher.prototype);


/**
 * Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
 * @param {Integer} infoId Specifies what information is requested
 * @param {Integer} infoTag An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
 * @param {AxString} info A string in which the requested information will be returned
 * @return {Boolean} True if the requested information is available and returned in the info parameter, false otherwise
 */
AxHtmlCanvasInputDispatcher.prototype.GetInfo = function(infoId, infoTag, info)
{
    switch (infoId)
    {
        case AxModuleDispatcher.infoId_TypeId:
            info = AxString(AxModuleDispatcher.typeId_InputDevice);
            return true;

        case AxModuleDispatcher.infoId_Name:
            info = 'Html canvas input';
            return true;

        case AxModuleDispatcher.infoId_Description:
            info = 'Html canvas input device utilizes the input received from an Html Canvas element';
            return true;
    }

    return false;
};

/**
 * Creates an instance of the dispatched object.
 * @return {AxHtmlCanvasInputDevice}
 */
AxHtmlCanvasInputDispatcher.prototype.CreateObject = function()
{
    return new AxHtmlCanvasInputDevice();
};