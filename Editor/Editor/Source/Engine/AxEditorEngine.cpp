//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "AxEditorEngine.h"

#include "../EditorTools/EditorUtils.h"

AxEditorEngine::AxEditorEngine(void *windowHandle)
    : Axis(windowHandle)
{
    this->CreateVisualControls();
    this->CreateThumbnailEntities();
}

AxEditorEngine::~AxEditorEngine()
{
    this->FreeThumbnailEntities();
    this->FreeVisualControls();
}


void AxEditorEngine::SetGraphicsDevice(AxGraphicsDevice *graphicsDevice)
{
    this->FreeThumbnailEntities();
    this->FreeVisualControls();

    Axis::SetGraphicsDevice(graphicsDevice);

    this->CreateVisualControls();
    this->CreateThumbnailEntities();
}

void AxEditorEngine::ProcessEntity(AxResource *entity, AxTraceParameters &parameters)
{
    Axis::ProcessEntity(entity, parameters);

    if (entity->resourceType == AxResourceType_Camera)
    {
        if ((parameters.renderTargetRef == 0) && (entity->properties[AxResource::propertyIndex_Enabled]->GetBool()))
            this->camera = parameters;
    }
}

void AxEditorEngine::Reset()
{
    this->camera = AxTraceParameters();

    Axis::Reset();
}

void AxEditorEngine::FreeCustomEntity(AxResource **entity)
{
    if (*entity != 0)
    {
        delete *entity;
        *entity = 0;
    }
}

