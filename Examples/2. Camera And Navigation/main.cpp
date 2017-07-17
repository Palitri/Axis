//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine example Copyright (C) 2014  Ivan Donev
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the engine, visit <http://morroworks.com>
//	You're very welcome to post your questions and get support at the community page!
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AxisEngine.h"

class MyRenderWindow :
	public AxWindowsForm
{
public:
	Axis *engine;
	
	AxTransform *cameraTransform;

	MyRenderWindow(void)
	{
		this->engine = new Axis(&this->handle);

		this->engine->SetGraphicsDevice("OpenGL");
		//this->engine->SetGraphicsDevice("Direct3D 11");
		
		this->engine->ImportScene("../../Resources/Terrain/Terrain.axs");

		this->cameraTransform = (AxTransform*)this->engine->FindResourceX("Camera", AxResourceType_Transform);
	};

	~MyRenderWindow()
	{
		delete this->engine;
	};

	void MainLoop()
	{
		if (this->cameraTransform != 0)
		{
			// Free flight navigation - as it would be in a non reference orientation environments like space
			this->cameraTransform->RotateIntrinsic(AxVector3(-this->mouseDeltaY / 100.0f, 0.0f, 0.0f));
			this->cameraTransform->RotateExtrinsic(AxVector3(0.0f, -this->mouseDeltaX / 100.0f, 0.0f));
			
			// Move camera with the WASD keys or by pressing mouse left and right buttons
			this->cameraTransform->TranslateIntrinsic(AxVector3(
				this->keys['D'] ? -1.0f : this->keys['A'] ? 1.0f : 0.0f,
				this->keys[VK_SPACE] ? 1.0f : this->keys[VK_SHIFT] ? -1.0f : 0.0f,
				(this->keys['W'] || ((this->mouseButtons & AxWindowsForm::MouseButton_Left) != 0)) ? 1.0f : (this->keys['S'] || ((this->mouseButtons & AxWindowsForm::MouseButton_Right) != 0)) ? -1.0f : 0.0f).Scale(this->engine->timer->time * 20.0f));
		}

		this->engine->RenderScene();
	};
};

int main(int argc, char *args[])
{
	MyRenderWindow renderWindow;
	return AxWindowsForm::PerformFormLoop();
}