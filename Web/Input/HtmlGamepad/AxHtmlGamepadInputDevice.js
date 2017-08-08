/**
 * Create a new AxGamepadInputDevice.
 * This input device uses an Html Canvas element to obtain user input from
 * @constructor
*/
function AxHtmlGamepadInputDevice()
{
    AxInputDevice.call(this);

    this.context = null;
    this.gamepadsProperties = new AxDictionary();
}

AxHtmlGamepadInputDevice.prototype = Object.create(AxInputDevice.prototype);


/**
 * Sets the context for the input device
 * @param {Axis} context The instance of Axis, to which the input device will apply
 */
AxHtmlGamepadInputDevice.prototype.SetContext = function(context)
{
    this.context = context;
    
    this.AcquireGamepadProperties(0);
    this.AcquireGamepadProperties(1);
    this.AcquireGamepadProperties(2);
    this.AcquireGamepadProperties(3);
    
    window.axGamepadInputDevice = this;
    window.addEventListener("gamepadconnected", AxHtmlGamepadInputDevice.OnGamepadConnected);
    window.addEventListener("gamepaddisconnected", AxHtmlGamepadInputDevice.OnGamepadDisconnected);
};

/**
 * Updates the device.
 * This method is called continuously on each rendering iteration
 */
AxHtmlGamepadInputDevice.prototype.Update = function()
{
    var gamepadsList = navigator.getGamepads ? navigator.getGamepads() : (navigator.webkitGetGamepads ? navigator.webkitGetGamepads : []);
    if (gamepadsList === null)
        return;
    
    for (var gamepadIndex = 0; gamepadIndex < gamepadsList.length; gamepadIndex++)
    {
        var gamepad = gamepadsList[gamepadIndex];
        if (gamepad !== null)
        {
            var isStandardMapping = gamepad.mapping === "standard";
                    
            var gamepadProperties = this.AcquireGamepadProperties(gamepad.index);
            gamepadProperties.padAvailable.SetBool(true);
            
            for (var buttonIndex = 0; buttonIndex < gamepad.buttons.length; buttonIndex++)
            {
                var keyProp = new AxReferenceParameter();
                if (gamepadProperties.keyProperties.GetValue(buttonIndex, keyProp))
                    AxHtmlGamepadInputDevice.SetPropertyButtonValue(keyProp.value, gamepad.buttons[buttonIndex]);
            }

           
            if (gamepad.axes.length > 1)
            {
                gamepadProperties.padAnalogXLeft.SetFloat(AxHtmlGamepadInputDevice.GetAxisValue(gamepad.axes[0]));
                gamepadProperties.padAnalogYLeft.SetFloat(-AxHtmlGamepadInputDevice.GetAxisValue(gamepad.axes[1]));
            }

            if (isStandardMapping)
            {
                if (gamepad.axes.length > 3)
                {
                    gamepadProperties.padAnalogXRight.SetFloat(AxHtmlGamepadInputDevice.GetAxisValue(gamepad.axes[2]));
                    gamepadProperties.padAnalogYRight.SetFloat(-AxHtmlGamepadInputDevice.GetAxisValue(gamepad.axes[3]));
                }
            }
            else
            {
                // Gamepad with no mapping might throw different stuff in different browser.
                // For example, multipe axes[] values mapped to the same hardware control, an axis containing value for d-pad, unmapped (zero) values in between
                // Example1: XBox360 controller, standard mapping: 0 - XLeft, 1 -YLeft, 2 - XRight, 3 - YRight
                // Example2: 0079-0006-Generic USB Joystick, no mapping, Firefox: 0 - XLeft, 1 -YLeft, 2 - XRight, 3 - XRight, 4 - YRight
                // Example3: 0079-0006-Generic USB Joystick, no mapping, Chrome: 0 - XLeft, 1 -YLeft, 2 - XRight, 3 - 0, 4 - 0, 5 - YRight, 6 - 0, 7 - 0, 8 - 0, 9 - DPad (Up: -1, Right: -0.43, Down: -0.14, Left: 0.71, None: 3.28);
                var axisIndex = 2;
                var xAxis = 0.0;
                var yAxis = xAxis;
                if (gamepadProperties.padAnalogXRightIndex === -1)
                {
                    for (axisIndex = 2; axisIndex < gamepad.axes.length; axisIndex++)
                    {
                        if (gamepad.axes[axisIndex] !== 0.0)
                        {
                            gamepadProperties.padAnalogXRightIndex = axisIndex;
                            xAxis = gamepad.axes[axisIndex];
                            break;
                        }
                    }
                }
                else
                    xAxis = gamepad.axes[gamepadProperties.padAnalogXRightIndex];

                if (gamepadProperties.padAnalogYRightIndex === -1)
                {
                    for (axisIndex++; axisIndex < gamepad.axes.length; axisIndex++)
                    {
                        var axisValue = gamepad.axes[axisIndex];
                        //var axesActive = (AxMath.Abs(xAxis) > 0.3) && (AxMath.Abs(xAxis) < 0.7) && (AxMath.Abs(axisValue) > 0.3) && (AxMath.Abs(axisValue) < 0.7);
                        var bothAxesActive = (AxMath.Abs(xAxis) > 0.3) && (AxMath.Abs(axisValue) > 0.3);
                        if ((axisValue !== 0.0) && (!bothAxesActive || (axisValue !== xAxis)))
                        {
                            if (bothAxesActive)
                                gamepadProperties.padAnalogYRightIndex = axisIndex;
                            yAxis = axisValue;
                            break;
                        }
                    }
                }
                else
                    yAxis = gamepad.axes[gamepadProperties.padAnalogYRightIndex];
                
                gamepadProperties.padAnalogXRight.SetFloat(AxHtmlGamepadInputDevice.GetAxisValue(xAxis));
                gamepadProperties.padAnalogYRight.SetFloat(-AxHtmlGamepadInputDevice.GetAxisValue(yAxis));
            }
        }
    }
};

