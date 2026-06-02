@echo off
::set /p puma_ipaddr=«Î ‰»ÎPUMAµƒIPµÿ÷∑ : 
::echo "PUMA IP ADDR" %puma_ipaddr%
start JGW_WlanServiceApplication.exe
:lynx_service
::tool\iperf3 -s -B %puma_ipaddr%
tool\iperf3 -s
Wscript sleep.vbs
goto lynx_service