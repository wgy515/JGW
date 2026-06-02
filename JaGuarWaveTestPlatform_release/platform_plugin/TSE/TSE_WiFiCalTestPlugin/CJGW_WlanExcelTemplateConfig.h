#pragma once
#include "CJGW_IPQ807xTestDataDefine.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include "CJGW_ExcelLibXL.h"
namespace JGW
{
    class CCJGW_WlanExcelTemplateConfig
    {
    public:
        CCJGW_WlanExcelTemplateConfig(void);
        ~CCJGW_WlanExcelTemplateConfig(void);
    public:
        //! excel config ini path,excel template path
        bool LoadWlanExcelTemplateConfig(const std::wstring& strPath,const std::wstring& strExcelPath,std::vector<S_EXCEL_WIFI_POS>& vsExcelWifiInfo);
        //!
        std::wstring GetErrorMessage();
    private:
        // 加载WIFI协议信息配置
        bool LoadWlanProtocolInfoConfig(CCJGW_ConfigIni& configIni,const std::wstring& strAppName,std::vector<S_EXCEL_WIFI_POS>& sWlanProtocolInfo);
        void LoadWlanSysConfig(CCJGW_ConfigIni& configIni,const std::wstring& strAppName,std::map<std::wstring,std::wstring>& mmapWlanSysConfig);
        void GetStartChannelExcelInfo(std::map<std::wstring,std::wstring>& mmapWlanSysConfig,S_EXCEL_WIFI_POS& sChannelExcelInfo);

        // 获取信道配置
        bool GetExcelSheetChannel(std::wstring& strStartChannelRowCol,std::vector<uint>& vChannel);
        // 获取WLAN RATE
        bool GetExcelSheetRate(const std::wstring& strRateRow,const std::wstring& strRateCol,std::vector<WLAN_UniversalDataRate>& vRate);
        // 获取WLAN速率配置信息
        WLAN_UniversalDataRate GetWlanUniversalDataRateToContent(const std::wstring& strContent);
    private:
        void GetArrayRowColToVector(const std::wstring& strValue,uint* uRowCol);
        void GetRxPowerPerRange(const std::wstring& strConfig,int& uRxStartPower,uint& nuRxPerRange);
    private:
        std::wstring mstrErrorMessage;
        CCJGW_ExcelLibXL mcExcelLibXL;
    };

}

