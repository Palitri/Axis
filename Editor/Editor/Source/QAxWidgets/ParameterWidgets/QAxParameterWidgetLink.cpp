//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetLink.h"

#include <QHBoxLayout>

QAxParameterWidgetLink::QAxParameterWidgetLink(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QWidget()
{
    this->parameter = parameter;

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    this->button = new QAxParameterWidgetLinkButton(this->parameter, editor);
    layout->addWidget(this->button);

    AxResource *linkReference = (AxResource*)this->parameter->value;

    if ((linkReference != 0) && (linkReference->resourceType == AxResourceType_Mechanism))
    {
        this->recallButton = new QAxParameterWidgetLinkRecall(this->parameter, editor);
        this->recallButton->setMaximumWidth(30);

        layout->addWidget(this->recallButton);
    }
    else
        this->recallButton = 0;
}
