#pragma once
#include <string>
#include <CSTSE_FoundationLib/CSTSE_ActionDefine.h>
#include <TSE_SequenceManagerPlugin/TSE_TestStatusListener.h>

namespace JGW
{
    class CCSTSE_TestIndexChangeJson
    {
    public:
        CCSTSE_TestIndexChangeJson(void);
        ~CCSTSE_TestIndexChangeJson(void);
    public:
        std::string GetTestIndexChangeJson();
        bool ParseTestIndexChangeJson(const std::string& strJson);
    public:
        ECSTSE_ACTION CSTSEAction;
        size_t Index;
        size_t SubIndex;
    };
}

