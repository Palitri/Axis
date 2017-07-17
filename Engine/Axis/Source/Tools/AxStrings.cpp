//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxStrings.h"


AxStrings::AxStrings(void)
	: AxList<AxString>()
{
}


AxStrings::~AxStrings(void)
{
}

void AxStrings::SplitString(AxString &string, const AxString &splitter, bool ignoreEmpty)
{
	this->Clear();
	
	int start = 0, end;
	do
	{
		end = string.IndexOf(splitter, start);
		if (end == -1)
			end = string.length;

		int length = end - start;
		if ((!ignoreEmpty) || (length > 0))
			this->Add(string.SubString(start, length));

		start = end + splitter.length;
	}
	while (start < string.length);

	// Attempt to reuse already existing strings
	//int start = 0, end, count = 0;
	//do
	//{
	//	end = string.IndexOf(splitter, start);
	//	if (end == -1)
	//		end = string.length;

	//	int length = end - start;
	//	if ((!ignoreEmpty) || (length > 0))
	//	{
	//		if (count + 1> this->count)
	//			this->Add(string.SubString(start, length));
	//		else
	//		{
	//			this->values[count].SetLength(length);
	//			this->values[count].CopyCharacters(0, string.contents, start, length);
	//		}

	//		count++;
	//	}

	//	start = end + splitter.length;
	//}
	//while (start < string.length);

	//this->SetSize(count);
}

AxString AxStrings::ToString(const AxString &splitter)
{
	AxString result;
	for (int i = 0;;)
	{
		result.Insert((*this)[i]);
		
		if (++i == this->count)
			break;

		result.Insert(splitter);
	}

	return AxString(result);
}

int AxStrings::IndexOf(AxString &string, AxStringComparison comparisonMode)
{
	if (comparisonMode == StringComparison_CaseSensitive)
	{
		for (int i = 0; i < this->count; i++)
			if ((*this)[i].Equals(string))
				return i;
	}
	else if (comparisonMode == StringComparison_CaseInsensitive)
	{
		AxString lower = string.ToLower();
		for (int i = 0; i < this->count; i++)
			if ((*this)[i].ToLower().Equals(lower))
				return i;
	}

	return -1;
}
