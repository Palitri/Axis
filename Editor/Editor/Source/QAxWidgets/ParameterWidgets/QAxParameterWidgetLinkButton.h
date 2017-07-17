#ifndef QMWPARAMETERWIDGETLINKBUTTON_H
#define QMWPARAMETERWIDGETLINKBUTTON_H

#include <QPushButton>

#include "QAxParameterWidget.h"

class QAxParameterWidgetLinkButton :
        public QPushButton,
        public QAxParameterWidget
{
    Q_OBJECT
public:
    explicit QAxParameterWidgetLinkButton(AxParameter *parameter, IAxEditor *editor);

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void OnClicked();
};

#endif // QMWPARAMETERWIDGETLINKBUTTON_H
