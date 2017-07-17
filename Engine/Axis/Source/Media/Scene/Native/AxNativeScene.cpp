//---------------------------------------------------------------------------------------------
//	Axis 3D Engine Copyright (C) 2017 Palitri
//
//	This software is released under the MIT License http://palitri.com/axis/license/
//
//	For help and documentation, visit http://palitri.com
//---------------------------------------------------------------------------------------------

#include "AxNativeScene.h"

#include "AxNativeSceneReader.h"
#include "AxNativeSceneWriter.h"

const char *AxNativeScene::formatName = "axs";

AxNativeScene::AxNativeScene(void)
{
}


AxNativeScene::~AxNativeScene(void)
{
}

bool AxNativeScene::Load(AxStream &source)
{
	if (this->context == 0)
		return false;

	AxNativeSceneReader reader(this->context);

	return reader.Read(source);
}

bool AxNativeScene::Save(AxStream &destination, const char *format)
{
	if (this->context == 0)
		return false;

	if (format != 0)
	{
		AxString encodingCode = AxString(format).Trim('.');
		if ((encodingCode.length != 0) && (!encodingCode.Equals(AxNativeScene::formatName)))
			return false;
	}


	AxNativeSceneWriter writer(this->context);
	writer.Write(destination);

	return true;
}
