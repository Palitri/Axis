#ifndef MWEDITORRENDEREVENTS_H
#define MWEDITORRENDEREVENTS_H

#include "Source/AxisEngine.h"

class AxEditorRenderEvents
        : public AxRenderEvents
{
private:
    AxEntitySet *axesSet;

public:
    AxEditorRenderEvents(Axis &axis);

    virtual void OnTransform(AxTraceParameters &parameters);

};

#endif // MWEDITORRENDEREVENTS_H
