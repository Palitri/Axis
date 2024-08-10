//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

// This example shows how to create a basic setup with Axis.
// It creates a Windows form and an instance of the engine, which opens a scene file and renders it.
// The sample sets "Direct3D 11" for graphics device, because the default is "Axis Soras", which is Axis' software device
// An AxWalkInput model is added, so that the camera can be controlled by mouse, keyboard and gamepad

#include <tchar.h>
#include <assert.h>
#include <stdio.h>
#include <iostream>


#include "AxisEngine.h"

struct Matrix3x3
{
	float _11, _12, _13,
		_21, _22, _23,
		_31, _32, _33;
};

struct Vector3
{
	float x, y, z;
};


class MyRenderWindow :
	public AxWindowsForm
{
public:
	Axis *engine;

	AxTransform *frogTransform;

	AxSerialStream *com;

	MyRenderWindow(void)
	{
		this->engine = new Axis(&this->handle);

		// Set the desired graphics device. "Axis Soras" is the default device and "Direct3D 11" and "OpenGL" are available. Check Axis::graphicsDeviceDispatchers for a list
		//this->engine->SetGraphicsDevice("Direct3D 11");
		this->engine->SetGraphicsDevice("OpenGL");

		// Open 3D scene
		this->engine->ImportScene("../../Resources/Frog/FrogSimple.axs");

		// Add an input control model
		this->engine->inputModels.Add(new AxWalkInputModel(this->engine, (AxTransform*)this->engine->FindResourceX("Camera", AxResourceType_Transform), 20.0f, 5.0f, 0.2f, 5.0f,
			"Mouse Left,Key W,Pad Analog Y left", "Mouse Right,Key S", "Key A", "Key D,Pad Analog X left", "Mouse Scroll,Key Space", "Key Shift", "", 
			"Mouse Y,Pad Analog Y right", "", "", "Mouse X,Pad Analog X right", "Mouse Left,Mouse Right,Pad Analog Y right,Pad Analog X right"));


		this->frogTransform = (AxTransform*)this->engine->FindResourceX("SMFrog", AxResourceType_Transform);

		this->com = new AxSerialStream("COM14", 9600);

	};

	~MyRenderWindow()
	{
		delete this->engine;

		delete this->com;
	};

	void MainLoop()
	{
		this->engine->RenderScene();

		while (com->position + 4 <= com->length)
		{
			int v = com->ReadUInt8();
			std::cout << com->length << "\t\t" << (int)v << "\r\n";
			this->frogTransform->SetRotationExtrinsic(AxVector3(AxMath::Pi * 2 * v / 255.0f, 0.0f, 0.0f));
		}

	};

	void Resized(int width, int height, bool done)
	{
		if (done)
			this->engine->SetViewport(width, height);
	};
};


int RunSHA1Test()
{
	char responseSHA1[20];
	//AxMath::SHA1(sha, "abc", 3);
	AxMath::SHA1(responseSHA1, "ySeD8oYiNm0IYcRPsxzhnA==258EAFA5-E914-47DA-95CA-C5AB0DC85B11", 60);
	//char *sha = SHA1::Calculate("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq", 56);

	char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	AxString x = AxString::ToBase64(responseSHA1, 20);

	for (int i = 0; i < 20; i++)
	{
		char const byte = responseSHA1[i];

		std::cout << hex_chars[(byte & 0xF0) >> 4];
		std::cout << hex_chars[(byte & 0x0F) >> 0];
	}

	std::getchar();

	return 0;
}

class ComPort : 
	public AxSerialTransmission
{
public:
	ComPort(AxString port, int baud)
		: AxSerialTransmission(port, baud)
	{
	};

	virtual ~ComPort() { };

	virtual void OnReceiveChunk(void *data, int dataSize)
	{
		std::cout << "Receive: ";
		for (int i = 0; i < dataSize; i++)
			std::cout << (int)((unsigned char*)data)[i] << " ";
		std::cout << "\r\n";
	}
};

