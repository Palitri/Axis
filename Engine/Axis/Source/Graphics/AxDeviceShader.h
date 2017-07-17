#pragma once

#include "..\AxGlobals.h"

#include "..\Tools\AxString.h"

#include "..\Engine\Primitive\AxShadingOperation.h"
#include "..\Engine\Primitive\AxColorOperation.h"
#include "..\Engine\Properties\AxParameterType.h"

class AXDLLCLASS AxDeviceShader
{
public:
	static const int invalidParameterAddress = -1;

	AxDeviceShader(void);
	virtual ~AxDeviceShader(void);

	virtual int GetOperationCount() = 0;
	virtual void AddOperation(AxShadingOperation shadingOp, AxColorOperation colorOp) = 0;
	virtual void GetOperation(int index, AxShadingOperation &shadingOp, AxColorOperation &colorOp) = 0;
	virtual void SetOperation(int index, AxShadingOperation shadingOp, AxColorOperation colorOp) = 0;
	virtual void ClearOperations() = 0;

	virtual int GetParameterAddress(AxString parameterName) = 0;
	virtual void SetParameter(int address, int index, void *value) = 0;

	virtual bool Update(AxString &compilationMessage) = 0;
};

