//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetColorAlpha.h"

#include "../../EditorTools/EditorUtils.h"

QAxParameterWidgetColorAlpha::QAxParameterWidgetColorAlpha(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QDoubleSpinBox()
{
    this->parameter = parameter;

    this->setRange(0.0, 1.0);
    this->setSingleStep(0.05);
    this->setValue((double)(((AxVector4*)this->parameter->value)->w));

    QObject::connect(this, SIGNAL(valueChanged(double)), this, SLOT(OnValueChanged(double)));
}

void QAxParameterWidgetColorAlpha::OnValueChanged(double)
{
    ((AxVector4*)this->parameter->value)->w = this->value();
}
