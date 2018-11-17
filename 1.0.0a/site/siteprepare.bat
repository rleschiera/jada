echo off

REM Prepara il sito per l'aggiornamento
echo Prepara il sito per l'aggiornamento...
del /Q C:\jada\site\download\*.*
del /Q C:\jada\site\clienti\test\*.exe 
echo Sito preparato per l'aggiornamento
pause

REM Copia l'ultima versione dei binari generici
echo Copia l'ultima versione dei binari generici...
copy C:\jada\bin\*	C:\jada\site\download
ren C:\jada\site\download\UltraVNC.ini	UltraVNC.ini.txt
echo Ultima versione dei binari generici copiata
pause

REM Prepara i file specifici per ciascun cliente
echo Prepara i file specifici per ciascun cliente...
call C:\jada\site\customerprepare.bat "http://jadatest/jada" "test"
call call C:\jada\site\customerprepare.bat "http://jada.iss.local/jada" "iss"
call C:\jada\site\customerprepare.bat "http://ec2-50-112-71-242.us-west-2.compute.amazonaws.com/jada" "aws"
echo File specifici per ciascun cliente preparati
pause

echo siteprepare.bat terminato.
pause

