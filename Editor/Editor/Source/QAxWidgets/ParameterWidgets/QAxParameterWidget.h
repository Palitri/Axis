#ifndef QMWPARAMETERWIDGET_H
#define QMWPARAMETERWIDGET_H

#include "../QAxEditorWidget.h"

#include <QDragEnterEvent>
#include <QDropEvent>

class QAxParameterWidget :
        public QAxEditorWidget
{
public:
    AxParameter *parameter;

    QAxParameterWidget(AxParameter *parameter, IAxEditor *editor);

protected:
    // Methods to handle events in inheriting classes. Can't simply override the events here, because this class shouldn't inherit QWidget, as its descendants themselves will inherit different classes descendants of QWidget, such as QCheckBox, QSpinEdit, QPushButton and etc.
    void HandleDragEnterEvent(QDragEnterEvent *event);
    void HandleDragDropEvent(QDropEvent *event);
};

#endif // QMWPARAMETERWIDGET_H
