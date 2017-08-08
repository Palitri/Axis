/**
 * Represents the format of a Pulse-Code Modulation (PCM) sound
 * @param {Integer} sampleSizeInBytes The size in bytes of a sample. This is 1 for 8-bit sound, 2 for 16-bit sound and so on
 * @param {Integer} frequency description The frequency of the sound in Hz
 * @param {Integer} channelsCount description The number of channels of the sound
 * @constructor
 */
function AxPCMFormat(sampleSizeInBytes, frequency, channelsCount)
{
    this.sampleSizeInBytes = sampleSizeInBytes;
    this.frequency = frequency;
    this.channelsCount = channelsCount;
}
