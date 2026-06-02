#pragma once
#include <string>
#include "TSE_FTM_Define.h"
namespace JGW
{
    class CTSE_FTM_JSONCommand
    {
    public:
        static std::string GetVersionJson();
        //! 
        static std::string GetKesyJson(E_TSE_FTM_KEY_STATUS eKeyStatus);
        //! 
        static std::string GetLcdColorJson(size_t color);
        //!
        static std::string GetLcdBackLightJson();
        //!
        static std::string GetMemoryJson();
        //!
        static std::string GetSimJson();
        //! 1组 :0x01 2组：0x02 3组：0x04 4组：0x08 闪烁：-1 关闭：0
        static std::string GetBreathingLedJson(int group);
        //!
        static std::string GetLedRGBJson();
        //!
        static std::string GetHandoverWiFiModeJson(int wifiMode);
        //!
        static std::string GetWiFiScanAPJson();
        //!
        static std::string GetHandoverADModeJson(int adMode);
        //!
        static std::string GetADScanAPJson();
        //!
        static std::string GetQC3InfoJson();
        //!
        static std::string GetOTGTeseJson();
    };
}


