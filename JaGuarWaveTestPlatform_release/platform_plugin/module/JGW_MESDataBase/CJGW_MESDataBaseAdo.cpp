#include "StdAfx.h"
#include "CJGW_MESDataBaseAdo.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_MESDatabaseJsonCmd.h"
#include "JSON/CJGW_MESGetInfoSomoResponseJson.h"
#include "JSON/CJGW_MESGetSNInfoResponseJson.h"
namespace JGW
{
    CCJGW_MESDataBaseAdo::CCJGW_MESDataBaseAdo(void) : mpsMESInfo(NULL),mbIsLoginMES(false)
    {
        mRecvBuf = (char*)calloc(MES_MAX_RECV_BUFFER_SIZE,1);
    }

    CCJGW_MESDataBaseAdo::~CCJGW_MESDataBaseAdo(void)
    {
        free(mRecvBuf);
    }

    LPS_MES_INFO CCJGW_MESDataBaseAdo::GetMESServiceSocketConnect()
    {
        if (!mpsMESInfo) { mpsMESInfo = &msMESInfo; mpsMESInfo->msMesConnectInfo.meMESModel = E_NOTONLINE_MES;mpsMESInfo->mstrOperationId = "-1";}
        return mpsMESInfo;
    }

    /*std::wstring& CCJGW_MESDataBaseAdo::GetMESErrorMsg()
    {
        return mstrMESErrorMsg;
    }*/

    std::wstring& CCJGW_MESDataBaseAdo::GetMESErrorMsgW()
    {
        return mstrMESErrorMsg;
    }

    bool CCJGW_MESDataBaseAdo::CheckMESStatus()
    {
        if (E_NOTONLINE_MES == mpsMESInfo->msMesConnectInfo.meMESModel)
        {
            PDLOG4WW(GetString(E_MES_WARNING_OFFLINE_MODE));
            return true;
        }
        if (NULL == mpsMESInfo->mstrOperationId.compare("-1"))
        {
            PDLOG4WW(GetString(E_MES_WARNING_STATION_NOT_PROCESS));
            return true;
        }

        return false;
    }

    bool CCJGW_MESDataBaseAdo::CheckConnectMESServiceSocket()
    {
        if (!mMESConnectSocket.CheckSocketIsConnected())
        {
            PDLOG4WW(GetString(E_MES_CONNECTING_DATA_SERVICE));
            if (!mMESConnectSocket.ConnectSocketTimeOut(
                mpsMESInfo->msMesConnectInfo.mstrMESServiceIP,
                mpsMESInfo->msMesConnectInfo.mnMESServiceSocketPort,
                mpsMESInfo->msMesConnectInfo.mnMESRecvTimeOut,
                mpsMESInfo->msMesConnectInfo.mnMESConnectTimeOut))
            {
                PELOG4WW(GetString(E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING));
                mbIsLoginMES = false;
                return false;
            }
            PDLOG4WW(GetString(E_MES_CONNECT_SUCCESSFULLY));
        }
        return true;
    }

    void CCJGW_MESDataBaseAdo::SetMESServiceSocketConnect(LPS_MES_INFO psMESInfo)
    {
        mpsMESInfo = psMESInfo;
    }

