#include "StdAfx.h"
#include "CJGW_MESDatabaseJsonCmd.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    std::string CCJGW_MESDatabaseJsonCmd::GetLoginJson(const std::string& strUsername,const std::string& strUserpwd,const std::string& strOperationId)
    {       
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"username\":\"%s\",\"userpwd\":\"%s\",\"operationId\":\"%s\"}",E_LOGIN_USER,strUsername.c_str(),strUserpwd.c_str(),strOperationId.c_str());
        return strTemp;
    } 

    std::string CCJGW_MESDatabaseJsonCmd::GetTestConnectJson()
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d}",E_TEST_CONNECT);
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetLogoutJson()
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d}",E_LOGOUT);
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetGetInfoSNJson(const std::string& strPCBASN,const std::string& strMono)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"pcbasn\":\"%s\",\"mono\":\"%s\"}",E_GET_INFO_SN,strPCBASN.c_str(),strMono.c_str());
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetGetInfoIMEIJson(const std::string& strLotPKSN,const std::string& strPCBSN,const std::string& strMONO)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"LotPKSN\":\"%s\",\"PCBSN\":\"%s\",\"MONO\":\"%s\"}",E_GET_INFO_IMEI,strLotPKSN.c_str(),strPCBSN.c_str(),strMONO.c_str());
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetGetInfoSOMOJson(const std::string& strmono,const std::string& strisso /* = "N" */,int paramvalpos /* = 0 */)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"mono\":\"%s\",\"isso\":\"%s\",\"paramvalpos\":%d}",E_GET_INFO_SOMO,strmono.c_str(),strisso.c_str(),paramvalpos);
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetGetTestStatusJson(const std::string& strlotSN,const std::string& stroperationId)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"lotSN\":\"%s\",\"operationId\":\"%s\"}",E_GET_TEST_STATUS,strlotSN.c_str(),stroperationId.c_str());
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetNextSNJson(const std::string& strLotSN,const std::string& strShiftID)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"LotSN\":\"%s\",\"ShiftID\":\"%s\"}",E_NEXT_SN,strLotSN.c_str(),strShiftID.c_str());
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetNextIMEIJson(const std::string& strIMEI_M_S_SN,const std::string& strLotSN,const std::string& strShiftID,const std::string& strSoftWareVarTinno /* = ""  */,const std::string& strSoftWareVarCustomer /* = "" */,const std::string& strRemark /* = "" */)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"IMEI_M_S_SN\":\"%s\",\"LotSN\":\"%s\",\"ShiftID\":\"%s\",\"SoftWareVarTinno\":\"%s\",\"SoftWareVarCustomer\":\"%s\",\"Remark\":\"%s\"}",E_NEXT_IMEI,strIMEI_M_S_SN.c_str(),strLotSN.c_str(),strShiftID.c_str(),strSoftWareVarTinno.c_str(),strSoftWareVarCustomer.c_str(),strRemark.c_str());
        return strTemp;
    }
}