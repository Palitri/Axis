set "QtDeployApplication=D:\Programs\Qt\Qt5.5.1\5.5\msvc2012\bin\windeployqt.exe"

set "ApplicationName=Axis Editor.exe"
set "ReleaseDir=..\build-AxisEditor-Desktop_Qt_5_5_1_MSVC2012_32bit-Release\release"
set "DeployDir=..\bin"

set "EngineDir=..\..\Engine\Release"

set "ThumbnailProvidersReleaseDir=..\..\Windows Thumbnail Providers\x64\Release"
set "ThumbnailProvidersDeployDir=..\bin\Thumbnail Providers"
set "ThumbnailProvidersName=AxisThumbnailProviders.dll"



set "ApplicationReleaseFullName=%ReleaseDir%\%ApplicationName%"
set "ApplicationDeployFullName=%DeployDir%\%ApplicationName%"

set "ThumbnailProvidersReleaseFullName=%ThumbnailProvidersReleaseDir%\%ThumbnailProvidersName%"
set "ThumbnailProvidersDeployFullName=%ThumbnailProvidersDeployDir%\%ThumbnailProvidersName%"
