//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxScriptPropertiesWidget.h"

#include "../EditorTools/EditorUtils.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QRadioButton>
#include <QLineEdit>
#include <QSpacerItem>

QAxScriptPropertiesWidget::QAxScriptPropertiesWidget(QWidget *parent) :
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

    QPushButton *appendButton = new QPushButton(controlPanelWidget);
    appendButton->setIcon(QIcon(EditorUtils::iconNew));
    QObject::connect(appendButton, SIGNAL(clicked()), this, SLOT(AppendLayerClicked()));
    controlPanelLayout->addWidget(appendButton);



    // Properties control panel
    this->layersWidget = new QWidget(this);
    new QGridLayout(this->layersWidget);
    mainLayout->addWidget(layersWidget);


    this->script = 0;

    this->selectedLayerIndex = -1;
}

void QAxScriptPropertiesWidget::SetResource(AxResource *resource)
{
    this->script = 0;

    if (resource != 0)
        if (resource->resourceType == AxResourceType_Script)
            this->script = (AxScript*)resource;

    this->UpdateLayersPanel();
}

void QAxScriptPropertiesWidget::SetPropertyTypeComboBox(QComboBox *comboBox, int layerIndex)
{
    comboBox->setProperty("layerIndex", QVariant(layerIndex));

    void *dictionaryNode = 0;
    while ((dictionaryNode = EditorUtils::parameterTypeNames.GetNodeNext(dictionaryNode)) != 0)
    {
        comboBox->addItem(EditorUtils::parameterTypeNames.GetNodeValue(dictionaryNode).value, QVariant((int)EditorUtils::parameterTypeNames.GetNodeValue(dictionaryNode).key));
        comboBox->setMaxVisibleItems(1000);
        if (EditorUtils::parameterTypeNames.GetNodeValue(dictionaryNode).key == this->script->properties[layerIndex]->type)
            comboBox->setCurrentIndex(comboBox->count() - 1);
    }
}

void QAxScriptPropertiesWidget::UpdateLayersPanel()
{
    QGridLayout *layersLayout = (QGridLayout*)this->layersWidget->layout();

    QLayoutItem *child;
    while ((child = layersLayout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    if (this->script == 0)
        return;

    for (int layerIndex = 0; layerIndex < this->script->properties.count; layerIndex++)
    {
        QRadioButton *layerSelectRadioButton = new QRadioButton(this->layersWidget);
        layerSelectRadioButton->setChecked(layerIndex == this->selectedLayerIndex);
        layerSelectRadioButton->setProperty("layerIndex", QVariant(layerIndex));
        layerSelectRadioButton->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Minimum);
        QObject::connect(layerSelectRadioButton, SIGNAL(clicked()), this, SLOT(LayerSelected()));
        layersLayout->addWidget(layerSelectRadioButton, layerIndex, 0);

        QLineEdit *layerNameEdit = new QLineEdit(this->layersWidget);
        layerNameEdit->setText(EditorUtils::AxToQString(this->script->properties[layerIndex]->name));
        QObject::connect(layerNameEdit, SIGNAL(editingFinished()), this, SLOT(LayerNameChanged()));
        layersLayout->addWidget(layerNameEdit, layerIndex, 1);

        QComboBox *shadingOpComboBox = new QComboBox(this->layersWidget);
        this->SetPropertyTypeComboBox(shadingOpComboBox, layerIndex);
        QObject::connect(shadingOpComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(LayerTypeChanged(int)));
        layersLayout->addWidget(shadingOpComboBox, layerIndex, 2);
    }
}

void QAxScriptPropertiesWidget::DeleteLayerClicked()
{
    if (this->selectedLayerIndex == -1)
        return;

    if (this->script == 0)
        return;

    delete this->script->properties[this->selectedLayerIndex];
    this->script->properties.RemoveAt(this->selectedLayerIndex);

    this->selectedLayerIndex--;

    this->UpdateLayersPanel();

    this->editor->UpdateResourceProperties(this->script);
}

void QAxScriptPropertiesWidget::AppendLayerClicked()
{
    if (this->script == 0)
        return;

    this->script->properties.Add(new AxProperty("New Property", 0, AxParameterType_None));

    this->selectedLayerIndex = this->script->properties.count - 1;

    this->UpdateLayersPanel();

    this->editor->UpdateResourceProperties(this->script);
}


void QAxScriptPropertiesWidget::LayerSelected()
{
    QRadioButton *sender = (QRadioButton*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();

    this->selectedLayerIndex = layerIndex;
}

void QAxScriptPropertiesWidget::LayerNameChanged()
{
    QLineEdit *sender = (QLineEdit*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();
    AxString name = EditorUtils::QToAxString(sender->text());

    this->script->properties[layerIndex]->name = name;

    this->editor->UpdateResourceProperties(this->script);
}

void QAxScriptPropertiesWidget::LayerTypeChanged(int index)
{
    QComboBox *sender = (QComboBox*)QObject::sender();
    int layerIndex = sender->property("layerIndex").toInt();
    AxParameterType type = (AxParameterType)sender->itemData(index).toInt();


    this->script->properties[layerIndex]->SetType(type);

    this->editor->UpdateResourceProperties(this->script);
}
