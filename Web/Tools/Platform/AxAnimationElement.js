/**
 * Gets fired when an AxAnimationElement is ready to render a frame
 * @callback AxAnimationElementRender
 */


/**
 * Utilizes the canvas element which will be used for rendering
 * Provides useful functionality for continuous rendering, mouse and keyboard input
 * @param {*} canvasId The id of the HTML canvas element
 * @param {AxAnimationElementRender} onRenderEvent The method which to perform rendering of a frame
 * @param {!Boolean} startRendering Denotes whether to start rendering immediately. If omitted, assumes a default value of false
 * @param {!Boolean} enableContextMenu Denotes whether to enable the default context menu on the canvas element. If omitted, assumes a default value of false
 * @param {!Boolean} touchScrollingEnabled Denotes whether to enable scrolling the page when touching the canvas in a touchscreen device
 * @constructor
 */
function AxAnimationElement(canvasId, onRenderEvent, startRendering, enableContextMenu, touchScrollingEnabled)
{
    if (AxUtils.IsUndefinedOrNull(startRendering))
        startRendering = false;
    
    if (AxUtils.IsUndefinedOrNull(enableContextMenu))
        enableContextMenu = false;

    if (AxUtils.IsUndefinedOrNull(touchScrollingEnabled))
        touchScrollingEnabled = false;

    
    this.renderingEnabled = false;
    this.contextMenuEnabled = enableContextMenu;
    this.touchScrollingEnabled = touchScrollingEnabled;
    
    this.OnRender = null;
    
    this.RequestAniamtionFrame;
    
    this.canvas = null;
    
    this.buttons = new Array(256);
    for (var i = 0; i < this.buttons.length; i++)
        this.buttons[i] = false;
    
    this.mouseButtons = 0;
    this.mouse = { x: 0, y: 0, lastX: 0, lastY: 0, deltaX: 0, deltaY: 0, originalX: 0, originalY: 0, left: false, right: false, middle: false, back: false, forward: false }; 
    
    this.OnMouseMove = function(x, y) { };
    this.OnMouseDown = function(x, y, button) { };
    this.OnMouseUp = function(x, y, button) { };
    this.OnKeyDown = function(key) { };
    this.OnKeyUp = function(key) { };
    
    if (!AxUtils.IsUndefinedOrNull(canvasId))
        this.Initialize(canvasId);

    if (!AxUtils.IsUndefinedOrNull(onRenderEvent))
        this.OnRender = onRenderEvent;
    
    if (startRendering)
        this.StartRendering();
}

AxAnimationElement.MouseButtonNone      = 0;
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
    
        // Canvas context passed for mouse events
        this.canvas.axWebRendering = this;
        this.canvas.width = this.canvas.offsetWidth;
        this.canvas.height = this.canvas.offsetHeight;
        this.canvas.addEventListener('mousemove', AxAnimationElement.CanvasMouseMoveEvent, false);
        this.canvas.addEventListener('mousedown', AxAnimationElement.CanvasMouseButtonEvent, false);
        this.canvas.addEventListener('mouseup', AxAnimationElement.CanvasMouseButtonEvent, false);
        this.canvas.addEventListener('mouseleave', AxAnimationElement.CanvasMouseLeaveEvent, false);
        
        this.canvas.addEventListener('touchmove', AxAnimationElement.CanvasTouchMoveEvent, false);
        this.canvas.addEventListener('touchstart', AxAnimationElement.CanvasTouchStartEvent, false);
        this.canvas.addEventListener('touchend', AxAnimationElement.CanvasTouchEndEvent, false);
        
        // Document context passed for keyboar events
        document.axWebRendering = this;
        document.addEventListener('keydown', AxAnimationElement.CanvasKeyDownEvent, false);
        document.addEventListener('keyup', AxAnimationElement.CanvasKeyUpEvent, false);

        // Handle context menu
        //this.canvas.addEventListener('contextmenu', CanvasContextMenuEvent, false);
        this.canvas.oncontextmenu = AxAnimationElement.CanvasContextMenuEvent;

    

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



AxAnimationElement.CanvasMouseMoveEvent = function(args) 
{
    var instance = this.axWebRendering;
    
    var rect = this.getBoundingClientRect();
    var x = args.clientX - rect.left;
    var y = args.clientY - rect.top;
    
    instance.OnMouseMove(x, y);

    instance.mouse.x = x;
    instance.mouse.y = y;
};

