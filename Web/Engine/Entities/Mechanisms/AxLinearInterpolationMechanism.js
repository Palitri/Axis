/**
 * Creates a new linear interpolation mechanism
 * This is a funcitonal mechanism used to linearly interpolate between two given values or extrapolate outside them, based on an input argument
 * @param {Number} argumentLeft The left endpoint of the argument's interval
 * @param {Number} argumentRight The right endpoint of the argument's interval
 * @param {Number} left The left endpoint of the resulting values interval
 * @param {Number} right The right endpoint of the resulting values interval
 * @param {Boolean} clamp Denotes whether the result should be clamped between the interval endpoints, thus producing interpolation only. If false, it will allow extrapolation for arguments beyond the argument interval
 * @constructor
 */
function AxLinearInterpolationMechanism(argumentLeft, argumentRight, left, right, clamp)
{
    AxMechanism.call(this);

    this.typeId = AxLinearInterpolationMechanism.classId;

    this.parameter.SetFloat(left);

    this.properties.Add(new AxProperty(new AxString("Argument"), argumentLeft));
    this.properties.Add(new AxProperty(new AxString("Argument left"), argumentLeft));
    this.properties.Add(new AxProperty(new AxString("Argument right"), argumentRight));
    this.properties.Add(new AxProperty(new AxString("Left"), left));
    this.properties.Add(new AxProperty(new AxString("Right"), right));
    this.properties.Add(new AxProperty(new AxString("Clamp"), clamp));
}

AxLinearInterpolationMechanism.prototype = Object.create(AxMechanism.prototype);


AxLinearInterpolationMechanism.classId = AxMechanism.classId | 105;

AxLinearInterpolationMechanism.propertyIndex_Argument       = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxLinearInterpolationMechanism.propertyIndex_ArgumentLeft   = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxLinearInterpolationMechanism.propertyIndex_ArgumentRight  = AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxLinearInterpolationMechanism.propertyIndex_Left           = AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxLinearInterpolationMechanism.propertyIndex_Right          = AxResource.propertyIndex_ChildPropertiesIndex + 4;
AxLinearInterpolationMechanism.propertyIndex_Clamp          = AxResource.propertyIndex_ChildPropertiesIndex + 5;

AxLinearInterpolationMechanism.prototype.AxLinearInterpolationMechanism = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxLinearInterpolationMechanism.prototype.Process = function(deltaTime)
{
    var argument = this.properties.Get(AxLinearInterpolationMechanism.propertyIndex_Argument).GetEffectiveParameter().value;
    var argumentLeft = this.properties.Get(AxLinearInterpolationMechanism.propertyIndex_ArgumentLeft).GetEffectiveParameter().value;
    var argumentRight = this.properties.Get(AxLinearInterpolationMechanism.propertyIndex_ArgumentRight).GetEffectiveParameter().value;
    var left = this.properties.Get(AxLinearInterpolationMechanism.propertyIndex_Left).GetEffectiveParameter().value;
    var right = this.properties.Get(AxLinearInterpolationMechanism.propertyIndex_Right).GetEffectiveParameter().value;
    var clamp = this.properties.Get(AxLinearInterpolationMechanism.propertyIndex_Clamp).GetEffectiveParameter().value;

    var argumentUnit = (argument - argumentLeft) / (argumentRight - argumentLeft);
    if (clamp)
        argumentUnit = AxMath.Trim(argument, 0.0, 1.0);

    this.parameter.value = left + (right - left) * argumentUnit;

    return true;
};