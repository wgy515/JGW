#include "StdAfx.h"
#include <TSE_FoundationLib/TSE_CheckTestResult.h>
#include <TSE_FoundationLib/TSE_TestBase.h>

namespace JGW
{
    CTSE_CheckTestResult::CTSE_CheckTestResult(void) : mnCompareIntCount(0),mnCompareStringCount(0),mnFindStringCount(0),mbInitCompareParam(false)
    {
    }


    CTSE_CheckTestResult::~CTSE_CheckTestResult(void)
    {
    }

    const wchar_t* CTSE_CheckTestResult::GetCompareParamDescription()
    {
        return L"{\
                \"CompareStringCount\": \"比较多少项字符串\",\
				\"CompareStringName1\": \"key值 1是索引\",\
                \"ErrorCompareString1\": \"key值 1是索引,表示错误字符串信息\",\
                \"CompareStringValue1\": \"与KEY值对应VALUE进行比对\",\
                \"CompareStringErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"CompareIntCount\": \"比对多少项INT类型\",\
				\"CompareIntName1\": \"key值 1是索引\",\
                \"ErrorCompareInt1\": \"key值 1是索引,表示错误字符串信息\",\
                \"CompareIntValue1\": \"与KEY值对应VALUE进行比对,A~C,当前值在大于等于A小于等于C即可\",\
                \"CompareIntErrorMsg1\": \"比对失败后显示的错误字符串\",\
                \"FindStringCount\": \"查找字符串,需要匹配多少项\",\
				\"FindStringName1\": \"key值 1是索引\",\
                \"ErrorFindString1\": \"key值 1是索引,表示错误字符串信息\",\
                \"FindStringValue1\": \"查找当前值是否在key值对应字符串中\",\
                \"FindStringErrorMsg1\": \"比对失败后显示的错误字符串\"\
                }";
    }

