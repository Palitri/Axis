#pragma once

#include "..\AxGlobals.h"

// TODO Consider moving the member variables to private

class AXDLLCLASS AxString
{
private:
	void Initialize();

public:
	wchar_t &operator [] (int index);

	// AxString operators
	void operator = (const AxString &argument);
	void operator += (const AxString &argument);
	AxString operator + (const AxString &argument);
	bool operator == (const AxString &argument);
	bool operator != (const AxString &argument);
	bool operator > (const AxString &argument);
	bool operator >= (const AxString &argument);
	bool operator < (const AxString &argument);
	bool operator <= (const AxString &argument);

	// WChar operators
	void operator = (const wchar_t *argument);
	void operator += (const wchar_t *argument);
	AxString operator + (const wchar_t *argument);
	bool operator == (const wchar_t *argument);
	bool operator != (const wchar_t *argument);
	bool operator > (const wchar_t *argument);
	bool operator >= (const wchar_t *argument);
	bool operator < (const wchar_t *argument);
	bool operator <= (const wchar_t *argument);

	// Char operators
	void operator = (const char *argument);
	void operator += (const char *argument);
	AxString operator + (const char *argument);
	bool operator == (const char *argument);
	bool operator != (const char *argument);
	bool operator > (const char *argument);
	bool operator >= (const char *argument);
	bool operator < (const char *argument);
	bool operator <= (const char *argument);

	int length, capacity;
	wchar_t *contents;
	
	int charContentsCapacity;
	bool charContentsChanged;
	char *charContents;

	AxString(void);
	AxString(AxString &str);
	AxString(const char *str);
	AxString(const wchar_t *str);
	AxString(const char *str, int length);
	AxString(const wchar_t *str, int length);
	AxString(char str);
	AxString(wchar_t str);
	AxString(int value, int radix = 10);
	AxString(unsigned int value, int radix = 10);
	AxString(long long value, int radix = 10);
	AxString(unsigned long long value, int radix = 10);
	AxString(double value);
	~AxString(void);

	// Sets capacity exactly to the desired amount	
	void ResizeCapacity(int newCapacity, bool keepContents = true);
	// Makes sure there is the amount of capacity deisred. If the desired capacity already fits in the current one, no change is made
	void EnsureCapacity(int length);
	// Sets the length of the string. If the new length is greater than the current one, the added characters are undefined
	void SetLength(int length);
	int GetLength();

	bool IsEmpty();
	bool IsLowerCase();
	bool IsUpperCase();
	
	const wchar_t *GetContents();
	const char *GetCharContents();

	void InsertCharacters(int position, int count);

	void CopyCharacters(int offset, const AxString &source, int sourceOffset, int count);
	void CopyCharacters(int offset, const char *source, int sourceOffset, int count);
	void CopyCharacters(int offset, const wchar_t *source, int sourceOffset, int count);

	void FillCharacters(int offset, const char fillChar, int count);
	void FillCharacters(int offset, const wchar_t fillChar, int count);

	int Compare(const AxString &str);
	int Compare(const char *str);
	int Compare(const wchar_t *str);

	bool Equals(const AxString &str);
	bool Equals(const char *str);
	bool Equals(const wchar_t *str);

	void Copy(const AxString &source);
	void Copy(const char *source);
	void Copy(const wchar_t *source);

	void Insert(const AxString &source, int position = -1, int count = -1);
	void Insert(const char *source, int position = -1, int count = -1);
	void Insert(const wchar_t *source, int position = -1, int count = -1);

	int IndexOf(const AxString &substr, int start = 0);
	int IndexOf(const char *substr, int start = 0);
	int IndexOf(const wchar_t *substr, int start = 0);

	int LastIndexOf(const AxString &substr, int start = -1);
	int LastIndexOf(const char *substr, int start = -1);
	int LastIndexOf(const wchar_t *substr, int start = -1);

	AxString SubString(int start, int count);
	AxString Remove(int start, int count);

	AxString Remove(const AxString &str);
	AxString Remove(const char *str);
	AxString Remove(const wchar_t *str);

	AxString Replace(const AxString &str, const AxString &substitute);
	AxString Replace(const char *str, const char *substitute);
	AxString Replace(const wchar_t *str, const wchar_t *substitute);

	int FirstDifferenceIndex(const AxString &str);
	int FirstDifferenceIndex(const char *str);
	int FirstDifferenceIndex(const wchar_t *str);

	AxString PadLeft(int minLength, const char fillChar);
	AxString PadLeft(int minLength, const wchar_t fillChar);

	AxString PadRight(int minLength, const char fillChar);
	AxString PadRight(int minLength, const wchar_t fillChar);

	bool StartsWith(const AxString &str);
	bool StartsWith(const char *str);
	bool StartsWith(const wchar_t *str);
	bool StartsWith(const char chr);
	bool StartsWith(const wchar_t chr);
	
	bool EndsWith(const AxString &str);
	bool EndsWith(const char *str);
	bool EndsWith(const wchar_t *str);
	bool EndsWith(const char chr);
	bool EndsWith(const wchar_t chr);

	AxString Trim(const char chr, bool left = true, bool right = true);
	AxString Trim(const wchar_t chr, bool left = true, bool right = true);
	
	bool ToInt(int &value, int radix = 10);
	bool ToUInt(unsigned int &value, int radix = 10);
	bool ToFloat(float &value);
	bool ToDouble(double &value);

	AxString ToLower();
	AxString ToUpper();

	AxString ToBase64();
	AxString FromBase64();

	static char *CopyNew(const char *source);
	static wchar_t *CopyNew(const wchar_t *source);
	static void Dispose(char **str);
	static void Dispose(wchar_t **str);

	static int GetLength(const wchar_t *str);
	static int GetLength(const char *str);

	static int ToInt(char *str, char **end, int radix = 10);
	static int ToInt(wchar_t *str, wchar_t **end, int radix = 10);
	static unsigned int ToUInt(char *str, char **end, int radix = 10);
	static unsigned int ToUInt(wchar_t *str, wchar_t **end, int radix = 10);
	static double ToDouble(char *str, char **end);
	static double ToDouble(wchar_t *str, wchar_t **end);

	static AxString ToBase64(const void *data, unsigned int dataSize);
	static bool FromBase64(AxString &base64String, void **data, unsigned int &dataSize);

	static AxString ToBaseX(const void *data, unsigned int dataSize, AxString alphabet);

	static unsigned int EncodeUtf8Char(void *result, unsigned int codePoint);
	static unsigned int DecodeUtf8Char(unsigned int &result, void *utf);

	static int EncodeUtf8String(void *result, AxString &str);
	static int DecodeUtf8String(AxString &result, void *utfData, int utfDataLength);

	static int GetStringAsUTF8Length(AxString &str);
	static int GetUTF8AsStringLength(void *utfData, int utfDataLength);

	//static AxString Format(const wchar_t *formatString, ...);
};

