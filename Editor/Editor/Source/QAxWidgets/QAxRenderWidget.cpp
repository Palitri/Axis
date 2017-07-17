//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxRenderWidget.h"

#include <QApplication>

#include <QDrag>
#include <QMimeData>

#include <QMessageBox>

#include "../EditorTools/EditorUtils.h"

QAxRenderWidget::QAxRenderWidget(QWidget *parent) :
    QWidget(parent)
{
    this->setMouseTracking(true);

    this->mode = QAxRenderWidget::mode_None;

    this->dragMode = QAxRenderWidget::DragMode_None;
    this->mouseButtons = 0;

    this->mouseScrollDegrees.Set(0.0f);

    this->translationAxes.Set(0);
    this->rotationAxes.Set(0);

    //this->maxMoveSpeed = 5.0f;

    //this->setFocusPolicy(Qt::ClickFocus); // Set by designer
    // this->setAttribute(Qt::WA_PaintOnScreen, true); // According to documentation, an appropriate flag for a real time rendering widget, but causes a warning message (QWidget::paintEngine: Should no longer be called)
    this->setUpdatesEnabled(false);
}

void QAxRenderWidget::focusOutEvent(QFocusEvent* event)
{
    this->keysPressed.Clear();
    this->mouseButtons = 0;

    QWidget::focusOutEvent(event);
}

void QAxRenderWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    this->keysPressed.Add(event->key());
}

void QAxRenderWidget::keyReleaseEvent(QKeyEvent* event)
{
    if (event->isAutoRepeat())
        return;

    this->keysPressed.RemoveAll(event->key());
}

void QAxRenderWidget::mousePressEvent(QMouseEvent * event)
{
    QWidget::mousePressEvent(event);

    this->mouseButtons |= event->button();
}

void QAxRenderWidget::mouseMoveEvent(QMouseEvent * event)
{
    QWidget::mouseMoveEvent(event);

    this->mousePos = this->editor->GetAxis()->ScreenCoordsPixelToUnit(AxVector2(event->pos().x(), event->pos().y()));
}

void QAxRenderWidget::mouseReleaseEvent(QMouseEvent * event)
{
    QWidget::mouseReleaseEvent(event);

    this->mouseButtons &= ~event->button();
}

void QAxRenderWidget::wheelEvent(QWheelEvent* event)
{
    QWidget::wheelEvent(event);

    this->mouseScrollDelta = AxVector2(event->angleDelta().x(), event->angleDelta().y()).Scale(AxMath::Pi2 / (24.0f * 120.0f));
    AxVector2::Add(this->mouseScrollDegrees, this->mouseScrollDegrees, this->mouseScrollDelta);
}

