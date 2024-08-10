//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxWebServer.h"

#include "..\..\AxMath.h"
#include "..\..\Platform\AxPlatformUtils.h"
#include "..\..\Streams\AxFileStream.h"
#include "..\..\Streams\AxMemoryStream.h"


AxWebServer::AxWebServer(void)
{
	AxTcpIpServer();

	this->rootPath = AxPlatformUtils::GetApplicationPath();
	this->synchronized = true;

	this->fileSystem = new AxPhysicalFileSystem();
	this->defaultFileSystem = true;

	this->sessionDurationSeconds = 20 * 60;
}


AxWebServer::~AxWebServer(void)
{
	for (int componentIndex = 0; componentIndex < this->components.count; componentIndex++)
		delete this->components[componentIndex];

	if (this->defaultFileSystem)
		delete this->fileSystem;

	for (void *entry = this->sessions.GetNodeNext(0); entry != 0; entry = this->sessions.GetNodeNext(entry))
	{
		this->OnEndSession(this->sessions.GetNodeValue(entry).value->data);
		delete this->sessions.GetNodeValue(entry).value;
	}
}

void AxWebServer::OnConnectionReceive(AxTcpIpConnection *connection, void *data, int dataSize)
{
	AxWebRequest *request = new AxWebRequest(this, connection, AxString((char*)data, dataSize));
	AxWebResponse *response = new AxWebResponse(request);

	if (request->supported)
	{
		this->GetSession(response);
		
		request->path = this->RerouteRequest(request->originalPath);

		if (!this->InterceptRequest(response))
		{
			if (this->fileSystem->FileExists(this->rootPath + "\\" + request->path))
			{
				if (this->GetContentType(request->path, response->dataType))
				{
					bool resourceAcquired;
					if (response->dataType == "text/html")
					{
						resourceAcquired = this->GenerateHtmlContents(request->path, request, response->data);
					}
					else
					{
						resourceAcquired = this->GetFileContents(request, response->data);
					}

					if (resourceAcquired)
					{
						this->OnResourceResponse(response);
					}
					else
					{
						this->OnInternalError(response);
					}
				}
				else
				{
					this->OnContentTypeNotSupported(response);
				}
			}
			else
			{
				this->OnResourceNotFound(response);
			}
		}
	}
	else
	{
		this->OnHttpMethodNotImplemented(response);
	}

	this->SendResponse(response);

	delete response;
	delete request;
		

	connection->Close();
}

void AxWebServer::Redirect(AxWebResponse *response, AxString url)
{
	this->GenerateHtmlResponse(response, "303 See Other", AxString("Location: ") + url + "\r\n", 
		"Not Imlemented.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>501 - HTTP Method Not Implemented</div></body></html>");
}

void AxWebServer::RequestAuthorization(AxWebResponse *response, AxString message)
{
	this->GenerateHtmlResponse(response, "401 Unauthorized", AxString("WWW-Authenticate: basic realm=\"") + message + "\"\r\n", 
		"Unauthorized.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>401 - Unauthorized</div></body></html>");
}

void AxWebServer::Reject(AxWebResponse *response)
{
	this->GenerateHtmlResponse(response, "403 Forbidden", "", 
		"Rejected.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>403 - Rejected</div></body></html>");
}


void AxWebServer::SetFileSystem(AxFileSystem *fileSystem)
{
	if (this->defaultFileSystem)
	{
		delete this->fileSystem;
		this->defaultFileSystem = false;
	}

	this->fileSystem = fileSystem;
}

void AxWebServer::SetRootPath(AxString rootPath)
{
	if (AxFileSystem::IsAbsolutePath(rootPath))
		this->rootPath = rootPath;
	else
		this->rootPath = AxFileSystem::MergePaths(AxPlatformUtils::GetApplicationPath(), rootPath);
}

void AxWebServer::RegisterComponent(AxWebPageComponent *component, AxString componentName)
{
	this->components.Add(component);
	component->webServer = this;
	component->name = componentName;
}

