#pragma once

#include "Base\AxEntityDispatcher.h"

class AxStandardEntityDispatcher :
	public AxEntityDispatcher
{
public:
	AxStandardEntityDispatcher(void);
	~AxStandardEntityDispatcher(void);

	virtual AxEntity *CreateEntity(Axis *context, int typeId);
};