void RunComTestWithTransmission()
{
	AxSerialTransmission *com = new ComPort("COM14", 9600);

	AxMemoryStream *stream = new AxMemoryStream();

	int opCode;
	do
	{
		std::cin >> opCode;

		stream->position = 0;
		stream->length = 0;
		stream->WriteUInt8(opCode);

		switch (opCode)
		{
			case 17:
				{
					int motor, tone;
					float time;
					std::cout << "Motor: ";
					std::cin >> motor;
					std::cout << "Tone: ";
					std::cin >> tone;
					std::cout << "Time (s): ";
					std::cin >> time;
					stream->WriteUInt8(motor);
					stream->WriteUInt8(tone);
					stream->WriteFloat32(time);

					break;
				}

			case 101:
				{
					int motor, interval, steps;
					std::cout << "Motor: ";
					std::cin >> motor;
					std::cout << "Interval (us): ";
					std::cin >> interval;
					std::cout << "Steps: ";
					std::cin >> steps;
					stream->WriteUInt8(motor);
					stream->WriteUInt16(interval);
					stream->WriteUInt16(steps);

					break;
				}

			case 102:
				{
					int radius, time;
					std::cout << "Radius: ";
					std::cin >> radius;
					std::cout << "Time (sec): ";
					std::cin >> time;
					stream->WriteUInt8(radius);
					stream->WriteUInt8(time);

					break;
				}

			case 103:
				{
					int time;
					std::cout << "Time (sec): ";
					std::cin >> time;
					stream->WriteUInt8(time);

					break;
				}

		}

		com->SendChunk(stream->data, stream->length);
	}
	while (opCode != -1);

	delete stream;
	delete com;
}

void RunComTestWithStream()
{
	AxSerialStream *com = new AxSerialStream("COM14", 9600, 50);

	while (true)
	{
		while (com->position < com->length)
		{
			unsigned char v = com->ReadUInt8();
			std::cout << (int)v << "\r\n";//AxString(v, 16).GetCharContents();
		}

		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;
	}

	delete com;
}

void RunComTest()
{
	void *com = AxPlatformUtils::OpenComPort("COM14");
	unsigned char buffer;

	while (true)
	{
		AxPlatformUtils::ReadComPort(com, &buffer, 1);
		std::cout << (unsigned int)buffer << "\r\n";

		if (GetKeyState(VK_ESCAPE) & 0x8000)
			break;
	}

	AxPlatformUtils::CloseComPort(&com);
}

void RunRotaryBufferTest()
{
	AxRotaryBufferStream s(10, AxRotaryBufferStreamWriteMode_SeparatePosition);
	for (int i = 0; i < 15; i++)
	{
		s.WriteUInt8(i);

		while (s.position < s.length)
		{

			unsigned char v = s.ReadUInt8();
			std::cout << (int)v << "\r\n";//AxString(v, 16).GetCharContents();
		}

		std::cin.get();
	}
}

void Plot(float x, float y, float z, float time)
{
  const float turnsPerUnit = 1.0f;
  const int stepsPerTurn = 200;
  const int stepsPerUnit = turnsPerUnit * stepsPerTurn;
  
  unsigned long intervals[3];
  intervals[0] = time * 1000000.0f / (x * (float)stepsPerUnit);
  intervals[1] = time * 1000000.0f / (y * (float)stepsPerUnit);
  intervals[2] = time * 1000000.0f / (z * (float)stepsPerUnit);
}

int main(int argc, char *args[])
{

	//Plot(1.0f, 0.5f, 0.1f, 1);
	//RunComTestWithTransmission();

	//RunComTestWithStream();


	//RunRotaryBufferTest();

	//return 0;



	//return RunSHA1Test();

	//AxWebSocketsExchangeServer myWebServer;
	//myWebServer.RegisterComponent(new AxWebPageFileListing(), "FileListing");
	//myWebServer.RegisterComponent(new AxWebPageHostName(), "HostName");
	//myWebServer.SetRootPath("../Web");
	//myWebServer.Open(80);


	//AxWebServer myWebServer;
	//myWebServer.RegisterComponent(new AxWebPageFileListing(), "FileListing");
	//myWebServer.RegisterComponent(new AxWebPageHostName(), "HostName");
	//myWebServer.SetRootPath("../Web");
	//myWebServer.Open(80);

	//ShellExecute(NULL, "open", "http://localhost:81", NULL, NULL, SW_SHOWNORMAL);

	//std::cout << "Axis web server is running at http://localhost:81\r\nPress Enter to exit\r\n";
	//std::cin.get();

	//myWebServer.Close();
	//return 0;

	MyRenderWindow renderWindow;
	return AxWindowsForm::PerformFormLoop();
}