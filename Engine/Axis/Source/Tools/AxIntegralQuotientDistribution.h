#pragma once

#include "..\AxGlobals.h"

class AXDLLCLASS AxIntegralQuotientDistribution
{
private:
	int integralQuotient, integralModulus ,integralError;

public:
	int dividend, divisor;

	AxIntegralQuotientDistribution(void);
	AxIntegralQuotientDistribution(int dividend, int divisor);
	~AxIntegralQuotientDistribution(void);

	void Setup(int dividend, int divisor);
	int PullSequential();
};

