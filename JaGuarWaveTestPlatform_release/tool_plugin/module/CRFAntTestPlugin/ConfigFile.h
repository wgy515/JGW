#pragma once
#include <string>
#include "configwnd/RfTestDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>

namespace JGW
{
    class CConfigFile
    {
    public:
        CConfigFile(void);
        ~CConfigFile(void);

        bool				LoadConfigFile();
        void				SetCacheConfigFilePath( const wchar_t* strIniPath);
        void				SetConfigFilePath( const wchar_t* strconfigIniPath);
        const wchar_t* GetErrorMsg(){return m_strErrorMsg.c_str();}
        S_RF_CONFIG_INFO	m_sRfConfigInfo;

        bool				ChangesModeSelecter(S_CHANGE_BAND_INFO& sChangeBandInfo);
        bool				ChangeChannelInfo(std::vector<S_CHANGE_Chanel_INFO>& vChannelInfo);

    private:
        bool                LoadMonitorConfig();
        bool                ChangeMonitorConfig();
        bool				LoadSysRfConfig(COMMUNICATIONSSYS esys,S_SYS_INFO& sSysInfo);
        bool				LoadLteConfig();
        bool				LoadWcdmaConfig();
        bool				LoadTdscdmaConfig();
        bool				LoadCdmaConfig();
        bool				LoadGsmConfig();
        bool				LoadGpsConfig();
        bool                LoadWlanConfig();
        bool				CheckGSMBandVal(int nBand);
        bool                CheckWlanBandVal(int nBand);
        bool                ParseWlanNormConfig(const wchar_t* strSec,const S_BAND_INFO &sBandInfo,S_WLAN_BAND_NORM_INFO& sWlanBandNormInfo);
        bool				ParseGsmNormConfig(const wchar_t* strSec,S_GSM_BAND_NORM_INFO& sGsmBandNormInfo);
    private:
        std::wstring		m_strCacheIniPath; 
        std::wstring		m_strConfigIniPath;
        CCJGW_ConfigIni     m_pCacheIniFile;
        CCJGW_ConfigIni	    m_pConfigIniFile;
        std::wstring m_strErrorMsg;
    };
}