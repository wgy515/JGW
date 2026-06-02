#include "StdAfx.h"
#include "TSE_SaharaOpenPort.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>


namespace JGW
{
    CTSE_SaharaOpenPort::CTSE_SaharaOpenPort(void) : mnMode(0),mnTimeOutSec(10),mstrResourceContextEnv(L"TSE_RESOURCE_CONTEXT")
    {
    }


    CTSE_SaharaOpenPort::~CTSE_SaharaOpenPort(void)
    {
    }

    const wchar_t* CTSE_SaharaOpenPort::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaharaOpenPort\",\
                \"TestName\": \"open sahara port\",\
                \"PortEnv\": \"is empty : auto select port,port environment\",\
                \"ResourceContextEnv\": \"save open port handle environment\",\
                \"Mode\": \"Sahara hello Packet mode, when mode = 0, this indicates a SAHARA_MODE_IMAGE_TX_PENDING mode when mode = 2, this indicates a SAHARA_MODE_MEMORY_DEBUG mode\",\
                \"TimeOutSec\": \"indicates the timeout between reads\"\
                }";
    }

    bool CTSE_SaharaOpenPort::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PortEnv"))
        {
            mstrPortEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Mode"))
        {
            mnMode = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TimeOutSec"))
        {
            mnTimeOutSec = _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaharaOpenPort::TSE_Run()
    {
        int nPort = -1;
        HANDLE hResourceContext = NULL;
        //! check port
        if (!mstrPortEnv.empty()) nPort = GetGlobalEnvironment()->GetInt(mstrPortEnv);
        if (nPort <= 0)
        {
            jgw_timer jt;
            while (jt.elapsed() < mnTimeOutSec)
            {
                LogD(L"Find Download ComPort ...\n");
                std::wstring strComPort = JGW_GetComPortToUsbServiceName(L"qcusbser",L"VID_05C6",L"PID_9008");
                if (strComPort.empty()) { Sleep(1000); continue; }
                JGW_ReplaceStringW(strComPort,L"COM",L"");
                nPort = _ttoi(strComPort.c_str());
                break;
            }
            if (nPort <= 0)
            {
                LogE(L"Find Download ComPort Fail\n");
                return false;
            }
        }

        LogD_F(L"Start Open Download ComPort(%d) ...\n",nPort);
        QLIB_SetLibraryMode(QLIB_LIB_MODE_QPHONEMS);

        unsigned long version = 0 ,sn = 0,ulmsid = 0;
        char pkHash[200] = {0};
        unsigned char bGetInfo = 1;
        hResourceContext = QLIB_QPHONEMS_ConnectServer_Sahara(nPort,&version,&sn,&ulmsid,pkHash,bGetInfo,mnMode,mnTimeOutSec,swdlQPHONEMSCB_Func);
        if (NULL == hResourceContext)
        {
            LogE(L"Could not connect to the COMPORT");
            return false;
        }
        GetGlobalEnvironment()->PutInt(mstrResourceContextEnv,(int)hResourceContext);
        return true;
    }
}
