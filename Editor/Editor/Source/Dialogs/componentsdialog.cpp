//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "componentsdialog.h"
#include "ui_componentsdialog.h"

#include "../EditorTools/EditorUtils.h"

#include <QSettings>

ComponentsDialog::ComponentsDialog(QWidget *parent, Axis *axis) :
    QDialog(parent),
    ui(new Ui::ComponentsDialog)
{
    ui->setupUi(this);

    this->ui->lComponents->viewport()->installEventFilter(this);

    QSettings settings;
    this->restoreGeometry(settings.value("componentsDialogGeometry").toByteArray());

    this->axis = axis;

    this->FillComponentTypes();
    this->FillModules();
}

ComponentsDialog::~ComponentsDialog()
{
    delete ui;

    QSettings settings;
    settings.setValue("componentsDialogGeometry", this->saveGeometry());
}

void ComponentsDialog::on_cbComponentTypes_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);

    unsigned int selectedComponentType = this->GetSelectedComponentType();
    AxList<AxModuleDispatcher*> *componentDispatchers = this->GetDispatchers(selectedComponentType);

    this->FillComponents(componentDispatchers);
}

void ComponentsDialog::on_lComponents_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(current);
    Q_UNUSED(previous);

    AxModuleDispatcher *component = this->GetSelectedComponent();
    this->FillComponentFeatures(component);
    this->HighlightComponentModule(component);
}

bool ComponentsDialog::eventFilter(QObject *object, QEvent *event)
{
    if (object == this->ui->lComponents->viewport())
    {
        if (event->type() == QEvent::Drop)
        {
            QDropEvent *dropEvent = (QDropEvent*)(event);

            // Invoke the list box's event, so that the items in the list get swapped by the drag/drop
            this->ui->lComponents->dropEvent(dropEvent);

            unsigned int selectedComponentType = this->GetSelectedComponentType();
            AxList<AxModuleDispatcher*> *componentDispatchers = this->GetDispatchers(selectedComponentType);
            AxList<AxModuleDispatcher*> oldOrder(componentDispatchers->values, componentDispatchers->count);

            for (int newIndex = 0; newIndex < this->ui->lComponents->count(); newIndex++)
            {
                QListWidgetItem *item = this->ui->lComponents->item(newIndex);
                int oldIndex = item->data(Qt::UserRole).toInt();

                item->setData(Qt::UserRole, QVariant(newIndex));
                componentDispatchers->Set(newIndex, oldOrder[oldIndex]);
            }
        }
    }

    return false; // Allow further processing
}



AxList<AxModuleDispatcher*> *ComponentsDialog::GetDispatchers(unsigned int typeId)
{
    switch (typeId)
    {
        case AxModuleDispatcher::typeId_Image:
            return &this->axis->imageMediaDispatchers;

        case AxModuleDispatcher::typeId_Sound:
            return &this->axis->soundMediaDispatchers;

        case AxModuleDispatcher::typeId_Scene:
            return &this->axis->sceneMediaDispatchers;

        case AxModuleDispatcher::typeId_GraphicsDevice:
            return &this->axis->graphicsDeviceDispatchers;

        case AxModuleDispatcher::typeId_AudioDevice:
            return &this->axis->audioDeviceDispatchers;

        case AxModuleDispatcher::typeId_FileSystem:
            return &this->axis->fileSystemDispatchers;

        case AxModuleDispatcher::typeId_InputDevice:
            return &this->axis->inputDeviceDispatchers;
    }

    return 0;
}

unsigned int ComponentsDialog::GetSelectedComponentType()
{
    return this->ui->cbComponentTypes->currentData().toInt();
}

int ComponentsDialog::GetSelectedComponentIndex()
{
    return this->ui->lComponents->currentIndex().row();
}

AxModuleDispatcher* ComponentsDialog::GetSelectedComponent()
{
    unsigned int selectedComponentType = this->GetSelectedComponentType();
    AxList<AxModuleDispatcher*> *componentDispatchers = this->GetDispatchers(selectedComponentType);
    int selectedComponentIndex = this->GetSelectedComponentIndex();

    return selectedComponentIndex < 0 ? 0 : componentDispatchers->Get(selectedComponentIndex);
}



void ComponentsDialog::FillModules()
{
    this->ui->lModule->clear();

    this->ui->lModule->addItem("[ Internal ]");

    for (int i = 0; i < this->axis->loadedModules.count; i++)
    {
        this->ui->lModule->addItem(EditorUtils::AxToQString(AxFileSystem::GetFileName(this->axis->loadedModules.Get(i)->name)));
    }
}

