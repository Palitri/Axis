#ifndef IMWEDITOR_H
#define IMWEDITOR_H

#include "../AxisEngine.h"

#include "Source/Engine/AxEditorEngine.h"

class IAxEditor
{
public:
    virtual void SelectResource(AxTraceParameters *resourceParameters) = 0;
    virtual void SelectResource(AxResource *resource) = 0;
    virtual AxResource *GetSelectedResource() = 0;

    virtual void SelectTransform(AxTraceParameters *parameters) = 0;
    virtual AxTraceParameters *GetSelectedTransform() = 0;

    virtual void UpdateResourceName(AxResource *resource) = 0;
    virtual void UpdateResourceProperties(AxResource *resource) = 0;

    //virtual bool IsKeyDown(int keyCode) = 0;

    virtual AxEditorEngine *GetAxis() = 0;

    virtual void OnTransformationChanged(int operation, AxVector3 transformationParameters) = 0;
};

#endif // IMWEDITOR_H
