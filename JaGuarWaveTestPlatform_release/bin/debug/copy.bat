::echo WScript.sleep 1000 > sleep.vbs
:check
set hasit=no
::Wscript sleep.vbs
ping 127.0.0.1 -n 2
for /F "tokens=1*" %%a in ('tasklist /nh /fi "imagename eq JaGuarWaveTestPlatform_Setup.exe"') do if %%a == JaGuarWaveTestPlatform_Se set hasit=yes
if %hasit% == yes goto check
rd /s /q c:\JaGuarWaveTestPlatformInstall\
xcopy JaGuarWaveTestPlatformInstall c:\JaGuarWaveTestPlatformInstall\ /s /e /h /y