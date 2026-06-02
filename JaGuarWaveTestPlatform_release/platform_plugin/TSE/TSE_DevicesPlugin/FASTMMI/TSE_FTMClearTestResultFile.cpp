#include "StdAfx.h"
#include "TSE_FTMClearTestResultFile.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_FTMClearTestResultFile::CTSE_FTMClearTestResultFile(void)
    {
    }


    CTSE_FTMClearTestResultFile::~CTSE_FTMClearTestResultFile(void)
    {
    }

    const wchar_t* CTSE_FTMClearTestResultFile::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTMClearTestResultFile\",\
                \"TestName\": \"清空FASTMMI测试结果信息\"\
                }";
    }

    bool CTSE_FTMClearTestResultFile::TSE_Run()
    {
        CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->FTM_AP_CLEAR_RESULTS();
        return true;
    }
}