#pragma once
#include <string>
#include <queue>
#include <poco/Mutex.h>
#include <TSE_MSFoundationLib/CTSE_LogMsgBase.h>
namespace JGW
{
    //! 1、读取log函数，读取不到或者没数据返回false
    //! 2、push log到queue中
    //! 3、确认是否做log缓冲处理 TDO: 后续在进行处理
    class CTSE_LogMsg : public CTSE_LogMsgBase
    {
    public:
        CTSE_LogMsg(const std::string& strID);
        ~CTSE_LogMsg(void);
    public:
        void SetID(const std::string& strID);
        /// 设置log MSG对应ID

        std::string GetID();
        /// 获取log MSG ID

        bool ReadLog(std::string& strRead);
        /// 读取log，无数据返回false

        void PushLog(const std::string& strLog);
        /// push log到缓冲区中

    private:
        std::string mstrID;
        std::queue<std::string> mqueueLogMsg;
        Poco::Mutex mMutex;
    };
}

