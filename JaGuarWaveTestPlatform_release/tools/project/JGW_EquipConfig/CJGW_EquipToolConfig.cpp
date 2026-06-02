#include "StdAfx.h"
#include "CJGW_EquipToolConfig.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_EquipToolConfig::CCJGW_EquipToolConfig(void)
    {
    }


    CCJGW_EquipToolConfig::~CCJGW_EquipToolConfig(void)
    {
    }

    bool CCJGW_EquipToolConfig::LoadEquipConfig(const std::wstring strEquipConfigIniPath,std::vector<S_EQUIP_PROJECT_CONFIG>& vsEquipProjetConfig)
    {
        CCJGW_ConfigIni equipToolConfig;
        if (!equipToolConfig.InitIniFilePath(strEquipConfigIniPath.c_str())) return false;

        int nProjectCount = equipToolConfig.GetIniKeyIntValue(L"PROJECT",L"PROJECT_COUNT");
        S_EQUIP_PROJECT_CONFIG sEquipProjectConfig;
        std::wstring strTemp;

        for (int i = 1;i <= nProjectCount;i ++)
        {
            JGW_FormatWString(strTemp,L"%d",i);
            sEquipProjectConfig.mstrConfigFileName = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"ConfigName",L"equipconfig.ini");
            sEquipProjectConfig.mstrGroupName = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"GroupName");
            sEquipProjectConfig.mnDeviceID = equipToolConfig.GetIniKeyIntValue(strTemp.c_str(),L"DeviceID",3);
            sEquipProjectConfig.mstrModuleFolder = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"ModuleFolder");
            sEquipProjectConfig.mstrTestName = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"TestName");
            sEquipProjectConfig.mcEquipType = equipToolConfig.GetIniKeyIntValue(strTemp.c_str(),L"EquipConfigType",E_EQUIP_CONFIG);
            vsEquipProjetConfig.push_back(sEquipProjectConfig);
        }

        return true;
    }
}