#include "StdAfx.h"
#include "TSE_ConnectQcomDiag.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "../TSE_DevicesGlobalResource.h"
namespace JGW
{
    CTSE_ConnectQcomDiag::CTSE_ConnectQcomDiag(void):m_bUserQpst(FALSE),mnPort(-1),mstrSaveResourceContextEnvironment(L"TSE_QUALCOMM_RESOURCECONTEXT")
    {
    }

    const wchar_t* CTSE_ConnectQcomDiag::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_ConnectQcomDiag\",\
                \"TestName\": \"连接高通DIAG(Qualcomm)\",\
                \"port\": \"-1:自动搜索端口 否则必须大于0\",\
                \"SaveResourceContextEnvironment\": \"Save Connect Resource Environment\",\
                \"PortEnvironment\": \"Port Environment\",\
                \"QMSLLibraryMode\": \"0:QPHONEMS, 1:QPST 2:UserDefinedTransport. User should also set QMSLUserDefinedTransportDLLPath and QMSLLibraryModes\",\
                \"TimeOut\": \"超时时间设置 以S为单位\"\
                }";
    }

    bool CTSE_ConnectQcomDiag::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("QMSLLibraryMode"),strParamName))
        {
            m_bUserQpst = _ttoi(strParamValue)/*?TRUE:FALSE*/;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("port"),strParamName))
        {
            mnPort = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("PortEnvironment"),strParamName))
        {
            mstrPortEnvironment = strParamValue;
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("SaveResourceContextEnvironment"),strParamName))
        {
            mstrSaveResourceContextEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("TimeOut"),strParamName))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_ConnectQcomDiag::TSE_Run()
    {
        if (!mstrPortEnvironment.empty())
        {
            size_t comport = GetGlobalEnvironment()->GetInt(mstrPortEnvironment);
            if (comport > 0)
            {
                return CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,m_bUserQpst)->ConnectDevice(comport,mnTimeOut);
            }
        }

        if (mnPort > 0)
        {
            jgw_timer tm;
            while (tm.elapsed() < mnTimeOut)
            {
                if (CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,m_bUserQpst)->ConnectDevice(mnPort,mnTimeOut))
                {
                    return true;
                }
                LogI(L"warining open qcomm port fail");
                Sleep(1000);
            }
            return false;
        }

        jgw_timer tm;
        std::wstring strComPort;
        size_t nComport = 0;
        while (tm.elapsed() < mnTimeOut)
        {
            //!Diag
            strComPort = JGW_GetComPortToUsbServiceName(L"qcusbser",L"VID_05C6",L"",L"Diag");
            if (strComPort.empty()) { Sleep(1000); continue; }
            JGW_ReplaceStringW(strComPort,L"COM",L"");
            nComport = _ttoi(strComPort.c_str());
            if (CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,m_bUserQpst)->ConnectDevice(nComport,mnTimeOut)) return true;
            Sleep(1000);
        }
        return false;
    }

    bool CTSE_ConnectQcomDiag::TSE_Exit()
    {
        GetGlobalEnvironment()->PutInt(mstrSaveResourceContextEnvironment,(int)CTSE_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,m_bUserQpst)->GetResourceContext());
        return true;
    }
}
