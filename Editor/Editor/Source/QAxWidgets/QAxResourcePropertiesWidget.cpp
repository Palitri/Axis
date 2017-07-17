//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxResourcePropertiesWidget.h"

#include "../QAxWidgets/ParameterWidgets/QAxParameterWidgetColor.h"
#include "../QAxWidgets/ParameterWidgets/QAxParameterWidgetLink.h"
#include "../QAxWidgets/ParameterWidgets/QAxParameterWidgetBool.h"
#include "../QAxWidgets/ParameterWidgets/QAxParameterWidgetInt.h"
#include "../QAxWidgets/ParameterWidgets/QAxParameterWidgetFloat.h"

#include "../EditorTools/EditorUtils.h"

#include <QLabel>

QWidget *QAxResourcePropertiesWidget::CreateAxParameterWidget(AxParameter *parameter, IAxEditor *editor)
{
    if (parameter->IsReference())
        return new QAxParameterWidgetLink(parameter, editor);

    switch (parameter->type)
    {
        case AxParameterType_Bool:
        {
            return new QAxParameterWidgetBool(parameter, editor);
        }

        case AxParameterType_Int:
        {
            return new QAxParameterWidgetInt(parameter, editor);
        }

        case AxParameterType_Float:
        {
            return new QAxParameterWidgetFloat(parameter, editor);
        }

        case AxParameterType_Color:
        {
            return new QAxParameterWidgetColor(parameter, editor);

            break;
        }

        default:
        {
            return 0;
        }
    }
}

QAxResourcePropertiesWidget::QAxResourcePropertiesWidget(QWidget *parent)
    : QWidget(parent)
{
    this->layout = new QGridLayout(this);
    this->resource = 0;
}

void QAxResourcePropertiesWidget::SetResource(AxResource *resource)
{
    this->resource = resource;

    QLayoutItem *child;
    while ((child = this->layout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    if (resource == 0)
        return;

    for (int i = 0; i < resource->properties.count; i++)
    {
        AxProperty *resourceProperty = resource->properties[i];

        this->layout->addWidget(new QLabel(resourceProperty->name.GetCharContents(), this), i, 0);

        QWidget *mwParameterWidget = QAxResourcePropertiesWidget::CreateAxParameterWidget(resourceProperty, this->editor);
        if (mwParameterWidget != 0)
            this->layout->addWidget(mwParameterWidget, i, 1);
    }
}