void AxWebServer::GetSession(AxWebResponse *response)
{
	// TODO: Tidy up this ugly method. Refine cookie management (as well as the session cookie)

	long long time = AxPlatformUtils::GetPerformanceCounter();
	long long freq = AxPlatformUtils::GetPerformanceCounterFrequency();

	// Find if there is such session already and whether it has not timed out
	WebServerSession *session;
	if (response->request->receivedSessionId && this->sessions.GetValue(response->request->sessionId, session))
	{
		if (((float)(time - session->time) / (float)freq) < this->sessionDurationSeconds)
		{
			session->time = time;
			response->request->session = session->data;
			response->newSession = false;
			return;
		}
	}

	// Remove any timed-out sessions. TODO: ?This can also be done in a background thread
	void *entry = this->sessions.GetNodeNext(0);
	while (entry != 0)
	{
		void *current = entry;
		entry = this->sessions.GetNodeNext(entry);
		WebServerSession *session = this->sessions.GetNodeValue(current).value;
		if (((float)(time - session->time) / (float)freq) >= this->sessionDurationSeconds)
		{
			this->OnEndSession(session->data);
			delete session;
			this->sessions.RemoveNode(current);
		}
	}

	// Add a new session
	do
	{
		response->request->sessionId = AxMath::Random(AxMath::IntMax) ^ (unsigned int)(AxPlatformUtils::GetPerformanceCounter());
	}
	while (this->sessions.KeyExists(response->request->sessionId));

	response->request->session = this->OnCreateSession(response->request);

	WebServerSession *newSession = new WebServerSession();
	newSession->data = response->request->session;
	newSession->time = time;
	
	this->sessions.Add(response->request->sessionId, newSession);

	response->newSession = true;
}

AxString AxWebServer::RerouteRequest(AxString originalUrl)
{
	originalUrl = originalUrl.Trim('\\', false, true);

	if ((originalUrl.length == 0) || this->fileSystem->DirectoryExists(this->rootPath + "\\" + originalUrl))
	{
		if (this->fileSystem->FileExists(this->rootPath + "\\index.html"))
			return AxString("index.html");

		if (this->fileSystem->FileExists(this->rootPath + "\\index.htm"))
			return AxString("index.htm");
	}

	return AxString(originalUrl);
}

bool AxWebServer::EndSession(unsigned int sessionId)
{
	WebServerSession *session;
	if (!this->sessions.GetValue(sessionId, session))
		return false;

	this->OnEndSession(session->data);

	delete session;

	this->sessions.Remove(sessionId);

	return true;
}

bool AxWebServer::InterceptRequest(AxWebResponse *response)
{
	return false;
}

bool AxWebServer::GetContentType(AxString requestedResource, AxString &responseType)
{
	int extensionTextStart = requestedResource.LastIndexOf(".") + 1;
	AxString requestedResourceExtension = requestedResource.SubString(extensionTextStart, requestedResource.length - extensionTextStart).ToLower();

	const int numSupportedExtensions = 12;
	const char *extensions[numSupportedExtensions][2] =
	{ 
		{ "htm", "text/html" },
		{ "html", "text/html" },
		{ "css", "text/css" },
		{ "xml", "application/xml" },
		{ "txt", "text/plain" },
		{ "jpg", "image/jpeg" },
		{ "jpeg", "image/jpeg" },
		{ "png", "image/png" },
		{ "gif", "image/gif" },
		{ "ico", "image/x-icon" },
		{ "zip", "application/zip"},
		{ "axs", "application/axis-scene1"}
	};

	for (int i = 0; i < numSupportedExtensions; i++)
	{
		if (requestedResourceExtension.Equals(extensions[i][0]))
		{
			responseType = extensions[i][1];
			return true;
		}
	}

	//responseType = "application/octet-stream";
	//return true;

	return false;
}


void *AxWebServer::OnCreateSession(AxWebRequest *request)
{
	return new AxString();
}

void AxWebServer::OnEndSession(void *session)
{
	AxString *s = (AxString*)session;
	delete s;
}

void AxWebServer::OnResourceResponse(AxWebResponse *response)
{
	AxString responseType;
	if (this->GetContentType(response->request->path, responseType))
		if (responseType.Equals("text/html"))
		{
			AxString *s = (AxString*)(response->request->session);
			if (AxMath::Random(5) == 0)
				response->request->server->EndSession(response->request->sessionId);
			else
				s->Insert(response->request->path);
		}


	
	response->headerCode = "200 OK";
}

void AxWebServer::OnInternalError(AxWebResponse *response)
{
	this->GenerateHtmlResponse(response, "500 Internal Server Error", "", 
		"Internal Error.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>500 - Internal Server Error</div></body></html>");
}

void AxWebServer::OnResourceNotFound(AxWebResponse *response)
{
	this->GenerateHtmlResponse(response, "404 Not Found", "", 
		"Not Found.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>404 - Not Found</div></body></html>");
}

