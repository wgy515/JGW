#include "StdAfx.h"
#include "TSE_MESGetSNInfo.h"
#include "CJGW_MESRestoreOperateID.hpp"
#include <regex>
namespace JGW
{

    CTSE_MESGetSNInfo::CTSE_MESGetSNInfo(void) : mbIsSMTMono(false)
    {
    }


    CTSE_MESGetSNInfo::~CTSE_MESGetSNInfo(void)
    {
    }

    const wchar_t* CTSE_MESGetSNInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MESGetSNInfo\",\
                \"TestName\": \"获取PCBA SN信息 以及检查当前号段状态\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"PCBASNEnvironment\": \"组装SN保存环境变量名称，为空则不绑定SN\",\
                \"IsSMTMono\": \"SMT MONO def:0\",\
                \"PCBASNEnvironmentLen\": \"组装SN环境变量值限定的长度，为0或者为空则不限定长度\",\
                \"ParamNames\": \"参数列表[IMEI_M,IMEI_S,SN,MAC,BT,SpecialSN,OTPpwd,RoutingStep,RoutingStep_Pre,RoutingStep_Next,imeimo,MONOSMT,MONOAssy,MONOPack，SONO,CPNSMT] IMEI_M:主IMEI；IMEI_S：副imei；SN：整机SN；MAC：wifimac ；BT：BTMAC；OTPpwd:；RoutingStep：当前工艺步骤,RoutingStep_Pre：前一工艺步骤；RoutingStep_Next：下一工艺步骤；imeimo：IMEI工单；MONOSMT:SMT工单；MONOAssy；组装工单；MONOPack：包装工单；SONO：订单；CPNSMT：SMT客户料号;\",\
                \"ParamValueRegexRule%d\": \"正则表达式规则 ParamValueRegexRule1,ParamValueRegexRule2....\",\
                \"EnvironmentValues\": \"将获取到参数信息保存到指定的环境变量组中，默认与ParamNames一一对应关系\"\
                }";
    }

    bool CTSE_MESGetSNInfo::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ParamNames"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrParamNames);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IsSMTMono"))
        {
            mbIsSMTMono = 1 == _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentValues"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrEnvironmentValues);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperateID"))
        {
            mstrOperateID = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PCBASNEnvironmentLen"))
        {
            mnSNEnvironmentValLen = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PCBASNEnvironment"))
        {
            mstrPCBASNEnvironment = strParamValue;
        }
        else
        {
            if (NULL != strParamName && strParamValue != NULL && NULL != JGW_WStrCaseWStr(strParamName,L"ParamValueRegexRule"))
            {
                mmapstrParamValuesRegexRule[strParamName] = strParamValue;
                return true;
            }
            return false;
        }

        return true;
    }

    bool CTSE_MESGetSNInfo::TSE_Run()
    {
        GetGlobalEnvironment()->PutBool(TSE_MES_RETEST_FLAG,false);
        std::vector<std::string> vParamNames,vParamVals;
        std::string strSN = "";
        if (!mstrPCBASNEnvironment.empty()) strSN = GetGlobalEnvironment()->GetAnsiString(mstrPCBASNEnvironment);
        if (mnSNEnvironmentValLen > 0)
        {
            if (mnSNEnvironmentValLen != strSN.length())
            {
                LogE_F("读取变量值为:%s,不等于%d位",strSN.c_str(),mnSNEnvironmentValLen);
                return false;
            }
        } 

        if (strSN.empty())
        {
            LogE_F("PCBSN(%s) Can not be empty",strSN.c_str());
            return false;
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(strSN).c_str());
        if (mstrOperateID.empty()) mstrOperateID = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId;
        //! 判断当前操作ID是否处于离线或者-1状态
        if (E_NOTONLINE_MES != GetMESDataBaseAdo()->GetMESServiceSocketConnect()->msMesConnectInfo.meMESModel &&  0 != mstrOperateID.compare("-1"))
        {
            GetGlobalEnvironment()->PutAnsiString(TSE_MES_TASK_NUMBER,GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrTaskNumber);
        }

        if (GetMESDataBaseAdo()->CheckMESStatus())
        {
            //LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,L"警告:当前站点不经过MES流程(MES处于离线模式或者当前站点未分配ID)");
            LogE(L"警告:当前站点不经过MES流程(MES处于离线模式或者当前站点未分配ID)");
            return true;
        }

        CCJGW_MESRestoreOperateID mesRestoreOperateID(mstrOperateID);
        if (!GetMESDataBaseAdo()->GetPCBSNInfoToWorkOrderNumber(strSN,vParamNames,vParamVals,mbIsSMTMono))
        {
            LogE(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            return false;
        }

        if (mvstrParamNames.size() != mvstrEnvironmentValues.size())
        {
            LogE(L"参数列表与环境变量列表不匹配");
            return false;
        }
        //! 查找判断参数是否正确
        bool bFind = true;
        std::wstring strKey,strValue,strTemp,strNumberInfo;
        for (size_t i = 0;i < mvstrParamNames.size();i ++)
        {
            bFind = false;
            for (size_t y = 0;y < vParamNames.size();y ++)
            {
                if (NULL == JGW_WStrComparenoCaseWStr(mvstrParamNames[i].c_str(),JGW_A2W(vParamNames[y]).c_str()))
                {
                    strKey = mvstrEnvironmentValues[i];
                    strValue = JGW_A2W(vParamVals[y]);
                    JGW_FormatWString(strTemp,L"%s : %s",mvstrEnvironmentValues[i].c_str(),strValue.c_str());
                    LogI(strTemp.c_str());
                    LogU(E_TSE_SHOW_TEST_INFO_TO_RICHEDIT,strTemp.c_str());
                    if (strNumberInfo.empty())
                    {
                        strNumberInfo = strValue;
                    }
                    else
                    {
                        strNumberInfo += L",";
                        strNumberInfo += strValue;
                    }
                    GetGlobalEnvironment()->PutString(mvstrEnvironmentValues[i],strValue);
                    bFind = true;
                }
            }
            if (!bFind)
            {
                LogE_F(L"获取%s号段信息失败",mvstrParamNames[i].c_str());
                return false;
            }
            JGW_FormatWString(strTemp,L"ParamValueRegexRule%d",i + 1);
            std::map<std::wstring,std::wstring>::iterator it = mmapstrParamValuesRegexRule.find(strTemp);
            if (it != mmapstrParamValuesRegexRule.end() && !it->second.empty())
            {
                std::wregex partten(it->second);
                if (!regex_match(strValue,partten))
                {
                    LogE_F(L"param:%s,rule:%s,value:%s,regex_match fail",mvstrParamNames[i].c_str(),it->second.c_str(),strValue.c_str());
                    return false;
                }
            }
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strNumberInfo.c_str());

        return true;
    }
}
