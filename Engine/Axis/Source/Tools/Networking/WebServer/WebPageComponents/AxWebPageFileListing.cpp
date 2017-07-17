//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWebPageFileListing.h"

#include "..\..\..\Platform\AxPlatformUtils.h"
#include "..\..\..\AxStrings.h"
#include "..\AxWebServer.h"

AxWebPageFileListing::AxWebPageFileListing(void)
	: AxWebPageComponent()
{
}


AxWebPageFileListing::~AxWebPageFileListing(void)
{
}

AxString AxWebPageFileListing::Output(AxWebRequest *request)
{
	AxString result = "<table class=\"fileListing\">";

	const char*lineStyleNames[2] =  { "fileListing_line_even", "fileListing_line_odd" };

	AxList<AxFileInfo> filesInfo;
	request->server->fileSystem->GetDirectoryContents(request->server->rootPath + "\\" + request->originalPath, filesInfo);

	int lineCounter = 0;
	for (int i = 0; i < filesInfo.count; i++)
		if (filesInfo[i].isDirectory)
			result += AxString("<tr class=\"") + lineStyleNames[lineCounter++ % 2] + "\"><td class=\"fileListing_name\"><a href=\"" + filesInfo[i].fileName + "/\">" + filesInfo[i].fileName + "</a></td><td class=\"fileListing_size\">DIR</td></tr>";

		for (int i = 0; i < filesInfo.count; i++)
		if (!filesInfo[i].isDirectory)
			result += AxString("<tr class=\"") + lineStyleNames[lineCounter++ % 2] + "\"><td class=\"fileListing_name\"><a href=\"" + filesInfo[i].fileName + "\">" + filesInfo[i].fileName + "</a></td><td class=\"fileListing_size\">" + request->server->fileSystem->GetSymbolicSize(filesInfo[i].size) + "</td></tr>";
	
	result += "</table>";

	return AxString(result);
}
