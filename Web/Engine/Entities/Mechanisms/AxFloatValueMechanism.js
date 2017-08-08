/**
 * Creates a new float value mechanism. Used to produce a simple floating point value
 * @param {Number} value The value which the mechanism is to produce
 * @constructor
 */
function AxFloatValueMechanism(value)
{
    AxMechanism.call(this);

    this.typeId = AxFloatValueMechanism.classId;

    this.parameter.SetFloat(value);

    this.properties.Add(new AxProperty(new AxString("Value"), value));
}

AxFloatValueMechanism.prototype = Object.create(AxMechanism.prototype);


AxFloatValueMechanism.classId = AxMechanism.classId | 101;


AxFloatValueMechanism.propertyIndex_Value = AxResource.propertyIndex_ChildPropertiesIndex + 0;



AxFloatValueMechanism.prototype.Dispose = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxFloatValueMechanism.prototype.Process = function(deltaTime)
{
    this.parameter.value = this.properties.Get(AxFloatValueMechanism.propertyIndex_Value).GetEffectiveParameter().value;
    return true;
};