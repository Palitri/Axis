//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxStructuresWidget.h"

#include "../EditorTools/EditorUtils.h"

#include <QApplication>
#include <QDrag>
#include <QMimeData>

QAxStructuresWidget::QAxStructuresWidget(QWidget *parent) :
    QTreeWidget(parent)
{
    this->contextMenu = new QMenu(this);
    QAction *deleteAction = new QAction("Delete", this);
    QObject::connect(deleteAction, SIGNAL(triggered()), this, SLOT(ContextMenuDeleteTriggered()));
    this->contextMenu->addAction(deleteAction);

    QAction *wrapInEntitySetAction = new QAction("Wrap in Entity set", this);
    QObject::connect(wrapInEntitySetAction, SIGNAL(triggered()), this, SLOT(ContextMenuWrapInEntitySetTriggered()));
    this->contextMenu->addAction(wrapInEntitySetAction);

    QObject::connect(this, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(CurrentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    this->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), SLOT(ContextMenuRequested(QPoint)));

    this->dragMode = QAxStructuresWidget::DragMode_None;
}

void QAxStructuresWidget::SetEditorInterface(IAxEditor *editor)
{
    QAxEditorWidget::SetEditorInterface(editor);

    QAxStructuresWidget::SetAxResourceToItem(this->invisibleRootItem(), this->editor->GetAxis()->root);
}

void QAxStructuresWidget::BuildAnew(AxEntitySet *entitySet)
{
    this->clear();

    this->BuildStructureTreeNode(entitySet, this->invisibleRootItem(), true);
}

int QAxStructuresWidget::FindResourceItems(AxList<QTreeWidgetItem*> &result, AxResource *resource)
{
    return QAxStructuresWidget::FindStructureChildItems(result, this->invisibleRootItem(), resource, true);
}

AxResource *QAxStructuresWidget::GetSelectedItem()
{
    if (this->selectedItems().count() == 0)
        return 0;

    return QAxStructuresWidget::GetAxResourceFromItem(this->selectedItems()[0]);
}

AxResource *QAxStructuresWidget::GetAxResourceFromItem(QTreeWidgetItem *item)
{
    if (item == 0)
        return 0;

    return (AxResource*)item->data(0, Qt::UserRole).toUInt();
}

void QAxStructuresWidget::SetAxResourceToItem(QTreeWidgetItem *item, AxResource *resource)
{
    QString iconName;
    if (EditorUtils::resourceTypeIcons.GetValue(resource->resourceType, iconName))
        item->setIcon(0, QIcon(iconName));
    item->setText(0, resource->name.GetCharContents());

    item->setData(0, Qt::UserRole, QVariant::fromValue((unsigned int)resource));
}

int QAxStructuresWidget::FindStructureChildItems(AxList<QTreeWidgetItem*> &result, QTreeWidgetItem *item, AxResource *resource, bool subItems)
{
    if (resource == 0)
        return 0;

    int numResultItems = result.count;

    int numChildren = item->childCount();
    for (int i = 0; i < numChildren; i++)
    {
        QTreeWidgetItem *childItem = item->child(i);
        if (resource == ((AxResource*)childItem->data(0, Qt::UserRole).value<unsigned int>()))
        {
            result.Add(childItem);
        }
        else
        {
            if (subItems)
            {
                QAxStructuresWidget::FindStructureChildItems(result, childItem, resource, subItems);
            }
        }
    }

    return result.count - numResultItems;
}

void QAxStructuresWidget::BuildStructureTreeNode(AxEntitySet *resourceNode, QTreeWidgetItem *viewNode, bool recursive)
{
    while (viewNode->childCount() > 0)
        viewNode->removeChild(viewNode->child(0));

    QAxStructuresWidget::SetAxResourceToItem(viewNode, resourceNode);

    for (int i = 0; i < resourceNode->references.count; i++)
    {
        AxResource *reference = resourceNode->references[i];

        QTreeWidgetItem *item = new QTreeWidgetItem(viewNode);

        QAxStructuresWidget::SetAxResourceToItem(item, reference);

        viewNode->addChild(item);

        if (reference->resourceType == AxResourceType_EntitySet)
            if (recursive)
                QAxStructuresWidget::BuildStructureTreeNode((AxEntitySet*)reference, item, recursive);
    }
}

