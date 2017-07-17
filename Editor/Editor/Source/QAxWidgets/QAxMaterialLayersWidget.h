#ifndef QMWMATERIALLAYERSWIDGET_H
#define QMWMATERIALLAYERSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>

#include "QAxEditorWidget.h"

class QAxMaterialLayersWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    QWidget *layersWidget, *messageWidget;
    QLabel *messageText;

    AxMaterial *material;

    int selectedLayerIndex;

    void UpdateLayersPanel();
    void SetMessage();

    void SetShaderOpComboBox(QComboBox *comboBox, int layerIndex);
    void SetColorOpComboBox(QComboBox *comboBox, int layerIndex);

public:
    explicit QAxMaterialLayersWidget(QWidget *parent = 0);

    void SetResource(AxResource *resource);

signals:

public slots:
    void DeleteLayerClicked();
    void MoveLayerDownClicked();
    void MoveLayerUpClicked();
    void InsertLayerClicked();
    void AppendLayerClicked();

    void LayerSelected();
    void LayerShadingOperationChanged(int index);
    void LayerColorOperationChanged(int index);

};

#endif // QMWMATERIALLAYERSWIDGET_H
