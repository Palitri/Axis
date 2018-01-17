//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxString.h"

#include <stdlib.h>
#include <string>

#include "AxMem.h"
#include "AxMath.h"

AxString base64Map = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

AxString::AxString(void)
{
	this->Initialize();
}

AxString::AxString(AxString &str)
{
	this->Initialize();
	*this = str;
}

AxString::AxString(const char *str)
{
	this->Initialize();
	*this = str;
}

AxString::AxString(const wchar_t *str)
{
	this->Initialize();
	*this = str;
}

AxString::AxString(const char *str, int length)
{
	this->Initialize();

	this->EnsureCapacity(length);

	int i;
	for (i = 0; i < length; i++)
		this->contents[i] = str[i];

	this->contents[length] = 0;
	this->length = length;

	this->charContentsChanged = true;
}

AxString::AxString(const wchar_t *str, int length)
{
	this->Initialize();
	
	this->EnsureCapacity(length);

	AxMem::Copy(this->contents, str, length * sizeof(wchar_t));

	this->contents[length] = 0;
	this->length = length;

	this->charContentsChanged = true;
}

AxString::AxString(char str)
{
	this->Initialize();

	this->EnsureCapacity(1);

	this->contents[0] = str;

	this->contents[1] = 0;
	this->length = 1;

	this->charContentsChanged = true;
}

AxString::AxString(wchar_t str)

{
	this->Initialize();

	this->EnsureCapacity(1);

	this->contents[0] = str;

	this->contents[1] = 0;
	this->length = 1;

	this->charContentsChanged = true;
}

AxString::AxString(int value, int radix)
{
	this->Initialize();

	int maxLength = sizeof(int) * 8;
	wchar_t *v = new wchar_t[maxLength + 1];
	_itow_s(value, v, maxLength, radix);
	*this = v;
	delete[] v;
}

AxString::AxString(unsigned int value, int radix)
{
	this->Initialize();

	int maxLength = sizeof(int) * 8;
	wchar_t *v = new wchar_t[maxLength + 1];
	_i64tow_s(value, v, maxLength, radix);
	*this = v;
	delete[] v;
}

AxString::AxString(long long value, int radix)
{
	this->Initialize();

	int maxLength = sizeof(int) * 8;
	wchar_t *v = new wchar_t[maxLength + 1];
	_i64tow_s(value, v, maxLength, radix);
	*this = v;
	delete[] v;
}

AxString::AxString(unsigned long long value, int radix)
{
	this->Initialize();

	int maxLength = sizeof(int) * 8;
	wchar_t *v = new wchar_t[maxLength + 1];
	_ui64tow_s(value, v, maxLength, radix);
	*this = v;
	delete[] v;
}

AxString::AxString(double value)
{
	this->Initialize();

	*this = std::to_wstring((long double)value).c_str();
}

AxString::~AxString(void)
{
	delete[] this->contents;
	
	if (this->charContents != 0)
		delete[] this->charContents;
}


wchar_t &AxString::operator [] (int index) 
{ 
	return this->contents[index];
}


// AxString operators
void AxString::operator = (const AxString &argument) 
{ 
	this->Copy(argument);
}

void AxString::operator += (const AxString &argument) 
{ 
	this->Insert(argument);
}

AxString AxString::operator + (const AxString &argument) 
{ 
	AxString result;
	result.Copy(*this);
	result.Insert(argument);

	return AxString(result.GetContents());
}

bool AxString::operator == (const AxString &argument) 
{ 
	return this->Compare(argument) == 0;
}

bool AxString::operator != (const AxString &argument) 
{ 
	return this->Compare(argument) != 0;
}

bool AxString::operator > (const AxString &argument) 
{ 
	return this->Compare(argument) > 0;
}

bool AxString::operator >= (const AxString &argument) 
{ 
	return this->Compare(argument) >= 0;
}

bool AxString::operator < (const AxString &argument) 
{ 
	return this->Compare(argument) < 0;
}

bool AxString::operator <= (const AxString &argument) 
{ 
	return Compare(argument) <= 0;
}


// WChar operators
void AxString::operator = (const wchar_t *argument) 
{ 
	this->Copy(argument);
}

void AxString::operator += (const wchar_t *argument) 
{ 
	this->Insert(argument);
}

AxString AxString::operator + (const wchar_t *argument) 
{ 
	AxString result;
	result.Copy(*this);
	result.Insert(argument);

	return AxString(result);
}

bool AxString::operator == (const wchar_t *argument) 
{ 
	return this->Compare(argument) == 0;
}

