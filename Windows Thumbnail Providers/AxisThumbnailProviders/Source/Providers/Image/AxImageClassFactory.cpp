/****************************** Module Header ******************************\
Module Name:  ClassFactory.cpp
Project:      CppShellExtThumbnailHandler
Copyright (c) Microsoft Corporation.

The file implements the class factory for the RecipeThumbnailProvider COM class. 

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#include "AxImageClassFactory.h"
#include "AxImageThumbnailProvider.h"
#include <new>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


extern long g_cDllRef;


const GUID AxImageClassFactory::CLSID_ThumbnailProvider = { 0x8a1d2a65, 0xd4e1, 0x440a, { 0x82, 0x7c, 0xd6, 0xab, 0xfc, 0x7e, 0xcf, 0x92 } }; // {8A1D2A65-D4E1-440A-827C-D6ABFC7ECF92}
const PCWSTR AxImageClassFactory::extensionName = L".axi";
const PCWSTR AxImageClassFactory::friendlyName = L"Axis Image";
//const PCWSTR AxImageClassFactory::fileTypeMIME = L"image/axis-image1";


AxImageClassFactory::AxImageClassFactory() : m_cRef(1)
{
    InterlockedIncrement(&g_cDllRef);
}

AxImageClassFactory::~AxImageClassFactory()
{
    InterlockedDecrement(&g_cDllRef);
}


//
// IUnknown
//

IFACEMETHODIMP AxImageClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(AxImageClassFactory, IClassFactory),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) AxImageClassFactory::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) AxImageClassFactory::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }
    return cRef;
}


// 
// IClassFactory
//

IFACEMETHODIMP AxImageClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_NOAGGREGATION;

    // pUnkOuter is used for aggregation. We do not support it in the sample.
    if (pUnkOuter == NULL)
    {
        hr = E_OUTOFMEMORY;

        // Create the COM component.
        AxImageThumbnailProvider *pExt = new (std::nothrow) AxImageThumbnailProvider();
        if (pExt)
        {
            // Query the specified interface.
            hr = pExt->QueryInterface(riid, ppv);
            pExt->Release();
        }
    }

    return hr;
}

IFACEMETHODIMP AxImageClassFactory::LockServer(BOOL fLock)
{
    if (fLock)
    {
        InterlockedIncrement(&g_cDllRef);
    }
    else
    {
        InterlockedDecrement(&g_cDllRef);
    }
    return S_OK;
}