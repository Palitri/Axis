/**
 * Creates a new dispatcher for AxWebImage
 * @constructor
 */
function AxWebFileSystemDispatcher()
{
    AxModuleDispatcher.call(this);
}

AxWebFileSystemDispatcher.prototype = Object.create(AxModuleDispatcher.prototype);


/**
 * Requests information about the dispatched object. The requested is specified by the infoId parameter, and depending on it, the infoTag parameter may have to be used. If the requested information is availabe, it is filled in the info parameter and the method returns true, otherwise returns false.
 * @param {Integer} infoId Specifies what information is requested
 * @param {Integer} infoTag An additional parameter, used for some infoIds. Depending on which infoId is used, the infoTag may or may not have a specific meaning.
 * @param {AxString} info A string in which the requested information will be returned
 * @return {Boolean} True if the requested information is available and returned in the info parameter, false otherwise
 */
AxWebFileSystemDispatcher.prototype.GetInfo = function(infoId, infoTag, info)
{
    switch (infoId)
    {
        case AxModuleDispatcher.infoId_TypeId:
            info = AxString(AxModuleDispatcher.typeId_FileSystem);
            return true;

        case AxModuleDispatcher.infoId_Name:
            info = 'Web File System';
            return true;

        case AxModuleDispatcher.infoId_Description:
            info = 'Axis Web File System is a file system implementation which provides access to remote files via HTTP';
            return true;
    }

    return false;
};

/**
 * Creates an instance of the dispatched object.
 * @return {AxWebGLGraphicsDevice}
 */
AxWebFileSystemDispatcher.prototype.CreateObject = function()
{
	return new AxWebFileSystem();
};