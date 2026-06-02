#pragma once
#include <string>
#include "JGW_MESDataBase_Define.h"
#include <JaGuarWaveTestPlatform/JGWDefine.h>

namespace JGW
{
    enum E_MES_ID
    {
        E_UNKNOW_MES_ID = -1,
        //! 请求登录账号
        E_LOGIN_USER = 0,
        //! 获取工单列表 TDO:未实现
        E_GET_MOLIST = 1,
        //! 获取订单列表 TDO:未实现
        E_GET_SOLIST = 2,
        //! 测试SQL连接
        E_TEST_CONNECT = 3,
        //!获取IMEI信息，并检查当前IMEI是否属于当前工单以及号段信息 绑定PCBA SN
        E_GET_INFO_IMEI = 4,
        //! 获取SN相关信息，检查当前SN与工单的对应关系
        E_GET_INFO_SN = 5,
        //! 获取订单工单信息
        E_GET_INFO_SOMO = 6,
        //! 获取测试状态信息
        E_GET_TEST_STATUS = 7,
        //! 退出用户登录
        E_LOGOUT = 8,
        //! SN过站信息
        E_NEXT_SN = 9,
        //! IMEI过站信息
        E_NEXT_IMEI = 10,
        //! 保存测试记录
        E_SAVE_TEST_RECORDS = 11,
    };

    enum E_MONO_TYPE
    {
        E_MONO_SMT = 0,
        E_MONO_ASSY,
        E_MONO_Pack
    };

    const char g_szMonoTypes[][20] = {"MONOSMT","MONOAssy","MONOPack"};

