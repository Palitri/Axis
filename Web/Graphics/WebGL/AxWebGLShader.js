function AxWebGLMaterialShadingOperation()
{
    this.shadingOp = AxShadingOperation.None;
    this.colorOp = AxColorOperation.None;
}

/**
 * Creates a new graphics device shader
 * This constructor shouldn't be called explicity, but instead the graphics device should create its shader
 * @constructor
 */
function AxWebGLShader()
{
    AxDeviceShader.call(this);
    
    this.context = null;
    
    this.shaderProgram = null;
    
    this.glVertexShader = null;
    this.glPixelShader = null;
    
    this.compilationMessage = null;
    
    
    this.vertexPositionAttribute = -1;
    this.vertexTexCoordsAttribute = -1;
    this.vertexNormalAttribute = -1;
    this.vertexBoneIndicesAttribute = -1;
    this.vertexBoneWeightsAttribute = -1;
    this.vertexTangentsAttribute = -1;
    this.vertexBitangentsAttribute = -1;
    
    this.viewMatrix = new AxMatrix();
    this.projectionMatrix = new AxMatrix();
    
    this.shadingOps = new AxList();
    
    this.worldMatrixAddress = -1;
    this.viewProjectionMatrixAddress = -1;
    this.lightsPosIntensityAddress = -1;
    this.lightsColorAddress = -1;
    
    this.setTextureIndex = 0;
    
    this.bones = new Float32Array(16 * AxWebGLShader.maxBones);
    this.isSkeletalAnimated = false;
    
    this.lightsPosIntensity = new Float32Array(4 * AxWebGLShader.maxLights);
    this.lightsColor = new Float32Array(4 * AxWebGLShader.maxLights);
    
    // Used for general purposes, for conversion between AxMatrix and GL's matrix format
    this.glMatrix = new Float32Array(16);
    
}

AxWebGLShader.prototype = Object.create(AxDeviceShader.prototype);

AxWebGLShader.maxBones = 48;
AxWebGLShader.maxLights = 4;


// There are no actual view and projection matrices in the shader, but a viewProjectionMatrix, for optimization
AxWebGLShader.viewMatrixAddress = -2;
AxWebGLShader.projectionMatrixAddress = -3;

// There actually is a world matrix, but WebGL uniform location objects can't be compared correctly, so in case there is skeletal animation, this fake value is used in supplemental to the real one, for comparison purposes
AxWebGLShader.worldMatrixFakeAddress = -4;

// Because these are array uniforms, they need special management. See AxWebGLShader.worldMatrixFakeAddress for clarification
// Actually, there is a lightsPosIntensity array vector in the shader, each element a 4-vector with xyz as position and w as intensity
AxWebGLShader.lightsPosFakeAddress = -5;
AxWebGLShader.lightsIntensityFakeAddress = -6;
AxWebGLShader.lightsColorFakeAddress = -7;

/**
 * Gets the number of operations the shader has
 * @return {Integer} The number of operations the shader has
 */
AxWebGLShader.prototype.GetOperationCount = function()
{
    return this.shadingOps.count;
};

/**
 * Adds an operation to the shader
 * @param {AxShadingOperation} shadingOp Type of shader to add
 * @param {AxColorOperation} colorOp Type of color operation
 */
AxWebGLShader.prototype.AddOperation = function(shadingOp, colorOp)
{
    var shadingOperation = new AxWebGLMaterialShadingOperation();
    shadingOperation.shadingOp = shadingOp;
    shadingOperation.colorOp = colorOp;

    this.shadingOps.Add(shadingOperation);    
};

/**
 * Gets an existing operation's parameters
 * @param {Integer} index Index of an existing operation to be set
 * @param {AxShadingOperation} shadingOp Returned type of shading operation. Returns as wrapped in an object
 * @param {AxColorOperation} colorOp Returned type of color operation. Returns as wrapped in an object
 */
AxWebGLShader.prototype.GetOperation = function(index, shadingOp, colorOp)
{
    shadingOp = new AxReferenceParameter(this.shadingOps.Get(index).shadingOp);
    colorOp = new AxReferenceParameter(this.shadingOps.Get(index).colorOp);
};

/**
 * Sets an existing operation's parameters
 * @param {Integer} index Index of an existing operation to be set
 * @param {AxShadingOperation} shadingOp Type of shader to add
 * @param {AxColorOperation} colorOp Type of color operation
 */
