#pragma once

#include "..\..\..\..\AxGlobals.h"

#include "..\..\..\..\Tools\AxMath.h"

struct SrVector3
{
    float x, y, z;

	static void Normalize(SrVector3 &vector)
	{
		float k = AxMath::Sqrt(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);
		if (k != 0.0f)
		{
			k = 1.0f / k;

			vector.x *= k;
			vector.y *= k;
			vector.z *= k;
		}	
	};
};
