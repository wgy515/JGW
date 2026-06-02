#pragma once
#include <string>
#include <JGW_MESDataBase/CJGW_MESDataBaseImpl.h>

namespace JGW
{
    class CCJGW_MESDatabaseJsonCmd
    {
    public:
        //! 获取登录的JSON
        static std::string GetLoginJson(const std::string& strUsername,const std::string& strUserpwd,const std::string& strOperationId);
        //! 获取测试连接
        static std::string GetTestConnectJson();
        //! 获取退出用户登录JSON
        static std::string GetLogoutJson();
        //! 获取SN相关信息
        static std::string GetGetInfoSNJson(const std::string& strPCBASN,const std::string& strMono);
        //! 获取IMEI相关信息
        static std::string GetGetInfoIMEIJson(const std::string& strLotPKSN,const std::string& strPCBSN,const std::string& strMONO);
        //! 获取订单工单信息 -- 获取工单对应订单信息
        static std::string GetGetInfoSOMOJson(const std::string& strmono,const std::string& strisso = "N");
        //! 获取测试状态信息
        static std::string GetGetTestStatusJson(const std::string& strlotSN,const std::string& stroperationId);
        //! SN过站 strShiftID:班次信息
        static std::string GetNextSNJson(const std::string& strLotSN,const std::string& strShiftID);
        //! IMEI过站
        static std::string GetNextIMEIJson(const std::string& strIMEI_M_S_SN,const std::string& strLotSN,const std::string& strShiftID,const std::string& strSoftWareVarTinno = "" ,const std::string& strSoftWareVarCustomer = "",const std::string& strRemark = "",const std::string& strUnlockcode = "",const std::string& strImeiInfo = "");
        //!
        static std::string GetSaveTestRecords(const std::string& sn,
            const std::string& shiftID,
            const std::string& startDate,
            const std::string& endDate,
            const std::string& status,
            const std::string& comment,
            const std::string& type,
            const std::string& testName = "",
            const std::string& fixtureNo = "",
            const std::string& cableNo = "",
            const std::string& instrumentNo = "",
            const std::string& version = "",
            const std::string& platform = "",
            const std::string& failCode = "",
            const std::string& failCodeDesc = "");
    };
}