void AxEditorEngine::CreateVisualControls()
{
    const float axisArrowWholeLength = 1.0f;
    const float axisArrowBodyDiameter = 0.08f;
    const float axisArrowTipLength = 0.3f;
    const float axisArrowTipDiameter = 0.2f;
    const int axisArrowSegments = 8;

    const float scalingAxisArrowWholeLength = 0.5f;
    const float scalingAxisArrowBodyDiameter = 0.08f;
    const float scalingAxisArrowTipSize = 0.12f;
    const int scalingAxisArrowBodySegments = 8;

    const float rotationTorusDiameter = 1.2f;
    //const float rotationTorusDiameter = 0.2f;
    const float rotationTorusThickness = 0.05f;
    const float rotationTorusPosition = 0.0f;
    //const float rotationTorusPosition = 0.6f;
    const int rotationTorusRadialSegments = 32;
    //const int rotationTorusRadialSegments = 16;
    const int rotationTorusRingSegments = 5;

    const float doubleAxisHandleSize = 0.6f;
    const float doubleAxisHandleThickness = 0.05f;

    const float doubleScaleHandleSize = 0.4f;
    const float doubleScaleHandleThickness = 0.05f;

    const float doubleAxisFlatteningScale = 0.5f;

    const float centralSphereDiameter = 0.3f;

    const float centralBoxSize = 0.2f;

    const float colorsScaling = 1.25f;
    const float colorsAlpha = 1.0f;


    // Create translation axis arrow
    AxDeviceIndependentMesh arrowBody, arrowTip;
    AxPrismGeometry(arrowBody, axisArrowBodyDiameter / 2.0f, axisArrowBodyDiameter / 2.0f, axisArrowWholeLength - axisArrowTipLength, axisArrowSegments, 1, 1, 0);
    AxPrismGeometry(arrowTip, axisArrowTipDiameter / 2.0f, 0, axisArrowTipLength, axisArrowSegments, 1, 1, 0);
    AxMatrix arrowBodyTransform, arrowTipTransform;
    AxMatrix::CreateTranslation(arrowBodyTransform, 0, (axisArrowWholeLength - axisArrowTipLength) / 2.0f, 0);
    AxMatrix::CreateTranslation(arrowTipTransform, 0, axisArrowWholeLength - axisArrowTipLength / 2.0f, 0);

    // YAxis
    this->yAxis = new AxMesh(this);
    AxMergeGeometry(*this->yAxis->deviceMesh, arrowBody, arrowTip, arrowBodyTransform, arrowTipTransform);

    this->yAxis->ComputeNormals();
    this->yAxis->ComputeBounds();

    this->yAxis->deviceMesh->UpdateVertices(0, this->yAxis->deviceMesh->GetVertexCount());
    this->yAxis->deviceMesh->UpdateIndices(0, this->yAxis->deviceMesh->GetIndexCount());


    // XAxis
    this->xAxis = new AxMesh(this);
    AxMatrix xAxisTransform;
    AxMatrix::CreateRotationZ(xAxisTransform, - AxMath::PiHalf);
    AxCopyGeometry(*this->xAxis->deviceMesh, *this->yAxis->deviceMesh, xAxisTransform);

    this->xAxis->ComputeBounds();

    this->xAxis->deviceMesh->UpdateVertices(0, this->xAxis->deviceMesh->GetVertexCount());
    this->xAxis->deviceMesh->UpdateIndices(0, this->xAxis->deviceMesh->GetIndexCount());


    // ZAxis
    this->zAxis = new AxMesh(this);
    AxMatrix zAxisTransform;
    AxMatrix::CreateRotationX(zAxisTransform, AxMath::PiHalf);
    AxCopyGeometry(*this->zAxis->deviceMesh, *this->yAxis->deviceMesh, zAxisTransform);

    this->zAxis->ComputeBounds();

    this->zAxis->deviceMesh->UpdateVertices(0, this->zAxis->deviceMesh->GetVertexCount());
    this->zAxis->deviceMesh->UpdateIndices(0, this->zAxis->deviceMesh->GetIndexCount());



    // Create double-axis translation handle
    AxDeviceIndependentMesh doubleTranslateHandle;
    AxMatrix doubleTranslateHandleTransform, doubleTranslateHandleScale;
    AxMatrix::CreateScaling(doubleTranslateHandleScale, AxVector3(1.0f, doubleAxisFlatteningScale, 1.0f));
    AxMatrix::CreateRotationX(doubleTranslateHandleTransform, -AxMath::PiHalf);
    AxMatrix::Multiply(doubleTranslateHandleTransform, doubleTranslateHandleScale, doubleTranslateHandleTransform);
    AxTorusGeometry(doubleTranslateHandle, 8, 4, 0, doubleAxisHandleSize, doubleAxisHandleThickness, 0.0f, 0.0f, 1.0f / 4.0f, false, true);

    // XYAxis
    this->xyAxis = new AxMesh(this);
    AxCopyGeometry(*this->xyAxis->deviceMesh, doubleTranslateHandle, doubleTranslateHandleTransform);

    this->xyAxis->ComputeNormals();
    this->xyAxis->ComputeBounds();

    this->xyAxis->deviceMesh->UpdateVertices(0, this->xyAxis->deviceMesh->GetVertexCount());
    this->xyAxis->deviceMesh->UpdateIndices(0, this->xyAxis->deviceMesh->GetIndexCount());

    // XZAxis
    this->xzAxis = new AxMesh(this);
    AxMatrix xzAxisTransform;
    AxMatrix::CreateRotationX(xzAxisTransform, AxMath::PiHalf);
    AxCopyGeometry(*this->xzAxis->deviceMesh, *this->xyAxis->deviceMesh, xzAxisTransform);

    this->xzAxis->ComputeNormals();
    this->xzAxis->ComputeBounds();

    this->xzAxis->deviceMesh->UpdateVertices(0, this->xzAxis->deviceMesh->GetVertexCount());
    this->xzAxis->deviceMesh->UpdateIndices(0, this->xzAxis->deviceMesh->GetIndexCount());

    // YZAxis
    this->yzAxis = new AxMesh(this);
    AxMatrix yzAxisTransform;
    AxMatrix::CreateRotationY(yzAxisTransform, -AxMath::PiHalf);
    AxCopyGeometry(*this->yzAxis->deviceMesh, *this->xyAxis->deviceMesh, yzAxisTransform);

    this->yzAxis->ComputeNormals();
    this->yzAxis->ComputeBounds();

    this->yzAxis->deviceMesh->UpdateVertices(0, this->yzAxis->deviceMesh->GetVertexCount());
    this->yzAxis->deviceMesh->UpdateIndices(0, this->yzAxis->deviceMesh->GetIndexCount());



    // Create double-axis scaling handle
    AxDeviceIndependentMesh doubleScaleHandle;
    AxMatrix doubleScaleHandleTransform, doubleScaleHandleScale;
    AxMatrix::CreateScaling(doubleScaleHandleScale, AxVector3(1.0f, doubleAxisFlatteningScale, 1.0f));
    AxMatrix::CreateRotationX(doubleScaleHandleTransform, AxMath::PiHalf);
    AxMatrix::Multiply(doubleScaleHandleTransform, doubleScaleHandleScale, doubleScaleHandleTransform);
    AxTorusGeometry(doubleScaleHandle, 1, 4, 0, doubleScaleHandleSize, doubleScaleHandleThickness / AxMath::Cos(AxMath::Pi / 4.0f), 0.0f, 0.0f, 1.0f / 4.0f, false, true);

    // XYScale
    this->xyScale = new AxMesh(this);
    AxCopyGeometry(*this->xyScale->deviceMesh, doubleScaleHandle, doubleScaleHandleTransform);

    this->xyScale->ComputeNormals();
    this->xyScale->ComputeBounds();

    this->xyScale->deviceMesh->UpdateVertices(0, this->xyScale->deviceMesh->GetVertexCount());
    this->xyScale->deviceMesh->UpdateIndices(0, this->xyScale->deviceMesh->GetIndexCount());

    // XZScale
    this->xzScale = new AxMesh(this);
    AxMatrix xzScaleTransform;
    AxMatrix::CreateRotationX(xzScaleTransform, AxMath::PiHalf);
    AxCopyGeometry(*this->xzScale->deviceMesh, *this->xyScale->deviceMesh, xzScaleTransform);

    this->xzScale->ComputeNormals();
    this->xzScale->ComputeBounds();

    this->xzScale->deviceMesh->UpdateVertices(0, this->xzScale->deviceMesh->GetVertexCount());
    this->xzScale->deviceMesh->UpdateIndices(0, this->xzScale->deviceMesh->GetIndexCount());

    // YZScale
    this->yzScale = new AxMesh(this);
    AxMatrix yzScaleTransform;
    AxMatrix::CreateRotationY(yzScaleTransform, -AxMath::PiHalf);
    AxCopyGeometry(*this->yzScale->deviceMesh, *this->xyScale->deviceMesh, yzScaleTransform);

    this->yzScale->ComputeNormals();
    this->yzScale->ComputeBounds();

    this->yzScale->deviceMesh->UpdateVertices(0, this->yzScale->deviceMesh->GetVertexCount());
    this->yzScale->deviceMesh->UpdateIndices(0, this->yzScale->deviceMesh->GetIndexCount());



    // YTorus
    AxDeviceIndependentMesh torus;
    AxTorusGeometry(torus, rotationTorusRadialSegments, rotationTorusRingSegments, 0, rotationTorusDiameter, rotationTorusThickness, 0.0f, 0.0f, 1.0, true, true);
    this->yTorus = new AxMesh(this);
    AxMatrix yTorusTransform;
    AxMatrix::CreateTranslation(yTorusTransform, 0.0f, rotationTorusPosition, 0.0f);
    AxCopyGeometry(*this->yTorus->deviceMesh, torus, yTorusTransform);

    this->yTorus->ComputeNormals();
    this->yTorus->ComputeBounds();

    this->yTorus->deviceMesh->UpdateVertices(0, this->yTorus->deviceMesh->GetVertexCount());
    this->yTorus->deviceMesh->UpdateIndices(0, this->yTorus->deviceMesh->GetIndexCount());


    // XTorus
    this->xTorus = new AxMesh(this);
    AxCopyGeometry(*this->xTorus->deviceMesh, *this->yTorus->deviceMesh, xAxisTransform);

    this->xTorus->ComputeBounds();

    this->xTorus->deviceMesh->UpdateVertices(0, this->xTorus->deviceMesh->GetVertexCount());
    this->xTorus->deviceMesh->UpdateIndices(0, this->xTorus->deviceMesh->GetIndexCount());


    // zTorus
    this->zTorus = new AxMesh(this);
    AxCopyGeometry(*this->zTorus->deviceMesh, *this->yTorus->deviceMesh, zAxisTransform);

    this->zTorus->ComputeBounds();

    this->zTorus->deviceMesh->UpdateVertices(0, this->zTorus->deviceMesh->GetVertexCount());
    this->zTorus->deviceMesh->UpdateIndices(0, this->zTorus->deviceMesh->GetIndexCount());



    // Create scaling axis arrow
    AxDeviceIndependentMesh scalingArrowBody, scalingArrowTip;
    AxPrismGeometry(scalingArrowBody, scalingAxisArrowBodyDiameter / 2.0f, scalingAxisArrowBodyDiameter / 2.0f, scalingAxisArrowWholeLength - scalingAxisArrowTipSize, scalingAxisArrowBodySegments, 1, 1, 0);
    AxBoxGeometry(scalingArrowTip, 1, 1, 1, false, AxVector3(scalingAxisArrowTipSize, 0.0f, 0.0f), AxVector3(0.0f, scalingAxisArrowTipSize, 0.0f), AxVector3(0.0f, 0.0f, scalingAxisArrowTipSize));
    AxMatrix scalingArrowBodyTransform, scalingArrowTipTransform;
    AxMatrix::CreateTranslation(scalingArrowBodyTransform, 0, -(scalingAxisArrowWholeLength - scalingAxisArrowTipSize) / 2.0f, 0);
    AxMatrix::CreateTranslation(scalingArrowTipTransform, 0, -(scalingAxisArrowWholeLength - scalingAxisArrowTipSize / 2.0f), 0);

    // Scaling YAxis
    this->yScaling = new AxMesh(this);
    AxMergeGeometry(*this->yScaling->deviceMesh, scalingArrowBody, scalingArrowTip, scalingArrowBodyTransform, scalingArrowTipTransform);

    this->yScaling->ComputeNormals();
    this->yScaling->ComputeBounds();

    this->yScaling->deviceMesh->UpdateVertices(0, this->yScaling->deviceMesh->GetVertexCount());
    this->yScaling->deviceMesh->UpdateIndices(0, this->yScaling->deviceMesh->GetIndexCount());

    // Scaling XAxis
    this->xScaling = new AxMesh(this);
    AxMatrix xScalingAxisTransform;
    AxMatrix::CreateRotationZ(xScalingAxisTransform, - AxMath::PiHalf);
    AxCopyGeometry(*this->xScaling->deviceMesh, *this->yScaling->deviceMesh, xScalingAxisTransform);

    this->xScaling->ComputeBounds();

    this->xScaling->deviceMesh->UpdateVertices(0, this->xScaling->deviceMesh->GetVertexCount());
    this->xScaling->deviceMesh->UpdateIndices(0, this->xScaling->deviceMesh->GetIndexCount());

    // Scaling ZAxis
    this->zScaling = new AxMesh(this);
    AxMatrix zScalingAxisTransform;
    AxMatrix::CreateRotationX(zScalingAxisTransform, AxMath::PiHalf);
    AxCopyGeometry(*this->zScaling->deviceMesh, *this->yScaling->deviceMesh, zScalingAxisTransform);

    this->zScaling->ComputeBounds();

    this->zScaling->deviceMesh->UpdateVertices(0, this->zScaling->deviceMesh->GetVertexCount());
    this->zScaling->deviceMesh->UpdateIndices(0, this->zScaling->deviceMesh->GetIndexCount());



    // Create central sphere
    this->uniformTranslation = new AxMesh(this);
    AxGeosphereGeometry(*this->uniformTranslation->deviceMesh, centralSphereDiameter / 2.0f, 2);

    this->uniformTranslation->ComputeNormals();
    this->uniformTranslation->ComputeBounds();

    this->uniformTranslation->deviceMesh->UpdateVertices(0, this->uniformTranslation->deviceMesh->GetVertexCount());
    this->uniformTranslation->deviceMesh->UpdateIndices(0, this->uniformTranslation->deviceMesh->GetIndexCount());


    // Create central box
    this->uniformScaling = new AxMesh(this);
    AxBoxGeometry(*this->uniformScaling->deviceMesh, 1, 1, 1, false, AxVector3(centralBoxSize, 0.0f, 0.0f), AxVector3(0.0f, centralBoxSize, 0.0f), AxVector3(0.0f, 0.0f, centralBoxSize));

    this->uniformScaling->ComputeNormals();
    this->uniformScaling->ComputeBounds();

    this->uniformScaling->deviceMesh->UpdateVertices(0, this->uniformScaling->deviceMesh->GetVertexCount());
    this->uniformScaling->deviceMesh->UpdateIndices(0, this->uniformScaling->deviceMesh->GetIndexCount());

    this->uniformScaling->properties[AxResource::propertyIndex_Enabled]->SetBool(false);


    // Create materials
    this->xAxisMaterial = new AxMaterial(this);
    this->xAxisMaterial->name = "xAxisMaterial";
    this->xAxisMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xAxisMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xAxisMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 0.0f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->xAxisMaterial->UpdateShader();

    this->yAxisMaterial = new AxMaterial(this);
    this->yAxisMaterial->name = "yAxisMaterial";
    this->yAxisMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->yAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->yAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->yAxisMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->yAxisMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(0.0f, 1.0f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->yAxisMaterial->UpdateShader();

    this->zAxisMaterial = new AxMaterial(this);
    this->zAxisMaterial->name = "zAxisMaterial";
    this->zAxisMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->zAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->zAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->zAxisMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->zAxisMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(0.0f, 0.0f, 1.0f).Scale(colorsScaling), colorsAlpha));
    this->zAxisMaterial->UpdateShader();

    this->xAxisRotationMaterial = new AxMaterial(this);
    this->xAxisRotationMaterial->name = "xAxisRotationMaterial";
    this->xAxisRotationMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xAxisRotationMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xAxisRotationMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xAxisRotationMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xAxisRotationMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 0.5f, 0.5f).Scale(colorsScaling), colorsAlpha));
    this->xAxisRotationMaterial->UpdateShader();

    this->yAxisRotationMaterial = new AxMaterial(this);
    this->yAxisRotationMaterial->name = "yAxisRotationMaterial";
    this->yAxisRotationMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->yAxisRotationMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->yAxisRotationMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->yAxisRotationMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->yAxisRotationMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(0.8f, 1.0f, 0.5f).Scale(colorsScaling), colorsAlpha));
    this->yAxisRotationMaterial->UpdateShader();

    this->zAxisRotationMaterial = new AxMaterial(this);
    this->zAxisRotationMaterial->name = "zAxisRotationMaterial";
    this->zAxisRotationMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->zAxisRotationMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->zAxisRotationMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->zAxisRotationMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->zAxisRotationMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(0.5f, 0.5f, 1.0f).Scale(colorsScaling), colorsAlpha));
    this->zAxisRotationMaterial->UpdateShader();

    this->xAxisScalingMaterial = new AxMaterial(this);
    this->xAxisScalingMaterial->name = "xAxisScalingMaterial";
    this->xAxisScalingMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xAxisScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xAxisScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xAxisScalingMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xAxisScalingMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 0.0f, 0.6f).Scale(colorsScaling), colorsAlpha));
    this->xAxisScalingMaterial->UpdateShader();

    this->yAxisScalingMaterial = new AxMaterial(this);
    this->yAxisScalingMaterial->name = "yAxisScalingMaterial";
    this->yAxisScalingMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->yAxisScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->yAxisScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->yAxisScalingMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->yAxisScalingMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(0.6f, 1.0f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->yAxisScalingMaterial->UpdateShader();

    this->zAxisScalingMaterial = new AxMaterial(this);
    this->zAxisScalingMaterial->name = "zAxisScalingMaterial";
    this->zAxisScalingMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->zAxisScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->zAxisScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->zAxisScalingMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->zAxisScalingMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(0.0f, 0.6f, 1.0f).Scale(colorsScaling), colorsAlpha));
    this->zAxisScalingMaterial->UpdateShader();

    this->uniformTransformMaterial = new AxMaterial(this);
    this->uniformTransformMaterial->name = "uniformTransformMaterial";
    this->uniformTransformMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->uniformTransformMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->uniformTransformMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->uniformTransformMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->uniformTransformMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 1.0f, 1.0f).Scale(colorsScaling), colorsAlpha));
    this->uniformTransformMaterial->UpdateShader();

    this->xyAxisMaterial = new AxMaterial(this);
    this->xyAxisMaterial->name = "xyAxisMaterial";
    this->xyAxisMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xyAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xyAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xyAxisMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xyAxisMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 0.75f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->xyAxisMaterial->UpdateShader();

    this->xzAxisMaterial = new AxMaterial(this);
    this->xzAxisMaterial->name = "xzAxisMaterial";
    this->xzAxisMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xzAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xzAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xzAxisMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xzAxisMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 0.75f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->xzAxisMaterial->UpdateShader();

    this->yzAxisMaterial = new AxMaterial(this);
    this->yzAxisMaterial->name = "yzAxisMaterial";
    this->yzAxisMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->yzAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->yzAxisMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->yzAxisMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->yzAxisMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 0.75f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->yzAxisMaterial->UpdateShader();

    this->xyScalingMaterial = new AxMaterial(this);
    this->xyScalingMaterial->name = "xyScalingMaterial";
    this->xyScalingMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xyScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xyScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xyScalingMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xyScalingMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 1.0f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->xyScalingMaterial->UpdateShader();

    this->xzScalingMaterial = new AxMaterial(this);
    this->xzScalingMaterial->name = "xzScalingMaterial";
    this->xzScalingMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->xzScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->xzScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->xzScalingMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->xzScalingMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 1.0f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->xzScalingMaterial->UpdateShader();

    this->yzScalingMaterial = new AxMaterial(this);
    this->yzScalingMaterial->name = "yzScalingMaterial";
    this->yzScalingMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->yzScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Add);
    this->yzScalingMaterial->AddShadingLayer(AxShadingOperation_FlatColor, AxColorOperation_Modulate);
    this->yzScalingMaterial->GetProperty("Flat color")->SetColor(AxVector4(1.0f));
    this->yzScalingMaterial->GetProperty("Flat color 1")->SetColor(AxVector4(AxVector3(1.0f, 1.0f, 0.0f).Scale(colorsScaling), colorsAlpha));
    this->yzScalingMaterial->UpdateShader();

    this->axesTransform = new AxTransform();

    this->axesSet = new AxEntitySet();
    this->axesSet->references.Add(this->axesTransform);
    this->axesSet->references.Add(this->xAxisMaterial);
    this->axesSet->references.Add(this->xAxis);
    this->axesSet->references.Add(this->xAxisRotationMaterial);
    this->axesSet->references.Add(this->xTorus);
    this->axesSet->references.Add(this->xAxisScalingMaterial);
    this->axesSet->references.Add(this->xScaling);
    this->axesSet->references.Add(this->yAxisMaterial);
    this->axesSet->references.Add(this->yAxis);
    this->axesSet->references.Add(this->yAxisRotationMaterial);
    this->axesSet->references.Add(this->yTorus);
    this->axesSet->references.Add(this->yAxisScalingMaterial);
    this->axesSet->references.Add(this->yScaling);
    this->axesSet->references.Add(this->zAxisMaterial);
    this->axesSet->references.Add(this->zAxis);
    this->axesSet->references.Add(this->zAxisRotationMaterial);
    this->axesSet->references.Add(this->zTorus);
    this->axesSet->references.Add(this->zAxisScalingMaterial);
    this->axesSet->references.Add(this->zScaling);
    this->axesSet->references.Add(this->xyAxisMaterial);
    this->axesSet->references.Add(this->xyAxis);
    this->axesSet->references.Add(this->xzAxisMaterial);
    this->axesSet->references.Add(this->xzAxis);
    this->axesSet->references.Add(this->yzAxisMaterial);
    this->axesSet->references.Add(this->yzAxis);
    this->axesSet->references.Add(this->xyScalingMaterial);
    this->axesSet->references.Add(this->xyScale);
    this->axesSet->references.Add(this->xzScalingMaterial);
    this->axesSet->references.Add(this->xzScale);
    this->axesSet->references.Add(this->yzScalingMaterial);
    this->axesSet->references.Add(this->yzScale);
    this->axesSet->references.Add(this->uniformTransformMaterial);
    this->axesSet->references.Add(this->uniformTranslation);
    this->axesSet->references.Add(this->uniformScaling);

    this->HighlightVisualControl(0);

    this->SetVisualControlsCoordinateSystemMode(AxEditorEngine::visualControlsCoordinateSystemMode_Extrinsic);
    this->SetVisualControlsTransformOperation(AxEditorEngine::visualControlsTransformOperation_Translation);
}


