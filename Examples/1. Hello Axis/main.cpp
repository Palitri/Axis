//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

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