void ComponentsDialog::FillComponentTypes()
{
    this->ui->cbComponentTypes->clear();

    this->ui->cbComponentTypes->addItem("Image", QVariant((int)AxModuleDispatcher::typeId_Image));
    this->ui->cbComponentTypes->addItem("Sound", QVariant((int)AxModuleDispatcher::typeId_Sound));
    this->ui->cbComponentTypes->addItem("Scene", QVariant((int)AxModuleDispatcher::typeId_Scene));
    this->ui->cbComponentTypes->addItem("Graphics Device", QVariant((int)AxModuleDispatcher::typeId_GraphicsDevice));
    this->ui->cbComponentTypes->addItem("Audio Device", QVariant((int)AxModuleDispatcher::typeId_AudioDevice));
    this->ui->cbComponentTypes->addItem("Input Device", QVariant((int)AxModuleDispatcher::typeId_InputDevice));
    this->ui->cbComponentTypes->addItem("File System", QVariant((int)AxModuleDispatcher::typeId_FileSystem));
}

void ComponentsDialog::AddComponentFeature(AxModuleDispatcher* dispatcher, QString featureName, unsigned int featureInfoId)
{
    // Because this->ui->tFeatures->setWordWrap(true); doesn't seem to produce any word wrapping, a label with word wrap is inserted into the treeview
    AxString info;
    if (dispatcher->GetInfo(featureInfoId, 0, info))
    {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem(QStringList(featureName));
        QTreeWidgetItem *subItem = new QTreeWidgetItem(treeItem);
        QLabel *lCaption = new QLabel(EditorUtils::AxToQString(info));
        lCaption->setWordWrap(true);
        this->ui->tFeatures->addTopLevelItem(treeItem);
        this->ui->tFeatures->setItemWidget(subItem, 0, lCaption);
    };
}

void ComponentsDialog::AddComponentSerializationFeature(AxModuleDispatcher* dispatcher, QString featureName, unsigned int formatExtensionInfoId, unsigned int formatDescriptionInfoId)
{
    // No word wrapping for serialization formats, because it looks better without
    AxString extension, description;
    int tag = 0;
    QTreeWidgetItem *treeItem = new QTreeWidgetItem(QStringList(featureName));
    while (dispatcher->GetInfo(formatExtensionInfoId, tag, extension) && dispatcher->GetInfo(formatDescriptionInfoId, tag, description))
    {
        new QTreeWidgetItem(treeItem, QStringList(EditorUtils::AxToQString(extension + " - " + description)));
        tag++;
    };

    if (tag != 0)
        this->ui->tFeatures->addTopLevelItem(treeItem);
}

void ComponentsDialog::FillComponents(AxList<AxModuleDispatcher*>* dispatchers)
{
    this->ui->lComponents->clear();

    if (dispatchers == 0)
        return;

    AxString info;
    for (int i = 0; i < dispatchers->count; i++)
    {
        if (dispatchers->Get(i)->GetInfo(AxModuleDispatcher::infoId_Name, 0, info))
        {
            QListWidgetItem *item = new QListWidgetItem(EditorUtils::AxToQString(info));
            item->setData(Qt::UserRole, QVariant(i));
            this->ui->lComponents->addItem(item);
        }
    }
}

void ComponentsDialog::FillComponentFeatures(AxModuleDispatcher* dispatcher)
{
    this->ui->tFeatures->clear();

    if (dispatcher == 0)
        return;

    this->AddComponentFeature(dispatcher, "Description", AxModuleDispatcher::infoId_Description);
    this->AddComponentSerializationFeature(dispatcher, "Deserialization", AxModuleDispatcher::infoId_MediaDeserializationFormatExtension, AxModuleDispatcher::infoId_MediaDeserializationFormatDescription);
    this->AddComponentSerializationFeature(dispatcher, "Serialization", AxModuleDispatcher::infoId_MediaSerializationFormatExtension, AxModuleDispatcher::infoId_MediaSerializationFormatDescription);

    this->ui->tFeatures->expandAll();
}

void ComponentsDialog::HighlightComponentModule(AxModuleDispatcher* dispatcher)
{
    bool isInternal = dispatcher != 0;
    for (int i = 0; i < this->axis->loadedModules.count; i++)
    {
        bool isParentModule = this->axis->loadedModules.Get(i)->dispatchersReferences.Contains(dispatcher);
        this->ui->lModule->item(i + 1)->setSelected(isParentModule);
        isInternal &= !isParentModule;
    }
    this->ui->lModule->item(0)->setSelected(isInternal);
}
