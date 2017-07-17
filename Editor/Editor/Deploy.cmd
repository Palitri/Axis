@REM Please make sure RELEASE builds of Axis ENGINE and EDITOR are available before deploying

@echo off

call "Set Variables.cmd"

@echo Deploying to %DeployDir%
@echo.

rmdir "%DeployDir%" /s/q
md "%DeployDir%"
copy "%ApplicationReleaseFullName%" "%ApplicationDeployFullName%"

%QtDeployApplication% "%ApplicationDeployFullName%"

@echo.
@echo Copying engine files
@echo.

copy "%EngineDir%\*.dll" "%DeployDir%\*.dll"

@echo.
@echo Copying thumbnail providers
@echo.

md "%ThumbnailProvidersDeployDir%"
copy "%ThumbnailProvidersReleaseFullName%" "%ThumbnailProvidersDeployFullName%"

@echo.
@echo Deployed to %DeployDir%

pause