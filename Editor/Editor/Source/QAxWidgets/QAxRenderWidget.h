#ifndef QMWRENDERWIDGET_H
#define QMWRENDERWIDGET_H

#include <QWidget>

#include "QAxEditorWidget.h"

#include <QMouseEvent>
#include <QResizeEvent>

#include "Source/AxisEngine.h"

class QAxRenderWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    static const int DragMode_None = 0;
    static const int DragMode_MouseDown = 1;
    static const int DragMode_Resource = 4;
    static const int DragMode_Transforming = 5;
    static const int DragMode_Link = 6;
    static const int DragMode_MoveCamera = 7;

    AxVector2 mouseDownPos, mousePos, mouseSmoothPos, mouseDelta, mouseSmoothDelta, mouseOldPos, transformDeltaPos;
    AxVector2 mouseScrollDegrees, mouseScrollSmoothDegrees, mouseScrollDelta, mouseScrollSmoothDelta;
    int mouseButtons, mouseOldButtons;

    AxVector3 moveVector, moveSmoothVector, moveDelta, moveSmoothDelta;

    int dragMode;

    AxIntersectionInfo mouseDownIntersectionInfo;
    AxTraceParameters mouseDownTraceParameters;

    AxVector3 translationAxes, rotationAxes, scalingAxes;

    //float moveSpeed, maxMoveSpeed;

    AxList<int> keysPressed;

    Qt::DropAction dropAction;

    void MouseRoutines();
    void KeyRoutines();

    bool IsKeyDown(int keyCode);

    void UpdateTransformInfoInEditor();

    bool SelectVisualControls(AxVector2 mousePosition, bool autoSetVisualControlsTransformOperation);
    bool PickScreenObject(AxVector2 mousePosition);
    bool SelectPickedObject();

    void focusOutEvent(QFocusEvent* event);

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void mouseReleaseEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);

    void wheelEvent(QWheelEvent* event);

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dropEvent(QDropEvent *event);

public:
    static const int mode_None                      = 0;
    static const int mode_Select                    = 1;
    static const int mode_Link                      = 2;
    static const int mode_TransformExtrinsic        = 3;
    static const int mode_TransformIntrinsic        = 4;
    static const int mode_TransformCameraTranslate  = 5;
    static const int mode_TransformCameraOrbit      = 6;
    static const int mode_TransformCameraRotate     = 7;

    explicit QAxRenderWidget(QWidget *parent = 0);

    int mode;
    bool fixedYUp, smoothManipulation;

    void Render();

    void SetMode(int mode);
    void UpdateTransformFromEditor(AxVector3 transformXYZ);


signals:

public slots:

};

#endif // QMWRENDERWIDGET_H
