//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetFloat.h"

QAxParameterWidgetFloat::QAxParameterWidgetFloat(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QDoubleSpinBox()
{
    this->parameter = parameter;

    this->setRange(-2000000, 2000000);
    this->setValue((double)(*((float*)this->parameter->value)));
    this->setSingleStep(AxMath::Max((float)(AxMath::Abs((float)this->value()) / 10.0f), 0.01f));
    this->setDecimals(5);

    this->setAcceptDrops(true);

    QObject::connect(this, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
}

void QAxParameterWidgetFloat::OnValueChanged(double)
{
    *((float*)this->parameter->value) = this->value();
    this->setSingleStep(AxMath::Max((float)(AxMath::Abs((float)this->value()) / 10.0f), 0.01f));
}

void QAxParameterWidgetFloat::dragEnterEvent(QDragEnterEvent *event)
{
    QAxParameterWidget::HandleDragEnterEvent(event);
}

void QAxParameterWidgetFloat::dropEvent(QDropEvent *event)
{
    QAxParameterWidget::HandleDragDropEvent(event);
}
