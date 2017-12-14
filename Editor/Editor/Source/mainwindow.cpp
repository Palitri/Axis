//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "EditorTools/EditorUtils.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Dialogs/componentsdialog.h"
#include "Dialogs/resourcepropertiesdialog.h"

#include "Engine/AxEditorRenderEvents.h"

#include "VersionInfo.h"

#include <QSettings>
#include <QFileDialog>
#include <QDrag>
#include <QMimeData>
#include <QPixmap>
#include <QDesktopServices>
#include <QMessageBox>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //AxPlatformUtils::ShowMessage(AxPlatformUtils::GetApplicationPath().GetContents(), 0);

    ui->setupUi(this);

    this->paused = false;
    this->ui->sbPlaySpeed->setRange(-100.0, 100.0);
    this->lastTickout = 0.0f;
    this->tickOutMode = false;

    // Restore window, docking and toolbars states
    QSettings settings;
    this->restoreState(settings.value("mainWindowState").toByteArray());
    this->restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    this->openSceneDir = settings.value("openSceneDir").toString();
    this->saveSceneDir = settings.value("saveSceneDir").toString();
    this->importSceneDir = settings.value("importSceneDir").toString();
    this->importImageDir = settings.value("importImageDir").toString();
    this->exportImageDir = settings.value("exportImageDir").toString();
    this->importSoundDir = settings.value("importSoundDir").toString();
    this->selectedGraphicsDevice = settings.value("selectedGraphicsDevice", QVariant(QString("Direct3D 11"))).toString();
    this->selectedAudioDevice = settings.value("selectedAudioDevice").toString();
    this->recentlyOpenedScenes = settings.value("recentlyOpenedScenes").toStringList();
    this->ui->wRender->fixedYUp = settings.value("fixedYUp", QVariant(true)).toBool();
    this->ui->wRender->smoothManipulation = settings.value("smoothManipulation", QVariant(true)).toBool();

    this->ui->actionProperties->setChecked(!this->ui->dwProperties->isHidden());
    this->ui->actionResources->setChecked(!this->ui->dwResources->isHidden());
    this->ui->actionStructure->setChecked(!this->ui->dwStructure->isHidden());

    this->ui->bYUp->setChecked(this->ui->wRender->fixedYUp);
    this->ui->bSmooth->setChecked(this->ui->wRender->smoothManipulation);

    // Initialize Axis Engine
    this->renderWindowHandle = this->ui->wRender->winId();
    this->engine = new AxEditorEngine((void*)&this->renderWindowHandle);

#ifdef QT_DEBUG
    this->engine->LoadModules(AxFileSystem::MergePaths(AxPlatformUtils::GetApplicationPath(), AxString("../../../Engine/Debug")));
#else
// Release is meant to be built and ran from the deployed folder. To run from Qt, with the modules loaded, uncomment the line below
//    this->engine->LoadModules(AxFileSystem::MergePaths(AxPlatformUtils::GetApplicationPath(), AxString("../../../Engine/Release")));
#endif

    int selectedGraphicsDeviceIndex = ((Axis*)this->engine)->SetGraphicsDevice(EditorUtils::QToAxString(this->selectedGraphicsDevice));
    int selectedAudioDeviceIndex = this->engine->SetAudioDevice(EditorUtils::QToAxString(this->selectedAudioDevice));

    // Create a timer for continuous rendering
    this->renderTimer = new QTimer(this);
    QObject::connect(this->renderTimer, SIGNAL(timeout()), this, SLOT(Render()));
    this->renderTimer->start(20);

    this->ui->wProperties->SetEditorInterface(this);
    this->ui->wResources->SetEditorInterface(this);
    this->ui->wRender->SetEditorInterface(this);
    this->ui->twStructure->SetEditorInterface(this);
    this->ui->wMaterialProperties->SetEditorInterface(this);
    this->ui->wTransformProperties->SetEditorInterface(this);
    this->ui->wScriptProperties->SetEditorInterface(this);
    this->ui->wScriptCode->SetEditorInterface(this);

    this->ui->wResources->SetItemSource(this->engine->resources);

    this->importSceneDialogFilter = this->CreateDialogFilesFilter(&this->engine->sceneMediaDispatchers, AxModuleDispatcher::infoId_MediaDeserializationFormatExtension, true);
    this->exportSceneDialogFilter = this->CreateDialogFilesFilter(&this->engine->sceneMediaDispatchers, AxModuleDispatcher::infoId_MediaSerializationFormatExtension, false);
    this->importImageDialogFilter = this->CreateDialogFilesFilter(&this->engine->imageMediaDispatchers, AxModuleDispatcher::infoId_MediaDeserializationFormatExtension, true);
    this->exportImageDialogFilter = this->CreateDialogFilesFilter(&this->engine->imageMediaDispatchers, AxModuleDispatcher::infoId_MediaSerializationFormatExtension, true);
    this->importSoundDialogFilter = this->CreateDialogFilesFilter(&this->engine->soundMediaDispatchers, AxModuleDispatcher::infoId_MediaDeserializationFormatExtension, true);

    this->CreateDevicesMenu(&this->engine->graphicsDeviceDispatchers, this->ui->menuGraphics_Device, SLOT(GraphicsDeviceMenuItemClicked()), selectedGraphicsDeviceIndex);
    this->CreateDevicesMenu(&this->engine->audioDeviceDispatchers, this->ui->menuAudio_Device, SLOT(AudioDeviceMenuItemClicked()), selectedAudioDeviceIndex);

    this->UpdateRecentlyOpenedScenesMenu();

    QObject::connect(this->ui->actionResources, SIGNAL(triggered()), this, SLOT(OnWindowVisibilityClicked()));
    QObject::connect(this->ui->actionProperties, SIGNAL(triggered()), this, SLOT(OnWindowVisibilityClicked()));
    QObject::connect(this->ui->actionStructure, SIGNAL(triggered()), this, SLOT(OnWindowVisibilityClicked()));

    QObject::connect(this->ui->bCamera, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bTransform, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bMesh, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bMaterial, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bTexture, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bMechanism, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bLight, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bScript, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bSound, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bSoundEmitter, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));
    QObject::connect(this->ui->bSoundReceptor, SIGNAL(clicked()), this, SLOT(ResourceFilterButtonClicked()));

    this->selectedResource = 0;
    this->SelectTransform(0);

    while (this->ui->tabWidget->count() > 1)
        this->ui->tabWidget->removeTab(1);

    this->settingsDialog = new ResourcePropertiesDialog(this, this, this->engine->settings);

    this->QueryFileTypesAssociation();
}

