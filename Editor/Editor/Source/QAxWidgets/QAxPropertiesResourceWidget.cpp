//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "QAxPropertiesResourceWidget.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../EditorTools/EditorUtils.h"

QAxPropertiesResourceWidget::QAxPropertiesResourceWidget(QWidget *parent) :
    QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->setContentsMargins(0, 0, 0, 10);

    this->nameIcon = new QLabel(this);
    nameLayout->addWidget(this->nameIcon);

    this->nameEdit = new QLineEdit(this);
    QObject::connect(this->nameEdit, SIGNAL(textChanged(QString)), this, SLOT(on_nameEdit_textChanged(QString)));
    QObject::connect(this->nameEdit, SIGNAL(editingFinished()), this, SLOT(on_nameEdit_editingFinished()));
    nameLayout->addWidget(this->nameEdit);

    layout->addLayout(nameLayout);

    this->propertiesWidget = new QAxResourcePropertiesWidget();
    layout->addWidget(this->propertiesWidget);

    //layout->addStretch();

    this->nameEdit->setEnabled(false);

    this->resource = 0;
}

void QAxPropertiesResourceWidget::SetEditorInterface(IAxEditor *editor)
{
    QAxEditorWidget::SetEditorInterface(editor);

    this->propertiesWidget->SetEditorInterface(this->editor);
}

void QAxPropertiesResourceWidget::SetResource(AxResource *resource)
{
    this->resource = resource;

    this->propertiesWidget->SetResource(this->resource);

    this->nameEdit->setEnabled(this->resource != 0);

    if (resource == 0)
    {
        this->nameIcon->setPixmap(QPixmap());
        this->nameEdit->setText("");
    }
    else
    {
        QString resourceIconName;
        if (EditorUtils::resourceTypeIcons.GetValue(this->resource->resourceType, resourceIconName))
        {
            this->nameIcon->setPixmap(QPixmap(resourceIconName));
        }

        this->nameEdit->setText(this->resource->name.GetCharContents());
    }
}

bool QAxPropertiesResourceWidget::CheckNameValidity(const AxString &name)
{
    if (this->editor == 0)
        return true;

    return !this->editor->GetAxis()->FindResourceX(name, this->resource->resourceType);
}

void QAxPropertiesResourceWidget::on_nameEdit_textChanged(const QString &arg1)
{
    if (this->resource == 0)
    {
        this->nameEdit->setStyleSheet("");
        return;
    }

    if (this->resource->name.Equals(EditorUtils::QToAxString(arg1)))
    {
        this->nameEdit->setStyleSheet("");
        return;
    }

    if (this->CheckNameValidity(EditorUtils::QToAxString(arg1)))
        this->nameEdit->setStyleSheet("color:green;");
    else
        this->nameEdit->setStyleSheet("color:red;");
}

void QAxPropertiesResourceWidget::on_nameEdit_editingFinished()
{
    this->nameEdit->setStyleSheet("");

    if (this->resource == 0)
        return;

    AxString newName = EditorUtils::QToAxString(this->nameEdit->text());
    if (!this->CheckNameValidity(newName))
    {
        this->nameEdit->setText(EditorUtils::AxToQString(this->resource->name));
        return;
    }

    this->resource->name = newName;

    if (this->editor != 0)
        this->editor->UpdateResourceName(this->resource);
}
