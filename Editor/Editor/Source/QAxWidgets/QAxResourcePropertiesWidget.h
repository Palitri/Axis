#ifndef QMWRESOURCEPROPERTIESWIDGET_H
#define QMWRESOURCEPROPERTIESWIDGET_H

#include <QWidget>
#include <QGridLayout>

#include "../AxisEngine.h"

#include "ParameterWidgets/QAxParameterWidget.h"

class QAxResourcePropertiesWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT

public:
    static QWidget *CreateAxParameterWidget(AxParameter *parameter, IAxEditor *editor = 0);

    QGridLayout *layout;

    AxResource *resource;

    QAxResourcePropertiesWidget(QWidget *parent = 0);

    void SetResource(AxResource *resource);
};

#endif // QMWRESOURCEPROPERTIESWIDGET_H
