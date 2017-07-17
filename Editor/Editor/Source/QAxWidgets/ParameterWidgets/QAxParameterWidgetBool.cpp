//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetBool.h"

QAxParameterWidgetBool::QAxParameterWidgetBool(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QCheckBox()
{
    this->parameter = parameter;

    QObject::connect(this, SIGNAL(stateChanged(int)), this, SLOT(OnStateChanged(int)));

    this->setChecked(*((bool*)this->parameter->value));

    this->setAcceptDrops(true);
}

void QAxParameterWidgetBool::OnStateChanged(int)
{
    *((bool*)this->parameter->value) = this->isChecked();
}

void QAxParameterWidgetBool::dragEnterEvent(QDragEnterEvent *event)
{
    QAxParameterWidget::HandleDragEnterEvent(event);
}

void QAxParameterWidgetBool::dropEvent(QDropEvent *event)
{
    QAxParameterWidget::HandleDragDropEvent(event);
}
