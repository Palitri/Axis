#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\..\Media\Image\AxImage.h"

#include "..\..\..\Engine\Primitive\AxVector4.h"

class AXDLLCLASS AxGaussianBlurImage
{
private:
	float GaussianDistribution(float x, float standardDeviation);
	float GaussianDistribution2D(float x, float y, float standardDeviation);

	void ComputeKernel(int kernelSize, float standardDeviation);
	void NormalizeKernel();
	void SampleWithKernel(float *result, float *source, int width, int height, int x, int y, int vectorX, int vectorY);

	float *CreateFloatImage(AxImage &image);

	float *kernel;
	int kernelSize, halfKernelSize;

public:
	AxGaussianBlurImage(AxImage &result, AxImage &originalImage, int kernelSize, float standardDeviation);
	virtual ~AxGaussianBlurImage(void);
};

