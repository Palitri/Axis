#pragma once

#include "..\..\..\..\AxGlobals.h"

class AXDLLCLASS SrARGB
{
public:
    unsigned char b, g, r, a;

    SrARGB(void)
	{
	};

	SrARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
    {
        this->b = b;
        this->g = g;
        this->r = r;
        this->a = a;
    };
};