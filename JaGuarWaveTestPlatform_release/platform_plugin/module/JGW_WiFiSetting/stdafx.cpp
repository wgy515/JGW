// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_WiFiSetting.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
#include <JGW_WiFiSetting/CJGW_WiFiSettingImpl.h>
#include "CJGW_WiFiSetting.h"

namespace JGW
{
    CCJGW_WiFiSetting* gpWiFiSetting = NULL;
    CCJGW_WiFiSettingImpl* GetWiFiSetting()
    {
        if (NULL == gpWiFiSetting) gpWiFiSetting = new CCJGW_WiFiSetting;
        return gpWiFiSetting;
    }

    void DestroyWiFiSetting()
    {
        if (gpWiFiSetting) 
        {
            delete gpWiFiSetting;
            gpWiFiSetting = NULL;
        }
    }
}