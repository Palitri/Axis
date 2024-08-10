function AxViewport(canvas, src)
{
    this.canvas = canvas;
    
    this.animation = null;
    this.axis = null;
    
    this.src = null;
    this.onInitialize = null;
    this.onLoad = null;
    this.onRender = null;
    this.orbitEnabled = false;
    this.rotateEnabled = false;
    this.panEnabled = false;
    this.zoomEnabled = false;
    
    this.setDefaultControls();
    this.controlsSet = false;
    
    QueryDocumentLoadedCallback(this, function(instance)
    {
        instance.Initialize(src);
    });    
}

AxViewport.prototype.Initialize = function(src)
{
    this.animation = new AxAnimationElement(this.canvas);
    this.axis = new Axis(this.animation.canvas);

    var instance = this;
    this.animation.OnRender = function() 
    { 
        instance.axis.RenderScene();

        if (!instance.controlsSet)
            instance.controlsSet = instance.ApplyControls();

        if (instance.onRender !== null)
            instance.onRender();
    };

    if (instance.onInitialize !== null)
        instance.onInitialize();
    
    if (!AxUtils.IsUndefinedOrNull(src))
        instance.setSrc(src);
};


AxViewport.prototype.ApplyControls = function()
{
    if ((this.axis === null) || (this.axis.currentCamera === null))
        return false;
    
    this.axis.inputModels.Clear();

    var camera = this.axis.currentCamera;
    var translationFactor = AxMaths.SizeFromDistanceAndAngle(camera.transformRef.GetPositionExtrinsic().GetLength(), camera.cameraRef.properties.Get(AxCamera.propertyIndex_FOV).GetFloat());

    if (this.orbitEnabled)
    {
        var pivot = new AxTransform();
        this.axis.inputModels.Add(new AxOrbitInputModel(this.axis, pivot, camera.transformRef, 1.0, 6.0,
            "", "Pointer Y", "Pointer X", "", "", "", "Pointer Pressed"));
    }
    else if (this.rotateEnabled)
    {
        this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor, 6.0, 1.0, 6.0,
            "", "", "", "", "", "", "", "Pointer Y", "", "", "Pointer X", "Pointer Pressed"));
    }

    if (this.zoomEnabled)
    {
        this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor / 4.0, 6.0, 1.0, 6.0,
            "Mouse Scroll", "", "", "", "", "", "", "", "", "", "", ""));

        this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor * 4.0, 6.0, 1.0, 6.0,
            "Mouse Y", "", "", "", "", "", "Mouse Middle", "", "", "", "", ""));
    }

    if (this.panEnabled)
    {
        this.axis.inputModels.Add(new AxWalkInputModel(this.axis, camera.transformRef, translationFactor, 6.0, 1.0, 6.0,
            "", "", "Mouse X", "", "", "Mouse Y", "Mouse Right", "", "", "", "", ""));
    }
    
    return true;
};


AxViewport.prototype.getSrc = function() {
    return this.src;
};

AxViewport.prototype.setSrc = function(value) 
{
    if (this.axis === null)
        return;

    this.src = value;
    
    var srcAttribute = this.src;
    try
    {
        srcAttribute = eval(srcAttribute);
    }
    catch(e)
    {
    }
    
    if (srcAttribute)
    {
        var instance = this;
        this.axis.Reset();
        this.axis.ImportScene(srcAttribute, function(context, fileName, success) 
        { 
            if (instance.onLoad!== null)
                instance.onLoad();

            instance.controlsSet = false;
            instance.animation.StartRendering();
        });
    }
};


AxViewport.prototype.getTouchScrollingEnabled = function() {
    return this.animation.touchScrollingEnabled;
};

AxViewport.prototype.setTouchScrollingEnabled = function(value) {
    this.animation.touchScrollingEnabled = value;
};



AxViewport.prototype.getContextMenuEnabled = function() {
    return this.animation.contextMenuEnabled;
};

AxViewport.prototype.setContextMenuEnabled = function(value) {
    this.animation.contextMenuEnabled = value;
};


AxViewport.prototype.getOnInitialize = function() {
    return this.onInitialize;
};

AxViewport.prototype.setOnInitialize = function(value) {
    this.onInitialize = value;
};


AxViewport.prototype.getOnLoad = function() {
    return this.onLoad;
};

AxViewport.prototype.setOnLoad = function(value) {
    this.onLoad = value;
};


AxViewport.prototype.getOnRender = function() {
    return this.onRender;
};

AxViewport.prototype.setOnRender = function(value) {
    this.onRender = value;
};


AxViewport.prototype.setDefaultControls = function() {
    this.rotateEnabled = false;
    this.orbitEnabled = true;
    this.panEnabled = true;
    this.zoomEnabled = true;
    
    this.ApplyControls();
};


AxViewport.prototype.getOrbitEnabled = function() {
    return this.orbitEnabled;
};

AxViewport.prototype.setOrbitEnabled = function(value) {
    this.orbitEnabled = value;

    this.ApplyControls();
};


AxViewport.prototype.getRotateEnabled = function() {
    return this.rotateEnabled;
};

AxViewport.prototype.setRotateEnabled = function(value) {
    this.rotateEnabled = value;

    this.ApplyControls();
};


AxViewport.prototype.getPanEnabled = function() {
    return this.panEnabled;
};

AxViewport.prototype.setPanEnabled = function(value) {
    this.panEnabled = value;

    this.ApplyControls();
};


AxViewport.prototype.getZoomEnabled = function() {
    return this.zoomEnabled;
};

AxViewport.prototype.setZoomEnabled = function(value) {
    this.zoomEnabled = value;

    this.ApplyControls();
};


