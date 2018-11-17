echo off

REM Rimuove il deploy precedente
echo Rimuove il deploy precedente
rmdir /S /Q C:\Apache22\htdocs\jada
mkdir C:\Apache22\htdocs\jada
REM pause

REM Effettua il nuovo deploy
echo Effettua il nuovo deploy
xcopy /S C:\jada\site		C:\Apache22\htdocs\jada

echo sitedeploy.bat terminato.
pause

