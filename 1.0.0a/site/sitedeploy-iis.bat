echo off

REM Rimuove il deploy precedente
echo Rimuove il deploy precedente
rmdir /S /Q C:\inetpub\wwwroot\jada
mkdir C:\inetpub\wwwroot\jada
REM pause

REM Effettua il nuovo deploy
echo Effettua il nuovo deploy
xcopy /S C:\jada\site		C:\inetpub\wwwroot\jada

echo sitedeploy.bat terminato.
pause

