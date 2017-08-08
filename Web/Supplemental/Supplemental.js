/**
 * Gets fired when the Axis web library progresses on loading a script file
 * @callback ScriptFilesLoadProgressCallback
 * @param {Number} Percentage of loading progress
 * @param {string} Url of the file just loaded
 */

/**
 * Gets fired when Axis is fully loaded by the AxisInitialize method
 * @callback ScriptFilesLoadedCallback
 */

/**
 * This function is provided only for consistency with Axis production project.
 * Originally, in the Axis production project, this function initializes Axis web engine by loading all necessary files of the library
 * When loading Axis from a single .js file, it is unnecessary and is provided only to be available, so switching between the prodution project and the single compiled file would be seemless.
 * @param {!ScriptFilesLoadedCallback} onLoaded Callback which gets fired once all libraries have been loaded and axis is ready to go
 * @param {!ScriptFilesLoadProgressCallback} onProgress Callback which gets fired each time the intialization has progressed. A Number parameter gives the progression percentage
 * @returns {undefined}
 */
function AxisInitialize(onLoaded, onProgress)
{
    if ((typeof onProgress !== 'undefined') && (onProgress !== null))
        onProgress(1.0, "");
    
    if ((typeof onLoaded !== 'undefined') && (onLoaded !== null))
        onLoaded();
}