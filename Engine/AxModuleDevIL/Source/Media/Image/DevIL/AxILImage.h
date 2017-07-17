#pragma once

#include "..\..\..\AxisEngine.h"

class AxILImage :
	public AxImage
{
private:
	class Initialization
	{
	public:
		Initialization(void);
	};

	static Initialization initialization;

public:
	AxILImage(void);
	~AxILImage(void);

	bool Load(AxStream &source);
	bool Save(AxStream &destination, const char *format);
};

