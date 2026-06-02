#pragma once
#include <string>
#include <CSTSE_FoundationLib/CSTSE_ActionDefine.h>

namespace JGW
{
    class CCSTSE_BaseJson
    {
    public:
        CCSTSE_BaseJson(ECSTSE_ACTION action);
        ~CCSTSE_BaseJson(void);
    public:
        bool ParseBaseJson(const std::string& strRead);
        std::string GetBaseJson();
    public: //! 
        ECSTSE_ACTION CSTSEAction;
    };
}

