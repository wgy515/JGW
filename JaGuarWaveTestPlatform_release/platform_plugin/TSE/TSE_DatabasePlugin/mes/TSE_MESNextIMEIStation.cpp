#include "StdAfx.h"
#include "TSE_MESNextIMEIStation.h"
#include "CJGW_MESRestoreOperateID.hpp"

namespace JGW
{
    CTSE_MESNextIMEIStation::CTSE_MESNextIMEIStation(void) : mnPCBASNEnvironmentLen(0)
    {
    }

    CTSE_MESNextIMEIStation::~CTSE_MESNextIMEIStation(void)
    {
    }

    const wchar_t* CTSE_MESNextIMEIStation::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MESNextIMEIStation\",\
                \"TestName\": \"包装IMEI/SN MES过站\",\
                \"OperateID\": \"MES的站点ID，如果不配置或者为空则取插件的站点ID\",\
                \"PCBASNEnvironment\": \"组装SN保存环境变量名称，为空则不绑定SN\",\
                \"PCBASNEnvironmentLen\": \"组装SN环境变量值限定的长度，为0或者为空则不限定长度\",\
                \"IMEISNEnvironment\": \"包装IMEI/SN保存的环境变量名称\",\
                \"SoftWareVarTinnoEnv\": \"内部软件版本（若非需要保存IMEI与SN对应关系的写号节点，此项为空）\",\
                \"SoftWareVarCustomerEnv\": \"客户软件版本（若非需要保存IMEI与SN对应关系的写号节点，此项为空）\",\
                \"RemarkCommand\": \"备注信息，该信息将写入包装过站时的事务表备注中（长度不可以超过200字符串）,如对号写号可写入：(写IMEI 采用扫描SN,校验SN； 校验版本，CIT：Y，ANT：Y，GPS/WIFI：Y；GPS Data：N；IMEI LOCK：N；ATTKFlag：N；Reset：Y；OTP写号异常处理：N；)标记信息 可为空在字段，若不用，可以直接不传该参数 def:{\\\\\\\"SN\\\\\\\":\\\\\\\"123456\\\\\\\"}\",\
                \"UnlockcodeCommand\": \"解锁码，写号完，某些机型需要工具上传Unlockcode信息给MES保存 可为空在字段，若不用，可以直接不传该参数。def:{\\\\\\\"SN\\\\\\\":\\\\\\\"123456\\\\\\\"}\",\
                \"ImeiInfoCommand\": \"Imei表其他信息，写号完，某些机型需要工具上传Imei表其他信息（如ESIM、NFC等信息）给MES保存 可为空在字段，若不用，可以直接不传该参数 def:{\\\\\\\"SN\\\\\\\":\\\\\\\"123456\\\\\\\"}\"\
                }";
    }

    bool CTSE_MESNextIMEIStation::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"IMEISNEnvironment"))
        {
            mstrIMEISNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PCBASNEnvironmentLen"))
        {
            mnPCBASNEnvironmentLen = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"PCBASNEnvironment"))
        {
            mstrPCBASNEnvironment = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SoftWareVarTinnoEnv"))
        {
            mstrSoftWareVarTinnoEnv = strParamValue; 
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperateID"))
        {
            mstrOperateID = JGW_W2A_W(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SoftWareVarCustomerEnv"))
        {
            mstrSoftWareVarCustomerEnv = strParamValue; 
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"RemarkCommand"))
        {
            mstrRemarkCommand = strParamValue; 
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"UnlockcodeCommand"))
        {
            mstrUnlockcodeCommand = strParamValue; 
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ImeiInfoCommand"))
        {
            mstrImeiInfoCommand = strParamValue; 
        }
        else 
        {
            return false;
        }
        return true;
    }

    bool CTSE_MESNextIMEIStation::TSE_Run()
    {
        std::string strIMEISN = JGW_W2A(GetGlobalEnvironment()->GetString(mstrIMEISNEnvironment));
        if (strIMEISN.empty())
        {
            LogE_F("读取IMEISN变量值为:%s,不能为空",strIMEISN.c_str());
            return false;
        }

        std::string strSN = "";
        if (!mstrPCBASNEnvironment.empty()) strSN = JGW_W2A(GetGlobalEnvironment()->GetString(mstrPCBASNEnvironment));

        if (mnPCBASNEnvironmentLen > 0)
        {
            if (mnPCBASNEnvironmentLen != strSN.length())
            {
                LogE_F("读取PCBA SN变量值为:%s,不等于%d位",strSN.c_str(),mnPCBASNEnvironmentLen);
                return false;
            }
        }

        if (GetGlobalEnvironment()->GetBool(TSE_MES_RETEST_FLAG))
        {
            LogU(E_TSE_SHOW_CONTENT_INFO,L"重复测试不过MES");
            return true;
        }

        LogU(E_TSE_SHOW_CONTENT_INFO,JGW_A2W(strIMEISN).c_str());
        if (mstrOperateID.empty()) mstrOperateID = GetMESDataBaseAdo()->GetMESServiceSocketConnect()->mstrOperationId;
        CCJGW_MESRestoreOperateID mesRestoreOperateID(mstrOperateID);

        std::string strSoftWareVarTinno = mstrSoftWareVarTinnoEnv.empty()?"":GetGlobalEnvironment()->GetAnsiString(mstrSoftWareVarTinnoEnv);
        std::string strSoftWareVarCustomer = mstrSoftWareVarCustomerEnv.empty()?"":GetGlobalEnvironment()->GetAnsiString(mstrSoftWareVarCustomerEnv);

        std::wstring strRemark(mstrRemarkCommand),strUnlockcode(mstrUnlockcodeCommand),strImeiInfo(mstrImeiInfoCommand);
        std::string strRemarkAnsi,strUnlockcodeAnsi,strImeiInfoAnsi;
        if (!strRemark.empty())
        {
            GetEnvironmentCommandString(strRemark);
            strRemarkAnsi = JGW_W2A(strRemark);
        }
        if (!strUnlockcode.empty())
        {
            GetEnvironmentCommandString(strUnlockcode);
            strUnlockcodeAnsi = JGW_W2A(strUnlockcode);
        }
        if (!strImeiInfo.empty())
        {
            GetEnvironmentCommandString(strImeiInfo);
            strImeiInfoAnsi = JGW_W2A(strImeiInfo);
        }
        if (!GetMESDataBaseAdo()->NextStationToIMEI(strIMEISN,strSN,strSoftWareVarTinno,strSoftWareVarCustomer,strRemarkAnsi,strUnlockcodeAnsi,strImeiInfoAnsi))
        {
            LogE(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str());
            return false;
        }

        return true;
    }
}