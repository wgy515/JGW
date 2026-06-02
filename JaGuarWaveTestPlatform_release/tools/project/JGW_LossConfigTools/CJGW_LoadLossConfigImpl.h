#pragma once
#include "CJGW_LossConfig_Define.h"
namespace JGW
{
    class CCJGW_LoadLossConfigImpl
    {
    public:
        //! 
        virtual bool LoadLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig) = 0;
        //! 
        virtual std::wstring GetLossConfigXmlPath() = 0;
        //! 
        virtual bool SaveLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig) = 0;
    };
}