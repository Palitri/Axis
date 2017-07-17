#pragma once

#include "..\..\AxGlobals.h"

class AXDLLCLASS AxWindowsRegistry
{
private:
    void *openedKey;
public:
    static const unsigned int HKey_Classes_Root;
    static const unsigned int HKey_CurrentUser;
    static const unsigned int HKey_Local_Machine;
    static const unsigned int HKey_Users;


    AxWindowsRegistry();
    virtual ~AxWindowsRegistry();

    bool OpenKey(unsigned int hKey, wchar_t *key);
    void CloseKey();
    bool CreateKey(unsigned int hKey, wchar_t *key, wchar_t *defaultValue = 0);

    bool SetStringValue(wchar_t *valueName, wchar_t *value);
    bool GetStringValue(wchar_t *valueName, wchar_t **value);
};
