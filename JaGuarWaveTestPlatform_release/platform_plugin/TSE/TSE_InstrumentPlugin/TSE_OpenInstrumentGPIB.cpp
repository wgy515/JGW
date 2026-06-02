#include "StdAfx.h"
#include "TSE_OpenInstrumentGPIB.h"
#include "CJGW_TSEIntrument.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
namespace JGW
{
    CTSE_OpenInstrumentGPIB::CTSE_OpenInstrumentGPIB(void) : m_cNIGPIBType(0),m_nGpibInterface(0),m_nPrimaryAddress(5),m_nSecAddress(0),mstrInstrumentEnvironment(L"TSE_INSTRUMENT_ENVIRONMENT")
    {
    }

    CTSE_OpenInstrumentGPIB::~CTSE_OpenInstrumentGPIB(void)
    {
        //CCJGW_TSEIntrument::CloseIEquipCommTL();
    }

    const wchar_t* CTSE_OpenInstrumentGPIB::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_OpenInstrumentGPIB\",\
                \"TestName\": \"打开仪器GPIB连接\",\
                \"NIGPIBTypeEnvionment\":\"GPIB厂商类型0:NI 1:AGILENT 对应的环境变量,默认：0\",\
                \"GpibInterfaceEnvionment\":\"GPIB 接口ID ,默认：0\",\
                \"PrimaryAddressEnvionment\":\"GPIB地址对应的环境变量,默认：5\",\
                \"SecAddressEnvionment\":\"GPIB次要地址对应的环境变量,默认：0\",\
                \"InstrumentEnvironment\":\"Save Instrument instance,default:TSE_INSTRUMENT_ENVIRONMENT\"\
                }";
    }

    bool CTSE_OpenInstrumentGPIB::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(_T("NIGPIBTypeEnvionment"),strParamName))
        {
            mstrNIGPIBTypeEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("PrimaryAddressEnvionment"),strParamName))
        {
            mstrPrimaryAddressEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("SecAddressEnvionment"),strParamName))
        {
            mstrSecAddressEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("GpibInterfaceEnvionment"),strParamName))
        {
            mstrGpibInterfaceEnvionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("InstrumentEnvironment"),strParamName))
        {
            mstrInstrumentEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_OpenInstrumentGPIB::TSE_Run()
    {
        GetGlobalEnvironment()->PutInt(mstrInstrumentEnvironment,int(&mcTSEIntrument));
        if (2 != mcTSEIntrument.GetIEquipCommTLType() 
            || m_cNIGPIBType != GetGlobalEnvironment()->GetInt(mstrNIGPIBTypeEnvionment)
            || m_nGpibInterface != GetGlobalEnvironment()->GetInt(mstrGpibInterfaceEnvionment)
            || m_nPrimaryAddress != GetGlobalEnvironment()->GetInt(mstrPrimaryAddressEnvionment)
            || m_nSecAddress != GetGlobalEnvironment()->GetInt(mstrSecAddressEnvionment))
        {
            m_cNIGPIBType = GetGlobalEnvironment()->GetInt(mstrNIGPIBTypeEnvionment);
            m_nGpibInterface = GetGlobalEnvironment()->GetInt(mstrGpibInterfaceEnvionment);
            m_nPrimaryAddress = GetGlobalEnvironment()->GetInt(mstrPrimaryAddressEnvionment);
            m_nSecAddress = GetGlobalEnvironment()->GetInt(mstrSecAddressEnvionment);
           mcTSEIntrument.CloseIEquipCommTL();
        }
        return NULL != mcTSEIntrument.OpenIEquipCommTLToGPIB(m_cNIGPIBType,m_nGpibInterface,m_nPrimaryAddress,m_nSecAddress);
    }
}