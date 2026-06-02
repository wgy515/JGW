#pragma once
#include "DUComm.h"
#include <string>
#define PID_VID_MSG_MAX_COUNT		3
#define PROCESS_MAX_VALUE			600

#define DOWNLOAD_THREAD_MAX			8


#define SORT_PORT_CONFIG_NAME		L"SortRecord.ini"
#define CONFIG_DOWNLOAD_NAME		L"down.ini"


#define PASS_PROCESS_COLOR	L"progress_fore.png"
#define PASS_STATUS_DOWN	L"#ff00ff00"

/*#define IDLE_STATUS_DOWN	L"#ff808000"*/

#define IDLE_STATUS_DOWN	L"#ffffff00"
#define IDEL_PROCESS_COLOR  L"progress_fore_gray.png"

#define FAIL_STATUS_DOWN	L"#ffff0000"
#define FAIL_PROCESS_COLOR	L"progress_fore_read.png"



namespace JGW
{
    extern std::wstring g_strDownloadDubugLog[DOWNLOAD_THREAD_MAX];

    enum E_GET_WORKTHREAD_ERROR
    {
        E_NO_BIND_THREAD = 99998,
        E_GET_INSTACNCEID_ERROR = 99999
    };
}