/**
 * Creates a new graphics device shader
 * This constructor shouldn't be called explicity, but instead the graphics device should create its shader
 * @constructor
 */
function AxDeviceShader()
{
}


AxDeviceShader.invalidParameterAddress = null;



/**
 * Gets the number of operations the shader has
 * @return {Integer} The number of operations the shader has
 */
AxDeviceShader.prototype.GetOperationCount = function() { };

/**
 * Adds an operation to the shader
 * @param {AxShadingOperation} shadingOp Type of shader to add
 * @param {AxColorOperation} colorOp Type of color operation
 */
AxDeviceShader.prototype.AddOperation = function(shadingOp, colorOp) { };

/**
 * Gets an existing operation's parameters
 * @param {Integer} index Index of an existing operation to be set
 * @param {AxShadingOperation} shadingOp Returned type of shading operation. Returns as wrapped in an object
 * @param {AxColorOperation} colorOp Returned type of color operation. Returns as wrapped in an object
 */
AxDeviceShader.prototype.GetOperation = function(index, shadingOp, colorOp) { };

/**
 * Sets an existing operation's parameters
 * @param {Integer} index Index of an existing operation to be set
 * @param {AxShadingOperation} shadingOp Type of shader to add
 * @param {AxColorOperation} colorOp Type of color operation
 */
AxDeviceShader.prototype.SetOperation = function(index, shadingOp, colorOp) { };

/**
 * Clears all operation from the shader
 */
AxDeviceShader.prototype.ClearOperations = function() { };



/**
 * Gets the address of a paramter of the shader
 * @param {AxString} parameterName Name of the parameter to get address for
 * @return {Integer} Address of the parameter with the given name
 */
AxDeviceShader.prototype.GetParameterAddress = function(parameterName) { };
/**
 * Sets a value to a parameter at the given address at a given index
 * @param {Integer} address Address of the parameter whose value is to be set
 * @param {Integer} index For indexed parameters, the index to set. For regular paramters should be 0
 * @param {Object} value Value to set to the parameter at the given address and index
 */
AxDeviceShader.prototype.SetParameter = function(address, index, value) { };



/**
 * Updates the shader according to its shading operations
 * @param {AxString} compilationMessage Returned message from the update
 * @return {Boolean} True if the update was successful. If the update was unsuccessful, returns true and an error should generally be returned in compliationMessage
 */
AxDeviceShader.prototype.Update = function(compilationMessage) { };