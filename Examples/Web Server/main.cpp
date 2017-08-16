//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

// This example shows how to run Axis Web Server
// The web server is intended for web interface of game servers.
// For example, if your game has a multiplayer, you could use the web server to create a web-accessible interface with settings, statistics or whatever.
// This might be particularly useful for games with dedicated servers.
//
// The web server can display dynamic content via its registered components (one component for example is "AxWebPageFileListing")
// To make the web server display the content you require, create a class which inherits "AxWebPageComponent" and register it using AxWebServer.RegisterComponent
// By overriding the component's output method, you can program its html output.
// Components are placed inside of html web pages simply as <axw> tags with a "component" attribute of value the component's name. For example <axw component="FileListing"/>
// See the contents of the "Engine\Web" folder, and the "index.hlml" file for a particular example.

#include "AxisEngine.h"

#include <iostream>
#include <Windows.h>

int main(int argc, char *args[])
{
	AxWebServer myWebServer;
	myWebServer.RegisterComponent(new AxWebPageFileListing(), "FileListing");
	myWebServer.RegisterComponent(new AxWebPageHostName(), "HostName");
	myWebServer.SetRootPath("../Web");
	myWebServer.Open(81);

	ShellExecute(NULL, "open", "http://localhost:81", NULL, NULL, SW_SHOWNORMAL);

	std::cout << "Axis web server is running at http://localhost:81\r\nPress Enter to exit\r\n";
	std::cin.get();

	myWebServer.Close();

	return 0;
}