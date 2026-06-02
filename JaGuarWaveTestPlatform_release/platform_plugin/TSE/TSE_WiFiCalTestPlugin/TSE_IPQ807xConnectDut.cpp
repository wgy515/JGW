#include "StdAfx.h"
#include "TSE_IPQ807xConnectDut.h"

namespace JGW
{
    CTSE_IPQ807xConnectDut::CTSE_IPQ807xConnectDut(void) : mbUseQPST(true),mnWlanID(8070),mhResourceContext(NULL),meType(QLIB_TARGET_TYPE_APQ),mstrComPort("COM30001"),mcDeviceApi(NULL,true)
    {
    }


    CTSE_IPQ807xConnectDut::~CTSE_IPQ807xConnectDut(void)
    {
    }

    const wchar_t* CTSE_IPQ807xConnectDut::TSE_GetParamDescription()
    {
        return L"";
    }

    bool CTSE_IPQ807xConnectDut::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if(CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("Type"),strParamName))
        {
            meType = (QLIB_TargetType_Enum)_ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("ComPort"),strParamName))
        {
            mstrComPort = JGW_W2A(strParamValue);
            JGW_MakeStringToUpper(mstrComPort);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("WlanID"),strParamName))
        {
            mnWlanID = _ttoi(strParamValue);
        }
        else if(TSE_PARAM_NAME_EQUAL(_T("UseQPST"),strParamName))
        {
            mbUseQPST = 1 == _ttoi(strParamValue);
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_IPQ807xConnectDut::TSE_Run()
    {
        if (mbUseQPST)
        {
            QLIB_SetLibraryMode(1);
            QLIB_SetTargetType( meType );
        }
        else
        {
            QLIB_SetLibraryMode(0);
            QLIB_SetTargetType(meType);
        }
        unsigned short comPort = QLIB_COM_AUTO_DETECT;
        if (0 == mstrComPort.compare("AUTO"))
        {
            comPort = QLIB_COM_AUTO_DETECT;
        }
        else
        {
            JGW_ReplaceStringA(mstrComPort,"COM","");
            comPort = static_cast<unsigned short>(atoi(mstrComPort.c_str()));
        }
        bool result = mcDeviceApi.ConnectPhone(60,comPort);
        mhResourceContext = mcDeviceApi.GetResourceContext();
        GetPhoneInstance()->SetResourceContent(mhResourceContext);
        return result;
    }
}

