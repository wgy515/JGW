#pragma once
#include "CJGW_LossConfig_Define.h"
#include <tinyxml/tinyxml.h>
#include "CJGW_LoadLossConfigImpl.h"
namespace JGW
{
    class CCJGW_LoadCalDBLossConfig : public CCJGW_LoadLossConfigImpl
    {
    public:
        CCJGW_LoadCalDBLossConfig(void);
        ~CCJGW_LoadCalDBLossConfig(void);
        //! 
        bool LoadLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig);
        //! 
        std::wstring GetLossConfigXmlPath();
        //! 
        bool SaveLossConfig(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_CAL_DB_LOSS_CONFIG& sCalDBLossConfig);
    private:
        bool PareseRootNode(TiXmlElement* xmlRoot);
        bool SaveRootNode(TiXmlElement* xmlRoot);
    private:
        PS_CAL_DB_LOSS_CONFIG mpsCalDBLossConfig;
        LPS_EQUIP_PROJECT_CONFIG mpsEquipProjectConfig;
        std::string mstrConfigFilePath;
    };
}