AxAnimationElement.CanvasMouseButtonEvent = function(args) 
{
    var instance = this.axWebRendering;

    var rect = this.getBoundingClientRect();
    var x = args.clientX - rect.left;
    var y = args.clientY - rect.top;
    var buttonsDown = (instance.mouseButtons ^ args.buttons) & args.buttons;
    var buttonsUp = (instance.mouseButtons ^ args.buttons) & (~args.buttons);

    if (buttonsDown !== 0)
    {
        instance.OnMouseDown(x, y, buttonsDown);
        instance.mouse.originalX = instance.mouse.x;
        instance.mouse.originalY = instance.mouse.y;
    }
    if (buttonsUp !== 0)
        instance.OnMouseUp(x, y, buttonsUp);

    instance.mouse.left = (args.buttons & AxAnimationElement.MouseButtonLeft) !== 0;
    instance.mouse.right = (args.buttons & AxAnimationElement.MouseButtonRight) !== 0;
    instance.mouse.middle = (args.buttons & AxAnimationElement.MouseButtonMiddle) !== 0;
    instance.mouse.back = (args.buttons & AxAnimationElement.MouseButtonBack) !== 0;
    instance.mouse.forward = (args.buttons & AxAnimationElement.MouseButtonForward) !== 0;

    instance.mouseButtons = args.buttons;
};

AxAnimationElement.CanvasMouseLeaveEvent = function(args) 
{
    var instance = this.axWebRendering;
    
    var buttons = AxAnimationElement.MouseButtonNone;
    
    var buttonsUp = (instance.mouseButtons ^ buttons) & (~buttons);

    if (buttonsUp !== 0)
        instance.OnMouseUp(instance.mouse.x, instance.mouse.y, buttonsUp);

    instance.mouse.left = false;
    instance.mouse.right = false;
    instance.mouse.middle = false;
    instance.mouse.back = false;
    instance.mouse.forward = false;

    instance.mouseButtons = buttons;
};

AxAnimationElement.CanvasTouchMoveEvent = function(args) 
{
    if (args.touches)
    {
        if (args.touches.length === 1)
        {
            var instance = this.axWebRendering;

            var touch = args.touches[0];
            var x = touch.pageX - touch.target.offsetLeft;
            var y = touch.pageY - touch.target.offsetTop;

            instance.mouse.x = x / instance.context.viewportWidth;
            instance.mouse.y = -y / instance.context.viewportHeight;
            
            if (!instance.touchScrollingEnabled)
                args.preventDefault();
        }
    }
};

AxAnimationElement.CanvasTouchStartEvent = function(args) 
{
    if (args.touches)
    {
        if (args.touches.length === 1)
        {
            var instance = this.axWebRendering;
            
            var touch = args.touches[0];
            var x = touch.pageX - touch.target.offsetLeft;
            var y = touch.pageY - touch.target.offsetTop;

            instance.mouse.x = x;
            instance.mouse.y = -y;
            instance.mouse.lastX = instance.mouse.x;
            instance.mouse.lastY = instance.mouse.y;
            instance.mouse.originalX = instance.mouse.x;
            instance.mouse.originalY = instance.mouse.y;

            instance.OnMouseDown(x, y, AxAnimationElement.MouseButtonLeft);

            instance.mouse.left = true;

            if (!instance.touchScrollingEnabled)
                args.preventDefault();
            
        }
    }
};

AxAnimationElement.CanvasTouchEndEvent = function(args) 
{
    var instance = this.axWebRendering;

    if (args.touches)
    {
        if (args.touches.length === 1)
        {
            var touch = args.touches[0];
            var x = touch.pageX - touch.target.offsetLeft;
            var y = touch.pageY - touch.target.offsetTop;
        
            instance.OnMouseUp(x, y, AxAnimationElement.MouseButtonLeft);
        }
    }

    instance.mouse.left = false;
};

AxAnimationElement.CanvasContextMenuEvent = function(args) 
{
    var instance = this.axWebRendering;

    return instance.contextMenuEnabled;
};


AxAnimationElement.CanvasKeyDownEvent = function(args)
{
    var instance = this.axWebRendering;

    instance.OnKeyDown(args.keyCode);
    
    if (args.keyCode < instance.buttons.length)
        instance.buttons[args.keyCode] = true;
};

AxAnimationElement.CanvasKeyUpEvent = function(args) 
{
    var instance = this.axWebRendering;

    instance.OnKeyUp(args.keyCode);

    if (args.keyCode < instance.buttons.length)
        instance.buttons[args.keyCode] = false;
};