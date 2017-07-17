//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "EditorUtils.h"

#include <QMimeData>
#include <QPixmap>
#include <QCheckBox>

#include <QSettings>
#include <QProcess>

QString EditorUtils::iconRecallParameter = QString(":/Icons/Icons/Manipulate/RecallParameter.png");
QString EditorUtils::iconPlay = QString(":/Icons/Icons/Manipulate/Play.png");
QString EditorUtils::iconPause = QString(":/Icons/Icons/Manipulate/Pause.png");
QString EditorUtils::iconNew = QString(":/Icons/Icons/Manipulate/New.png");
QString EditorUtils::iconDelete = QString(":/Icons/Icons/Manipulate/Delete.png");
QString EditorUtils::iconInsert = QString(":/Icons/Icons/Manipulate/Insert.png");
QString EditorUtils::iconMoveUp = QString(":/Icons/Icons/Manipulate/MoveUp.png");
QString EditorUtils::iconMoveDown = QString(":/Icons/Icons/Manipulate/MoveDown.png");
QString EditorUtils::iconScriptProperties = QString(":/Icons/Icons/Other/ScriptProperties.png");

AxDictionary<AxResourceType, QString> EditorUtils::resourceTypeIcons;
AxDictionary<AxShadingOperation, QString> EditorUtils::ShadingOperationNames;
AxDictionary<AxColorOperation, QString> EditorUtils::colorOperationNames;
AxDictionary<AxTransformOperation, QString> EditorUtils::transformOperationNames;
AxDictionary<AxParameterType, QString> EditorUtils::parameterTypeNames;

EditorUtils::Initialization EditorUtils::initialization;

EditorUtils::Initialization::Initialization(void)
{
    // Map resource types to their respective icon names
    EditorUtils::resourceTypeIcons.Add(AxResourceType_EntitySet, ":/Icons/Icons/Resources/EntitySet.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Mesh, ":/Icons/Icons/Resources/Mesh.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Transform, ":/Icons/Icons/Resources/Transform.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Material, ":/Icons/Icons/Resources/Material.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Mechanism, ":/Icons/Icons/Resources/Mechanism.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Camera, ":/Icons/Icons/Resources/Camera.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Texture, ":/Icons/Icons/Resources/Texture.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Light, ":/Icons/Icons/Resources/Light.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Sound, ":/Icons/Icons/Resources/Sound.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_SoundEmitter, ":/Icons/Icons/Resources/SoundEmitter.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_SoundReceptor, ":/Icons/Icons/Resources/SoundReceptor.png");
    EditorUtils::resourceTypeIcons.Add(AxResourceType_Script, ":/Icons/Icons/Resources/Script.png");

    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_None, "None");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_BasicShading, "Basic Shading");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_FlatColor, "Flat Color");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_Tint, "Tint");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_AmbientLighting, "Ambient Lighting");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_LightSource, "Light Source");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_VertexLighting, "Vertex Lighting");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_PixelLighting, "Pixel Lighting");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ColorMap, "Color Map");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ReflectionMap, "Reflection Map");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_RefractionMap, "Refraction Map");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_NormalMap, "Normal Map");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_BlurMap, "Blur Map");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_HBlur, "Horizontal Blur");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_VBlur, "Vertical Blur");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_AlignScreenQuad, "Align Screen Quad");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ProjectTexture, "Project Texture");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ProjectPicture, "Project Picture");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ShadowMapWriter, "Shadow Map Writer");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ShadowMap, "Shadow Map");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_TextureTransform, "Texture Transform");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_Skinning, "Skinning");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_Material, "Material");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_Fog, "Fog");
    EditorUtils::ShadingOperationNames.Add(AxShadingOperation_ChangeCulling, "Change Culling");

    EditorUtils::colorOperationNames.Add(AxColorOperation_None, "None");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Set, "Set");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Blend, "Blend");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Add, "Add");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Subtract, "Subtract");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Modulate, "Modulate");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Lighter, "Lighter");
    EditorUtils::colorOperationNames.Add(AxColorOperation_Darker, "Darker");
    EditorUtils::colorOperationNames.Add(AxColorOperation_AlphaBlend, "Alpha Blend");
    EditorUtils::colorOperationNames.Add(AxColorOperation_AlphaAdd, "Alpha Add");
    EditorUtils::colorOperationNames.Add(AxColorOperation_AlphaSubtract, "Alpha Subtract");
    EditorUtils::colorOperationNames.Add(AxColorOperation_AlphaModulate, "Alpha Modulate");

    EditorUtils::transformOperationNames.Add(AxTransformOperation_Translate, "Translate");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_RotateX, "RotateX");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_RotateY, "RotateY");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_RotateZ, "RotateZ");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_Scale, "Scale");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_TranslateAbsolute, "TranslateAbsolute");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_RotateAbsoluteX, "RotateAbsoluteX");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_RotateAbsoluteY, "RotateAbsoluteY");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_RotateAbsoluteZ, "RotateAbsoluteZ");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_ScaleAbsolute, "ScaleAbsolute");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_Reflect, "Reflect");
    EditorUtils::transformOperationNames.Add(AxTransformOperation_Skybox, "Skybox");

    EditorUtils::parameterTypeNames.Add(AxParameterType_None, "None");
    EditorUtils::parameterTypeNames.Add(AxParameterType_Int, "Int");
    EditorUtils::parameterTypeNames.Add(AxParameterType_Float, "Float");
    EditorUtils::parameterTypeNames.Add(AxParameterType_Bool, "Bool");
    EditorUtils::parameterTypeNames.Add(AxParameterType_Color, "Color");
    EditorUtils::parameterTypeNames.Add(AxParameterType_ReferenceVector3, "Vector3");
    EditorUtils::parameterTypeNames.Add(AxParameterType_ReferenceTexture, "Texture");
    EditorUtils::parameterTypeNames.Add(AxParameterType_ReferenceCamera, "Camera");
    EditorUtils::parameterTypeNames.Add(AxParameterType_ReferenceMechanism, "Mecanism");
    EditorUtils::parameterTypeNames.Add(AxParameterType_ReferenceTransform, "Transform");
    EditorUtils::parameterTypeNames.Add(AxParameterType_ReferenceSound, "Sound");
}

