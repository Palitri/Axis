//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxCodeProcessor.h"

#include "..\..\Tools\AxMem.h"

void AxCodeProcessor::Operation_JMP(AxCodeProcessor *context)
{
    int jumpOffset = context->memory->ReadInt32();
	context->memory->Seek(jumpOffset, StreamSeekMode_Relative);
}

void AxCodeProcessor::Operation_JZ(AxCodeProcessor *context)
{
    int condition = context->PopInt();
    int jumpOffset = context->memory->ReadInt32();
    if (condition == 0)
        context->memory->Seek(jumpOffset, StreamSeekMode_Relative);
}

void AxCodeProcessor::Operation_JNZ(AxCodeProcessor *context)
{
    int condition = context->PopInt();
    int jumpOffset = context->memory->ReadInt32();
    if (condition != 0)
        context->memory->Seek(jumpOffset, StreamSeekMode_Relative);
}

void AxCodeProcessor::Operation_Call(AxCodeProcessor *context)
{
	int currentAddress = (int)context->memory->position;
    int jumpOffset = context->memory->ReadInt32();
    context->PushInt(currentAddress);
    context->memory->Seek(jumpOffset, StreamSeekMode_Relative);
}

void AxCodeProcessor::Operation_CallOut(AxCodeProcessor *context)
{
    //int calloutId = context->memory->ReadInt32();
    int calloutId = context->PopInt();
    context->OnCallout(calloutId);
}

void AxCodeProcessor::Operation_RET(AxCodeProcessor *context)
{
    int returnOffset = context->PopInt();
    context->memory->Seek(returnOffset, StreamSeekMode_Relative);
}

// Integer operations

void AxCodeProcessor::Operation_IToF(AxCodeProcessor *context)
{
	context->PushFloat((float)context->PopInt());
}

void AxCodeProcessor::Operation_IPushAddress(AxCodeProcessor *context)
{
    context->PushInt(context->memory->ReadInt32());
}

void AxCodeProcessor::Operation_IPushIndexedAddress(AxCodeProcessor *context)
{
    int offset = context->PopInt();
    context->PushInt(context->memory->ReadInt32() + offset * 4);
}

void AxCodeProcessor::Operation_IPushAddressValue(AxCodeProcessor *context)
{
    context->PushInt(context->ReadAddressIntValue(context->memory->ReadInt32()));
}

void AxCodeProcessor::Operation_IPushIndexedAddressValue(AxCodeProcessor *context)
{
    int offset = context->PopInt();
    context->PushInt(context->ReadAddressIntValue(context->memory->ReadInt32() + offset * 4));
}


// Evaluation operations

void AxCodeProcessor::Operation_INot(AxCodeProcessor *context)
{
    int a = context->PopInt();
    context->PushInt(~a);
}

void AxCodeProcessor::Operation_IAnd(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a & b);
}

void AxCodeProcessor::Operation_IOr(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a | b);
}

void AxCodeProcessor::Operation_IXor(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a ^ b);
}

void AxCodeProcessor::Operation_IShl(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a << b);
}

void AxCodeProcessor::Operation_IShr(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a >> b);
}

void AxCodeProcessor::Operation_IDiv(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a / b);
}

void AxCodeProcessor::Operation_IMod(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a % b);
}

void AxCodeProcessor::Operation_IMul(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a * b);
}

void AxCodeProcessor::Operation_ISub(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a - b);
}

void AxCodeProcessor::Operation_IAdd(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(a + b);
}

void AxCodeProcessor::Operation_ILogicalNot(AxCodeProcessor *context)
{
    int a = context->PopInt();
    context->PushInt(a == 0 ? 1 : 0);
}

void AxCodeProcessor::Operation_ILogicalAnd(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt(((a != 0) && (b != 0)) ? 1 : 0);
}

void AxCodeProcessor::Operation_ILogicalOr(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a | b) != 0 ? 1 : 0);
}

void AxCodeProcessor::Operation_IGreater(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a > b) ? 1 : 0);
}

void AxCodeProcessor::Operation_ILesser(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a < b) ? 1 : 0);
}

void AxCodeProcessor::Operation_IEqual(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a == b) ? 1 : 0);
}

void AxCodeProcessor::Operation_INotEqual(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a != b) ? 1 : 0);
}

void AxCodeProcessor::Operation_IGreaterEqual(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a >= b) ? 1 : 0);
}

void AxCodeProcessor::Operation_ILesserEqual(AxCodeProcessor *context)
{
    int a = context->PopInt();
    int b = context->PopInt();
    context->PushInt((a <= b) ? 1 : 0);
}

// Assignment operations

void AxCodeProcessor::Operation_ISet(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, value);
}

void AxCodeProcessor::Operation_IAddAdd(AxCodeProcessor *context)
{
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) + 1);
}

