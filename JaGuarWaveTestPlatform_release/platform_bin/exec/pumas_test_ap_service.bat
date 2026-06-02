@echo on
:lynx_service
start JGW_WlanServiceApplication.exe
TSEAPSUpgradeTool.exe TSE_SUITE_CONFIG_FILE_PATH="cache\TSE_PUMAS_THROUGHPUT_SERVICE\suite.xml" TSE_SUITE_AUTO_RUN=1 TSE_SUITE_AUTO_RUN_INTERVAL=3000
Wscript sleep.vbs
goto lynx_service