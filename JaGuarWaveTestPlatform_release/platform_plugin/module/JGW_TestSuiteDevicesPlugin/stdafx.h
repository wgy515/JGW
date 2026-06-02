// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <tchar.h>

#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_TestSuiteGlobalResourcesLibrary/CJGW_TestSuiteLogResource.h>

// TODO: 在此处引用程序需要的其他头文件
#define JGW_TAG "TestSuiteDevices"
#include <JGW_Log4cplusImplPlugin\log4cplusimpl.h>

#define DEBUG_MSG   PDLOG4WW
#define INFO_MSG    PDLOG4WW
#define ERROR_MSG   PELOG4WW
#define ERROR_MSG_FORMAT PELOG4WW_F
#define DEBUG_MSG_FORMAT PDLOG4WW_F
#define INFO_MSG_FORMAT PDLOG4WW_F
#define HTML_FILE_PATH_MSG PHLOG4WW
//! TDO:
#define TEST_PROJECT_NAME_FORMAT(log_buf,...) CCJGW_TestSuiteLogResource::DispatchSuiteTestLogMsgFormat(E_TEST_SUITE_TEST_NAME,log_buf,__VA_ARGS__)
#define TEST_PROJECT_NAME_MSG CCJGW_TestSuiteLogResource::DispatchSuiteTestNameMsg