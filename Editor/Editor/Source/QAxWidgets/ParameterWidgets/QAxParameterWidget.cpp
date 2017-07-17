//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxParameterWidget.h"

#include <QMimeData>

QAxParameterWidget::QAxParameterWidget(AxParameter *parameter, IAxEditor *editor)
{
    this->parameter = parameter;

    this->SetEditorInterface(editor);
}

void QAxParameterWidget::HandleDragEnterEvent(QDragEnterEvent *event)
{
    if (!event->mimeData()->hasFormat("application/mwresource"))
        return;

    unsigned int resource;
    if (!AxString(event->mimeData()->data("application/mwresource").data()).ToUInt(resource))
        return;

    AxResource *droppedResource = (AxResource*)resource;

    bool accept = false;

    switch (droppedResource->resourceType)
    {
        case AxResourceType_Mechanism:
        {
            accept = this->parameter->GetEffectiveParameter()->type == ((AxMechanism*)droppedResource)->parameter.GetEffectiveParameter()->type;
            break;
        }

        case AxResourceType_Texture:
        {
            accept = this->parameter->GetEffectiveParameter()->type == AxParameterType_ReferenceTexture;
            break;
        }
        case AxResourceType_Camera:
        {
            accept = this->parameter->GetEffectiveParameter()->type == AxParameterType_ReferenceCamera;
            break;
        }
        case AxResourceType_Transform:
        {
            accept = (this->parameter->GetEffectiveParameter()->type == AxParameterType_ReferenceTransform) ||
                     (this->parameter->GetEffectiveParameter()->type == AxParameterType_ReferenceVector3);
            break;
        }

        case AxResourceType_Sound:
        {
            accept = this->parameter->GetEffectiveParameter()->type == AxParameterType_ReferenceSound;
        }

        case AxResourceType_Mesh:
        {
            accept = this->parameter->GetEffectiveParameter()->type == AxParameterType_ReferenceMesh;
        }
    }

    if (accept)
        event->acceptProposedAction();
}

void QAxParameterWidget::HandleDragDropEvent(QDropEvent *event)
{
    unsigned int resource;
    if (!AxString(event->mimeData()->data("application/mwresource").data()).ToUInt(resource))
        return;

    AxResource *droppedResource = (AxResource*)resource;

    switch (droppedResource->resourceType)
    {
        case AxResourceType_Mechanism:
        {
            this->parameter->SetValue(droppedResource, AxParameterType_ReferenceMechanism);

            break;
        }

        default:
        {
            this->parameter->SetValue(droppedResource, this->parameter->GetEffectiveParameter()->type);
        }
    }

    event->acceptProposedAction();

    // TODO: Generally, UpdateResourceProperties() should not be called directly from here, as it recreates all the parameter widgets and therefore destroys this object while its calling method has not yet returned
    this->editor->UpdateResourceProperties(this->editor->GetSelectedResource());
}