void QAxRenderWidget::MouseRoutines()
{
    int mouseDeltaButtons = this->mouseButtons ^ this->mouseOldButtons;
    this->mouseOldButtons = this->mouseButtons;

    // Reset smoothPos when a button is first clicked, so that no residual smooth motion is applied when mouse is just clicked
    if (this->mouseButtons == mouseDeltaButtons)
        this->mouseSmoothPos = this->mousePos;

    this->mouseDelta = this->mousePos.Subtract(this->mouseOldPos);
    this->mouseOldPos = this->mousePos;

    this->mouseSmoothDelta = this->mousePos.Subtract(this->mouseSmoothPos).Scale(10.0f * 0.95f * this->editor->GetAxis()->timer->actualTime);
    AxVector2::Add(this->mouseSmoothPos, this->mouseSmoothPos, this->mouseSmoothDelta);

    AxVector2 mouseDelta = this->smoothManipulation ? this->mouseSmoothDelta : this->mouseDelta;
    AxVector2 mousePos = this->smoothManipulation ? this->mouseSmoothPos : this->mousePos;


    this->mouseScrollSmoothDelta = this->mouseScrollDegrees.Subtract(this->mouseScrollSmoothDegrees).Scale(10.0f * 0.95f * this->editor->GetAxis()->timer->actualTime);
    AxVector2::Add(this->mouseScrollSmoothDegrees, this->mouseScrollSmoothDegrees, this->mouseScrollSmoothDelta);

    AxVector2 mouseScrollDelta = this->smoothManipulation ? this->mouseScrollSmoothDelta: this->mouseScrollDelta;


    if (mouseScrollDelta.GetLength() != 0.0f)
    {
        AxTransform *cameraTransform = this->editor->GetAxis()->camera.cameraRef != 0 ? this->editor->GetAxis()->camera.transformRef : 0;
        if (cameraTransform != 0)
        {
            cameraTransform->TranslateIntrinsic(AxVector3(0.0f, 0.0f, mouseScrollDelta.y * 250.0f));
        }
    }



    switch (this->dragMode)
    {
        case QAxRenderWidget::DragMode_None:
        {
        if ((this->mode == QAxRenderWidget::mode_TransformExtrinsic) ||
            (this->mode == QAxRenderWidget::mode_TransformIntrinsic))
        {
            this->SelectVisualControls(this->mousePos, false);
        }

            // Left button was clicked just now
            if ((this->mouseButtons & mouseDeltaButtons & Qt::LeftButton) != 0)
            {
                AxTraceParameters *transformParams = this->editor->GetSelectedTransform();

                if ((this->mode == QAxRenderWidget::mode_TransformCameraTranslate) ||
                    (this->mode == QAxRenderWidget::mode_TransformCameraOrbit) ||
                    (this->mode == QAxRenderWidget::mode_TransformCameraRotate))
                {
                    this->dragMode = QAxRenderWidget::DragMode_MoveCamera;
                    break;
                }

                else if ((this->mode == QAxRenderWidget::mode_TransformExtrinsic) ||
                    (this->mode == QAxRenderWidget::mode_TransformIntrinsic))
                {
                    if (transformParams != 0)
                    {
                        if (this->SelectVisualControls(this->mousePos, true))
                        {
                            AxVector3 transformPos3 = transformParams->cameraRef->ProjectVector(transformParams->transformRef->GetPositionExtrinsic());
                            this->transformDeltaPos = AxVector2(transformPos3.x, transformPos3.y).Subtract(this->mousePos);

                            this->dragMode = QAxRenderWidget::DragMode_Transforming;
                            break;
                        }
                    }
                }

                if (this->PickScreenObject(this->mousePos))
                {
                    this->dragMode = QAxRenderWidget::DragMode_MouseDown;
                    break;
                }
            }


            // Right button was clicked just now
            if (((this->mouseButtons & mouseDeltaButtons & Qt::RightButton) != 0) ||
                ((this->mouseButtons & mouseDeltaButtons & Qt::MiddleButton) != 0))
            {
                this->dragMode = QAxRenderWidget::DragMode_MoveCamera;
                break;
            }

            break;
        }


        case QAxRenderWidget::DragMode_MouseDown:
        {
            if (this->mouseDelta.GetLength() != 0.0f)
            {
                // Left button has been held down and mouse was moved
                if (((this->mouseButtons & Qt::LeftButton) != 0) && ((mouseDeltaButtons & Qt::LeftButton) == 0))
                {
                    // Dragging is initiated here, on mouse move rather than on mouse down, because we want it to start when mouse has actually started dragging, rather than when immediately after it has been been clicked

                    if (this->mode == QAxRenderWidget::mode_Link)
                    {
                        if (this->mouseDownTraceParameters.meshRef != 0)
                        {
                            this->dropAction = EditorUtils::CreateObjectDrag(this, &this->mouseDownTraceParameters, "application/linkobject", ":/Icons/Icons/Manipulate/Link.png")->exec(Qt::MoveAction);

                            this->mouseDownPos = this->mousePos;
                            this->dragMode = QAxRenderWidget::DragMode_Link;
                            break;
                        }
                    }

                    else
                    {
                        if (this->mouseDownTraceParameters.materialRef != 0)
                        {
                            this->dropAction = EditorUtils::CreateResourceDrag(this, this->mouseDownTraceParameters.materialRef)->exec(Qt::CopyAction | Qt::MoveAction);

                            this->dragMode = QAxRenderWidget::DragMode_Resource;
                            break;
                        }
                    }
                }
            }

            break;
        }


        case QAxRenderWidget::DragMode_MoveCamera:
        {
            AxTransform *cameraTransform = this->editor->GetAxis()->camera.cameraRef != 0 ? this->editor->GetAxis()->camera.transformRef : 0;
            if (cameraTransform == 0)
                break;

            if ((this->mouseButtons & Qt::MiddleButton) != 0)
            {
                cameraTransform->TranslateIntrinsic(AxVector3(0.0f, 0.0f, mouseDelta.y * 200.0f));
            }

            if ((this->mode == QAxRenderWidget::mode_TransformCameraTranslate) || (this->mouseButtons == (Qt::LeftButton | Qt::RightButton)))
            {
                cameraTransform->TranslateIntrinsic(AxVector3(mouseDelta.x * 200.0f, -mouseDelta.y * 200.0f, 0.0f));
            }

            else if ((this->mode == QAxRenderWidget::mode_TransformCameraOrbit) || (this->mouseButtons == Qt::RightButton))
            {
                AxTransform *selectedTransform = this->editor->GetSelectedTransform() != 0 ? (AxTransform*)this->editor->GetSelectedTransform()->transformRef : 0;
                if (selectedTransform != 0)
                {
                    cameraTransform->RotateAroundRayExtrinsic(selectedTransform->GetPositionExtrinsic(), AxVector3(0.0f, 1.0f, 0.0f), mouseDelta.x * AxMath::Pi);
                    cameraTransform->RotateAroundRayExtrinsic(selectedTransform->GetPositionExtrinsic(), cameraTransform->GetRightVectorExtrinsic(), mouseDelta.y * AxMath::Pi);
                }
                else
                {
                    if (this->fixedYUp)
                    {
                        cameraTransform->RotateExtrinsic(AxVector3(0.0f, -mouseDelta.x, 0.0f).Scale(AxMath::Pi));
                        cameraTransform->RotateIntrinsic(AxVector3(mouseDelta.y, 0.0f, 0.0f).Scale(AxMath::Pi));
                    }
                    else
                    {
                        cameraTransform->RotateIntrinsic(AxVector3(mouseDelta.y, mouseDelta.x, 0.0f).Scale(AxMath::Pi));
                    }
                }
            }

            else if (this->mode == QAxRenderWidget::mode_TransformCameraRotate)
            {
                if (this->fixedYUp)
                {
                    cameraTransform->RotateExtrinsic(AxVector3(0.0f, -mouseDelta.x, 0.0f).Scale(AxMath::Pi));
                    cameraTransform->RotateIntrinsic(AxVector3(mouseDelta.y, 0.0f, 0.0f).Scale(AxMath::Pi));
                }
                else
                {
                    cameraTransform->RotateIntrinsic(AxVector3(mouseDelta.y, mouseDelta.x, 0.0f).Scale(AxMath::Pi));
                }
            }

            break;
        }

        case QAxRenderWidget::DragMode_Transforming:
        {
            AxTraceParameters *selectedResourceParameters = this->editor->GetSelectedTransform();

            AxTransform *transform = (AxTransform*)selectedResourceParameters->transformRef;
            AxCamera *camera = selectedResourceParameters->cameraRef;

            if ((transform == 0) || (camera == 0))
                break;

            AxVector3 transformPos = transform->GetPositionExtrinsic();

            bool translateX = this->translationAxes.x != 0.0f;
            bool translateY = this->translationAxes.y != 0.0f;
            bool translateZ = this->translationAxes.z != 0.0f;

            // Translation
            if (translateX || translateY || translateZ)
            {
                AxVector3 movingPlaneNormal;
                AxVector3 movingAxisX, movingAxisY, movingAxisZ;

                if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
                {
                    movingAxisX.Set(1.0f, 0.0f, 0.0f);
                    movingAxisY.Set(0.0f, 1.0f, 0.0f);
                    movingAxisZ.Set(0.0f, 0.0f, 1.0f);
                }
                else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
                {
                    movingAxisX.Set(transform->GetRightVectorExtrinsic().Normalize());
                    movingAxisY.Set(transform->GetUpVectorExtrinsic().Normalize());
                    movingAxisZ.Set(transform->GetForwardVectorExtrinsic().Normalize());
                };

                if (translateX && translateY && translateZ)
                {
                    if (this->fixedYUp)
                        movingPlaneNormal = camera->xAxis.Cross(AxVector3(0.0f, 1.0f, 0.0f)).Normalize();
                    else
                        movingPlaneNormal = camera->zAxis;
                }
                else if (translateX && translateY)
                {
                    movingPlaneNormal.Set(movingAxisZ);
                }
                else if (translateX && translateZ)
                {
                    movingPlaneNormal.Set(movingAxisY);
                }
                else if (translateY && translateZ)
                {
                    movingPlaneNormal.Set(movingAxisX);
                }
                else if (translateX)
                {
                    movingPlaneNormal.Set(movingAxisZ);
                }
                else if (translateY)
                {
                    movingPlaneNormal.Set(movingAxisZ);
                }
                else if (translateZ)
                {
                    movingPlaneNormal.Set(movingAxisX);
                }

                AxVector3 mouseRay = camera->CastVector(mousePos.Add(this->transformDeltaPos));
                AxVector3 mouseRayPoint;
                AxMaths::RayToPlaneIntersection(mouseRayPoint, camera->position, mouseRay, transformPos, movingPlaneNormal);

                if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
                {
                    AxVector3 nonTranslationAxes = AxVector3(1.0f).Subtract(this->translationAxes);
                    transform->SetPositionExtrinsic(transformPos.Scale(nonTranslationAxes).Add(mouseRayPoint.Scale(this->translationAxes)));
                }
                else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
                {
                    // Projects the point selected by the mouse, onto the local axis of the transform and sets the position to this projected point
                    AxVector3 position(0.0f);
                    AxVector3 axisProjected;

                    if (translateX)
                    {
                        AxMaths::PointToRayProjection(axisProjected, mouseRayPoint, transformPos, movingAxisX);
                        AxVector3::Add(position, position, axisProjected.Subtract(transformPos));
                    }

                    if (translateY)
                    {
                        AxMaths::PointToRayProjection(axisProjected, mouseRayPoint, transformPos, movingAxisY);
                        AxVector3::Add(position, position, axisProjected.Subtract(transformPos));
                    }

                    if (translateZ)
                    {
                        AxMaths::PointToRayProjection(axisProjected, mouseRayPoint, transformPos, movingAxisZ);
                        AxVector3::Add(position, position, axisProjected.Subtract(transformPos));
                    }

                    transform->SetPositionExtrinsic(transformPos.Add(position));
                }
            }

            //float mouseLinearMove = AxMath::Sign(mouseDeltaPos.x + mouseDeltaPos.y) * mouseDeltaPos.GetLength();
            float mouseLinearMove = AxMath::Sign(mouseDelta.x + mouseDelta.y) * mouseDelta.GetLength();

            // Rotation
            if (this->rotationAxes.GetLength() != 0)
            {
                AxVector3 rotationVector(mouseLinearMove * AxMath::Pi);
                AxVector3::Scale(rotationVector, rotationVector, this->rotationAxes);
                if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
                    transform->RotateExtrinsic(rotationVector);
                else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
                    transform->RotateIntrinsic(rotationVector);
            }

            // Scaling
            if (this->scalingAxes.GetLength() != 0)
            {
                AxVector3 scalingVector(mouseLinearMove);
                AxVector3::Scale(scalingVector, scalingVector, this->scalingAxes);
                AxVector3::Add(scalingVector, scalingVector, AxVector3(1.0f));
                if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
                    transform->ScaleExtrinsic(scalingVector);
                else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
                    transform->ScaleIntrinsic(scalingVector);
            }

            this->UpdateTransformInfoInEditor();

            break;
        }
    }

//    // Any button up
    //    if (((this->mouseButtons & mouseDeltaButtons) == 0) && (mouseDeltaButtons != 0))
    // All buttons up
    if ((this->mouseButtons == 0) && (mouseDeltaButtons != 0))
    {
        switch (this->dragMode)
        {
        case QAxRenderWidget::DragMode_None:
        case QAxRenderWidget::DragMode_MouseDown:
            {
                this->SelectPickedObject();

                // Reflect the selected transform in main window
                this->UpdateTransformInfoInEditor();

                break;
            }

            case QAxRenderWidget::DragMode_Transforming:
            {
                // Reflect in main window eventual change in transformOperation in case the user has clicked a different translate/rotate/scale control
                this->UpdateTransformInfoInEditor();

                break;
            }
        }

        // Update any highlighting on the visual controls
        this->SelectVisualControls(this->mousePos, false);

        this->dragMode = QAxRenderWidget::DragMode_None;
    }

    this->mouseScrollDelta.Set(0.0f);
}

