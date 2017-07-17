#ifndef MWEDITORENGINE_H
#define MWEDITORENGINE_H

#include "Source/AxisEngine.h"

#include <QImage>

class AxEditorEngine
        : public Axis
{
private:
    AxTransform *axesTransform;
    AxMesh *xAxis, *yAxis, *zAxis, *xyAxis, *xzAxis, *yzAxis, *xyScale, *xzScale, *yzScale, *xTorus, *yTorus, *zTorus, *xScaling, *yScaling, *zScaling, *uniformTranslation, *uniformScaling;
    AxMaterial *xAxisMaterial, *xAxisRotationMaterial, *yAxisMaterial, *yAxisRotationMaterial, *zAxisMaterial, *zAxisRotationMaterial, *uniformTransformMaterial, *xAxisScalingMaterial, *yAxisScalingMaterial, *zAxisScalingMaterial, *xyAxisMaterial, *xzAxisMaterial, *yzAxisMaterial, *xyScalingMaterial, *xzScalingMaterial, *yzScalingMaterial;
    AxEntitySet *axesWrapper, *axesSet;

    AxMatrix savedWorldMatrix;

    AxCamera *thumbnailCamera;
    AxTransform *thumbnailCameraTransform, *thumbnailMeshTransform, *thumbnailMaterialTransform;
    AxMesh *thumbnailMaterialMesh, *thumbnailTextureMesh;
    AxMaterial *thumbnailMeshMaterial, *thumbnailTextureMaterial;
    AxEntitySet *thumbnailMaterialSet, *thumbnailMeshSet, *thumbnailTextureSet;
    AxProperty *thumbnailTextureProperty;
    int thumbnailMeshIndex, thumbnailMaterialIndex;


    void FreeCustomEntity(AxResource **entity);

    void CreateVisualControls();
    void FreeVisualControls();

    void EnableVisualControlsMode(AxTraceParameters *selectedResourceParameters);
    void DisableVisualControlsMode(AxTraceParameters *selectedResourceParameters);

    void CreateThumbnailEntities();
    void FreeThumbnailEntities();

public:
    static const int visualControlsCoordinateSystemMode_Extrinsic    = 1;
    static const int visualControlsCoordinateSystemMode_Intrinsic    = 2;

    static const int visualControlsTransformOperation_None         = 0;
    static const int visualControlsTransformOperation_Translation  = 1;
    static const int visualControlsTransformOperation_Rotation     = 2;
    static const int visualControlsTransformOperation_Scaling      = 4;

    int visualControlsCoordinateSystemMode;
    int visualControlsTransformOperation;

    AxTraceParameters camera;

    AxEditorEngine(void *windowHandle);
    ~AxEditorEngine();



    // Inherited methods

    void SetGraphicsDevice(AxGraphicsDevice *graphicsDevice);

    void ProcessEntity(AxResource *entity, AxTraceParameters &parameters);

    void Reset();


    // Custom methods

    void RenderVisualControls(AxTraceParameters *selectedResourceParameters);

    void HighlightVisualControl(AxMaterial *material);

    bool PickVisualControlFromScreen(AxVector2 &screenUnitCoords, AxTraceParameters *selectedResourceParameters, AxVector3 &translationAxes, AxVector3 &rotationAxes, AxVector3 &scalingAxes, bool autoSetVisualControlsTransformOperation, bool autoHighlightVisualControl);

    void SetVisualControlsCoordinateSystemMode(int visualControlsCoordinateSystemMode);
    void SetVisualControlsTransformOperation(int visualControlsTransformOperation);

    QImage *RenderThumbnailMesh(AxMesh *mesh, int width, int height);
    QImage *RenderThumbnailMaterial(AxMaterial *material, int width, int height);
    QImage *RenderThumbnailTexture(AxTexture2D *texture, int width, int height);
};

#endif // MWEDITORENGINE_H
