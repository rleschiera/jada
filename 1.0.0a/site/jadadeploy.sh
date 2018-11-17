#! /bin/bash

szDeployURL="http://areas-demo.eng.it/jada/"

#******************************************************************************
#
#    P A T C H     D E G L I     S T A R T E R
#
#******************************************************************************
szOldURL="http://AAA.BBB.CCC.DDD/jada/aggiornare_url_con_il_valore_specifico_della_installazione"

# Effettua il patch degli starter
bin/patch-starter -in bin/winagent.template.exe -out bin/winagent.exe -patch $szDeployURL
printf "    winagent.exe modificato.\n"
bin/patch-starter -in bin/winmanager.template.exe -out bin/winmanager.exe -patch $szDeployURL
printf "    winmanager.exe modificato.\n"

#******************************************************************************
#
#    M A N I F E S T
#
#******************************************************************************
# Crea il file di manifest generale
md5sum bin/* config/* | sed 's/  / /g' > all.manifest.txt
printf "    File di manifest generale creato.\n"

# Crea il file di manifest della componente agent
rm -f agent.manifest.txt
szFile=$(grep "bin/winagent.exe"     all.manifest.txt); echo "delayed $szFile" >> agent.manifest.txt
szFile=$(grep "bin/jada.dll"         all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/jadawrapper.dll"  all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/msvcp100d.dll"    all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/msvcr100d.dll"    all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/options.vnc"      all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/pthreadVC.dll"    all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/UltraVNC.ini"     all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/vncserver.exe"    all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/wa.exe"           all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "bin/zlib.dll"         all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "config/agent.ini"     all.manifest.txt); echo "notfound $szFile" >> agent.manifest.txt
szFile=$(grep "config/area.combo"    all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "config/cliente.combo" all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "config/regione.combo" all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
szFile=$(grep "config/settings.ini"  all.manifest.txt); echo "updated $szFile" >> agent.manifest.txt
unix2dos -q agent.manifest.txt
unix2dos -q config/area.combo 
unix2dos -q config/cliente.combo
unix2dos -q config/regione.combo
unix2dos -q config/agent.ini
unix2dos -q config/settings.ini
printf "    File di manifest agent creato.\n"

# Crea il file di manifest della componente manager
rm -f manager.manifest.txt
szFile=$(grep "bin/winmanager.exe"         all.manifest.txt); echo "delayed $szFile" >> manager.manifest.txt
szFile=$(grep "bin/jada.dll"               all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/jadawrapper.dll"        all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/DirectShowLib-2005.dll" all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/HighQualityVideo.prx"   all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/msvcp100d.dll"          all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/msvcr100d.dll"          all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/options.vnc"            all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/pthreadVC.dll"          all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/Splicer.dll"            all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/UltraVNC.ini"           all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/videomaker.exe"         all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/VncSharp.dll"           all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/WebCamCapture.dll"      all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/wm.exe"                 all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "bin/zlib.dll"               all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "config/area.combo"          all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
szFile=$(grep "config/manager.ini"         all.manifest.txt); echo "notfound $szFile" >> manager.manifest.txt
szFile=$(grep "config/settings.ini"        all.manifest.txt); echo "updated $szFile" >> manager.manifest.txt
unix2dos -q manager.manifest.txt
unix2dos -q config/area.combo
unix2dos -q config/manager.ini
unix2dos -q config/settings.ini
printf "    File di manifest manager creato.\n"

echo "Terminato."
