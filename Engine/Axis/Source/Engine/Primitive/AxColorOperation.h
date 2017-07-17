#pragma once

#include "..\..\AxGlobals.h"

enum AxColorOperation
{
    AxColorOperation_None = 0,
    AxColorOperation_Set = 1,
    AxColorOperation_Blend = 2,
    AxColorOperation_Add = 3,
    AxColorOperation_Subtract = 4,
    AxColorOperation_Modulate = 5,
    AxColorOperation_Lighter = 6,
    AxColorOperation_Darker = 7,
    AxColorOperation_AlphaBlend = 8,
    AxColorOperation_AlphaAdd = 9,
    AxColorOperation_AlphaSubtract = 10,
    AxColorOperation_AlphaModulate = 11
};