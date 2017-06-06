/**
 * Creates an object which contains parameters used for serialization
 * @constructor
 */
function AxSerializationParameters()
{
    // The root dir for the serialization. Used as a base for relative paths. Can be used when loading from a stream, when fullSourceName is not available
    this.rootDir = new AxString();
    // A friendly name of the serialization source. Used when such is needed, for example to give names to resources
    this.friendlyName = new AxString();
    // The full name of the source. Generally, the path with file name.
    this.fullSourceName = new AxString();
}