AxWebGLShader.prototype.SetOperation = function(index, shadingOp, colorOp)
{
    this.shadingOps.Get(index).shadingOp = shadingOp;
    this.shadingOps.Get(index).colorOp = colorOp;
};

/**
 * Clears all operation from the shader
 */
AxWebGLShader.prototype.ClearOperations = function()
{
    this.shadingOps.Clear();
};



/**
 * Gets the address of a paramter of the shader
 * @param {AxString} parameterName Name of the parameter to get address for
 * @return {Integer} Address of the parameter with the given name
 */
AxWebGLShader.prototype.GetParameterAddress = function(parameterName)
{
    if (parameterName.Equals('View matrix'))
        return AxWebGLShader.viewMatrixAddress;
    if (parameterName.Equals('Projection matrix'))
        return AxWebGLShader.projectionMatrixAddress;
    if ((this.isSkeletalAnimated) && (parameterName.Equals('World matrix')))
        return AxWebGLShader.worldMatrixFakeAddress;
    if (parameterName.Equals('Lights pos'))
        return AxWebGLShader.lightsPosFakeAddress;
    if (parameterName.Equals('Lights intensity'))
        return AxWebGLShader.lightsIntensityFakeAddress;
    if (parameterName.Equals('Lights color'))
        return AxWebGLShader.lightsColorFakeAddress;

    var uniformName = AxDynamicShaderWriter.PropertyNameToParameterName(parameterName);
    var uniformLocation = this.context.gl.getUniformLocation(this.shaderProgram, uniformName.GetContents());
    
    if (AxUtils.IsUndefinedOrNull(uniformLocation))
        return AxDeviceShader.invalidParameterAddress;
    
    return uniformLocation;
};

/**
 * Sets a fake parameter and returns whether succeeded
 * Fake parameters are such paramters that do not actually exist in the shader program and/or need some special treatment instead of just being set straight to the program
 * The reason of having fake parameters is usually for when a the specific graphics device can't directly match Axis' architecture. Also when there's a need to intercept when a parameter is passed
 * @param {Integer} address Address of the parameter whose value is to be set
 * @param {Integer} index For indexed parameters, the index to set. For regular paramters should be 0
 * @param {Object} value Value to set to the parameter at the given address and index
 * @returns {Boolean} True if the parameter was successfully set.
 */
AxWebGLShader.prototype.SetFakeParameter = function(address, index, value)
{
    if (!(address < -1))
        return false;
    
    if (address === AxWebGLShader.viewMatrixAddress)
    {
        AxMatrix.Copy(this.viewMatrix, value);
        return true;
    }
    
    if (address === AxWebGLShader.projectionMatrixAddress)
    {
        AxMatrix.Copy(this.projectionMatrix, value);
        return true;
    }
    
    if ((address === AxWebGLShader.worldMatrixFakeAddress) && (this.isSkeletalAnimated))
    {
        // Apply this.bones at graphics device's SetShader
        if (index < AxWebGLShader.maxBones)
            AxWebGLShader.AxMatrixToGLUniform(value, this.bones, index * 16);
        return true;
    }
    
    if (address === AxWebGLShader.lightsPosFakeAddress)
    {
        if (index < AxWebGLShader.maxLights)
            AxWebGLShader.AxVector3ToGLUniform(value, this.lightsPosIntensity, index * 4);
        return true;
    }
    
    if (address === AxWebGLShader.lightsIntensityFakeAddress)
    {
        if (index < AxWebGLShader.maxLights)
            this.lightsPosIntensity[index * 4 + 3] = value;
        return true;
    }
    
    if (address === AxWebGLShader.lightsColorFakeAddress)
    {
        if (index < AxWebGLShader.maxLights)
            AxWebGLShader.AxVector4ToGLUniform(value, this.lightsColor, index * 4);
        return true;
    }
    
    return false;
};


/**
 * Sets a value to a parameter at the given address at a given index
 * @param {Integer} address Address of the parameter whose value is to be set
 * @param {Integer} index For indexed parameters, the index to set. For regular paramters should be 0
 * @param {Object} value Value to set to the parameter at the given address and index
 */