void QAxRenderWidget::KeyRoutines()
{
    AxVector3 currentMoveVector(
        ((this->IsKeyDown(Qt::Key_A) ? 1.0f : 0.0f)     - (this->IsKeyDown(Qt::Key_D) ? 1.0f : 0.0f)),
        ((this->IsKeyDown(Qt::Key_Space) ? 1.0f : 0.0f) - (this->IsKeyDown(Qt::Key_Shift) ? 1.0f : 0.0f)),
        ((this->IsKeyDown(Qt::Key_W) ? 1.0f : 0.0f)     - (this->IsKeyDown(Qt::Key_S) ? 1.0f : 0.0f)));

    this->moveDelta = currentMoveVector.Subtract(this->moveVector);
    this->moveVector = currentMoveVector;

//    if (this->moveVector.GetLength() != 0.0f)
//        this->moveSpeed = AxMath::Min(this->moveSpeed + this->maxMoveSpeed * this->editor->GetAxis()->timer->actualTime, this->maxMoveSpeed);
//    else
//        this->moveSpeed = 0.0f;

    this->moveSmoothDelta = this->moveVector.Subtract(this->moveSmoothVector).Scale(5.0f * this->editor->GetAxis()->timer->actualTime);
    AxVector3::Add(this->moveSmoothVector, this->moveSmoothVector, this->moveSmoothDelta);

    AxVector3 moveVector = this->smoothManipulation ? this->moveSmoothVector : this->moveVector;

    AxTransform *cameraTransform = this->editor->GetAxis()->camera.cameraRef != 0 ? this->editor->GetAxis()->camera.transformRef : 0;
    if (cameraTransform != 0)
    {
        cameraTransform->TranslateIntrinsic(moveVector.Scale(50.0f * this->editor->GetAxis()->timer->time));
    }
}

