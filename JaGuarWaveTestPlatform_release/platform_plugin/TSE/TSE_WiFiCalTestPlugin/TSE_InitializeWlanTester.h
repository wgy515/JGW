#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_WlanInstrumentConfig.h"
#include <JGW_Device_WCNTester_CommonSCPIWLAN/WCNTester_CommonSCPIWLAN.h>
#include "CJGW_WlanConfigFile.h"
namespace JGW
{
    class CTSE_InitializeWlanTester : public CTSE_TestBase
    {
    public:
        CTSE_InitializeWlanTester(void);
        ~CTSE_InitializeWlanTester(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Init();
        bool TSE_Run();
    private:
        //! D:\UC600AA\StationCalDataFile_UC600.XML
        std::wstring mstrStationCalFileName;
        //！Intsrument Config
        std::wstring mstrTesterConfigFileName;
        //!
        std::wstring mstrWlanConfigFileName;
        //! BH0_LP,BH0_LP,BH0_LP,BH0_LP
        std::wstring mstrTxStationCalPathName;
        //! BH0_LP,BH0_LP,BH0_LP,BH0_LP
        std::wstring mstrRxStationCalPathName;
        //! VSA 配置的端口 => 设备TX
        //! 1,1;1,1;1,1;1,1;  => 1 , 1 => 2.4G 5G RF Port
        std::wstring mstrTRxChainPort;
        std::wstring mstrTRxChainPortEnv;
        //! VSG 配置端口 => 设备RX
        //! 1,1;1,1;1,1;1,1;  => 1 , 1 => 2.4G 5G RF Port
        //std::wstring mstrRxVSGChainPort;
        CCJGW_WlanInstrumentConfig mWlanInstrumentConfig;
        //! 
        WCNTester_CommonSCPIWLAN mWCNTesterCommonSCPIWLAN;
        //! 
        CCJGW_WlanConfigFile mWlanConfigFile;
    };
}