void AxCodeProcessor::Operation_ISubSub(AxCodeProcessor *context)
{
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) - 1);
}

void AxCodeProcessor::Operation_IAddEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) + value);
}

void AxCodeProcessor::Operation_ISubEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) - value);
}

void AxCodeProcessor::Operation_IMulEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) * value);
}

void AxCodeProcessor::Operation_IModEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) % value);
}

void AxCodeProcessor::Operation_IDivEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) / value);
}

void AxCodeProcessor::Operation_IXorEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) ^ value);
}

void AxCodeProcessor::Operation_IOrEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) | value);
}

void AxCodeProcessor::Operation_IAndEquals(AxCodeProcessor *context)
{
    int value = context->PopInt();
    int address = context->PopInt();
    context->WriteAddressIntValue(address, context->ReadAddressIntValue(address) & value);
}

// Floating point operaions

void AxCodeProcessor::Operation_FToI(AxCodeProcessor *context)
{
	context->PushInt((int)context->PopFloat());
}

void AxCodeProcessor::Operation_FPushAddress(AxCodeProcessor *context)
{
    context->PushInt(context->memory->ReadInt32());
}

void AxCodeProcessor::Operation_FPushIndexedAddress(AxCodeProcessor *context)
{
    int offset = context->PopInt();
    context->PushInt(context->memory->ReadInt32() + offset * 4);
}

void AxCodeProcessor::Operation_FPushAddressValue(AxCodeProcessor *context)
{
    context->PushFloat(context->ReadAddressFloatValue(context->memory->ReadInt32()));
}

void AxCodeProcessor::Operation_FPushIndexedAddressValue(AxCodeProcessor *context)
{
    int offset = (int)context->PopInt();
    context->PushFloat(context->ReadAddressFloatValue(context->memory->ReadInt32() + offset * 4));
}

// Evaluation operations

void AxCodeProcessor::Operation_FDiv(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushFloat(a / b);
}

void AxCodeProcessor::Operation_FMul(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushFloat(a * b);
}

void AxCodeProcessor::Operation_FSub(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushFloat(a - b);
}

void AxCodeProcessor::Operation_FAdd(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushFloat(a + b);
}

void AxCodeProcessor::Operation_FGreater(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushInt((a > b) ? 1 : 0);
}

void AxCodeProcessor::Operation_FLesser(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushInt((a < b) ? 1 : 0);
}

void AxCodeProcessor::Operation_FEqual(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushInt((a == b) ? 1 : 0);
}

void AxCodeProcessor::Operation_FNotEqual(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushInt((a != b) ? 1 : 0);
}

void AxCodeProcessor::Operation_FGreaterEqual(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushInt((a >= b) ? 1 : 0);
}

void AxCodeProcessor::Operation_FLesserEqual(AxCodeProcessor *context)
{
	float a = context->PopFloat();
    float b = context->PopFloat();
    context->PushInt((a <= b) ? 1 : 0);
}

// Assignment operations

void AxCodeProcessor::Operation_FSet(AxCodeProcessor *context)
{
    float value = context->PopFloat();
    int address = context->PopInt();
    context->WriteAddressFloatValue(address, value);
}

void AxCodeProcessor::Operation_FAddEquals(AxCodeProcessor *context)
{
    float value = context->PopFloat();
    int address = context->PopInt();
    context->WriteAddressFloatValue(address, context->ReadAddressFloatValue(address) + value);
}

void AxCodeProcessor::Operation_FSubEquals(AxCodeProcessor *context)
{
    float value = context->PopFloat();
    int address = context->PopInt();
    context->WriteAddressFloatValue(address, context->ReadAddressFloatValue(address) - value);
}

void AxCodeProcessor::Operation_FMulEquals(AxCodeProcessor *context)
{
    float value = context->PopFloat();
    int address = context->PopInt();
    context->WriteAddressFloatValue(address, context->ReadAddressFloatValue(address) * value);
}

void AxCodeProcessor::Operation_FDivEquals(AxCodeProcessor *context)
{
    float value = context->PopFloat();
    int address = context->PopInt();
    context->WriteAddressFloatValue(address, context->ReadAddressFloatValue(address) / value);
}

