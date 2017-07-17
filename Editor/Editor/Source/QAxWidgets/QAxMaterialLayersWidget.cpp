//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxMaterialLayersWidget.h"

#include "../EditorTools/EditorUtils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QSpacerItem>

QAxMaterialLayersWidget::QAxMaterialLayersWidget(QWidget *parent) :
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



    // Shader layers panel
    this->layersWidget = new QWidget(this);
    new QGridLayout(this->layersWidget);
    mainLayout->addWidget(layersWidget);



    // Message panel
    this->messageWidget = new QWidget(this);
    QVBoxLayout *messageLayout = new QVBoxLayout(this->messageWidget);
    mainLayout->addWidget(messageWidget);

    messageLayout->addItem(new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Expanding));

    QLabel *messageIcon = new QLabel(this->messageWidget);
    messageIcon->setPixmap(QPixmap(EditorUtils::iconDelete));
    messageLayout->addWidget(messageIcon);

    this->messageText = new QLabel(this->messageWidget);
    this->messageText->setStyleSheet("color:brown;");
    messageLayout->addWidget(this->messageText);

    this->messageWidget->setVisible(false);

    this->selectedLayerIndex = -1;

    this->material = 0;
}

void QAxMaterialLayersWidget::SetResource(AxResource *resource)
{
    if (resource != this->material)
        this->selectedLayerIndex = -1;

    this->material = 0;

    if (resource != 0)
        if (resource->resourceType == AxResourceType_Material)
            this->material = (AxMaterial*)resource;

    this->UpdateLayersPanel();
    this->SetMessage();
}

