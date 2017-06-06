/**
 * Creates a new graphics device
 * @constructor
 */
function AxGraphicsDevice()
{
}



/**
 * Creates a new mesh for the respective device
 * @return {AxGraphicsDeviceMesh} New mesh for the respective device
 */
AxGraphicsDevice.prototype.CreateMesh = function() { };

/**
 * Creates a new texture for the respective device
 * @return {AxDeviceTexture2D} New texture for the respective device
 */
AxGraphicsDevice.prototype.CreateTexture = function() { };

/**
 * Creates a new shader for the respective device
 * @return {AxDeviceShader} New shader for the respective device
 */
AxGraphicsDevice.prototype.CreateShader = function() { };;



/**
 * Sets a target window for presenting the rendering result
 * @param {Canvas} windowHandle Canvas to use for presenting the rendering result
 * @return {Bool} New mesh for the respective device
 */
AxGraphicsDevice.prototype.SetRenderTargetWindow = function(windowHandle) { };

/**
 * Sets a target texture to render onto
 * @param {AxDeviceTexture2D} texture Texture to use for rendering onto
 * @return {Bool} New mesh for the respective device
 */
AxGraphicsDevice.prototype.SetRenderTargetTexture = function(texture) { };



/**
 * Gets the pixel format of the device
 * @return {AxPixelFormat} The pixel format of the device
 */
AxGraphicsDevice.prototype.GetPixelFormat = function() { };



/**
 * Clears the frame buffer with a specified color
 * @param {AxVector4} color Color to cleare the frame buffer with
 */
AxGraphicsDevice.prototype.ClearScreen = function(color) { };

/**
 * Clears the depth buffer
 */
AxGraphicsDevice.prototype.ClearDepthBuffer = function() { };



/**
 * Begins rendering a new scene.
 * This method should be called before rendering a new frame
 */
AxGraphicsDevice.prototype.BeginScene = function() { };

/**
 * Ends rendering of a scene.
 * This method should be called after all rendering in a frame is done
 */
AxGraphicsDevice.prototype.EndScene = function() { };



/**
 * Sets the viewport of the frame buffer
 * @param {Integer} x Left boundary in pixels of the viewport
 * @param {Integer} y Top boundary in pixels of the viewport
 * @param {Integer} width Width in pixels of the viewport
 * @param {Integer} height Height in pixels of the viewport
 */
AxGraphicsDevice.prototype.SetViewport = function(x, y, width, height) { };



/**
 * Sets the mesh to render
 * @param {AxDeviceMesh} mesh The mesh to be rendered
 */
AxGraphicsDevice.prototype.SetMesh = function(mesh) { };

/**
 * Sets the rendering shader
 * @param {AxDeviceShader} shader The shader to be set
 */
AxGraphicsDevice.prototype.SetShader = function(shader) { };

/**
 * Sets the render state
 * The render state represents a set of settings for the graphics device
 * @param {AxRenderState} renderState The rendering shader to be set
 */
AxGraphicsDevice.prototype.SetRenderState = function(renderState) { };



/**
 * Renders the mesh set to the device using the set shader and device settings
 */
AxGraphicsDevice.prototype.RenderMesh = function() { };



/**
 * Presents the frame buffer on the screen to the set rendering target window
 * @return {Boolean} True if presenting succeeded.
 */
AxGraphicsDevice.prototype.Present = function() { };