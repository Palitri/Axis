/**
 * Gets fired when an AxAnimationElement is ready to render a frame
 * @callback AxAnimationElementRender
 */


/**
 * Utilizes the canvas element which will be used for rendering
 * Provides useful functionality for continuous rendering, mouse and keyboard input
 * @param {*} canvasId The id of the HTML canvas element
 * @param {AxAnimationElementRender} onRenderEvent The method which to perform rendering of a frame
 * @param {Boolean} startRendering Denotes whether to start rendering immediately
 */
function AxAnimationElement(canvasId, onRenderEvent, startRendering)
{
    this.renderingEnabled = false;
    
    this.OnRender = null;
    
    this.RequestAniamtionFrame;
    
    this.canvas = null;
    
    this.buttons = new Array(256);
    for (var i = 0; i < this.buttons.length; i++)
        this.buttons[i] = false;
    
    this.mouseButtons = 0;
    this.mouse = { x: 0, y: 0, lastX: 0, lastY: 0, deltaX: 0, deltaY: 0, left: false, right: false, middle: false, back: false, forward: false }; 
    
    this.onMouseMove = function(x, y) { };
    this.onMouseDown = function(x, y, button) { };
    this.onMouseUp = function(x, y, button) { };
    this.onKeyDown = function(key) { };
    this.onKeyUp = function(key) { };
    
    this.nodeType_Element = 1;
    this.nodeType_Attr = 2;
    this.nodeType_Text = 3;
    this.nodeType_CDATASection = 4;
    this.nodeType_EntityReference = 5;
    this.nodeType_Entity = 6;
    this.nodeType_ProcessingInstruction = 7;
    this.nodeType_Comment = 8;
    this.nodeType_Document = 9;
    this.nodeType_DocumentType = 10;
    this.nodeType_DocumentFragment = 11;
    this.nodeType_Notation = 12;
    
    if (!AxUtils.IsUndefinedOrNull(canvasId))
        this.Initialize(canvasId);

    if (!AxUtils.IsUndefinedOrNull(onRenderEvent))
        this.OnRender = onRenderEvent;
    
    if (!AxUtils.IsUndefinedOrNull(startRendering))
        if (startRendering)
            this.StartRendering();
}

AxAnimationElement.MouseButtonLeft      = 1;
AxAnimationElement.MouseButtonRight     = 2;
AxAnimationElement.MouseButtonMiddle    = 4;
AxAnimationElement.MouseButtonBack      = 8;
AxAnimationElement.MouseButtonForward   = 16;

/**
 * Provides managment for the element which will be used for rendering
 * requestAnimationFrame in a cross browser way.
 * @param {*} canvasId The id of the HTML canvas element
 */
AxAnimationElement.prototype.Initialize = function(canvasId)
{
    this.canvas = document.getElementById(canvasId);
    if (AxUtils.IsUndefinedOrNull(this.canvas) || (this.canvas.tagName.toLowerCase() !== 'canvas'))
        throw 'AxWebRendering.Initialize error: Element ' + canvasId + ' is not a valid canvas';
    
        this.canvas.axWebRendering = this;
        this.canvas.width = this.canvas.offsetWidth;
        this.canvas.height = this.canvas.offsetHeight;
        this.canvas.addEventListener('mousemove', CanvasMouseMoveEvent, false);
        this.canvas.addEventListener('mousedown', CanvasMouseButtonEvent, false);
        this.canvas.addEventListener('mouseup', CanvasMouseButtonEvent, false);
        //Works
        document.axWebRendering = this;
        document.onkeydown = CanvasKeyDownEvent;
        document.onkeyup = CanvasKeyUpEvent;
        ////Doesn't work
        //this.canvas.addEventListener('keydown', CanvasKeyDownEvent, false);
        //this.canvas.addEventListener('keyup', CanvasKeyUpEvent, false);
        //
        //this.canvas.onkeydown = CanvasKeyDownEvent;
        //this.canvas.onkeyup = CanvasKeyUpEvent;


    

    var requestAnimationFrameCandidates = 
    [
        window.requestAnimationFrame,
        window.webkitRequestAnimationFrame,
        window.mozRequestAnimationFrame,
        window.oRequestAnimationFrame,
        window.msRequestAnimationFrame,
        function(callback) 
        {
            window.setTimeout(callback, 1000 / 60);
        }
    ];
    
    for (var i = 0; AxUtils.IsUndefinedOrNull(this.RequestAniamtionFrame); i++)
        this.RequestAniamtionFrame = requestAnimationFrameCandidates[i];

    window.axWebRendering = this;
};

