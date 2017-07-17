#pragma once

#include "..\..\AxGlobals.h"

#include "..\AxDeviceShader.h"

#include "..\..\Tools\AxList.h"

struct AxDeviceIndependentMaterialShadingOperation
{
	AxShadingOperation shadingOp;
	AxColorOperation colorOp;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxDeviceIndependentMaterialShadingOperation>;

class AXDLLCLASS AxDeviceIndependentShader :
	public AxDeviceShader
{
public:
	AxList<AxDeviceIndependentMaterialShadingOperation> shadingOps;

	AxDeviceIndependentShader(void);
	virtual ~AxDeviceIndependentShader(void);

	virtual int GetOperationCount();
	virtual void AddOperation(AxShadingOperation shadingOp, AxColorOperation colorOp);
	virtual void GetOperation(int index, AxShadingOperation &shadingOp, AxColorOperation &colorOp);
	virtual void SetOperation(int index, AxShadingOperation shadingOp, AxColorOperation colorOp);
	virtual void ClearOperations();

	virtual int GetParameterAddress(AxString parameterName);
	virtual void SetParameter(int address, void *value);

	virtual bool Update();

	int IndexOf(AxShadingOperation shadingOp);
};

