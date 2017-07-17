/******************************** Module Header ********************************\
Module Name:  RecipeThumbnailProvider.cpp
Project:      CppShellExtThumbnailHandler
Copyright (c) Microsoft Corporation.

The code sample demonstrates the C++ implementation of a thumbnail handler 
for a new file type registered with the .recipe extension. 

A thumbnail image handler provides an image to represent the item. It lets you 
customize the thumbnail of files with a specific file extension. Windows Vista 
and newer operating systems make greater use of file-specific thumbnail images 
than earlier versions of Windows. Thumbnails of 32-bit resolution and as large 
as 256x256 pixels are often used. File format owners should be prepared to 
display their thumbnails at that size. 

The example thumbnail handler implements the IInitializeWithStream and 
IThumbnailProvider interfaces, and provides thumbnails for .recipe files. 
The .recipe file type is simply an XML file registered as a unique file name 
extension. It includes an element called "Picture", embedding an image file. 
The thumbnail handler extracts the embedded image and asks the Shell to 
display it as a thumbnail.

This source is subject to the Microsoft Public License.
See http://www.microsoft.com/opensource/licenses.mspx#Ms-PL.
All other rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\*******************************************************************************/

#include "AxSceneThumbnailProvider.h"
#include "..\..\Utils\DIBBitmap.h"
#include "..\..\Utils\AxPixelFormat.h"
#include "..\..\Utils\AxChunkId.h"

#include <Shlwapi.h>
//#include <Wincrypt.h>   // For CryptStringToBinary.
//#include <msxml6.h>

#pragma comment(lib, "Shlwapi.lib")
//#pragma comment(lib, "Crypt32.lib")
//#pragma comment(lib, "msxml6.lib")


extern HINSTANCE g_hInst;
extern long g_cDllRef;


AxSceneThumbnailProvider::AxSceneThumbnailProvider() : m_cRef(1), m_pStream(NULL)
{
    InterlockedIncrement(&g_cDllRef);
}


AxSceneThumbnailProvider::~AxSceneThumbnailProvider()
{
    InterlockedDecrement(&g_cDllRef);
}


#pragma region IUnknown

// Query to the interface the component supported.
IFACEMETHODIMP AxSceneThumbnailProvider::QueryInterface(REFIID riid, void **ppv)
{
    static const QITAB qit[] = 
    {
        QITABENT(AxSceneThumbnailProvider, IThumbnailProvider),
        QITABENT(AxSceneThumbnailProvider, IInitializeWithStream), 
        { 0 },
    };
    return QISearch(this, qit, riid, ppv);
}

// Increase the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) AxSceneThumbnailProvider::AddRef()
{
    return InterlockedIncrement(&m_cRef);
}

// Decrease the reference count for an interface on an object.
IFACEMETHODIMP_(ULONG) AxSceneThumbnailProvider::Release()
{
    ULONG cRef = InterlockedDecrement(&m_cRef);
    if (0 == cRef)
    {
        delete this;
    }

    return cRef;
}

#pragma endregion


#pragma region IInitializeWithStream

// Initializes the thumbnail handler with a stream.
IFACEMETHODIMP AxSceneThumbnailProvider::Initialize(IStream *pStream, DWORD grfMode)
{
    // A handler instance should be initialized only once in its lifetime. 
    HRESULT hr = HRESULT_FROM_WIN32(ERROR_ALREADY_INITIALIZED);
    if (m_pStream == NULL)
    {
        // Take a reference to the stream if it has not been initialized yet.
        hr = pStream->QueryInterface(&m_pStream);
    }
    return hr;
}

#pragma endregion


#pragma region IThumbnailProvider