void AxEditorEngine::FreeVisualControls()
{
    this->FreeCustomEntity((AxResource**)&this->axesTransform);
    this->FreeCustomEntity((AxResource**)&this->xAxis);
    this->FreeCustomEntity((AxResource**)&this->yAxis);
    this->FreeCustomEntity((AxResource**)&this->zAxis);
    this->FreeCustomEntity((AxResource**)&this->xTorus);
    this->FreeCustomEntity((AxResource**)&this->yTorus);
    this->FreeCustomEntity((AxResource**)&this->zTorus);
    this->FreeCustomEntity((AxResource**)&this->xScaling);
    this->FreeCustomEntity((AxResource**)&this->yScaling);
    this->FreeCustomEntity((AxResource**)&this->zScaling);
    this->FreeCustomEntity((AxResource**)&this->xyAxis);
    this->FreeCustomEntity((AxResource**)&this->xzAxis);
    this->FreeCustomEntity((AxResource**)&this->yzAxis);
    this->FreeCustomEntity((AxResource**)&this->xyScale);
    this->FreeCustomEntity((AxResource**)&this->xzScale);
    this->FreeCustomEntity((AxResource**)&this->yzScale);
    this->FreeCustomEntity((AxResource**)&this->uniformTranslation);
    this->FreeCustomEntity((AxResource**)&this->uniformScaling);
    this->FreeCustomEntity((AxResource**)&this->xAxisMaterial);
    this->FreeCustomEntity((AxResource**)&this->yAxisMaterial);
    this->FreeCustomEntity((AxResource**)&this->zAxisMaterial);
    this->FreeCustomEntity((AxResource**)&this->xAxisRotationMaterial);
    this->FreeCustomEntity((AxResource**)&this->yAxisRotationMaterial);
    this->FreeCustomEntity((AxResource**)&this->zAxisRotationMaterial);
    this->FreeCustomEntity((AxResource**)&this->xAxisScalingMaterial);
    this->FreeCustomEntity((AxResource**)&this->yAxisScalingMaterial);
    this->FreeCustomEntity((AxResource**)&this->zAxisScalingMaterial);
    this->FreeCustomEntity((AxResource**)&this->xyAxisMaterial);
    this->FreeCustomEntity((AxResource**)&this->xzAxisMaterial);
    this->FreeCustomEntity((AxResource**)&this->yzAxisMaterial);
    this->FreeCustomEntity((AxResource**)&this->xyScalingMaterial);
    this->FreeCustomEntity((AxResource**)&this->xzScalingMaterial);
    this->FreeCustomEntity((AxResource**)&this->yzScalingMaterial);
    this->FreeCustomEntity((AxResource**)&this->uniformTransformMaterial);
    this->FreeCustomEntity((AxResource**)&this->axesSet);
}

