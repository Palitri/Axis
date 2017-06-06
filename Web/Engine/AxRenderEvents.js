/**
 * Creates an object with render events
 * Trace events occur while tracing the scene in axis in order to render it
 * @param {Axis} context The instance of axis, which calls the events
 * @constructor
 */
function AxRenderEvents(context) 
{
    AxTraceEvents.call(this, context);

    this.numRenderedObjects = 0;
    this.updateState = false;
}
	
AxRenderEvents.prototype = Object.create(AxTraceEvents.prototype);



AxRenderEvents.prototype.Dispose = function()
{
};



AxRenderEvents.prototype.SetupNewFrame = function()
{
    this.numRenderedObjects = 0;
    this.updateState = !this.updateState;
};

AxRenderEvents.prototype.OnMaterial = function(parameters)
{
    if (!parameters.materialRef.successfullyCompiled)
        return;

    this.context.graphicsDevice.SetShader(parameters.materialRef.deviceShader);

    parameters.materialRef.deviceShader.SetParameter(parameters.materialRef.paramAddressAmbientLight, 0, parameters.materialRef.context.settings.properties.Get(AxSettings.propertyIndex_AmbientLight).GetEffectiveValue());

    parameters.materialRef.ApplyProperties();
};
    
   
AxRenderEvents.prototype.OnMesh = function(parameters)
{
    if ((parameters.materialRef === null) || (parameters.cameraRef === null) || (parameters.transformRef === null))
        return;

    if (!parameters.materialRef.successfullyCompiled)
        return;

    var transform = new AxMatrix();
    AxMatrix.Multiply(transform, parameters.transformRef.pivotedWorldMatrix, parameters.cameraRef.viewProjection);
    if (parameters.meshRef.properties.Get(AxMesh.propertyIndex_Cull).GetBool())
        if (!parameters.meshRef.bounds.InsideViewFrustum(transform))
            return;

    parameters.materialRef.deviceShader.SetParameter(parameters.materialRef.paramAddressViewMatrix, 0, parameters.cameraRef.view);
    parameters.materialRef.deviceShader.SetParameter(parameters.materialRef.paramAddressProjectionMatrix, 0, parameters.cameraRef.projection);
    parameters.materialRef.deviceShader.SetParameter(parameters.materialRef.paramAddressViewPosition, 0, parameters.cameraRef.position);

    if (parameters.materialRef.isAnimated)
        parameters.materialRef.ApplyBones();
    else
        parameters.materialRef.deviceShader.SetParameter(parameters.materialRef.paramAddressWorldMatrix, 0, parameters.transformRef.pivotedWorldMatrix);

    var lightsReferenceLocation = new AxVector3();
    AxMatrix.GetTranslation(lightsReferenceLocation, parameters.transformRef.pivotedWorldMatrix);
    parameters.materialRef.ApplyLights(lightsReferenceLocation);


    this.context.graphicsDevice.SetShader(parameters.materialRef.deviceShader);

    this.context.graphicsDevice.SetMesh(parameters.meshRef.deviceMesh);

    this.context.graphicsDevice.RenderMesh();

    this.numRenderedObjects++;
};
    
AxRenderEvents.prototype.OnMechanism = function(parameters)
{
    if (parameters.mechanismRef._updateState !== this.updateState)
    {
        parameters.mechanismRef.Process(this.context.timer.time);
        parameters.mechanismRef._updateState = this.updateState;
    }
};

AxRenderEvents.prototype.OnTransform = function(parameters)
{
    if (parameters.transformRef._updateState !== this.updateState)
    {
        parameters.transformRef.Process(parameters.parentTransformRef);
        parameters.transformRef._updateState = this.updateState;
    }
};

AxRenderEvents.prototype.OnLight = function(parameters)
{
    if (AxUtils.IsUndefinedOrNull(parameters.transformRef))
        return;

    this.context.activeLightsCollecting.SetSize(this.context.activeLightsCollecting.count + 1);
    var lightInfo = this.context.activeLightsCollecting.Get(this.context.activeLightsCollecting.count - 1);

    if (AxUtils.IsUndefinedOrNull(lightInfo))
    {
        lightInfo = new AxActiveLightInfo();
        this.context.activeLightsCollecting.Set(this.context.activeLightsCollecting.count - 1, lightInfo);
    }

    lightInfo.lightRef = parameters.lightRef;
    lightInfo.lightTransform = parameters.transformRef.worldMatrix;
    AxMatrix.GetTranslation(lightInfo.lightPos, lightInfo.lightTransform);
};

AxRenderEvents.prototype.OnSoundEmitter = function(parameters)
{
    if (!AxUtils.IsUndefinedOrNull(parameters.transformRef))
        parameters.soundEmitterRef.ApplyWorldLocation(parameters.transformRef.worldMatrix);

    parameters.soundEmitterRef.ApplyProperties();
};

AxRenderEvents.prototype.OnSoundReceptor = function(parameters)
{
    if (!AxUtils.IsUndefinedOrNull(parameters.transformRef))
        parameters.soundReceptorRef.ApplyWorldLocation(parameters.transformRef.worldMatrix);
};

AxRenderEvents.prototype.OnTexture2D = function(parameters)
{
    parameters.textureRef.UpdateToProperties();

    this.context.graphicsDevice.SetRenderTargetTexture(parameters.textureRef.deviceTexture);

    this.context.currentRenderTarget = parameters.textureRef;

    if (this.context.clearOnRenderTarget)
        this.context.graphicsDevice.Clear(new AxVector4());
};

AxRenderEvents.prototype.OnExitEntitySet = function(parameters)
{
    if (this.context.currentRenderTarget !== parameters.textureRef)
    {
        if (parameters.textureRef === null)
            this.context.graphicsDevice.SetRenderTargetWindow(this.context.windowHandle);
        else
            this.context.graphicsDevice.SetRenderTargetTexture(parameters.textureRef.deviceTexture);

        this.context.currentRenderTarget = parameters.textureRef;
    }
};
