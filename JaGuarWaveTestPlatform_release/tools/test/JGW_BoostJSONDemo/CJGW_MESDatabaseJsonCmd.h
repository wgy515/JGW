#pragma once
#include <string>

namespace JGW
{
    enum E_MES_ID
    {
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
        static std::string GetGetInfoSOMOJson(const std::string& strmono,const std::string& strisso = "N",int paramvalpos = 0);
        //! 获取测试状态信息
        static std::string GetGetTestStatusJson(const std::string& strlotSN,const std::string& stroperationId);
        //! SN过站 strShiftID:班次信息
        static std::string GetNextSNJson(const std::string& strLotSN,const std::string& strShiftID);
        //! IMEI过站
        static std::string GetNextIMEIJson(const std::string& strIMEI_M_S_SN,const std::string& strLotSN,const std::string& strShiftID,const std::string& strSoftWareVarTinno = "" ,const std::string& strSoftWareVarCustomer = "",const std::string& strRemark = "");
    };
}