void AxEditorEngine::HighlightVisualControl(AxMaterial *material)
{
    for (int i = 0; i < this->axesSet->references.count; i++)
    {
        if (this->axesSet->references[i]->resourceType == AxResourceType_Material)
        {
            AxMaterial *referenceMaterial = (AxMaterial*)this->axesSet->references[i];
            AxProperty *highlightProperty = referenceMaterial->GetProperty("Flat color amount");
            if (highlightProperty != 0)
            {
                highlightProperty->SetFloat(referenceMaterial == material ? 0.5f : 0.0f);
            }
        }
    }
}

void AxEditorEngine::EnableVisualControlsMode(AxTraceParameters *selectedResourceParameters)
{
    AxVector3 transformPos;
    selectedResourceParameters->transformRef->Process(selectedResourceParameters->parentTransformRef);
    AxMatrix::GetTranslation(transformPos, selectedResourceParameters->transformRef->worldMatrix);
    float distance = AxVector3::Distance(selectedResourceParameters->cameraRef->position, transformPos);
    float controlsAngularSize = selectedResourceParameters->cameraRef->properties.Get(AxCamera::propertyIndex_FOV)->GetFloat() / 8.0f;
    float scalingFactor = AxMaths::SizeFromDistanceAndAngle(distance, controlsAngularSize);

    AxMatrix::CreateIdentity(this->axesTransform->transform);

    AxMatrix::Copy(this->savedWorldMatrix, selectedResourceParameters->transformRef->worldMatrix);
    if (this->visualControlsCoordinateSystemMode == AxEditorEngine::visualControlsCoordinateSystemMode_Extrinsic)
        AxMatrix::CreateIdentity(selectedResourceParameters->transformRef->worldMatrix);
    AxMatrix::SetTranslation(selectedResourceParameters->transformRef->worldMatrix, transformPos);
    AxMatrix::SetScaling(selectedResourceParameters->transformRef->worldMatrix, AxVector3(scalingFactor));
}

