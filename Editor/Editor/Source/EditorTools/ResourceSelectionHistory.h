#ifndef RESOURCESELECTIONHISTORY_H
#define RESOURCESELECTIONHISTORY_H

#include "EditorUtils.h"

class ResourceSelectionHistory
{
public:
    AxList<AxResource*> history;
    int selectedIndex, capacity;

    ResourceSelectionHistory(int capacity = 20);
    ~ResourceSelectionHistory();

    void Clear();

    void PushHistory(AxResource *selection);

    AxResource *Back();
    AxResource *Forward();
};

#endif // RESOURCESELECTIONHISTORY_H
