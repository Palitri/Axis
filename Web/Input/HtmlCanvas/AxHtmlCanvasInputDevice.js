/**
 * Create a new AxHtmlCanvasInputDevice.
 * This input device uses an Html Canvas element to obtain user input from
 * @constructor
 */
function AxHtmlCanvasInputDevice()
{
    AxInputDevice.call(this);

    this.context = null;
    this.canvas = null;
    
    this.mouseWheel = 0;
    this.mouseWheelHorizontal = 0; 
}

AxHtmlCanvasInputDevice.prototype = Object.create(AxInputDevice.prototype);


AxHtmlCanvasInputDevice.MouseButtonNone      = 0;
AxHtmlCanvasInputDevice.MouseButtonLeft      = 1;
AxHtmlCanvasInputDevice.MouseButtonRight     = 2;
AxHtmlCanvasInputDevice.MouseButtonMiddle    = 4;
AxHtmlCanvasInputDevice.MouseButtonBack      = 8;
AxHtmlCanvasInputDevice.MouseButtonForward   = 16;

AxHtmlCanvasInputDevice.maxTouchPoints = 5;


/**
 * Sets the context for the input device
 * @param {Axis} context The instance of Axis, to which the input device will apply
 */
AxHtmlCanvasInputDevice.prototype.SetContext = function(context)
{
    this.context = context;

    this.canvas = this.context.GetRenderingWindow();
    
    // Canvas context passed for mouse events
    this.canvas.axHtmlCanvasInputDevice = this;
    this.canvas.addEventListener('mousemove', AxHtmlCanvasInputDevice.CanvasMouseMoveEvent, false);
    this.canvas.addEventListener('mousedown', AxHtmlCanvasInputDevice.CanvasMouseButtonEvent, false);
    this.canvas.addEventListener('mouseup', AxHtmlCanvasInputDevice.CanvasMouseButtonEvent, false);
    this.canvas.addEventListener('mouseleave', AxHtmlCanvasInputDevice.CanvasMouseLeaveEvent, false);
    this.canvas.addEventListener('wheel', AxHtmlCanvasInputDevice.CanvasMouseWheelEvent, false);
    
    this.canvas.addEventListener('touchmove', AxHtmlCanvasInputDevice.CanvasTouchMoveEvent, false);
    this.canvas.addEventListener('touchstart', AxHtmlCanvasInputDevice.CanvasTouchStartEvent, false);
    this.canvas.addEventListener('touchend', AxHtmlCanvasInputDevice.CanvasTouchEndEvent, false);

//    this.canvas.onmousemove = AxHtmlCanvasInputDevice.CanvasMouseMoveEvent;
//    this.canvas.onmousedown = AxHtmlCanvasInputDevice.CanvasMouseButtonEvent;
//    this.canvas.onmouseup = AxHtmlCanvasInputDevice.CanvasMouseButtonEvent;
//    this.canvas.onmouseleave = AxHtmlCanvasInputDevice.CanvasMouseLeaveEvent;
//    this.canvas.onwheel = AxHtmlCanvasInputDevice.CanvasMouseWheelEvent;

    // Document context passed for keyboard events
    document.axHtmlCanvasInputDevice = this;
    document.addEventListener('keydown', AxHtmlCanvasInputDevice.CanvasKeyDownEvent, false);
    document.addEventListener('keyup', AxHtmlCanvasInputDevice.CanvasKeyUpEvent, false);

    
    this.keyProperties = new AxDictionary();
    
    this.keyProperties.Add(0x30			, this.context.input.properties.Add(new AxProperty(new AxString("Key 0"), false)));
    this.keyProperties.Add(0x31			, this.context.input.properties.Add(new AxProperty(new AxString("Key 1"), false)));
    this.keyProperties.Add(0x32			, this.context.input.properties.Add(new AxProperty(new AxString("Key 2"), false)));
    this.keyProperties.Add(0x33			, this.context.input.properties.Add(new AxProperty(new AxString("Key 3"), false)));
    this.keyProperties.Add(0x34			, this.context.input.properties.Add(new AxProperty(new AxString("Key 4"), false)));
    this.keyProperties.Add(0x35			, this.context.input.properties.Add(new AxProperty(new AxString("Key 5"), false)));
    this.keyProperties.Add(0x36			, this.context.input.properties.Add(new AxProperty(new AxString("Key 6"), false)));
    this.keyProperties.Add(0x37			, this.context.input.properties.Add(new AxProperty(new AxString("Key 7"), false)));
    this.keyProperties.Add(0x38			, this.context.input.properties.Add(new AxProperty(new AxString("Key 8"), false)));
    this.keyProperties.Add(0x39			, this.context.input.properties.Add(new AxProperty(new AxString("Key 9"), false)));
    this.keyProperties.Add(0x41			, this.context.input.properties.Add(new AxProperty(new AxString("Key A"), false)));
    this.keyProperties.Add(0x42			, this.context.input.properties.Add(new AxProperty(new AxString("Key B"), false)));
    this.keyProperties.Add(0x43			, this.context.input.properties.Add(new AxProperty(new AxString("Key C"), false)));
    this.keyProperties.Add(0x44			, this.context.input.properties.Add(new AxProperty(new AxString("Key D"), false)));
    this.keyProperties.Add(0x45			, this.context.input.properties.Add(new AxProperty(new AxString("Key E"), false)));
    this.keyProperties.Add(0x46			, this.context.input.properties.Add(new AxProperty(new AxString("Key F"), false)));
    this.keyProperties.Add(0x47			, this.context.input.properties.Add(new AxProperty(new AxString("Key G"), false)));
    this.keyProperties.Add(0x48			, this.context.input.properties.Add(new AxProperty(new AxString("Key H"), false)));
    this.keyProperties.Add(0x49			, this.context.input.properties.Add(new AxProperty(new AxString("Key I"), false)));
    this.keyProperties.Add(0x4A			, this.context.input.properties.Add(new AxProperty(new AxString("Key J"), false)));
    this.keyProperties.Add(0x4B			, this.context.input.properties.Add(new AxProperty(new AxString("Key K"), false)));
    this.keyProperties.Add(0x4C			, this.context.input.properties.Add(new AxProperty(new AxString("Key L"), false)));
    this.keyProperties.Add(0x4D			, this.context.input.properties.Add(new AxProperty(new AxString("Key M"), false)));
    this.keyProperties.Add(0x4E			, this.context.input.properties.Add(new AxProperty(new AxString("Key N"), false)));
    this.keyProperties.Add(0x4F			, this.context.input.properties.Add(new AxProperty(new AxString("Key O"), false)));
    this.keyProperties.Add(0x50			, this.context.input.properties.Add(new AxProperty(new AxString("Key P"), false)));
    this.keyProperties.Add(0x51			, this.context.input.properties.Add(new AxProperty(new AxString("Key Q"), false)));
    this.keyProperties.Add(0x52			, this.context.input.properties.Add(new AxProperty(new AxString("Key R"), false)));
    this.keyProperties.Add(0x53			, this.context.input.properties.Add(new AxProperty(new AxString("Key S"), false)));
    this.keyProperties.Add(0x54			, this.context.input.properties.Add(new AxProperty(new AxString("Key T"), false)));
    this.keyProperties.Add(0x55			, this.context.input.properties.Add(new AxProperty(new AxString("Key U"), false)));
    this.keyProperties.Add(0x56			, this.context.input.properties.Add(new AxProperty(new AxString("Key V"), false)));
    this.keyProperties.Add(0x57			, this.context.input.properties.Add(new AxProperty(new AxString("Key W"), false)));
    this.keyProperties.Add(0x58			, this.context.input.properties.Add(new AxProperty(new AxString("Key X"), false)));
    this.keyProperties.Add(0x59			, this.context.input.properties.Add(new AxProperty(new AxString("Key Y"), false)));
    this.keyProperties.Add(0x5A			, this.context.input.properties.Add(new AxProperty(new AxString("Key Z"), false)));
    this.keyProperties.Add(0x70                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F1"), false)));
    this.keyProperties.Add(0x71                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F2"), false)));
    this.keyProperties.Add(0x72                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F3"), false)));
    this.keyProperties.Add(0x73                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F4"), false)));
    this.keyProperties.Add(0x74                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F5"), false)));
    this.keyProperties.Add(0x75                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F6"), false)));
    this.keyProperties.Add(0x76                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F7"), false)));
    this.keyProperties.Add(0x77                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F8"), false)));
    this.keyProperties.Add(0x78                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F9"), false)));
    this.keyProperties.Add(0x79                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F10"), false)));
    this.keyProperties.Add(0x7A                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F11"), false)));
    this.keyProperties.Add(0x7B                 , this.context.input.properties.Add(new AxProperty(new AxString("Key F12"), false)));
    this.keyProperties.Add(0xC0                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Tilde"), false)));
    this.keyProperties.Add(0xBD                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Minus"), false)));
    this.keyProperties.Add(0xBB                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Equals"), false)));
    this.keyProperties.Add(0x08                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Backspace"), false)));
    this.keyProperties.Add(0xDB                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Square bracket left"), false)));
    this.keyProperties.Add(0xDD                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Square bracket right"), false)));
    this.keyProperties.Add(0xDC                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Backslash"), false)));
    this.keyProperties.Add(0xBA                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Semicolon"), false)));
    this.keyProperties.Add(0xDE                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Quote"), false)));
    this.keyProperties.Add(0xBC                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Comma"), false)));
    this.keyProperties.Add(0xBE                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Period"), false)));
    this.keyProperties.Add(0xBF                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Slash"), false)));
    this.keyProperties.Add(0x20                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Space"), false)));
    this.keyProperties.Add(0x1B                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Escape"), false)));
    this.keyProperties.Add(0x0D                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Enter"), false)));
    this.keyProperties.Add(0x09                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Tab"), false)));
    this.keyProperties.Add(0x14                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Caps"), false)));
    this.keyProperties.Add(0x10                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Shift"), false)));
    this.keyProperties.Add(0x11                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Control"), false)));
    this.keyProperties.Add(0x12                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Alt"), false)));
    this.keyProperties.Add(0x00                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Print screen"), false)));
    this.keyProperties.Add(0x91                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Scroll lock"), false)));
    this.keyProperties.Add(0x13                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Pause"), false)));
    this.keyProperties.Add(0x2D                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Insert"), false)));
    this.keyProperties.Add(0x2E                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Delete"), false)));
    this.keyProperties.Add(0x24                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Home"), false)));
    this.keyProperties.Add(0x23                 , this.context.input.properties.Add(new AxProperty(new AxString("Key End"), false)));
    this.keyProperties.Add(0x21                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Page up"), false)));
    this.keyProperties.Add(0x22                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Page down"), false)));
    this.keyProperties.Add(0x25                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Arrow left"), false)));
    this.keyProperties.Add(0x27                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Arrow right"), false)));
    this.keyProperties.Add(0x26                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Arrow up"), false)));
    this.keyProperties.Add(0x28                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Arrow down"), false)));
    this.keyProperties.Add(0x5B                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Win left"), false)));
    this.keyProperties.Add(0x5C                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Win right"), false)));
    this.keyProperties.Add(0x5D                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Context menu"), false)));
    this.keyProperties.Add(0x60                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 0"), false)));
    this.keyProperties.Add(0x61                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 1"), false)));
    this.keyProperties.Add(0x62                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 2"), false)));
    this.keyProperties.Add(0x63                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 3"), false)));
    this.keyProperties.Add(0x64                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 4"), false)));
    this.keyProperties.Add(0x65                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 5"), false)));
    this.keyProperties.Add(0x66                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 6"), false)));
    this.keyProperties.Add(0x67                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 7"), false)));
    this.keyProperties.Add(0x68                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 8"), false)));
    this.keyProperties.Add(0x69                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num 9"), false)));
    this.keyProperties.Add(0x6F                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num divide"), false)));
    this.keyProperties.Add(0x6A                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num multiply"), false)));
    this.keyProperties.Add(0x6D                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num minus"), false)));
    this.keyProperties.Add(0x6B                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num plus"), false)));
    this.keyProperties.Add(0x6D                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num decimal"), false)));
    this.keyProperties.Add(0x90                 , this.context.input.properties.Add(new AxProperty(new AxString("Key Num lock"), false)));

    this.keyLShift				= this.context.input.properties.Add(new AxProperty(new AxString("Key Shift left"), false));
    this.keyRShift				= this.context.input.properties.Add(new AxProperty(new AxString("Key Shift right"), false));
    this.keyLCtrl				= this.context.input.properties.Add(new AxProperty(new AxString("Key Control left"), false));
    this.keyRCtrl				= this.context.input.properties.Add(new AxProperty(new AxString("Key Control right"), false));
    this.keyLAlt				= this.context.input.properties.Add(new AxProperty(new AxString("Key Alt left"), false));
    this.keyRAlt				= this.context.input.properties.Add(new AxProperty(new AxString("Key Alt right"), false));
    this.keyStateNumLock        = this.context.input.properties.Add(new AxProperty(new AxString("Key state Num lock"), false));
    this.keyStateCapsLock       = this.context.input.properties.Add(new AxProperty(new AxString("Key state Caps lock"), false));
    this.keyStateScrollLock     = this.context.input.properties.Add(new AxProperty(new AxString("Key state Scroll lock"), false));

    this.mouseRight	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Right"), false));
    this.mouseMiddle	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Middle"), false));
    this.mouseScroll	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Scroll"), 0));
    this.mouseHScroll	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Scroll horizontal"), 0));
    
    this.mousePointer = new AxHtmlCanvasInputDevicePointer();
    this.mousePointer.X		= this.context.input.properties.Add(new AxProperty(new AxString("Mouse X"), 0.0));
    this.mousePointer.Y		= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Y"), 0.0));
    this.mousePointer.XPos	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse X pos"), 0.0));
    this.mousePointer.YPos	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Y pos"), 0.0));    
    this.mousePointer.pressed	= this.context.input.properties.Add(new AxProperty(new AxString("Mouse Left"), false));

    // General pointer properties, updated by any relevant pointing device (mouse, touch, etc.) To be used as a reliable generic pointing device
    this.pointer = new AxHtmlCanvasInputDevicePointer();
    this.pointer.X	= this.context.input.properties.Add(new AxProperty(new AxString("Pointer X"), 0.0));
    this.pointer.Y	= this.context.input.properties.Add(new AxProperty(new AxString("Pointer Y"), 0.0));
    this.pointer.XPos	= this.context.input.properties.Add(new AxProperty(new AxString("Pointer X pos"), 0.0));
    this.pointer.YPos	= this.context.input.properties.Add(new AxProperty(new AxString("Pointer Y pos"), 0.0));    
    this.pointer.pressed    = this.context.input.properties.Add(new AxProperty(new AxString("Pointer Pressed"), false));
    // "Pointer Tracking"? for mouse wheel, touch scaling, etc.
    
    // Per touch point properties
    this.touchPoints = [];
    for (var touchPointIndex = 0; touchPointIndex < AxHtmlCanvasInputDevice.maxTouchPoints; touchPointIndex++)
    {
        var stringIndex = (touchPointIndex + 1).toString();
        var touchPoint = new AxHtmlCanvasInputDevicePointer();
        touchPoint.X = this.context.input.properties.Add(new AxProperty(new AxString("Touch point " + stringIndex + " X"), 0.0));
        touchPoint.Y = this.context.input.properties.Add(new AxProperty(new AxString("Touch point " + stringIndex + " Y"), 0.0));
        touchPoint.XPos = this.context.input.properties.Add(new AxProperty(new AxString("Touch point " + stringIndex + " X pos"), 0.0));
        touchPoint.YPos = this.context.input.properties.Add(new AxProperty(new AxString("Touch point " + stringIndex + " Y pos"), 0.0));
        touchPoint.pressed = this.context.input.properties.Add(new AxProperty(new AxString("Touch point " + stringIndex + " Pressed"), false));

        this.touchPoints.push(touchPoint);
    }
    
    // General touch properties providing a mean touch point info in any case of touch points count
    this.touch = new AxHtmlCanvasInputDevicePointer();
    this.touch.X	= this.context.input.properties.Add(new AxProperty(new AxString("Touch X"), 0.0));
    this.touch.Y	= this.context.input.properties.Add(new AxProperty(new AxString("Touch Y"), 0.0));
    this.touch.XPos	= this.context.input.properties.Add(new AxProperty(new AxString("Touch X pos"), 0.0));
    this.touch.YPos	= this.context.input.properties.Add(new AxProperty(new AxString("Touch Y pos"), 0.0));
    this.touch.pressed	= this.context.input.properties.Add(new AxProperty(new AxString("Touch Pressed"), false));
    this.touch.rotation	= this.context.input.properties.Add(new AxProperty(new AxString("Touch Rotation"), 0.0));
    this.touch.scaling	= this.context.input.properties.Add(new AxProperty(new AxString("Touch Scaling"), 1.0));

    // Helper variables
    this.touch.pointsCount = 0;
    this.touch.lastPointsCount = 0;
    this.touch.distanceValue = 0.0;
    this.touch.lastDistanceValue = 0.0;
    this.touch.rotationValue = 0.0;
    this.touch.lastRotationValue = 0.0;

    // Per number of touch points properties providing a mean touch point info
    this.touches = [];
    for (var touchIndex = 0; touchIndex < AxHtmlCanvasInputDevice.maxTouchPoints; touchIndex++)
    {
        var stringIndex = (touchIndex + 1).toString();
        var touch = new AxHtmlCanvasInputDevicePointer();
        touch.X = this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " X"), 0.0));
        touch.Y = this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " Y"), 0.0));
        touch.XPos = this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " X pos"), 0.0));
        touch.YPos = this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " Y pos"), 0.0));
        touch.pressed	= this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " Pressed"), false));
        touch.rotation	= this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " Rotation"), 0.0));
        touch.scaling	= this.context.input.properties.Add(new AxProperty(new AxString("Touch " + stringIndex + " Scaling"), 1.0));
        
        this.touches.push(touch);
    }
};

/**
 * Updates the device.
 * This method is called continuously on each rendering iteration
 */
AxHtmlCanvasInputDevice.prototype.Update = function() 
{
    this.mousePointer.Update();

    for (var touchPointIndex = 0; touchPointIndex < AxHtmlCanvasInputDevice.maxTouchPoints; touchPointIndex++)
        this.touchPoints[touchPointIndex].Update();

    this.touch.eventX = 0.0;
    this.touch.eventY = 0.0;
    if (this.touch.pointsCount > 0)
    {
        // Calculate center
        for (var touchPointIndex = 0; touchPointIndex < this.touch.pointsCount; touchPointIndex++)
        {
            this.touch.eventX += this.touchPoints[touchPointIndex].eventX;
            this.touch.eventY += this.touchPoints[touchPointIndex].eventY;
        }
        this.touch.eventX /= this.touch.pointsCount;
        this.touch.eventY /= this.touch.pointsCount;

        // Calculate mean distance and rotation
        this.touch.distanceValue = 0.0;
        this.touch.rotationValue = 0.0;
        for (var touchPointIndex = 0; touchPointIndex < this.touch.pointsCount; touchPointIndex++)
        {
            this.touch.distanceValue += AxVector2.Distance(new AxVector2(this.touch.eventX, this.touch.eventY), new AxVector2(this.touchPoints[touchPointIndex].eventX, this.touchPoints[touchPointIndex].eventY));
            this.touch.rotationValue += AxMath.ArcTan2(this.touchPoints[touchPointIndex].eventX - this.touch.eventX, this.touchPoints[touchPointIndex].eventY - this.touch.eventY);
        }
        this.touch.distanceValue = this.touch.distanceValue * 2.0 / this.touch.pointsCount;
        this.touch.rotationValue = this.touch.rotationValue / this.touch.pointsCount;
        
        //var v = new AxVector3(this.touchPoints[1].eventX - this.touch.eventX, this.touchPoints[1].eventY - this.touch.eventY, 0.0);
        //v = v.Normalize();
        //touchRotationValue = AxMaths.VectorAngle(angleZeroVector, v);
        //document.getElementById("appTitle").innerHTML = touchRotationValue.toString();
        //document.getElementById("appTitle").innerHTML = "X: " + this.touch.eventX.toString() + ", Y: " + this.touch.eventY.toString();
//        document.getElementById("appTitle").innerHTML = "X: " + (this.touch.eventX).toFixed(3) + 
//            "<br/>Y: " + (this.touch.eventY).toFixed(3) +
//            "<br/>X: " + (this.touchPoints[1].eventX).toFixed(3) + 
//            "<br/>Y: " + (this.touchPoints[1].eventY).toFixed(3) +
//            "<br/>X: " + (this.touchPoints[1].eventX - this.touch.eventX).toFixed(3) + 
//            "<br/>Y: " + (this.touchPoints[1].eventY - this.touch.eventY).toFixed(3) +
//            "<br/>X: " + (v.x).toFixed(3) + 
//            "<br/>Y: " + (v.y).toFixed(3) +
//            "<br/>R: " + (180.0 * touchRotationValue / AxMath.Pi).toFixed(0) +
//            "<br/>" + s;
            
    }
    
    if (this.touch.pointsCount !== this.touch.lastPointsCount)
    {
        this.touch.eventLastX = this.touch.eventX;
        this.touch.eventLastY = this.touch.eventY;
        this.touch.lastDistanceValue = this.touch.distanceValue;
        this.touch.lastRotationValue = this.touch.rotationValue;
    }
    this.touch.Update();
    this.touch.pressed.SetBool(this.touch.pointsCount > 0);
    this.touch.scaling.SetFloat(this.touch.lastDistanceValue === 0.0 ? 1.0 : this.touch.distanceValue / this.touch.lastDistanceValue);
    this.touch.rotation.SetFloat(this.touch.rotationValue - this.touch.lastRotationValue);
    this.touch.lastDistanceValue = this.touch.distanceValue;
    this.touch.lastRotationValue = this.touch.rotationValue;
    
    for (var touchIndex = 0; touchIndex < AxHtmlCanvasInputDevice.maxTouchPoints; touchIndex++)
    {
        var touch = this.touches[touchIndex];
        if (touchIndex === (this.touch.pointsCount - 1))
        {
            touch.Copy(this.touch);
            touch.rotation.SetFloat(this.touch.rotation.GetFloat());
            touch.scaling.SetFloat(this.touch.scaling.GetFloat());
        }
        else
        {
            touch.X.SetFloat(0.0);
            touch.Y.SetFloat(0.0);
            touch.rotation.SetFloat(0.0);
            touch.scaling.SetFloat(1.0);
            touch.pressed.SetBool(false);
        }
    }
    
    if (this.touch.pointsCount > 0)
        this.pointer.Copy(this.touch);
    else
        this.pointer.Copy(this.mousePointer);
    
    this.touch.lastPointsCount = this.touch.pointsCount;


    this.mouseScroll.SetFloat(this.mouseWheel);
    this.mouseHScroll.SetFloat(this.mouseWheelHorizontal);
    this.mouseWheel = 0;
    this.mouseWheelHorizontal = 0;
};


/**
 * Sets keys which are acquired in a special way.
 * These keys are the Left and Right of Shift, Control and Alt
 * Passing a non-special state key results in no action
 * For internal usage
 * @param {Integer} key The keycode for which to set special state
 * @param {type} eventArgs Event arguments, as passed to the event handler
 * @param {Boolean} state True for key state down, false for key state up
 */
AxHtmlCanvasInputDevice.prototype.SetSpecialKeyState = function(key, eventArgs, state)
{
    switch (key)
    {
        case 0x10:
        {
            if (eventArgs.location === KeyboardEvent.DOM_KEY_LOCATION_LEFT)
                this.keyLShift.SetBool(state);
            else if (eventArgs.location === KeyboardEvent.DOM_KEY_LOCATION_RIGHT)
                this.keyRShift.SetBool(state);
            
            break;
        }

        case 0x11:
        {
            if (eventArgs.location === KeyboardEvent.DOM_KEY_LOCATION_LEFT)
                this.keyLCtrl.SetBool(state);
            else if (eventArgs.location === KeyboardEvent.DOM_KEY_LOCATION_RIGHT)
                this.keyRCtrl.SetBool(state);
            
            break;
        }

        case 0x12:
        {
            if (eventArgs.location === KeyboardEvent.DOM_KEY_LOCATION_LEFT)
                this.keyLAlt.SetBool(state);
            else if (eventArgs.location === KeyboardEvent.DOM_KEY_LOCATION_RIGHT)
                this.keyRAlt.SetBool(state);
            
            break;
        }
    }
};



// Html element Event methods


AxHtmlCanvasInputDevice.CanvasMouseMoveEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;
    
    var rect = this.getBoundingClientRect();
    var x = args.clientX - rect.left;
    var y = args.clientY - rect.top;
    
    instance.mousePointer.eventX = x / instance.context.viewportWidth;
    instance.mousePointer.eventY = -y / instance.context.viewportHeight;
};

AxHtmlCanvasInputDevice.CanvasMouseButtonEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;

    instance.mousePointer.pressed.SetBool((args.buttons & AxHtmlCanvasInputDevice.MouseButtonLeft) !== 0);
    instance.mouseRight.SetBool((args.buttons & AxHtmlCanvasInputDevice.MouseButtonRight) !== 0);
    instance.mouseMiddle.SetBool((args.buttons & AxHtmlCanvasInputDevice.MouseButtonMiddle) !== 0);
};

AxHtmlCanvasInputDevice.CanvasMouseLeaveEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;
    
    instance.mousePointer.pressed.SetBool(false);
    instance.mouseRight.SetBool(false);
    instance.mouseMiddle.SetBool(false);
};

AxHtmlCanvasInputDevice.CanvasMouseWheelEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;
    
    var delta = AxMath.Sign(-args.deltaY);
    var deltaH = AxMath.Sign(-args.deltaX);
    
    instance.mouseWheel += delta;
    instance.mouseWheelHorizontal += deltaH;
};

AxHtmlCanvasInputDevice.CanvasTouchMoveEvent = function(args) 
{
    if (args.touches)
    {
        var instance = this.axHtmlCanvasInputDevice;

        instance.touch.pointsCount = AxMath.Min(args.touches.length, AxHtmlCanvasInputDevice.maxTouchPoints);
        for (var touchIndex = 0; touchIndex < instance.touch.pointsCount; touchIndex++)
        {
            var touch = args.touches[touchIndex];
            var x = touch.pageX - touch.target.offsetLeft;
            var y = touch.pageY - touch.target.offsetTop;

            instance.touchPoints[touchIndex].eventX = x / instance.context.viewportWidth;
            instance.touchPoints[touchIndex].eventY = -y / instance.context.viewportHeight;
        }
    }
};

AxHtmlCanvasInputDevice.CanvasTouchStartEvent = function(args) 
{
    if (args.touches)
    {
        var instance = this.axHtmlCanvasInputDevice;
        
        instance.touch.pointsCount = AxMath.Min(args.touches.length, AxHtmlCanvasInputDevice.maxTouchPoints);
        for (var touchIndex = 0; touchIndex < instance.touch.pointsCount; touchIndex++)
        {
            var touch = args.touches[touchIndex];
            var x = touch.pageX - touch.target.offsetLeft;
            var y = touch.pageY - touch.target.offsetTop;

            instance.touchPoints[touchIndex].pressed.SetBool(true);
            instance.touchPoints[touchIndex].eventX = x / instance.context.viewportWidth;
            instance.touchPoints[touchIndex].eventY = -y / instance.context.viewportHeight;
            instance.touchPoints[touchIndex].eventLastX = instance.touchPoints[touchIndex].eventX;
            instance.touchPoints[touchIndex].eventLastY = instance.touchPoints[touchIndex].eventY;
        }
    }
};

AxHtmlCanvasInputDevice.CanvasTouchEndEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;

    instance.touch.pointsCount = AxMath.Min(args.touches.length, AxHtmlCanvasInputDevice.maxTouchPoints);
    for (var touchIndex = 0; touchIndex < AxHtmlCanvasInputDevice.maxTouchPoints; touchIndex++)
        instance.touchPoints[touchIndex].pressed.SetBool(touchIndex < instance.touch.pointsCount);
};

AxHtmlCanvasInputDevice.CanvasKeyDownEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;
    
    var key = args.keyCode;
    
    var keyProp = new AxReferenceParameter();
    if (instance.keyProperties.GetValue(key, keyProp))
        keyProp.value.SetBool(true);
    
    instance.SetSpecialKeyState(key, args, true);
    
    //console.log(keyProp.value.name.GetContents());
};

AxHtmlCanvasInputDevice.CanvasKeyUpEvent = function(args) 
{
    var instance = this.axHtmlCanvasInputDevice;

    var key = args.keyCode;
    
    var keyProp = new AxReferenceParameter();
    if (instance.keyProperties.GetValue(key, keyProp))
        keyProp.value.SetBool(false);
    
    instance.SetSpecialKeyState(key, args, false);
};


/*
 * Helper class for AxHtmlCanvasInputDevice
 * Used to hold data on pointers such as mouse and touch points.
 * When the input event occurs on the device, data is written in this class, 
 * instead of directly into the corresponding input property (AxProperty), 
 * because an input event may occur multiple times during a single frame.
 * Input data is usually processed once every frame, so acumulated data
 * such as delta position must also be calculated once per frame
 */
function AxHtmlCanvasInputDevicePointer()
{
    this.X = null;
    this.Y = null;
    this.XPos = null;
    this.YPos = null;
    this.pressed = null;
    
    this.eventX = 0.0;
    this.eventY = 0.0;
    this.eventLastX = 0.0;
    this.eventLastY = 0.0;
}

AxHtmlCanvasInputDevicePointer.prototype.Update = function()
{
    this.X.SetFloat(this.eventX - this.eventLastX);
    this.Y.SetFloat(this.eventY - this.eventLastY);
    this.XPos.SetFloat(this.eventX);
    this.YPos.SetFloat(this.eventY);
    
    this.eventLastX = this.eventX;
    this.eventLastY = this.eventY;
};

AxHtmlCanvasInputDevicePointer.prototype.Copy = function(source)
{
    this.X.SetFloat(source.X.GetFloat());
    this.Y.SetFloat(source.Y.GetFloat());
    this.XPos.SetFloat(source.XPos.GetFloat());
    this.YPos.SetFloat(source.YPos.GetFloat());
    this.pressed.SetBool(source.pressed.GetBool());
};
