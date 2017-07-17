//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxMath.h"

#include <stdlib.h>
#include <time.h>
#include <cmath>
#include <float.h>
#include <limits.h>

const float AxMath::Pi = 3.14159265358979f;
const double AxMath::Pi_d = 3.141592653589793238462;

// a.k.a. Tau
const float AxMath::Pi2 = AxMath::Pi * 2.0f;
const double AxMath::Pi2_d = AxMath::Pi_d * 2.0;

const float AxMath::PiHalf = AxMath::Pi / 2.0f;
const double AxMath::PiHalf_d = AxMath::Pi_d / 2.0;

// a.k.a The golden ratio
const float AxMath::Phi = 1.61803398874989f;
const double AxMath::Phi_d = 1.6180339887498948482;

// Euler's number
const float AxMath::E = 2.71828182845904f;
const double AxMath::E_d = 2.7182818284590452353;

const int AxMath::IntMin = INT_MIN;
const int AxMath::IntMax = INT_MAX;

const float AxMath::FloatMin = FLT_MIN;
const float AxMath::FloatMax = FLT_MAX;

const unsigned int AxMath::crcPolynomial_Normal = 0x04C11DB7;
const unsigned int AxMath::crcPolynomial_Reversed = 0xEDB88320;
const unsigned int AxMath::crcPolynomial_ReversedReciprocal = 0x82608EDB;

unsigned int AxMath::crcTable[256];

AxMath::Initialization AxMath::initialization;

AxMath::Initialization::Initialization(void)
{
	AxMath::CRC32GenerateTable(AxMath::crcPolynomial_Reversed);
	AxMath::Randomize();
}

AxMath::AxMath(void)
{
}


AxMath::~AxMath(void)
{
}


float AxMath::Trunc(float x)
{
	return (float)((int)x);
}

double AxMath::Trunc(double x)
{
	return (double)((int)x);
}

float AxMath::Floor(float x)
{
	return floorf(x);
}

double AxMath::Floor(double x)
{
	return floor(x);
}

float AxMath::Ceil(float x)
{
	return ceilf(x);
}

double AxMath::Ceil(double x)
{
	return ceil(x);
}

float AxMath::Round(float x)
{
	return floorf(x + 0.5f);
}

double AxMath::Round(double x)
{
	return floor(x + 0.5);
}

int AxMath::Min(int a, int b)
{
	if (a < b)
		return a;

	return b;
}

long long AxMath::Min(long long a, long long b)
{
	if (a < b)
		return a;

	return b;
}

float AxMath::Min(float a, float b)
{
	if (a < b)
		return a;

	return b;
}

int AxMath::Max(int a, int b)
{
	if (a > b)
		return a;

	return b;
}

long long AxMath::Max(long long a, long long b)
{
	if (a > b)
		return a;

	return b;
}

float AxMath::Max(float a, float b)
{
	if (a > b)
		return a;

	return b;
}

int AxMath::Trim(int x, int min, int max)
{
	if (x < min)
		return min;

	if (x > max)
		return max;

	return x;
}

long long AxMath::Trim(long long x, long long min, long long max)
{
	if (x < min)
		return min;

	if (x > max)
		return max;

	return x;
}

float AxMath::Trim(float x, float min, float max)
{
	if (x < min)
		return min;

	if (x > max)
		return max;

	return x;
}

int AxMath::High(int value)
{
	return value >> 16;
}

int AxMath::Low(int value)
{
	return value & 0xffff;
}

int AxMath::HighLow(int high, int low)
{
	return (high << 16) | (low & 0xffff);
}

int AxMath::Sign(int x)
{
	return x > 0 ? 1 : x < 0 ? -1 : 0;
}

float AxMath::Sign(float x)
{
	return x > 0 ? 1.0f : x < 0 ? -1.0f : 0.0f;
}

int AxMath::Abs(int x)
{
	return x >= 0 ? x : -x;
}

