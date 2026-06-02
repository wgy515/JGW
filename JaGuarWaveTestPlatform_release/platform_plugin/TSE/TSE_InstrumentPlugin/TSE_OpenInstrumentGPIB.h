#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include "CJGW_TSEIntrument.h"
namespace JGW
{
    struct S_NIGPIB_INSTRUMENT_INFO
    {
        
    };


    struct S_VISA_INSTRUMENT_INFO
    {
        std::string mstrVisaAddress;
    };

    struct S_TCPIP_INSTRUMENT_INFO
    {
        size_t mnSubInstrument;
        std::string mstrIPAddress;
    };
    //! MONITOR MODEL PRIMARY_ADDRESS GPIB_INTERFACE
    //! GPIB TCP IP SERIAL
    struct S_MONITOR_INFO
    {
        BYTE                m_cInterfaceType;
        std::wstring        m_strModelName;
        S_NIGPIB_INSTRUMENT_INFO msNIGPIBInstrumentInfo;
        S_VISA_INSTRUMENT_INFO msVISAInstrumentInfo;
        S_TCPIP_INSTRUMENT_INFO msTCPIPInstrumentInfo;
    };

    enum
    {
        NI_GPIB_TYPE = 0,
        AGILENT_GPIB_TYPE
    };

    enum
    {
        E_GPIB_HIVE,//! GPIB连接 又分为NI和Agen
        E_TCP_IP_HIVE, //! TCP IP连接
        E_VISA_HIVE, //! VISA
        E_SERIAL_HIVE //! Serial连接
    };

    class CTSE_OpenInstrumentGPIB : public CTSE_TestBase
    {
    public:
        CTSE_OpenInstrumentGPIB(void);
        ~CTSE_OpenInstrumentGPIB(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
    private:
        bool TSE_Run();
        bool LoadMonitorConfig();
    private:
        BYTE m_cNIGPIBType;
        DWORD m_nGpibInterface;
        DWORD m_nPrimaryAddress;
        DWORD m_nSecAddress;
        std::wstring mstrNIGPIBTypeEnvionment;
        std::wstring mstrPrimaryAddressEnvionment;
        std::wstring mstrSecAddressEnvionment;
        std::wstring mstrGpibInterfaceEnvionment;
        std::wstring mstrInstrumentEnvironment;
        CCJGW_TSEIntrument mcTSEIntrument;
    };
}