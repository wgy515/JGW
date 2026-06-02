#include "StdAfx.h"
#include "TSE_MESCheckSNStatus.h"
#include "CJGW_MESRestoreOperateID.hpp"

namespace JGW
{
    CTSE_MESCheckSNStatus::CTSE_MESCheckSNStatus(void) : mstrSNEnvironment(L"TSE_SN_NUMBER"),mnSNEnvironmentValLen(0),mbIsSMTMono(false)
    {
    }


    CTSE_MESCheckSNStatus::~CTSE_MESCheckSNStatus(void)
    {
    }

    const wchar_t* CTSE_MESCheckSNStatus::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MESCheckSNStatus\",\
                \"TestName\": \"MES检查SN当前状态\",\
                \"SNEnvironment\": \"SN保存环境变量名称\",\
                \"IsSMTMono\": \"SMT MONO def:0\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"EnvironmentLen\": \"SN环境变量值限定的长度，为0或者为空则不限定长度\"\
                }";
    }

    bool CTSE_MESCheckSNStatus::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
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
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsSMTMono"))
        {
            mbIsSMTMono = 1 == _ttoi(strParamValue);
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

    bool CTSE_MESCheckSNStatus::TSE_Run()
    {
        std::string strSN = GetGlobalEnvironment()->GetAnsiString(mstrSNEnvironment);
        GetGlobalEnvironment()->PutBool(TSE_MES_RETEST_FLAG,false);
        if (mnSNEnvironmentValLen > 0 && mnSNEnvironmentValLen != strSN.length())
        {
            LogE_F("读取变量值为:%s,不等于%d位",strSN.c_str(),mnSNEnvironmentValLen);
            return false;
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(strSN).c_str());
        //! 判断离线和在线
        if (mstrOperateID.empty()) mstrOperateID = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId;
        //! 判断当前操作ID是否处于离线或者-1状态
        if (E_NOTONLINE_MES != GetMESDataBaseAdo()->GetMESServiceSocketConnect()->msMesConnectInfo.meMESModel &&  0 != mstrOperateID.compare("-1"))
        {
            GetGlobalEnvironment()->PutAnsiString(TSE_MES_TASK_NUMBER,GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrTaskNumber);
        }

        CCJGW_MESRestoreOperateID mesRestoreOperateID(mstrOperateID);
        if (GetMESDataBaseAdo()->CheckSNStatus(strSN,mbIsSMTMono)) return true;
        LogE(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
        /*if (GetGlobalEnvironment()->GetBool(TSE_MES_ALLOW_RETEST))
        {
        if (IDOK == MessageBox(GetGlobalMainHwnd(),L"当前号段下一站点不属于当前测试站点\n是否要继续进行测试请点击确定按钮(不会过MES站点)",L"当前号段不属于当前站点",MB_OKCANCEL))
        {
        GetGlobalEnvironment()->PutBool(TSE_MES_RETEST_FLAG,true);
        return true;
        }
        }*/
        return false;
    }
}