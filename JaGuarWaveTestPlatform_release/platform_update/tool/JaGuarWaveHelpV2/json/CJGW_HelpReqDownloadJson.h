#pragma once
#include "CJGW_HelpReqBaseJson.h"

namespace JGW
{
    class CCJGW_HelpReqDownloadJson : public CCJGW_HelpReqBaseJson
    {
    public:
        CCJGW_HelpReqDownloadJson(void);
        ~CCJGW_HelpReqDownloadJson(void);
    public:
       virtual bool FromJosn(CCJGW_FTM_Json& msgJson);
    public:
        std::string taskNumber; //! ¶©µ¥ºÅ
        std::string execFolder; //! ³ÌÐòÄ¿Â¼
    };
}