#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxWebRequest.h"

#include "..\..\AxString.h"
#include "..\..\Streams\AxMemoryStream.h"

class AXDLLCLASS AxWebResponse
{
public:
	AxWebRequest *request;
	
	AxString headerCode, headerAppendix, dataType;
	AxMemoryStream *data;
	bool newSession;	

	AxWebResponse(AxWebRequest *request);
	~AxWebResponse(void);
};

