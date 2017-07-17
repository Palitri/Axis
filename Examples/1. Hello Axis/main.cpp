//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine example Copyright (C) 2014  Ivan Donev
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the engine, visit <http://morroworks.com>
//	You're very welcome to post your questions and get support at the community page!
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// This example shows how to create a basic setup with Axis.
// It creates a windows form, an instance of the engine, opens a scene and renders it.
// It also sets an OpenGL graphics device in place of the default renderer, which is Axis' software rasterizer - Soras.


#include "AxisEngine.h"


class MyRenderWindow :
	public AxWindowsForm
{
public:
	Axis *engine;

	MyRenderWindow(void)
	{
		this->engine = new Axis(&this->handle);

		this->engine->SetGraphicsDevice("OpenGL");

		this->engine->ImportScene("../../Resources/Robot/Robot.axs");
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