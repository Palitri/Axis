#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\..\AxString.h"
#include "..\..\AxList.h"

#include "AxWebSocketsExchangeConnection.h"

class AXDLLCLASS AxWebSocketsExchangeChannel :
	public AxList<AxWebSocketsExchangeConnection*>
{
public:
	AxString id;

	AxWebSocketsExchangeChannel(void);
	virtual ~AxWebSocketsExchangeChannel(void);
};