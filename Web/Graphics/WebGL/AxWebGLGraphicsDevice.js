/**
 * Creates a new graphics device
 * @constructor
 */
function AxWebGLGraphicsDevice()
{
    this.gl = null;
    
    this.width = 0;
    this.height = 0;
    
    this.currentMesh = null;
    this.currentShader = null;
    
    this.currentRenderTarget = null;
    
    this.cullMode = AxRenderState.CullBackFace;
}



/**
 * Creates a new mesh for the respective device
 * @return {AxGraphicsDeviceMesh} New mesh for the respective device
 */
AxWebGLGraphicsDevice.prototype.CreateMesh = function()
{
    var result = new AxWebGLMesh();
    result.context = this;
    return result;
};

/**
 * Creates a new texture for the respective device
 * @return {AxDeviceTexture2D} New texture for the respective device
 */
AxWebGLGraphicsDevice.prototype.CreateTexture = function()
{
    var result = new AxWebGLTexture2D();
    result.context = this;
    return result;
};

/**
 * Creates a new shader for the respective device
 * @return {AxDeviceShader} New shader for the respective device
 */
AxWebGLGraphicsDevice.prototype.CreateShader = function()
{
    var result = new AxWebGLShader();
    result.context = this;
    return result;
};



/**
 * Sets a target window for presenting the rendering result
 * @param {Canvas} windowHandle Canvas to use for presenting the rendering result
 * @return {Bool} New mesh for the respective device
 */
AxWebGLGraphicsDevice.prototype.SetRenderTargetWindow = function(windowHandle)
{
    if (this.gl === null)
    {
        try 
        {
            this.gl = this.GetGLContext(windowHandle);

            this.gl.enable(this.gl.DEPTH_TEST);
            
            this.gl.enable(this.gl.CULL_FACE);
            this.gl.frontFace(this.gl.CW);
            this.gl.cullFace(this.gl.BACK);

            this.SetViewport(0, 0, windowHandle.width, windowHandle.height);
        }
        catch (e) 
        {
        }

        if (!this.gl) 
        {
            console.log("Could not initialize WebGL");
        }
    }
    else
    {
        this.gl.bindFramebuffer(this.gl.FRAMEBUFFER, null);
        
        this.currentRenderTarget = null;
    
        this.SetViewport(0, 0, this.width, this.height);
    }
};

/**
 * Sets a target texture to render onto
 * @param {AxDeviceTexture2D} texture Texture to use for rendering onto
 * @return {Bool} True if the render target was set successfully
 */
AxWebGLGraphicsDevice.prototype.SetRenderTargetTexture = function(texture)
{
    var glTexture = texture;
    
    if (this.currentRenderTarget === texture)
        return true;
    
    if (!glTexture.isFrameBuffer)
    {
        glTexture.Update(null, glTexture.width, glTexture.height, true);
    }
    
    this.gl.bindFramebuffer(this.gl.FRAMEBUFFER, glTexture.frameBuffer);
    
    this.currentRenderTarget = texture;

    this.gl.clear(this.gl.DEPTH_BUFFER_BIT);

    this.SetViewport(0, 0, glTexture.width, glTexture.height);
    
    return true;
};



/**
 * Gets the pixel format of the device
 * @return {AxPixelFormat} The pixel format of the device
 */
AxWebGLGraphicsDevice.prototype.GetPixelFormat = function()
{
    return new AxPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat.ChannelIdRed, AxPixelFormat.ChannelIdGreen, AxPixelFormat.ChannelIdBlue, AxPixelFormat.ChannelIdAlpha);
};



/**
 * Clears the frame buffer with a specified color
 * @param {AxVector4} color Color to cleare the frame buffer with
 */
AxWebGLGraphicsDevice.prototype.ClearScreen = function(color)
{
    this.gl.clearColor(color.x, color.y, color.z, color.w);
    this.gl.clear(this.gl.COLOR_BUFFER_BIT);
};

/**
 * Clears the depth buffer
 */
AxWebGLGraphicsDevice.prototype.ClearDepthBuffer = function() 
{
    this.gl.clear(this.gl.DEPTH_BUFFER_BIT);
};



/**
 * Begins rendering a new scene.
 * This method should be called before rendering a new frame
 */
AxWebGLGraphicsDevice.prototype.BeginScene = function() { };

/**
 * Ends rendering of a scene.
 * This method should be called after all rendering in a frame is done
 */
AxWebGLGraphicsDevice.prototype.EndScene = function() { };



/**
 * Sets the viewport of the frame buffer
 * @param {Integer} x Left boundary in pixels of the viewport
 * @param {Integer} y Top boundary in pixels of the viewport
 * @param {Integer} width Width in pixels of the viewport
 * @param {Integer} height Height in pixels of the viewport
 */
AxWebGLGraphicsDevice.prototype.SetViewport = function(x, y, width, height)
{
    this.gl.viewport(x, y, width, height);
    
    if (this.currentRenderTarget === null)
    {
        this.width = width;
        this.height = height;
    }
};



/**
 * Sets the mesh to render
 * @param {AxDeviceMesh} mesh The mesh to be rendered
 */
AxWebGLGraphicsDevice.prototype.SetMesh = function(mesh)
{
    this.currentMesh = mesh;
};

/**
 * Sets the rendering shader
 * @param {AxDeviceShader} shader The shader to be set
 */
