/**
 * Provides various platform-related functionality
 * @constructor
 */
function AxPlatformUtils()
{
}

/**
 * Gets the current time stamp of the system's timer
 * The measuring unit is given by the GetPerformanceCounterFrequency method, which returns how many of the units are equal to one second
 * @return {Integer} The current time stamp
 */
AxPlatformUtils.GetPerformanceCounter = function()
{
    return (new Date()).getTime();
};

/**
 * Gets the frequency of the system's timer. The frequency is the timer's precicion and is measured in units per second
 * @return {Integer} The frequency of the system's timer in units per second
 */
AxPlatformUtils.GetPerformanceCounterFrequency = function()
{
    return 1000;
};

/**
 * Shows a message box on the screen
 * @param {String} text Contents of the message
 */
AxPlatformUtils.ShowMessage = function(text)
{
    window.alert(text);
};

/**
 * Returns the full name of the application, including absolute path
 * @returns {AxString} The full name of the application, including absolute path
 */
AxPlatformUtils.GetApplicationName = function()
{
    return new AxString(window.location.href);
};

/**
 * Returns the path to the application
 * @return {AxString} The path to the application
 */
AxPlatformUtils.GetApplicationPath = function()
{
    var applicationName = AxPlatformUtils.GetApplicationName();
    return applicationName.SubString(0, applicationName.LastIndexOf('/'));
};

/**
 * Returns the size of the given window
 * In the WebGL implementation, the window is represented by a canvas
 * @param {Canvas} windowHandle Window for which to get size for
 * @return {Boolean} True if the size was successfully acquired
 */
/*TODO
bool AxPlatformUtils::GetWindowClientSize(windowHandle)
{
    return { width = windowHandle.width, height = windowHandle.height };
    HWND handle = windowHandle == 0 ? NULL : *(HWND*)windowHandle;

    RECT rect;
    bool result = GetClientRect(handle, &rect) != 0;

    width = rect.right;
    height = rect.bottom;

    return result;
}
*/

/**
 * Decodes a native string containing data encoded in base64 format and returns an ArrayBuffer containing the data itself
 * @param {String} base64String The native string containing the data, encoded in base64 format
 * @returns {ArrayBuffer} An array buffer containing the decoded data
 */
AxPlatformUtils.Base64ToArrayBuffer = function(base64String)
{
    var dataString = window.atob(base64String);
    var dataLength = dataString.length;
    var data = new Uint8Array(dataLength);
    for (var i = 0; i < dataLength; i++)
        data[i] = dataString.charCodeAt(i);

    return data.buffer;
};