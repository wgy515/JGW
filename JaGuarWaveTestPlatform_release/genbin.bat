rd /s /q bin\debug\
rd /s /q bin\release\
xcopy platform_bin bin\debug\ /s /e /h /y
xcopy platform_bin bin\release\ /s /e /h /y
xcopy platform_bin_debug\exec bin\debug\exec_debug\ /s /e /h /y
xcopy platform_bin_debug\exec bin\release\exec_release\ /s /e /h /y
xcopy platform_bin_debug\help bin\debug\help_debug\ /s /e /h /y
xcopy platform_bin_debug\help bin\release\help_release\ /s /e /h /y