MainWindow::~MainWindow()
{
    this->renderTimer->stop();

    delete this->engine;

    delete this->settingsDialog;

    delete this->ui;
}

void MainWindow::QueryFileTypesAssociation(bool forceQuery)
{
    const QString settingsKey_askForFileTypeAssociaion = "askForFileTypeAssociation";

    QSettings settings;

    if (!forceQuery)
        if (!settings.value(settingsKey_askForFileTypeAssociaion, QVariant(true)).toBool())
            return;

    AxString appPath = AxFileSystem::GetFileDir(AxPlatformUtils::GetApplicationName());

    bool dontAskAgain = false;
    bool registered = false;
    if (EditorUtils::MessageDialogWithCheckbox(QString("Would you like to register Axis file types and associate them with the editor at its current location at<br/><br/>") + EditorUtils::AxToQString(appPath) + "<br/>", "Don't ask again", dontAskAgain, QMessageBox::Ok, QMessageBox::No) == QMessageBox::Ok)
    {
        registered = EditorUtils::RegisterFileTypes();

        EditorUtils::MessageDialog(registered ? "File types associated." : "An error occurred while creating file types association.", QMessageBox::Ok);
    }

    if (registered || dontAskAgain)
    {
        settings.setValue(settingsKey_askForFileTypeAssociaion, QVariant(false));
    }
}

void MainWindow::OpenScene(QString fileName)
{
    this->engine->Reset();

    //TODO: remove this
    this->ResetEnvironment();

    if (!this->engine->ImportScene(EditorUtils::QToAxString(fileName)))
    {
        EditorUtils::MessageDialog(QString("Could not open file ") + EditorUtils::AxToQString(AxFileSystem::GetFileName(EditorUtils::QToAxString(fileName))), QMessageBox::Ok);
        return;
    }

    this->openSceneDir = QFileInfo(fileName).absoluteFilePath();

    this->SetOpenedSceneName(fileName);

    this->UpdateEnvironment();

    this->PushRecentlyOpenedItemsList(fileName, this->recentlyOpenedScenes, MainWindow::recentlyOpenedScenesMax, false);
    this->UpdateRecentlyOpenedScenesMenu();
}

void MainWindow::SaveScene(QString fileName)
{
    this->engine->ExportScene(AxFileSystem::CreateFileNameWithExtension(EditorUtils::QToAxString(fileName), AxString(AxNativeScene::formatName)));

    // Make a backup copy of the file
    //AxFileStream(AxFileSystem::CreateFileNameWithExtension(EditorUtils::QToAxString(fileName), AxString("axsbu")), FileStreamAccessMode_Write).WriteStreamData(AxFileStream(AxFileSystem::CreateFileNameWithExtension(EditorUtils::QToAxString(fileName).Replace("/", "\\"), AxString(AxNativeScene::formatName)), FileStreamAccessMode_Read));

    this->saveSceneDir = QFileInfo(fileName).absoluteFilePath();

    this->SetOpenedSceneName(fileName);
}

