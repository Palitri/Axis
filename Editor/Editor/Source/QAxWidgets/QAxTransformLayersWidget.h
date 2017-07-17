#ifndef QMWTRANSFORMLAYERSWIDGET_H
#define QMWTRANSFORMLAYERSWIDGET_H

#include <QWidget>
#include <QComboBox>

#include "QAxEditorWidget.h"

class QAxTransformLayersWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    QWidget *layersWidget;

    AxTransform *transform;

    int selectedLayerIndex;

    void UpdateLayersPanel();

    void SetLayerComboBox(QComboBox *comboBox, int layerIndex);

public:
    explicit QAxTransformLayersWidget(QWidget *parent = 0);

    void SetResource(AxResource *resource);

signals:

public slots:
    void DeleteLayerClicked();
    void MoveLayerDownClicked();
    void MoveLayerUpClicked();
    void InsertLayerClicked();
    void AppendLayerClicked();

    void LayerSelected();
    void LayerOperationChanged(int index);

};

#endif // QMWTRANSFORMLAYERSWIDGET_H
