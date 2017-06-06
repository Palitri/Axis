/**
 * Creates a new sine mechanism
 * The sine mechanism is a non-animate mechanism that produces a result of a sine function by a given argument
 * @param {Number} periodStart The value at which the provided argument will produce the start of the sine function
 * @param {Number} periodEnd The value at which the provided argument will produce the end of the sine function
 * @param {Number} amplitudeMin The minimum value of the resulting sine function
 * @param {Number} amplitudeMax The peak value of the resulting sine function
 */
function AxSineMechanism(periodStart, periodEnd, amplitudeMin, amplitudeMax)
{
    AxMechanism.call(this);

    this.typeId = AxSineMechanism.classId;

    this.parameter.SetFloat(0.0);

    this.properties.Add(new AxProperty(new AxString("Argument"), periodStart));
    this.properties.Add(new AxProperty(new AxString("Period start"), periodStart));
    this.properties.Add(new AxProperty(new AxString("Period end"), periodEnd));
    this.properties.Add(new AxProperty(new AxString("Amplitude min"), amplitudeMin));
    this.properties.Add(new AxProperty(new AxString("Amplitude max"), amplitudeMax));
}

AxSineMechanism.prototype = Object.create(AxMechanism.prototype);


AxSineMechanism.classId = AxMechanism.classId | 104;

AxSineMechanism.propertyIndex_Argument      = AxResource.propertyIndex_ChildPropertiesIndex + 0;
AxSineMechanism.propertyIndex_PeriodStart   = AxResource.propertyIndex_ChildPropertiesIndex + 1;
AxSineMechanism.propertyIndex_PeriodEnd     = AxResource.propertyIndex_ChildPropertiesIndex + 2;
AxSineMechanism.propertyIndex_AmplitudeMin  = AxResource.propertyIndex_ChildPropertiesIndex + 3;
AxSineMechanism.propertyIndex_AmplitudeMax  = AxResource.propertyIndex_ChildPropertiesIndex + 4;



AxSineMechanism.prototype.Dispose = function()
{
};


/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxSineMechanism.prototype.Process = function(deltaTime)
{
    var argument = this.properties.Get(AxSineMechanism.propertyIndex_Argument).GetEffectiveValue();
    var periodStart = this.properties.Get(AxSineMechanism.propertyIndex_PeriodStart).GetEffectiveValue();
    var periodEnd = this.properties.Get(AxSineMechanism.propertyIndex_PeriodEnd).GetEffectiveValue();
    var amplitudeMin = this.properties.Get(AxSineMechanism.propertyIndex_AmplitudeMin).GetEffectiveValue();
    var amplitudeMax = this.properties.Get(AxSineMechanism.propertyIndex_AmplitudeMax).GetEffectiveValue();

    var angle = AxMath.Pi2 * (argument - periodStart) / (periodEnd - periodStart);

    this.parameter.value = amplitudeMin + (amplitudeMax - amplitudeMin) * (AxMath.Sin(angle) + 1.0) / 2.0;

    return true;
};