void MainWindow::PushRecentlyOpenedItemsList(QString item, QStringList &list, int max, bool caseSensitive)
{
    int itemIndex;
    for (itemIndex = list.size() - 1; (itemIndex >= 0) && (QString::compare(list[itemIndex], item, caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive) != 0); itemIndex--);

    if (itemIndex >= 0)
    {
        list.move(itemIndex, 0);
    }
    else
    {
        while (list.size() >= max)
            list.removeLast();

        list.insert(0, item);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    // Save window, docking and toolbars states
    QSettings settings;
    settings.setValue("mainWindowGeometry", this->saveGeometry());
    settings.setValue("mainWindowState", this->saveState());
    // Save working dirs
    settings.setValue("openSceneDir", this->openSceneDir);
    settings.setValue("saveSceneDir", this->saveSceneDir);
    settings.setValue("importSceneDir", this->importSceneDir);
    settings.setValue("importImageDir", this->importImageDir);
    settings.setValue("exportImageDir", this->exportImageDir);
    settings.setValue("importSoundDir", this->importSoundDir);
    settings.setValue("selectedGraphicsDevice", this->selectedGraphicsDevice);
    settings.setValue("selectedAudioDevice", this->selectedAudioDevice);
    //
    settings.setValue("recentlyOpenedScenes", this->recentlyOpenedScenes);
    //
    settings.setValue("fixedYUp", this->ui->wRender->fixedYUp);
    settings.setValue("smoothManipulation", this->ui->wRender->smoothManipulation);
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    if (e->isAutoRepeat())
        return;

    //this->keysPressed.Add(e->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent* e)
{
    if (e->isAutoRepeat())
        return;

    //this->keysPressed.RemoveAll(e->key());
}

QString MainWindow::CreateDialogFilesFilter(AxList<AxModuleDispatcher *> *dispatchers, unsigned int infoId, bool addAllSupportedAndAny)
{
    // The filter has the format "Filter name1 (*.ext1 *.extN);;Filter nameN (*.ext1 *.extN)"
    AxStrings allUniqueExtensions;
    AxString result;
    int numDispatchers = dispatchers->count;
    for (int dispatcherIndex = 0; dispatcherIndex < numDispatchers; dispatcherIndex++)
    {
        AxString formatExtension;
        AxStrings formatExtensions;
        for (int formatIndex = 0; dispatchers->Get(dispatcherIndex)->GetInfo(infoId, formatIndex, formatExtension); formatIndex++)
        {
            formatExtension = formatExtension.ToLower();

            formatExtensions.Add(formatExtension);

            if (allUniqueExtensions.IndexOf(formatExtension) == -1)
                allUniqueExtensions.Add(formatExtension);
        }

        if (formatExtensions.count > 0)
        {
            if (result.length != 0)
                result += ";;";

            AxString dispatcherName;
            dispatchers->Get(dispatcherIndex)->GetInfo(AxModuleDispatcher::infoId_Name, 0, dispatcherName);
            result += dispatcherName + " (*." + formatExtensions.ToString(" *.") + ")";
        }
    }

    if (addAllSupportedAndAny)
        if (result.length != 0)
            result = AxString("Supported Files (*.") + allUniqueExtensions.ToString(" *.") + ");;" + result + ";;All Files (*.*)";

    return EditorUtils::AxToQString(result);
}

void MainWindow::UpdateRecentlyOpenedScenesMenu()
{
    this->ui->menuOpen_Recent->clear();

    for (int i = 0; i < this->recentlyOpenedScenes.size(); i++)
    {
        QAction *action = new QAction(this);
        action->setText(this->recentlyOpenedScenes[i]);
        action->setToolTip(this->recentlyOpenedScenes[i]);
        action->setProperty("fileName", this->recentlyOpenedScenes[i]);

        QObject::connect(action, SIGNAL(triggered()), this, SLOT(RecentlyOpenedSceneMenuItemClicked()));

        this->ui->menuOpen_Recent->addAction(action);
    }
}

template <typename Func> void MainWindow::CreateDevicesMenu(AxList<AxModuleDispatcher*> *dispatchers, QMenu *menu, Func slot, int selectedIndex)
{
    QActionGroup *deviceSelectActionGroup = new QActionGroup(this);
    deviceSelectActionGroup->setExclusive(true);
    for (int i = 0; i < dispatchers->count; i++)
    {
        AxString deviceName, deviceDescription;
        dispatchers->Get(i)->GetInfo(AxModuleDispatcher::infoId_Name, 0, deviceName);
        dispatchers->Get(i)->GetInfo(AxModuleDispatcher::infoId_Description, 0, deviceDescription);

        QAction *action = new QAction(this);
        action->setText(EditorUtils::AxToQString(deviceName));
        action->setToolTip(EditorUtils::AxToQString(deviceDescription));
        action->setCheckable(true);
        action->setChecked(i == selectedIndex);
        action->setProperty("dispatcherIndex", QVariant::fromValue(i));

        deviceSelectActionGroup->addAction(action);

        QObject::connect(action, SIGNAL(triggered()), this, slot);

        menu->addAction(action);
    }
}

void MainWindow::SetOpenedSceneName(QString fileName)
{
    bool hasFileName = fileName != 0;

    QString fileNameOnly = QFileInfo(fileName).baseName();

    QString supplement = hasFileName ? " - " + fileName : "";
    this->setWindowTitle("Axis" + supplement);

    supplement = hasFileName ? " \"" + fileNameOnly + "\"" : "";
    this->ui->actionSave->setText("Save" + supplement);
    this->ui->actionSave_As->setText("Save" + supplement + " As...");

    this->openedScene = fileName;
}


void MainWindow::UpdateEnvironment()
{
    this->ui->twStructure->BuildAnew(this->engine->root);
    this->ui->wResources->SetItemSource(this->engine->resources);
}

void MainWindow::ResetEnvironment()
{
    this->SelectTransform(0);
    this->SelectResource((AxResource*)0);
    this->selectionHistory.Clear();

    this->UpdateEnvironment();
}

void MainWindow::SelectResource(AxTraceParameters *resourceParameters)
{
    AxResource *resource = 0;

    if (resourceParameters != 0)
    {
        if (this->ui->bCamera->isChecked())
            resource = resourceParameters->cameraRef;
        else if (this->ui->bTransform->isChecked())
            resource = resourceParameters->transformRef;
        else if (this->ui->bMaterial->isChecked())
            resource = resourceParameters->materialRef;
        else if (this->ui->bMesh->isChecked())
            resource = resourceParameters->meshRef;
        else if (this->ui->bTexture->isChecked())
            resource = resourceParameters->textureRef;
        else if (this->ui->bMechanism->isChecked())
            resource = resourceParameters->mechanismRef;
        else if (this->ui->bLight->isChecked())
            resource = resourceParameters->lightRef;
    //    else if (this->ui->bPath->isChecked())
    //        resource = resourceParameters->pathRef;
//        else if (this->ui->bScript->isChecked())
//            resource = resourceParameters->scriptRef;
    //    else if (this->ui->bSound->isChecked())
    //        resource = resourceParameters->soundRef;
        else if (this->ui->bSoundEmitter->isChecked())
            resource = resourceParameters->soundEmitterRef;
        else if (this->ui->bSoundReceptor->isChecked())
            resource = resourceParameters->soundReceptorRef;
        else
            resource = resourceParameters->materialRef;
    }

    this->SelectResource(resource);
}

void MainWindow::SelectResource(AxResource *resource)
{
    if (resource == this->selectedResource)
        return;

    bool differentResourceTypes = ((resource == 0) != (this->selectedResource == 0)) || ((resource != 0) && (this->selectedResource != 0) && (resource->typeId != this->selectedResource->typeId));

    this->selectedResource = resource;

    this->selectionHistory.PushHistory(this->selectedResource);

    this->ui->wProperties->SetResource(this->selectedResource);

    this->ui->wMaterialProperties->SetResource(this->selectedResource);
    this->ui->wTransformProperties->SetResource(this->selectedResource);
    this->ui->wScriptProperties->SetResource(this->selectedResource);
    this->ui->wScriptCode->SetResource(this->selectedResource);

    if (differentResourceTypes)
    {
        while (this->ui->tabWidget->count() > 1)
            this->ui->tabWidget->removeTab(1);

        if (this->selectedResource != 0)
        {
            QString iconName;
            EditorUtils::resourceTypeIcons.GetValue(this->selectedResource->resourceType, iconName);

            if (this->selectedResource->resourceType == AxResourceType_Material)
                this->ui->tabWidget->addTab(this->ui->tabMaterial, QIcon(iconName), "");
            else if (this->selectedResource->resourceType == AxResourceType_Transform)
                this->ui->tabWidget->addTab(this->ui->tabTransform, QIcon(iconName), "");
            else if (this->selectedResource->resourceType == AxResourceType_Script)
            {
                this->ui->tabWidget->addTab(this->ui->tabScriptProperties, QIcon(EditorUtils::iconScriptProperties), "");
                this->ui->tabWidget->addTab(this->ui->tabScriptCode, QIcon(iconName), "");
            }
        }
    }
}

AxResource *MainWindow::GetSelectedResource()
{
    return this->selectedResource;
}

void MainWindow::SelectTransform(AxTraceParameters *parameters)
{
    this->hasSelectedTransform = parameters != 0;

    if (this->hasSelectedTransform)
        this->selectedTransformParameters = *parameters;
}

AxTraceParameters *MainWindow::GetSelectedTransform()
{
    return this->hasSelectedTransform ? &this->selectedTransformParameters : 0;
}

void MainWindow::UpdateResourceName(AxResource *resource)
{
    AxList<QTreeWidgetItem*> result;
    this->ui->twStructure->FindResourceItems(result, resource);
    for (int i = 0; i < result.count; i++)
    {
        QAxStructuresWidget::SetAxResourceToItem(result[i], resource);
    }

    this->ui->wResources->RefreshItems();
}

void MainWindow::UpdateResourceProperties(AxResource *resource)
{
    if (resource == this->selectedResource)
    {
        this->ui->wProperties->SetResource(this->selectedResource);
    }
}

//bool MainWindow::IsKeyDown(int keyCode)
//{
//    return this->keysPressed.IndexOf(keyCode) != -1;
//}

AxEditorEngine *MainWindow::GetAxis()
{
    return this->engine;
}


void MainWindow::Render()
{
    this->ui->wRender->Render();
}

void MainWindow::on_actionNew_triggered()
{
    this->engine->Reset();

    this->ResetEnvironment();

    this->SetOpenedSceneName("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open scene file"), this->openSceneDir, this->importSceneDialogFilter);
    if (fileName != 0)
        this->OpenScene(fileName);
}

void MainWindow::on_actionSave_triggered()
{
    QString fileName = this->openedScene;

    if (fileName == 0)
        fileName = QFileDialog::getSaveFileName(this, tr("Save scene file"), this->saveSceneDir, this->exportSceneDialogFilter);

    if (fileName != 0)
        this->SaveScene(fileName);
}

void MainWindow::on_actionSave_As_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save scene file"), this->saveSceneDir, this->exportSceneDialogFilter);

    if (fileName != 0)
        this->SaveScene(fileName);
}