void AxEditorEngine::DisableVisualControlsMode(AxTraceParameters *selectedResourceParameters)
{
    AxMatrix::Copy(selectedResourceParameters->transformRef->worldMatrix, this->savedWorldMatrix);
}

void AxEditorEngine::RenderVisualControls(AxTraceParameters *selectedResourceParameters)
{
    if (selectedResourceParameters == 0)
        return;

    if ((selectedResourceParameters->cameraRef == 0) || (selectedResourceParameters->transformRef == 0))
        return;

    this->EnableVisualControlsMode(selectedResourceParameters);

    this->traceEvents = this->renderEvents;
    AxTraceParameters axesParams(*selectedResourceParameters);
    this->ProcessEntity(this->axesSet, axesParams);

    this->DisableVisualControlsMode(selectedResourceParameters);
}

bool AxEditorEngine::PickVisualControlFromScreen(AxVector2 &screenUnitCoords, AxTraceParameters *selectedResourceParameters, AxVector3 &translationAxes, AxVector3 &rotationAxes, AxVector3 &scalingAxes, bool autoSetVisualControlsTransformOperation, bool autoHighlightVisualControl)
{
    translationAxes.Set(0.0f);
    rotationAxes.Set(0.0f);

    if ((selectedResourceParameters->cameraRef == 0) || (selectedResourceParameters->transformRef == 0))
        return false;


    this->EnableVisualControlsMode(selectedResourceParameters);

    this->screenPickEvents->SetupScreenPick(screenUnitCoords);

    this->traceEvents = this->screenPickEvents;
    AxTraceParameters axesParams(*selectedResourceParameters);
    this->ProcessEntity(this->axesSet, axesParams);

    this->DisableVisualControlsMode(selectedResourceParameters);

    if (autoHighlightVisualControl)
    {
        this->HighlightVisualControl(this->screenPickEvents->intersectionInfo.hasIntersected ? this->screenPickEvents->entityInfo.materialRef : 0);
    }

    if (this->screenPickEvents->intersectionInfo.hasIntersected)
    {
        translationAxes.x = (this->screenPickEvents->entityInfo.meshRef == this->xAxis) || (this->screenPickEvents->entityInfo.meshRef == this->uniformTranslation) || (this->screenPickEvents->entityInfo.meshRef == this->xyAxis) || (this->screenPickEvents->entityInfo.meshRef == this->xzAxis) ? 1.0f : 0.0f;
        translationAxes.y = (this->screenPickEvents->entityInfo.meshRef == this->yAxis) || (this->screenPickEvents->entityInfo.meshRef == this->uniformTranslation) || (this->screenPickEvents->entityInfo.meshRef == this->xyAxis) || (this->screenPickEvents->entityInfo.meshRef == this->yzAxis) ? 1.0f : 0.0f;
        translationAxes.z = (this->screenPickEvents->entityInfo.meshRef == this->zAxis) || (this->screenPickEvents->entityInfo.meshRef == this->uniformTranslation) || (this->screenPickEvents->entityInfo.meshRef == this->xzAxis) || (this->screenPickEvents->entityInfo.meshRef == this->yzAxis) ? 1.0f : 0.0f;

        rotationAxes.x = this->screenPickEvents->entityInfo.meshRef == this->xTorus ? 1.0f : 0.0f;
        rotationAxes.y = this->screenPickEvents->entityInfo.meshRef == this->yTorus ? 1.0f : 0.0f;
        rotationAxes.z = this->screenPickEvents->entityInfo.meshRef == this->zTorus ? 1.0f : 0.0f;

        scalingAxes.x = (this->screenPickEvents->entityInfo.meshRef == this->xScaling) || (this->screenPickEvents->entityInfo.meshRef == this->uniformScaling) || (this->screenPickEvents->entityInfo.meshRef == this->xyScale) || (this->screenPickEvents->entityInfo.meshRef == this->xzScale) ? 1.0f : 0.0f;
        scalingAxes.y = (this->screenPickEvents->entityInfo.meshRef == this->yScaling) || (this->screenPickEvents->entityInfo.meshRef == this->uniformScaling) || (this->screenPickEvents->entityInfo.meshRef == this->xyScale) || (this->screenPickEvents->entityInfo.meshRef == this->yzScale) ? 1.0f : 0.0f;
        scalingAxes.z = (this->screenPickEvents->entityInfo.meshRef == this->zScaling) || (this->screenPickEvents->entityInfo.meshRef == this->uniformScaling) || (this->screenPickEvents->entityInfo.meshRef == this->xzScale) || (this->screenPickEvents->entityInfo.meshRef == this->yzScale) ? 1.0f : 0.0f;

        if (autoSetVisualControlsTransformOperation)
        {
            int visualControlsOperation = AxEditorEngine::visualControlsTransformOperation_None;
            visualControlsOperation |= (scalingAxes.x != 0) || (scalingAxes.y != 0) || (scalingAxes.z != 0) ? AxEditorEngine::visualControlsTransformOperation_Scaling : AxEditorEngine::visualControlsTransformOperation_None;
            visualControlsOperation |= (rotationAxes.x != 0) || (rotationAxes.y != 0) || (rotationAxes.z != 0) ? AxEditorEngine::visualControlsTransformOperation_Rotation : AxEditorEngine::visualControlsTransformOperation_None;
            visualControlsOperation |= (translationAxes.x != 0) || (translationAxes.y != 0) || (translationAxes.z != 0) ? AxEditorEngine::visualControlsTransformOperation_Translation : AxEditorEngine::visualControlsTransformOperation_None;

            this->SetVisualControlsTransformOperation(visualControlsOperation);
        }

        return true;
    }

    return false;
}