bool QAxRenderWidget::IsKeyDown(int keyCode)
{
    return this->keysPressed.Contains(keyCode);
}

void QAxRenderWidget::resizeEvent(QResizeEvent * event)
{
    QWidget::resizeEvent(event);

    if (this->editor != 0)
        this->editor->GetAxis()->SetViewport(event->size().width(), event->size().height());
}

void QAxRenderWidget::dragEnterEvent(QDragEnterEvent *event)
{
    event->accept();
}

void QAxRenderWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (this->editor == 0)
    {
        event->ignore();
        return;
    }

    AxIntersectionInfo mouseOverIntersectionInfo;
    AxTraceParameters mouseOverEntityInfo;
    this->editor->GetAxis()->PickByScreenCoordsPixel(AxVector2(event->pos().x(), event->pos().y()), mouseOverIntersectionInfo, mouseOverEntityInfo);

    bool accept = false;

    if (this->mode == QAxRenderWidget::mode_Link)
    {
        AxTraceParameters *droppedObject;
        if (!EditorUtils::GetObjectDrag((void**)&droppedObject, event->mimeData(), "application/linkobject"))
        {
            event->ignore();
            return;
        }

        accept = true;
    }
    else
    {
        AxResource *droppedResource;
        if (!EditorUtils::GetResourceDrag(&droppedResource, event->mimeData()) || !mouseOverIntersectionInfo.hasIntersected)
        {
            event->ignore();
            return;
        }

        switch (droppedResource->resourceType)
        {
            case AxResourceType_Material:
            {
                accept = mouseOverEntityInfo.meshRef != 0;
                break;
            }
        }
    }

    if (accept)
        event->accept();
    else
        event->ignore();
}

