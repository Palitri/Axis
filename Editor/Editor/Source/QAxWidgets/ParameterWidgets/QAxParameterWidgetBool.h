#ifndef QMWPARAMETERWIDGETBOOL_H
#define QMWPARAMETERWIDGETBOOL_H

#include <QCheckBox>

#include "QAxParameterWidget.h"

class QAxParameterWidgetBool :
        public QCheckBox,
        public QAxParameterWidget
{
    Q_OBJECT
public:
    explicit QAxParameterWidgetBool(AxParameter *parameter, IAxEditor *editor);

signals:

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public slots:
    void OnStateChanged(int);

};

#endif // QMWPARAMETERWIDGETBOOL_H
