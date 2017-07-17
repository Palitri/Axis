//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxEditorWidget.h"

QAxEditorWidget::QAxEditorWidget()
{
    this->editor = 0;
}

void QAxEditorWidget::SetEditorInterface(IAxEditor *editor)
{
    this->editor = editor;
}
