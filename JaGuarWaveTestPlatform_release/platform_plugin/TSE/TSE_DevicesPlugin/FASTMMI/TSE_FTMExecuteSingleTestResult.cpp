#include "StdAfx.h"
#include "TSE_FTMExecuteSingleTestResult.h"
#include "../TSE_DevicesGlobalResource.h"
#include <JGW_FoundationFunc/jgw_timer.hpp>

namespace JGW
{
    CTSE_FTMExecuteSingleTestResult::CTSE_FTMExecuteSingleTestResult(void) : mstrResultFilePath("d:\\mmi.res"),mstrRemoteFile("mmi.res"),mnCheckResultTimeOut(3000),mnTestWaitResult(100)
    {
    }

    CTSE_FTMExecuteSingleTestResult::~CTSE_FTMExecuteSingleTestResult(void)
    {
    }

    const wchar_t* CTSE_FTMExecuteSingleTestResult::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTMExecuteSingleTestResult\",\
                \"TestName\": \"FASTMMI执行单个测试并比较测试结果文件(INI文件格式)\",\
                \"TestCase\": \"FASTMMI测试项目名称TEST LIST，如LCD\",\
                \"Tip\": \"为空则不进行测试提示\",\
                \"TestWaitResult\":\"等待测试结果延时时间MS\",\
                \"CheckResultTimeOut\":\"检查测试结果超时时间\",\
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

    bool CTSE_FTMExecuteSingleTestResult::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Tip"))
        {
            mstrTip = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestCase"))
        {
            mwstrTestCase = strParamValue;
            mstrTestCase = JGW_W2A(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestWaitResult"))
        {
            mnTestWaitResult = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CheckResultTimeOut"))
        {
            mnCheckResultTimeOut = _ttoi(strParamValue);
        }
        else
        {
            AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_FTMExecuteSingleTestResult::TSE_Init()
    {
        InitCompareParam();
        return true;
    }

    bool CTSE_FTMExecuteSingleTestResult::TSE_Run()
    {
        if (!CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->FTM_AP_EXECUTE_SINGLE_TEST(mstrTestCase.c_str()))
        {
            LogE(L"发送测试Case失败，请检查当前Case是否存在\n");
            return false;
        }

        if (!mstrTip.empty() && IDOK  != MessageBox(GetGlobalMainHwnd(),mstrTip.c_str(),L"测试提示",MB_OKCANCEL))
        {
            return false;
        }
        Sleep(mnTestWaitResult);

        jgw_timer jt;

        while (mnCheckResultTimeOut >= jt.elapsed())
        {
            if (TSE_CheckTestResult()) return true;
            Sleep(200);
        }
        return false;
    }

    bool CTSE_FTMExecuteSingleTestResult::TSE_CheckTestResult()
    {
        if (0 == mnCompareIntCount && 0 == mnCompareStringCount && 0 == mnFindStringCount) return true;

        if (!CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->FTM_AP_READ_FILE(mstrResultFilePath.c_str(),mstrRemoteFile.c_str()))
        {
            LogE(L"读取测试结果文件失败\n");
            return false;
        }

        mResultIni.InitIniFilePath(JGW_A2W(mstrResultFilePath).c_str());
        if(!CompareStringResultFile() || !CompareIntResultFile() || !FindStringResultFile())
        {
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return true;
    }

    bool CTSE_FTMExecuteSingleTestResult::FindStringResultFile()
    {
        std::wstring strRead;
        for (std::vector<S_FTM_COMPARE_STRING_ATTR>::iterator it = mvFindStringAttrs.begin();it != mvFindStringAttrs.end(); ++ it)
        {
            strRead = mResultIni.GetIniKeyValue(mwstrTestCase.c_str(),it[0].mstrAttrName.c_str());
            if (NULL == JGW_WStrCaseWStr(strRead.c_str(),it[0].mstrAttrVal.c_str()))
            {
                LogE(it[0].mstrErrorMsg.c_str());
                return false;
            }
        }
        return true;
    }

    bool CTSE_FTMExecuteSingleTestResult::CompareStringResultFile()
    {
        std::wstring strRead;
        for (std::vector<S_FTM_COMPARE_STRING_ATTR>::iterator it = mvCompareStringAttrs.begin();it != mvCompareStringAttrs.end(); ++ it)
        {
            strRead = mResultIni.GetIniKeyValue(mwstrTestCase.c_str(),it[0].mstrAttrName.c_str());
            if (NULL != JGW_WStrComparenoCaseWStr(strRead.c_str(),it[0].mstrAttrVal.c_str()))
            {
                LogE(it[0].mstrErrorMsg.c_str());
                return false;
            }
        }
        return true;
    }

    bool CTSE_FTMExecuteSingleTestResult::CompareIntResultFile()
    {
        int nRead;
        for (std::vector<S_FTM_COMPARE_Int_ATTR>::iterator it = mvCompareIntAttrs.begin();it != mvCompareIntAttrs.end(); ++ it)
        {
            nRead = mResultIni.GetIniKeyIntValue(mwstrTestCase.c_str(),it[0].mstrAttrName.c_str());
            if (nRead < it[0].mnMinAttrVal || nRead > it[0].mnMaxAttrVal)
            {
                LogE(it[0].mstrErrorMsg.c_str());
                return false;
            }
        }
        return true;
    }
}