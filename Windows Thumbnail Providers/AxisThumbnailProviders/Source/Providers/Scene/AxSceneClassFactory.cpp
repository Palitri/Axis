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

#include "AxSceneClassFactory.h"
#include "AxSceneThumbnailProvider.h"
#include <new>
#include <Shlwapi.h>

#pragma comment(lib, "shlwapi.lib")


extern long g_cDllRef;


const GUID AxSceneClassFactory::CLSID_ThumbnailProvider = { 0xfe6bd32, 0xc80e, 0x495f, { 0xa2, 0x55, 0x6c, 0x1d, 0x1a, 0xfa, 0xfc, 0xae } }; // {0FE6BD32-C80E-495F-A255-6C1D1AFAFCAE}
const PCWSTR AxSceneClassFactory::extensionName = L".axs";
const PCWSTR AxSceneClassFactory::friendlyName = L"Axis Scene";
//const PCWSTR AxSceneClassFactory::fileTypeMIME = L"application/axis-scene1";


AxSceneClassFactory::AxSceneClassFactory() : m_cRef(1)
{
    InterlockedIncrement(&g_cDllRef);
}

AxSceneClassFactory::~AxSceneClassFactory()
{
    InterlockedDecrement(&g_cDllRef);
}


//
// IUnknown
//

IFACEMETHODIMP AxSceneClassFactory::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(AxSceneClassFactory, IClassFactory),
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

IFACEMETHODIMP_(ULONG) AxSceneClassFactory::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

IFACEMETHODIMP_(ULONG) AxSceneClassFactory::Release()
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

IFACEMETHODIMP AxSceneClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppv)
{
    HRESULT hr = CLASS_E_NOAGGREGATION;

    // pUnkOuter is used for aggregation. We do not support it in the sample.
    if (pUnkOuter == NULL)
    {
        hr = E_OUTOFMEMORY;

        // Create the COM component.
        AxSceneThumbnailProvider *pExt = new (std::nothrow) AxSceneThumbnailProvider();
        if (pExt)
        {
            // Query the specified interface.
            hr = pExt->QueryInterface(riid, ppv);
            pExt->Release();
        }
    }

    return hr;
}

IFACEMETHODIMP AxSceneClassFactory::LockServer(BOOL fLock)
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