AxCodeProcessor::AxCodeProcessor(void)
{
	this->operationId_JMP						= this->AddOperation(AxCodeProcessor::Operation_JMP							);
	this->operationId_JZ						= this->AddOperation(AxCodeProcessor::Operation_JZ							);
	this->operationId_JNZ						= this->AddOperation(AxCodeProcessor::Operation_JNZ							);
	this->operationId_Call						= this->AddOperation(AxCodeProcessor::Operation_Call						);
	this->operationId_RET						= this->AddOperation(AxCodeProcessor::Operation_RET							);
	this->operationId_CallOut					= this->AddOperation(AxCodeProcessor::Operation_CallOut						);
	
	// Integer
	this->operationId_IToF						= this->AddOperation(AxCodeProcessor::Operation_IToF						);
	this->operationId_IPushAddress				= this->AddOperation(AxCodeProcessor::Operation_IPushAddress				);
	this->operationId_IPushIndexedAddress		= this->AddOperation(AxCodeProcessor::Operation_IPushIndexedAddress			);
	this->operationId_IPushAddressValue			= this->AddOperation(AxCodeProcessor::Operation_IPushAddressValue			);
	this->operationId_IPushIndexedAddressValue	= this->AddOperation(AxCodeProcessor::Operation_IPushIndexedAddressValue	);
	// Evaluation
	this->operationId_INot						= this->AddOperation(AxCodeProcessor::Operation_INot						);
	this->operationId_IAnd						= this->AddOperation(AxCodeProcessor::Operation_IAnd						);
	this->operationId_IOr						= this->AddOperation(AxCodeProcessor::Operation_IOr							);
	this->operationId_IXor						= this->AddOperation(AxCodeProcessor::Operation_IXor						);
	this->operationId_IShl						= this->AddOperation(AxCodeProcessor::Operation_IShl						);
	this->operationId_IShr						= this->AddOperation(AxCodeProcessor::Operation_IShr						);
	this->operationId_IDiv						= this->AddOperation(AxCodeProcessor::Operation_IDiv						);
	this->operationId_IMod						= this->AddOperation(AxCodeProcessor::Operation_IMod						);
	this->operationId_IMul						= this->AddOperation(AxCodeProcessor::Operation_IMul						);
	this->operationId_ISub						= this->AddOperation(AxCodeProcessor::Operation_ISub						);
	this->operationId_IAdd						= this->AddOperation(AxCodeProcessor::Operation_IAdd						);
	this->operationId_ILogicalNot				= this->AddOperation(AxCodeProcessor::Operation_ILogicalNot					);
	this->operationId_ILogicalAnd				= this->AddOperation(AxCodeProcessor::Operation_ILogicalAnd					);
	this->operationId_ILogicalOr				= this->AddOperation(AxCodeProcessor::Operation_ILogicalOr					);
	this->operationId_IGreater					= this->AddOperation(AxCodeProcessor::Operation_IGreater					);
	this->operationId_ILesser					= this->AddOperation(AxCodeProcessor::Operation_ILesser						);
	this->operationId_IEqual					= this->AddOperation(AxCodeProcessor::Operation_IEqual						);
	this->operationId_INotEqual					= this->AddOperation(AxCodeProcessor::Operation_INotEqual					);
	this->operationId_IGreaterEqual				= this->AddOperation(AxCodeProcessor::Operation_IGreaterEqual				);
	this->operationId_ILesserEqual				= this->AddOperation(AxCodeProcessor::Operation_ILesserEqual				);
	// Assignment	
	this->operationId_ISet						= this->AddOperation(AxCodeProcessor::Operation_ISet						);
	this->operationId_IAddAdd					= this->AddOperation(AxCodeProcessor::Operation_IAddAdd						);
	this->operationId_ISubSub					= this->AddOperation(AxCodeProcessor::Operation_ISubSub						);
	this->operationId_IAddEquals				= this->AddOperation(AxCodeProcessor::Operation_IAddEquals					);
	this->operationId_ISubEquals				= this->AddOperation(AxCodeProcessor::Operation_ISubEquals					);
	this->operationId_IMulEquals				= this->AddOperation(AxCodeProcessor::Operation_IMulEquals					);
	this->operationId_IModEquals				= this->AddOperation(AxCodeProcessor::Operation_IModEquals					);
	this->operationId_IDivEquals				= this->AddOperation(AxCodeProcessor::Operation_IDivEquals					);
	this->operationId_IXorEquals				= this->AddOperation(AxCodeProcessor::Operation_IXorEquals					);
	this->operationId_IOrEquals					= this->AddOperation(AxCodeProcessor::Operation_IOrEquals					);
	this->operationId_IAndEquals				= this->AddOperation(AxCodeProcessor::Operation_IAndEquals					);
	
	// Floating point
	this->operationId_FToI						= this->AddOperation(AxCodeProcessor::Operation_FToI						);
	this->operationId_FPushAddress				= this->AddOperation(AxCodeProcessor::Operation_FPushAddress				);
	this->operationId_FPushIndexedAddress		= this->AddOperation(AxCodeProcessor::Operation_FPushIndexedAddress			);
	this->operationId_FPushAddressValue			= this->AddOperation(AxCodeProcessor::Operation_FPushAddressValue			);
	this->operationId_FPushIndexedAddressValue	= this->AddOperation(AxCodeProcessor::Operation_FPushIndexedAddressValue	);
	// Evaluation
	this->operationId_FDiv						= this->AddOperation(AxCodeProcessor::Operation_FDiv						);
	this->operationId_FMul						= this->AddOperation(AxCodeProcessor::Operation_FMul						);
	this->operationId_FSub						= this->AddOperation(AxCodeProcessor::Operation_FSub						);
	this->operationId_FAdd						= this->AddOperation(AxCodeProcessor::Operation_FAdd						);
	this->operationId_FGreater					= this->AddOperation(AxCodeProcessor::Operation_FGreater					);
	this->operationId_FLesser					= this->AddOperation(AxCodeProcessor::Operation_FLesser						);
	this->operationId_FEqual					= this->AddOperation(AxCodeProcessor::Operation_FEqual						);
	this->operationId_FNotEqual					= this->AddOperation(AxCodeProcessor::Operation_FNotEqual					);
	this->operationId_FGreaterEqual				= this->AddOperation(AxCodeProcessor::Operation_FGreaterEqual				);
	this->operationId_FLesserEqual				= this->AddOperation(AxCodeProcessor::Operation_FLesserEqual				);
	// Assignment	
	this->operationId_FSet						= this->AddOperation(AxCodeProcessor::Operation_FSet						);
	this->operationId_FAddEquals				= this->AddOperation(AxCodeProcessor::Operation_FAddEquals					);
	this->operationId_FSubEquals				= this->AddOperation(AxCodeProcessor::Operation_FSubEquals					);
	this->operationId_FMulEquals				= this->AddOperation(AxCodeProcessor::Operation_FMulEquals					);
	this->operationId_FDivEquals				= this->AddOperation(AxCodeProcessor::Operation_FDivEquals					);

	this->stackCapacity = 0;
	this->stackPos = 0;
	this->stack = 0;
	this->EnsureStackCapacity(256);
}


