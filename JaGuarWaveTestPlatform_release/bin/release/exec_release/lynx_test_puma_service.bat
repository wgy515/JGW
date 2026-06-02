@echo on
:loop
adb get-state | find "device" 
if %ERRORLEVEL% == 0 (
    break
) else (
	Wscript sleep.vbs
    adb get-state
    goto loop
)

:lynx_service
start JGW_WlanServiceApplication.exe
tool\iperf-2.0.9-win32\iperf -s
Wscript sleep.vbs
goto lynx_service