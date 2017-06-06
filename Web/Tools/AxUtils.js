/**
 * @constructor
 */
function AxUtils()
{
}



// Static methods



/**
 * Returns whether a variable is not defined
 * @param {Number} variable The variable to check
 * @return {Boolean} True if the variable is not defined, False otherwise
 */
AxUtils.IsUndefined = function(variable)
{
    return typeof variable === 'undefined';
};

/**
 * Returns whether a variable is not defined or null
 * @param {Number} variable The variable to check
 * @return {Boolean} True if the variable is not defined or null, False otherwise
 */
AxUtils.IsUndefinedOrNull = function(variable)
{
    return (typeof variable === 'undefined') || (variable === null);
};

/**
 * Returns whether the given value is an integer.
 * If the value is a string containing an integer, returns false
 * @param {*} variable The variable to check
 * @returns {Boolean} True if the variable is an integer.
 */
AxUtils.IsInteger = function(variable)
{
    var n = Number(variable);
    return !isNaN(variable) && (n === variable) && (n % 1 === 0);
};

/**
 * Returns whether the given value is a number. This includes floating point and integer numbers.
 * If the value is a string containing a number, returns false
 * @param {*} variable The variable to check
 * @returns {Boolean} True if the variable is a number.
 */
AxUtils.IsNumber = function(variable)
{
    return !isNaN(variable) && (Number(variable) === variable);
};

/**
 * Returns whether the given value is a number. This includes floating point and integer numbers.
 * If the value is a string containing a number, returns false
 * @param {*} variable The variable to check
 * @returns {Boolean} True if the variable is a number.
 */
AxUtils.IsBoolean = function(variable)
{
    return typeof variable === 'boolean';
};