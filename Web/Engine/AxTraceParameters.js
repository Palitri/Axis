function AxTraceParameters(parent)
{
    if (AxUtils.IsUndefinedOrNull(parent))
        this.Clear();
    else
        this.FromParent(parent);
}

AxTraceParameters.prototype.Dispose = function()
{
};

AxTraceParameters.prototype.Clear = function()
{
	this.meshRef = null;
	this.cameraRef = null;
        this.parentTransformRef = null;
	this.transformRef = null;
	this.renderTargetRef = null;
	this.materialRef = null;
	this.mechanismRef = null;
	this.lightRef = null;
	this.soundEmitterRef = null;
	this.soundReceptorRef = null;
	this.textureRef = null;
        this.entitySetRef = null;

	this.meshBranch = null;
	this.cameraBranch = null;
	this.transformBranch = null;
	this.materialBranch = null;
	this.lightBranch = null;
	this.soundEmitterBranch = null;
	this.soundReceptorBranch = null;
        this.entitySetBranch = null;

	this.meshIndex = -1;
	this.cameraIndex = -1;
	this.transformIndex = -1;
	this.materialIndex = -1;
	this.lightIndex = -1;
	this.soundEmitterIndex = -1;
	this.soundReceptorIndex = -1;
        this.entitySetIndex = -1;

        this.traceIndex = -1;
};

AxTraceParameters.prototype.FromParent = function(parent)
{
	this.meshRef = parent.meshRef;
	this.cameraRef = parent.cameraRef;
        this.parentTransformRef = parent.transformRef;
	this.transformRef = parent.transformRef;
	this.renderTargetRef = parent.renderTargetRef;
	this.materialRef = parent.materialRef;
	this.mechanismRef = parent.mechanismRef;
	this.lightRef = parent.lightRef;
	this.soundEmitterRef = parent.soundEmitterRef;
	this.soundReceptorRef = parent.soundReceptorRef;
	this.textureRef = parent.textureRef;
        this.entitySetRef = parent.entitySetRef;

	this.meshBranch = parent.meshBranch;
	this.cameraBranch = parent.cameraBranch;
	this.transformBranch = parent.transformBranch;
	this.materialBranch = parent.materialBranch;
	this.lightBranch = parent.lightBranch;
	this.soundEmitterBranch = parent.soundEmitterBranch;
	this.soundReceptorBranch = parent.soundReceptorBranch;
        this.entitySetBranch = parent.entitySetBranch;

	this.meshIndex = parent.meshIndex;
	this.cameraIndex = parent.cameraIndex;
	this.transformIndex = parent.transformIndex;
	this.materialIndex = parent.materialIndex;
	this.lightIndex = parent.lightIndex;
	this.soundEmitterIndex = parent.soundEmitterIndex;
	this.soundReceptorIndex = parent.soundReceptorIndex;
        this.entitySetIndex = parent.entitySetIndex;

        this.traceIndex = parent.traceIndex;
};
