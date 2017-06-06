/**
 * Createa a new timer.
 * Provided time measuring, convenient for continuously occuring events such as in a real time application
 * @constructor
 * @returns {AxTimer}
 */
function AxTimer()
{
    this.timeAnchor = 0;
    this.lastSecondTime = 0.0;
    this.lastSecondTicks = 0;
    this.tickOutTime = 0.0;

    this.time = 0.0;
    this.actualTime = 0.0;
    this.elapsedTime = 0.0;
    this.elapsedActualTime = 0.0;
    this.ticksPerSecond = 0.0;
    this.ticksLastSecond = 0.0;
    this.speed = 0.0;

    this.ticksTotal = 0;
        
    this.Reset();
}


/**
 * Resets the timer
 */
AxTimer.prototype.Reset = function()
{
    this.time = 0.0;
    this.actualTime = 0.0;
    this.elapsedTime = 0.0;
    this.elapsedActualTime = 0.0;
    this.ticksPerSecond = 0.0;
    this.ticksLastSecond = 0.0;
    this.speed = 1.0;
    this.ticksTotal = 0;

    this.lastSecondTicks = 0;
    this.lastSecondTime = 0.0;

    this.tickOutTime = 0.0;

    this.timeAnchor = AxTimer.GetAnchor();
};

/**
 * Performs time measuring routines
 * Meant to be called at each loop of a continuously occurring event, so as to provide information about the timing of the event
 */
AxTimer.prototype.Tick = function()
{
    this.actualTime = AxTimer.GetAnchorTime(this.timeAnchor);
    this.time = this.actualTime * this.speed + this.tickOutTime;
    this.elapsedTime += this.time;
    this.elapsedActualTime += this.actualTime;
    this.ticksPerSecond = 1.0 / this.actualTime;
    this.ticksTotal++;
    this.lastSecondTicks++;
    this.lastSecondTime += this.actualTime;
    if (this.lastSecondTime >= 1.0)
    {
        this.ticksLastSecond = this.lastSecondTicks / this.lastSecondTime;
        this.lastSecondTicks = 0;
        this.lastSecondTime = 0.0;
    }
        
    this.tickOutTime = 0.0;

    this.timeAnchor = AxTimer.GetAnchor();
};

/**
 * Ticks out a given amount of time, forcing the timer to appear as if the given amount of time has passed
 * @param {Number} time The time in seconds to tick out
 */
AxTimer.prototype.TickOut = function(time)
{
    this.tickOutTime = time;
};



// Static methods



/**
 * Gets the frequency of the timer. The timer's frequency is its precicion and is measured in ticks per second
 * @return {Integer} The frequency of the timer, denoting its precision
 */
AxTimer.GetFrequency = function()
{
    return AxPlatformUtils.GetPerformanceCounterFrequency();
};

/**
 * Gets the current time stamp, effectively creating an anchor which can be used later to measure the time spent between the measurement and the anchoring
 * @return {Integer} The current time stamp
 */
AxTimer.GetAnchor = function()
{
    return AxPlatformUtils.GetPerformanceCounter();
};

/**
 * Gets the time spent between the time at which the anchor was taken and the current moment of measuring. The returned value is in seconds
 * @param {Integer} anchor A time anchor, taken by AxTimer.GetAnchor
 * @return {Number} The time in seconds, spent between the anchor was taken and the current time.
 */
AxTimer.GetAnchorTime = function(anchor)
{
    return (AxTimer.GetAnchor() - anchor) / AxTimer.GetFrequency();
};
