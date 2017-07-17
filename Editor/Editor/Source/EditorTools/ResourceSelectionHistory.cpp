//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "ResourceSelectionHistory.h"

ResourceSelectionHistory::ResourceSelectionHistory(int capacity)
{
    this->capacity = capacity;

    this->selectedIndex = -1;
}

ResourceSelectionHistory::~ResourceSelectionHistory()
{
}

void ResourceSelectionHistory::Clear()
{
    this->history.Clear();

    this->selectedIndex = -1;
}

void ResourceSelectionHistory::PushHistory(AxResource *selection)
{
    if (selection == 0)
        return;

    if (this->selectedIndex != -1)
        if (this->history[selectedIndex] == selection)
            return;

    this->history.RemoveRange(selectedIndex + 1, history.count - selectedIndex - 1);
    this->history.Add(selection);
    this->history.RemoveRange(0, AxMath::Max(history.count - this->capacity, 0));

    this->selectedIndex = this->history.count - 1;
}

AxResource *ResourceSelectionHistory::Back()
{
    if (this->selectedIndex > 0)
    {
        this->selectedIndex--;
        return this->history[this->selectedIndex];
    }
    else
        return 0;
}

AxResource *ResourceSelectionHistory::Forward()
{
    if (this->selectedIndex < this->history.count - 1)
    {
        this->selectedIndex++;
        return this->history[this->selectedIndex];
    }
    else
        return 0;
}
