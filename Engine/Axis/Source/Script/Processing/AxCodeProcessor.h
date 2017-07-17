#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxList.h"
#include "..\..\Tools\Streams\AxStream.h"

class AxCodeProcessor;

typedef void (*AxCodeProcessorOperation) (AxCodeProcessor *context);

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxCodeProcessorOperation>;


class AXDLLCLASS AxCodeProcessor
{
private:
	static void Operation_JMP						(AxCodeProcessor *context);
	static void Operation_JZ						(AxCodeProcessor *context);
	static void Operation_JNZ						(AxCodeProcessor *context);
	static void Operation_Call						(AxCodeProcessor *context);
	static void Operation_RET						(AxCodeProcessor *context);
	static void Operation_CallOut					(AxCodeProcessor *context);
	
	// Integer
	static void Operation_IToF						(AxCodeProcessor *context);
	static void Operation_IPushAddress				(AxCodeProcessor *context);
	static void Operation_IPushIndexedAddress		(AxCodeProcessor *context);
	static void Operation_IPushAddressValue			(AxCodeProcessor *context);
	static void Operation_IPushIndexedAddressValue	(AxCodeProcessor *context);
	// Evaluation
	static void Operation_INot						(AxCodeProcessor *context);
	static void Operation_IAnd						(AxCodeProcessor *context);
	static void Operation_IOr						(AxCodeProcessor *context);
	static void Operation_IXor						(AxCodeProcessor *context);
	static void Operation_IShl						(AxCodeProcessor *context);
	static void Operation_IShr						(AxCodeProcessor *context);
	static void Operation_IDiv						(AxCodeProcessor *context);
	static void Operation_IMod						(AxCodeProcessor *context);
	static void Operation_IMul						(AxCodeProcessor *context);
	static void Operation_ISub						(AxCodeProcessor *context);
	static void Operation_IAdd						(AxCodeProcessor *context);
	static void Operation_ILogicalNot				(AxCodeProcessor *context);
	static void Operation_ILogicalAnd				(AxCodeProcessor *context);
	static void Operation_ILogicalOr				(AxCodeProcessor *context);
	static void Operation_IGreater					(AxCodeProcessor *context);
	static void Operation_ILesser					(AxCodeProcessor *context);
	static void Operation_IEqual					(AxCodeProcessor *context);
	static void Operation_INotEqual					(AxCodeProcessor *context);
	static void Operation_IGreaterEqual				(AxCodeProcessor *context);
	static void Operation_ILesserEqual				(AxCodeProcessor *context);
	// Assignment
	static void Operation_ISet						(AxCodeProcessor *context);
	static void Operation_IAddAdd					(AxCodeProcessor *context);
	static void Operation_ISubSub					(AxCodeProcessor *context);
	static void Operation_IAddEquals				(AxCodeProcessor *context);
	static void Operation_ISubEquals				(AxCodeProcessor *context);
	static void Operation_IMulEquals				(AxCodeProcessor *context);
	static void Operation_IModEquals				(AxCodeProcessor *context);
	static void Operation_IDivEquals				(AxCodeProcessor *context);
	static void Operation_IXorEquals				(AxCodeProcessor *context);
	static void Operation_IOrEquals					(AxCodeProcessor *context);
	static void Operation_IAndEquals				(AxCodeProcessor *context);
	
	// Floating point
	static void Operation_FToI						(AxCodeProcessor *context);
	static void Operation_FPushAddress				(AxCodeProcessor *context);
	static void Operation_FPushIndexedAddress		(AxCodeProcessor *context);
	static void Operation_FPushAddressValue			(AxCodeProcessor *context);
	static void Operation_FPushIndexedAddressValue	(AxCodeProcessor *context);
	// Evaluations
	static void Operation_FGreater					(AxCodeProcessor *context);
	static void Operation_FLesser					(AxCodeProcessor *context);
	static void Operation_FEqual					(AxCodeProcessor *context);
	static void Operation_FNotEqual					(AxCodeProcessor *context);
	static void Operation_FGreaterEqual				(AxCodeProcessor *context);
	static void Operation_FLesserEqual				(AxCodeProcessor *context);
	static void Operation_FDiv						(AxCodeProcessor *context);
	static void Operation_FMul						(AxCodeProcessor *context);
	static void Operation_FSub						(AxCodeProcessor *context);
	static void Operation_FAdd						(AxCodeProcessor *context);
	// Assignment
	static void Operation_FSet						(AxCodeProcessor *context);
	static void Operation_FAddEquals				(AxCodeProcessor *context);
	static void Operation_FSubEquals				(AxCodeProcessor *context);
	static void Operation_FMulEquals				(AxCodeProcessor *context);
	static void Operation_FDivEquals				(AxCodeProcessor *context);


public: // public only for debugging. make private
	void *stack;
	int stackCapacity, stackPos;

	void EnsureStackCapacity(int capacity);
	
	void PushInt(int value);
	int PopInt();

	void PushFloat(float value);
	float PopFloat();

	int ReadAddressIntValue(int address);
	void WriteAddressIntValue(int address, int value);

	float ReadAddressFloatValue(int address);
	void WriteAddressFloatValue(int address, float value);

public:
	int 
		operationId_JMP,
		operationId_JZ,
		operationId_JNZ,
		operationId_Call,
		operationId_RET,
		operationId_CallOut,

		operationId_IToF,
		operationId_IPushAddress,
		operationId_IPushIndexedAddress,
		operationId_IPushAddressValue,
		operationId_IPushIndexedAddressValue,

		operationId_INot,		
		operationId_IAnd,		
		operationId_IOr,			
		operationId_IXor,		
		operationId_IShl,			
		operationId_IShr,			
		operationId_IDiv,			
		operationId_IMul,			
		operationId_IMod,			
		operationId_ISub,			
		operationId_IAdd,			
		operationId_ILogicalNot,		
		operationId_ILogicalAnd,		
		operationId_ILogicalOr,		
		operationId_IGreater,		
		operationId_ILesser,		
		operationId_IEqual,			
		operationId_INotEqual,		
		operationId_IGreaterEqual,	
		operationId_ILesserEqual,	
	
		operationId_ISet,			
		operationId_IAddAdd,		
		operationId_ISubSub,		
		operationId_IAddEquals,		
		operationId_ISubEquals,		
		operationId_IModEquals,
		operationId_IMulEquals,		
		operationId_IDivEquals,		
		operationId_IXorEquals,		
		operationId_IOrEquals,		
		operationId_IAndEquals,

		operationId_FToI,
		operationId_FPushAddress,
		operationId_FPushIndexedAddress,
		operationId_FPushAddressValue,
		operationId_FPushIndexedAddressValue,

		operationId_FDiv,			
		operationId_FMul,			
		operationId_FSub,			
		operationId_FAdd,			
		operationId_FGreater,		
		operationId_FLesser,		
		operationId_FEqual,			
		operationId_FNotEqual,		
		operationId_FGreaterEqual,	
		operationId_FLesserEqual,	
	
		operationId_FSet,			
		operationId_FAddEquals,		
		operationId_FSubEquals,		
		operationId_FMulEquals,		
		operationId_FDivEquals;

	AxList<AxCodeProcessorOperation> operations;

	AxStream *memory;


	AxCodeProcessor(void);
	~AxCodeProcessor(void);

	// Adds an operation to the processor the returns the id given to that operation
	int AddOperation(AxCodeProcessorOperation operation);

	// Sets the memory which holds the code to be processed
	void SetMemory(AxStream *memory);
	// Runs the code from memory
	void Process();

	// Called in a Callout operation, used for communication between script and native code
	virtual void OnCallout(int calloutId);
};