    class CCJGW_MESDataBaseImpl
    {
    public:
        //! 设置MES服务SOCKET连接信息
        virtual void SetMESServiceSocketConnect(LPS_MES_INFO psMESInfo) = 0;
        //!
        virtual LPS_MES_INFO GetMESServiceSocketConnect() = 0;
        //!
        //! 检查MES连接状态
        virtual bool CheckMESStatus() = 0;
        //! 连接MES 服务 如果已经连接则直接返回true，否则则连接MES SOCKET
        virtual bool CheckConnectMESServiceSocket() = 0;
        //! 获取MES错误信息
        //virtual std::wstring& GetMESErrorMsg() = 0;
        //!
        virtual std::wstring& GetMESErrorMsgW() = 0;
        //! 测试MES连接,检查MES与数据库连接是否正常
        virtual bool TestConnectMES() = 0;
        //! 登录MES账号
        virtual bool LoginMES() = 0;
        //! 是否已经登录MES
        //! virtual bool IsLoginMES();
        //! 注销MES登录
        virtual void LogoutMES() = 0;
        //! 根据工单号获取订单名称 返回空字符串则表示当前工单号错误
        virtual bool GetTaskNameToWorkOrderNumber(const std::string& strWorkOrderNumber,std::string& strTaskNumber) = 0;
		//! 根据订单号获取工单号
		//! virtual std::string GetWorkOrderNumberToTaskName(const std::string& strTaskNumber) = 0;
        //! 根据SN号来检查当前SN号状态
        virtual bool CheckSNStatus(const std::string& strPCBASN,bool bIsSMTMONO = false) = 0;
        //! 
        virtual bool GetPCBSNInfoToWorkOrderNumber(const std::string& strPCBASN,std::vector<std::string>& vstrParamNames,std::vector<std::string>& vstrParamValues,bool bIsSMTMONO = false) = 0;
		//! 根据SN来获取订单号以及任务单号
		virtual bool GetTestWorkOrderNumberToPCBASN(const std::string& strPCBASN,std::string& strTaskNumber,std::string& strWorkOrderNumber,E_MONO_TYPE meMonoType = E_MONO_SMT) = 0;
        //! 根据IMEI号段来获取当前IMEI号信息 
		virtual bool GetTestWorkOrderNumberToIMEI(const std::string& strIMEI,std::string& strTaskNumber,std::string& strWorkOrderNumber) = 0;
        //! strLotPKSN: 整机SN、主IMEI、副IMEI
        //! strPCBASN: PCBA SN（空则不校验主板号）
        virtual bool GetIMEIInfoToIMEI(const std::string& strLotPKSN,const std::string& strPCBASN,std::vector<std::string>& vstrParamNames,std::vector<std::string>& vstrParamValues) = 0;
        //! 根据条形码( 整机SN、主IMEI、副IMEI)获取当前测试状态
        virtual bool GetTestStatusToBarcode(const std::string& strBarcode,const std::string& strOperationId) = 0;
        //! IMEI过站
        //! strIMEI_M_S_SN: 主副IMEI或者背标SN
        //! strSN: PCBA SN号（若非需要保存IMEI与SN对应关系的写号节点，此项为空）
        //! strSoftWareVarTinno: 内部软件版本（若非需要保存IMEI与SN对应关系的写号节点，此项为空）
        //! strSoftWareVarCustomer: 客户软件版本（若非需要保存IMEI与SN对应关系的写号节点，此项为空）
        //! strRemark: 备注信息，该信息将写入包装过站时的事务表备注中（长度不可以超过200字符串）,如对号写号可写入：(写IMEI 采用扫描SN,校验SN； 校验版本，CIT：Y，ANT：Y，GPS/WIFI：Y；GPS Data：N；IMEI LOCK：N；ATTKFlag：N；Reset：Y；OTP写号异常处理：N；)标记信息 可为空在字段，若不用，可以直接不传该参数。
        virtual bool NextStationToIMEI(const std::string& strIMEI_M_S_SN,const std::string& strSN,const std::string& strSoftWareVarTinno= "",const std::string& strSoftWareVarCustomer= "",const std::string& strRemark = "",const std::string& strUnlockcode = "",const std::string& strImeiInfo = "") = 0;
        //! SN 过站 strSN : SN号段
        virtual bool NextStationToSN(const std::string& strSN) = 0;
        //! 保存测试记录
        /// <param name="sn">@LotSN varchar(20) = '', --PCBSN号 </param>
        /// <param name="shiftID">@ShiftID varchar(20) = '', --班次 </param>
        /// <param name="startDate">@StartDate varchar(20) = '', --开始时间 </param>
        /// <param name="endDate">@EndDate varchar(20) = '', --结束时间 </param>
        /// <param name="status">@Status varchar(20) = '', --状态 </param>
        /// <param name="comment">@Comment varchar(200) = '', --备注信息 </param>
        /// <param name="type">@Type varchar(20) = ''--添加记录或者更新记录 </param>
        /// TestName nvarchar(64) ='',  -- 测试名称 如ATE
        /// FixtureNo nvarchar(64) ='',  -- 治具编号
        /// CableNo nvarchar(64)='',  -- RF线编号
        /// InstrumentNo nvarchar(64)='',   -- 仪器编号
        /// Version nvarchar(32)='',  -- 工具版本（平台版本_插件版本）
        /// Platform nvarchar(32)='',  -- 工具平台：MTK或Qualcomm
        /// FailCode varchar(256)='',  -- 失败编码
        /// FailCodeDesc nvarchar(512)='' -- 失败编码描述
        virtual bool SaveTestRecordsMES(const std::string& sn,
            const std::string& startDate,
            const std::string& endDate,
            const std::string& status,
            const std::string& comment,
            const std::string& type) = 0;

#if 0
        //! 设置工序ID
        //! virtual void SetOperationId(std::wstring& strOperationId) = 0;
        //! 获取当前工序ID
        //! virtual std::wstring GetOperationID() = 0; 
#endif
    };

#ifdef __cplusplus
    extern "C" 
    {
#endif
        JGW_MESDATABASE_API CCJGW_MESDataBaseImpl* GetMESDataBaseAdo();

        JGW_MESDATABASE_API void DestoryMESDataBaseAdo();
#ifdef __cplusplus
    };
#endif
}