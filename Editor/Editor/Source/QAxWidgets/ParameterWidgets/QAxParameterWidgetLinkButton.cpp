//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetLinkButton.h"

#include "../../EditorTools/EditorUtils.h"

QAxParameterWidgetLinkButton::QAxParameterWidgetLinkButton(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QPushButton()
{
    this->parameter = parameter;

    AxResource *linkReference = (AxResource*)this->parameter->value;

    if (linkReference != 0)
    {
        this->setText(linkReference->name.GetCharContents());

        QString iconName;
        if (EditorUtils::resourceTypeIcons.GetValue(linkReference->resourceType, iconName))
            this->setIcon(QIcon(iconName));

        QObject::connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));
    }

    this->setAcceptDrops(true);
}

void QAxParameterWidgetLinkButton::dragEnterEvent(QDragEnterEvent *event)
{
    QAxParameterWidget::HandleDragEnterEvent(event);
}

void QAxParameterWidgetLinkButton::dropEvent(QDropEvent *event)
{
    QAxParameterWidget::HandleDragDropEvent(event);
}


void QAxParameterWidgetLinkButton::OnClicked()
{
    this->editor->SelectResource((AxResource*)this->parameter->value);
}
