#include "StdAfx.h"
#include "TSE_DownloadPDBToPeraso.h"
#include "..\TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_DownloadPDBToPeraso::CTSE_DownloadPDBToPeraso(void) : mbInitPDBInfo(false),mbDownloadOTP(true),mbDownloadConfig(true),mbDownloadToFile(false)
    {
    }

    CTSE_DownloadPDBToPeraso::~CTSE_DownloadPDBToPeraso(void)
    {
    }

    const wchar_t* CTSE_DownloadPDBToPeraso::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_DownloadPDBToPeraso\",\
                \"TestName\": \"下载PDB文件(Peraso)\",\
                \"InitPDBInfo\": \"初始化PDB信息每个测试序列必须初始化一次\",\
                \"DownloadToFile\": \"是否下载PDB Config到文件\",\
                \"DownloadConfig\": \"是否下载PDB Config文件\",\
                \"DownloadOTP\": \"是否下载PDB OTP文件\",\
                \"PDBHeaderFilePathEnvironment\": \"是否下载PDB HEADER文件\",\
                \"PDBBobyFilePathEnvironment\": \"是否下载PDB BOBY文件\"\
                }";
    }

    bool CTSE_DownloadPDBToPeraso::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"InitPDBInfo"))
        {
            mbInitPDBInfo = _ttoi(strParamValue)?true:false;
        }
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"DownloadOTP"))
        {
            mbDownloadOTP = _ttoi(strParamValue)?true:false;
        }
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"DownloadConfig"))
        {
            mbDownloadConfig = _ttoi(strParamValue)?true:false;
        }
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"DownloadToFile"))
        {
            mbDownloadToFile = _ttoi(strParamValue)?true:false;
        }
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PDBHeaderFilePathEnvironment"))
        {
            mstrPDBHeaderFilePathEnvironment = strParamValue;
        }
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"PDBBobyFilePathEnvironment"))
        {
            mstrPDBBobyFilePathEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_DownloadPDBToPeraso::TSE_Run()
    {
        if (mbInitPDBInfo && !CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->InitPDBInfoToDongle())
        {
            LogD(L"Init PDB Info To Dongles Fail\n");
            return false;
        }

        if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->BuildOTPCRC32())
        {
            LogE(L"Build PDB CRC Fail\n");
            return false;
        }

        if (mbDownloadToFile)
        {
            std::wstring strPDBHeaderFilePath = GetGlobalEnvironment()->GetString(mstrPDBHeaderFilePathEnvironment),strPDBBobyFilePath = mstrPDBBobyFilePathEnvironment.empty()?L"":GetGlobalEnvironment()->GetString(mstrPDBBobyFilePathEnvironment);
            if (mbDownloadOTP)
            {
                if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->SavePDBImageFileToDongleFlash(E_OTP,strPDBHeaderFilePath,strPDBBobyFilePath))
                {
                    LogE(L"Download OPT Image File Fail\n");
                    return false;
                }
            }

            if (mbDownloadConfig)
            {
                if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->SavePDBImageFileToDongleFlash(E_CONFIG,strPDBHeaderFilePath,strPDBBobyFilePath))
                {
                    LogE(L"Download OPT Image File Fail\n");
                    return false;
                }
            }
        }
        else
        {
            if (mbDownloadOTP)
            {
                if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->DownloadPDBImageFileToDongleFlash(E_OTP))
                {
                    LogE(L"Download OPT Image File Fail\n");
                    return false;
                }
            }

            if (mbDownloadConfig)
            {
                if (!CTSE_DevicesGlobalResource::GetDownloadPDBInstance()->DownloadPDBImageFileToDongleFlash(E_CONFIG))
                {
                    LogE(L"Download OPT Image File Fail\n");
                    return false;
                }
            }
        }
        
        return true;
    }
}
