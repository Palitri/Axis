#pragma once

#include "..\..\..\AxGlobals.h"

#include "..\AxScene.h"

class AXDLLCLASS AxNativeJSScene :
	public AxScene
{
private:
	static const char *signature;

public:
	static const char *formatName;

	AxNativeJSScene(void);
	~AxNativeJSScene(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