void AxEditorEngine::SetVisualControlsCoordinateSystemMode(int visualControlsMode)
{
    this->visualControlsCoordinateSystemMode = visualControlsMode;
}

void AxEditorEngine::SetVisualControlsTransformOperation(int visualControlsOperation)
{
    this->visualControlsTransformOperation = visualControlsOperation;

    // Change the uniform (central) handler only if a scaling or translation handle is picked, because only translation and scaling have centran handle. That is, leave it as it is when another transformation handle (rotation) is picked
    if (((this->visualControlsTransformOperation & AxEditorEngine::visualControlsTransformOperation_Scaling) != 0) || ((this->visualControlsTransformOperation & AxEditorEngine::visualControlsTransformOperation_Translation) != 0))
    {
        this->uniformTranslation->properties[AxResource::propertyIndex_Enabled]->SetBool(!(this->visualControlsTransformOperation & AxEditorEngine::visualControlsTransformOperation_Scaling) != 0);
        this->uniformScaling->properties[AxResource::propertyIndex_Enabled]->SetBool((this->visualControlsTransformOperation & AxEditorEngine::visualControlsTransformOperation_Scaling) != 0);
    }
}

void AxEditorEngine::CreateThumbnailEntities()
{
    this->thumbnailCamera = new AxCamera(this);

    this->thumbnailCameraTransform = new AxTransform();
    this->thumbnailMeshTransform = new AxTransform();
    this->thumbnailMaterialTransform = new AxTransform();

    this->thumbnailMaterialMesh = new AxMesh(this);
    AxSphereGeometry(*this->thumbnailMaterialMesh->deviceMesh, 100.0f, 16, 16, 0.0f, false);
    this->thumbnailMaterialMesh->ComputeNormals();
    this->thumbnailMaterialMesh->ComputeBounds();
    this->thumbnailMaterialMesh->deviceMesh->UpdateVertices(0, this->thumbnailMaterialMesh->deviceMesh->GetVertexCount());
    this->thumbnailMaterialMesh->deviceMesh->UpdateIndices(0, this->thumbnailMaterialMesh->deviceMesh->GetIndexCount());

    this->thumbnailTextureMesh = new AxMesh(this);
    AxPlaneGeometry(*this->thumbnailTextureMesh->deviceMesh, 1, 1, AxVector3(2.0f, 0.0f, 0.0f), AxVector3(0.0f, 2.0f, 0.0f));
    this->thumbnailTextureMesh->ComputeNormals();
    this->thumbnailTextureMesh->ComputeBounds();
    this->thumbnailTextureMesh->deviceMesh->UpdateVertices(0, this->thumbnailTextureMesh->deviceMesh->GetVertexCount());
    this->thumbnailTextureMesh->deviceMesh->UpdateIndices(0, this->thumbnailTextureMesh->deviceMesh->GetIndexCount());
    this->thumbnailTextureMesh->GetProperty("Cull")->SetBool(false);

    this->thumbnailMeshMaterial = new AxMaterial(this);
    this->thumbnailMeshMaterial->name = "thumbnailMeshMaterial";
    this->thumbnailMeshMaterial->AddShadingLayer(AxShadingOperation_BasicShading, AxColorOperation_Set);
    this->thumbnailMeshMaterial->UpdateShader();

    this->thumbnailTextureMaterial = new AxMaterial(this);
    this->thumbnailTextureMaterial->name = "thumbnailTextureMaterial";
    this->thumbnailTextureMaterial->AddShadingLayer(AxShadingOperation_AlignScreenQuad, AxColorOperation_None);
    this->thumbnailTextureMaterial->AddShadingLayer(AxShadingOperation_ColorMap, AxColorOperation_Set);
    this->thumbnailTextureMaterial->AddShadingLayer(AxShadingOperation_ChangeCulling, AxColorOperation_None);
    this->thumbnailTextureMaterial->UpdateShader();
    this->thumbnailTextureProperty = this->thumbnailTextureMaterial->GetProperty("Color map");


    this->thumbnailMeshSet = new AxEntitySet();
    this->thumbnailMeshSet->references.Add(this->thumbnailCameraTransform);
    this->thumbnailMeshSet->references.Add(this->thumbnailCamera);
    this->thumbnailMeshSet->references.Add(this->thumbnailMeshTransform);
    this->thumbnailMeshSet->references.Add(this->thumbnailMeshMaterial);
    this->thumbnailMeshIndex = this->thumbnailMeshSet->references.count;

    this->thumbnailMaterialSet = new AxEntitySet();
    this->thumbnailMaterialSet->references.Add(this->thumbnailCameraTransform);
    this->thumbnailMaterialSet->references.Add(this->thumbnailCamera);
    this->thumbnailMaterialSet->references.Add(this->thumbnailMaterialTransform);
    this->thumbnailMaterialIndex = this->thumbnailMaterialSet->references.count;
    this->thumbnailMaterialSet->references.Add(this->thumbnailMaterialMesh);

    this->thumbnailTextureSet = new AxEntitySet();
    this->thumbnailTextureSet->references.Add(this->thumbnailCameraTransform);
    this->thumbnailTextureSet->references.Add(this->thumbnailCamera);
    this->thumbnailTextureSet->references.Add(this->thumbnailTextureMaterial);
    this->thumbnailTextureSet->references.Add(this->thumbnailTextureMesh);
}