void QAxRenderWidget::dropEvent(QDropEvent *event)
{
    // Reset mouse buttons, because when in drag-drop mode, mouseReleaseEvent is not invoked
    this->mouseButtons = 0;

    if (this->editor == 0)
    {
        event->ignore();
        return;
    }

    AxIntersectionInfo mouseOverIntersectionInfo;
    AxTraceParameters mouseOverEntityInfo;
    this->editor->GetAxis()->PickByScreenCoordsPixel(AxVector2(event->pos().x(), event->pos().y()), mouseOverIntersectionInfo, mouseOverEntityInfo);

    if (this->mode == QAxRenderWidget::mode_Link)
    {
        AxTraceParameters *droppedObject;
        if (EditorUtils::GetObjectDrag((void**)&droppedObject, event->mimeData(), "application/linkobject"))
        {
            if (mouseOverIntersectionInfo.hasIntersected)
                this->editor->GetAxis()->Attach(droppedObject->entitySetBranch, droppedObject->entitySetIndex, mouseOverEntityInfo.meshBranch, mouseOverEntityInfo.meshIndex + 1, mouseOverEntityInfo.transformRef);
            else
                this->editor->GetAxis()->Detach(droppedObject->entitySetBranch, droppedObject->entitySetIndex);
        }
    }
    else
    {
        AxResource *droppedResource;
        if (!EditorUtils::GetResourceDrag(&droppedResource, event->mimeData()) || !mouseOverIntersectionInfo.hasIntersected)
        {
            event->ignore();
            return;
        }

        switch (droppedResource->resourceType)
        {
            case AxResourceType_Material:
            {
                if (mouseOverEntityInfo.materialRef != 0)
                {
                    mouseOverEntityInfo.materialBranch->references[mouseOverEntityInfo.materialIndex] = droppedResource;
                }
                else
                {
                    mouseOverEntityInfo.meshBranch->references.Insert(mouseOverEntityInfo.meshIndex, droppedResource);
                }

                break;
            }
        }
    }

    event->acceptProposedAction();
}

