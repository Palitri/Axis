//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetInt.h"

QAxParameterWidgetInt::QAxParameterWidgetInt(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QSpinBox()
{
    this->parameter = parameter;

    this->setRange(-2000000, 2000000);
    this->setValue(*((int*)this->parameter->value));
    this->setSingleStep(AxMath::Max(AxMath::Abs(this->value()) / 10, 1));

    this->setAcceptDrops(true);

    QObject::connect(this, SIGNAL(valueChanged(int)), this, SLOT(OnValueChanged(int)));
}

void QAxParameterWidgetInt::OnValueChanged(int)
{
    *((int*)this->parameter->value) = this->value();
    this->setSingleStep(AxMath::Max(AxMath::Abs(this->value()) / 10, 1));
}

void QAxParameterWidgetInt::dragEnterEvent(QDragEnterEvent *event)
{
    QAxParameterWidget::HandleDragEnterEvent(event);
}

void QAxParameterWidgetInt::dropEvent(QDropEvent *event)
{
    QAxParameterWidget::HandleDragDropEvent(event);
}
