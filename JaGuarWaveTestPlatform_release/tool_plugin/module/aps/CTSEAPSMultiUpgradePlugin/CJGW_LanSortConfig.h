#pragma once
#include "CTSEAPS_UpgradeDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
namespace JGW
{
    class CCJGW_LanSortConfig
    {
    public:
        CCJGW_LanSortConfig(LPS_NET_CARD_PARAM psNetCardParams);
        ~CCJGW_LanSortConfig(void);
    public:
        //! 加载LAN排序配置文件
        bool LoadLanSortConfig(const std::wstring& strSortFilePath);
        //! 保存LAN排序配置文件
        void SaveLanSortConfig(LPS_NET_CARD_PARAM psNetCardParam);
        //! clear 排序配置文件
        void ClearLanSortConfig();
    private:
        LPS_NET_CARD_PARAM mpsNetCardParams;
        CCJGW_ConfigIni mConfigIni;
    };
}