#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "AxisEngine.h"

#include <QMainWindow>

#include <QTimer>
#include <QTreeWidgetItem>

#include "EditorTools/ResourceSelectionHistory.h"
#include "QAxWidgets/IAxEditor.h"

#include "Engine/AxEditorEngine.h"

#include "Dialogs/resourcepropertiesdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow :
        public QMainWindow,
        public IAxEditor
{
    Q_OBJECT

private:
    Ui::MainWindow *ui;


    //AxList<int> keysPressed;

    bool paused;
    float lastTickout;
    bool tickOutMode;
    WId renderWindowHandle;

    ResourcePropertiesDialog *settingsDialog;

    static const int recentlyOpenedScenesMax = 7;

    QString importSceneDialogFilter, exportSceneDialogFilter, importImageDialogFilter, exportImageDialogFilter, importSoundDialogFilter;
    QString openSceneDir, saveSceneDir, importSceneDir, importImageDir, exportImageDir, importSoundDir;
    QString selectedGraphicsDevice, selectedAudioDevice;
    QStringList recentlyOpenedScenes;

    QString openedScene;

    QString CreateDialogFilesFilter(AxList<AxModuleDispatcher *> *dispatchers, unsigned int infoId, bool addAllSupportedAndAny);

    void QueryFileTypesAssociation(bool forceQuery = false);

    void UpdateRecentlyOpenedScenesMenu();
    template <typename Func> void CreateDevicesMenu(AxList<AxModuleDispatcher *> *dispatchers, QMenu *menu, Func slot, int selectedIndex);
    void SetOpenedSceneName(QString fileName);

    void PushRecentlyOpenedItemsList(QString item, QStringList &list, int max, bool caseSensitive);

    void UpdateSelectedTransformXYZ();

    // Events
    void closeEvent(QCloseEvent *event);
    void keyPressEvent(QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

public:
    AxEditorEngine *engine;

    AxResource *selectedResource;
    AxTraceParameters selectedTransformParameters;
    bool hasSelectedTransform;

    ResourceSelectionHistory selectionHistory;

    QTimer *renderTimer;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    void OpenScene(QString fileName);
    void SaveScene(QString fileName);

    // IAxEditor interface methods

    void UpdateEnvironment();
    void ResetEnvironment();

    void SelectResource(AxTraceParameters *resourceParameters);
    void SelectResource(AxResource *resource);
    AxResource *GetSelectedResource();

    void SelectTransform(AxTraceParameters *parameters);
    AxTraceParameters *GetSelectedTransform();


    //bool IsKeyDown(int keyCode);

    AxEditorEngine *GetAxis();

    void UpdateResourceName(AxResource *resource);
    void UpdateResourceProperties(AxResource *resource);

    void OnTransformationChanged(int operation, AxVector3 transformationParameters);


private slots:
    void Render();

    void RecentlyOpenedSceneMenuItemClicked();

    void GraphicsDeviceMenuItemClicked();
    void AudioDeviceMenuItemClicked();

    void ResourceFilterButtonClicked();

    void OnWindowVisibilityClicked();

    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();

    void on_actionImport_triggered();

    void on_bBack_clicked();
    void on_bForward_clicked();

    void on_eFilterResource_textChanged(const QString &arg1);
    void on_bPlayPause_clicked();
    void on_sbPlaySpeed_valueChanged(double);
    void on_horizontalSlider_valueChanged(int value);
    void on_horizontalSlider_sliderPressed();
    void on_horizontalSlider_sliderReleased();

    void on_actionEmpty_Texture_triggered();
    void on_actionFrom_File_Texture_triggered();

    void on_actionEmpty_Sound_triggered();
    void on_actionFrom_File_Sound_triggered();

    void on_actionCreate_Material_triggered();
    void on_actionCreate_Sound_Emitter_triggered();
    void on_actionCreate_Sound_Receptor_triggered();
    void on_actionCreate_Light_triggered();
    void on_actionCreate_Camera_triggered();
    void on_actionCreate_Transform_triggered();
    void on_actionScreen_Plane_triggered();

    void on_actionLinear_triggered();
    void on_actionScalar_triggered();
    void on_actionPolynomial_triggered();
    void on_actionLinear_interpolation_triggered();
    void on_actionSine_triggered();
    void on_actionPivotal_triggered();
    void on_actionOrientation_triggered();
    void on_actionTime_rate_triggered();
    void on_actionCreate_Script_triggered();
    void on_actionPlane_triggered();
    void on_actionOnlineDocumentation_triggered();
    void on_actionHome_Page_triggered();
    void on_actionAbout_triggered();
    void on_actionExport_triggered();
    void on_actionComponents_triggered();
    void on_actionNormal_map_generator_triggered();
    void on_actionSave_Screenshot_triggered();
    void on_bDeleteResource_clicked();
    void on_bExtrinsic_clicked();
    void on_bIntrinsic_clicked();
    void on_editTransformationX_editingFinished();
    void on_editTransformationY_editingFinished();
    void on_editTransformationZ_editingFinished();
    void on_bSelect_clicked();
    void on_bLink_clicked();
    void on_bCameraTranslate_clicked();
    void on_bCameraOrbit_clicked();
    void on_bCameraRotate_clicked();
    void on_bYUp_clicked();
    void on_bSmooth_clicked();
    void on_dwProperties_visibilityChanged(bool visible);
    void on_dwResources_visibilityChanged(bool visible);
    void on_dwStructure_visibilityChanged(bool visible);
    void on_actionHeight_map_triggered();
    void on_actionEmpty_Mesh_triggered();
    void on_actionCone_triggered();
    void on_actionBox_triggered();
    void on_actionTorus_triggered();
    void on_actionGeosphere_triggered();
    void on_actionHue_map_triggered();
    void on_actionPlasma_Fractal_triggered();
    void on_actionSphere_triggered();
    void on_actionNormal_map_from_Height_map_triggered();
    void on_actionSave_selected_texture_triggered();
    void on_actionRegister_file_types_triggered();
    void on_actionBlur_triggered();
    void on_actionProperties_2_triggered();
    void on_actionChecker_pattern_triggered();
    void on_actionCompute_selected_mesh_normals_triggered();
};

#endif // MAINWINDOW_H
