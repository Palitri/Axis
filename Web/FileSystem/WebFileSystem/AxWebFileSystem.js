/**
 * 
 * Creates a new instance of AxWebFileSystem
 * @constructor
 */
function AxWebFileSystem()
{
    AxFileSystem.call(this);
}

AxWebFileSystem.prototype = Object.create(AxFileSystem.prototype);


/**
 * Opens a file for reading
 * @param {AxString} path Path to the file to open
 * @param {*} context Object representing a context, which will be passed to the callback
 * @param {AxFileSystemFileOpened} callback A callback which is called after the file gets opened
 * @return {AxStream} Stream with the file's contents
 */
AxWebFileSystem.prototype.OpenFile = function(path, context, callback)
{
    var httpRequest = new XMLHttpRequest();
    httpRequest.responseType = 'arraybuffer';
    //xmlHttp.onreadystatechange = function() { if (xmlHttp.readyState == 4 && xmlHttp.status == 200) {} }

    httpRequest.fileSystem = this;
    
    httpRequest.addEventListener("progress", function(eventArg)
    {
        //console.log("progress: " + this.path.contents);
    });
    
    httpRequest.addEventListener("load", function(eventArg)
    {
        var stream = new AxMemoryStream();
        stream.AxMemoryStream_2(httpRequest.response, httpRequest.response.byteLength);
        if (!AxUtils.IsUndefinedOrNull(callback))
            callback(context, stream, path, this.fileSystem);
        
        //console.log("loaded: " + this.path.contents);
    });
    
    httpRequest.addEventListener("error", function(eventArg)
    {
        //console.log("error: " + this.path.contents);
    });
    
    httpRequest.addEventListener("abort", function(eventArg)
    {
        //console.log("abort: " + this.path.contents);
    });
    
    
    httpRequest.open('GET', this.ToNativePathSyntax(path).GetContents(), true);
    httpRequest.send();
};

/**
 * Creates a file for writing
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to create
 * @return {AxStream} Stream which represents the newly created file
 */
AxWebFileSystem.prototype.CreateNewFile = function(path) { };

// 
/**
 * Gets information on the specified file.
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to get information on
 * @param {AxFileInfo} result Parameter in which to return the information on the file
 * @return {Boolean} True if succeeded or false otherwise (for example if the file does not exist)
 */
AxWebFileSystem.prototype.GetFileInfo = function(path, result) { };

/**
 * Gets information on the contents of the specified directory.
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the directory whose contents to get
 * @param {AxList} result Parameter in which to return a list of AxFileInfo items contained in the directory
 * @return {Boolean} True if succeeded or false otherwise (for example if the directory does not exist)
 */
AxWebFileSystem.prototype.GetDirectoryContents = function(path, result) { };

/**
 * Converts a path from the specific file system's path syntax to Axis' path syntax
 * If the path is already in Axis' path syntax, it is returned unaltered
 * @param {AxString} path A path either in the specific file system's syntax or in Axis' syntax
 * @return {AxString} The path in the format of Axis' path syntax
 */
AxWebFileSystem.prototype.FromNativePathSyntax = function(path) 
{ 
    var x = path.Replace('://', AxFileSystem.rootSignature + AxFileSystem.dirSplitter);
    return x; 
};

/**
 * Converts a path from Axis' path syntax to the specific file system's path syntax
 * If the path is already in the file system's path syntax, it is returned unaltered
 * @param {AxString} path A path either in Axis' syntax or in the file systems' syntax
 * @return {AxString} The path in the format of the specific file system's path syntax
 */
AxWebFileSystem.prototype.ToNativePathSyntax = function(path) 
{ 
    // Already in native syntax
    if (path.IndexOf('://') !== -1)
        return path;
    
    return path.Replace(AxFileSystem.rootSignature + AxFileSystem.dirSplitter, '://'); 
};