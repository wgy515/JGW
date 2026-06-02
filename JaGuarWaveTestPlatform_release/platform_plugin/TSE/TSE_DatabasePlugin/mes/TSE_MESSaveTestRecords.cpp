#include "StdAfx.h"
#include "TSE_MESSaveTestRecords.h"
#include "CJGW_MESRestoreOperateID.hpp"

namespace JGW
{
    std::string CTSE_MESSaveTestRecords::mstrEndTestTime;
    std::string CTSE_MESSaveTestRecords::mstrStartTestTime;

    CTSE_MESSaveTestRecords::CTSE_MESSaveTestRecords(void) : mbUpdateStartTestTime(false),mbUpdateEndTestTime(false)
    {
    }


    CTSE_MESSaveTestRecords::~CTSE_MESSaveTestRecords(void)
    {
    }

    const wchar_t* CTSE_MESSaveTestRecords::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MESSaveTestRecords\",\
                \"TestName\": \"组装SN MES过站\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"IMEISNEnvironment\": \"IMEI/SN保存的环境变量名称\",\
                \"StartTestTime\": \"开始测试时间标记位，1：重新记录开始测试时间，0：使用上次调用CTSE_MESSaveTestRecords的测试件\",\
                \"EndTestTime\": \"结束测试时间标记位，1：结束测试标记位，表示当前测试完成，上传测试记录相关信息 0：则表示当前为起始测试标记位\"\
                }";
    }

    bool CTSE_MESSaveTestRecords::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"IMEISNEnvironment"))
        {
            mstrSNIMEIEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperateID"))
        {
            mstrOperateID = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentLen"))
        {
            mnSNEnvironmentValLen = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"StartTestTime"))
        {
            mbUpdateStartTestTime = _ttoi(strParamValue)?true:false;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EndTestTime"))
        {
            mbUpdateEndTestTime = _ttoi(strParamValue)?true:false;
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_MESSaveTestRecords::TSE_Run()
    {
        if (mbUpdateStartTestTime) mstrStartTestTime = JGW_W2A(JGW_GetCurrentTimeStamp());
        if (!mbUpdateEndTestTime) return true;
        if (mstrStartTestTime.empty()) return false;

        mstrEndTestTime = JGW_W2A(JGW_GetCurrentTimeStamp());
        std::string strSN = JGW_W2A(GetGlobalEnvironment()->GetString(mstrSNIMEIEnvironment));
        if (mnSNEnvironmentValLen > 0 && mnSNEnvironmentValLen != strSN.length())
        {
            LogE_F("读取变量值为:%s,不等于%d位",strSN.c_str(),mnSNEnvironmentValLen);
            return false;
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(strSN).c_str());
        if (mstrOperateID.empty()) mstrOperateID = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId;
        CCJGW_MESRestoreOperateID mesRestoreOperateID(mstrOperateID);
        //! mstrStartTestTime = "";
        if (!GetMESDataBaseAdo()->SaveTestRecordsMES(strSN,mstrStartTestTime,mstrEndTestTime,GetGlobalEnvironment()->GetInt(TSE_TEST_RESULT)?"PASS":"FAIL","","ADD"))
        {
            LogE(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
			mstrStartTestTime = "";
            return false;
        }
		mstrStartTestTime = "";
        return true;
    }
}