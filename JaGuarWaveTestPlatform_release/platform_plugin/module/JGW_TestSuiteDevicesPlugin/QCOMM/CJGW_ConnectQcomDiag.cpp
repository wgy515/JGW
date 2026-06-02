#include "StdAfx.h"
#include "CJGW_ConnectQcomDiag.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "../CJGW_DevicesGlobalResource.h"
namespace JGW
{
    CCJGW_ConnectQcomDiag::CCJGW_ConnectQcomDiag(void):m_bUserQpst(FALSE),mnPort(-1)
    {
    }

    bool CCJGW_ConnectQcomDiag::CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CCJGW_TestSuiteImpl::CreateParam(strParamName,strParamValue)) return true;
        if(0 == JGW_WStrComparenoCaseWStr(_T("QMSLLibraryMode"),strParamName))
        {
            m_bUserQpst = _ttoi(strParamValue)?TRUE:FALSE;
        }
        else if (0 == JGW_WStrComparenoCaseWStr(_T("port"),strParamName))
        {
            mnPort = _ttoi(strParamValue);
        }
        else if (0 == JGW_WStrComparenoCaseWStr(_T("TimeOut"),strParamName))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CCJGW_ConnectQcomDiag::Run()
    {
        if (mnPort > 0)
        {
            return CCJGW_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,m_bUserQpst)->ConnectDevice(mnPort,mnTimeOut);
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
            if (CCJGW_DevicesGlobalResource::GetConnetQcommDiagActionIsntance(NULL,m_bUserQpst)->ConnectDevice(mnPort,mnTimeOut)) return true;
            Sleep(1000);
        }
        return false;
    }

}
