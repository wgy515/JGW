#include "StdAfx.h"
#include "TSE_QualcommFlashImageDownloadV2.h"
#include "../TSE_DevicesGlobalResource.h"
#include <TSE_SequenceManagerPlugin/GlobalEnvironmentName_Define.h>

#include <JGW_WindowsFuncPlugin/CJGW_ProcessPipe.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>
#include <JGW_FoundationFunc/ctrim.h>

namespace JGW
{

    CTSE_QualcommFlashImageDownloadV2::CTSE_QualcommFlashImageDownloadV2(void) : mbIgnore(false)
    {
    }


    CTSE_QualcommFlashImageDownloadV2::~CTSE_QualcommFlashImageDownloadV2(void)
    {
    }


    const wchar_t* CTSE_QualcommFlashImageDownloadV2::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_QualcommFlashImageDownloadV2\",\
                \"TestName\": \"镜像下载(Qualcomm)\",\
                \"Command\": \"下载命令\",\
                \"TimeOut\": \"下载超时时间\",\
                \"CompareStringCount\": \"比较多少项字符串\",\
                \"CompareStringName1\": \"key值 1是索引\",\
                \"CompareStringValue1\": \"与KEY值对应VALUE进行比对\",\
                \"CompareStringErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"CompareIntCount\": \"比对多少项INT类型\",\
                \"CompareIntName1\": \"key值 1是索引\",\
                \"CompareIntValue1\": \"与KEY值对应VALUE进行比对,A~C,当前值在大于等于A小于等于C即可\",\
                \"CompareIntErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"FindStringCount\": \"查找字符串,需要匹配多少项\",\
                \"FindStringName1\": \"key值 1是索引\",\
                \"FindStringValue1\": \"查找当前值是否在key值对应字符串中\",\
                \"FindStringErrorMsg1\": \"比对失败后显示的错误字符串\"\
                }";
    }

    bool CTSE_QualcommFlashImageDownloadV2::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if(TSE_PARAM_NAME_EQUAL(_T("Command"),strParamName))
        {
            mstrSrcCmd = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(_T("TimeOut"),strParamName))
        {
            mnTimeOut = _ttoi(strParamValue);
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_QualcommFlashImageDownloadV2::TSE_Init()
    {
        mbIgnore = false;
        mCheckTestResult.InitCompareParam();
        if (!GetGlobalEnvironment()->GetBool(TSE_RESET_ENABLE_PARAM))
        {
            if (NULL != JGW_WStrCaseWStr(mstrSrcCmd.c_str(),L"--reset"))
            {
                mbIgnore = true;
            }
        }
        return false;
    }

    bool CTSE_QualcommFlashImageDownloadV2::TSE_Run()
    {
        if (mbIgnore) return true;
        if (mstrSrcCmd.empty()) { LogE("Cmd命令不能为空\n");return false;}

        mDesFh_Loader_Cmd = mstrSrcCmd;
        GetEnvironmentCommandString(mDesFh_Loader_Cmd);
        //! TSE_InitDownloadParam();
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
            if (mCheckTestResult.CheckCompareResult(strRead)) return true;
            else return false;
        }
        pCMDPipe->CloseProcessPipe();
        return false;
    }
}
