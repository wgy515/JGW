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

:loop_root
adb root

:loop1
adb get-state | find "device" 
if %ERRORLEVEL% == 0 (
    break
) else (
	Wscript sleep.vbs
    adb get-state
    goto loop1
)

adb shell "rmmod -f wlan" | find "/system/bin/sh" 

adb shell "insmod /system/lib/modules/wlan.ko con_mode=5"
if %ERRORLEVEL% == 1 (
	Wscript sleep.vbs
    goto loop_root
) else (
    break
)
adb shell "ftmdaemon  -n"
