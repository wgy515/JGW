#include "StdAfx.h"
#include "TSE_QualcommFlashImageDownload.h"
#include "../TSE_DevicesGlobalResource.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>

#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/ctrim.h>

namespace JGW
{
    CTSE_QualcommFlashImageDownload::CTSE_QualcommFlashImageDownload(void) : mnTimeOut(120),mbIgnore(false)
    {
    }


    CTSE_QualcommFlashImageDownload::~CTSE_QualcommFlashImageDownload(void)
    {
    }

    const wchar_t* CTSE_QualcommFlashImageDownload::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QualcommFlashImageDownload\",\
                \"TestName\": \"镜像下载(Qualcomm)\",\
                \"Command\": \"下载命令\",\
                \"CommandOk\": \"下载完成后返回字符串判断,默认：All Finished Successfull\",\
                \"TimeOut\": \"下载超时时间\"\
                }";
    }

    bool CTSE_QualcommFlashImageDownload::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("Command"),strParamName))
        {
            mstrSrcCmd = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("CommandOk"),strParamName))
        {
            mstrCommandOk = strParamValue;
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

    void CTSE_QualcommFlashImageDownload::TSE_InitDownloadParam()
    {
        mDesFh_Loader_Cmd = mstrSrcCmd;
        int nDownloadPort = GetGlobalEnvironment()->GetInt(TSE_DOWNLOAD_PORT_PARAM);
        std::wstring strDownloadPort;
        JGW_FormatWString(strDownloadPort,L"\\\\.\\COM%d",nDownloadPort);

        JGW_ReplaceStringW(mDesFh_Loader_Cmd,L"$$TSE_DOWNLOAD_PORT_PARAM$$",strDownloadPort);

        JGW_ReplaceStringW(mDesFh_Loader_Cmd,L"$$TSE_DOWNLOAD_PATH_PARAM$$", GetGlobalEnvironment()->GetString(TSE_DOWNLOAD_PATH_PARAM));

        JGW_ReplaceStringW(mDesFh_Loader_Cmd,L"$$TSE_SAHARA_FIREHOSE_IMAGE_PARAM$$", GetGlobalEnvironment()->GetString(TSE_SAHARA_FIREHOSE_IMAGE_PARAM));

        JGW_ReplaceStringW(mDesFh_Loader_Cmd,L"$$TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM$$", GetGlobalEnvironment()->GetString(TSE_DOWNLOAD_PROVISION_XML_NAME_PARAM));

        JGW_ReplaceStringW(mDesFh_Loader_Cmd,L"$$TSE_RAWPROGRAM_XML_NAMES_PARAM$$", GetGlobalEnvironment()->GetString(TSE_RAWPROGRAM_XML_NAMES_PARAM));

        JGW_ReplaceStringW(mDesFh_Loader_Cmd,L"$$TSE_PATCH_XML_NAMES_PARAM$$", GetGlobalEnvironment()->GetString(TSE_PATCH_XML_NAMES_PARAM));
    }

    bool CTSE_QualcommFlashImageDownload::TSE_Init()
    {
        mbIgnore = false;
        if (!GetGlobalEnvironment()->GetBool(TSE_RESET_ENABLE_PARAM))
        {
            if (NULL != JGW_WStrCaseWStr(mstrSrcCmd.c_str(),L"--reset"))
            {
                mbIgnore = true;
            }
        }
        return false;
    }

    bool CTSE_QualcommFlashImageDownload::TSE_Run()
    {
        if (mbIgnore) return true;
        if (mstrSrcCmd.empty()) { LogE("Cmd命令不能为空\n");return false;}
        TSE_InitDownloadParam();
        //! 打开控制台
        CCJGW_ProcessPipe* pCMDPipe = CTSE_DevicesGlobalResource::GetProcessCMDPipe();
        if (!pCMDPipe)
        {
            LogE(L"打开控制台失败\n");
            return false;
        }

        jgw_timer tm;
        std::wstring strRead;
        while(tm.elapsed() < mnTimeOut)
        {
            if(!pCMDPipe->timed_send(mDesFh_Loader_Cmd,mnTimeOut)) { Sleep(100); continue;}
            CTSE_DevicesGlobalResource::ReadCMDProcessPipeEOF(pCMDPipe,strRead,mnTimeOut);
            if (NULL != JGW_WStrCaseWStr(strRead.c_str(),mstrCommandOk.c_str())) return true;
            else return false;
        }
        pCMDPipe->CloseProcessPipe();
        return false;
    }

    bool CTSE_QualcommFlashImageDownload::TSE_Exit()
    {
        return true;
    }
}