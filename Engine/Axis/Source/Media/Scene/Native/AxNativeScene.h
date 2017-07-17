#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxScene.h"

class AXDLLCLASS AxNativeScene :
	public AxScene
{
public:
	static const char *formatName;

	AxNativeScene(void);
	~AxNativeScene(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