bool AxString::operator != (const wchar_t *argument) 
{ 
	return this->Compare(argument) != 0;
}

bool AxString::operator > (const wchar_t *argument) 
{ 
	return this->Compare(argument) > 0;
}

bool AxString::operator >= (const wchar_t *argument) 
{ 
	return this->Compare(argument) >= 0;
}

bool AxString::operator < (const wchar_t *argument) 
{ 
	return this->Compare(argument) < 0;
}

bool AxString::operator <= (const wchar_t *argument) 
{ 
	return this->Compare(argument) <= 0;
}


// Char operators
void AxString::operator = (const char *argument) 
{ 
	this->Copy(argument);
}

void AxString::operator += (const char *argument) 
{ 
	this->Insert(argument);
}

AxString AxString::operator + (const char *argument) 
{
	AxString result;
	result.Copy(*this);
	result.Insert(argument);

	return AxString(result);
}

bool AxString::operator == (const char *argument) 
{ 
	return this->Compare(argument) == 0;
}

bool AxString::operator != (const char *argument) 
{ 
	return this->Compare(argument) != 0;
}

bool AxString::operator > (const char *argument) 
{ 
	return this->Compare(argument) > 0;
}

bool AxString::operator >= (const char *argument) 
{ 
	return this->Compare(argument) >= 0;
}

bool AxString::operator < (const char *argument) 
{ 
	return this->Compare(argument) < 0;
}

bool AxString::operator <= (const char *argument) 
{ 
	return this->Compare(argument) <= 0;
}

// Dynamic methods
void AxString::Initialize()
{
	this->length = 0;
	this->capacity = 0;
	this->contents = 0;
	this->ResizeCapacity(16);

	this->charContentsCapacity = 0;
	this->charContents = 0;
	this->charContentsChanged = true;
}

void AxString::ResizeCapacity(int newCapacity, bool keepContents)
{
	wchar_t *newContents = new wchar_t[newCapacity + 1];
	int newLength = newCapacity > this->length ? this->length : newCapacity;
	
	if (keepContents)
		if (this->length > 0)
			AxMem::Copy(newContents, this->contents, newLength * sizeof(wchar_t));
	newContents[newLength] = 0;

	if (this->contents != 0)
		delete[] this->contents;

	this->contents = newContents;
	this->length = newLength;
	this->capacity = newCapacity;

	this->charContentsChanged = true;
};

void AxString::EnsureCapacity(int length)
{
	int newCapacity = this->capacity;
	while (length > newCapacity)
	{
		newCapacity *= 2;
	}

	if (newCapacity > this->capacity)
	{
		this->ResizeCapacity(newCapacity, true);
	}
}

void AxString::SetLength(int length)
{
	if (length == this->length)
		return;

	this->EnsureCapacity(length);

	this->length = length;
	this->contents[this->length] = 0;
	this->charContentsChanged = true;
}

int AxString::GetLength()
{
	return this->length;
}

bool AxString::IsEmpty()
{
	return this->length == 0;
}

bool AxString::IsLowerCase()
{
	if (this->length == 0)
		return false;

	for (int i = 0; i < this->length; i++)
		if (islower(this->contents[i]) == 0)
			return false;

	return true;
}

bool AxString::IsUpperCase()
{
	if (this->length == 0)
		return false;

	for (int i = 0; i < this->length; i++)
		if (isupper(this->contents[i]) == 0)
			return false;

	return true;
}

const wchar_t *AxString::GetContents()
{
	return this->contents;
}

const char *AxString::GetCharContents()
{
	if (this->charContentsChanged)
	{
		if (this->capacity > this->charContentsCapacity)
		{
			if (this->charContents != 0)
				delete[] this->charContents;

			this->charContentsCapacity = this->capacity;
			this->charContents = new char[this->charContentsCapacity + 1];
		}

		for (int i = 0; i < this->length; i++)
			this->charContents[i] = (char)this->contents[i];

		this->charContents[this->length] = 0;

		this->charContentsChanged = false;
	}

	return this->charContents;
}


int AxString::Compare(const AxString &str)
{
	return this->Compare(str.contents);
}

int AxString::Compare(const char *str)
{
	int length = AxString::GetLength(str);
	int extraLength = length - this->length;
	int minLength = extraLength < 0 ? length : this->length;

	for (int i = 0; i < minLength; i++)
	{
		int difference = str[i] - this->contents[i];

		if (difference > 0)
			return -1;
		else if (difference < 0)
			return 1;
	}

	if (extraLength > 0)
		return -1;
	else if (extraLength < 0)
		return 1;

	return 0;
}

