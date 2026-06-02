#include "StdAfx.h"
#include "TSE_FTMCheckTestResult.h"
#include "../TSE_DevicesGlobalResource.h"

namespace JGW
{
    CTSE_FTMCheckTestResult::CTSE_FTMCheckTestResult(void) : mstrResultFilePath("d:\\mmi.res"),mstrRemoteFile("mmi.res")
    {
    }


    CTSE_FTMCheckTestResult::~CTSE_FTMCheckTestResult(void)
    {
    }

    const wchar_t* CTSE_FTMCheckTestResult::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_FTMCheckTestResult\",\
                \"TestName\": \"比较FASTMMI测试结果文件(INI文件格式)\",\
                \"TestCase\": \"FASTMMI测试项目名称TEST LIST，如LCD\",\
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

    bool CTSE_FTMCheckTestResult::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"TestCase"))
        {
            mstrTestCase = strParamValue;
        }
        else
        {
           AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }

    bool CTSE_FTMCheckTestResult::TSE_Init()
    {
        InitCompareParam();
        return true;
    }

    bool CTSE_FTMCheckTestResult::TSE_Run()
    {
         if (CTSE_DevicesGlobalResource::GetQcommDiagActionIsntance()->FTM_AP_READ_FILE(mstrResultFilePath.c_str(),mstrRemoteFile.c_str())) return true;
		 Sleep(mnIntervalTimeMS);
		 return false;
    }

    bool CTSE_FTMCheckTestResult::TSE_Exit()
    {
        if (0 == mnCompareIntCount && 0 == mnCompareStringCount && 0 == mnFindStringCount) return true;
        mResultIni.InitIniFilePath(JGW_A2W(mstrResultFilePath).c_str());
        if(!CompareStringResultFile() || !CompareIntResultFile() || !FindStringResultFile())
        {
            Sleep(mnIntervalTimeMS);
            return false;
        }
        return true;
    }

    bool CTSE_FTMCheckTestResult::FindStringResultFile()
    {
        std::wstring strRead;
        for (std::vector<S_FTM_COMPARE_STRING_ATTR>::iterator it = mvFindStringAttrs.begin();it != mvFindStringAttrs.end(); ++ it)
        {
            strRead = mResultIni.GetIniKeyValue(mstrTestCase.c_str(),it[0].mstrAttrName.c_str());
            if (NULL == JGW_WStrCaseWStr(strRead.c_str(),it[0].mstrAttrVal.c_str()))
            {
                LogE(it[0].mstrErrorMsg.c_str());
                return false;
            }
        }
        return true;
    }

    bool CTSE_FTMCheckTestResult::CompareStringResultFile()
    {
        std::wstring strRead;
        for (std::vector<S_FTM_COMPARE_STRING_ATTR>::iterator it = mvCompareStringAttrs.begin();it != mvCompareStringAttrs.end(); ++ it)
        {
            strRead = mResultIni.GetIniKeyValue(mstrTestCase.c_str(),it[0].mstrAttrName.c_str());
            if (NULL != JGW_WStrComparenoCaseWStr(strRead.c_str(),it[0].mstrAttrVal.c_str()))
            {
                LogE(it[0].mstrErrorMsg.c_str());
                return false;
            }
        }
        return true;
    }

    bool CTSE_FTMCheckTestResult::CompareIntResultFile()
    {
        int nRead;
        for (std::vector<S_FTM_COMPARE_Int_ATTR>::iterator it = mvCompareIntAttrs.begin();it != mvCompareIntAttrs.end(); ++ it)
        {
            nRead = mResultIni.GetIniKeyIntValue(mstrTestCase.c_str(),it[0].mstrAttrName.c_str());
            if (nRead < it[0].mnMinAttrVal || nRead > it[0].mnMaxAttrVal)
            {
                LogE(it[0].mstrErrorMsg.c_str());
                return false;
            }
        }
        return true;
    }
}