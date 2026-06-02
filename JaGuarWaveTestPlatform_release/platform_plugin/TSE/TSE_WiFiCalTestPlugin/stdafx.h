// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#define GETINSTRUMENT GetWlanInstrumentInterface()


// TODO: 在此处引用程序需要的其他头文件
#include <TSE_FoundationLib/TSE_FoundationLib_Define.h>
#include "JGW_Phone.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>
#include <JGW_Device_WCNTester_CommonSCPIWLAN/JGW_IWlanInstrumentInterface.h>
#include "CJGW_WlanConfigFile.h"

namespace JGW
{
    extern CJGW_Phone* GetPhoneInstance();

    extern CCJGW_WlanConfigFile* GetWlanConfigFile();

    extern CJGW_IWlanInstrumentInterface* GetWlanInstrumentInterface();

    extern void mapWlanRate(WLAN_DataRate wlanRate, bool short11b_nGuard,WLAN_RATE& rate,WLAN_Gen6_PHYDBG_PreambleRate& preambleType, uint channel2 /*= 0u*/);
}