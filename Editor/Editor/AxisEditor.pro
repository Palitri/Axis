#-------------------------------------------------
#
# Project created by QtCreator 2014-04-02T20:00:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = "Axis Editor"
TEMPLATE = app

SOURCES += Source/main.cpp \
    Source/mainwindow.cpp \
    Source/EditorTools/EditorUtils.cpp \
    Source/EditorTools/ResourceSelectionHistory.cpp \
    Source/Dialogs/componentsdialog.cpp \
    Source/Engine/AxEditorEngine.cpp \
    Source/Engine/AxEditorRenderEvents.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidget.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetBool.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetColor.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetColorAlpha.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetColorRgb.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetFloat.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetInt.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetLink.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetLinkButton.cpp \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetLinkRecall.cpp \
    Source/QAxWidgets/QAxEditorWidget.cpp \
    Source/QAxWidgets/QAxMaterialLayersWidget.cpp \
    Source/QAxWidgets/QAxPropertiesResourceWidget.cpp \
    Source/QAxWidgets/QAxRenderWidget.cpp \
    Source/QAxWidgets/QAxResourceItemButtonWidget.cpp \
    Source/QAxWidgets/QAxResourceItemWidget.cpp \
    Source/QAxWidgets/QAxResourcePropertiesWidget.cpp \
    Source/QAxWidgets/QAxResourcesWidget.cpp \
    Source/QAxWidgets/QAxScriptCodeWidget.cpp \
    Source/QAxWidgets/QAxScriptPropertiesWidget.cpp \
    Source/QAxWidgets/QAxStructuresWidget.cpp \
    Source/QAxWidgets/QAxTransformLayersWidget.cpp \
    Source/Dialogs/resourcepropertiesdialog.cpp

HEADERS  += Source/mainwindow.h \
    Source/EditorTools/EditorUtils.h \
    Source/EditorTools/ResourceSelectionHistory.h \
    Source/AxisEngine.h \
    Source/Dialogs/componentsdialog.h \
    Source/Engine/AxEditorEngine.h \
    Source/Engine/AxEditorRenderEvents.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidget.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetBool.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetColor.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetColorAlpha.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetColorRgb.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetFloat.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetInt.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetLink.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetLinkButton.h \
    Source/QAxWidgets/ParameterWidgets/QAxParameterWidgetLinkRecall.h \
    Source/QAxWidgets/IAxEditor.h \
    Source/QAxWidgets/QAxEditorWidget.h \
    Source/QAxWidgets/QAxMaterialLayersWidget.h \
    Source/QAxWidgets/QAxPropertiesResourceWidget.h \
    Source/QAxWidgets/QAxRenderWidget.h \
    Source/QAxWidgets/QAxResourceItemButtonWidget.h \
    Source/QAxWidgets/QAxResourceItemWidget.h \
    Source/QAxWidgets/QAxResourcePropertiesWidget.h \
    Source/QAxWidgets/QAxResourcesWidget.h \
    Source/QAxWidgets/QAxScriptCodeWidget.h \
    Source/QAxWidgets/QAxScriptPropertiesWidget.h \
    Source/QAxWidgets/QAxStructuresWidget.h \
    Source/QAxWidgets/QAxTransformLayersWidget.h \
    Source/VersionInfo.h \
    Source/Dialogs/resourcepropertiesdialog.h

FORMS    += Source/mainwindow.ui \
    Source/Dialogs/componentsdialog.ui \
    Source/Dialogs/resourcepropertiesdialog.ui

RESOURCES += \
    AxisResources.qrc

RC_FILE = AxisEditor.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../Engine/Release/ -lAxis
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../Engine/Debug/ -lAxis
else:unix: LIBS += -L$$PWD/../../Engine/ -lAxis

DISTFILES += \
    AxisEditor.rc
