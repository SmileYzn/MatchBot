@echo off

tasklist /fi "imagename eq hlds.exe" |find ":" > nul
	if errorlevel 1 (taskkill /f /im "hlds.exe")

ping 127.0.0.1 -n 2 > nul

set "outputFile=..\Release\matchbot_mm.dll"

set "copyPath=D:\Files\Desktop\hlds\hlds\cstrike\addons\matchbot\dlls"

if exist %copyPath% (copy %outputFile% %copyPath%)

set "hldsParam=-console -game cstrike -secure -pingboost 3 -master -bots +port 27020 +map de_inferno +maxplayers 32 +sys_ticrate 1000"

set "hldsPath=D:\Files\Desktop\hlds\hlds"

if exist %hldsPath% (start /min /d "%hldsPath%" hlds.exe %hldsParam%)