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
 * Accepts one of the following parameters set:
 * 1. AxInputControls and a String
 * 2. String
 * @param {!AxInputControls} arg1 A list to be filled with AxProperty properties from the input, corresponding the the given property names
 * @param {String|AxString} arg2 Names of input properties. Can be a single name or multiple comma separated names
 * @returns {AxInputControls} The input controls for the given names. If an AxInputControls parameter is provided, it coincides with the returned value 
 */
AxInput.prototype.GetInputControls = function(arg1, arg2)
{
    if (AxUtils.IsUndefinedOrNull(arg2))
    {
        arg2 = new AxInputControls();
        this.GetInputControls_1(arg2, arg1);
        return arg2;
    }
    else
    {
        this.GetInputControls_1(arg1, arg2);
        return arg1;
    }
};

/**
 * Fills an AxInputControls with the input properties corresponding to the given names
 * @param {AxInputControls} result A list to be filled with AxProperty properties from the input, corresponding the the given property names
 * @param {String|AxString} inputNames Names of input properties. Can be a single name or multiple comma separated names
 */
AxInput.prototype.GetInputControls_1 = function(result, inputNames)
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