void AxEditorEngine::FreeThumbnailEntities()
{
    this->FreeCustomEntity((AxResource**)&thumbnailCamera);
    this->FreeCustomEntity((AxResource**)&thumbnailCameraTransform);
    this->FreeCustomEntity((AxResource**)&thumbnailMeshTransform);
    this->FreeCustomEntity((AxResource**)&thumbnailMaterialTransform);
    this->FreeCustomEntity((AxResource**)&thumbnailMaterialMesh);
    this->FreeCustomEntity((AxResource**)&thumbnailTextureMesh);
    this->FreeCustomEntity((AxResource**)&thumbnailMeshMaterial);
    this->FreeCustomEntity((AxResource**)&thumbnailTextureMaterial);
    this->FreeCustomEntity((AxResource**)&thumbnailMeshSet);
    this->FreeCustomEntity((AxResource**)&thumbnailMaterialSet);
    this->FreeCustomEntity((AxResource**)&thumbnailTextureSet);
}

QImage *AxEditorEngine::RenderThumbnailMesh(AxMesh *mesh, int width, int height)
{
    float distance = AxMaths::DistanceFromSizeAndAngle(mesh->bounds.sphereRadius * 2.0f, this->thumbnailCamera->properties[AxCamera::propertyIndex_FOV]->GetFloat());
    AxMatrix::CreateIdentity(this->thumbnailCameraTransform->transform);
    this->thumbnailCameraTransform->SetPositionExtrinsic(mesh->bounds.center.Add(AxVector3(0.0f, 0.0f, -1.0f).Normalize().Scale(distance)));

    AxEntitySet *originalRoot = this->root;
    this->root = this->thumbnailMeshSet;
    this->thumbnailMeshSet->references.Insert(this->thumbnailMeshIndex, mesh);

    AxImage *image = this->TakeScreenshot(width, height);

    this->thumbnailMeshSet->references.RemoveAt(this->thumbnailMeshIndex);
    this->root = originalRoot;

    QImage *result = EditorUtils::AxToQImage(image);
    delete image;

    return result;
}

