//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidgetLinkRecall.h"

#include "../../EditorTools/EditorUtils.h"

QAxParameterWidgetLinkRecall::QAxParameterWidgetLinkRecall(AxParameter *parameter, IAxEditor *editor) :
    QAxParameterWidget(parameter, editor),
    QPushButton()
{
    this->parameter = parameter;

    this->setIcon(QIcon(EditorUtils::iconRecallParameter));

    QObject::connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

void QAxParameterWidgetLinkRecall::OnClicked()
{
    this->parameter->RecallEffectiveParameter();
    // TODO: Generally, UpdateResourceProperties() should not be called directly from here, as it recreates all the parameter widgets and therefore destroys this object while its calling method has not yet returned
    this->editor->UpdateResourceProperties(this->editor->GetSelectedResource());
}
