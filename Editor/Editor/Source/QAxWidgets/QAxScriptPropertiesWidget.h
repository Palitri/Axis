#ifndef QMWSCRIPTPROPERTIESWIDGET_H
#define QMWSCRIPTPROPERTIESWIDGET_H

#include <QWidget>
#include <QComboBox>

#include "QAxEditorWidget.h"

class QAxScriptPropertiesWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    void SetPropertyTypeComboBox(QComboBox *comboBox, int layerIndex);
    void UpdateLayersPanel();

public:
    QWidget *layersWidget;

    AxScript *script;

    int selectedLayerIndex;

    explicit QAxScriptPropertiesWidget(QWidget *parent = 0);

    void SetResource(AxResource *resource);

signals:

public slots:
    void DeleteLayerClicked();
    void AppendLayerClicked();

    void LayerSelected();
    void LayerNameChanged();
    void LayerTypeChanged(int index);

};

#endif // QMWSCRIPTPROPERTIESWIDGET_H
