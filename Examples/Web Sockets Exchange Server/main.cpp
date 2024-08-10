//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

// This example shows how to run Axis Exchange Server for Web Sockets
// Axis Exchange Server allows multiple clients to open a common transmission channel and send and receive data between each other
// This is a Web Sockets implementation of Axis Exchange Server and it augments the Axis Exchange interface with all features of Web Sockets

#include "AxisEngine.h"

#include <iostream>

int main(int argc, char* args[])
{
	AxWebSocketsExchangeServer myWebServer;
	myWebServer.RegisterComponent(new AxWebPageFileListing(), "FileListing");
	myWebServer.RegisterComponent(new AxWebPageHostName(), "HostName");
	myWebServer.SetRootPath("../Web");
	myWebServer.Open(90);

	//ShellExecute(NULL, "open", "http://localhost:90", NULL, NULL, SW_SHOWNORMAL);

	std::cout << "Axis WebSockets server is running at http://localhost:90\r\nPress Enter to exit\r\n";
	std::cin.get();

	return 0;
}