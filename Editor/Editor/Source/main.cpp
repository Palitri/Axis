//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//	Axis 3D Engine Editor Copyright (C) 2017  Palitri
//
//	This program is released under the MIT License. You are free to use, distribute and modify this code as you see fit. For more info, please read the MIT License
//
//	For help and documentation on the editor and the engine, visit <http://palitri.com>
//----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#include "mainwindow.h"
#include "VersionInfo.h"
#include <QApplication>

#include "AxisEngine.h"
#include "EditorTools/EditorUtils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Needed to save window, docking, toolbars states and etc.
    QCoreApplication::setOrganizationDomain(VER_COMPANYDOMAIN_STR);
    QCoreApplication::setOrganizationName(VER_COMPANYNAME_STR);
    QCoreApplication::setApplicationName(VER_FILEDESCRIPTION_STR);
    QCoreApplication::setApplicationVersion(VER_PRODUCTVERSION_STR);

    MainWindow w;
    w.show();

    if (argc > 1)
        w.OpenScene(EditorUtils::AxToQString(AxString(argv[1]).Replace("\\", "/")));

    return a.exec();
}
