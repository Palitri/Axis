//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWindowsRegistry.h"

#include <windows.h>
#include <strsafe.h>

const unsigned int AxWindowsRegistry::HKey_Classes_Root = (LONG)HKEY_CLASSES_ROOT;
const unsigned int AxWindowsRegistry::HKey_CurrentUser = (LONG)HKEY_CURRENT_USER;
const unsigned int AxWindowsRegistry::HKey_Local_Machine = (LONG)HKEY_LOCAL_MACHINE;
const unsigned int AxWindowsRegistry::HKey_Users = (LONG)HKEY_USERS;

AxWindowsRegistry::AxWindowsRegistry()
{
    this->openedKey = 0;
}

AxWindowsRegistry::~AxWindowsRegistry()
{
	this->CloseKey();
}


bool AxWindowsRegistry::OpenKey(unsigned int hKey, wchar_t *key)
{
    this->CloseKey();

    HRESULT hr;
    HKEY openedKey = NULL;
    HKEY root = (( HKEY ) (ULONG_PTR)((LONG)hKey) );

    hr = HRESULT_FROM_WIN32(RegCreateKeyExW(root, key, 0,
        NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &openedKey, NULL));

    if (FAILED(hr))
        return false;

    this->openedKey = new HKEY;
    *((HKEY*)this->openedKey) = openedKey;

    return true;
}

void AxWindowsRegistry::CloseKey()
{
    if (this->openedKey == 0)
        return;

    RegCloseKey(*((HKEY*)this->openedKey));
    delete this->openedKey;

    this->openedKey = 0;
}

bool AxWindowsRegistry::CreateKey(unsigned int hKey, wchar_t *key, wchar_t *defaultValue)
{
    if (!this->OpenKey(hKey, key))
        return false;

    if (defaultValue != 0)
        this->SetStringValue(0, defaultValue);

    this->CloseKey();

    return true;
}

// If valueName is null, the key's default value is assumed
bool AxWindowsRegistry::SetStringValue(wchar_t *valueName, wchar_t *value)
{
    if ((this->openedKey == 0) || (value == 0))
        return false;

    HRESULT hr;
    DWORD dataLength = lstrlenW(value) * sizeof(*value);
    hr = HRESULT_FROM_WIN32(RegSetValueExW(*((HKEY*)this->openedKey), valueName, 0,
        REG_SZ, reinterpret_cast<const BYTE *>(value), dataLength));

    return SUCCEEDED(hr);
}

// If valueName is null, the key's default value is assumed
bool AxWindowsRegistry::GetStringValue(wchar_t *valueName, wchar_t **value)
{
    if (this->openedKey == 0)
        return false;

    HRESULT hr;
    PWSTR dataBuffer[1024];
    DWORD dataLength;
    hr = HRESULT_FROM_WIN32(RegQueryValueExW(*((HKEY*)this->openedKey), valueName, NULL,
        NULL, reinterpret_cast<LPBYTE>(dataBuffer), &dataLength));

    *value = new wchar_t[dataLength];
    memcpy(*value, dataBuffer, dataLength);

    return SUCCEEDED(hr);
}
