#ifndef RESOURCEPROPERTIESDIALOG_H
#define RESOURCEPROPERTIESDIALOG_H

#include "../AxisEngine.h"

#include <QDialog>

#include "../QAxWidgets/IAxEditor.h"

namespace Ui {
class ResourcePropertiesDialog;
}

class ResourcePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResourcePropertiesDialog(QWidget *parent, IAxEditor *editor, AxResource *resource = 0);
    ~ResourcePropertiesDialog();

    void SetResource(AxResource *resource);

private:
    Ui::ResourcePropertiesDialog *ui;

private slots:

};

#endif // RESOURCEPROPERTIESDIALOG_H
