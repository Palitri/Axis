#pragma once

#include "..\..\..\..\AxGlobals.h"

class AXDLLCLASS SrARGBInt
{
public:
    int b, g, r, a;

    SrARGBInt(void)
	{
	};

    SrARGBInt(int a, int r, int g, int b)
    {
        this->b = b;
        this->g = g;
        this->r = r;
        this->a = a;
    };
};