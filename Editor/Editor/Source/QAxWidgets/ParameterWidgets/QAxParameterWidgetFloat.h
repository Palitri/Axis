#ifndef QMWPARAMETERWIDGETFLOAT_H
#define QMWPARAMETERWIDGETFLOAT_H

#include <QDoubleSpinBox>

#include "QAxParameterWidget.h"

class QAxParameterWidgetFloat :
        public QDoubleSpinBox,
        public QAxParameterWidget
{
    Q_OBJECT
public:
    QAxParameterWidgetFloat(AxParameter *parameter, IAxEditor *editor);

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public slots:
    void OnValueChanged(double);

};

#endif // QMWPARAMETERWIDGETFLOAT_H
