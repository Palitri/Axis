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

#include <iostream>

bool CheckSet(AxEntitySet *setToCheck, AxEntitySet * targetSet)
{
	if (setToCheck == targetSet)
		return true;

	for (int i = 0; i < targetSet->references.count; i++)
	{
		AxResource *r = targetSet->references[i];
		if (r->resourceType == AxResourceType_EntitySet)
			if (CheckSet(setToCheck, (AxEntitySet*)r))
				return true;
	}

	return false;
}

class MyRenderWindow :
	public AxWindowsForm
{
public:
	Axis *engine;

	MyRenderWindow(void)
	{
		/*
		//AxFileStream *fin = new AxFileStream("C://Users//Me//Desktop//Offline 3D samples//heart//HeartModel.axs", FileStreamAccessMode_Read);
		//AxFileStream *fout = new AxFileStream("C://Users//Me//Desktop//Offline 3D samples//heart//HeartModel.json", FileStreamAccessMode_Write);

		AxFileStream *fin = new AxFileStream("C://Users//Me//Desktop//heart3//HeartSource.axs", FileStreamAccessMode_Read);
		AxFileStream *fout = new AxFileStream("C://Users//Me//Desktop//heart3//HeartModel.js", FileStreamAccessMode_Write);

		//AxFileStream *fin = new AxFileStream("C:\\Users\\Me\\Desktop\\body\\Body.axs", FileStreamAccessMode_Read);
		//AxFileStream *fout = new AxFileStream("C:\\Users\\Me\\Desktop\\body\\BodyModel.js", FileStreamAccessMode_Write);

		//AxFileStream *fin = new AxFileStream("C:\\Users\\Me\\Desktop\\lymphatic\\Lymphatic.axs", FileStreamAccessMode_Read);
		//AxFileStream *fout = new AxFileStream("C:\\Users\\Me\\Desktop\\lymphatic\\LymphaticModel.js", FileStreamAccessMode_Write);
		
		//AxFileStream *fin = new AxFileStream("C://Users//Me//Desktop//skeleton//Skeleton.axs", FileStreamAccessMode_Read);
		//AxFileStream *fout = new AxFileStream("C://Users//Me//Desktop//skeleton//SkeletonModel.js", FileStreamAccessMode_Write);

		//AxFileStream *fin = new AxFileStream("C://Users//Me//Desktop//skeletonheart//SkeletonHeart.axs", FileStreamAccessMode_Read);
		//AxFileStream *fout = new AxFileStream("C://Users//Me//Desktop//skeletonheart//SkeletonHeartModel.js", FileStreamAccessMode_Write);

		//AxFileStream *fin = new AxFileStream("C://Users//Me//Desktop//innerear//InnerEar.axs", FileStreamAccessMode_Read);
		//AxFileStream *fout = new AxFileStream("C://Users//Me//Desktop//innerear//InnerEarModel.js", FileStreamAccessMode_Write);

		char *data = new char[fin->length];
		fin->ReadData(data, fin->length);
		AxString b64 = AxString::ToBase64(data, fin->length);
		fout->WriteText("var fileData = \"");
		fout->WriteText(b64);
		fout->WriteText("\";");
		delete fin;
		delete fout;
		delete data;


		//AxString colon(",");
		//AxString jsContent = "var fileData = new Uint8Array([";
		//while (fin->position < fin->length - 1)
		//{
		//	jsContent += AxString(fin->ReadUInt8(), 10);
		//	
		//	if (fin->position < fin->length - 1)
		//		jsContent += ",";
		//}
		//jsContent += "]);";
		//fout->WriteText(jsContent);
		//delete fin;
		//delete fout;
		//*/

		
		
		this->engine = new Axis(&this->handle);

		// Set the desired graphics device. "Axis Soras" is the default device and "Direct3D 11" and "OpenGL" are available. Check Axis::graphicsDeviceDispatchers for a list
		this->engine->SetGraphicsDevice("Direct3D 11");

		//this->engine->ImportScene("../../Resources/Space/Space.axs");
		//this->engine->ImportScene("../../Resources/TileGame/TileOrientation.axs");
		//this->engine->ImportScene("C:/Users/Me/Desktop/cardiac-anatomy-external-view/source/maar dan beter.OBJ");
		//this->engine->ImportScene("C://Users//Me//Desktop//body//BodySource2.axs");
		//this->engine->ImportScene("C:/Users/Me/Desktop/morph/BoxMorph.fbx");
		//this->engine->ImportScene("C:/Users/Me/Desktop/heart3/Heart.axs");
		this->engine->ImportScene("C:/Users/Me/Desktop/heart3/Heart.fbx");
		//this->engine->ImportScene("C:/Users/Me/Desktop/heart3/heart3dsmax.FBX");
		//this->engine->ImportScene("C:/Users/Me/Desktop/heart3/test.fbx");
		//this->engine->ImportScene("C:/Users/Me/Desktop/morph/BoxMorph.fbx");
		//this->engine->ImportScene("C:/Users/Me/Desktop/heart3/Heart test.fbx");
		//this->engine->ImportScene("C://Users//Me//Desktop//innerear//InnerEar.axs");

		/*
		for (int i = 0; i < this->engine->resources.count; i++)
		{
			AxResource *r = this->engine->resources[i];
			if (r->resourceType == AxResourceType_EntitySet)
				if (!CheckSet((AxEntitySet*)r, this->engine->root))
				{
					this->engine->RemoveResource(r);
					i--;
				}
		}
		this->engine->ExportScene("C:/Users/Me/Desktop/heart3/HeartSource.axs");
		//*/

		//this->engine->inputModels.Add(new AxWalkInputModel(this->engine, (AxTransform*)this->engine->FindResourceX("Camera", AxResourceType_Transform), 20.0f, 5.0f, 0.2f, 5.0f,
		//	"Mouse Left,Key W,Pad Analog Y left", "Mouse Right,Key S", "Key A", "Key D,Pad Analog X left", "Mouse Scroll,Key Space", "Key Shift", "", 
		//	"Mouse Y,Pad Analog Y right", "", "", "Mouse X,Pad Analog X right", "Mouse Left,Mouse Right,Pad Analog Y right,Pad Analog X right"));
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