int AxString::Compare(const wchar_t *str)
{
	int length = AxString::GetLength(str);
	int extraLength = length - this->length;
	int minLength = extraLength < 0 ? length : this->length;

	for (int i = 0; i < minLength; i++)
	{
		int difference = str[i] - this->contents[i];

		if (difference > 0)
			return -1;
		else if (difference < 0)
			return 1;
	}

	if (extraLength > 0)
		return -1;
	else if (extraLength < 0)
		return 1;

	return 0;
}

bool AxString::Equals(const AxString &str)
{
	return this->Compare(str) == 0;
}

bool AxString::Equals(const char *str)
{
	return this->Compare(str) == 0;
}

bool AxString::Equals(const wchar_t *str)
{
	return this->Compare(str) == 0;
}

void AxString::InsertCharacters(int position, int count)
{
	int oldLength = this->length;
	int tailCharacters = oldLength - position;
	
	this->SetLength(this->length + count);

	if (count > tailCharacters)
	{
		AxMem::Copy(&this->contents[position + count], &this->contents[position], tailCharacters * sizeof(wchar_t));
	}
	else
	{
		void *tailData = new wchar_t[tailCharacters];
		AxMem::Copy(tailData, &this->contents[position], tailCharacters * sizeof(wchar_t));
		AxMem::Copy(&this->contents[position + count], tailData, count * sizeof(wchar_t));
		AxMem::FreeAndNull(&tailData);
	}

	this->charContentsChanged = true;
}

void AxString::CopyCharacters(int offset, const AxString &source, int sourceOffset, int count)
{
	this->CopyCharacters(offset, source.contents, sourceOffset, count);
}

void AxString::CopyCharacters(int offset, const char *source, int sourceOffset, int count)
{
	for (int i = 0; i < count; i++)
		this->contents[offset + i] = source[sourceOffset + i];

	this->charContentsChanged = true;
}

void AxString::CopyCharacters(int offset, const wchar_t *source, int sourceOffset, int count)
{
	AxMem::Copy(&this->contents[offset], &source[sourceOffset], count * sizeof(wchar_t));

	this->charContentsChanged = true;
}

void AxString::FillCharacters(int offset, const char fillChar, int count)
{
	int end = offset + count;
	for (int i = offset; i < end; i++)
		this->contents[i] = fillChar;

	this->charContentsChanged = true;
}

void AxString::FillCharacters(int offset, const wchar_t fillChar, int count)
{
	int end = offset + count;
	for (int i = offset; i < end; i++)
		this->contents[i] = fillChar;

	this->charContentsChanged = true;
}

void AxString::Copy(const AxString &source)
{
	this->Copy(source.contents);
}

void AxString::Copy(const char *source)
{
	int length = AxString::GetLength(source);
	
	this->EnsureCapacity(length);

	int i;
	for (i = 0; i < length; i++)
		this->contents[i] = source[i];

	this->contents[length] = 0;
	this->length = length;

	this->charContentsChanged = true;
}

void AxString::Copy(const wchar_t *source)
{
	int length = AxString::GetLength(source);
	
	this->EnsureCapacity(length);

	AxMem::Copy(this->contents, source, length * sizeof(wchar_t));

	this->contents[length] = 0;
	this->length = length;

	this->charContentsChanged = true;
}

void AxString::Insert(const AxString &source, int position, int count)
{
	this->Insert(source.contents, position);
}

void AxString::Insert(const char *source, int position, int count)
{
	if (position == -1)
		position = this->length;

	if (count == -1)
		count = AxString::GetLength(source);

	this->InsertCharacters(position, count);
	this->CopyCharacters(position, source, 0, count);
}

void AxString::Insert(const wchar_t *source, int position, int count)
{
	if (position == -1)
		position = this->length;

	if (count == -1)
		count = AxString::GetLength(source);

	this->InsertCharacters(position, count);
	this->CopyCharacters(position, source, 0, count);
}

int AxString::IndexOf(const AxString &substr, int start)
{
	return this->IndexOf(substr.contents, start);
}

int AxString::IndexOf(const char *substr, int start)
{
	int substrLength = AxString::GetLength(substr);

	for (int index = start; index <= this->length - substrLength; index++)
	{
		int thisIndex = index;
		int substrIndex = 0;
		while (this->contents[thisIndex] == substr[substrIndex])
		{
			substrIndex++;
			if (substrIndex == substrLength)
				return index;
			thisIndex++;
			if (thisIndex == this->length)
				return -1;
		}
	}

	return -1;
}

