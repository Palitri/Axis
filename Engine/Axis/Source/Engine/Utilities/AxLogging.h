#pragma once

#include "..\..\Tools\AxString.h"

class AXDLLCLASS AxLogging
{
public:
	static const int MessageType_Info			= 0;
	static const int MessageType_Warning		= 1;
	static const int MessageType_Error			= 2;
	static const int MessageType_CriticalError	= 3;

	AxLogging(void);
	~AxLogging(void);

	static void AddMessage(const AxString &message, int messageType);
};

