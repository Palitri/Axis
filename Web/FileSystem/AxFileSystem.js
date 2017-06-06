function AxFileSystem()
{
}


AxFileSystem.dirSplitter = '/';
AxFileSystem.extensionSplitter = '.';
AxFileSystem.rootSignature = ':';
AxFileSystem.previousDir = '..';
AxFileSystem.currentDir = '.';


/**
 * Gets fired when a file system has openened a file asynchronously and the file stream is ready
 * @callback AxFileSystemFileOpened
 * @param {*} context The opened file's stream
 * @param {AxStream} stream The opened file's stream
 * @param {AxString} path The path of the opened file
 * @param {AxFileSystem} fileSystem A reference to the file system
 */



/**
 * Explicitly dispose of the resources allocated by the object
 */
AxFileSystem.prototype.Dispose = function()
{
};



// Virtual methods



/**
 * Opens a file for reading
 * @param {AxString} path Path to the file to open
 * @param {*} context Object representing a context, which will be passed to the callback
 * @param {AxFileSystemFileOpened} callback A callback which is called after the file gets opened
 * @return {AxStream} Stream with the file's contents
 */
AxFileSystem.prototype.OpenFile = function(path, context, callback) { };

/**
 * Creates a file for writing
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to create
 * @return {AxStream} Stream which represents the newly created file
 */
AxFileSystem.prototype.CreateNewFile = function(path) { };

// 
/**
 * Gets information on the specified file.
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to get information on
 * @param {AxFileInfo} result Parameter in which to return the information on the file
 * @return {Boolean} True if succeeded or false otherwise (for example if the file does not exist)
 */
AxFileSystem.prototype.GetFileInfo = function(path, result) { };

/**
 * Gets information on the contents of the specified directory.
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the directory whose contents to get
 * @param {AxList} result Parameter in which to return a list of AxFileInfo items contained in the directory
 * @return {Boolean} True if succeeded or false otherwise (for example if the directory does not exist)
 */
AxFileSystem.prototype.GetDirectoryContents = function(path, result) { };

/**
 * Converts a path from the specific file system's path syntax to Axis' path syntax
 * If the path is already in Axis' path syntax, it is returned unaltered
 * @param {AxString} path A path either in the specific file system's syntax or in Axis' syntax
 * @return {AxString} The path in the format of Axis' path syntax
 */
AxFileSystem.prototype.FromNativePathSyntax = function(path) { return path; };

/**
 * Converts a path from Axis' path syntax to the specific file system's path syntax
 * If the path is already in the file system's path syntax, it is returned unaltered
 * @param {AxString} path A path either in Axis' syntax or in the file systems' syntax
 * @return {AxString} The path in the format of the specific file system's path syntax
 */
AxFileSystem.prototype.ToNativePathSyntax = function(path) { return path; };



// Non-virtual methods



/**
 * Checkes whether a file with a given path exists
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to check on
 * @return {Boolean} True if the file exists. False otherwise
 */
AxFileSystem.prototype.FileExists = function(path)
{
    throw 'Not implemented';
};

/**
 * Checkes whether a directory with a given path exists
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to check on
 * @return {Boolean} True if the directory exists. False otherwise
 */
AxFileSystem.prototype.DirectoryExists = function(path)
{
    throw 'Not implemented';
};

/**
 * Checkes whether a file or a directory with a given path exists
 * This method is only provided for consistency with other implementations. It is not meant to be used in this implementation
 * @param {AxString} path Path to the file to check on
 * @return {Boolean} True if a file or a directory with the given path exists. False otherwise
 */
AxFileSystem.prototype.FileOrDirectoryExists = function(path)
{
    throw 'Not implemented';
};



// Static methods



/**
 * Checks if a given path is an absolute path
 * @param {AxString} path The path to be checked
 * @return {Boolean} True if the path is absolute. False otherwise, for example if the path is relative
 */
AxFileSystem.IsAbsolutePath = function(path)
{
    var firstDelimiter = path.IndexOf(AxFileSystem.dirSplitter);
    if (firstDelimiter === -1)
        firstDelimiter = path.length;
    return path.IndexOf(AxFileSystem.rootSignature) !== -1;
};



