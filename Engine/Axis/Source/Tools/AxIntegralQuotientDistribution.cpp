//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxIntegralQuotientDistribution.h"


// Represents a dithered integral distribution. Can be used for tasks of dispersing the error of integral division among the quotiens, such as Bresenham's line algorithm
AxIntegralQuotientDistribution::AxIntegralQuotientDistribution(void)
{
}

AxIntegralQuotientDistribution::AxIntegralQuotientDistribution(int dividend, int divisor)
{
	this->Setup(dividend, divisor);
}


AxIntegralQuotientDistribution::~AxIntegralQuotientDistribution(void)
{
}

void AxIntegralQuotientDistribution::Setup(int dividend, int divisor)
{
	this->dividend = dividend;
	this->divisor = divisor;

	this->integralQuotient = dividend / divisor;
    this->integralModulus = dividend % divisor;
    this->integralError = divisor >> 1;
}

int AxIntegralQuotientDistribution::PullSequential()
{
    this->integralError -= this->integralModulus;
    if (this->integralError < 0)
    {
        this->integralError += this->divisor;
        return this->integralQuotient + 1;
    }

	return this->integralQuotient;
}