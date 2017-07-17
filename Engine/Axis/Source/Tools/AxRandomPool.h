#pragma once

#include "..\AxGlobals.h"

#include "AxList.h"

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<int>;

class AXDLLCLASS AxRandomPool
{
public:
	AxList<int> pool;

	AxRandomPool(void);
	AxRandomPool(int count);
	~AxRandomPool(void);

	void Generate(int count);

	void Put(int value);
	int Pull();

	bool Contains(int value);
	bool Remove(int value);
};

