#pragma once

#include "..\..\..\AxGlobals.h"

#include "AxWebRequest.h"
#include "AxWebResponse.h"

#include "WebPageComponents\AxWebPageComponent.h"

#include "..\..\AxDictionary.h"
#include "..\TCPIP\AxTcpIpServer.h"

#include "..\..\..\FileSystem\PhysicalFileSystem\AxPhysicalFileSystem.h"


struct WebServerSession
{
	void *data;
	long long time;
};

AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxDictionary<unsigned int, WebServerSession*>;
AXDLLTEMPLATEMEMBER template class AXDLLCLASS AxList<AxWebPageComponent*>;

class AXDLLCLASS AxWebServer :
	public AxTcpIpServer
{
private:
	bool defaultFileSystem;
	AxDictionary<unsigned int, WebServerSession*> sessions;

	void GetSession(AxWebResponse *response);

	void SendResponse(AxWebResponse *response);
	bool GetFileContents(AxWebRequest *request, AxStream *responseContents);
	bool GenerateHtmlContents(AxString requestFile, AxWebRequest *request, AxStream *responseContents);
	
	void GenerateHtmlResponse(AxWebResponse *response, AxString headerCode, AxString headerAppendix, AxString fileName, AxString backupHtmlCode);

public:
	AxFileSystem *fileSystem;
	float sessionDurationSeconds;

	AxString rootPath;

	AxList<AxWebPageComponent*> components;

	AxWebServer(void);
	~AxWebServer(void);

	virtual void OnConnectionReceive(AxTcpIpConnection *connection, void *data, int dataSize);

	// Creates a response with http code 303, which makes the client redirect to the given url
	void Redirect(AxWebResponse *response, AxString url);
	// Creates a response with http code 401, which makes the client ask the user for a user name and a password. The following requests should contain the username and password entered.
	void RequestAuthorization(AxWebResponse *response, AxString message);
	// Creates a response with http code 403, which denotes that the client has been rejected access to the requested resource.
	void Reject(AxWebResponse *response);

	// Sets the file system, which the server is to acquire the requested resources
	void SetFileSystem(AxFileSystem *fileSystem);
	// Sets the root path where the html files are located
	void SetRootPath(AxString rootPath);

	// Registers a component with dynamic page content
	void RegisterComponent(AxWebPageComponent *component, AxString componentName);

	// Ends a session. Next time the user with that session makes a request, a new session will me made. Returns true if a session with the given session id existed, false otherwise
	bool EndSession(unsigned int sessionId);	

	// Provides the ability to reroute the url, which was originally requested by the client.
	// The return value of this method is stored in the AxWebRequest.path, while the originally requested url is stored in AxWebRequest.originalPath
	virtual AxString RerouteRequest(AxString originalUrl);
	// Provides the ability to intercept and "steal" a web request. If this method returns true, then the request will not be futherly processed by the standard pipeline of the web server.
	// It is a convenient place to ask the client for authorization, to redirect, reject or do anything before the standard processing happens
	virtual bool InterceptRequest(AxWebResponse *response);
	// Gets the type of the requested resource outputs it in "responseType". Requested resource is the name of the resource and may include full path. Returns true if the type is found and supported, false otherwise.
	// If this method returns false, normally the web server will return an error that the requested resource is not of supported type.
	virtual bool GetContentType(AxString requestedResource, AxString &responseType);

	// Occurs when a new session is created. The method can be overridden to create an object to hold data for the session and return it as a result.
	virtual void *OnCreateSession(AxWebRequest *request);
	// Occurs when a session is ending. If an object has been created in OnCreateSession, it should be freed here.
	virtual void OnEndSession(void *session);
	
	// Http code 200. Occurs just before the server is to respond to a request. Method parameters hold the data, which is to be used in the response.
	virtual void OnResourceResponse(AxWebResponse *response);
	// Http code 500. Occurs just before the server is to respond that some general error prevents it from performing normally. For example, an error while parsing html code file, or not succeeding to return a resource without any definite cause.
	virtual void OnInternalError(AxWebResponse *response);
	// Http code 404. Occurs just before the server is to responded that a requested resource was not found. Method parameters hold the data, which is to be used in the response.
	virtual void OnResourceNotFound(AxWebResponse *response);
	// Http code 415. Occurs just before the server is to responded that a requested resource is not of a supported type. 
	// This occurs when the requested resource type is not in the server's supportedTypes list or when the GetContentType method returns false for the requested resource. Method parameters hold the data, which is to be used in the response.
	virtual void OnContentTypeNotSupported(AxWebResponse *response);
	// Http code 501. Occurs just before the server is to responded that an HTTP method is not supported by the server. Method parameters hold the data, which is to be used in the response.
	virtual void OnHttpMethodNotImplemented(AxWebResponse *response);
};

