#include "StdAfx.h"
#include "TSE_Puma_OpenDevice.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include "../CJGW_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_PUMA_OpenDevice::CTSE_PUMA_OpenDevice(void):mnPort(-1)
    {
    }

    bool CTSE_PUMA_OpenDevice::CreateParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CCJGW_TestSuiteImpl::CreateParam(strParamName,strParamValue)) return true;
        if (0 == JGW_WStrComparenoCaseWStr(_T("port"),strParamName))
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

    bool CTSE_PUMA_OpenDevice::Run()
    {
        if (mnPort > 0)
        {
            std::wstring strCom;
            JGW_FormatWString(strCom,L"COM%d",mnPort);
            return CCJGW_DevicesGlobalResource::GetDownloadPDBInstance()->OpenDongleComPort(strCom);
        }

        jgw_timer tm;
        std::wstring strComPort;
        size_t nComport = 0;
        while (tm.elapsed() < mnTimeOut)
        {
            //!Diag
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (strComPort.empty()) { Sleep(1000); continue; }
            if ( CCJGW_DevicesGlobalResource::GetDownloadPDBInstance()->OpenDongleComPort(strComPort)) return true;
            Sleep(1000);
        }
        return false;
    }
}