int AxString::IndexOf(const wchar_t *substr, int start)
{
	int substrLength = AxString::GetLength(substr);

	for (int index = start; index <= this->length - substrLength; index++)
	{
		int thisIndex = index;
		int substrIndex = 0;
		while (this->contents[thisIndex] == substr[substrIndex])
		{
			substrIndex++;
			if (substrIndex == substrLength)
				return index;
			thisIndex++;
			if (thisIndex == this->length)
				return -1;
		}
	}

	return -1;
}

int AxString::LastIndexOf(const AxString &substr, int start)
{
	return this->LastIndexOf(substr.contents, start);
}

int AxString::LastIndexOf(const char *substr, int start)
{
	if (start == -1)
		start = AxString::GetLength(this->contents) - 1;
	
	int substrLast = AxString::GetLength(substr) - 1;

	for (int index = start; index >= substrLast; index--)
	{
		int thisIndex = index;
		int substrIndex = substrLast;
		while (this->contents[thisIndex] == substr[substrIndex])
		{
			if (substrIndex == 0)
				return index - substrLast;
			if (thisIndex == 0)
				return -1;
			substrIndex--;
			thisIndex--;
		}
	}

	return -1;
}

int AxString::LastIndexOf(const wchar_t *substr, int start)
{
	if (start == -1)
		start = AxString::GetLength(this->contents) - 1;
	
	int substrLast = AxString::GetLength(substr) - 1;

	for (int index = start; index >= substrLast; index--)
	{
		int thisIndex = index;
		int substrIndex = substrLast;
		while (this->contents[thisIndex] == substr[substrIndex])
		{
			if (substrIndex == 0)
				return index;
			if (thisIndex == 0)
				return -1;
			substrIndex--;
			thisIndex--;
		}
	}

	return -1;
}

AxString AxString::SubString(int start, int count)
{
	start = AxMath::Trim(start, 0, this->length);
	count = AxMath::Trim(count, 0, this->length - start);

	AxString result;
	result.SetLength(count);
	AxMem::Copy(result.contents, &this->contents[start], count * sizeof(wchar_t));
	result.contents[count] = 0;

	return AxString(result.contents);
}

AxString AxString::Remove(int start, int count)
{
	start = AxMath::Trim(start, 0, this->length);
	count = AxMath::Trim(count, 0, this->length - start);

	AxString result;
	result.SetLength(this->length - count);
	AxMem::Copy(result.contents, this->contents, start * sizeof(wchar_t));
	AxMem::Copy(&result.contents[start], &this->contents[start + count], (this->length - start - count) * sizeof(wchar_t));
	result.contents[result.length] = 0;

	return AxString(result.contents);
}

AxString AxString::Remove(const AxString &str)
{
	return this->Remove(str.contents);
}

AxString AxString::Remove(const char *str)
{
	return this->Replace(str, "");
}

AxString AxString::Remove(const wchar_t *str)
{
	return this->Replace(str, L"");
}

AxString AxString::Replace(const AxString &str, const AxString &substitute)
{
	return this->Replace(str.contents, substitute.contents);
}

AxString AxString::Replace(const char *str, const char *substitute)
{
	AxString result;

	int strLength = AxString::GetLength(str);
	int srcPos = 0;
	for (;;)
	{
		int pos = this->IndexOf(str, srcPos);

		if (pos == -1)
			pos = this->length;

		if (pos >= srcPos)
		{
			int chunkLength = pos - srcPos;
			result.Insert(&this->contents[srcPos], result.length, chunkLength);
			if (pos != this->length)
				result.Insert(substitute);
		}

		srcPos = pos + strLength;

		if (srcPos >= this->length)
			break;
	}

	return AxString(result);
}

AxString AxString::Replace(const wchar_t *str, const wchar_t *substitute)
{
	AxString result;

	int strLength = AxString::GetLength(str);
	int srcPos = 0;
	for (;;)
	{
		int pos = this->IndexOf(str, srcPos);

		if (pos == -1)
			pos = this->length;

		if (pos >= srcPos)
		{
			int chunkLength = pos - srcPos;
			result.Insert(&this->contents[srcPos], result.length, chunkLength);
			if (pos != this->length)
				result.Insert(substitute);
		}

		srcPos = pos + strLength;

		if (srcPos >= this->length)
			break;
	}

	return AxString(result);
}

int AxString::FirstDifferenceIndex(const AxString &str)
{
	return this->FirstDifferenceIndex(str.contents);
}

