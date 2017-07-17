#pragma once

#include "..\..\..\..\AxGlobals.h"

class Soras;

typedef bool (*SrCullMode)(Soras *soras);

class AXDLLCLASS SrCullModes
{
public:
    static bool None(Soras *soras);
    static bool CounterClockWise(Soras *soras);
	static bool ClockWise(Soras *soras);
};