void QAxMaterialLayersWidget::UpdateLayersPanel()
{
    QGridLayout *layersLayout = (QGridLayout*)this->layersWidget->layout();

    QLayoutItem *child;
    while ((child = layersLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    if (this->material == 0)
        return;

    for (int layerIndex = 0; layerIndex < this->material->shadingLayers.count; layerIndex++)
    {
        QRadioButton *layerSelectRadioButton = new QRadioButton(this->layersWidget);
        layerSelectRadioButton->setChecked(layerIndex == this->selectedLayerIndex);
        layerSelectRadioButton->setProperty("layerIndex", QVariant(layerIndex));
        layerSelectRadioButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        QObject::connect(layerSelectRadioButton, SIGNAL(clicked()), this, SLOT(LayerSelected()));
        layersLayout->addWidget(layerSelectRadioButton, layerIndex, 0);

        QComboBox *shadingOpComboBox = new QComboBox(this->layersWidget);
        this->SetShaderOpComboBox(shadingOpComboBox, layerIndex);
        QObject::connect(shadingOpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(LayerShadingOperationChanged(int)));
        layersLayout->addWidget(shadingOpComboBox, layerIndex, 1);

        QComboBox *colorOpComboBox = new QComboBox(this->layersWidget);
        this->SetColorOpComboBox(colorOpComboBox, layerIndex);
        QObject::connect(colorOpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(LayerColorOperationChanged(int)));
        layersLayout->addWidget(colorOpComboBox, layerIndex, 2);
    }
}

void QAxMaterialLayersWidget::SetMessage()
{
    if (this->material == 0)
    {
        this->messageWidget->setVisible(false);
        return;
    }

    this->messageText->setText(EditorUtils::AxToQString(this->material->compilationMessage));
    this->messageWidget->setVisible(!this->material->successfullyCompiled);
}

void QAxMaterialLayersWidget::SetShaderOpComboBox(QComboBox *comboBox, int layerIndex)
{
    comboBox->setProperty("layerIndex", QVariant(layerIndex));

    void *dictionaryNode = 0;
    while ((dictionaryNode = EditorUtils::ShadingOperationNames.GetNodeNext(dictionaryNode)) != 0)
    {
        comboBox->addItem(EditorUtils::ShadingOperationNames.GetNodeValue(dictionaryNode).value, QVariant((int)EditorUtils::ShadingOperationNames.GetNodeValue(dictionaryNode).key));
        comboBox->setMaxVisibleItems(1000);
        if (EditorUtils::ShadingOperationNames.GetNodeValue(dictionaryNode).key == this->material->shadingLayers[layerIndex].shadingOp)
            comboBox->setCurrentIndex(comboBox->count() - 1);
    }
}

void QAxMaterialLayersWidget::SetColorOpComboBox(QComboBox *comboBox, int layerIndex)
{
    comboBox->setProperty("layerIndex", QVariant(layerIndex));

    void *dictionaryNode = 0;
    while ((dictionaryNode = EditorUtils::colorOperationNames.GetNodeNext(dictionaryNode)) != 0)
    {
        comboBox->addItem(EditorUtils::colorOperationNames.GetNodeValue(dictionaryNode).value, QVariant((int)EditorUtils::colorOperationNames.GetNodeValue(dictionaryNode).key));
        comboBox->setMaxVisibleItems(1000);
        if (EditorUtils::colorOperationNames.GetNodeValue(dictionaryNode).key == this->material->shadingLayers[layerIndex].colorOp)
            comboBox->setCurrentIndex(comboBox->count() - 1);
    }
}

void QAxMaterialLayersWidget::DeleteLayerClicked()
{
    if (this->selectedLayerIndex == -1)
        return;

    if (this->material == 0)
        return;

    this->material->RemoveShadingLayer(this->selectedLayerIndex);
    this->material->UpdateShader();

    this->selectedLayerIndex--;

    this->UpdateLayersPanel();
    this->SetMessage();

    this->editor->UpdateResourceProperties(this->material);
}

void QAxMaterialLayersWidget::MoveLayerDownClicked()
{
    if (this->selectedLayerIndex == -1)
        return;

    if (this->material == 0)
        return;

    if (this->selectedLayerIndex >= this->material->shadingLayers.count - 1)
        return;

    this->material->MoveShadingLayer(this->selectedLayerIndex, this->selectedLayerIndex + 1);
    this->material->UpdateShader();

    this->selectedLayerIndex++;

    this->UpdateLayersPanel();
    this->SetMessage();

    this->editor->UpdateResourceProperties(this->material);
}

void QAxMaterialLayersWidget::MoveLayerUpClicked()
{
    if (this->selectedLayerIndex == -1)
        return;

    if (this->material == 0)
        return;

    if (this->selectedLayerIndex < 1)
        return;

    this->material->MoveShadingLayer(this->selectedLayerIndex, this->selectedLayerIndex - 1);
    this->material->UpdateShader();

    this->selectedLayerIndex--;

    this->UpdateLayersPanel();
    this->SetMessage();

    this->editor->UpdateResourceProperties(this->material);
}

void QAxMaterialLayersWidget::InsertLayerClicked()
{
    if (this->material == 0)
        return;

    this->selectedLayerIndex = AxMath::Trim(this->selectedLayerIndex + 1, 0, this->material->shadingLayers.count - 1);

    this->material->InsertShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set, this->selectedLayerIndex);
    this->material->UpdateShader();

    this->UpdateLayersPanel();
    this->SetMessage();

    this->editor->UpdateResourceProperties(this->material);
}

void QAxMaterialLayersWidget::AppendLayerClicked()
{
    if (this->material == 0)
        return;

    this->material->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->material->UpdateShader();

    this->selectedLayerIndex = this->material->shadingLayers.count - 1;

    this->UpdateLayersPanel();
    this->SetMessage();

    this->editor->UpdateResourceProperties(this->material);
}

void QAxMaterialLayersWidget::LayerSelected()
{
    QRadioButton *sender = (QRadioButton*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();

    this->selectedLayerIndex = layerIndex;
}

void QAxMaterialLayersWidget::LayerShadingOperationChanged(int index)
{
    QComboBox *sender = (QComboBox*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();
    AxShadingOperation shadingOp = (AxShadingOperation)sender->itemData(index).toInt();
    AxColorOperation colorOp = this->material->shadingLayers[layerIndex].colorOp;

    this->material->RemoveShadingLayer(layerIndex);
    this->material->InsertShadingLayer(shadingOp, colorOp, layerIndex);
    this->material->UpdateShader();

    this->SetMessage();

    this->editor->UpdateResourceProperties(this->material);
}

void QAxMaterialLayersWidget::LayerColorOperationChanged(int index)
{
    QComboBox *sender = (QComboBox*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();
    AxColorOperation colorOp = (AxColorOperation)sender->itemData(index).toInt();

    this->material->shadingLayers[layerIndex].colorOp = colorOp;
    this->material->UpdateShader();

    this->SetMessage();
}

