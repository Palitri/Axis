/**
 * Represents the format of a Pulse-Code Modulation (PCM) sound
 * @param {Integer} sampleSizeInBytes description
 * @param {Integer} frequency description
 * @param {Integer} channelsCount description
 * @constructor
 */
function AxPCMFormat(sampleSizeInBytes, frequency, channelsCount)
{
    this.sampleSizeInBytes = sampleSizeInBytes;
    this.frequency = frequency;
    this.channelsCount = channelsCount;
}
