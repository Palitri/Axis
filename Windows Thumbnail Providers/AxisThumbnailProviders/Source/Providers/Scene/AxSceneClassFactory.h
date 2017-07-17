/****************************** Module Header ******************************\
Module Name:  ClassFactory.h
Project:      CppShellExtThumbnailHandler
Copyright (c) Microsoft Corporation.

The file declares the class factory for the RecipeThumbnailProvider COM class. 

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once

#include <unknwn.h>     // For IClassFactory
#include <windows.h>


class AxSceneClassFactory : public IClassFactory
{
public:
	// {0FE6BD32-C80E-495F-A255-6C1D1AFAFCAE}
	static const GUID CLSID_ThumbnailProvider;
	static const PCWSTR extensionName;
	static const PCWSTR friendlyName;
	//static const PCWSTR fileTypeMIME;


	// IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv);
    IFACEMETHODIMP_(ULONG) AddRef();
    IFACEMETHODIMP_(ULONG) Release();

    // IClassFactory
    IFACEMETHODIMP CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv);
    IFACEMETHODIMP LockServer(BOOL fLock);

    AxSceneClassFactory();

protected:
    ~AxSceneClassFactory();

private:
    long m_cRef;
};