QImage *AxEditorEngine::RenderThumbnailMaterial(AxMaterial *material, int width, int height)
{
    float distance = AxMaths::DistanceFromSizeAndAngle(this->thumbnailMaterialMesh->bounds.sphereRadius * 2.0f, this->thumbnailCamera->properties[AxCamera::propertyIndex_FOV]->GetFloat());
    this->thumbnailCameraTransform->SetPositionExtrinsic(this->thumbnailMaterialMesh->bounds.center.Add(AxVector3(0.0f, 0.0f, -1.0f).Normalize().Scale(distance)));

    AxEntitySet *originalRoot = this->root;
    this->root = this->thumbnailMaterialSet;
    this->thumbnailMaterialSet->references.Insert(this->thumbnailMaterialIndex, material);

    AxImage *image = this->TakeScreenshot(width, height);

    this->thumbnailMaterialSet->references.RemoveAt(this->thumbnailMaterialIndex);
    this->root = originalRoot;

    QImage *result = EditorUtils::AxToQImage(image);
    delete image;

    return result;
}

QImage *AxEditorEngine::RenderThumbnailTexture(AxTexture2D *texture, int width, int height)
{
    //return 0;

    AxEntitySet *originalRoot = this->root;
    this->root = this->thumbnailTextureSet;
    this->thumbnailTextureProperty->SetValue(texture, AxParameterType_ReferenceTexture);

    AxImage *image = this->TakeScreenshot(width, height);

    this->thumbnailTextureProperty->SetValue(0, AxParameterType_ReferenceTexture);
    this->root = originalRoot;

    QImage *result = EditorUtils::AxToQImage(image);
    delete image;

    return result;


//    if ((texture->properties[AxTexture2D::propertyIndex_Width]->GetInt() == 0) || (texture->properties[AxTexture2D::propertyIndex_Height]->GetInt() == 0))
//        return 0;

//    AxImage *image = texture->ToImage();
//    if ((image->width == 0) || (image->height == 0))
//    {
//        delete image;
//        return 0;
//    }

//    AxImage *resultImage = new AxImage();
//    resultImage->SetPixelFormat(image->pixelFormat);
//    resultImage->SetSize(width, height, 1);

//    for (int y = 0; y < height; y++)
//    {
//        float v = (float)y / (float)(height - 1);
//        for (int x = 0; x < width; x++)
//        {
//            float u = (float)x / (float)(width - 1);
//            resultImage->SetPixel(x, y, 0, image->GetSample(u, v, (int)0));
//        }
//    }

//    QImage *result = EditorUtils::AxToQImage(resultImage);
//    delete resultImage;
//    delete image;

//    return result;
}