int AxString::FirstDifferenceIndex(const char *str)
{
    int index = 0;
	while (this->contents[index] == str[index])
    {
		if ((this->contents[index] == 0) || (str[index] == 0))
			return index;

		index++;
	}

	return index;
}

int AxString::FirstDifferenceIndex(const wchar_t *str)
{
    int index = 0;
	while (this->contents[index] == str[index])
    {
		if ((this->contents[index] == 0) || (str[index] == 0))
			return index;

		index++;
	}

	return index;
}

AxString AxString::PadLeft(int minLength, const char fillChar)
{
	int padLength = minLength - this->length;
	if (padLength <= 0)
		return AxString(*this);

	AxString result;
	result.SetLength(minLength);

	result.FillCharacters(0, fillChar, padLength);
	result.CopyCharacters(padLength, this->contents, 0, this->length);

	return AxString(result);
}

AxString AxString::PadLeft(int minLength, const wchar_t fillChar)
{
	int padLength = minLength - this->length;
	if (padLength <= 0)
		return AxString(*this);

	AxString result;
	result.SetLength(minLength);

	result.FillCharacters(0, fillChar, padLength);
	result.CopyCharacters(padLength, this->contents, 0, this->length);

	return AxString(result);
}

AxString AxString::PadRight(int minLength, const char fillChar)
{
	int padLength = minLength - this->length;
	if (padLength <= 0)
		return AxString(*this);

	AxString result;
	result.SetLength(minLength);

	result.CopyCharacters(0, this->contents, 0, this->length);
	result.FillCharacters(padLength, fillChar, padLength);

	return AxString(result);
}

AxString AxString::PadRight(int minLength, const wchar_t fillChar)
{
	int padLength = minLength - this->length;
	if (padLength <= 0)
		return AxString(*this);

	AxString result;
	result.SetLength(minLength);

	result.CopyCharacters(0, this->contents, 0, this->length);
	result.FillCharacters(padLength, fillChar, padLength);

	return AxString(result);
}

bool AxString::StartsWith(const AxString &str)
{
	return this->StartsWith(str.contents);
}

bool AxString::StartsWith(const char *str)
{
	int index = 0;

	while ((str[index] != 0) && (index < this->length))
	{
		if (str[index] != this->contents[index])
			return false;

		index++;
	}

	return str[index] == 0;
}

bool AxString::StartsWith(const wchar_t *str)
{
	int index = 0;

	while ((str[index] != 0) && (index < this->length))
	{
		if (str[index] != this->contents[index])
			return false;

		index++;
	}

	return str[index] == 0;
}

bool AxString::StartsWith(const char chr)
{
	if (this->length == 0)
		return false;

	return this->contents[0] == chr;
}

bool AxString::StartsWith(const wchar_t chr)
{
	if (this->length == 0)
		return false;

	return this->contents[0] == chr;
}


bool AxString::EndsWith(const AxString &str)
{
	return this->EndsWith(str.contents);
}

bool AxString::EndsWith(const char *str)
{
	int thisIndex = this->length - AxString::GetLength(str);
	int index = 0;

	while ((str[index] != 0) && (thisIndex < this->length))
	{
		if (str[index] != this->contents[thisIndex])
			return false;

		index++;
		thisIndex++;
	}

	return str[index] == 0;
}

bool AxString::EndsWith(const wchar_t *str)
{
	int thisIndex = this->length - AxString::GetLength(str);
	int index = 0;

	while ((str[index] != 0) && (thisIndex < this->length))
	{
		if (str[index] != this->contents[thisIndex])
			return false;

		index++;
		thisIndex++;
	}

	return str[index] == 0;
}

bool AxString::EndsWith(const char chr)
{
	if (this->length == 0)
		return false;

	return this->contents[this->length - 1] == chr;
}

bool AxString::EndsWith(const wchar_t chr)
{
	if (this->length == 0)
		return false;

	return this->contents[this->length - 1] == chr;
}


AxString AxString::Trim(const char chr, bool left, bool right)
{
	int leftIndex = 0;
	if (left)
	{
		while (this->contents[leftIndex] == chr)
			leftIndex++;
	}

	int rightIndex = this->length - 1;
	if (right)
	{
		while (this->contents[rightIndex] == chr)
			rightIndex--;
	}

	AxString result(*this);
	return AxString(result.SubString(leftIndex, rightIndex - leftIndex + 1));
}

