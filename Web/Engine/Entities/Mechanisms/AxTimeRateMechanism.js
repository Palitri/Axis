/**
 * Creates a new time rate mechanism.
 * This mechanism yields the current time rate of the scene
 * @param {Axis} context The context to use for the mechanism
 * @constructor
 */
function AxTimeRateMechanism(context)
{
    AxMechanism.call(this);

    this.context = context;

    this.typeId = AxTimeRateMechanism.classId;

    this.parameter.SetFloat(this.context.timer.speed);
}

AxTimeRateMechanism.prototype = Object.create(AxMechanism.prototype);


AxTimeRateMechanism.classId = AxMechanism.classId | 111;


AxTimeRateMechanism.prototype.Dispose = function()
{
};

/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxTimeRateMechanism.prototype.Process = function(deltaTime)
{
    this.parameter.SetFloat(this.context.timer.speed);

    return true;
};
