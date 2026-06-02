#include "StdAfx.h"
#include "TSE_SaharaArmPrgDownload.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>

namespace JGW
{
    CTSE_SaharaArmPrgDownload::CTSE_SaharaArmPrgDownload(void) : mstrResourceContextEnv(L"TSE_RESOURCE_CONTEXT")
    {
    }


    CTSE_SaharaArmPrgDownload::~CTSE_SaharaArmPrgDownload(void)
    {
    }

    const wchar_t* CTSE_SaharaArmPrgDownload::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaharaArmPrgDownload\",\
                \"TestName\": \"Sahara Arm Prg Download\",\
                \"ResourceContextEnv\": \"get open port handle environment\",\
                \"ArmPrgDownloadFilePathCmd\": \"ARMPRG file path name. For example for 9x25 MTP, NPRG9x25.mbn(regular ARMPRG) or ENPRG9x25.mbn (emergency downloader)\"\
                }";
    }

    bool CTSE_SaharaArmPrgDownload::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ArmPrgDownloadFilePathCmd"))
        {
            mstrArmPrgDownloadFilePathCmd = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaharaArmPrgDownload::TSE_Run()
    {
        std::wstring strArmPrgDownloadFilePath (mstrArmPrgDownloadFilePathCmd);
        GetEnvironmentCommandString(strArmPrgDownloadFilePath);
        if (!JGW_PathFileExists(strArmPrgDownloadFilePath.c_str()))
        {
            LogE_F(L"download Flash Programmer(%s) is not Exists",strArmPrgDownloadFilePath.c_str());
            return false;
        }

        HANDLE hResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnv);
        if (NULL == hResourceContext)
        {
            LogE_F(L"Could not connect to the Resource Context");
            return false;
        }

        QLIB_QPHONEMS_UploadSBMultiImage_ConfigureCallBack(hResourceContext,swdlQPHONEMSCB_Func);
        unsigned char result = QLIB_QPHONEMS_Sahara_FlashProgrammer(hResourceContext, const_cast<char*>(JGW_W2A(strArmPrgDownloadFilePath).data()));
        if (TRUE != result)
        {
            LogE(L"Unable to download Flash Programmer using Sahara Protocol");
            return false;
        }
        return true;
    }
}
