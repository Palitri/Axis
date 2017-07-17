#pragma once

#include "..\AxGlobals.h"

#include "AxFileInfo.h"
#include "..\Tools\AxString.h"
#include "..\Tools\AxList.h"
#include "..\Tools\Streams\AxStream.h"

class AXDLLCLASS AxFileSystem
{
public:
	static const wchar_t dirSplitter;
	static const wchar_t extensionSplitter;
	static const wchar_t rootSignature;
	static const wchar_t *previousDir;
	static const wchar_t *currentDir;

	AxFileSystem(void);
	virtual ~AxFileSystem(void);

	// Opens a file for reading
	virtual AxStream *OpenFile(AxString path) = 0;
	// Creates a file for writing
	virtual AxStream *CreateNewFile(AxString path) = 0;

	// Gets information on the specified file. Returns true if succeeded or false otherwise (for example if the file does not exist)
	virtual bool GetFileInfo(AxString path, AxFileInfo &result) = 0;
	// Gets information on the contents of the specified directory. Returns true if succeeded or false otherwise (for example if the directory does not exist)
	virtual bool GetDirectoryContents(AxString path, AxList<AxFileInfo> &result) = 0;

	// Returns true if the given file exists or false otherwise
	bool FileExists(AxString path);
	// Returns true if the given directory exists or false otherwise
	bool DirectoryExists(AxString path);
	// Returns true if the given path is of an existing file or directory and false otherwise
	bool FileOrDirectoryExists(AxString path);

	static bool IsAbsolutePath(AxString path);
	// Returns a string containing a symbolic size of the given one, shortening large numbers using their magnitudes. For example, an input size of 10485760 may return simply "10 MB".
	// The "magnitudeBase" parameter allows to use different magnitudes, such as 1024 or 1000. The "magnitudeThreshold" determines the greatest value in number of magnitudes to be displayed in smaller magnitude
	static AxString GetSymbolicSize(unsigned long long size, int magnitudeBase = 1024, int magnitudeThreshold = 4);
	// Normalizes a path, rebuilding it regarding and removing the "." and ".." directories
	static AxString NormalizePath(AxString &path);
	// Creates a path, which is relative to the one given in "basePath". In other words, how to go from "basePath" to "path", or how to reach "path", starting from "basePath". In arithmetical terms, basePath + GetRelativeFilePath(basePath, path) should be equivalent, but not necessarily equal, to path.
	// For example, if path is "C:\Files\File.txt", and basePath is "C:", the result will be "Files\File.txt", and if basePath is "C:\Files\Subdir", the result will be "..\File.txt"
	static AxString GetRelativeFilePath(AxString &basePath, AxString &path);
	// Returns only a file name with its extension from a given path. For example for "C:\Files\File.txt", the method will return "File.txt"
	static AxString GetFileName(AxString &path);
	// Returns only the file extension, if any, of the given path. For example for "C:\Files\File.txt", the method will return "txt"
	static AxString GetFileExtension(AxString &path);
	// Returns only a file name without its extension from a given path. For example for "C:\Files\File.txt", the method will return "File"
	static AxString GetFileNameWithoutExtension(AxString &path);
	// Returns only the directory of a given path. For example for "C:\Files\File.txt", the method will return "C:\Files".
	static AxString GetFileDir(AxString &path);
	// Creates a file name with the given extension. If the given file name has an extension, then it will be changed to the new one, otherwise the new one will be appended
	static AxString CreateFileNameWithExtension(AxString &fileName, AxString &extension);
	// Merges two paths and normalizes the result
	static AxString MergePaths(AxString &path1, AxString &path2);
};

