#ifndef QMWSTRUCTURESWIDGET_H
#define QMWSTRUCTURESWIDGET_H

//#include "../../Engine/Axis/Source/AxisEngine.h"

#include "QAxEditorWidget.h"

#include <QTreeWidget>
#include <QMouseEvent>
#include <QMenu>

class QAxStructuresWidget :
        public QTreeWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    QMenu *contextMenu;
    QTreeWidgetItem *contextMenuItem;

    int dragMode;

    QTreeWidgetItem *GetItemParent(QTreeWidgetItem *item);
    void InsertItem(QTreeWidgetItem *destItemParent, QTreeWidgetItem *item, int destIndex);

public:
    static const int DragMode_None = 0;
    static const int DragMode_Resource = 1;

    explicit QAxStructuresWidget(QWidget *parent = 0);

    virtual void SetEditorInterface(IAxEditor *editor);

    void BuildAnew(AxEntitySet *entitySet);
    int FindResourceItems(AxList<QTreeWidgetItem*> &result, AxResource *resource);

    AxResource *GetSelectedItem();

    static AxResource *GetAxResourceFromItem(QTreeWidgetItem *item);
    static void SetAxResourceToItem(QTreeWidgetItem *item, AxResource *resource);

    // Returns the number of items found and added to result
    static int FindStructureChildItems(AxList<QTreeWidgetItem*> &result, QTreeWidgetItem *item, AxResource *resource, bool subItems = true);
    static void BuildStructureTreeNode(AxEntitySet *resourceNode, QTreeWidgetItem *viewNode, bool recursive);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

signals:

public slots:
    void CurrentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);

    void ContextMenuRequested(QPoint pos);
    void ContextMenuDeleteTriggered();
    void ContextMenuWrapInEntitySetTriggered();

private:
    QPoint dragStartPos;

};

#endif // QMWSTRUCTURESWIDGET_H
