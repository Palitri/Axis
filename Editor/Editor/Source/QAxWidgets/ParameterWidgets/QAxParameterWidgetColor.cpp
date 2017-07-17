//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetColor.h"

#include <QHBoxLayout>

QAxParameterWidgetColor::QAxParameterWidgetColor(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QWidget()
{
    this->parameter = parameter;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    this->setAcceptDrops(true);

    this->rgbWidget = new QAxParameterWidgetColorRgb(this->parameter, editor);
    layout->addWidget(this->rgbWidget);

    this->alphaWidget = new QAxParameterWidgetColorAlpha(this->parameter, editor);
    layout->addWidget(this->alphaWidget);
}

void QAxParameterWidgetColor::dragEnterEvent(QDragEnterEvent *event)
{
    QAxParameterWidget::HandleDragEnterEvent(event);
}

void QAxParameterWidgetColor::dropEvent(QDropEvent *event)
{
    QAxParameterWidget::HandleDragDropEvent(event);
}
