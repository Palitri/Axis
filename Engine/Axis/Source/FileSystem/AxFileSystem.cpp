//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxFileSystem.h"

#include "..\Tools\AxMath.h"
#include "..\Tools\AxStrings.h"

const wchar_t AxFileSystem::dirSplitter = L'/';
const wchar_t AxFileSystem::extensionSplitter = L'.';
const wchar_t AxFileSystem::rootSignature = L':';
const wchar_t *AxFileSystem::previousDir = L"..";
const wchar_t *AxFileSystem::currentDir = L".";

AxFileSystem::AxFileSystem(void)
{
}


AxFileSystem::~AxFileSystem(void)
{
}

bool AxFileSystem::FileExists(AxString path)
{
	AxFileInfo fileInfo;

	bool exists = this->GetFileInfo(path, fileInfo);

	return exists && (!fileInfo.isDirectory);
}

bool AxFileSystem::DirectoryExists(AxString path)
{
	AxFileInfo fileInfo;

	bool exists = this->GetFileInfo(path, fileInfo);

	return exists && (fileInfo.isDirectory);
}

bool AxFileSystem::FileOrDirectoryExists(AxString path)
{
	AxFileInfo fileInfo;
	return this->GetFileInfo(path, fileInfo);
}

bool AxFileSystem::IsAbsolutePath(AxString path)
{
	int firstDelimiter = path.IndexOf(AxFileSystem::dirSplitter);
	if (firstDelimiter == -1)
		firstDelimiter = path.length;
	return path.IndexOf(AxFileSystem::rootSignature) != -1;
}

AxString AxFileSystem::GetSymbolicSize(unsigned long long size, int magnitudeBase, int magnitudeThreshold)
{
	const int numMagnitudes = 6;
	const char *magnitudeNames[numMagnitudes] = { "B", "KB", "MB", "GB", "TB", "PB" };

	unsigned long long threshold = magnitudeBase * magnitudeThreshold;
	int magnitudeIndex = 0;
	while ((size > threshold) && (magnitudeIndex < numMagnitudes))
	{
		size /= magnitudeBase;
		magnitudeIndex++;
	}

	return AxString(AxString((int)size) + " " + magnitudeNames[magnitudeIndex]);
}

AxString AxFileSystem::GetRelativeFilePath(AxString &basePath, AxString &path)
{
	if (basePath.length == 0)
        return AxString(path);

	AxString fullBasePath = basePath;
	if (!fullBasePath.EndsWith(AxFileSystem::dirSplitter))
		fullBasePath += AxFileSystem::dirSplitter;

    // Find the delimiter, until which path and fullBasePath are the same
	int delimiterIndex = -1;
    int index = 0;
    int length = AxMath::Min(fullBasePath.length, path.length);
    while (index < length)
    {
        if (path[index] == fullBasePath[index])
        {
			if (path[index] == AxFileSystem::dirSplitter)
            {
                delimiterIndex = index;
            }
        }
        else
            break;

        index++;
    }

    AxString result;

    // If the file is not inside of the the fullPath directory (or in an inner directory), then add a back path
	if (delimiterIndex != (fullBasePath.length - 1))
    {
        int pos = delimiterIndex;
        do
        {
			pos = fullBasePath.IndexOf(AxFileSystem::dirSplitter, pos + 1);
            if (pos != -1)
				result += AxString(AxFileSystem::previousDir) + AxString(AxFileSystem::dirSplitter);
            else
                break;
        }
        while (true);
    }

    result += path.Remove(0, delimiterIndex + 1);

    return AxString(result);
}

AxString AxFileSystem::NormalizePath(AxString &path)
{
	AxStrings pathSegments;
	pathSegments.SplitString(path, AxFileSystem::dirSplitter);

	int *normalizedSegments = pathSegments.count > 0 ? new int[pathSegments.count] : 0;
	int numNormalizedSegments = 0;

	for (int segmentIndex = 0; segmentIndex < pathSegments.count; segmentIndex++)
	{
		if (pathSegments[segmentIndex].Equals(AxFileSystem::currentDir))
		{
		}

		else if (pathSegments[segmentIndex].Equals(AxFileSystem::previousDir))
		{
			if (numNormalizedSegments > 0)
				numNormalizedSegments--;
		}

		else
		{
			normalizedSegments[numNormalizedSegments] = segmentIndex;
			numNormalizedSegments++;
		}
	}


	AxString result;

	// Optimize, so that the string's length is not changed for every segment
	int resultLength = AxMath::Max(numNormalizedSegments - 1, 0);
	for (int segmentIndex = 0; segmentIndex < numNormalizedSegments; segmentIndex++)
		resultLength += pathSegments[normalizedSegments[segmentIndex]].length;

	result.EnsureCapacity(resultLength);

	for (int segmentIndex = 0; segmentIndex < numNormalizedSegments; segmentIndex++)
	{
		result += pathSegments[normalizedSegments[segmentIndex]];
		if (segmentIndex != numNormalizedSegments - 1)
			result += AxFileSystem::dirSplitter;
	}

	if (normalizedSegments != 0)
		delete[] normalizedSegments;

	return AxString(result);
}

AxString AxFileSystem::GetFileName(AxString &path)
{
	int index = path.LastIndexOf(AxFileSystem::dirSplitter) + 1;

	return AxString(path.SubString(index, path.length - index));
}

AxString AxFileSystem::GetFileExtension(AxString &path)
{
	int index = path.LastIndexOf(AxFileSystem::extensionSplitter);
	if ((index == -1) || (index < path.LastIndexOf(AxFileSystem::dirSplitter)))
		return AxString();
	
	index++;

	return AxString(path.SubString(index, path.length - index));
}

AxString AxFileSystem::GetFileNameWithoutExtension(AxString &path)
{
	int start = path.LastIndexOf(AxFileSystem::dirSplitter) + 1;

	int end = path.LastIndexOf(AxFileSystem::extensionSplitter);
	if ((end == -1) || (end < start))
		end = path.length;
	
	return AxString(path.SubString(start, end - start));
}

AxString AxFileSystem::CreateFileNameWithExtension(AxString &fileName, AxString &extension)
{
	int splitterPos = fileName.LastIndexOf(AxFileSystem::extensionSplitter);
	if (splitterPos != 0)
	{
		if (splitterPos > fileName.LastIndexOf(AxFileSystem::dirSplitter))
		{
			return AxString(fileName.SubString(0, splitterPos) + "." + extension);
		}
	}

	return AxString(fileName + "." + extension);
}

AxString AxFileSystem::GetFileDir(AxString &path)
{
	return AxString(path.SubString(0, path.LastIndexOf(AxFileSystem::dirSplitter)));
}

AxString AxFileSystem::MergePaths(AxString &path1, AxString &path2)
{
	return AxString(AxFileSystem::NormalizePath(path1 + AxFileSystem::dirSplitter + path2));
}