    void CTSE_CheckTestResult::AddCompareParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"CompareStringCount"))
        {
            mnCompareStringCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"CompareIntCount"))
        {
            mnCompareIntCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"FindStringCount"))
        {
            mnFindStringCount = _ttoi(strParamValue);
        }
        else
        {
            mmapAttrNameVal[strParamName] = strParamValue;
        }

        mbInitCompareParam = false;
    }

    void CTSE_CheckTestResult::InitCompareParam()
    {
        if (!mbInitCompareParam)
        {
            std::wstring strName,strVal,strMsg,strIsError,strTemp;
            S_FTM_COMPARE_STRING_ATTR sFtmCompStrAttr;
            S_FTM_COMPARE_Int_ATTR sFtmCompIntAttr;
            std::map<std::wstring,std::wstring>::iterator it;

            mvCompareStringAttrs.clear();
            mvCompareIntAttrs.clear();
            mvFindStringAttrs.clear();

            for (size_t i = 1;i <= mnCompareStringCount;i ++)
            {
                sFtmCompStrAttr.mbIsErrorStringInfo = false;
				JGW_FormatWString(strName,L"CompareStringName%d",i);
                JGW_FormatWString(strIsError,L"ErrorCompareString%d",i);
                JGW_FormatWString(strVal,L"CompareStringValue%d",i);
                JGW_FormatWString(strMsg,L"CompareStringErrorMsg%d",i);
                if (mmapAttrNameVal.end() != mmapAttrNameVal.find(strIsError))
                {
                    sFtmCompStrAttr.mbIsErrorStringInfo =  (1 == _ttoi(mmapAttrNameVal[strIsError].c_str()));
                }
				sFtmCompStrAttr.mstrAttrName = mmapAttrNameVal[strName];
                sFtmCompStrAttr.mstrAttrVal = mmapAttrNameVal[strVal];
                sFtmCompStrAttr.mstrErrorMsg = mmapAttrNameVal[strMsg];
                mvCompareStringAttrs.push_back(sFtmCompStrAttr);
            }

            for (size_t i = 1;i <= mnCompareIntCount;i ++)
            {
                sFtmCompIntAttr.mbIsErrorIntInfo = false;
				JGW_FormatWString(strName,L"CompareIntName%d",i);
                JGW_FormatWString(strIsError,L"ErrorCompareInt%d",i);
                JGW_FormatWString(strVal,L"CompareIntValue%d",i);
                JGW_FormatWString(strMsg,L"CompareIntErrorMsg%d",i);
				sFtmCompIntAttr.mstrAttrName = mmapAttrNameVal[strName];
                if (mmapAttrNameVal.end() != mmapAttrNameVal.find(strIsError))
                {
                    sFtmCompIntAttr.mbIsErrorIntInfo =  (1 == _ttoi(mmapAttrNameVal[strIsError].c_str()));
                }
                sFtmCompIntAttr.mstrErrorMsg = mmapAttrNameVal[strMsg];
                strTemp = mmapAttrNameVal[strVal];
                _stscanf_s(strTemp.c_str(),L"%d~%d",&sFtmCompIntAttr.mnMinAttrVal,&sFtmCompIntAttr.mnMaxAttrVal);
                mvCompareIntAttrs.push_back(sFtmCompIntAttr);
            }

            for (size_t i = 1;i <= mnFindStringCount;i ++)
            {
                sFtmCompStrAttr.mbIsErrorStringInfo = false;
				JGW_FormatWString(strName,L"FindStringName%d",i);
                JGW_FormatWString(strIsError,L"ErrorFindString%d",i);
                JGW_FormatWString(strVal,L"FindStringValue%d",i);
                JGW_FormatWString(strMsg,L"FindStringErrorMsg%d",i);
				sFtmCompStrAttr.mstrAttrName = mmapAttrNameVal[strName];
                if (mmapAttrNameVal.end() != mmapAttrNameVal.find(strIsError))
                {
                    sFtmCompStrAttr.mbIsErrorStringInfo =  (1 == _ttoi(mmapAttrNameVal[strIsError].c_str()));
                }
                //sFtmCompStrAttr.mbIsErrorIntInfo =  (1 == _ttoi(mmapAttrNameVal[strName].c_str()));
                sFtmCompStrAttr.mstrAttrVal = mmapAttrNameVal[strVal];
                sFtmCompStrAttr.mstrErrorMsg = mmapAttrNameVal[strMsg];
                mvFindStringAttrs.push_back(sFtmCompStrAttr);
            }

            mbInitCompareParam = true;
        }
    }

    bool CTSE_CheckTestResult::IsCompareResult()
    {
        return !(0 == mnCompareIntCount && 0 == mnCompareStringCount && 0 == mnFindStringCount);
    }

    bool CTSE_CheckTestResult::CheckCompareResult(const std::wstring& strRead)
    {
        if (0 == mnCompareIntCount && 0 == mnCompareStringCount && 0 == mnFindStringCount) return true;

        return CompareStringResultFile(strRead) && CompareIntResultFile(strRead) && FindStringResultFile(strRead);
    }

    bool CTSE_CheckTestResult::FindStringResultFile(const std::wstring& strRead)
    {
        std::wstring strFind;
        for (std::vector<S_FTM_COMPARE_STRING_ATTR>::iterator it = mvFindStringAttrs.begin();it != mvFindStringAttrs.end(); ++ it)
        {
            strFind = it[0].mstrAttrVal;
            GetEnvironmentCommandString(strFind);
            if (it->mbIsErrorStringInfo)
            {               
                if (NULL != JGW_WStrCaseWStr(strRead.c_str(),strFind.c_str()))
                {
                    LogE(it[0].mstrErrorMsg.c_str());
                    LogE_F(L"Error Read: %s",strRead.c_str());
                    return false;
                }
            }
            else
            {
                if (NULL == JGW_WStrCaseWStr(strRead.c_str(),strFind.c_str()))
                {
                    LogE(it[0].mstrErrorMsg.c_str());
                    LogE_F(L"Read: %s,Config: %s",strRead.c_str(),strFind.c_str());
                    return false;
                }
            }
        }
        return true;
    }

    bool CTSE_CheckTestResult::CompareStringResultFile(const std::wstring& strRead)
    {
        std::wstring strFind;
        for (std::vector<S_FTM_COMPARE_STRING_ATTR>::iterator it = mvCompareStringAttrs.begin();it != mvCompareStringAttrs.end(); ++ it)
        {
            strFind = it[0].mstrAttrVal;
            GetEnvironmentCommandString(strFind);
            if (it->mbIsErrorStringInfo)
            {
                if (NULL == JGW_WStrComparenoCaseWStr(strRead.c_str(),strFind.c_str()))
                {
                    LogE(it[0].mstrErrorMsg.c_str());
                    LogE_F(L"Error Read: %s",strRead.c_str());
                    return false;
                }
            }
            else
            {
                if (NULL != JGW_WStrComparenoCaseWStr(strRead.c_str(),strFind.c_str()))
                {
                    LogE(it[0].mstrErrorMsg.c_str());
                    LogE_F(L"Read: %s,Config: %s",strRead.c_str(),strFind.c_str());
                    return false;
                }
            }       
        }
        return true;
    }

    bool CTSE_CheckTestResult::CompareIntResultFile(const std::wstring& strRead)
    {
        int nRead;
		std::wstring strTemp(strRead);
		JGW_EraseLastAndFristTwoCharsW(strTemp);
		JGW_EraseLastAndFristChars(strTemp,' ');
        for (std::vector<S_FTM_COMPARE_Int_ATTR>::iterator it = mvCompareIntAttrs.begin();it != mvCompareIntAttrs.end(); ++ it)
        {
            nRead = _ttoi(strTemp.c_str());
            if (it->mbIsErrorIntInfo)
            {
                if (nRead >= it[0].mnMinAttrVal && nRead <= it[0].mnMaxAttrVal)
                {
                    LogE(it[0].mstrErrorMsg.c_str());
                    LogE_F(L"Read(%s): %d(< %d || > %d)",strTemp.c_str(),nRead,it[0].mnMinAttrVal,it[0].mnMaxAttrVal);
                    return false;
                }
            }
            else
            {
                if (nRead < it[0].mnMinAttrVal || nRead > it[0].mnMaxAttrVal)
                {
                    LogE(it[0].mstrErrorMsg.c_str());
                    LogE_F(L"Read(%s): %d,Config: Max: %d,Min: %d",strTemp.c_str(),nRead,it[0].mnMaxAttrVal,it[0].mnMinAttrVal);
                    return false;
                }
            } 
        }
        return true;
    }
}