AxWebGLGraphicsDevice.prototype.SetShader = function(shader)
{
    this.currentShader = shader;
    
    this.currentShader.setTextureIndex = 0;
    
    this.gl.useProgram(this.currentShader.shaderProgram);
    
    var viewProjectionMatrix = new AxMatrix();
    AxMatrix.Multiply(viewProjectionMatrix, this.currentShader.viewMatrix, this.currentShader.projectionMatrix);
    this.currentShader.SetParameter(this.currentShader.viewProjectionMatrixAddress, 0, viewProjectionMatrix);
    
    if (this.currentShader.isSkeletalAnimated)
        this.gl.uniformMatrix4fv(this.currentShader.worldMatrixAddress, false, this.currentShader.bones);

    if (this.currentShader.lightsPosIntensityAddress !== -1)
        this.gl.uniform4fv(this.currentShader.lightsPosIntensityAddress, this.currentShader.lightsPosIntensity);
    
    if (this.currentShader.lightsColorAddress !== -1)
        this.gl.uniform4fv(this.currentShader.lightsColorAddress, this.currentShader.lightsColor);
};

/**
 * Sets the render state
 * The render state represents a set of settings for the graphics device
 * @param {AxRenderState} renderState The rendering state to be set
 */
AxWebGLGraphicsDevice.prototype.SetRenderState = function(renderState) 
{
    switch (renderState)
    {
        case AxRenderState.CullBackFace:
        {
            if (this.cullMode !== renderState)
            {
                this.gl.enable(this.gl.CULL_FACE);
                this.gl.cullFace(this.gl.BACK);
                
                this.cullMode = renderState;
            }

            break;
        }

        case AxRenderState.CullFrontFace:
        {
            if (this.cullMode !== renderState)
            {
                this.gl.enable(this.gl.CULL_FACE);
                this.gl.cullFace(this.gl.FRONT);

                this.cullMode = renderState;
            }

            break;
        }

        case AxRenderState.CullNone:
        {
            if (this.cullMode !== renderState)
            {
                this.gl.disable(this.gl.CULL_FACE);

                this.cullMode = renderState;
            }

            break;
        }
    }
};



/**
 * Renders the mesh set to the device using the set shader and device settings
 */
AxWebGLGraphicsDevice.prototype.RenderMesh = function()
{
    if (this.currentShader.vertexPositionAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexPositionAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesPositionsBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexPositionAttribute, this.currentMesh.vertexPositionElementsCount, this.gl.FLOAT, false, 0, 0);
    }

    if (this.currentShader.vertexTexCoordsAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexTexCoordsAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesTexCoordsBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexTexCoordsAttribute, this.currentMesh.vertexTexCoordsElementsCount, this.gl.FLOAT, false, 0, 0);
    }

    if (this.currentShader.vertexNormalAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexNormalAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesNormalsBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexNormalAttribute, this.currentMesh.vertexNormalElementsCount, this.gl.FLOAT, false, 0, 0);
    }
    
    if (this.currentShader.vertexBoneIndicesAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexBoneIndicesAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesBoneIndicesBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexBoneIndicesAttribute, this.currentMesh.vertexBoneIndicesElementsCount, this.gl.FLOAT, false, 0, 0);
    }
    
    if (this.currentShader.vertexBoneWeightsAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexBoneWeightsAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesBoneWeightsBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexBoneWeightsAttribute, this.currentMesh.vertexBoneWeightsElementsCount, this.gl.FLOAT, false, 0, 0);
    }

    if (this.currentShader.vertexTangentsAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexTangentsAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesTangentBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexTangentsAttribute, this.currentMesh.vertexTangentElementsCount, this.gl.FLOAT, false, 0, 0);
    }
    
    if (this.currentShader.vertexBitangentsAttribute !== -1)
    {
        this.gl.enableVertexAttribArray(this.currentShader.vertexBitangentsAttribute);
        this.gl.bindBuffer(this.gl.ARRAY_BUFFER, this.currentMesh.verticesBitangentBuffer);
        this.gl.vertexAttribPointer(this.currentShader.vertexBitangentsAttribute, this.currentMesh.vertexBitangentElementsCount, this.gl.FLOAT, false, 0, 0);
    }
   

    
    this.gl.bindBuffer(this.gl.ELEMENT_ARRAY_BUFFER, this.currentMesh.indicesBuffer);

    this.gl.drawElements(this.gl.TRIANGLES, this.currentMesh.indicesCount, this.gl.UNSIGNED_SHORT, 0);
};



/**
 * Presents the frame buffer on the screen to the set rendering target window
 * @return {Boolean} True if presenting succeeded.
 */
AxWebGLGraphicsDevice.prototype.Present = function()
{
    
};


/**
 * Gets a GL context from the given canvas element
 * @param {Canvas} canvas The canvas element to get GL context from
 * @return {WebGLContext} If succeeded, a GL context is returned. If failed null is returned
 */
AxWebGLGraphicsDevice.prototype.GetGLContext = function(canvas) 
{
    var glContextCandidateNames = ["webgl", "experimental-webgl", "webkit-3d", "moz-webgl"];
    var context = null;
    
    for (var i = 0; (i < glContextCandidateNames.length) && (context === null); i++)
        context = canvas.getContext(glContextCandidateNames[i]);

    return context;
};