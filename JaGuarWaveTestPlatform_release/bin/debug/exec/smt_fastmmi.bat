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
adb push tool/misc_smt.img /data/
adb shell dd if=/data/misc_smt.img  of=/dev/block/sda4
adb reboot