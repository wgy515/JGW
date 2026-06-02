#include "StdAfx.h"
#include "TSE_MESGetIMEIInfo.h"
#include "CJGW_MESRestoreOperateID.hpp"
#include <regex>
namespace JGW
{
    CTSE_MESGetIMEIInfo::CTSE_MESGetIMEIInfo(void) : mstrIMEIEnvironment(_T("TSE_IMEI_NUMBER"))
    {
    }

    CTSE_MESGetIMEIInfo::~CTSE_MESGetIMEIInfo(void)
    {
    }

    const wchar_t* CTSE_MESGetIMEIInfo::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MESGetIMEIInfo\",\
                \"TestName\": \"获取包装IMEI/SN的相关MES信息以及检查当前号段状态\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"SNEnvironment\": \"组装SN保存环境变量名称，为空则不绑定SN\",\
                \"SNEnvironmentLen\": \"组装SN环境变量值限定的长度，为0或者为空则不限定长度\",\
                \"IMEIEnvironment\": \"包装IMEI/SN保存的环境变量名称\",\
                \"ParamNames\": \"包装IMEI/SN获取的相关MES信息参数组，参数名称: IMEI_M  ,IMEI_S  ,SN ,MAC ,BT,SpecialSN,OTPpwd, imeimo, RoutingStep,RoutingStep_Pre,RoutingStep_Next,Color, MEID,pESN,SONO:包装订单,MONOSMT:SMT工单,MONOPack：包装工单，MONOAssy： 组装工单，SIMInfo：SIMLock码，json格式] ,MSL,OTKSL\",\
                \"ParamValueRegexRule%d\": \"正则表达式规则 ParamValueRegexRule1,ParamValueRegexRule2....\",\
                \"EnvironmentValues\": \"将获取到参数信息保存到指定的环境变量组中，默认与ParamNames一一对应关系\"\
                }";
    }

    bool CTSE_MESGetIMEIInfo::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ParamNames"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrParamNames);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EnvironmentValues"))
        {
            JGW_ParserStrW(strParamValue,L",",mvstrEnvironmentValues);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"IMEIEnvironment"))
        {
            mstrIMEIEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperateID"))
        {
            mstrOperateID = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironmentLen"))
        {
            mnSNEnvironmentValLen = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SNEnvironment"))
        {
            mstrSNEnvironment = strParamValue;
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

    bool CTSE_MESGetIMEIInfo::TSE_Run()
    {
        GetGlobalEnvironment()->PutBool(TSE_MES_RETEST_FLAG,false);
        std::string strIMEI,strSN;
        std::vector<std::string> vParamNames,vParamVals;

        if (!mstrIMEIEnvironment.empty()) strIMEI = JGW_W2A(GetGlobalEnvironment()->GetString(mstrIMEIEnvironment));
        if (!mstrSNEnvironment.empty()) strSN = GetGlobalEnvironment()->GetAnsiString(mstrSNEnvironment);
        //! 判断字符串长度是否符合要求
        if (mnSNEnvironmentValLen > 0)
        {
            if (mnSNEnvironmentValLen != strSN.length())
            {
                LogE_F("读取SN为:%s,不等于%d位",strSN.c_str(),mnSNEnvironmentValLen);
                return false;
            }
        } 
        //! IMEI 与SN同时为空，判断失败
        if (strIMEI.empty() && strSN.empty())
        {
            LogE_F("LotPKSN(%s) And PCBSN(%s) Can not be empty",strIMEI.c_str(),strSN.c_str());
            return false;
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(strIMEI).c_str());
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
        //! 获取相关IMEI信息
        CCJGW_MESRestoreOperateID mesRestoreOperateID(mstrOperateID);
        if (!GetMESDataBaseAdo()->GetIMEIInfoToIMEI(strIMEI,strSN,vParamNames,vParamVals))
        {
            LogE(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            return false;
        }
        //！ 
        if (mvstrParamNames.size() != mvstrEnvironmentValues.size())
        {
            LogE(L"参数列表与环境变量列表不匹配");
            return false;
        }
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