void QAxStructuresWidget::mousePressEvent(QMouseEvent *event)
{
    QTreeWidget::mousePressEvent(event);
    //return;

    if (event->button() == Qt::LeftButton)
        this->dragStartPos = event->pos();

    this->dragMode = QAxStructuresWidget::DragMode_None;
}

void QAxStructuresWidget::mouseMoveEvent(QMouseEvent *event)
{
    QTreeWidget::mouseMoveEvent(event);
    //return;

    if (!(event->buttons() & Qt::LeftButton))
        return;

//    if ((event->pos() - this->dragStartPos).manhattanLength() < QApplication::startDragDistance())
//        return;


    QTreeWidgetItem *draggedItem = this->itemAt(this->dragStartPos);
    AxResource *selectedResource = QAxStructuresWidget::GetAxResourceFromItem(draggedItem);
    if (selectedResource == 0)
        return;

    switch (this->dragMode)
    {
        case QAxStructuresWidget::DragMode_None:
        {
            QDrag *drag = new QDrag(this);
            QMimeData *mimeData = new QMimeData;
            mimeData->setData("application/mwresource", AxString((unsigned int)selectedResource).GetCharContents());
            mimeData->setData("application/twitem", AxString((unsigned int)draggedItem).GetCharContents());
            drag->setMimeData(mimeData);
            QString iconName;
            if (EditorUtils::resourceTypeIcons.GetValue(selectedResource->resourceType, iconName))
                drag->setPixmap(QPixmap(iconName));

            this->dragMode = QAxStructuresWidget::DragMode_Resource;

            drag->exec(Qt::CopyAction | Qt::MoveAction);

            break;
        }
    }
}

void QAxStructuresWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void QAxStructuresWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (this->editor == 0)
    {
        event->ignore();
        return;
    }

    if (!event->mimeData()->hasFormat("application/mwresource"))
    {
        event->ignore();
        return;
    }

    event->accept();
}

void QAxStructuresWidget::dropEvent(QDropEvent *event)
{
    unsigned int resource;
    if (!AxString(event->mimeData()->data("application/mwresource").data()).ToUInt(resource))
    {
        event->ignore();
        return;
    }

    AxResource *droppedResource = (AxResource*)resource;

    QTreeWidgetItem *destItemParent;
    AxEntitySet *destParentSet;
    int destIndex;
    QTreeWidgetItem *destItem = this->itemAt(event->pos());
    if (destItem != 0)
    {
        destItemParent = this->GetItemParent(destItem);
        destParentSet = (AxEntitySet*)this->GetAxResourceFromItem(destItemParent);
        destIndex = destItemParent->indexOfChild(destItem);
    }
    else
    {
        destItemParent = this->invisibleRootItem();
        destParentSet = this->editor->GetAxis()->root;
        destIndex = this->editor->GetAxis()->root->references.count;
    }

    switch (this->dragMode)
    {
        case QAxStructuresWidget::DragMode_None:
        {
            destParentSet->references.Insert(destIndex, droppedResource);

            QTreeWidgetItem *item = new QTreeWidgetItem(destItemParent);
            QAxStructuresWidget::SetAxResourceToItem(item, droppedResource);
            this->InsertItem(destItemParent, destItem, destIndex);

            event->acceptProposedAction();

            break;
        }

        // Dragging has started from this widget, so the dropped resource has to be moved. In order to do that, first we need to remove it from its old location
        case QAxStructuresWidget::DragMode_Resource:
        {
            unsigned int sourceItemAddress;
            if (!AxString(event->mimeData()->data("application/twitem").data()).ToUInt(sourceItemAddress))
                break;

            QTreeWidgetItem *sourceItem = (QTreeWidgetItem*)sourceItemAddress;
            if (sourceItem == destItem)
                break;

            QTreeWidgetItem *sourceItemParent = this->GetItemParent(sourceItem);
            AxEntitySet *sourceParentSet = (AxEntitySet*)this->GetAxResourceFromItem(sourceItemParent);
            int sourceIndex = sourceItemParent->indexOfChild(sourceItem);

            if ((sourceItemParent == destItemParent) && (sourceIndex < destIndex))
                destIndex--;

            sourceParentSet->references.RemoveAt(sourceIndex);
            sourceItemParent->removeChild(sourceItem);

            destParentSet->references.Insert(destIndex, droppedResource);
            this->InsertItem(destItemParent, sourceItem, destIndex);

            event->acceptProposedAction();

            break;
        }
    }
}

