#pragma once
#include "CJGW_LossConfig_Define.h"
#include <vector>
namespace JGW
{
    class CCJGW_LossToolConfig
    {
    public:
        CCJGW_LossToolConfig(void);
        ~CCJGW_LossToolConfig(void);
    public:
        bool LoadLossToolConfig(const std::wstring strLossConfigIniPath,std::vector<S_EQUIP_PROJECT_CONFIG>& vsLossProjetConfig);
    };
}

