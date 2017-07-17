//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxScriptTokens.h"


AxScriptTokens::AxScriptTokens(void)
{
}


AxScriptTokens::~AxScriptTokens(void)
{
}


int AxScriptTokens::AddSign(AxString sign)
{
	this->signSet.Add(sign);
	return this->signSet.count - 1;
}

void AxScriptTokens::Parse(AxString sourceCode)
{
	this->sourceCode = sourceCode;

	int scriptPos = 0;
	int identStart = 0;

	while (scriptPos < this->sourceCode.length)
	{
		int identLength = scriptPos - identStart;

		if (this->ignoreCharset.IndexOf(this->sourceCode[scriptPos]) != -1)
		{
			// Add any token that might have been read so far
			if (identLength > 0)
				this->Add(AxScriptToken(sourceCode.SubString(identStart, identLength), -1, identStart, scriptPos));

			identStart = scriptPos + 1;
		}
		else
		{
			int signIndex = this->FindSignIndex(scriptPos);
			if (signIndex != -1)
			{
				// Add any token that might have been read so far
				if (identLength > 0)
					this->Add(AxScriptToken(sourceCode.SubString(identStart, identLength), -1, identStart, scriptPos));

				// Add the special sign token that was met
				int signLength = this->signSet[signIndex].length;
				this->Add(AxScriptToken(sourceCode.SubString(scriptPos, signLength), signIndex, scriptPos, scriptPos + signLength));

				scriptPos += signLength - 1;
				identStart = scriptPos + 1;
			}
		}

		scriptPos++;
	}

	int identLength = scriptPos - identStart;
	if (identLength > 0)
		this->Add(AxScriptToken(sourceCode.SubString(identStart, identLength), -1, identStart, scriptPos));
}

int AxScriptTokens::FindSignIndex(int scriptPos)
{
	int result = -1;
	int resultLength = -1;
	for (int signIndex = 0; signIndex < this->signSet.count; signIndex++)
	{
		int stringLength = this->signSet[signIndex].length;
		if (stringLength > resultLength)
			if (scriptPos + stringLength <= this->sourceCode.length)
				for (int charIndex = 0; charIndex < stringLength; charIndex++)
					if (this->sourceCode[scriptPos + charIndex] == this->signSet[signIndex][charIndex])
					{
						if (charIndex == stringLength - 1)
						{
							result = signIndex;
							resultLength = stringLength;
						}
					}
					else
						break;

	}

	return result;
}

int AxScriptTokens::FindId(int startIndex, int endIndex, int signId)
{
	for (int index = startIndex; index <= endIndex; index++)
		if ((*this)[index].id == signId)
			return index;

	return -1;
}

int AxScriptTokens::FindClosingBracket(int closingBracketTokenId, int startIndex, int endIndex)
{
	int openingBracketTokenId = (*this)[startIndex].id;

	int numBrackets = 1;

	if (endIndex == -1)
		endIndex = this->count - 1;

	for (int i = startIndex + 1; i <= endIndex; i++)
	{
		int tokenId = (*this)[i].id;

		if (tokenId == openingBracketTokenId)
			numBrackets++;

		if (tokenId == closingBracketTokenId)
		{
			numBrackets--;

			if (numBrackets == 0)
				return i;
		}
	}

	return -1;
}