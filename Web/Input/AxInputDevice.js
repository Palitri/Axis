/**
 * Create a new AxInputDevice
 * AxInputDevice is the base class of Axis devices, which manage input from keyboard, mouse, gamepads, joystics and etcetera
 */
function AxInputDevice()
{
}

/**
 * Sets the context for the input device
 * @param {Axis} context The instance of Axis, to which the input device will apply
 */
AxInputDevice.prototype.SetContext = function(context) { };

/**
 * Updates the device.
 * This method is called continuously on each rendering iteration
 */
AxInputDevice.prototype.Update = function() { };