    bool CCJGW_MESDataBaseAdo::LoginMES()
    {
        Log4WD(L"CCJGW_MESDataBaseAdo::LoginMES");
        if (CheckMESStatus()) return true;
        if (!CheckConnectMESServiceSocket()) { mstrMESErrorMsg = GetString(E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING);return false;}
        //! 
        CCJGW_FTM_Json ftmJson;
        if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetLoginJson(
            mpsMESInfo->msMesConnectInfo.mstrUsername,
            mpsMESInfo->msMesConnectInfo.mstrUserpwd,
            mpsMESInfo->mstrOperationId),E_LOGIN_USER,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut))
        {
            mstrMESErrorMsg = GetString(E_MES_CONNECT_SERVICE_FAILED);
            mMESConnectSocket.CleanupSocket();
            return false;
        }

        CCJGW_MESResponseBaseJson mesResponseBaseJson;
        mesResponseBaseJson.FromJosn(ftmJson);
        mstrMESErrorMsg =  JGW_A2W(mesResponseBaseJson.returnMessage);
        mbIsLoginMES = mesResponseBaseJson.bStatus;
        return mesResponseBaseJson.bStatus;
    }

    bool CCJGW_MESDataBaseAdo::TestConnectMES()
    {
        if (CheckMESStatus()) return true;
        if (!CheckConnectMESServiceSocket()) return false;
        //! 
        CCJGW_FTM_Json ftmJson;
        if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetTestConnectJson(),E_TEST_CONNECT,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut))
        {
            return false;
        }

        CCJGW_MESResponseBaseJson mesResponseBaseJson;
        mesResponseBaseJson.FromJosn(ftmJson);
        return mesResponseBaseJson.bStatus;
    }

    void CCJGW_MESDataBaseAdo::LogoutMES()
    {
        if (CheckMESStatus()) return ;
        if (!CheckConnectMESServiceSocket()) return ;
        //! 
        CCJGW_FTM_Json ftmJson;
        ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetLogoutJson(),E_LOGOUT,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut);
        mbIsLoginMES = false;
    }

    /*std::string CCJGW_MESDataBaseAdo::GetWorkOrderNumberToTaskName(const std::string& strTaskNumber)
    {
    CCJGW_MESGetInfoSomoResponseJson mesGetInfoSomoResponseJson;
    if (ExecuteMESDataBaseAdo(mesGetInfoSomoResponseJson,[&]()-> bool{
    CCJGW_FTM_Json ftmJson;
    if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoSOMOJson(strTaskNumber,"Y",1),E_GET_INFO_SOMO,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
    mesGetInfoSomoResponseJson.FromJosn(ftmJson);
    return true;
    })) mstrMESErrorMsg = mesGetInfoSomoResponseJson.returnMessage;
    return mesGetInfoSomoResponseJson.bStatus?mesGetInfoSomoResponseJson.paramval:"";
    }*/

    bool CCJGW_MESDataBaseAdo::GetTaskNameToWorkOrderNumber(const std::string& strWorkOrderNumber,std::string& strTaskNumber)
    {
        CCJGW_MESGetInfoSomoResponseJson mesGetInfoSomoResponseJson;
        if (ExecuteMESDataBaseAdo(mesGetInfoSomoResponseJson,[&]()-> bool{
            CCJGW_FTM_Json ftmJson;
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoSOMOJson(strWorkOrderNumber),E_GET_INFO_SOMO,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
            mesGetInfoSomoResponseJson.FromJosn(ftmJson);
            strTaskNumber = mesGetInfoSomoResponseJson.mstrTaskNumber;
            return true;
        })) mstrMESErrorMsg = JGW_A2W(mesGetInfoSomoResponseJson.returnMessage);
        /*return mesGetInfoSomoResponseJson.bStatus?mesGetInfoSomoResponseJson.paramval:"";*/
        return mesGetInfoSomoResponseJson.bStatus;
    }

    bool CCJGW_MESDataBaseAdo::CheckSNStatus(const std::string& strPCBASN,bool bIsSMTMONO/* = false*/)
    {
#if 0
        CCJGW_MESResponseBaseJson responseBaseJson;
         if (ExecuteMESDataBaseAdo(responseBaseJson,[&]()-> bool{
            CCJGW_FTM_Json ftmJson;
#ifdef GET_INFO_WORK_ORDER_NUMBER
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoSNJson(strPCBASN,bIsSMTMONO?mpsMESInfo->mstrTaskNumber:mpsMESInfo->mstrWorkOrderNumber),E_GET_INFO_SN,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
#else
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoSNJson(strPCBASN,mpsMESInfo->mstrTaskNumber),E_GET_INFO_SN,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
#endif 
            responseBaseJson.FromJosn(ftmJson);
            return true;
        })) mstrMESErrorMsg = JGW_A2W(responseBaseJson.returnMessage);
        return responseBaseJson.bStatus;
#else
		return ExecuteMESDataBaseAdo_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
			if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetTestStatusJson(strPCBASN,mpsMESInfo->mstrOperationId),E_GET_TEST_STATUS,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
			return true;
		});
