#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "SrVector3.h"
#include "SrARGBInt.h"

class AXDLLCLASS SrLight
{
public:
    SrVector3 position;
    SrARGBInt color;
    float range;

    SrLight()
    {
        this->color = SrARGBInt(0xff, 0xff, 0xff, 0xff);
        this->range = 500;
    };

	~SrLight(void)
	{
	};
};