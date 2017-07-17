@echo off

echo Resetting the list of applications for which FTH is mitigating problems

echo.

@echo on
Rundll32.exe fthsvc.dll,FthSysprepSpecialize
@echo off

echo.

echo To view FTH logged information:
echo 1. Click the Start menu.
echo 2. Right-click Computer and click Manage.
echo 3. Click Event Viewer > Applications and Services Logs > Microsoft > Windows > Fault-Tolerant-Heap
echo 4. View FTH Events.

echo.

pause