#pragma once

#include "..\..\AxGlobals.h"

// TODO: Consider renaming to AxColorFormat. ColorFormat does sound more intuitive, though on the other hand, the class does contain information not only on how color is coded (RGB, BGR, ARGB and etc.) but also how a pixel is stored (bits per channel, bytes per pixel)
class AXDLLCLASS AxPixelFormat
{
public:
	static const unsigned char ChannelsMax = 4;

	static const unsigned char ChannelIdRed		= 0;
	static const unsigned char ChannelIdGreen	= 1;
	static const unsigned char ChannelIdBlue	= 2;
	static const unsigned char ChannelIdAlpha	= 3;

	static void RGBToHSL(float* result, float *rgbValues);
	// Converts an HSL color to RGB color
	// H holds the hue value, which wraps over the interval [0, 1). The interval starts with red and goes through the color spectrum available over RGB and ends with read again
	static void HSLToRGB(float* result, float *hlsValues);
	static void HueToRGB(float* result, float hue);

	int numChannels, sizeInBytes;
	unsigned char channelBitCount[4], channelId[4];

	AxPixelFormat(void);
	AxPixelFormat(int numChannels, int sizeInBytes, unsigned char channel0BitCount, unsigned char channel1BitCount, unsigned char channel2BitCount, unsigned char channel3BitCount, unsigned char channel0Id, unsigned char channel1Id, unsigned char channel2Id, unsigned char channel3Id);
	~AxPixelFormat(void);

	bool Equals(AxPixelFormat &pixelFormat);
	int FindChannelIdIndex(unsigned char channelId);
	int FindChannelIdBitOffset(unsigned char channelId);
	
	// TODO: Remove the RGBA methods and replace them with methods for returning the pixel in a user-specified format, rather than RGBA. Having these RGBA methods supposes a kind of standardizing the RGBA format, which is not what we want

	// Returns the raw values of the individual color channels of the given pixelValue in an RGBA sequence
	// The returned values are in RGBA sequence and in the original raw format - depending on the format, not all RGBA channels may be present and each channel may be of different bit count
	void GetRGBAValues(unsigned int *result, unsigned int pixelValue);
	// Returns the unit values of the individual color channels of the given pixelValue in an RGBA sequence
	// The returned values are in RGBA sequence and in floating point unit format - that is, each channel value is normalized in the range [0, 1]
	void GetRGBAUnitValues(float *result, unsigned int pixelValue);

	// Returns the raw values of the individual color channels of the given pixelValue in the original channel sequence of the pixel format
	// The returned values are in the format's original channels sequence and in the original raw format - depending on the format each channel may be of different bit count
	void GetChannelsValues(unsigned int *result, unsigned int pixelValue);
	// Returns the unit values of the individual color channels of the given pixelValue in the original channel sequence of the pixel format
	// The returned values are in the format's original channels sequence and in floating point unit format - that is, each value is normalized in the range [0, 1]
	void GetChannelsUnitValues(float *result, unsigned int pixelValue);

	// Returns a whole pixel value, constructed from the given raw RGBA values
	// The given values must be in an RGBA sequence and each in the original bit count of the format.
	unsigned int SetRGBAValues(unsigned int *rgbaValues);
	// Returns a whole pixel value, constructed from the given RGBA unit values
	// The given values must be in an RGBA sequence and each normalized in the range [0, 1]
	unsigned int SetRGBAUnitValues(float *rgbaValues);

	// Returns a whole pixel value, constructed from the given individual color channels raw values
	// The given values must be in the original channel sequence of the format and each of the original bit count of the format
	unsigned int SetChannelValues(unsigned int *channelValues);
	// Returns a whole pixel value, constructed from the given normalized floating point values
	// The given values must be in the original channel sequence of the format and each normalized in the range [0, 1]
	unsigned int SetChannelUnitValues(float *channelValues);

};