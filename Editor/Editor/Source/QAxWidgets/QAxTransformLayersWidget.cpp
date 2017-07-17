//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxTransformLayersWidget.h"

#include "../EditorTools/EditorUtils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QSpacerItem>

QAxTransformLayersWidget::QAxTransformLayersWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Control panel with buttons
    QWidget *controlPanelWidget = new QWidget(this);
    QHBoxLayout *controlPanelLayout = new QHBoxLayout(controlPanelWidget);
    mainLayout->addWidget(controlPanelWidget);

    QPushButton *deleteButton = new QPushButton(controlPanelWidget);
    deleteButton->setIcon(QIcon(EditorUtils::iconDelete));
    QObject::connect(deleteButton, SIGNAL(clicked()), this, SLOT(DeleteLayerClicked()));
    controlPanelLayout->addWidget(deleteButton);

    controlPanelLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *downButton = new QPushButton(controlPanelWidget);
    downButton->setIcon(QIcon(EditorUtils::iconMoveDown));
    QObject::connect(downButton, SIGNAL(clicked()), this, SLOT(MoveLayerDownClicked()));
    controlPanelLayout->addWidget(downButton);

    QPushButton *upButton = new QPushButton(controlPanelWidget);
    upButton->setIcon(QIcon(EditorUtils::iconMoveUp));
    QObject::connect(upButton, SIGNAL(clicked()), this, SLOT(MoveLayerUpClicked()));
    controlPanelLayout->addWidget(upButton);

    controlPanelLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

    QPushButton *insertButton = new QPushButton(controlPanelWidget);
    insertButton->setIcon(QIcon(EditorUtils::iconInsert));
    QObject::connect(insertButton, SIGNAL(clicked()), this, SLOT(InsertLayerClicked()));
    controlPanelLayout->addWidget(insertButton);

    QPushButton *appendButton = new QPushButton(controlPanelWidget);
    appendButton->setIcon(QIcon(EditorUtils::iconNew));
    QObject::connect(appendButton, SIGNAL(clicked()), this, SLOT(AppendLayerClicked()));
    controlPanelLayout->addWidget(appendButton);



    // Layers panel
    this->layersWidget = new QWidget(this);
    new QGridLayout(this->layersWidget);
    mainLayout->addWidget(layersWidget);



    this->selectedLayerIndex = -1;

    this->transform = 0;
}

void QAxTransformLayersWidget::SetResource(AxResource *resource)
{
    if (resource != this->transform)
        this->selectedLayerIndex = -1;

    this->transform = 0;

    if (resource != 0)
        if (resource->resourceType == AxResourceType_Transform)
            this->transform = (AxTransform*)resource;

    this->UpdateLayersPanel();
}


void QAxTransformLayersWidget::UpdateLayersPanel()
{
    QGridLayout *layersLayout = (QGridLayout*)this->layersWidget->layout();

    QLayoutItem *child;
    while ((child = layersLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    if (this->transform == 0)
        return;

    for (int layerIndex = 0; layerIndex < this->transform->transformLayers.count; layerIndex++)
    {
        QRadioButton *layerSelectRadioButton = new QRadioButton(this->layersWidget);
        layerSelectRadioButton->setChecked(layerIndex == this->selectedLayerIndex);
        layerSelectRadioButton->setProperty("layerIndex", QVariant(layerIndex));
        layerSelectRadioButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        QObject::connect(layerSelectRadioButton, SIGNAL(clicked()), this, SLOT(LayerSelected()));
        layersLayout->addWidget(layerSelectRadioButton, layerIndex, 0);

        QComboBox *transformOpComboBox = new QComboBox(this->layersWidget);
        this->SetLayerComboBox(transformOpComboBox, layerIndex);
        QObject::connect(transformOpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(LayerOperationChanged(int)));
        layersLayout->addWidget(transformOpComboBox, layerIndex, 1);
    }
}


void QAxTransformLayersWidget::SetLayerComboBox(QComboBox *comboBox, int layerIndex)
{
    comboBox->setProperty("layerIndex", QVariant(layerIndex));

    void *dictionaryNode = 0;
    while ((dictionaryNode = EditorUtils::transformOperationNames.GetNodeNext(dictionaryNode)) != 0)
    {
        comboBox->addItem(EditorUtils::transformOperationNames.GetNodeValue(dictionaryNode).value, QVariant((int)EditorUtils::transformOperationNames.GetNodeValue(dictionaryNode).key));
        comboBox->setMaxVisibleItems(1000);
        if (EditorUtils::transformOperationNames.GetNodeValue(dictionaryNode).key == this->transform->transformLayers[layerIndex].operation)
            comboBox->setCurrentIndex(comboBox->count() - 1);
    }
}


// SLOTS

void QAxTransformLayersWidget::DeleteLayerClicked()
{
    if (this->selectedLayerIndex == -1)
        return;

    if (this->transform == 0)
        return;

    this->transform->RemoveTransformLayer(this->selectedLayerIndex);

    this->selectedLayerIndex--;

    this->UpdateLayersPanel();

    this->editor->UpdateResourceProperties(this->transform);
}

void QAxTransformLayersWidget::MoveLayerDownClicked()
{
}

void QAxTransformLayersWidget::MoveLayerUpClicked()
{
}

void QAxTransformLayersWidget::InsertLayerClicked()
{
    if (this->transform == 0)
        return;

    this->selectedLayerIndex = AxMath::Trim(this->selectedLayerIndex + 1, 0, this->transform->transformLayers.count - 1);

    this->transform->InsertTransformLayer(AxTransformOperation_Translate, this->selectedLayerIndex);

    this->selectedLayerIndex = this->transform->transformLayers.count - 1;

    this->UpdateLayersPanel();

    this->editor->UpdateResourceProperties(this->transform);
}

void QAxTransformLayersWidget::AppendLayerClicked()
{
    if (this->transform == 0)
        return;

    this->transform->AddTransformLayer(AxTransformOperation_Translate);

    this->selectedLayerIndex = this->transform->transformLayers.count - 1;

    this->UpdateLayersPanel();

    this->editor->UpdateResourceProperties(this->transform);
}


void QAxTransformLayersWidget::LayerSelected()
{
    QRadioButton *sender = (QRadioButton*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();

    this->selectedLayerIndex = layerIndex;
}

void QAxTransformLayersWidget::LayerOperationChanged(int index)
{
    QComboBox *sender = (QComboBox*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();
    AxTransformOperation transformOp = (AxTransformOperation)sender->itemData(index).toInt();

    this->transform->RemoveTransformLayer(layerIndex);
    this->transform->InsertTransformLayer(transformOp, layerIndex);

    this->editor->UpdateResourceProperties(this->transform);
}

