#ifndef QMWPARAMETERWIDGETCOLOR_H
#define QMWPARAMETERWIDGETCOLOR_H

#include <QWidget>

#include "QAxParameterWidgetColorRgb.h"
#include "QAxParameterWidgetColorAlpha.h"

class QAxParameterWidgetColor :
        public QWidget,
        public QAxParameterWidget
{
public:
    QAxParameterWidgetColorRgb *rgbWidget;
    QAxParameterWidgetColorAlpha *alphaWidget;

    explicit QAxParameterWidgetColor(AxParameter *parameter, IAxEditor *editor);
protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

};

#endif // QMWPARAMETERWIDGETCOLOR_H
