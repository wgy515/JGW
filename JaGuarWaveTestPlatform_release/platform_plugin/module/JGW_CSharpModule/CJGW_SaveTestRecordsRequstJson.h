#pragma once
namespace JGW
{
    class CCJGW_SaveTestRecordsRequstJson
    {
    public:
        CCJGW_SaveTestRecordsRequstJson(void);
        ~CCJGW_SaveTestRecordsRequstJson(void);
    public:
        std::string OperateID;
        std::string IMEI_SN;
        std::string StartTestTime;
        std::string EndTestTime;
    };
}


