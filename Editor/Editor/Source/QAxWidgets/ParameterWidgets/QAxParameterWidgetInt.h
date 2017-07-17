#ifndef QMWPARAMETERWIDGETINT_H
#define QMWPARAMETERWIDGETINT_H

#include <QSpinBox>

#include "QAxParameterWidget.h"

class QAxParameterWidgetInt :
        public QSpinBox,
        public QAxParameterWidget
{
    Q_OBJECT
public:
    explicit QAxParameterWidgetInt(AxParameter *parameter, IAxEditor *editor);

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

signals:

public slots:
    void OnValueChanged(int);
};

#endif // QMWPARAMETERWIDGETINT_H
