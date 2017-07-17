#ifndef QMWPARAMETERWIDGETCOLORALPHA_H
#define QMWPARAMETERWIDGETCOLORALPHA_H

#include <QDoubleSpinBox>
#include "QAxParameterWidget.h"

class QAxParameterWidgetColorAlpha :
        public QDoubleSpinBox,
        public QAxParameterWidget
{
    Q_OBJECT

public:
    explicit QAxParameterWidgetColorAlpha(AxParameter *parameter, IAxEditor *editor);

public slots:
    void OnValueChanged(double);
};

#endif // QMWPROPERTYWIDGETCOLORALPHA_H