#endif
    }

	bool CCJGW_MESDataBaseAdo::GetTestWorkOrderNumberToPCBASN(const std::string& strPCBASN,std::string& strTaskNumber,std::string& strWorkOrderNumber,E_MONO_TYPE meMonoType /* = E_MONO_SMT */)
	{
		CCJGW_MESGetSNInfoResponseJson responseBaseJson;
        responseBaseJson.mstrMONOType = g_szMonoTypes[meMonoType];
		if (ExecuteMESDataBaseAdo(responseBaseJson,[&]()-> bool{
			CCJGW_FTM_Json ftmJson;
			if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoSNJson(strPCBASN,""),E_GET_INFO_SN,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
			responseBaseJson.FromJosn(ftmJson);
			strTaskNumber = responseBaseJson.mstrTaskNumber;
			strWorkOrderNumber = responseBaseJson.mstrWorkOrderNumber;
			return true;
		})) mstrMESErrorMsg = JGW_A2W(responseBaseJson.returnMessage);
		return responseBaseJson.bStatus;
	}

    bool CCJGW_MESDataBaseAdo::GetPCBSNInfoToWorkOrderNumber(const std::string& strPCBASN,std::vector<std::string>& vstrParamNames,std::vector<std::string>& vstrParamValues,bool bIsSMTMONO /* = false */)
    {
        CCJGW_MESGetSNInfoResponseJson responseBaseJson;
        if (ExecuteMESDataBaseAdo(responseBaseJson,[&]()-> bool{
            CCJGW_FTM_Json ftmJson;
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoSNJson(strPCBASN,bIsSMTMONO?mpsMESInfo->mstrTaskNumber:mpsMESInfo->mstrWorkOrderNumber),E_GET_INFO_SN,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
            responseBaseJson.FromJosn(ftmJson);
            ftmJson.GetJsonValueToKey("paraName",vstrParamNames);
            ftmJson.GetJsonValueToKey("paraValue",vstrParamValues);
            return true;
        })) mstrMESErrorMsg = JGW_A2W(responseBaseJson.returnMessage);
        return responseBaseJson.bStatus;
    }

	bool CCJGW_MESDataBaseAdo::GetTestWorkOrderNumberToIMEI(const std::string& strIMEI,std::string& strTaskNumber,std::string& strWorkOrderNumber)
	{
		CCJGW_MESResponseBaseJson responseBaseJson;
		if (ExecuteMESDataBaseAdo(responseBaseJson,[&]()-> bool{
			CCJGW_FTM_Json ftmJson;
			if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoIMEIJson(strIMEI,"",""),E_GET_INFO_IMEI,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
			responseBaseJson.FromJosn(ftmJson);
			std::vector<std::string> vstrParamNames, vstrParamValues;
			ftmJson.GetJsonValueToKey("paraName",vstrParamNames);
			ftmJson.GetJsonValueToKey("paraValue",vstrParamValues);
			//! MONOPack SONO
			for (size_t y = 0;y < vstrParamNames.size();y ++)
			{
				if (NULL == JGW_StrComparenoCaseStr("MONOPack",vstrParamNames[y].c_str()))
				{
					strWorkOrderNumber = vstrParamValues[y];
					continue;
				}
				if (NULL == JGW_StrComparenoCaseStr("SONO",vstrParamNames[y].c_str()))
				{
					strTaskNumber = vstrParamValues[y];
				}
			}
			return true;
		})) mstrMESErrorMsg = JGW_A2W(responseBaseJson.returnMessage);
		return responseBaseJson.bStatus;
	}

    bool CCJGW_MESDataBaseAdo::GetIMEIInfoToIMEI(const std::string& strLotPKSN,const std::string& strPCBASN,std::vector<std::string>& vstrParamNames,std::vector<std::string>& vstrParamValues)
    {
        CCJGW_MESResponseBaseJson responseBaseJson;
        if (ExecuteMESDataBaseAdo(responseBaseJson,[&]()-> bool{
            CCJGW_FTM_Json ftmJson;
#ifdef GET_INFO_WORK_ORDER_NUMBER
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoIMEIJson(strLotPKSN,strPCBASN,mpsMESInfo->mstrWorkOrderNumber),E_GET_INFO_IMEI,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
#else
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetInfoIMEIJson(strLotPKSN,strPCBASN,mpsMESInfo->mstrTaskNumber),E_GET_INFO_IMEI,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
#endif
            responseBaseJson.FromJosn(ftmJson);
            ftmJson.GetJsonValueToKey("paraName",vstrParamNames);
            ftmJson.GetJsonValueToKey("paraValue",vstrParamValues);
            return true;
        })) mstrMESErrorMsg = JGW_A2W(responseBaseJson.returnMessage);
        return responseBaseJson.bStatus;
    }

    bool CCJGW_MESDataBaseAdo::GetTestStatusToBarcode(const std::string& strBarcode,const std::string& strOperationId)
    {
       return ExecuteMESDataBaseAdo_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetGetTestStatusJson(strBarcode,strOperationId),E_GET_TEST_STATUS,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_MESDataBaseAdo::NextStationToSN(const std::string& strSN)
    {
        return ExecuteMESDataBaseAdo_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetNextSNJson(strSN,mpsMESInfo->mstrShiftID),E_NEXT_SN,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_MESDataBaseAdo::NextStationToIMEI(const std::string& strIMEI_M_S_SN,const std::string& strSN,const std::string& strSoftWareVarTinno /* = "" */,const std::string& strSoftWareVarCustomer /* = ""  */,const std::string& strRemark /* = "" */,const std::string& strUnlockcode /* = "" */,const std::string& strImeiInfo /* = "" */)
    {
        return ExecuteMESDataBaseAdo_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetNextIMEIJson(strIMEI_M_S_SN,strSN,mpsMESInfo->mstrShiftID,strSoftWareVarTinno,strSoftWareVarCustomer,strRemark,strUnlockcode,strImeiInfo),E_NEXT_IMEI,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_MESDataBaseAdo::SaveTestRecordsMES(const std::string& sn, const std::string& startDate, const std::string& endDate, const std::string& status, const std::string& comment, const std::string& type)
    {
        return ExecuteMESDataBaseAdo_G([&](CCJGW_FTM_Json& ftmJson)-> bool{
            if (!ReadJsonSocketToWriteSocket(CCJGW_MESDatabaseJsonCmd::GetSaveTestRecords(sn,mpsMESInfo->mstrShiftID,startDate,endDate,status,comment,type),E_SAVE_TEST_RECORDS,ftmJson,mpsMESInfo->msMesConnectInfo.mnMESSendTimeOut)) return false;
            return true;
        });
    }

    bool CCJGW_MESDataBaseAdo::WriteJsonSocket(const std::string& strWriteJson,size_t timeOutSec /* = 10 */,int retryCount /* = 3 */)
    {
        for (int i = 0;i < retryCount;i ++)
        {
            if(mMESConnectSocket.AsyncWriteSocket(strWriteJson)) return true;
        }
        mstrMESErrorMsg = L"Send MES Service Command Fail";
        return false;
    }

    bool CCJGW_MESDataBaseAdo::ReadJsonSocketToWriteSocket(const std::string& strWriteJson,size_t nReqCode,CCJGW_FTM_Json& ftmJson,size_t timeOutSec /* = 10 */,int retryCount /* = 3 */)
    {
        std::stringstream msg_ss;
        int nRsponseCode = -1;
        //! 写入的JSON为空则直接读
        if (!WriteJsonSocket(strWriteJson,timeOutSec,retryCount)) return false;

        for (int i = 0;i < retryCount;i ++)
        {
            memset(mRecvBuf,0x00,MES_MAX_RECV_BUFFER_SIZE);
            if (!mMESConnectSocket.AsyncReadSocket(mRecvBuf,MES_MAX_RECV_BUFFER_SIZE))
            {
                mstrMESErrorMsg = L"Recv Json Fail";
                continue;
            }
            if (!ftmJson.FromJsonToString(mRecvBuf)) {PELOG4WW_F(L"MES Read:%s",JGW_A2W_A(mRecvBuf).c_str());continue;}
            //! log print
            for(boost::property_tree::ptree::iterator msg_it = ftmJson.GetJsonTree().begin(); msg_it != ftmJson.GetJsonTree().end(); ++msg_it)
            {
                PDLOG4WW_F(L"%s : %s",JGW_A2W(msg_it->first).c_str(),JGW_A2W(ftmJson.GetJsonTree().get<std::string>(msg_it->first)).c_str());
               /* PDLOG4WA_F("%s : %s",msg_it->first.c_str(),ftmJson.GetJsonTree().get<std::string>(msg_it->first).c_str());*/
            }
            //! 判断请求code和响应code是否一致
            if (!ftmJson.GetJsonValueToKey<int>("responseType",nRsponseCode)) continue;
            if (nRsponseCode == nReqCode) return true;
            else return false;
        }
        return false;
    }
}