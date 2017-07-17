#pragma once

#include "..\..\AxGlobals.h"

#include "..\..\Tools\AxStrings.h"
#include "..\..\Tools\AxList.h"

#include "AxScriptToken.h"

class AXDLLCLASS AxScriptTokens :
	public AxList<AxScriptToken>
{
private:
	// Checks if a sign from the signSet is found at the given script position and returns its index. If not found, returns -1
	int FindSignIndex(int scriptPos);


public:
	AxString ignoreCharset;
	AxStrings signSet;

	AxString sourceCode;
	
	AxScriptTokens(void);
	~AxScriptTokens(void);

	// Adds a sign to be distinguished as a separate token in the script and returns its id.
	int AddSign(AxString sign);

	// Reads the tokens from the given script code and fills them in
	void Parse(AxString sourceCode);

	// Searches for the index of the bracket, closing the expression starting at startIndex.
	int FindClosingBracket(int closingBracketTokenId, int startIndex, int endIndex = -1);

	// Searches within startIndex and endIndex for the first token with the given code and returns its index. If not found returns -1
	int FindId(int startIndex, int endIndex, int tokenId);
};

