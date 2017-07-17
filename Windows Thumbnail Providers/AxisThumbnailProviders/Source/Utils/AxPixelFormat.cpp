//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2014  Ivan Donev
//
//	This library is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License version 3.0 as published by the Free Software Foundation
//	This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//	See the GNU LGPL v3.0 for more details. You should have received a copy of the GNU LGPL v3.0 along with this library. If not, see <http://www.gnu.org/licenses/>.
//
//	For help and documentation on the engine, visit <http://morroworks.com>
//	You're very welcome to post your questions and get support at the community page!
//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AxPixelFormat.h"

AxPixelFormat::AxPixelFormat(void)
{
}

AxPixelFormat::AxPixelFormat(int numChannels, int sizeInBytes, unsigned char channel0BitCount, unsigned char channel1BitCount, unsigned char channel2BitCount, unsigned char channel3BitCount, unsigned char channel0Id, unsigned char channel1Id, unsigned char channel2Id, unsigned char channel3Id)
{
	this->numChannels = numChannels;
	this->sizeInBytes = sizeInBytes;
	this->channelBitCount[0] = channel0BitCount;
	this->channelBitCount[1] = channel1BitCount;
	this->channelBitCount[2] = channel2BitCount;
	this->channelBitCount[3] = channel3BitCount;
	this->channelId[0] = channel0Id;
	this->channelId[1] = channel1Id;
	this->channelId[2] = channel2Id;
	this->channelId[3] = channel3Id;
}

AxPixelFormat::~AxPixelFormat(void)
{
}

bool AxPixelFormat::Equals(AxPixelFormat &pixelFormat)
{
	return (this->numChannels == pixelFormat.numChannels) &&
		(this->sizeInBytes == pixelFormat.sizeInBytes) &&
		(this->channelBitCount[0] == pixelFormat.channelBitCount[0]) &&
		(this->channelBitCount[1] == pixelFormat.channelBitCount[1]) &&
		(this->channelBitCount[2] == pixelFormat.channelBitCount[2]) &&
		(this->channelBitCount[3] == pixelFormat.channelBitCount[3]) &&
		(this->channelId[0] == pixelFormat.channelId[0]) &&
		(this->channelId[1] == pixelFormat.channelId[1]) &&
		(this->channelId[2] == pixelFormat.channelId[2]) &&
		(this->channelId[3] == pixelFormat.channelId[3]);
}

int AxPixelFormat::FindChannelIdIndex(unsigned char channelId)
{
	for (int channelIndex = 0; channelIndex < this->numChannels; channelIndex++)
		if (this->channelId[channelIndex] == channelId)
			return channelIndex;

	return -1;
}

int AxPixelFormat::FindChannelIdBitOffset(unsigned char channelId)
{
	int offset = 0;
	for (int channelIndex = 0; channelIndex < this->numChannels; channelIndex++)
	{
		if (this->channelId[channelIndex] == channelId)
			return offset;

		offset += this->channelBitCount[channelIndex];
	}

	return -1;
}

void AxPixelFormat::GetRGBAValues(unsigned int *result, unsigned int pixelValue)
{
	for (int channelIndex = 0; channelIndex < this->numChannels; channelIndex++)
	{
		int channelBitCount = this->channelBitCount[channelIndex];
		int channelMaxValue = (1 << channelBitCount) - 1;

		result[this->channelId[channelIndex]] = pixelValue & channelMaxValue;

		pixelValue >>= channelBitCount;
	}
}

void AxPixelFormat::GetRGBAUnitValues(float *result, unsigned int pixelValue)
{
	for (int channelIndex = 0; channelIndex < this->numChannels; channelIndex++)
	{
		int channelBitCount = this->channelBitCount[channelIndex];
		int channelMaxValue = (1 << channelBitCount) - 1;

		result[this->channelId[channelIndex]] = (float)(pixelValue & channelMaxValue) / (float)channelMaxValue;

		pixelValue >>= channelBitCount;
	}
}

void AxPixelFormat::GetChannelsValues(unsigned int *result, unsigned int pixelValue)
{
	for (int channelIndex = 0; channelIndex < this->numChannels; channelIndex++)
	{
		int channelBitCount = this->channelBitCount[channelIndex];
		int channelMaxValue = (1 << channelBitCount) - 1;

		result[channelIndex] = pixelValue & channelMaxValue;

		pixelValue >>= channelBitCount;
	}
}

