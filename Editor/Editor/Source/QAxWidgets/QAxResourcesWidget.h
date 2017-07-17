#ifndef QMWRESOURCESWIDGET_H
#define QMWRESOURCESWIDGET_H

#include <QWidget>
#include <QGridLayout>

#include "QAxEditorWidget.h"

class QAxResourcesWidget :
        public QWidget,
        public QAxEditorWidget

{
    Q_OBJECT
private:

public:
    AxResourceType filterType;
    AxString filterString;
    AxList<AxResource*> *items;

    QGridLayout *layout;

    explicit QAxResourcesWidget(QWidget *parent = 0);

    void RefreshItems();

    void SetItemSource(AxList<AxResource *> &items);

    void SetFilterType(AxResourceType filterType);
    void SetFilterString(const AxString &filterString);

    void RefreshItem(AxResource *resource);

signals:

public slots:

};

#endif // QMWRESOURCESWIDGET_H