void AxWebServer::OnContentTypeNotSupported(AxWebResponse *response)
{
	this->GenerateHtmlResponse(response, "415 Unsupported Media Type", "", 
		"Not Supported.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>415 - Unsupported Media Type</div></body></html>");
}

void AxWebServer::OnHttpMethodNotImplemented(AxWebResponse *response)
{
	this->GenerateHtmlResponse(response, "501 Not Implemented", "", 
		"Not Imlemented.html",
		"<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head><body><h2>Axis Web Server</h2><div>501 - HTTP Method Not Implemented</div></body></html>");
}


void AxWebServer::SendResponse(AxWebResponse *response)
{
	AxString header = AxString("HTTP/1.1 ") + response->headerCode + "\r\nServer: Axis Web Server\r\nContent-Type: " + response->dataType + "\r\nContent-Length: " + AxString(response->data->length) + "\r\n" + response->headerAppendix;
	if (response->newSession)
		header += AxString("Set-Cookie: ") + "axsession=" + AxString(response->request->sessionId) + "; HttpOnly\r\n";
	header += "Connection: close\r\n\r\n";
	response->request->connection->Send(header.GetCharContents(), header.length);
	if (response->data->length > 0)
	{
		char *data = new char[(int)response->data->length];
		response->data->Seek(0);
		response->data->ReadData(data, response->data->length);
		response->request->connection->Send(data, (int)response->data->length);
		delete[] data;
	}
}

void AxWebServer::GenerateHtmlResponse(AxWebResponse *response, AxString headerCode, AxString headerAppendix, AxString fileName, AxString backupHtmlCode)
{
	response->headerCode = headerCode;
	response->dataType = "text/html";
	response->headerAppendix = headerAppendix;
	if (!this->GenerateHtmlContents(fileName, response->request, response->data))
	{
		response->data->WriteData(backupHtmlCode.GetCharContents(), backupHtmlCode.length);
	}
}

bool AxWebServer::GetFileContents(AxWebRequest *request, AxStream *responseContents)
{
	AxString fileName = this->rootPath + "\\" + request->path;

	if (!this->fileSystem->FileExists(fileName))
		return false;

	AxFileStream fileData(fileName.contents, FileStreamAccessMode_Read);
	responseContents->WriteStreamData(fileData);

	return true;
}

bool AxWebServer::GenerateHtmlContents(AxString requestFile, AxWebRequest *request, AxStream *responseContents)
{
	AxString fileName = this->rootPath + "\\" + requestFile;

	if (!this->fileSystem->FileExists(fileName))
		return false;

	AxFileStream codeStream(fileName.contents, FileStreamAccessMode_Read);

	char *sourceData = new char[(unsigned int)codeStream.length + 1];
	codeStream.ReadData(sourceData, codeStream.length);
	sourceData[codeStream.length] = 0;
	AxString source(sourceData);
	delete[] sourceData;

	AxString output;

	// TODO: Real parsing of html

	int parsedIndex = 0;
	while (parsedIndex < source.length)
	{
		int axwTagStart = source.IndexOf("<axw", parsedIndex);
		int axwTagEnd = -1;
		if (axwTagStart != -1)
		{
			int classNameStart = source.IndexOf("component=\"", axwTagStart + 5);
			if (classNameStart != -1)
			{
				classNameStart += 11;
				int classNameEnd = source.IndexOf("\"", classNameStart);
				if (classNameEnd != -1)
				{
					axwTagEnd = source.IndexOf("/>", axwTagStart);
					if (axwTagEnd != -1)
						axwTagEnd += 2;
					else
					{
						axwTagEnd = source.IndexOf("</axw>", axwTagStart);
						if (axwTagEnd != -1)
							axwTagEnd += 6;
					}

					if (axwTagEnd != -1)
					{
						output += source.SubString(parsedIndex, axwTagStart - parsedIndex);
						parsedIndex = axwTagEnd;

						AxString className = source.SubString(classNameStart, classNameEnd - classNameStart);
						
						int componentIndex = this->components.count - 1;
						for (; componentIndex >= 0; componentIndex--)
							if (this->components[componentIndex]->name.Equals(className))
								break;

						if (componentIndex != -1)
						{
							AxString s = this->components[componentIndex]->Output(request);
							output += s;
						}
					}
				}
			}
		}

		if (axwTagEnd == -1)
		{
			output += source.SubString(parsedIndex, source.length - parsedIndex);
			parsedIndex = source.length;
		}
	}
	
	responseContents->WriteData((void*)output.GetCharContents(), output.length);

	return true;
}
