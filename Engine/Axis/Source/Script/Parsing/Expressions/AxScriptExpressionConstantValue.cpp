//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptExpressionConstantValue.h"


AxScriptExpressionConstantValue::AxScriptExpressionConstantValue(AxCodeBuilder *codeBuilder) :
	AxScriptExpression(codeBuilder)
{
}


AxScriptExpressionConstantValue::~AxScriptExpressionConstantValue(void)
{
}

bool AxScriptExpressionConstantValue::Run()
{
    if (this->codeBuilder->tokenIndex < this->codeBuilder->tokens.count)
	{
		if (this->codeBuilder->tokenIndex + 2 < this->codeBuilder->tokens.count)
		{
			if (this->codeBuilder->tokens[this->codeBuilder->tokenIndex + 1].id == this->codeBuilder->tokenId_Point)
			{
				AxString floatValue = this->codeBuilder->tokens[this->codeBuilder->tokenIndex].name + this->codeBuilder->tokens[this->codeBuilder->tokenIndex + 1].name + this->codeBuilder->tokens[this->codeBuilder->tokenIndex + 2].name;
				if (floatValue.EndsWith('f'))
					floatValue.SetLength(floatValue.length - 1);
				double value;
				if (floatValue.ToDouble(value))
				{
					if (this->codeBuilder->dataType == this->codeBuilder->dataType_float)
					{
						this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FPushAddress);
						this->codeBuilder->code->WriteFloat32((float)value);
					}
					else if (this->codeBuilder->dataType == this->codeBuilder->dataType_int)
					{
						this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IPushAddress);
						this->codeBuilder->code->WriteInt32((int)value);
					}

					this->codeBuilder->tokenIndex += 3;

					return true;
				}
			}
		}

		int value;
		if (this->codeBuilder->GetToken()->name.ToInt(value))
		{
			if (this->codeBuilder->dataType == this->codeBuilder->dataType_int)
			{
				this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_IPushAddress);
				this->codeBuilder->code->WriteInt32(value);
			}
			else if (this->codeBuilder->dataType == this->codeBuilder->dataType_float)
			{
				this->codeBuilder->code->WriteUInt8(this->codeBuilder->processor->operationId_FPushAddress);
				this->codeBuilder->code->WriteFloat32((float)value);
			}

			this->codeBuilder->tokenIndex += 1;

			return true;
		}
	}

	return false;
}
