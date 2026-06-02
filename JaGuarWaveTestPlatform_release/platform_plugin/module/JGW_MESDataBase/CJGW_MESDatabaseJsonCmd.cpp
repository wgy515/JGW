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

    std::string CCJGW_MESDatabaseJsonCmd::GetGetInfoSOMOJson(const std::string& strmono,const std::string& strisso /* = "N" */)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"mono\":\"%s\",\"isso\":\"%s\"}",E_GET_INFO_SOMO,strmono.c_str(),strisso.c_str());
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

    std::string CCJGW_MESDatabaseJsonCmd::GetNextIMEIJson(const std::string& strIMEI_M_S_SN,const std::string& strLotSN,const std::string& strShiftID,const std::string& strSoftWareVarTinno /* = ""  */,const std::string& strSoftWareVarCustomer /* = "" */,const std::string& strRemark /* = "" */,const std::string& strUnlockcode /* = "" */,const std::string& strImeiInfo /* = "" */)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"IMEI_M_S_SN\":\"%s\",\"LotSN\":\"%s\",\"ShiftID\":\"%s\",\"SoftWareVarTinno\":\"%s\",\"SoftWareVarCustomer\":\"%s\",\"Remark\":\"%s\",\"Unlockcode\":\"%s\",\"ImeiInfo\":\"%s\"}",E_NEXT_IMEI,strIMEI_M_S_SN.c_str(),strLotSN.c_str(),strShiftID.c_str(),strSoftWareVarTinno.c_str(),strSoftWareVarCustomer.c_str(),strRemark.c_str(),strUnlockcode.c_str(),strImeiInfo.c_str());
        return strTemp;
    }

    std::string CCJGW_MESDatabaseJsonCmd::GetSaveTestRecords(const std::string& sn, const std::string& shiftID, const std::string& startDate, const std::string& endDate, const std::string& status, const std::string& comment, const std::string& type, const std::string& testName /* = "" */, const std::string& fixtureNo /* = "" */, const std::string& cableNo /* = "" */, const std::string& instrumentNo /* = "" */, const std::string& version /* = "" */, const std::string& platform /* = "" */, const std::string& failCode /* = "" */, const std::string& failCodeDesc /* = "" */)
    {
        std::string strTemp;
        JGW::JGW_FormatString(strTemp,"{\"requestType\":%d,\"sn\":\"%s\",\"shiftID\":\"%s\",\"startDate\":\"%s\",\"endDate\":\"%s\",\"status\":\"%s\",\"comment\":\"%s\",\"type\":\"%s\",\"testName\":\"%s\",\"fixtureNo\":\"%s\",\"cableNo\":\"%s\",\"instrumentNo\":\"%s\",\"version\":\"%s\",\"platform\":\"%s\",\"failCode\":\"%s\",\"failCodeDesc\":\"%s\"}",E_SAVE_TEST_RECORDS,sn.c_str(),shiftID.c_str(),startDate.c_str(),endDate.c_str(),status.c_str(),comment.c_str(),type.c_str(),testName.c_str(),fixtureNo.c_str(),cableNo.c_str(),instrumentNo.c_str(),version.c_str(),platform.c_str(),failCode.c_str(),failCodeDesc.c_str());
        return strTemp;
    }
}