#include "StdAfx.h"
#include "TSE_MESNextSNStation.h"
#include "CJGW_MESRestoreOperateID.hpp"
namespace JGW
{
    CTSE_MESNextSNStation::CTSE_MESNextSNStation(void) : mstrSNEnvironment(L"TSE_SN_NUMBER"),mnSNEnvironmentValLen(0)
    {
    }


    CTSE_MESNextSNStation::~CTSE_MESNextSNStation(void)
    {
    }

    const wchar_t* CTSE_MESNextSNStation::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MESNextSNStation\",\
                \"TestName\": \"组装SN MES过站\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"SNEnvironment\": \"组装SN保存环境变量名称\",\
                \"EnvironmentLen\": \"组装SN环境变量值限定的长度，为0或者为空则不限定长度\"\
                }";
    }

    bool CTSE_MESNextSNStation::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironment"))
        {
            mstrSNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperateID"))
        {
            mstrOperateID = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentLen"))
        {
            mnSNEnvironmentValLen = _ttoi(strParamValue);
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_MESNextSNStation::TSE_Run()
    {
        std::string strSN = JGW_W2A(GetGlobalEnvironment()->GetString(mstrSNEnvironment));
        if (mnSNEnvironmentValLen > 0 && mnSNEnvironmentValLen != strSN.length())
        {
            LogE_F("读取变量值为:%s,不等于%d位",strSN.c_str(),mnSNEnvironmentValLen);
            return false;
        }

        if (GetGlobalEnvironment()->GetBool(TSE_MES_RETEST_FLAG))
        {
            LogU(E_TSE_SHOW_CONTENT_INFO,L"重复测试不过MES");
            return true;
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(strSN).c_str());
        if (mstrOperateID.empty()) mstrOperateID = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId;
        CCJGW_MESRestoreOperateID mesRestoreOperateID(mstrOperateID);
        if (!GetMESDataBaseAdo()->NextStationToSN(strSN))
        {
            LogE(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            return false;
        }
        return true;
    }
}