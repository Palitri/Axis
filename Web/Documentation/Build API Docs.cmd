@echo off

@echo Building API help...

jsdoc -c ./JSDocConfig.json --readme ./Homepage.txt

@echo API Help building finished.
pause