/**
 * Returns a string containing a symbolic size of the given one, shortening large numbers using their magnitudes. For example, an input size of 10485760 may return simply "10 MB".
 * @param {Integer} size description
 * @param {!Integer} magnitudeBase The base of magnitude. Allows to use different magnitudes, such as 1024 or 1000 for example. Default is 1024
 * @param {!Integer} magnitudeThreshold The smallest value that can be used to display a greater magnitude. This allows the rounding into a greater magnitude to occur at greater values. For example, at magnitudeBase 1024, a magnitude threshold of 4 will cause values between 1024 to 4095 not to appear in the higher magnitude (1 to 3) which would keep precision for smaller values
 * @return {AxString} A string with the symbolic size which uses the notation of multiple of bytes
 */
AxFileSystem.GetSymbolicSize = function(size, magnitudeBase, magnitudeThreshold)
{
    var numMagnitudes = 6;
    var magnitudeNames = [ 'B', 'KB', 'MB', 'GB', 'TB', 'PB' ];
    
    if (AxUtils.IsUndefinedOrNull(magnitudeBase))
        magnitudeBase = 1024;
    
    if (AxUtils.IsUndefinedOrNull(magnitudeThreshold))
        magnitudeThreshold = 4;

    var threshold = magnitudeBase * magnitudeThreshold;
    var magnitudeIndex = 0;
    while ((size > threshold) && (magnitudeIndex < numMagnitudes))
    {
        size = AxMath.Floor(size / magnitudeBase);
        magnitudeIndex++;
    }

    return new AxString(size.toString() + ' ' + magnitudeNames[magnitudeIndex]);
};

/**
 * Creates a path, which is relative to the one given in "basePath". In other words, how to go from "basePath" to "path", or how to reach "path", starting from "basePath". In arithmetical terms, basePath + GetRelativeFilePath(basePath, path) should be equivalent, but not necessarily equal, to path.
 * For example, if path is "C:\Files\File.txt", and basePath is "C:", the result will be "Files\File.txt", and if basePath is "C:\Files\Subdir", the result will be "..\File.txt"
 * @param {AxString} basePath Base path to relate the given path to
 * @param {AxString} path Path to be converted to relative to the given basePath
 * @return {AxString} The path relative to the given basePath
 */
AxFileSystem.GetRelativeFilePath = function(basePath, path)
{
    if (basePath.length === 0)
        return AxString(path);

    var fullBasePath = new AxString(basePath);
    if (!fullBasePath.EndsWith(AxFileSystem.dirSplitter))
        fullBasePath += AxFileSystem.dirSplitter;

    // Find the delimiter, until which path and fullBasePath are the same
    var delimiterIndex = -1;
    var index = 0;
    var length = AxMath.Min(fullBasePath.length, path.length);
    while (index < length)
    {
        if (path.Get(index) === fullBasePath.Get(index))
        {
            if (path.Get(index) === AxFileSystem.dirSplitter)
            {
                delimiterIndex = index;
            }
        }
        else
            break;

        index++;
    }

    result = new AxString();

    // If the file is not inside of the the fullPath directory (or in an inner directory), then add a back path
    if (delimiterIndex !== (fullBasePath.length - 1))
    {
        var pos = delimiterIndex;
        do
        {
            pos = fullBasePath.IndexOf(AxFileSystem.dirSplitter, pos + 1);
            if (pos !== -1)
                result.Insert((new AxString(AxFileSystem.previousDir)).Insert(AxFileSystem.dirSplitter));
            else
                break;
        }
        while (true);
    }

    result.Insert(path.Remove(0, delimiterIndex + 1));

    return new AxString(result);
};

/**
 * Normalizes a path, rebuilding it regarding and removing the "." and ".." directories
 * The normalized path should point to the same location as the original, but have any redundant "." and ".." directories removed
 * @param {AxString} path The path to be normalized
 * @return {AxString} The normalized path
 */
