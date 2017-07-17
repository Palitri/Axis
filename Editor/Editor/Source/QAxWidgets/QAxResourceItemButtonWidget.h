#ifndef QMWRESOURCEITEMBUTTONWIDGET_H
#define QMWRESOURCEITEMBUTTONWIDGET_H

#include <QPushButton>

#include "QAxEditorWidget.h"

class QAxResourceItemButtonWidget :
        public QPushButton,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    QPoint dragStartPos;
    AxResource *GetAxResource();

    QIcon *CreateIcon();

public:
    AxResource *resource;

    explicit QAxResourceItemButtonWidget(QWidget *parent = 0);

    void SetResource(AxResource *resource);

protected:
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:
    void OnClicked();

};

#endif // QMWRESOURCEITEMBUTTONWIDGET_H
