#pragma once

#include "..\AxGlobals.h"

#include "AxString.h"
#include "AxList.h"

enum AxStringComparison
{
	StringComparison_CaseSensitive,
	StringComparison_CaseInsensitive
};

class AXDLLCLASS AxStrings
	: public AxList<AxString>
{
public:
	AxStrings(void);
	AxStrings(AxStrings *strings);
	~AxStrings(void);

	void SplitString(AxString &string, const AxString &splitter, bool ignoreEmpty = true);
	AxString ToString(const AxString &splitter = ",");

	int IndexOf(AxString &string, AxStringComparison comparisonMode = StringComparison_CaseSensitive);
};