AxCodeProcessor::~AxCodeProcessor(void)
{
	AxMem::FreeAndNull(&this->stack);
}

void AxCodeProcessor::EnsureStackCapacity(int capacity)
{
	if (this->stackCapacity >= capacity)
		return;

	if (this->stackCapacity <= 0)
		this->stackCapacity = 1;
		
	while (this->stackCapacity < capacity)
		this->stackCapacity *= 2;

	void *newStack = AxMem::Get(capacity);
	AxMem::Copy(newStack, this->stack, this->stackPos);
	AxMem::FreeAndNull(&this->stack);
	this->stack = newStack;
	this->stackCapacity = capacity;
}

void AxCodeProcessor::PushInt(int value)
{
	this->EnsureStackCapacity(this->stackPos + 4);

	*(int*)((int)this->stack + this->stackPos) = value;
	this->stackPos += 4;
}

int AxCodeProcessor::PopInt()
{
	this->stackPos -= 4;
	int result = *(int*)((int)this->stack + this->stackPos);
	return result;
}

void AxCodeProcessor::PushFloat(float value)
{
	this->EnsureStackCapacity(this->stackPos + 4);

	*(float*)((int)this->stack + this->stackPos) = value;
	this->stackPos += 4;
}

float AxCodeProcessor::PopFloat()
{
	this->stackPos -= 4;
	float result = *(float*)((int)this->stack + this->stackPos);
	return result;
}

int AxCodeProcessor::ReadAddressIntValue(int address)
{
	long long oldPosition = this->memory->position;

	this->memory->Seek(address);
	int result = this->memory->ReadInt32();

	this->memory->Seek(oldPosition);

	return result;
}

void AxCodeProcessor::WriteAddressIntValue(int address, int value)
{
	long long oldPosition = this->memory->position;

	this->memory->Seek(address);
	this->memory->WriteInt32(value);

	this->memory->Seek(oldPosition);
}

float AxCodeProcessor::ReadAddressFloatValue(int address)
{
	long long oldPosition = this->memory->position;

	this->memory->Seek(address);
	float result = this->memory->ReadFloat32();

	this->memory->Seek(oldPosition);

	return result;
}

void AxCodeProcessor::WriteAddressFloatValue(int address, float value)
{
	long long oldPosition = this->memory->position;

	this->memory->Seek(address);
	this->memory->WriteFloat32(value);

	this->memory->Seek(oldPosition);
}

int AxCodeProcessor::AddOperation(AxCodeProcessorOperation operation)
{
	this->operations.Add(operation);
	return this->operations.count - 1;
}

void AxCodeProcessor::SetMemory(AxStream *memory)
{
	this->memory = memory;
}

void AxCodeProcessor::Process()
{
	while (this->memory->position < this->memory->length)
	{
		unsigned char operationId = this->memory->ReadUInt8();

		this->operations[operationId](this);
	}
}

void AxCodeProcessor::OnCallout(int calloutId)
{
}