void QAxStructuresWidget::CurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);

    this->editor->SelectResource(QAxStructuresWidget::GetAxResourceFromItem(current));
}

void QAxStructuresWidget::ContextMenuRequested(QPoint pos)
{
    this->contextMenuItem = this->itemAt(pos);

    if (this->contextMenuItem != 0)
        this->contextMenu->popup(this->viewport()->mapToGlobal(pos));
}

void QAxStructuresWidget::ContextMenuDeleteTriggered()
{
    QTreeWidgetItem *contextItemParent = this->GetItemParent(this->contextMenuItem);
    AxEntitySet *contextParentSet = (AxEntitySet*)this->GetAxResourceFromItem(contextItemParent);
    int contextItemIndex = contextItemParent->indexOfChild(this->contextMenuItem);

    contextParentSet->references.RemoveAt(contextItemIndex);

    contextItemParent->removeChild(this->contextMenuItem);
}

void QAxStructuresWidget::ContextMenuWrapInEntitySetTriggered()
{
    AxResource *contextMenuResource = this->GetAxResourceFromItem(this->contextMenuItem);
    QTreeWidgetItem *contextItemParent = this->GetItemParent(this->contextMenuItem);
    AxEntitySet *contextParentSet = (AxEntitySet*)this->GetAxResourceFromItem(contextItemParent);
    int contextItemIndex = contextItemParent->indexOfChild(this->contextMenuItem);

    // Create new entity set and put the resoruce inside
    AxEntitySet *newSet = (AxEntitySet*)this->editor->GetAxis()->AddResource(new AxEntitySet());
    newSet->name = this->editor->GetAxis()->AcquireResourceName(contextMenuResource->name, AxResourceType_EntitySet);
    newSet->references.Add(contextMenuResource);

    // Remove the resource and put the new entity set in its place
    contextParentSet->references.RemoveAt(contextItemIndex);
    contextParentSet->references.Insert(contextItemIndex, newSet);

    // Create the new tree widget item
    QTreeWidgetItem *item = new QTreeWidgetItem(contextItemParent);
    this->BuildStructureTreeNode(newSet, item, true);

    // Remove the old tree widget item and put the new one in its place
    contextItemParent->removeChild(this->contextMenuItem);
    this->InsertItem(contextItemParent, item, contextItemIndex);
}

QTreeWidgetItem *QAxStructuresWidget::GetItemParent(QTreeWidgetItem *item)
{
    QTreeWidgetItem *result = item->parent();

    if (result != 0)
        return result;

    return this->invisibleRootItem();
}

// Method to be used instead of QAxStructuresWidget.insertChild(), which does not work as expected
void QAxStructuresWidget::InsertItem(QTreeWidgetItem *destItemParent, QTreeWidgetItem *item, int destIndex)
{
    //destItemParent->insertChild(destIndex, item); // Does not work. Always "inserts" the item at the end (Qt 5.2.1)

    destItemParent->addChild(item);

    int swapCount = destItemParent->childCount() - destIndex - 1;
    for (int i = 0; i < swapCount; i++)
    {
        QTreeWidgetItem *child = destItemParent->child(destIndex);
        destItemParent->removeChild(child);
        destItemParent->addChild(child);
    }
}
