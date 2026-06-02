#pragma once
#include <string>
#include "CTSE_MSFoundationDefine.h"
namespace JGW
{
    class CTSE_MSLogJson
    {
    public:
        CTSE_MSLogJson(void);
        ~CTSE_MSLogJson(void);
    public:
        static std::string GetMSLogJson(const std::string& strID,const std::string& strMsg,bool encrypt = false,bool bLog = true);
        /// 

        static bool GetMSLogToJson(std::string& strID,std::string& strMsg,bool& encrypt,bool& bLog,const std::string& strJson);
        /// json ת id  msg encrypt

        static bool GetMSControlType(TSEControlTypeEnum& eControlType,const std::string& strJson); 
        /// 

        static std::string GetMSBaseJson(TSEControlTypeEnum eControlType);
        /// Base Json msg

        static std::string GetMSXttJson(int engineType,const std::string& strXttFilePath);
        /// MS -> MC

        static bool GetMCResponseJsonToJson(const std::string& strJson,S_MC_RSP_JSON& smcRspJson);
        /// 

        static std::string GetMSGlobalVariableJson(bool bSet,const std::string& strKey,const std::string& strValue);
        /// 
    };
}