AxWebGLShader.prototype.SetParameter = function(address, index, value)
{
    try
    {
    if (AxUtils.IsUndefinedOrNull(address))
        return;
    
    if (this.SetFakeParameter(address, index, value))
        return;
    
    // Number
    if (AxUtils.IsNumber(value))
        this.context.gl.uniform1f(address, value);

    // Vector4
    else if (!AxUtils.IsUndefinedOrNull(value.w))
        this.context.gl.uniform4f(address, value.x, value.y, value.z, value.w);

    // Vector3
    else if (!AxUtils.IsUndefinedOrNull(value.z))
        this.context.gl.uniform3f(address, value.x, value.y, value.z);

    // Matrix
    else if (!AxUtils.IsUndefinedOrNull(value._11))
    {
        AxWebGLShader.AxMatrixToGLUniform(value, this.glMatrix, 0);
        this.context.gl.uniformMatrix4fv(address, false, this.glMatrix);
    }
    
    // Texture
    else if (!AxUtils.IsUndefinedOrNull(value.texture))
    {
        this.context.gl.uniform1i(address, this.setTextureIndex);
        this.context.gl.activeTexture(this.context.gl.TEXTURE0 + this.setTextureIndex);
        this.context.gl.bindTexture(this.context.gl.TEXTURE_2D, value.texture);

        this.setTextureIndex++;
    }
}
catch (e)
{
    console.log(e);
}
};

/**
 * Updates the shader according to its shading operations
 * @param {AxString} compilationMessage Returned message from the update
 * @return {Boolean} True if the update was successful. If the update was unsuccessful, returns true and an error should generally be returned in compliationMessage
 */
AxWebGLShader.prototype.Update = function(compilationMessage)
{
    this.Release();
    
    this.isSkeletalAnimated = this.IndexOf(AxShadingOperation.Skinning) !== -1;

    var shaderWriter = new AxDynamicShaderWriter(this.isSkeletalAnimated, AxWebGLShader.maxBones);

    for (var i = 0; i < this.shadingOps.count; i++)
        shaderWriter.AddShadingLayer(this.shadingOps.Get(i).shadingOp, this.shadingOps.Get(i).colorOp);

    var vertexShader = shaderWriter.GetVSSourceCode();
    var pixelShader = shaderWriter.GetPSSourceCode();
    
    if (!this.CompileShader(vertexShader, pixelShader))
    {
        console.log(this.compilationMessage);
        return false;
    }
    
    return true;
};

/**
 * Private
 * Compiles a vertex shader and a pixel shader
 * @param {String} vertexShaderCode Source code of the vertex shader
 * @param {String} pixelShaderCode Source code of the pixel shader
 * @return {Boolean} True if compilation was successful. If unsuccessful, returns false and stores the error in compilationMessage
 */
AxWebGLShader.prototype.CompileShader = function(vertexShaderCode, pixelShaderCode)
{
    if (AxUtils.IsUndefinedOrNull(this.glVertexShader))
        this.glVertexShader = this.context.gl.createShader(this.context.gl.VERTEX_SHADER);
    this.context.gl.shaderSource(this.glVertexShader, vertexShaderCode);
    this.context.gl.compileShader(this.glVertexShader);
    if (!this.context.gl.getShaderParameter(this.glVertexShader, this.context.gl.COMPILE_STATUS))
    {
        this.compilationMessage = 'Vertex shader error:\r\n' + this.context.gl.getShaderInfoLog(this.glVertexShader);
        console.log(this.compilationMessage);
        return false;
    }
    
    
    
    if (AxUtils.IsUndefinedOrNull(this.glPixelShader))
        this.glPixelShader = this.context.gl.createShader(this.context.gl.FRAGMENT_SHADER);
    this.context.gl.shaderSource(this.glPixelShader, pixelShaderCode);
    this.context.gl.compileShader(this.glPixelShader);
    if (!this.context.gl.getShaderParameter(this.glPixelShader, this.context.gl.COMPILE_STATUS))
    {
        this.compilationMessage = 'Pixel shader error:\r\n' + this.context.gl.getShaderInfoLog(this.glPixelShader);
        console.log(this.compilationMessage);
        return false;
    }
    
    
    
    if (AxUtils.IsUndefinedOrNull(this.shaderProgram))
        this.shaderProgram = this.context.gl.createProgram();
    this.context.gl.attachShader(this.shaderProgram, this.glVertexShader);
    this.context.gl.attachShader(this.shaderProgram, this.glPixelShader);
    this.context.gl.linkProgram(this.shaderProgram);
    if (!this.context.gl.getProgramParameter(this.shaderProgram, this.context.gl.LINK_STATUS)) 
    {
        this.compilationMessage = 'Linking shader error:\r\n' + this.context.gl.getProgramInfoLog(this.shaderProgram);
        console.log(this.compilationMessage);
        return false;
    }
    
    
    
    this.vertexPositionAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_pos');
    this.vertexTexCoordsAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_texCoords');
    this.vertexNormalAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_normal');
    this.vertexBoneIndicesAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_boneIndices');
    this.vertexBoneWeightsAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_boneWeights');
    this.vertexTangentsAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_tangent');
    this.vertexBitangentsAttribute = this.context.gl.getAttribLocation(this.shaderProgram, 'input_biTangent');
    
    // The viewProjectionMatrix parameter is actually real, but needs special treatment, because Axis sets view and projection separately - for optimization
    this.viewProjectionMatrixAddress = this.GetParameterAddress(new AxString('View projection matrix'));
    // The following parameters are used internally and GetParameterAddress returns fake addresses for them
    this.worldMatrixAddress = this.context.gl.getUniformLocation(this.shaderProgram, 'worldMatrix');
    this.lightsPosIntensityAddress = this.context.gl.getUniformLocation(this.shaderProgram, 'lightsPosIntensity');
    this.lightsColorAddress = this.context.gl.getUniformLocation(this.shaderProgram, 'lightsColor');

    return true;
};
    
