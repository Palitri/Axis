#pragma once

#include "..\AxGlobals.h"

class AXDLLCLASS AxMath
{
private:
	class AXDLLCLASS Initialization
	{
	public:
		Initialization(void);
	};

	static Initialization initialization;

public:
	static const float Pi;
	static const double Pi_d;

	static const float Pi2;			// a.k.a. Tau
	static const double Pi2_d;		// a.k.a. Tau

	static const float PiHalf;
	static const double PiHalf_d;

	static const float Phi;			// a.k.a The golden ratio
	static const double Phi_d;		// a.k.a The golden ratio

	static const float E;
	static const double E_d;

	static const int IntMin;
	static const int IntMax;
	static const float FloatMin;
	static const float FloatMax;

	static const int SHA1SizeBytes;

	static const unsigned int crcPolynomial_Normal;
	static const unsigned int crcPolynomial_Reversed;
	static const unsigned int crcPolynomial_ReversedReciprocal;
	static unsigned int crcTable[256];

	AxMath(void);
	~AxMath(void);

	static float Trunc(float x);
	static double Trunc(double x);

	static float Floor(float x);
	static double Floor(double x);

	static float Ceil(float x);
	static double Ceil(double x);

	static float Round(float x);
	static double Round(double x);

	static int Min(int a, int b);
	static long long Min(long long a, long long b);
	static float Min(float a, float b);
	
	static int Max(int a, int b);
	static long long Max(long long a, long long b);
	static float Max(float a, float b);
	
	static int Trim(int x, int min, int max);
	static long long Trim(long long x, long long int min, long long int max);
	static float Trim(float x, float min, float max);

	static int High(int value);
	static int Low(int value);
	static int HighLow(int high, int low);

	static int Sign(int x);
	static float Sign(float x);
	
	static int Abs(int x);
	static float Abs(float x);

	static int Power(int base, int exponent);
	static float Power(float base, float exponent);
	static double Power(double base, double exponent);

	static float Exp(float x);
	static double Exp(double x);

	static float Sqrt(float x);
	static double Sqrt(double x);

	static float InvSqrt(float x);

	static float Log(float base, float x);
	static double Log(double base, double x);

	static float Ln(float x);
	static double Ln(double x);

	static float Lg(float x);
	static double Lg(double x);

	static float Sin(float x);
	static double Sin(double x);

	static float Cos(float x);
	static double Cos(double x);

	static float Tan(float x);
	static double Tan(double x);

	static float Cot(float x);
	static double Cot(double x);

	static float ArcSin(float x);
	static double ArcSin(double x);

	static float ArcCos(float x);
	static double ArcCos(double x);

	static float ArcTan(float x);
	static double ArcTan(double x);

	static float ArcCot(float x);
	static double ArcCot(double x);

	static float ArcTan2(float y, float x);
	static double ArcTan2(double y, double x);

	static unsigned char ROL(unsigned char value, int bits);
	static unsigned short ROL(unsigned short value, int bits);
	static unsigned unsigned int ROL(unsigned unsigned int value, int bits);
	static unsigned long long ROL(unsigned long long value, int bits);

	static unsigned char ROR(unsigned char value, int bits);
	static unsigned short ROR(unsigned short value, int bits);
	static unsigned int ROR(unsigned int value, int bits);
	static unsigned long long ROR(unsigned long long value, int bits);


	static int Random(int n);
	static float Random();
	static void Randomize();

	static void CRC32GenerateTable(unsigned int crcPolynomial);
	static unsigned int CRC32(const void *source, unsigned int size, unsigned int seed);
	static unsigned int IterativeCRC32Init(unsigned int seed);
	static unsigned int IterativeCRC32Iterate(unsigned int crc16, unsigned char data);

	static unsigned short CRC16(const void *source, unsigned int size, unsigned short seed = 0x1D0F);
	static unsigned short IterativeCRC16Init(unsigned short seed = 0x1D0F);
	static unsigned short IterativeCRC16Iterate(unsigned short crc16, unsigned char data);

	static void SHA1(void *result, const void *data, int size);

};

