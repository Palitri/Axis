/**
 * Creates a new graphics device 2D texture
 * This constructor shouldn't be called explicity, but instead the graphics device should create its texture
 * @constructor
 */
function AxWebGLTexture2D()
{
    AxDeviceTexture2D.call(this);
    
    this.context;
    
    this.texture = null;
    this.depthBuffer = null;
    this.frameBuffer = null;
    
    this.width = 0;
    this.height = 0;
    
    this.isFrameBuffer = false;
}

AxWebGLTexture2D.prototype = Object.create(AxDeviceTexture2D.prototype);



AxWebGLTexture2D.prototype.Dispose = function()
{
    this.texture = null;
    this.depthBuffer = null;
    this.frameBuffer = null;
    
    this.width = 0;
    this.height = 0;
    this.isFrameBuffer = false;
    return;
    
    if (this.texture !== null)
    {
        this.context.gl.deleteTextures(1, this.texture);
        this.texture = null;
    }
    
    if (this.depthBuffer !== null)
    {
        this.context.gl.deleteRenderBuffers(1, this.depthBuffer);
        this.depthBuffer = null;
    }
    
    if (this.frameBuffer !== null)
    {
        this.context.gl.deleteFrameBuffers(1, this.frameBuffer);
        this.frameBuffer = null;
    }
    
    this.width = 0;
    this.height = 0;

    this.isFrameBuffer = false;
};

/**
 * Updates the texture with the given data
 * @param {Uint8Array} data The pixel data to update the texture with
 * @param {Integer} width The width in pixels of the texture data
 * @param {Integer} height The height in pixels of the texture data
 * @param {Boolean} renderTarget Denotes whether the texture is to be used as a render target
 */
AxWebGLTexture2D.prototype.Update = function(data, width, height, renderTarget)
{
    if ((this.width !== width) || (this.height !== height) || (this.isFrameBuffer !== renderTarget))
    {
        this.Dispose();
        
        if (renderTarget)
        {
            this.texture = this.context.gl.createTexture();
            this.context.gl.bindTexture(this.context.gl.TEXTURE_2D, this.texture);
            
            if (AxUtils.IsInstanceOf(data, Image))
            {
                this.context.gl.pixelStorei(this.context.gl.UNPACK_FLIP_Y_WEBGL, true);
                this.context.gl.texImage2D(this.context.gl.TEXTURE_2D, 0, this.context.gl.RGBA, this.context.gl.RGBA, this.context.gl.UNSIGNED_BYTE, data);
            }
            else
                this.context.gl.texImage2D(this.context.gl.TEXTURE_2D, 0, this.context.gl.RGBA, width, height, 0, this.context.gl.RGBA, this.context.gl.UNSIGNED_BYTE, data);
            
            this.context.gl.texParameteri(this.context.gl.TEXTURE_2D, this.context.gl.TEXTURE_MAG_FILTER, this.context.gl.LINEAR);
            this.context.gl.texParameteri(this.context.gl.TEXTURE_2D, this.context.gl.TEXTURE_MIN_FILTER, this.context.gl.LINEAR);

            this.depthBuffer = this.context.gl.createRenderbuffer();
            this.context.gl.bindRenderbuffer(this.context.gl.RENDERBUFFER, this.depthBuffer);
            this.context.gl.renderbufferStorage(this.context.gl.RENDERBUFFER, this.context.gl.DEPTH_COMPONENT16, width, height);

            this.frameBuffer = this.context.gl.createFramebuffer();
            this.context.gl.bindFramebuffer(this.context.gl.FRAMEBUFFER, this.frameBuffer);
            this.context.gl.framebufferTexture2D(this.context.gl.FRAMEBUFFER, this.context.gl.COLOR_ATTACHMENT0, this.context.gl.TEXTURE_2D, this.texture, 0);
            this.context.gl.framebufferRenderbuffer(this.context.gl.FRAMEBUFFER, this.context.gl.DEPTH_ATTACHMENT, this.context.gl.RENDERBUFFER, this.depthBuffer);
        }
        else
        {
            this.texture = this.context.gl.createTexture();
            this.context.gl.bindTexture(this.context.gl.TEXTURE_2D, this.texture);
            
            if (AxUtils.IsInstanceOf(data, Image))
            {
                this.context.gl.pixelStorei(this.context.gl.UNPACK_FLIP_Y_WEBGL, true);
                this.context.gl.texImage2D(this.context.gl.TEXTURE_2D, 0, this.context.gl.RGBA, this.context.gl.RGBA, this.context.gl.UNSIGNED_BYTE, data);
            }
            else
                this.context.gl.texImage2D(this.context.gl.TEXTURE_2D, 0, this.context.gl.RGBA, width, height, 0, this.context.gl.RGBA, this.context.gl.UNSIGNED_BYTE, data);
            
            this.context.gl.texParameteri(this.context.gl.TEXTURE_2D, this.context.gl.TEXTURE_MAG_FILTER, this.context.gl.LINEAR);
            this.context.gl.texParameteri(this.context.gl.TEXTURE_2D, this.context.gl.TEXTURE_MIN_FILTER, this.context.gl.LINEAR_MIPMAP_NEAREST);
            this.context.gl.generateMipmap(this.context.gl.TEXTURE_2D);
        }
        
//       this.context.gl.texParameteri(this.context.gl.TEXTURE_2D, this.context.gl.TEXTURE_WRAP_S, this.context.gl.CLAMP_TO_EDGE);
//       this.context.gl.texParameteri(this.context.gl.TEXTURE_2D, this.context.gl.TEXTURE_WRAP_T, this.context.gl.CLAMP_TO_EDGE);
   
    }
    
    this.width = width;
    this.height = height;
    this.isFrameBuffer = renderTarget;
};

/**
 * Gets the pixel data of a texture
 * @param {Uint8Array} data The data buffer into which to store the texture's pixel data
 */
AxWebGLTexture2D.prototype.GetData = function(data)
{
    if (this.frameBuffer === null)
    {
        this.frameBuffer = this.context.gl.createFramebuffer();
        this.context.gl.bindFramebuffer(this.context.gl.FRAMEBUFFER, this.frameBuffer);
        this.context.gl.framebufferTexture2D(this.context.gl.FRAMEBUFFER, this.context.gl.COLOR_ATTACHMENT0, this.context.gl.TEXTURE_2D, this.texture, 0);
    }
    else
        this.context.gl.bindFramebuffer(this.context.gl.FRAMEBUFFER, this.frameBuffer);

    if (this.context.gl.checkFramebufferStatus(this.context.gl.FRAMEBUFFER) === this.context.gl.FRAMEBUFFER_COMPLETE) 
    {
        this.context.gl.readPixels(0, 0, this.width, this.height, this.context.gl.RGBA, this.context.gl.UNSIGNED_BYTE, data);
    }
};