float AxMath::Abs(float x)
{
	return x >= 0 ? x : -x;
}

int AxMath::Power(int base, int exponent)
{
	return (int)pow((float)base, exponent);
}

float AxMath::Power(float base, float exponent)
{
	return powf(base, exponent);
}

double AxMath::Power(double base, double exponent)
{
	return pow(base, exponent);
}

float AxMath::Exp(float x)
{
	return exp(x);
}

double AxMath::Exp(double x)
{
	return exp(x);
}

float AxMath::Sqrt(float x)
{
	return sqrtf(x);
}

double AxMath::Sqrt(double x)
{
	return sqrt(x);
}

// TODO: test against 1 / Sqrt (SSE rsqrtss is faster and the compiler might optimize the code)
float AxMath::InvSqrt(float x)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;
 
    x2 = x * 0.5f;
    y  = x;
    i  = *(long*)&y;						
    i  = 0x5F3759DF - (i >> 1);				
    y  = *(float*)&i;
    y  = y * (threehalfs - (x2 * y * y));	
 
    return y;
}

float AxMath::Log(float base, float x)
{
	return log(x) / log(base);
}

double AxMath::Log(double base, double x)
{
	return log(x) / log(base);
}


float AxMath::Ln(float x)
{
	return log(x);
}

double AxMath::Ln(double x)
{
	return log(x);
}


float AxMath::Lg(float x)
{
	return log10(x);
}

double AxMath::Lg(double x)
{
	return log10(x);
}


float AxMath::Sin(float x)
{
	return sinf(x);
}

double AxMath::Sin(double x)
{
	return sin(x);
}


float AxMath::Cos(float x)
{
	return cosf(x);
}

double AxMath::Cos(double x)
{
	return cos(x);
}


float AxMath::Tan(float x)
{
	return tanf(x);
}

double AxMath::Tan(double x)
{
	return tan(x);
}


float AxMath::Cot(float x)
{
	return 1.0f / tanf(x);
}

double AxMath::Cot(double x)
{
	return 1.0 / tan(x);
}


float AxMath::ArcSin(float x)
{
	return asinf(x);
}

double AxMath::ArcSin(double x)
{
	return asin(x);
}


float AxMath::ArcCos(float x)
{
	return acosf(x);
}

double AxMath::ArcCos(double x)
{
	return acos(x);
}


float AxMath::ArcTan(float x)
{
	return atanf(x);
}

double AxMath::ArcTan(double x)
{
	return atan(x);
}


float AxMath::ArcCot(float x)
{
	return AxMath::Pi / 2.0f - atanf(x);
}

double AxMath::ArcCot(double x)
{
	return AxMath::PiHalf_d - atan(x);
}


float AxMath::ArcTan2(float y, float x)
{
	return atan2f(y, x);
}

double AxMath::ArcTan2(double y, double x)
{
	return atan2(y, x);
}


int AxMath::Random(int n)
{
	return rand() % n;
}

float AxMath::Random()
{
	return (float)rand() / (float)RAND_MAX;
}

void AxMath::Randomize()
{
	srand((unsigned int)time(NULL));
}

void AxMath::CRC32GenerateTable(unsigned int crcPolynomial)
{
	for (int i = 0; i < 256; i++)
	{
		unsigned int remainder = i;
		for(int j = 0; j < 8; j++)
		{
			if (remainder & 1)
			{
				remainder = (remainder >> 1) ^ crcPolynomial;
			}
			else
			{
				remainder >>= 1;
			}
		}

		AxMath::crcTable[i] = remainder;
	}
}

unsigned int AxMath::CRC32(const void *source, unsigned int size, unsigned int seed)
{
    unsigned int result = ~seed;

    for (unsigned int i = 0; i < size; i++)
        result = AxMath::crcTable[((result & 0xFF) ^ ((unsigned char*)source)[i])] ^ (result >> 8);
            
    return ~result;
}