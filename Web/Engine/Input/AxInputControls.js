/**
 * Creates an object instance of AxInputControls
 * AxInputControls is used for input operations to hold the input properties of multiple controls, which correspond to a single action
 * The input properties held by AxInputControls can be considered alternatives for the same control
 * @constructor
 */
function AxInputControls()
{
    AxList.call(this);
}

AxInputControls.prototype = Object.create(AxList.prototype);



/**
 * Gets a value for the controls
 * The value for boolean properties like buttons controls is 0.0 or 1.0
 * The value for float properties of analog controls is usually between -1.0 and 1.0 or between 0.0 and 1.0, depending on the control. There might be exceptions
 * The value for float properties of other controls
 * Generally, a typical approach would be to consider the value of 1.0 as an "active" state and the value of 0.0 as inactive
 * @param {Number} deltaTime The time in seconds, passed since the last processing
 * @returns {Number} The value for the controls
 */ 
AxInputControls.prototype.GetValue = function(deltaTime)
{
	for (var i = 0; i < this.count; i++)
	{
		var inputProperty = this.Get(i);
		if (inputProperty !== 0)
			if (inputProperty.type === AxParameterType.Bool)
			{
				// If value is false, instead of returning, continue checking other alternative inputs
				if (inputProperty.GetBool())
					return 1.0 * deltaTime;
			}
			else if (inputProperty.type === AxParameterType.Float)
			{
				// If value is 0.0, instead of returning, continue checking other alternative inputs
				// If property name contains the word "analog" this means that the input is much like a button with value 0 and 1, but analog - with values in the interval [0, 1] and possibly negative. If so, multiply it by delta time as with digital buttons. The word "analog" stands to denote it's range is [0,1] (or [-1,1]) but including the real numbers in between. It is unlike other analog inputs, such as the mouse, which produce a momentary delta value that generally has no determined range and typically calls for using the value as absolute, in contrast to buttons and analogs, where the value is usually taken in relation to the time it was on
				var value = inputProperty.GetFloat();
				if (value !== 0.0)
					return inputProperty.name.ToLower().IndexOf("analog") === -1 ? value : value * deltaTime;
			}
	}

	return 0.0;
};