/**
 * Acquires an object which holds properties for a given gamepad
 * For internal usage
 * @param {Integer} gamepadIndex The index of the gamepad
 * @returns {AxHtmlGamepadProperties}
 */
AxHtmlGamepadInputDevice.prototype.AcquireGamepadProperties = function(gamepadIndex)
{
    var keyProp = new AxReferenceParameter();
    if (this.gamepadsProperties.GetValue(gamepadIndex, keyProp))
        return keyProp.value;
    
    var gamepadProperties = new AxHtmlGamepadProperties();
    
    var idName = gamepadIndex === 0 ? "" : (gamepadIndex + 1).toString() + " ";

    gamepadProperties.keyProperties.Add(0, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "A"), false)));
	gamepadProperties.keyProperties.Add(1, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "B"), false)));
	gamepadProperties.keyProperties.Add(2, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "X"), false)));
	gamepadProperties.keyProperties.Add(3, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Y"), false)));
	gamepadProperties.keyProperties.Add(4, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Trigger button left"), false)));
	gamepadProperties.keyProperties.Add(5, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Trigger button right"), false)));
	gamepadProperties.keyProperties.Add(6, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Trigger analog left"), 0.0)));
	gamepadProperties.keyProperties.Add(7, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Trigger analog right"), 0.0)));
	gamepadProperties.keyProperties.Add(8, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Select"), false)));
	gamepadProperties.keyProperties.Add(9, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Start"), false)));
	gamepadProperties.keyProperties.Add(10, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Thumb left"), true)));
	gamepadProperties.keyProperties.Add(11, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Thumb right"), false)));
	gamepadProperties.keyProperties.Add(12, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Up"), false)));
	gamepadProperties.keyProperties.Add(13, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Down"), false)));
	gamepadProperties.keyProperties.Add(14, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Left"), false)));
	gamepadProperties.keyProperties.Add(15, this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Right"), false)));

    gamepadProperties.padAnalogXLeft            = this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Analog X left"), 0.0));
	gamepadProperties.padAnalogYLeft            = this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Analog Y left"), 0.0));
	gamepadProperties.padAnalogXRight			= this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Analog X right"), 0.0));
	gamepadProperties.padAnalogYRight			= this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Analog Y right"), 0.0));
	gamepadProperties.padAvailable              = this.context.input.properties.Add(new AxProperty(new AxString("Pad " + idName + "Available"), false));
    
    this.gamepadsProperties.Add(gamepadIndex, gamepadProperties);
    
    return gamepadProperties;
};




// Static methods



/**
 * Returns the floating point value of an axis by eliminating small values, which appear when the axis control is actually at rest
 * The returned value is in the range [-1, 1]
 * For internal usage
 * @param {type} rawValue The original value of the analog axis
 * @returns {Number} The filtered value in the range [-1, 1] of the analog axis
 */
AxHtmlGamepadInputDevice.GetAxisValue = function(rawValue)
{
	var threshold = 0.2;
	var range = 1.0;
	var usedRange = range - threshold;

	return rawValue > threshold ? (rawValue - threshold) / usedRange : rawValue < -threshold ? (rawValue + threshold) / usedRange : 0.0;
};

AxHtmlGamepadInputDevice.FindNextUsableAxisIndex = function(axes, startIndex, initial)
{
    var axesLength = axes.length;
    if (startIndex < axesLength)
    {
        var initialValue = axes[startIndex];
        for (var i = startIndex; i < axesLength; i++)
        {
            var axisValue = axes[i];
            if ((axisValue !== 0.0) && (initial || (axisValue !== initialValue)))
                return i;
        }
    }
    
    return startIndex;
};

/**
 * Sets an AxProperty value according to the value of a gamepad button 
 * For internal usage
 * @param {AxProperty} prop The property to be set
 * @param {type} button The button value to be used
 */
AxHtmlGamepadInputDevice.SetPropertyButtonValue = function(prop, button)
{
    if (typeof button === "object")
    {
        if (prop.type === AxParameterType.Bool)
            prop.SetBool(button.pressed);
        else
            prop.SetFloat(button.value);
    }
    else
    {
        if (prop.type === AxParameterType.Bool)
            prop.SetBool(button);
        else
            prop.SetFloat(button);
    }
};
        

AxHtmlGamepadInputDevice.OnGamepadConnected = function(args)
{
    var instance = this.axGamepadInputDevice;
    
    //console.log("Gamepad " + args.gamepad.id + " connected.");
};

AxHtmlGamepadInputDevice.OnGamepadDisconnected = function(args)
{
    var instance = this.axGamepadInputDevice;

    //console.log("Gamepad " + args.gamepad.id + " disconnected.");
};



/**
 * Creates an AxHtmlGamepadProperties object, used to hold the input properties for a gamepad device
 * For internal usage
 * @constructor
*/
function AxHtmlGamepadProperties()
{
    this.keyProperties = new AxDictionary();

    this.padAnalogXLeft = null;
	this.padAnalogYLeft = null;
	this.padAnalogXRight = null;
	this.padAnalogYRight = null;
	this.padTriggerAnalogLeft = null;
	this.padTriggerAnalogRight = null;
    
	// Used for determining the indices of the right thumbstick axes for non-standard gamapad mappings
    this.padAnalogXRightIndex = -1;
	this.padAnalogYRightIndex = -1;
}