/**
 * Initiates continuous rendering onto the canvas
 */
AxAnimationElement.prototype.StartRendering = function()
{
    if (this.renderingEnabled)
        return;
    
    this.renderingEnabled = true;
    this.Render();
};

/**
 * Stops continuous rendering
 */
AxAnimationElement.prototype.StopRendering = function()
{
    this.renderingEnabled = false;
};

/**
 * Performs various routines to handle the element input and renders s frame using the OnRender method
 * This method is called when rendering continuously
 * This method uses the OnRender method to draw a frame on the canvas element
 */
AxAnimationElement.prototype.Render = function()
{
    var instance = window.axWebRendering;
    
    if (!instance.renderingEnabled)
        return;
    
    // Calling directly instance.RequestAniamtionFrame fails, need to call through a local variable
    var requestAnimationFrame = instance.RequestAniamtionFrame;
    requestAnimationFrame(instance.Render);

    if (!AxUtils.IsUndefinedOrNull(instance.OnRender))
        instance.OnRender();
    
    instance.mouse.deltaX = instance.mouse.x - instance.mouse.lastX;
    instance.mouse.deltaY = instance.mouse.y - instance.mouse.lastY;
    instance.mouse.lastX = instance.mouse.x;
    instance.mouse.lastY = instance.mouse.y;
};



// Element event subscribtion callbacks



function CanvasMouseMoveEvent(args) 
{
    var instance = this.axWebRendering;
    
    var rect = this.getBoundingClientRect();
    var x = args.clientX - rect.left;
    var y = args.clientY - rect.top;
    
    instance.onMouseMove(x, y);

    instance.mouse.x = x;
    instance.mouse.y = y;
}

function CanvasMouseButtonEvent(args) 
{
    var instance = this.axWebRendering;

    var rect = this.getBoundingClientRect();
    var x = args.clientX - rect.left;
    var y = args.clientY - rect.top;
    var buttonsDown = (instance.mouseButtons ^ args.buttons) & args.buttons;
    var buttonsUp = (instance.mouseButtons ^ args.buttons) & (~args.buttons);

    if (buttonsDown !== 0)
        instance.onMouseDown(x, y, buttonsDown);
    if (buttonsUp !== 0)
        instance.onMouseUp(x, y, buttonsUp);

    instance.mouse.left = args.buttons & AxAnimationElement.MouseButtonLeft !== 0;
    instance.mouse.right = args.buttons & AxAnimationElement.MouseButtonRight !== 0;
    instance.mouse.middle = args.buttons & AxAnimationElement.MouseButtonMiddle !== 0;
    instance.mouse.back = args.buttons & AxAnimationElement.MouseButtonBack !== 0;
    instance.mouse.forward = args.buttons & AxAnimationElement.MouseButtonForward !== 0;

    instance.mouseButtons = args.buttons;
}

function CanvasKeyDownEvent(args) 
{
    var instance = this.axWebRendering;

    instance.onKeyDown(args.keyCode);
    
    if (args.keyCode < instance.buttons.length)
        instance.buttons[args.keyCode] = true;
    
    //console.log('keyDown', args.keyCode);
}

function CanvasKeyUpEvent(args) 
{
    var instance = this.axWebRendering;

    instance.onKeyUp(args.keyCode);

    if (args.keyCode < instance.buttons.length)
        instance.buttons[args.keyCode] = false;
    
    //console.log('keyUp', args.keyCode);
}