AxString AxString::Trim(const wchar_t chr, bool left, bool right)
{
	int leftIndex = 0;
	if (left)
	{
		while (this->contents[leftIndex] == chr)
			leftIndex++;
	}

	int rightIndex = this->length - 1;
	if (right)
	{
		while (this->contents[rightIndex] == chr)
			rightIndex--;
	}

	AxString result(*this);
	return AxString(result.SubString(leftIndex, rightIndex - leftIndex + 1));
}


bool AxString::ToInt(int &value, int radix)
{
	if (this->length == 0)
		return false;

	wchar_t *end;
	value = AxString::ToInt(this->contents, &end, radix);

	return end == &this->contents[this->length];
}

bool AxString::ToUInt(unsigned int &value, int radix)
{
	if (this->length == 0)
		return false;

	wchar_t *end;
	value = AxString::ToUInt(this->contents, &end, radix);

	return end == &this->contents[this->length];
}

bool AxString::ToFloat(float &value)
{
	if (this->length == 0)
		return false;

	wchar_t *end;
	value = (float)AxString::ToDouble(this->contents, &end);

	return end == &this->contents[this->length];
}

bool AxString::ToDouble(double &value)
{
	if (this->length == 0)
		return false;

	wchar_t *end;
	value = AxString::ToDouble(this->contents, &end);

	return end == &this->contents[this->length];
}

AxString AxString::ToLower()
{
	AxString result = *this;
	_wcslwr_s(result.contents, result.length + 1);
	return AxString(result);
}

AxString AxString::ToUpper()
{
	AxString result = *this;
	_wcsupr_s(result.contents, result.length + 1);
	return AxString(result);
}

// Static Methods

int AxString::GetLength(const char *str)
{
	if (str == 0)
		return -1;

	for (int result = 0; ; result++)
		if (str[result] == 0)
			return result;
}

int AxString::GetLength(const wchar_t *str)
{
	if (str == 0)
		return -1;

	for (int result = 0; ; result++)
		if (str[result] == 0)
			return result;
}

char *AxString::CopyNew(const char *source)
{
	int length = AxString::GetLength(source);

	char *newCopy = new char[length + 1];
	AxMem::Copy(newCopy, source, length + 1);

	return newCopy;
}

wchar_t *AxString::CopyNew(const wchar_t *source)
{
	int length = AxString::GetLength(source);

	wchar_t *newCopy = new wchar_t[length + 1];
	AxMem::Copy(newCopy, source, (length + 1) * sizeof(wchar_t));

	return newCopy;
}

void AxString::Dispose(char **str)
{
	if (*str == 0)
		return;

	delete[] *str;
	*str = 0;
}

void AxString::Dispose(wchar_t **str)
{
	if (*str == 0)
		return;

	delete[] *str;
	*str = 0;
}

AxString AxString::ToBase64()
{
	return AxString::ToBase64(this->GetCharContents(), this->length);
}

AxString AxString::FromBase64()
{
	char *data;
	unsigned int length;
	if (AxString::FromBase64(*this, (void**)&data, length))
	{
		AxString result(data, length);
		delete[] data;

		return AxString(result);
	}

	return AxString();
}

AxString AxString::ToBase64(const void *data, unsigned int dataSize)
{
	int dataTrail = dataSize % 3;
	int b64DataTrail = (dataTrail * 8 + 5) / 6;
	int b64FillerTrail = (4 - b64DataTrail) % 4;
    int fullGroupsCount = dataSize / 3;
	int b64ActualLength = ((dataSize + 2) / 3) * 4;
	int b64FullLength = ((dataSize + 2) / 3) * 4;
	
	unsigned char *dataBytes = (unsigned char*)data;

	AxString result;
	result.SetLength(b64FullLength);

	int b64Index = 0;
	int dataByteIndex = 0;
	for (int i = 0; i < fullGroupsCount; i++)
    {
        int groupBits = dataBytes[dataByteIndex++] << 16;
		groupBits |= dataBytes[dataByteIndex++] << 8;
		groupBits |= dataBytes[dataByteIndex++];

		result[b64Index++] = base64Map[groupBits >> 18];
        result[b64Index++] = base64Map[groupBits >> 12 & 0x3F];
        result[b64Index++] = base64Map[groupBits >> 6 & 0x3F];
        result[b64Index++] = base64Map[groupBits & 0x3F];
    }

    if (dataTrail > 0)
    {
		int groupBits = 0;
		for (int i = 0; i < dataTrail; i++)
			groupBits |= dataBytes[dataByteIndex++] << (16 - i * 8);

        for (int i = 0; i < b64DataTrail; i++)
			result[b64Index++] = base64Map[(groupBits >> 18 - (i * 6)) & 0x3F];
    }

	for (int i = 0; i < b64FillerTrail; i++)
		result[b64Index++] = '=';

    return AxString(result);
}

