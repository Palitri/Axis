//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxGaussianBlurImage.h"

#include "..\..\..\Tools\AxMath.h"

AxGaussianBlurImage::AxGaussianBlurImage(AxImage &result, AxImage &originalImage, int kernelSize, float standardDeviation)
{
	this->kernel = 0;

	this->ComputeKernel(kernelSize, standardDeviation);
	this->NormalizeKernel();

	float *originalFloatImage = this->CreateFloatImage(originalImage);
	float *horizontalBlurFloatImage = new float[originalImage.width * originalImage.height * 4];

	float *horizontalImageSample = horizontalBlurFloatImage;
	for (int y = 0; y < originalImage.height; y++)
	{
		for (int x = 0; x < originalImage.width; x++)
		{
			this->SampleWithKernel(horizontalImageSample, originalFloatImage, originalImage.width, originalImage.height, x, y, 1, 0);
			horizontalImageSample += 4;
		}
	}

	delete[] originalFloatImage;

	result.SetPixelFormat(originalImage.pixelFormat);
	result.SetSize(originalImage.width, originalImage.height, 1);
	float sample[4];
	for (int y = 0; y < originalImage.height; y++)
	{
		for (int x = 0; x < originalImage.width; x++)
		{
			this->SampleWithKernel(sample, horizontalBlurFloatImage, originalImage.width, originalImage.height, x, y, 0, 1);
			result.SetPixel(x, y, 0, result.pixelFormat.SetChannelUnitValues(sample));
		}
	}

	delete[] horizontalBlurFloatImage;
}


AxGaussianBlurImage::~AxGaussianBlurImage(void)
{
	this->ComputeKernel(0, 0.0f);
}

float AxGaussianBlurImage::GaussianDistribution(float x, float standardDeviation)
{
	const float sqrtPi2 = AxMath::Sqrt(AxMath::Pi2);

	return AxMath::Exp(- (x * x) / (2.0f * standardDeviation * standardDeviation)) / (standardDeviation * sqrtPi2);
}

float AxGaussianBlurImage::GaussianDistribution2D(float x, float y, float standardDeviation)
{
	float standardDeviation2 = standardDeviation * standardDeviation;

	return AxMath::Exp(- (x * x + y * y) / (2.0f * standardDeviation2)) / (AxMath::Pi2 * standardDeviation2);
}

void AxGaussianBlurImage::ComputeKernel(int kernelSize, float standardDeviation)
{
	if (this->kernel != 0)
	{
		delete this->kernel;
		this->kernel = 0;
	}

	this->kernelSize = kernelSize;
	this->halfKernelSize = kernelSize / 2;

	if (kernelSize <= 0)
		return;

	this->kernel = new float[this->kernelSize];
	for (int i = 0; i < this->kernelSize; i++)
	{
		float g = this->GaussianDistribution(i - this->kernelSize / 2, standardDeviation);
		this->kernel[i] = g;
	}
}

void AxGaussianBlurImage::NormalizeKernel()
{
	float sum = 0.0f;

	for (int i = 0; i < this->kernelSize; i++)
		sum += this->kernel[i];
	
	for (int i = 0; i < this->kernelSize; i++)
		this->kernel[i] /= sum;
}

void AxGaussianBlurImage::SampleWithKernel(float *result, float *source, int width, int height, int x, int y, int vectorX, int vectorY)
{
	x -= vectorX * this->halfKernelSize;
	y -= vectorY * this->halfKernelSize;

	result[0] = 0.0f;
	result[1] = 0.0f;
	result[2] = 0.0f;
	result[3] = 0.0f;

	for (int i = 0; i < this->kernelSize; i++)
	{
		int px = x + i * vectorX;
		px = (width + px) % width;

		int py = y + i * vectorY;
		py = (height + py) % height;

		float *sample = source + (py * width + px) * 4;

		float kernelValue = this->kernel[i];

		result[0] += sample[0] * kernelValue;
		result[1] += sample[1] * kernelValue;
		result[2] += sample[2] * kernelValue;
		result[3] += sample[3] * kernelValue;
	}
}


float *AxGaussianBlurImage::CreateFloatImage(AxImage &image)
{
	float *result = new float[image.width * image.height * 4];
	float *sample = result;
	for (int y = 0; y < image.height; y++)
	{
		for (int x = 0; x < image.width; x++)
		{
			image.pixelFormat.GetRGBAUnitValues(sample, image.GetPixel(x, y, 0));
			sample += 4;
		}
	}

	return result;
}
