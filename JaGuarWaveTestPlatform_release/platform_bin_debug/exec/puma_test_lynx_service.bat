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
adb shell iperf -s
Wscript sleep.vbs
goto lynx_service