//AxString AxString::ToBase64(const void *data, unsigned int dataSize)
//{
//	int actualLength = ((dataSize * 8) + 5) / 6;
//	int fullLength = ((actualLength + 3) / 4) * 4;
//	
//	AxString result;
//	result.SetLength(fullLength);
//
//	int charIndex = 0;	
//	for (int i = 0; i < actualLength; i++)
//	{
//		AxMem::CopyBits(data, i * 6, &charIndex, 2, 6);
//		result[i] = base64Map[charIndex];
//	}
//
//	for (int i = actualLength; i < fullLength; i++)
//		result[i] = L'=';
//	
//	return AxString(result);
//}

bool AxString::FromBase64(AxString &base64String, void **data, unsigned int &dataSize)//const void* data, const size_t len)
{
	static const int b64Bits[256] =
	{ 
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
		0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 62, 63, 62, 62, 63, 52, 53, 54, 55,
	   56, 57, 58, 59, 60, 61,  0,  0,  0,  0,  0,  0,  0,  0,  1,  2,  3,  4,  5,  6,
		7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,  0,
		0,  0,  0, 63,  0, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
	   41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51
	};

	const char* b64 = base64String.GetCharContents();
	int b64Length = base64String.length;
	while ((b64Length > 0) && (b64[b64Length - 1] == '='))
		b64Length--;
	int b64Trail = b64Length % 4;
	int dataTrail = (b64Trail * 6) / 8;
	int fullGroupsCount = b64Length / 4;
    dataSize = fullGroupsCount * 3 + dataTrail;
	*data = new char[dataSize];
	
	unsigned char *decodedBytes = (unsigned char*)*data;
	AxMem::Zero(decodedBytes, dataSize);

	int b64Index = 0;
	int decodedByteIndex = 0;
	for (int i = 0; i < fullGroupsCount; i++)
    {
        int groupBits = b64Bits[b64[b64Index++]] << 18;
		groupBits |= b64Bits[b64[b64Index++]] << 12;
		groupBits |= b64Bits[b64[b64Index++]] << 6;
		groupBits |= b64Bits[b64[b64Index++]];

        decodedBytes[decodedByteIndex++] = groupBits >> 16;
        decodedBytes[decodedByteIndex++] = groupBits >> 8 & 0xFF;
        decodedBytes[decodedByteIndex++] = groupBits & 0xFF;
    }

	if (b64Trail > 0)
    {
		int groupBits = 0;
		for (int i = 0; i < b64Trail; i++)
			groupBits |= b64Bits[b64[b64Index++]] << (18 - i * 6);

		for (int i = 0; i < dataTrail; i++)
			decodedBytes[decodedByteIndex++] = groupBits >> (16 - i * 8);
    }

    return true;
}

//bool AxString::FromBase64(AxString &base64String, void **data, unsigned int &dataSize)
//{
//	int actualLength = base64String.length;
//	while ((actualLength >= 0) && ((base64String.contents[actualLength - 1] == L'=')))
//			actualLength--;
//
//	dataSize = (actualLength * 6) / 8;
//	*data = new char[dataSize + 1];
//
//	for (int i = 0; i < actualLength; i++)
//	{
//		int charIndex = base64Map.IndexOf(AxString(base64String.contents[i]));
//		if (charIndex == -1)
//		{
//			delete[] (*data);
//			return false;
//		}
//		AxMem::CopyBits(&charIndex, 2, *data, i * 6, 6);
//	}
//
//	((char*)(*data))[dataSize] = 0;
//
//	return true;
//}

AxString AxString::ToBaseX(const void *data, unsigned int dataSize, AxString alphabet)
{
	int base = alphabet.length;

	AxString result;



	return AxString(result);
}


int AxString::ToInt(char *str, char **end, int radix)
{
	*end = str;
	return strtol(str, end, radix);
}

int AxString::ToInt(wchar_t *str, wchar_t **end, int radix)
{
	*end = str;
	return wcstol(str, end, radix);
}

unsigned int AxString::ToUInt(char *str, char **end, int radix)
{
	*end = str;
	return strtoul(str, end, radix);
}

unsigned int AxString::ToUInt(wchar_t *str, wchar_t **end, int radix)
{
	*end = str;
	return wcstoul(str, end, radix);
}

double AxString::ToDouble(char *str, char **end)
{
	*end = str;
	return strtod(str, end);
}