void AxPixelFormat::GetChannelsUnitValues(float *result, unsigned int pixelValue)
{
	for (int channelIndex = 0; channelIndex < this->numChannels; channelIndex++)
	{
		int channelBitCount = this->channelBitCount[channelIndex];
		int channelMaxValue = (1 << channelBitCount) - 1;

		result[channelIndex] = (float)(pixelValue & channelMaxValue) / (float)channelMaxValue;

		pixelValue >>= channelBitCount;
	}
}

unsigned int AxPixelFormat::SetRGBAValues(unsigned int *rgbaValues)
{
	unsigned int result = 0;

	for (int channelIndex = this->numChannels - 1; channelIndex >= 0 ; channelIndex--)
	{
		int channelBitCount = this->channelBitCount[channelIndex];

		result <<= channelBitCount;

		result |= rgbaValues[this->channelId[channelIndex]];
	}

	return result;
}

unsigned int AxPixelFormat::SetRGBAUnitValues(float *channelValues)
{
	unsigned int result = 0;

	for (int channelIndex = this->numChannels - 1; channelIndex >= 0 ; channelIndex--)
	{
		int channelBitCount = this->channelBitCount[channelIndex];
		int channelMaxValue = (1 << channelBitCount) - 1;

		result <<= channelBitCount;

		result |= (unsigned int)(channelValues[this->channelId[channelIndex]] * channelMaxValue);
	}

	return result;
}

unsigned int AxPixelFormat::SetChannelValues(unsigned int *channelValues)
{
	unsigned int result = 0;

	for (int channelIndex = this->numChannels - 1; channelIndex >= 0 ; channelIndex--)
	{
		int channelBitCount = this->channelBitCount[channelIndex];

		result <<= channelBitCount;

		result |= channelValues[channelIndex];
	}

	return result;
}

unsigned int AxPixelFormat::SetChannelUnitValues(float *rgbaValues)
{
	unsigned int result = 0;

	for (int channelIndex = this->numChannels - 1; channelIndex >= 0 ; channelIndex--)
	{
		int channelBitCount = this->channelBitCount[channelIndex];
		int channelMaxValue = (1 << channelBitCount) - 1;

		result <<= channelBitCount;

		result |= (unsigned int)(rgbaValues[channelIndex] * channelMaxValue);
	}

	return result;
}


void AxPixelFormat::RGBToHSL(float* result, float *rgbValues)
{
}

void AxPixelFormat::HSLToRGB(float* result, float *hlsValues)
{
	AxPixelFormat::HueToRGB(result, hlsValues[0]);

	float s = hlsValues[1];
	result[0] = 0.5f + (result[0] - 0.5f) * s;
	result[1] = 0.5f + (result[1] - 0.5f) * s;
	result[2] = 0.5f + (result[2] - 0.5f) * s;

	float l = hlsValues[2];
	if (l > 0.5f)
	{
		l = (l - 0.5f) * 2.0f;
		result[0] += (1.0f - result[0]) * l;
		result[1] += (1.0f - result[1]) * l;
		result[2] += (1.0f - result[2]) * l;
	}
	else
	{
		l *= 2.0f;
		result[0] *= l;
		result[1] *= l;
		result[2] *= l;
	}
}

void AxPixelFormat::HueToRGB(float* result, float hue)
{
	const float hueKeypoints[] = 
	{
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f
	};

	float normalized = hue - (int)hue;
	float keyPoint = normalized * 6.0f;
	float keyPointTransition = keyPoint - (int)keyPoint;

	int keyPointIndex1 = (int)keyPoint;
	int keyPointIndex2 = (keyPointIndex1 + 1) % 6;

	keyPointIndex1 *= 3;
	keyPointIndex2 *= 3;
	
	result[0] = hueKeypoints[keyPointIndex1] + (hueKeypoints[keyPointIndex2] - hueKeypoints[keyPointIndex1]) * keyPointTransition;
	result[1] = hueKeypoints[keyPointIndex1 + 1] + (hueKeypoints[keyPointIndex2 + 1] - hueKeypoints[keyPointIndex1 + 1]) * keyPointTransition;
	result[2] = hueKeypoints[keyPointIndex1 + 2] + (hueKeypoints[keyPointIndex2 + 2] - hueKeypoints[keyPointIndex1 + 2]) * keyPointTransition;
}
