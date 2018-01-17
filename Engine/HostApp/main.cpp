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

#include "AxisEngine.h"

class MyRenderWindow :
	public AxWindowsForm
{
public:
	Axis *engine;

	MyRenderWindow(void)
	{
		this->engine = new Axis(&this->handle);

		// Set the desired graphics device. "Axis Soras" is the default device and "Direct3D 11" and "OpenGL" are available. Check Axis::graphicsDeviceDispatchers for a list
		this->engine->SetGraphicsDevice("Direct3D 11");

		// Open 3D scene
		this->engine->ImportScene("../../Resources/Terrain/Terrain.axs");

		// Add an input control model
		this->engine->inputModels.Add(new AxWalkInputModel(this->engine, (AxTransform*)this->engine->FindResourceX("Camera", AxResourceType_Transform), 20.0f, 5.0f, 0.2f, 5.0f,
			"Mouse Left,Key W,Pad Analog Y left", "Mouse Right,Key S", "Key A", "Key D,Pad Analog X left", "Mouse Scroll,Key Space", "Key Shift", "", 
			"Mouse Y,Pad Analog Y right", "", "", "Mouse X,Pad Analog X right", "Mouse Left,Mouse Right,Pad Analog Y right,Pad Analog X right"));
	};

	~MyRenderWindow()
	{
		delete this->engine;
	};

	void MainLoop()
	{
		this->engine->RenderScene();
	};

	void Resized(int width, int height, bool done)
	{
		if (done)
			this->engine->SetViewport(width, height);
	};
};


int main(int argc, char *args[])
{
	MyRenderWindow renderWindow;
	return AxWindowsForm::PerformFormLoop();
}