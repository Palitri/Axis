/**
 * Creates a new dispatcher for AxWebGLGraphicsDevice
 * @constructor
 */
function AxWebGLGraphicsDeviceDispatcher()
{
    AxModuleDispatcher.call(this);
}

AxWebGLGraphicsDeviceDispatcher.prototype = Object.create(AxModuleDispatcher.prototype);


/**
 * Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
 * @param {Integer} infoId Specifies what information is requested
 * @param {Integer} infoTag An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
 * @param {AxString} info A string in which the requested information will be returned
 * @return {Boolean} True if the requested information is available and returned in the info parameter, false otherwise
 */
AxWebGLGraphicsDeviceDispatcher.prototype.GetInfo = function(infoId, infoTag, info)
{
    switch (infoId)
    {
        case AxModuleDispatcher.infoId_TypeId:
            info = AxString(AxModuleDispatcher.typeId_GraphicsDevice);
            return true;

        case AxModuleDispatcher.infoId_Name:
            info = 'Axis WebGL';
            return true;

        case AxModuleDispatcher.infoId_Description:
            info = 'Axis WebGL is an implementation of a graphics device for use in web browsers which support HTML 5';
            return true;
    }

    return false;
};

/**
 * Creates an instance of the dispatched object.
 * @return {AxWebGLGraphicsDevice}
 */
AxWebGLGraphicsDeviceDispatcher.prototype.CreateObject = function()
{
	return new AxWebGLGraphicsDevice();
};