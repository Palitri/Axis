/**
 * Represents a parameter of a specifiv type, which holds a value of that type.
 * Used to store values in axis
 * @param {!*} value The value of the parameter
 * @param {!AxParameterType} type Type of the value if the value is a reference
 * @constructor
 */
function AxParameter(value, type)
{
    this.type = AxParameterType.None;
    this.value = 0;
    
    if (!AxUtils.IsUndefined(value))
    {
        if (AxUtils.IsUndefinedOrNull(type))
        {
            if (AxUtils.IsInteger(value))
                type = AxParameterType.Int;
            else if (AxUtils.IsNumber(value))
                type = AxParameterType.Float;
            else if (AxUtils.IsBoolean(value))
                type = AxParameterType.Bool;
            else if (typeof value === 'AxVector4')
                type = AxParameterType.Color;
        }

        this.SetValue(value, type);
    }
};


/**
 * Returns the parameter's effective parameter.
 * For parameters which don't reference a mechanism, this would be the parameter itself.
 * For parameters which reference a mechanism, this would be the parameter at the end of the mechanism-reference chain, in other words the parameter wich holds the effective value
 * @param {AxParameter} parameter The parameter whose effective parameter to get
 * @return {AxParameter} The effective paramter of the given one
 */
AxParameter.GetEffectiveParameter = function(parameter)
{
    if (parameter.type === AxParameterType.ReferenceMechanism)
    {
        return AxParameter.GetEffectiveParameter((parameter.value).parameter);
    }
    else
        return parameter;
};

/**
 * Returns whether a given parameter type is that of a parameter which holds a reference to an object, rather than a value
 * @param {AxParameterType} type The parameter type to check
 * @returns {Boolean} True if the given parameter type is that of a reference parameter
 */
AxParameter.IsReference = function(type)
{
    return type >= 1000;
};

/**
 * Returns whether the value of the parameter is equal to the value of another parameter
 * @param {AxParameter} parameter The parameter to compare value against
 * @returns {Boolean} True if the value of the parameter is equal to the value of the provided parameter
 */
AxParameter.prototype.Equals = function(parameter)
{
    if (this.type !== parameter.type)
        return false;

    switch (this.type)
    {
        case AxParameterType.Int:
        case AxParameterType.Float:
        case AxParameterType.Bool:
            return this.value === parameter.value;
        case AxParameterType.Color:
            return this.value.Equals(parameter.value);
    }

    return this.value === parameter.value;
};

/**
 * Returns whether the parameter holds a reference value
 * @returns {Boolean} True if the value of the parameter is a reference
 */
AxParameter.prototype.IsReference = function()
{
    return AxParameter.IsReference(this.type);
};

/**
 * Provided for implementation consistency
 * Disposes of the parameter's value.
 */
AxParameter.prototype.DisposeValue = function()
{
    this.value = 0;
};

/**
 * Gets the parameter's effective parameter
 * For parameters referencing a mechanism chain, this would be the parameter of the mechanism at the end of the chain. Otherwise it's the parameter
 * @return {AxParameter} The parameter's effective parameter.
 */
AxParameter.prototype.GetEffectiveParameter = function()
{
    return AxParameter.GetEffectiveParameter(this);
};

/**
 * Sets the parameter value to it's evvective value
 * This eliminates any reference chain and makes the parameter adopt the value at the end of that chain at the moment of the recall.
 * For non reference chain parameter, the value remains the same
 */
AxParameter.prototype.RecallEffectiveParameter = function()
{
    var parameter = this.GetEffectiveParameter();

    if (parameter !== this)
        this.SetValue(parameter.value, parameter.type);
};

/**
 * Sets the parameter's type, initializing it's value, if necessary
 * @param {AxParameterType} type Type to set for the parameter
 */
AxParameter.prototype.SetType = function(type)
{
    if (this.type === type)
        return;

    this.DisposeValue();

    switch (type)
    {
        case AxParameterType.Int:
        {
            this.value = 0;
            break;
        }

        case AxParameterType.Float:
        {
            this.value = 0.0;
            break;
        }

        case AxParameterType.Bool:
        {
            this.value = false;
            break;
        }

        case AxParameterType.Color:
        {
            this.value = new AxVector4();
            break;
        }
    }

    this.type = type;
};

/**
 * Sets the parameter's value to a referece
 * @param {*} value Value of the referece to set
 * @param {AxParameterType} type Type of the referece to set
 */
AxParameter.prototype.SetValue = function(value, type)
{
    this.SetType(type);
    this.value = value;
};

/**
 * Gets the parameter's effective value of the parameter
 * For parameters referencing a mechanism chain, this would be the value of the mechanism's parameter at the end of the chain. Otherwise it's the parameter's value itself
 * @return {*} The effective value of the parameter
 */
AxParameter.prototype.GetEffectiveValue = function()
{
    return this.GetEffectiveParameter().value;
};

/**
 * Provided for implementation consistency. Use SetValue instead
 * @param {Integer} value The integer value to be set
 * Sets an integer value to the parameter
 */
AxParameter.prototype.SetInt = function(value)
{
    this.SetType(AxParameterType.Int);
    this.value = value;
};

/**
 * Provided for implementation consistency. Use GetEffectiveValue instead
 * Gets the effective value of the parameter as an integer
 * @return {Integer} The parameter's effective value as an integer
 */
AxParameter.prototype.GetInt = function()
{
    return this.GetEffectiveParameter().value;
};

/**
 * Provided for implementation consistency. Use SetValue instead
 * @param {Number} value The floating point value to be set
 * Sets a floating point value to the parameter
 */
AxParameter.prototype.SetFloat = function(value)
{
    this.SetType(AxParameterType.Float);
    this.value = value;
};

/**
 * Provided for implementation consistency. Use GetEffectiveValue instead
 * Gets the effective value of the parameter as a floating point
 * @return {Number} The parameter's effective value as a floating point
 */
AxParameter.prototype.GetFloat = function()
{
    return this.GetEffectiveParameter().value;
};

/**
 * Provided for implementation consistency. Use SetValue instead
 * @param {Boolean} value The boolean value to be set
 * Sets a boolean value to the parameter
 */
AxParameter.prototype.SetBool = function(value)
{
    this.SetType(AxParameterType.Bool);
    this.value = value;
};

/**
 * Provided for implementation consistency. Use GetEffectiveValue instead
 * Gets the effective value of the parameter as a boolean
 * @return {Boolean} The parameter's effective value as a boolean
 */
AxParameter.prototype.GetBool = function()
{
    return this.GetEffectiveParameter().value;
};

/**
 * Provided for implementation consistency. Use SetValue instead
 * @param {AxVector4} value The color vector value to be set
 * Sets a color vector value to the parameter
 */
AxParameter.prototype.SetColor = function(value)
{
    this.SetType(AxParameterType.Color);
    this.value = value;
};

/**
 * Provided for implementation consistency. Use GetEffectiveValue instead
 * Gets the effective value of the parameter as a color vector
 * @return {AxVector4} The parameter's effective value as a boolean
 */
AxParameter.prototype.GetColor = function()
{
    return this.GetEffectiveParameter().value;
};