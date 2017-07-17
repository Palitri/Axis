#ifndef QMWPARAMETERWIDGETLINK_H
#define QMWPARAMETERWIDGETLINK_H

#include <QWidget>

#include "QAxParameterWidgetLinkButton.h"
#include "QAxParameterWidgetLinkRecall.h"

class QAxParameterWidgetLink :
        public QWidget,
        public QAxParameterWidget
{
public:
    QAxParameterWidgetLinkButton *button;
    QAxParameterWidgetLinkRecall *recallButton;

    QAxParameterWidgetLink(AxParameter *parameter, IAxEditor *editor);
};

#endif // QMWPARAMETERWIDGETLINK_H
