//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxRandomPool.h"

#include "AxMath.h"

AxRandomPool::AxRandomPool(void)
{
}


AxRandomPool::AxRandomPool(int count)
{
	this->Generate(count);
}

AxRandomPool::~AxRandomPool(void)
{
}


void AxRandomPool::Generate(int count)
{
	this->pool.Clear();
	this->pool.EnsureCapacity(count);

	for (int i = 0; i < count; i++)
		this->pool.Add(i);
}

void AxRandomPool::Put(int value)
{
	this->pool.Add(value);
}

int AxRandomPool::Pull()
{
	if (this->pool.count == 0)
		return 0;

	int index = AxMath::Random(this->pool.count);
	this->pool.count--;
	int result = this->pool.values[index];
	this->pool.values[index] = this->pool.values[this->pool.count];
	
	return result;
}

bool AxRandomPool::Contains(int value)
{
	return this->pool.IndexOf(value) != -1;
}

bool AxRandomPool::Remove(int value)
{
	int index = this->pool.IndexOf(value);
	if (index == -1)
		return false;

	this->pool.count--;
	int result = this->pool.values[index];
	this->pool.values[index] = this->pool.values[this->pool.count];

	return true;
}
