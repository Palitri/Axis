#ifndef EDITORUTILS_H
#define EDITORUTILS_H

#include <QString>
#include <QColor>
#include <QDrag>
#include <QMessageBox>

#include "../AxisEngine.h"


#include "../QAxWidgets/IAxEditor.h"

class EditorUtils
{
private:
    class Initialization
    {
    public:
        Initialization(void);
    };

    static Initialization initialization;

public:
    static QString iconRecallParameter;
    static QString iconPlay;
    static QString iconPause;
    static QString iconNew;
    static QString iconDelete;
    static QString iconInsert;
    static QString iconMoveUp;
    static QString iconMoveDown;
    static QString iconScriptProperties;

    static AxDictionary<AxResourceType, QString> resourceTypeIcons;
    static AxDictionary<AxShadingOperation, QString> ShadingOperationNames;
    static AxDictionary<AxColorOperation, QString> colorOperationNames;
    static AxDictionary<AxTransformOperation, QString> transformOperationNames;
    static AxDictionary<AxParameterType, QString> parameterTypeNames;

    EditorUtils();

    static QString Vector4ToHtmlColor(AxVector4 &vector, bool withAlpha = false);
    static QColor Vector4ToQColor(AxVector4 &vector);
    static void QColorToVector4(const QColor &color, AxVector4 &vector, bool withAlpha);

    static QDrag *CreateObjectDrag(QObject *dragSource, void *objectData, QString mimeType, QString dragIconFileName = 0);
    static bool GetObjectDrag(void **result, const QMimeData *mimeData, QString mimeType);
    static QDrag *CreateResourceDrag(QObject *dragSource, AxResource *resource);
    static bool GetResourceDrag(AxResource **resource, const QMimeData *mimeData);

    static AxString QToAxString(const QString &string);
    static QString AxToQString(const AxString &string);

    static int MessageDialog(QString text, QMessageBox::StandardButton buttonAction, QMessageBox::StandardButton buttonCancel = QMessageBox::NoButton, QMessageBox::StandardButtons otherButtons = QMessageBox::NoButton);
    static int MessageDialogWithCheckbox(QString text, QString checkBoxText, bool &checkBoxResult, QMessageBox::StandardButton buttonAction, QMessageBox::StandardButton buttonCancel = QMessageBox::NoButton, QMessageBox::StandardButtons otherButtons = QMessageBox::NoButton);

    static QImage *AxToQImage(AxImage *image);

    static bool RegisterFileTypes();

    static AxString QToAxFilePath(const AxString &qPath);
};

#endif // EDITORUTILS_H
