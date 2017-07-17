//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxResourcesWidget.h"

#include "QAxResourceItemWidget.h"

QAxResourcesWidget::QAxResourcesWidget(QWidget *parent) :
    QWidget(parent)
{
    this->layout = new QGridLayout(this);
    this->layout->setSpacing(4);

    this->items = 0;
    this->filterType = (AxResourceType)-1;
    this->filterString = "";
}

void QAxResourcesWidget::SetItemSource(AxList<AxResource*> &items)
{
    this->items = &items;
    this->RefreshItems();
}

void QAxResourcesWidget::SetFilterType(AxResourceType filterType)
{
    this->filterType = filterType;
    this->RefreshItems();
}

void QAxResourcesWidget::SetFilterString(const AxString &filterString)
{
    this->filterString = filterString;
    this->RefreshItems();
}

void QAxResourcesWidget::RefreshItems()
{
    const int itemWidth = 64;
    int ml, mr, mt, mb;
    this->layout->getContentsMargins(&ml, &mr, &mt, &mb);
    int clientWidth = this->width() - ml - mr;
    int hSpacing = this->layout->horizontalSpacing();
    int numHorizontalItems = (clientWidth + hSpacing) / (itemWidth + hSpacing);

    QLayoutItem *child;
    while ((child = this->layout->takeAt(0)) != 0)
    {
        delete child->widget();
        delete child;
    }

    if (this->items == 0)
        return;

    int column = 0;
    int row = 0;
    for (int i = 0; i < this->items->count; i++)
    {
        AxResource *item = (*(this->items))[i];
        if ((((int)this->filterType == -1) || (this->filterType == item->resourceType)) && (item->resourceType != AxResourceType_EntitySet))
        {
            if ((this->filterString.length == 0) || (item->name.IndexOf(this->filterString) != -1))
            {
                QAxResourceItemWidget *widget = new QAxResourceItemWidget();
                this->layout->addWidget(widget, row, column);

                widget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
                widget->setMinimumSize(74, 74);
                widget->setMaximumSize(74, 74);
                widget->SetEditorInterface(this->editor);
                widget->SetResource(item);


                column++;
                if (column >= numHorizontalItems)
                {
                    column = 0;
                    row++;
                }
            }
        }
    }
}

void QAxResourcesWidget::RefreshItem(AxResource *resource)
{
}
