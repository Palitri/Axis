@echo off

set "jsminApplication=D:\Programs\jsmin\jsmin.exe"

set "DeploymentApplication=Deployment\AxisWebDeployment\Debug\AxisWebDeployment.exe"

set "MergedFileName=AxisEngine-full.js"
set "MinifiedFileName=AxisEngine-min.js"

set "MergedFileFullName=%cd%\AxisEngine-full.js"
set "MinifiedFileFullName=%cd%\AxisEngine-min.js"



@echo Merging files into %MergedFileName%...
%DeploymentApplication% -merged %MergedFileName% -root %cd% -silent

@echo Minifying into %MinifiedFileName%...
%jsminApplication% <%MergedFileFullName% >%MinifiedFileFullName%


@echo Stamping license...
set "TempFile=deploy.tmp"
set "LicenseFile=Supplemental\AxisLicense-min.js"
type %LicenseFile% > %TempFile%
type %MinifiedFileFullName% >> %TempFile%
type %TempFile% > %MinifiedFileFullName%
del %TempFile%

@echo Deploy finished.
pause