#pragma once
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>
#include "CJGW_MESConnectSocket.h"
#include "JSON/CJGW_FTM_Json.h"
#include "JSON/CJGW_MESResponseBaseJson.h"
#include <JGW_WindowsFuncPlugin/CJGW_AsyncSocket.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>

#define MES_DATABSE_COMMAND_RECOUNT 3
#define MES_MAX_RECV_BUFFER_SIZE 1024 * 8

namespace JGW
{
    class CCJGW_MESDataBaseAdo : public CCJGW_MESDataBaseImpl
    {
    public:
        CCJGW_MESDataBaseAdo(void);
        ~CCJGW_MESDataBaseAdo(void);
        //! 
        void SetMESServiceSocketConnect(LPS_MES_INFO psMESInfo);
        //!
        LPS_MES_INFO GetMESServiceSocketConnect();
        //!
        //std::wstring& GetMESErrorMsg();
        //!
        std::wstring& GetMESErrorMsgW();
        //! 
        bool LoginMES();
        //! 
        bool TestConnectMES();
        //!
        void LogoutMES();
        //! 
        bool GetTaskNameToWorkOrderNumber(const std::string& strWorkOrderNumber,std::string& strTaskNumber);
		//!
		//std::string GetWorkOrderNumberToTaskName(const std::string& strTaskNumber);
        //!
        bool CheckSNStatus(const std::string& strPCBASN,bool bIsSMTMONO = false);
		//!
		bool GetTestWorkOrderNumberToPCBASN(const std::string& strPCBASN,std::string& strTaskNumber,std::string& strWorkOrderNumber,E_MONO_TYPE meMonoType = E_MONO_SMT);
        //! 
        bool GetPCBSNInfoToWorkOrderNumber(const std::string& strPCBASN,std::vector<std::string>& vstrParamNames,std::vector<std::string>& vstrParamValues,bool bIsSMTMONO = false);
        //!
		bool GetTestWorkOrderNumberToIMEI(const std::string& strIMEI,std::string& strTaskNumber,std::string& strWorkOrderNumber);
		//!
        bool GetIMEIInfoToIMEI(const std::string& strLotPKSN,const std::string& strPCBASN,std::vector<std::string>& vstrParamNames,std::vector<std::string>& vstrParamValues);
        //!
        bool GetTestStatusToBarcode(const std::string& strBarcode,const std::string& strOperationId);
        //!
        bool NextStationToSN(const std::string& strSN);
        //!
        bool NextStationToIMEI(const std::string& strIMEI_M_S_SN,const std::string& strSN,const std::string& strSoftWareVarTinno = "",const std::string& strSoftWareVarCustomer = "" ,const std::string& strRemark = "",const std::string& strUnlockcode = "",const std::string& strImeiInfo = "");
        //!
        bool SaveTestRecordsMES(const std::string& sn,
            const std::string& startDate,
            const std::string& endDate,
            const std::string& status,
            const std::string& comment,
            const std::string& type);

    public:
        //! 是否已经登录MES
        //! bool IsLoginMES();
        //! 检查MES账号信息
        bool CheckMESConnectInfo();
        //! 检查MES连接状态
        bool CheckMESStatus();
        //! 连接MES 服务 如果已经连接则直接返回true，否则则连接MES SOCKET
        bool CheckConnectMESServiceSocket();
        //! 
        bool WriteJsonSocket(const std::string& strWriteJson,size_t timeOutSec = 10,int retryCount  = 3 );
        //! 
        bool ReadJsonSocketToWriteSocket(const std::string& strWriteJson,size_t nReqCode,CCJGW_FTM_Json& ftmJson,size_t timeOutSec = 10 ,int retryCount = 3);
    private:
        bool mbIsLoginMES;
        char* mRecvBuf;
        LPS_MES_INFO mpsMESInfo;
        S_MES_INFO msMESInfo;
        std::wstring mstrMESErrorMsg;  
        CCJGW_AsyncSocket mMESConnectSocket;
        //! CCJGW_MESConnectSocket mMESConnectSocket;    
    };

    //! 通用执行
    template <typename Func> 
    inline bool ExecuteMESDataBaseAdo_G(Func func)
    {
        if (GetMESDataBaseAdo()->CheckMESStatus()) return true;
        if (!GetMESDataBaseAdo()->CheckConnectMESServiceSocket()) { GetMESDataBaseAdo()->GetMESErrorMsgW() = GetString(E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING); return false;}
        try
        {
            bool bResult = false;
            CCJGW_MESResponseBaseJson resResponseJson;
            CCJGW_FTM_Json ftmJson;
            for (int i = 0;i < MES_DATABSE_COMMAND_RECOUNT && !bResult;i ++)
            {
                if (!GetMESDataBaseAdo()->LoginMES()) { PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str()); continue;}
                bResult = func(ftmJson); 
                GetMESDataBaseAdo()->LogoutMES();
            }
            resResponseJson.FromJosn(ftmJson);
            GetMESDataBaseAdo()->GetMESErrorMsgW() = JGW_A2W(resResponseJson.returnMessage);
            return resResponseJson.bStatus;
        }
        catch (...) 
        {
            return false;
        }
    }

    //! 执行SQL语句
    template <typename Func> 
    inline bool ExecuteMESDataBaseAdo(CCJGW_MESResponseBaseJson& esResponseJson,Func func)
    {
        if (GetMESDataBaseAdo()->CheckMESStatus()) {esResponseJson.bStatus = true; return true;}
        if (!GetMESDataBaseAdo()->CheckConnectMESServiceSocket()) { GetMESDataBaseAdo()->GetMESErrorMsgW() = GetString(E_MES_CONNECT_FAIL_CHECK_SERVICE_IS_RUNING); return false;}
        try
        {
            bool bResult = false;

            for (int i = 0;i < MES_DATABSE_COMMAND_RECOUNT && !bResult;i ++)
            {
                if (!GetMESDataBaseAdo()->LoginMES()) { PELOG4WW(GetMESDataBaseAdo()->GetMESErrorMsgW().c_str()); continue;}
                bResult = func();
                GetMESDataBaseAdo()->LogoutMES();
            }
            return bResult;
        }
        catch (...) 
        {
            return false;
        }
    }
}