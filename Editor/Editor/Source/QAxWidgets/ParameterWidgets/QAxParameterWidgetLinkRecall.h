#ifndef QMWPARAMETERWIDGETLINKRECALL_H
#define QMWPARAMETERWIDGETLINKRECALL_H

#include <QPushButton>

#include "QAxParameterWidget.h"

class QAxParameterWidgetLinkRecall :
        public QPushButton,
        public QAxParameterWidget
{
    Q_OBJECT
public:
    QAxParameterWidgetLinkRecall(AxParameter *parameter, IAxEditor *editor);

public slots:
    void OnClicked();
};

#endif // QMWPARAMETERWIDGETLINKRECALL_H
