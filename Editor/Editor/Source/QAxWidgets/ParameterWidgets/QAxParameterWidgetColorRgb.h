#ifndef QMWPARAMETERWIDGETCOLORRGB_H
#define QMWPARAMETERWIDGETCOLORRGB_H

#include <QPushButton>
#include <QColorDialog>
#include "QAxParameterWidget.h"


class QAxParameterWidgetColorRgb :
        public QPushButton,
        public QAxParameterWidget
{
    Q_OBJECT

private:
    QColorDialog colorDialog;
    AxVector4 revertColor;

public:
    explicit QAxParameterWidgetColorRgb(AxParameter *parameter, IAxEditor *editor);

public slots:
    void OnClicked();

    void OnColorDialogColorChanged(const QColor & color);
    void OnColorDialogColorAccepted();
    void OnColorDialogColorRejected();
};

#endif // QMWPARAMETERWIDGETCOLORRGB_H