void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import scene file"), this->importSceneDir, this->importSceneDialogFilter);
    if (fileName != 0)
    {
        this->engine->ImportScene(AxString(fileName.toStdWString().c_str()));

        this->importSceneDir = QFileInfo(fileName).absoluteFilePath();

        this->UpdateEnvironment();
    }
}


void MainWindow::on_actionFrom_File_Texture_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import image file"), this->importImageDir, this->importImageDialogFilter);
    if (fileName != 0)
    {
        this->engine->AcquireTexture(AxString(fileName.toStdWString().c_str()));

        this->importImageDir = QFileInfo(fileName).absoluteFilePath();

        this->UpdateEnvironment();
    }
}

void MainWindow::on_actionEmpty_Texture_triggered()
{
    this->engine->AddResource(new AxTexture2D(this->engine))->name = this->engine->AcquireResourceName(AxString("Empty texture"), AxResourceType_Texture);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionFrom_File_Sound_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Import sound file"), this->importSoundDir, this->importSoundDialogFilter);
    if (fileName != 0)
    {
        this->engine->AcquireSound(AxString(fileName.toStdWString().c_str()));

        this->importSoundDir = QFileInfo(fileName).absoluteFilePath();

        this->UpdateEnvironment();
    }
}

void MainWindow::on_actionEmpty_Sound_triggered()
{
    this->engine->AddResource(new AxSound(this->engine))->name = this->engine->AcquireResourceName(AxString("Sound"), AxResourceType_Sound);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Material_triggered()
{
    this->engine->AddResource(new AxMaterial(this->engine))->name = this->engine->AcquireResourceName(AxString("Material"), AxResourceType_Material);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Sound_Emitter_triggered()
{
    this->engine->AddResource(new AxSoundEmitter(this->engine))->name = this->engine->AcquireResourceName(AxString("Sound emitter"), AxResourceType_SoundEmitter);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Sound_Receptor_triggered()
{
    this->engine->AddResource(new AxSoundReceptor(this->engine))->name = this->engine->AcquireResourceName(AxString("Sound receptor"), AxResourceType_SoundReceptor);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Light_triggered()
{
    this->engine->AddResource(new AxLight())->name = this->engine->AcquireResourceName(AxString("Light"), AxResourceType_Light);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Camera_triggered()
{
    this->engine->AddResource(new AxCamera(this->engine))->name = this->engine->AcquireResourceName(AxString("Camera"), AxResourceType_Camera);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Transform_triggered()
{
    this->engine->AddResource(new AxTransform())->name = this->engine->AcquireResourceName(AxString("Transform"), AxResourceType_Transform);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCreate_Script_triggered()
{
    this->engine->AddResource(new AxScript(this->engine))->name = this->engine->AcquireResourceName(AxString("Script"), AxResourceType_Script);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionScreen_Plane_triggered()
{
    AxMesh *mesh = (AxMesh*)this->engine->AddResource(new AxMesh(this->engine));
    mesh->name = this->engine->AcquireResourceName(AxString("Screen plane"), AxResourceType_Mesh);
    this->ui->wResources->RefreshItems();

    AxPlaneGeometry(*mesh->deviceMesh, 1, 1, AxVector3(2.0f, 0.0f, 0.0f), AxVector3(0.0f, 2.0f, 0.0f));

    mesh->deviceMesh->UpdateVertices(0, mesh->deviceMesh->GetVertexCount());
    mesh->deviceMesh->UpdateIndices(0, mesh->deviceMesh->GetIndexCount());
}

// Creation of new Mechanisms >>
void MainWindow::on_actionLinear_triggered()
{
    this->engine->AddResource(new AxLinearMechanism(0.0f, 1.0f, 1.0f, false, false, false))->name = this->engine->AcquireResourceName(AxString("Linear"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionScalar_triggered()
{
    this->engine->AddResource(new AxFloatValueMechanism(0.0f))->name = this->engine->AcquireResourceName(AxString("Scalar"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}


void MainWindow::on_actionPolynomial_triggered()
{
    this->engine->AddResource(new AxPolynomialMechanism(3))->name = this->engine->AcquireResourceName(AxString("Polynomial"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionLinear_interpolation_triggered()
{
    this->engine->AddResource(new AxLinearInterpolationMechanism(0.0f, 1.0f, 0.0f, 1.0f, false, false))->name = this->engine->AcquireResourceName(AxString("Linear interpolation"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionSine_triggered()
{
    this->engine->AddResource(new AxSineMechanism(0.0f, 1.0f, 0.0f, 1.0f))->name = this->engine->AcquireResourceName(AxString("Sine"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionPivotal_triggered()
{
    this->engine->AddResource(new AxPivotalMechanism())->name = this->engine->AcquireResourceName(AxString("Pivotal"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionOrientation_triggered()
{
    this->engine->AddResource(new AxOrientationMechanism())->name = this->engine->AcquireResourceName(AxString("Orientation"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionTime_rate_triggered()
{
    this->engine->AddResource(new AxTimeRateMechanism(this->engine))->name = this->engine->AcquireResourceName(AxString("Time rate"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}
// Creation of new Mechanisms <<

void MainWindow::on_bBack_clicked()
{
    AxResource *previous = this->selectionHistory.Back();
    if (previous != 0)
        this->SelectResource(previous);
}

void MainWindow::on_bForward_clicked()
{
    AxResource *next = this->selectionHistory.Forward();
    if (next != 0)
        this->SelectResource(next);
}

void MainWindow::on_eFilterResource_textChanged(const QString &arg1)
{
    this->ui->wResources->SetFilterString(EditorUtils::QToAxString(arg1));
}

void MainWindow::RecentlyOpenedSceneMenuItemClicked()
{
    QAction *sender = (QAction*)QObject::sender();

    QString fileName = sender->property("fileName").toString();

    this->OpenScene(fileName);
}

void MainWindow::GraphicsDeviceMenuItemClicked()
{
    QAction *sender = (QAction*)QObject::sender();

    //if (!sender->isChecked()) // sender is already checked when this method is called
    {
        int dispatcherIndex = sender->property("dispatcherIndex").toInt();

        this->engine->SetGraphicsDevice((AxGraphicsDevice*)this->engine->graphicsDeviceDispatchers[dispatcherIndex]->CreateObject());
        this->ResetEnvironment();

        AxString deviceName;
        this->engine->graphicsDeviceDispatchers[dispatcherIndex]->GetInfo(AxModuleDispatcher::infoId_Name, 0, deviceName);
        this->selectedGraphicsDevice = EditorUtils::AxToQString(deviceName);
    }
}

void MainWindow::AudioDeviceMenuItemClicked()
{
    QAction *sender = (QAction*)QObject::sender();

    //if (!sender->isChecked()) // sender is already checked when this method is called
    {
        int dispatcherIndex = sender->property("dispatcherIndex").toInt();

        this->engine->SetAudioDevice((AxAudioDevice*)this->engine->audioDeviceDispatchers[dispatcherIndex]->CreateObject());
        this->ResetEnvironment();

        AxString deviceName;
        this->engine->audioDeviceDispatchers[dispatcherIndex]->GetInfo(AxModuleDispatcher::infoId_Name, 0, deviceName);
        this->selectedAudioDevice = EditorUtils::AxToQString(deviceName);
    }
}

void MainWindow::ResourceFilterButtonClicked()
{
    QPushButton *sender = (QPushButton*)QObject::sender();
    AxResourceType filterType = (AxResourceType)sender->property("AxResourceType").toInt();

    if (this->ui->wResources->filterType == filterType)
    {
        sender->setCheckable(false);
        sender->setChecked(false);
        sender->setCheckable(true);
        sender->update();

        this->ui->wResources->SetFilterType((AxResourceType)-1);
    }
    else
        this->ui->wResources->SetFilterType(filterType);
}

void MainWindow::OnWindowVisibilityClicked()
{
    QAction *sender = (QAction*)QObject::sender();
    QString windowName = sender->property("windowName").toString();

    QWidget *window = this->findChild<QWidget*>(windowName);
    window->setVisible(sender->isChecked());
}

void MainWindow::on_bPlayPause_clicked()
{
    this->paused = !this->paused;
    if (this->paused)
    {
        this->engine->timer->speed = 0.0f;
        this->ui->bPlayPause->setIcon(QIcon(EditorUtils::iconPlay));
    }
    else
    {
        this->engine->timer->speed = (float)this->ui->sbPlaySpeed->value();
        this->ui->bPlayPause->setIcon(QIcon(EditorUtils::iconPause));
    }
}

void MainWindow::on_sbPlaySpeed_valueChanged(double)
{
    this->ui->sbPlaySpeed->setSingleStep(AxMath::Max((float)(AxMath::Abs((float)this->ui->sbPlaySpeed->value()) / 10.0f), 0.01f));
    if (!this->paused)
        this->engine->timer->speed = this->ui->sbPlaySpeed->value();
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    if (!this->tickOutMode)
        return;

    float x = value / 100.0f;
    //x = x * x * x * 60.0f;
    x *= 10.0f;
    this->engine->timer->TickOut(-this->lastTickout + x);
    this->ui->lTickOutTime->setText(QString::number(x));
    this->lastTickout = x;
}

void MainWindow::on_horizontalSlider_sliderPressed()
{
    this->tickOutMode = true;
    this->engine->timer->speed = 0.0f;
    //this->ui->lTickOutTime->setVisible(true);
}

void MainWindow::on_horizontalSlider_sliderReleased()
{
    this->tickOutMode = false;
    //this->ui->lTickOutTime->setVisible(false);
    this->ui->lTickOutTime->setText("");
    this->ui->horizontalSlider->setValue(0);

    if (!this->paused)
        this->engine->timer->speed = this->ui->sbPlaySpeed->value();
}

void MainWindow::on_actionOnlineDocumentation_triggered()
{
    QDesktopServices::openUrl(QUrl("http://palitri.com/resources/"));
}

void MainWindow::on_actionHome_Page_triggered()
{
    QDesktopServices::openUrl(QUrl("http://palitri.com/axis/"));
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, "Axis", QString("Axis Editor<br/>Version ") + VER_PRODUCTVERSION_STR + "<br/><a href=\"http://palitri.com\">palitri.com</a>");
}

void MainWindow::on_actionExport_triggered()
{
    if (this->selectedResource == 0)
        return;

    if (this->selectedResource->resourceType == AxResourceType_Mesh)
    {
        QString fileName = QFileDialog::getSaveFileName(this, tr("Export"), this->openSceneDir, this->importSceneDialogFilter);
        if (fileName != 0)
        {
            AxMesh *mesh = (AxMesh*)this->selectedResource;

            AxFileStream file(EditorUtils::QToAxString(fileName), FileStreamAccessMode_Write);
            mesh->Serialize(file);
        }
    }
}

void MainWindow::on_actionComponents_triggered()
{
    ComponentsDialog *dialog = new ComponentsDialog(this, this->engine);
    dialog->exec();

    delete dialog;
}

void MainWindow::on_actionNormal_map_generator_triggered()
{
    QString inputFileName = QFileDialog::getOpenFileName(this, tr("Open original image file"), this->importImageDir, this->importImageDialogFilter);
    if (inputFileName == 0)
        return;

    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save normal map file"), this->exportImageDir, this->exportImageDialogFilter);
    if (outputFileName == 0)
        return;

    AxImage *originalImage = this->engine->LoadImageX(AxString(inputFileName.toStdWString().c_str()));
    AxImage *normalMapImage = new AxImage();

    AxGenerateNormalMapImage(*normalMapImage, *originalImage, AxVector4(1.0f, 1.0f, 1.0f, 0.0f));

    this->engine->SaveImage(normalMapImage, AxString(outputFileName.toStdWString().c_str()));

    delete originalImage;
    delete normalMapImage;


    this->importImageDir = QFileInfo(inputFileName).absoluteFilePath();
    this->exportImageDir = QFileInfo(outputFileName).absoluteFilePath();


    if (EditorUtils::MessageDialog("Open the newly created normal map image?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    this->engine->AcquireTexture(AxString(outputFileName.toStdWString().c_str()));

    this->UpdateEnvironment();
}


void MainWindow::on_actionSave_Screenshot_triggered()
{
    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save screenshot"), this->exportImageDir, this->exportImageDialogFilter);
    if (outputFileName == 0)
        return;


    AxImage *screenshot = this->engine->TakeScreenshot(this->engine->viewportWidth, this->engine->viewportHeight);
    this->engine->SaveImage(screenshot, AxString(outputFileName.toStdWString().c_str()));
    delete screenshot;

    this->exportImageDir = QFileInfo(outputFileName).absoluteFilePath();
}

void MainWindow::on_bDeleteResource_clicked()
{
    if (this->selectedResource == 0)
        return;

    if (EditorUtils::MessageDialog("Delete \"" + EditorUtils::AxToQString(this->selectedResource->name) + "\"?", QMessageBox::Yes, QMessageBox::No) != QMessageBox::Yes)
        return;

    this->engine->RemoveResource(this->selectedResource);

    this->UpdateEnvironment();
}

void MainWindow::on_bSelect_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_Select);
}

void MainWindow::on_bLink_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_Link);
}

void MainWindow::on_bExtrinsic_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_TransformExtrinsic);
}

void MainWindow::on_bIntrinsic_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_TransformIntrinsic);
}

void MainWindow::on_bCameraTranslate_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_TransformCameraTranslate);
}

void MainWindow::on_bCameraOrbit_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_TransformCameraOrbit);
}

void MainWindow::on_bCameraRotate_clicked()
{
    this->ui->wRender->SetMode(QAxRenderWidget::mode_TransformCameraRotate);
}

void MainWindow::on_bYUp_clicked()
{
    this->ui->wRender->fixedYUp = this->ui->bYUp->isChecked();
}

void MainWindow::on_bSmooth_clicked()
{
    this->ui->wRender->smoothManipulation = this->ui->bSmooth->isChecked();
}

void MainWindow::on_editTransformationX_editingFinished()
{
    this->UpdateSelectedTransformXYZ();
}

void MainWindow::on_editTransformationY_editingFinished()
{
    this->UpdateSelectedTransformXYZ();
}

void MainWindow::on_editTransformationZ_editingFinished()
{
    this->UpdateSelectedTransformXYZ();
}

void MainWindow::OnTransformationChanged(int operation, AxVector3 transformationParameters)
{
    float editRange;
    int editDecimals;

    switch (operation)
    {
        case AxEditorEngine::visualControlsTransformOperation_Translation:
        {
            this->ui->lblTransformationTypeIcon->setPixmap(QPixmap(":/Icons/Icons/Manipulate/Translate.png"));

            editRange = 99999.0f;
            editDecimals = 2;

            break;
        }

        case AxEditorEngine::visualControlsTransformOperation_Rotation:
        {
            this->ui->lblTransformationTypeIcon->setPixmap(QPixmap(":/Icons/Icons/Manipulate/Rotate.png"));

            editRange = 9.0f;
            editDecimals = 6;

            AxVector3::Scale(transformationParameters, transformationParameters, 1.0f / AxMath::Pi2);

            break;
        }

        case AxEditorEngine::visualControlsTransformOperation_Scaling:
        {
            this->ui->lblTransformationTypeIcon->setPixmap(QPixmap(":/Icons/Icons/Manipulate/Scale.png"));

            editRange = 999.0f;
            editDecimals = 4;

            break;
        }

        default:
        {
            this->ui->lblTransformationTypeIcon->setPixmap(QPixmap());
            break;
        }
    }

    this->ui->editTransformationX->setRange(-editRange, editRange);
    this->ui->editTransformationX->setDecimals(editDecimals);
    this->ui->editTransformationY->setRange(-editRange, editRange);
    this->ui->editTransformationY->setDecimals(editDecimals);
    this->ui->editTransformationZ->setRange(-editRange, editRange);
    this->ui->editTransformationZ->setDecimals(editDecimals);

    this->ui->editTransformationX->setValue(transformationParameters.x);
    this->ui->editTransformationY->setValue(transformationParameters.y);
    this->ui->editTransformationZ->setValue(transformationParameters.z);

    this->ui->editTransformationX->setEnabled(operation != AxEditorEngine::visualControlsTransformOperation_None);
    this->ui->editTransformationY->setEnabled(operation != AxEditorEngine::visualControlsTransformOperation_None);
    this->ui->editTransformationZ->setEnabled(operation != AxEditorEngine::visualControlsTransformOperation_None);
}

void MainWindow::UpdateSelectedTransformXYZ()
{
    AxVector3 transformationParameters((float)this->ui->editTransformationX->value(), (float)this->ui->editTransformationY->value(), (float)this->ui->editTransformationZ->value());

    if (this->engine->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Rotation)
        AxVector3::Scale(transformationParameters, transformationParameters, AxMath::Pi2);

    this->ui->wRender->UpdateTransformFromEditor(transformationParameters);
}

void MainWindow::on_dwProperties_visibilityChanged(bool visible)
{
    this->ui->actionProperties->setChecked(visible);
}

void MainWindow::on_dwResources_visibilityChanged(bool visible)
{
    this->ui->actionResources->setChecked(visible);
}

void MainWindow::on_dwStructure_visibilityChanged(bool visible)
{
    this->ui->actionStructure->setChecked(visible);
}

void MainWindow::on_actionEmpty_Mesh_triggered()
{
    AxMesh *mesh = (AxMesh*)this->engine->AddResource(new AxMesh(this->engine));
    mesh->name = this->engine->AcquireResourceName(AxString("Empty mesh"), AxResourceType_Mesh);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionPlane_triggered()
{
    this->engine->AddResource(new AxPlaneGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Plane geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionBox_triggered()
{
    this->engine->AddResource(new AxBoxGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Box geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionCone_triggered()
{
    this->engine->AddResource(new AxConeGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Cone geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionTorus_triggered()
{
    this->engine->AddResource(new AxTorusGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Torus geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionHeight_map_triggered()
{
    this->engine->AddResource(new AxHeightmapGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Height map geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionGeosphere_triggered()
{
    this->engine->AddResource(new AxGeosphereGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Geosphere geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionHue_map_triggered()
{
    this->engine->AddResource(new AxHueMapTextureMechanism())->name = this->engine->AcquireResourceName(AxString("Hue map texture"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionNormal_map_from_Height_map_triggered()
{
    this->engine->AddResource(new AxNormalMapTextureMechanism())->name = this->engine->AcquireResourceName(AxString("Normal map texture"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionPlasma_Fractal_triggered()
{
    this->engine->AddResource(new AxPlasmaFractalTextureMechanism())->name = this->engine->AcquireResourceName(AxString("Plasma fractal texture"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionBlur_triggered()
{
    this->engine->AddResource(new AxBlurTextureMechanism())->name = this->engine->AcquireResourceName(AxString("Blur texture"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionChecker_pattern_triggered()
{
    this->engine->AddResource(new AxCheckerTextureMechanism())->name = this->engine->AcquireResourceName(AxString("Checker texture"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionSphere_triggered()
{
    this->engine->AddResource(new AxSphereGeometryMechanism())->name = this->engine->AcquireResourceName(AxString("Sphere geometry"), AxResourceType_Mechanism);
    this->ui->wResources->RefreshItems();
}

void MainWindow::on_actionSave_selected_texture_triggered()
{
    AxResource *selectedResource = this->GetSelectedResource();
    if (selectedResource == 0)
        return;

    if (selectedResource->resourceType != AxResourceType_Texture)
        return;


    QString outputFileName = QFileDialog::getSaveFileName(this, tr("Save normal map file"), this->exportImageDir, this->exportImageDialogFilter);
    if (outputFileName == 0)
        return;

//    AxImage *image = this->engine->LoadImageX(EditorUtils::QToAxString(outputFileName));
//    this->engine->SaveImage(image, EditorUtils::QToAxString(outputFileName + ".jpg"));
//    return;

    AxTexture2D *selectedTexture = (AxTexture2D*)selectedResource;
    AxImage *selectedTextureImage = selectedTexture->ToImage();

    this->GetAxis()->SaveImage(selectedTextureImage, EditorUtils::QToAxString(outputFileName));

    delete selectedTextureImage;
}

void MainWindow::on_actionRegister_file_types_triggered()
{
    this->QueryFileTypesAssociation(true);
}

void MainWindow::on_actionProperties_2_triggered()
{
    this->settingsDialog->SetResource(this->engine->settings);
    this->settingsDialog->show();
}


void MainWindow::on_actionCompute_selected_mesh_normals_triggered()
{
    AxResource *selectedResource = this->GetSelectedResource();
    if (selectedResource == 0)
        return;

    if (selectedResource->resourceType != AxResourceType_Mesh)
        return;

    AxMesh* selectedMesh = (AxMesh*)selectedResource;

    selectedMesh->ComputeNormals();

}
