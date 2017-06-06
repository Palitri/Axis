/**
 * Represents a property, which is a named parameter and is used to store values and access them by name
 * @param {AxString} name The name of the property
 * @param {!*} value The value of the parameter
 * @param {!AxParameterType} type Type of the value if the value is a reference
 * @constructor
 */
function AxProperty(name, value, type)
{
    AxParameter.call(this, value, type);
    
    this.name = name;
};

AxProperty.prototype = Object.create(AxParameter.prototype);