AxFileSystem.NormalizePath = function(path)
{
    pathSegments = new AxStrings();
    pathSegments.SplitString(path, AxFileSystem.dirSplitter);

    var normalizedSegments = pathSegments.count > 0 ? new Int32Array(pathSegments.count) : null;
    var numNormalizedSegments = 0;

    for (var segmentIndex = 0; segmentIndex < pathSegments.count; segmentIndex++)
    {
        if (pathSegments.Get(segmentIndex).Equals(AxFileSystem.currentDir))
        {
        }

        else if (pathSegments.Get(segmentIndex).Equals(AxFileSystem.previousDir))
        {
            if (numNormalizedSegments > 0)
                numNormalizedSegments--;
        }

        else
        {
            normalizedSegments[numNormalizedSegments] = segmentIndex;
            numNormalizedSegments++;
        }
    }


    result = new AxString();

    // Optimize, so that the string's length is not changed for every segment
    var resultLength = AxMath.Max(numNormalizedSegments - 1, 0);
    for (var segmentIndex = 0; segmentIndex < numNormalizedSegments; segmentIndex++)
        resultLength += pathSegments.Get(normalizedSegments[segmentIndex]).length;

    result.EnsureCapacity(resultLength);

    for (var segmentIndex = 0; segmentIndex < numNormalizedSegments; segmentIndex++)
    {
        result.Insert(pathSegments.Get(normalizedSegments[segmentIndex]));
        if (segmentIndex !== numNormalizedSegments - 1)
            result.Insert(AxFileSystem.dirSplitter);
    }

    if (normalizedSegments !== null)
        normalizedSegments = null;

    return result;
};

/**
 * Returns only the file name from a full file path. Included file extension
 * @param {AxString} path The full file path from which to extract the file name
 * @return {AxString} The file name and extension extracted from the full path
 */
AxFileSystem.GetFileName = function(path)
{
    var index = path.LastIndexOf(AxFileSystem.dirSplitter) + 1;

    return path.SubString(index, path.length - index);
};

/**
 * Returns only the file extension from a given file path or file name
 * @param {AxString} path Original file path or file name from which to extract the file extension
 * @return {AxString} The file extension extracted from the given file path of file name
 */
AxFileSystem.GetFileExtension = function(path)
{
    var index = path.LastIndexOf(AxFileSystem.extensionSplitter);
    if ((index === -1) || (index < path.LastIndexOf(AxFileSystem.dirSplitter)))
        return new AxString();

    index++;

    return path.SubString(index, path.length - index);
};

/**
 * Returns only the file name without the extension from a full file path
 * @param {AxString} path The full file path from which to extract the file name
 * @return {AxString} The file name without extension, extracted from the full path
 */
AxFileSystem.GetFileNameWithoutExtension = function(path)
{
    var start = path.LastIndexOf(AxFileSystem.dirSplitter) + 1;

    var end = path.LastIndexOf(AxFileSystem.extensionSplitter);
    if ((end === -1) || (end < start))
        end = path.length;

    return path.SubString(start, end - start);
};

/**
 * Creates a file name with a given extension. If the given file name has an extension it gets changed with the new one
 * @param {AxString} fileName The original file name to use with the new extension. If it already has an extension, then this extension gets changed with the new one
 * @param {AxString} extension The extension for the new file name
 * @return {AxString} The new file name, constructed from the given original file name and the given extension
 */
AxFileSystem.CreateFileNameWithExtension = function(fileName, extension)
{
    var splitterPos = fileName.LastIndexOf(AxFileSystem.extensionSplitter);
    if (splitterPos !== 0)
    {
        if (splitterPos > fileName.LastIndexOf(AxFileSystem.dirSplitter))
        {
            return AxString(fileName.SubString(0, splitterPos) + "." + extension);
        }
    }

    return AxString(fileName + "." + extension);
};

/**
 * Gets the directory of a full file path
 * @param {AxString} path The full file path from which to extract the directory
 * @return {AxString} The directory of the given path
 */
AxFileSystem.GetFileDir = function(path)
{
    return path.SubString(0, path.LastIndexOf(AxFileSystem.dirSplitter));
};

/**
 * Merges two paths by appending the second path - a relative one, to the first one - an absolute or relative one
 * @param {AxString} path1 The path to which to append. Can be an absolute or relative path
 * @param {AxString} path2 The path which is to be appended. Can be a relative path
 * @return {AxString} A normalized path constructed by merging the two given paths
 */
AxFileSystem.MergePaths = function(path1, path2)
{
    var p = new AxString(path1);
    p.Insert(AxFileSystem.dirSplitter);
    p.Insert(path2);
    return AxFileSystem.NormalizePath(p);
};