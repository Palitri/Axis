/**
 * Creates a AxPropertyChangeNotifiedMechanism mechanism
 * The property change notified mechanism processes the AxResource's NotifyPropertiesChange() method on each frame and therefore automatically utilizes the OnPropertiesChanged() and OnPropertyChanged() methods
 * @constructor
 */
function AxPropertyChangeNotifiedMechanism()
{
    AxMechanism.call(this);

    this.typeId = AxPropertyChangeNotifiedMechanism.classId;
}

AxPropertyChangeNotifiedMechanism.prototype = Object.create(AxMechanism.prototype);


AxPropertyChangeNotifiedMechanism.classId = (AxResourceType.Mechanism << 16) | 1;


/**
 * Performs the routines of the mechanism and returns whether the mechanism should continue to be processed the next frame
 * @param {Number} deltaTime The time in milliseconds, between the previous and the current call. Used to perform time based routines
 * @return {Boolean} True, if the mechanism hasn't finished its job and should continue to be processed in the next frame
 */
AxPropertyChangeNotifiedMechanism.prototype.Process = function(deltaTime)
{
    this.NotifyPropertiesChange();

    return true;
};
