echo off
set url=%1
set customer=%2

REM Copia i file specifici per il cliente
echo Copia i file specifici per il cliente %customer%...
copy C:\jada\bin\*.template.exe		C:\jada\site\clienti\%customer%
REM pause

REM Esegue il patch per il cliente
echo Esegue il patch per il cliente %customer%...
cd C:\jada\site\clienti\%customer%
call C:\jada\bin\patch-cmds.bat %url% %customer%
REM pause

REM Produce i manifest per il cliente
echo Produce i manifest per il cliente %customer%...
cd C:\jada\site\clienti\%customer%
C:\jada\bin\make-manifest.exe %customer%
REM pause

echo customerprepare.bat per il cliente %customer% terminato.
REM pause