double AxString::ToDouble(wchar_t *str, wchar_t **end)
{
	*end = str;
	return wcstod(str, end);
}

//AxString AxString::Format(const wchar_t *formatString, ...)
//{
//}


unsigned int AxString::EncodeUtf8Char(void *result, unsigned int codePoint)
{
	const int trailingByteHeader = 0x80;
	const int trailingByteBits = 0x3f;

	unsigned char *resultBytes = (unsigned char*)result;

	if (codePoint >> 7 == 0)
	{
		*resultBytes = (unsigned char)codePoint;
		return 1;
	}

	if (codePoint >> 11 == 0)
	{
		*(resultBytes++) = trailingByteHeader | (codePoint & trailingByteBits);
		*resultBytes = (unsigned char)(0xff << (5 + 1)) | (codePoint >> 6);
		return 2;
	}

	if (codePoint >> 16 == 0)
	{
		*(resultBytes++) = trailingByteHeader | ((unsigned char)codePoint & trailingByteBits);
		*(resultBytes++) = trailingByteHeader | ((unsigned char)(codePoint >> 6) & trailingByteBits);
		*resultBytes = (unsigned char)(0xff << (4 + 1)) | (codePoint >> 12);
		return 3;
	}

	if (codePoint >> 21 == 0)
	{
		*(resultBytes++) = trailingByteHeader | ((unsigned char)codePoint & trailingByteBits);
		*(resultBytes++) = trailingByteHeader | ((unsigned char)(codePoint >> 6) & trailingByteBits);
		*(resultBytes++) = trailingByteHeader | ((unsigned char)(codePoint >> 12) & trailingByteBits);
		*resultBytes = (unsigned char)(0xff << (3 + 1)) | (codePoint >> 18);
		return 4;
	}

	return 0;
}

unsigned int AxString::DecodeUtf8Char(unsigned int &result, void *utf)
{
	const int trailingByteHeader = 0x80;
	const int trailingByteBitMask = 0x3f;

	unsigned char *utfBytes = (unsigned char*)utf;

	if ((*utfBytes & trailingByteHeader) == 0)
	{
		result = *utfBytes;
		return 1;
	}

	result = 0;
	int bytesDecoded = 0;
	int readBits = 0;
	while ((*utfBytes) >> 6 == 2)
	{
		result |= (*utfBytes & trailingByteBitMask) << readBits;
		readBits += 6;
		utfBytes++;
		bytesDecoded++;
	}

	int headerByteBitMask = (1 << (7 - bytesDecoded)) - 1;
	result |= (*utfBytes & headerByteBitMask) << readBits;
	bytesDecoded++;

	return bytesDecoded;
}


int AxString::EncodeUtf8String(void *result, AxString &str)
{
	int bytesEncoded = 0;
	for (int i = 0; i < str.length; i++)
	{
		bytesEncoded += AxString::EncodeUtf8Char((void*)((unsigned int)result + bytesEncoded), str.contents[i]);
	}
	return bytesEncoded;
}

int AxString::DecodeUtf8String(AxString &result, void *utfData, int utfDataLength)
{
	result.SetLength(AxString::GetUTF8AsStringLength(utfData, utfDataLength));
	
	int charIndex = 0;
	int bytesDecoded = 0;
	while (bytesDecoded < utfDataLength)
	{
		unsigned int decodedChar;
		int charBytesDecoded = AxString::DecodeUtf8Char(decodedChar, (void*)((unsigned int)utfData + bytesDecoded));
		if (charBytesDecoded == 0)
			break;
		result.contents[charIndex] = (wchar_t)decodedChar;
		bytesDecoded += charBytesDecoded;
		charIndex++;
	}

	return bytesDecoded;
}


int AxString::GetStringAsUTF8Length(AxString &str)
{
	int bytesEncoded = 0;
	for (int i = 0; i < str.length; i++)
	{
		int utf8;
		bytesEncoded += AxString::EncodeUtf8Char(&utf8, str.contents[i]);
	}

	return bytesEncoded;
}

int AxString::GetUTF8AsStringLength(void *utfData, int utfDataLength)
{
	int charIndex = 0;
	int bytesDecoded = 0;
	while (bytesDecoded < utfDataLength)
	{
		unsigned int decodedChar;
		int charBytesDecoded = AxString::DecodeUtf8Char(decodedChar, (void*)((unsigned int)utfData + bytesDecoded));
		if (charBytesDecoded == 0)
			break;
		bytesDecoded += charBytesDecoded;
		charIndex++;
	}

	return charIndex;
}
