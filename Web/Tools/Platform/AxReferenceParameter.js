/**
 * JavaScript specific prototype
 * Creates a new object, which can be used to wrap a value that needs to be passed as a reference parameter in a method
 * The value can be changed by the method and then 
 * @param {*} value The value which needs to be passed as a reference
 * @constructor
 */
function AxReferenceParameter(value)
{
    this.value = value;
};