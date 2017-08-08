/**
 * Creates an object instance of AxInput
 * AxInput is used to contain the input received by all input devices
 * @constructor
 */
function AxInput()
{
    AxResource.call(this);
}

AxInput.prototype = Object.create(AxResource.prototype);

/**
 * Acquires an input property. If the property does not exist, it will be created.
 * If a property of the same name, but of different type exists, returns null
 * @param {AxString} inputName The initial (starting) point of the ray
 * @param {AxParameterType} inputType The initial (starting) point of the ray
 * @returns {AxProperty} The acquired property or null if a property with the same name, but of different type exists
 */
AxInput.prototype.AcquireInput = function(inputName, inputType)
{
    var result = this.GetProperty(inputName);

    if (result === null)
        return this.properties.Add(new AxProperty(inputName, false, inputType));

    if (result.type === inputType)
        return result;

    return null;
};

/**
 * Fills an AxInputControls with the input properties corresponding to the given names
 * @param {AxInputControls} result A list to be filled with AxProperty properties from the input, corresponding the the given property names
 * @param {String|AxString} inputNames Names of input properties. Can be a single name or multiple comma separated names
 */
AxInput.prototype.GetInputControls = function(result, inputNames)
{
    inputNames = AxString.GetAxString(inputNames);

    var names = new AxStrings();
	names.SplitString(inputNames, ",");
	for (var i = 0; i < names.count; i++)
	{
		var prop = this.GetProperty(names.Get(i));
		if (prop !== null)
			result.Add(prop);
	}
};
