::echo WScript.sleep 1000 > sleep.vbs
::if not exist JaGuarWaveTestPlatform_Setup.exe goto nofile
::Wscript sleep.vbs
ping 127.0.0.1 -n 2
set /a a=0
:check
set hasit=no
::Wscript sleep.vbs
ping 127.0.0.1 -n 2
set /a a+=1
if %a% equ 20 goto start_application
for /F "tokens=1*" %%a in ('tasklist /nh /fi "imagename eq Uninstall.exe"') do if %%a == Uninstall.exe set hasit=yes
if %hasit% == yes goto check
::Wscript sleep.vbs
ping 127.0.0.1 -n 2
:start_application
if not exist JaGuarWaveTestPlatform_Setup.exe goto nofile
start JaGuarWaveTestPlatform_Setup.exe
:nofile