/**
 * Release any resources allocated by the device
 */
AxWebGLShader.prototype.Release = function()
{
};


/**
 * Private
 * Gets the first index of occurence of the given shading operation
 * @param {AxShadingOperation} shadingOp The operation for which to search
 * @return {Integer} The index at which the given operation was found in the shading operations collection. If not found, returns -1
 */
AxWebGLShader.prototype.IndexOf = function(shadingOp)
{
    for (var i = 0; i < this.shadingOps.count; i++)
        if (this.shadingOps.Get(i).shadingOp === shadingOp)
            return i;

    return -1;
};


// Static methods



/**
 * Private
 * Translates data from a AxMatrix to a GLUniform
 * @param {AxMatrix} axMatrix The input matrix whose data is to be translated to the GL uniform
 * @param {Float32Array} glUniform The GL uniform, where the matrix' data is to be translated
 * @param {Integer} offset The offset in matrix elements, at which the translation to the GL uniform should start
 */
AxWebGLShader.AxMatrixToGLUniform = function(axMatrix, glUniform, offset)
{
    glUniform[offset + 0] = axMatrix._11;
    glUniform[offset + 1] = axMatrix._12;
    glUniform[offset + 2] = axMatrix._13;
    glUniform[offset + 3] = axMatrix._14;
    glUniform[offset + 4] = axMatrix._21;
    glUniform[offset + 5] = axMatrix._22;
    glUniform[offset + 6] = axMatrix._23;
    glUniform[offset + 7] = axMatrix._24;
    glUniform[offset + 8] = axMatrix._31;
    glUniform[offset + 9] = axMatrix._32;
    glUniform[offset + 10] = axMatrix._33;
    glUniform[offset + 11] = axMatrix._34;
    glUniform[offset + 12] = axMatrix._41;
    glUniform[offset + 13] = axMatrix._42;
    glUniform[offset + 14] = axMatrix._43;
    glUniform[offset + 15] = axMatrix._44;    
};

/**
 * Private
 * Translates data from a AxVector3 to a GLUniform
 * @param {AxVector3} axVector3 The input vector whose data is to be translated to the GL uniform
 * @param {Float32Array} glUniform The GL uniform, where the vector's data is to be translated
 * @param {Integer} offset The offset in vector elements, at which the translation to the GL uniform should start
 */
AxWebGLShader.AxVector3ToGLUniform = function(axVector3, glUniform, offset)
{
    glUniform[offset + 0] = axVector3.x;
    glUniform[offset + 1] = axVector3.y;
    glUniform[offset + 2] = axVector3.z;
};

/**
 * Private
 * Translates data from a AxVector4 to a GLUniform
 * @param {AxVector4} axVector4 The input vector whose data is to be translated to the GL uniform
 * @param {Float32Array} glUniform The GL uniform, where the vector's data is to be translated
 * @param {Integer} offset The offset in vector elements, at which the translation to the GL uniform should start
 */
AxWebGLShader.AxVector4ToGLUniform = function(axVector4, glUniform, offset)
{
    glUniform[offset + 0] = axVector4.x;
    glUniform[offset + 1] = axVector4.y;
    glUniform[offset + 2] = axVector4.z;
    glUniform[offset + 3] = axVector4.w;
};