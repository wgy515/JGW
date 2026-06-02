#pragma once

namespace JGW
{
    class CCJGW_NextIMEIStationRequstJson
    {
    public:
        CCJGW_NextIMEIStationRequstJson(void);
        ~CCJGW_NextIMEIStationRequstJson(void);
    public:
        std::string IMEI;
        std::string SN;
        std::string OperateID;
        std::string SoftWareVarTinno;
        std::string SoftWareVarCustomer;
        std::string RemarkCommand;
        std::string UnlockcodeCommand;
        std::string ImeiInfoCommand;
    };

}

