/**
 * Creates an object instance of AxInputModel
 * AxInputModel is the base for input models which perform manipulations according to the received input
 * An input model can, for example, be used to automatically control camera with mouse/keyboard or gamepad or respond to mouse input to rotate an object of interest in the scene
 * @constructor
 */
function AxInputModel()
{
}

/**
 * Parses the names of the inputs for the model
 * For internal use
 * @param {AxInput} input The input instance, which will be used to control the model
 * @param {AxList} properties A list to be filled with AxProperty properties from the input, corresponding the the given property names
 * @param {String|AxString} propertyNames Names of input properties. Can be comma separated
 */
AxInputModel.ParseInputNames = function(input, properties, propertyNames)
{
    propertyNames = AxString.GetAxString(propertyNames);

    var names = new AxStrings();
    names.SplitString(propertyNames, ",");
    for (var i = 0; i < names.count; i++)
    {
        var prop = input.GetProperty(names.Get(i));
        if (prop !== null)
            properties.Add(prop);
    }
};

/**
 * Returns the value for the given input properties
 * For internal use
 * @param {AxList} inputProperties A list of AxProperty properties for a control
 * @param {Number} deltaTime The time in seconds, passed since the last processing
 * @returns {Number} The value of the given input properties
 */
AxInputModel.ProcessInputProperties = function(inputProperties, deltaTime)
{
    for (var i = 0; i < inputProperties.count; i++)
    {
        var inputProperty = inputProperties.Get(i);
        if (inputProperty !== null)
        {
            if (inputProperty.type === AxParameterType.Bool)
            {
                if (inputProperty.GetBool())
                    return 1.0 * deltaTime;
            }
            else if (inputProperty.type === AxParameterType.Float)
            {
                var value = inputProperty.GetFloat();
                if (value !== 0.0)
                    return inputProperty.name.ToLower().IndexOf("analog") === -1 ? value : value * deltaTime;;
            }
        }
    }

    return 0.0;
};