bool QAxRenderWidget::SelectVisualControls(AxVector2 mousePosition, bool autoSetVisualControlsTransformOperation)
{
    AxTraceParameters *transformParams = this->editor->GetSelectedTransform();
    if (transformParams != 0)
        return this->editor->GetAxis()->PickVisualControlFromScreen(mousePosition, transformParams, this->translationAxes, this->rotationAxes, this->scalingAxes, autoSetVisualControlsTransformOperation, true);

    return false;
}

bool QAxRenderWidget::PickScreenObject(AxVector2 mousePosition)
{
    return this->editor->GetAxis()->PickByScreenCoordsUnit(mousePosition, this->mouseDownIntersectionInfo, this->mouseDownTraceParameters);
}

bool QAxRenderWidget::SelectPickedObject()
{
    this->editor->SelectTransform(this->mouseDownIntersectionInfo.hasIntersected ? &this->mouseDownTraceParameters : 0);
    this->editor->SelectResource(this->mouseDownIntersectionInfo.hasIntersected ? &this->mouseDownTraceParameters : 0);
    return this->mouseDownIntersectionInfo.hasIntersected;
}

void QAxRenderWidget::UpdateTransformInfoInEditor()
{
    AxTraceParameters *selectedResourceParameters = this->editor->GetSelectedTransform();
    if (selectedResourceParameters == 0)
        return;

    AxTransform *transform = (AxTransform*)selectedResourceParameters->transformRef;
    if (transform == 0)
        return;

    AxVector3 transformationParameters;

    if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
    {
        if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Translation)
            transformationParameters = transform->GetPositionExtrinsic();
        else if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Rotation)
            transformationParameters = transform->GetRotationExtrinsic();
        else if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Scaling)
            transformationParameters = transform->GetScalingExtrinsic();
    }
    else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
    {
        if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Translation)
            transformationParameters = transform->GetPositionIntrinsic();
        else if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Rotation)
            transformationParameters = transform->GetRotationIntrinsic();
        else if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Scaling)
            transformationParameters = transform->GetScalingIntrinsic();
    }
    else
        transformationParameters.Set(0);

    this->editor->OnTransformationChanged(this->editor->GetAxis()->visualControlsTransformOperation, transformationParameters);
}


