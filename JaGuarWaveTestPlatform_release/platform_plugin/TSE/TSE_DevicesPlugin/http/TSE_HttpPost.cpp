#include "StdAfx.h"
#include "TSE_HttpPost.h"
#include <JGW_CurlPlugin/CJGW_CurlHttp.h>
namespace JGW
{
    CTSE_HttpPost::CTSE_HttpPost(void) : mbWriteOnly(false)
    {
    }


    CTSE_HttpPost::~CTSE_HttpPost(void)
    {
    }

    const wchar_t* CTSE_HttpPost::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_HttpPost\",\
                \"TestName\": \"发送Curl Post请求\",\
                \"SourceAddrEnvironment\": \"发送url路由出口\",\
                \"Url\": \"http url\",\
                \"PostData\": \"url请求参数\",\
                \"PostDataEnvrionment\": \"url PostData environment\",\
                \"Header\": \"http header\",\
                \"WriteOnly\": \"只写不读标记位\",\
                \"SaveReceiveEnvironment\": \"Save Receive Environment\",\
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

    bool CTSE_HttpPost::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"Url"))
        {
            mstrUrl = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PostData"))
        {
            mstrPostData = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Header"))
        {
            mstrHeader = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteOnly"))
        {
            mbWriteOnly = (1 == _ttoi(strParamValue));
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PostDataEnvrionment"))
        {
            mstrPostDataEnvrionment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SourceAddrEnvironment"))
        {
            mstrSourceAddrEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveReceiveEnvironment"))
        {
            mstrSaveReceiveEnvironment = strParamValue;
        }
        else
        {
            mCheckTestResult.AddCompareParam(strParamName,strParamValue);
        }
        return true;
    }


    bool CTSE_HttpPost::TSE_Init()
    {
        mCheckTestResult.InitCompareParam();
        return true;
    }

    bool CTSE_HttpPost::TSE_Run()
    {
        std::string strSourceAddr = GetGlobalEnvironment()->GetAnsiString(mstrSourceAddrEnvironment);
        std::wstring strPostData = mstrPostDataEnvrionment.empty()?mstrPostData:GetGlobalEnvironment()->GetString(mstrPostDataEnvrionment),strResponce,strErrorMsg;
        CCJGW_CurlHttp curlHttp(strSourceAddr);

        if (mbWriteOnly)
        {
            if (!curlHttp.SendCurlHttpPostData(mstrUrl,strPostData,strResponce,strErrorMsg,mstrHeader))
            {
                strResponce = strErrorMsg;
            }
        }
        else
        {
            if (!curlHttp.SendCurlHttpPostData(mstrUrl,strPostData,strResponce,strErrorMsg,mstrHeader))
            {
                LogE_F(L"Http Post Fail:%s",strErrorMsg.c_str());
                return false;
            }
        }

        LogI_F(L"Recv Http Post Msg:%s",strResponce.c_str());
        if (!mstrSaveReceiveEnvironment.empty())
        {
            LogI_F(L"%s:%s",mstrSaveReceiveEnvironment.c_str(),strResponce.c_str());
            GetGlobalEnvironment()->PutString(mstrSaveReceiveEnvironment,strResponce);
        }

        return mCheckTestResult.CheckCompareResult(strResponce);
    }
}

