//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "resourcepropertiesdialog.h"
#include "ui_resourcepropertiesdialog.h"

ResourcePropertiesDialog::ResourcePropertiesDialog(QWidget *parent, IAxEditor *editor, AxResource *resource) :
    QDialog(parent),
    ui(new Ui::ResourcePropertiesDialog)
{
    ui->setupUi(this);

    this->ui->wProperties->SetEditorInterface(editor);
    this->SetResource(resource);
}

ResourcePropertiesDialog::~ResourcePropertiesDialog()
{
    delete ui;
}

void ResourcePropertiesDialog::SetResource(AxResource *resource)
{
    this->ui->wProperties->SetResource(resource);
}
