//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetColorRgb.h"

#include <QDoubleSpinBox>

#include "../../EditorTools/EditorUtils.h"

QAxParameterWidgetColorRgb::QAxParameterWidgetColorRgb(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QPushButton()
{
    this->parameter = parameter;

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));

    QObject::connect(&this->colorDialog, SIGNAL(currentColorChanged(const QColor &)), this, SLOT(OnColorDialogColorChanged(const QColor &)));
    QObject::connect(&this->colorDialog, SIGNAL(accepted()), this, SLOT(OnColorDialogColorAccepted()));
    QObject::connect(&this->colorDialog, SIGNAL(rejected()), this, SLOT(OnColorDialogColorRejected()));

    this->setStyleSheet(QString("background-color:#%1;").arg(EditorUtils::Vector4ToHtmlColor(*(AxVector4*)this->parameter->value)));
}

void QAxParameterWidgetColorRgb::OnClicked()
{
    this->revertColor = *((AxVector4*)this->parameter->value);

    this->colorDialog.setCurrentColor(EditorUtils::Vector4ToQColor(this->revertColor));
    this->colorDialog.show();
}

void QAxParameterWidgetColorRgb::OnColorDialogColorChanged(const QColor & color)
{
    EditorUtils::QColorToVector4(color, *((AxVector4*)this->parameter->value), false);
}

void QAxParameterWidgetColorRgb::OnColorDialogColorAccepted()
{
    this->setStyleSheet(QString("background-color:#%1;").arg(EditorUtils::Vector4ToHtmlColor(*(AxVector4*)this->parameter->value)));
}

void QAxParameterWidgetColorRgb::OnColorDialogColorRejected()
{
    *((AxVector4*)this->parameter->value) = this->revertColor;
}
