//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxResourceItemButtonWidget.h"

#include <QVariant>
#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>
#include <QPaintEngine>

#include "../EditorTools/EditorUtils.h"

QAxResourceItemButtonWidget::QAxResourceItemButtonWidget(QWidget *parent) :
    QPushButton(parent),
    QAxEditorWidget()
{
    QObject::connect(this, SIGNAL(clicked()), this, SLOT(OnClicked()));
}

QIcon *QAxResourceItemButtonWidget::CreateIcon()
{
    QImage *thumbnail = 0;
    QImage *icon = 0;

    QString iconName;
    if (EditorUtils::resourceTypeIcons.GetValue(this->resource->resourceType, iconName))
        icon = new QImage(iconName);

    if (this->resource->resourceType == AxResourceType_Texture)
        thumbnail = this->editor->GetAxis()->RenderThumbnailTexture((AxTexture2D*)this->resource, this->iconSize().width(), this->iconSize().height());
    else if (this->resource->resourceType == AxResourceType_Mesh)
        thumbnail = this->editor->GetAxis()->RenderThumbnailMesh((AxMesh*)this->resource, this->iconSize().width(), this->iconSize().height());
    else if (this->resource->resourceType == AxResourceType_Material)
        thumbnail = this->editor->GetAxis()->RenderThumbnailMaterial((AxMaterial*)this->resource, this->iconSize().width(), this->iconSize().height());

    if ((thumbnail != 0) && (icon != 0))
    {
        QPainter paintEngine;
        paintEngine.begin(thumbnail);
        paintEngine.drawImage(0, thumbnail->height() - icon->height(), *icon);
        paintEngine.end();
    }

    QIcon *result;

    if (thumbnail != 0)
        result = new QIcon(QPixmap::fromImage(*thumbnail));
    else if (icon != 0)
        result = new QIcon(QPixmap::fromImage(*icon));
    else
        result = new QIcon();

    if (thumbnail != 0)
        delete thumbnail;

    if (icon != 0)
        delete icon;

    return result;
}

void QAxResourceItemButtonWidget::SetResource(AxResource *resource)
{
    this->resource = resource;

    this->setIconSize(QSize(68, 53));
    QIcon *icon = this->CreateIcon();
    this->setIcon(*icon);
    delete icon;
}

void QAxResourceItemButtonWidget::mousePressEvent(QMouseEvent *event)
{
    QPushButton::mousePressEvent(event);

    if (event->button() == Qt::LeftButton)
        dragStartPos = event->pos();
}

void QAxResourceItemButtonWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPushButton::mouseMoveEvent(event);

    if (!(event->buttons() & Qt::LeftButton))
        return;

    if ((event->pos() - dragStartPos).manhattanLength() < QApplication::startDragDistance())
        return;

    if (this->resource == 0)
        return;

    Qt::DropAction dropAction = EditorUtils::CreateResourceDrag(this, this->resource)->exec(Qt::CopyAction | Qt::MoveAction);
}

void QAxResourceItemButtonWidget::OnClicked()
{
    if (this->editor != 0)
        this->editor->SelectResource(this->resource);
}
