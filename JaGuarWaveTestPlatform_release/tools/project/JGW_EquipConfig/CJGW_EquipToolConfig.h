#pragma once
#include "JGW_EquipConfig_Define.h"
#include <vector>

namespace JGW
{
    class CCJGW_EquipToolConfig
    {
    public:
        CCJGW_EquipToolConfig(void);
        ~CCJGW_EquipToolConfig(void);
    public:
        bool LoadEquipConfig(const std::wstring strEquipConfigIniPath,std::vector<S_EQUIP_PROJECT_CONFIG>& vsEquipProjetConfig);
    };
}