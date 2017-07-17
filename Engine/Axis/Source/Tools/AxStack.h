#pragma once

#include "..\AxGlobals.h"

#include "AxList.h"

template <typename T> class AXDLLCLASS AxStack
	: public AxList<T>
{
public:

	AxStack(int capacity = 8)
		: AxList(capacity)
	{
	};

	T& Push(T value)
	{
		return this->Add(value);
	};

	T& Pop()
	{
		return this->values[--this->count];
	};

	T& Peek()
	{
		return this->values[this->count - 1];
	};
};