EditorUtils::EditorUtils()
{
}

QString EditorUtils::Vector4ToHtmlColor(AxVector4 &vector, bool withAlpha)
{
    unsigned int value = AxMath::Trim((int)(0xff * vector.x / 1.0f), 0x00, 0xff);
    value <<= 8;
    value |= AxMath::Trim((int)(0xff * vector.y / 1.0f), 0x00, 0xff);
    value <<= 8;
    value |= AxMath::Trim((int)(0xff * vector.z / 1.0f), 0x00, 0xff);

    if (withAlpha)
    {
        value <<= 8;
        value |= AxMath::Trim((int)(0xff * vector.w / 1.0f), 0x00, 0xff);

        return QString(AxString(value, 16).PadLeft(8, '0').GetCharContents());
    }
    else
        return QString(AxString(value, 16).PadLeft(6, '0').GetCharContents());
}

QColor EditorUtils::Vector4ToQColor(AxVector4 &vector)
{
    return QColor(AxMath::Trim((int)(0xff * vector.x / 1.0f), 0x00, 0xff),
                  AxMath::Trim((int)(0xff * vector.y / 1.0f), 0x00, 0xff),
                  AxMath::Trim((int)(0xff * vector.z / 1.0f), 0x00, 0xff),
                  AxMath::Trim((int)(0xff * vector.w / 1.0f), 0x00, 0xff));
}

void EditorUtils::QColorToVector4(const QColor &color, AxVector4 &vector, bool withAlpha)
{
    vector.x = (float)color.redF();
    vector.y = (float)color.greenF();
    vector.z = (float)color.blueF();
    if (withAlpha)
        vector.w = (float)color.alphaF();
}

QDrag *EditorUtils::CreateObjectDrag(QObject *dragSource, void *objectData, QString mimeType, QString dragIconFileName)
{
    QDrag *drag = new QDrag(dragSource);

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType, QByteArray(AxString((unsigned int)objectData).GetCharContents()));
    drag->setMimeData(mimeData);

    if (dragIconFileName != 0 && !dragIconFileName.isEmpty())
        drag->setPixmap(QPixmap(dragIconFileName));

    return drag;
}

bool EditorUtils::GetObjectDrag(void **result, const QMimeData *mimeData, QString mimeType)
{
    if (!mimeData->hasFormat(mimeType))
        return false;

    unsigned int object;
    if (!AxString(mimeData->data(mimeType).data()).ToUInt(object))
        return false;

    *result = (void*)object;
    return true;
}