void QAxRenderWidget::UpdateTransformFromEditor(AxVector3 transformXYZ)
{
    AxTraceParameters *selectedResourceParameters = this->editor->GetSelectedTransform();
    if (selectedResourceParameters == 0)
        return;

    AxTransform *transform = (AxTransform*)selectedResourceParameters->transformRef;
    if (transform == 0)
        return;

    if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Translation)
    {
        if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
            transform->SetPositionExtrinsic(transformXYZ);
        else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
            transform->SetPositionIntrinsic(transformXYZ);
    }
    else if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Rotation)
    {
        if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
            transform->SetRotationExtrinsic(transformXYZ);
        else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
            transform->SetRotationIntrinsic(transformXYZ);
    }
    else if (this->editor->GetAxis()->visualControlsTransformOperation == AxEditorEngine::visualControlsTransformOperation_Scaling)
    {
        if (this->mode == QAxRenderWidget::mode_TransformExtrinsic)
            transform->SetScalingExtrinsic(transformXYZ);
        else if (this->mode == QAxRenderWidget::mode_TransformIntrinsic)
            transform->SetScalingIntrinsic(transformXYZ);
    }
}

void QAxRenderWidget::Render()
{
    AxEditorEngine *engine = this->editor->GetAxis();

    if (engine == 0)
        return;

    this->MouseRoutines();

    this->KeyRoutines();

    engine->RenderScene(false);

    if ((this->mode == QAxRenderWidget::mode_TransformExtrinsic) || (this->mode == QAxRenderWidget::mode_TransformIntrinsic) ||
        (this->mode == QAxRenderWidget::mode_TransformCameraTranslate) || (this->mode == QAxRenderWidget::mode_TransformCameraOrbit) || (this->mode == QAxRenderWidget::mode_TransformCameraRotate))
    {
        engine->graphicsDevice->ClearDepthBuffer();
        engine->RenderVisualControls(this->editor->GetSelectedTransform());
    }

    engine->PresentOnScreen();
}

void QAxRenderWidget::SetMode(int mode)
{
    this->mode = mode;

    switch (this->mode)
    {
        case QAxRenderWidget::mode_TransformExtrinsic:
        {
            this->editor->GetAxis()->SetVisualControlsCoordinateSystemMode(AxEditorEngine::visualControlsCoordinateSystemMode_Extrinsic);
            break;
        }

        case QAxRenderWidget::mode_TransformIntrinsic:
        {
            this->editor->GetAxis()->SetVisualControlsCoordinateSystemMode(AxEditorEngine::visualControlsCoordinateSystemMode_Intrinsic);
            break;
        }
    }

    this->UpdateTransformInfoInEditor();
}
