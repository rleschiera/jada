echo off
set arg1=%1
set arg2=%2
C:\jada\bin\patch-starter.exe -in winagent.template.exe -out winagent.exe -url %arg1% -customer %arg2%
C:\jada\bin\patch-starter.exe -in winmanager.template.exe -out winmanager.exe -url %arg1% -customer %arg2%
REM pause