QDrag *EditorUtils::CreateResourceDrag(QObject *dragSource, AxResource *resource)
{
    QString iconName;
    EditorUtils::resourceTypeIcons.GetValue(resource->resourceType, iconName);

    return EditorUtils::CreateObjectDrag(dragSource, resource, "application/mwresource", iconName);
}

bool EditorUtils::GetResourceDrag(AxResource **resource, const QMimeData *mimeData)
{
    return EditorUtils::GetObjectDrag((void**)resource, mimeData, "application/mwresource");
}

AxString EditorUtils::QToAxString(const QString &string)
{
    return AxString(string.toStdWString().c_str());
}


QString EditorUtils::AxToQString(const AxString &string)
{
    return QString::fromWCharArray(string.contents);
}

int EditorUtils::MessageDialog(QString text, QMessageBox::StandardButton buttonAction, QMessageBox::StandardButton buttonCancel, QMessageBox::StandardButtons otherButtons)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setStandardButtons(buttonAction | buttonCancel | otherButtons);
    msgBox.setDefaultButton(buttonAction);
    msgBox.setEscapeButton(buttonCancel);
    return msgBox.exec();
}

int EditorUtils::MessageDialogWithCheckbox(QString text, QString checkBoxText, bool &checkBoxResult, QMessageBox::StandardButton buttonAction, QMessageBox::StandardButton buttonCancel, QMessageBox::StandardButtons otherButtons)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setStandardButtons(buttonAction | buttonCancel | otherButtons);
    msgBox.setDefaultButton(buttonAction);
    msgBox.setEscapeButton(buttonCancel);

    QCheckBox checkBox;
    checkBox.setText(checkBoxText);
    msgBox.setCheckBox(&checkBox);

    int result = msgBox.exec();
    checkBoxResult = checkBox.isChecked();
    return result;
}

QImage *EditorUtils::AxToQImage(AxImage *image)
{
    AxPixelFormat qtPixelFormat(4, 4, 8, 8, 8, 8, AxPixelFormat::ChannelIdBlue, AxPixelFormat::ChannelIdGreen, AxPixelFormat::ChannelIdRed, AxPixelFormat::ChannelIdAlpha);
    QImage *result = new QImage(image->width, image->height, QImage::Format_ARGB32);
    unsigned int color[4];
    for (int y = 0; y < image->height; y++)
    {
        for (int x = 0; x < image->width; x++)
        {
            image->pixelFormat.GetRGBAValues(color, image->GetPixel(x, image->height - 1 - y, 0));
            int pixel = qtPixelFormat.SetRGBAValues(color);
            result->setPixel(x, y, pixel);
        }
    }
    return result;
}

bool EditorUtils::RegisterFileTypes()
{
    AxString appFullName = AxPlatformUtils::GetApplicationName();
    AxString appPath = AxFileSystem::GetFileDir(appFullName);

    QSettings settings("HKEY_CLASSES_ROOT", QSettings::NativeFormat);

    settings.setValue(".axs/Default", "Axis.axs");
    settings.setValue("Axis.axs/Default", "Axis Scene");
    settings.setValue("Axis.axs/DefaultIcon/Default", EditorUtils::AxToQString(appFullName.Replace("/", "\\") + ",0"));
    settings.setValue("Axis.axs/shell/open/command/Default", EditorUtils::AxToQString(AxString("\"") + appFullName.Replace("/", "\\") + "\" \"%1\""));

    settings.setValue(".axi/Default", "Axis.axi");
    settings.setValue("Axis.axi/Default", "Axis Image");
    settings.setValue("Axis.axi/DefaultIcon/Default", EditorUtils::AxToQString(appFullName.Replace("/", "\\") + ",1"));

    settings.sync();

#ifdef QT_DEBUG
    AxString thumbnailHandlerRelativePath = "../../../Windows Thumbnail Providers/x64/Debug/AxisThumbnailProviders.dll";
#else
    AxString thumbnailHandlerRelativePath = "Thumbnail providers/AxisThumbnailProviders.dll";
#endif

    AxString thumbnailHanderFullName = AxFileSystem::MergePaths(appPath, thumbnailHandlerRelativePath);

    QProcess process;
    process.start("regsvr32", QStringList() << EditorUtils::AxToQString(thumbnailHanderFullName.Replace("/", "\\")) << "/s");
    process.waitForFinished();
    process.close();

    return true;
}

AxString EditorUtils::QToAxFilePath(const AxString &qPath)
{
    return AxString(qPath.contents);
}
