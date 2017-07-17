#ifndef QMWPROPERTIESRESOURCEWIDGET_H
#define QMWPROPERTIESRESOURCEWIDGET_H

#include <QLabel>
#include <QLineEdit>

#include "QAxResourcePropertiesWidget.h"
#include "IAxEditor.h"

class QAxPropertiesResourceWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    bool CheckNameValidity(const AxString &name);

public:
    QLabel *nameIcon;
    QLineEdit *nameEdit;

    QAxResourcePropertiesWidget *propertiesWidget;

    AxResource *resource;

    explicit QAxPropertiesResourceWidget(QWidget *parent = 0);

    void SetEditorInterface(IAxEditor *editor);

    void SetResource(AxResource *resource);

signals:

public slots:
    void on_nameEdit_textChanged(const QString &arg1);
    void on_nameEdit_editingFinished();

};

#endif // QMWPROPERTIESRESOURCEWIDGET_H
