#ifndef COMPONENTSDIALOG_H
#define COMPONENTSDIALOG_H

#include "../AxisEngine.h"

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class ComponentsDialog;
}

class ComponentsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ComponentsDialog(QWidget *parent, Axis *axis);
    ~ComponentsDialog();

    // Method for the event filtering of the Components list's drop event
    bool eventFilter(QObject *object, QEvent *event);

private slots:
    void on_lComponents_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_cbComponentTypes_currentIndexChanged(const QString &arg1);

private:
    Ui::ComponentsDialog *ui;

    Axis *axis;

    // Methods
    AxList<AxModuleDispatcher*> *GetDispatchers(unsigned int typeId);
    unsigned int GetSelectedComponentType();
    AxModuleDispatcher* GetSelectedComponent();
    int GetSelectedComponentIndex();

    void FillModules();
    void FillComponentTypes();
    void AddComponentFeature(AxModuleDispatcher* dispatcher, QString featureName, unsigned int featureInfoId);
    void AddComponentSerializationFeature(AxModuleDispatcher* dispatcher, QString featureName, unsigned int formatExtensionInfoId, unsigned int formatDescriptionInfoId);
    void FillComponents(AxList<AxModuleDispatcher *>* dispatchers);
    void FillComponentFeatures(AxModuleDispatcher* dispatcher);
    void HighlightComponentModule(AxModuleDispatcher* dispatcher);
};

#endif // COMPONENTSDIALOG_H
