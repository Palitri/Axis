#ifndef QMWRESOURCEITEMWIDGET_H
#define QMWRESOURCEITEMWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

#include "QAxResourceItemButtonWidget.h"

class QAxResourceItemWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
public:
    AxResource *resource;

    QVBoxLayout *layout;
    QAxResourceItemButtonWidget *button;
    QLabel *nameLabel;

    explicit QAxResourceItemWidget(QWidget *parent = 0);

    void SetEditorInterface(IAxEditor *editor);

    void SetResource(AxResource *resource);

signals:

public slots:

};

#endif // QMWRESOURCEITEMWIDGET_H