// Gets a thumbnail image and alpha type. The GetThumbnail is called with the 
// largest desired size of the image, in pixels. Although the parameter is 
// called cx, this is used as the maximum size of both the x and y dimensions. 
// If the retrieved thumbnail is not square, then the longer axis is limited 
// by cx and the aspect ratio of the original image respected. On exit, 
// GetThumbnail provides a handle to the retrieved image. It also provides a 
// value that indicates the color format of the image and whether it has 
// valid alpha information.
IFACEMETHODIMP AxSceneThumbnailProvider::GetThumbnail(UINT cx, HBITMAP *phbmp,  WTS_ALPHATYPE *pdwAlpha)
{
	//*pdwAlpha = WTSAT_ARGB;
	//phbmp = 0;
	//return E_FAIL;

	if (this->m_pStream == NULL)
		return E_FAIL;

	ULONG bytesRead;
	ULARGE_INTEGER currentPos;
	LARGE_INTEGER seek;
	unsigned int chunkID;
	long long chunkSize;
	long long mainChunkEnd = -1;

	int thumbnailWidth = 0;
	int thumbnailHeight = 0;
	int thumbnailDepth = 0;
	unsigned int thumbnailDataSize = 0;
	unsigned char *thumbnailData = 0;
	AxPixelFormat thumbnailPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdAlpha, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdBlue);

	while (this->m_pStream->Read(&chunkID, 4, &bytesRead) == S_OK)
	{
		if (this->m_pStream->Read(&chunkSize, 8, &bytesRead) != S_OK)
		{
			this->m_pStream->Release();
			this->m_pStream = 0;

			return E_FAIL;
		}

		seek.QuadPart = 0;
		this->m_pStream->Seek(seek, STREAM_SEEK_CUR, &currentPos);

		switch (chunkID)
		{
			case AxChunkId::SerializationId_Main:
			{
				mainChunkEnd = currentPos.QuadPart + chunkSize;
				
				break;
			}

			case AxChunkId::SerializationId_Thumbnail:
			case AxChunkId::ImageSerializationId_Main:
			{
				break;
			}

			case AxChunkId::ImageSerializationId_Size:
			{
				this->m_pStream->Read(&thumbnailWidth, 4, &bytesRead);
				this->m_pStream->Read(&thumbnailHeight, 4, &bytesRead);
				this->m_pStream->Read(&thumbnailDepth, 4, &bytesRead);

				break;
			}

			case AxChunkId::ImageSerializationId_Format:
			{
				this->m_pStream->Read(&thumbnailPixelFormat.numChannels, 2, &bytesRead);
				this->m_pStream->Read(&thumbnailPixelFormat.sizeInBytes, 2, &bytesRead);
				this->m_pStream->Read(thumbnailPixelFormat.channelId, 4, &bytesRead);
				this->m_pStream->Read(thumbnailPixelFormat.channelBitCount, 4, &bytesRead);

				break;
			}

			case AxChunkId::ImageSerializationId_Data:
			{
				thumbnailDataSize = thumbnailWidth * thumbnailHeight * 4;
				thumbnailData = new unsigned char[thumbnailDataSize];
				this->m_pStream->Read(thumbnailData, thumbnailDataSize, &bytesRead);

				break;
			}

			default:
			{
				if ((chunkSize < 0) || (currentPos.QuadPart + chunkSize > mainChunkEnd))
				{
					this->m_pStream->Release();
					this->m_pStream = 0;

					return E_FAIL;
				}

				seek.QuadPart = chunkSize;
				this->m_pStream->Seek(seek, STREAM_SEEK_CUR, &currentPos);

				break;
			}
		}

		if ((thumbnailData != 0) ||(currentPos.QuadPart >= mainChunkEnd))
			break;
	}

	this->m_pStream->Release();
	this->m_pStream = 0;

	if (thumbnailDataSize == 0)
		return E_FAIL;

	*phbmp = DIBBitmap::CreateBitmap(thumbnailWidth, thumbnailHeight, thumbnailPixelFormat, thumbnailData);
	*pdwAlpha = WTSAT_ARGB;

	delete[] thumbnailData;

	if (phbmp == NULL)
		return E_FAIL;

	return S_OK;
}

#pragma endregion
