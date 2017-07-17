//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxResourceItemWidget.h"

#include "../EditorTools/EditorUtils.h"

QAxResourceItemWidget::QAxResourceItemWidget(QWidget *parent) :
    QWidget(parent),
    QAxEditorWidget()
{
    this->layout = new QVBoxLayout(this);
    this->layout->setContentsMargins(0, 0, 0, 0);
    this->layout->setSpacing(2);

    this->button = new QAxResourceItemButtonWidget();
    this->button->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    this->layout->addWidget(this->button);

    this->nameLabel = new QLabel();
    this->nameLabel->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    this->nameLabel->setAlignment(Qt::AlignCenter);
    this->nameLabel->setWordWrap(true);
    this->layout->addWidget(this->nameLabel);
}

void QAxResourceItemWidget::SetEditorInterface(IAxEditor *editor)
{
    QAxEditorWidget::SetEditorInterface(editor);

    this->button->SetEditorInterface(editor);
}

void QAxResourceItemWidget::SetResource(AxResource *resource)
{
    this->resource = resource;

    this->button->SetResource(resource);

    this->nameLabel->setText(EditorUtils::AxToQString(this->resource->name));
}
