#include "StdAfx.h"
#include "CJGW_LossToolConfig.h"
#include <JGW_WindowsFuncPlugin/CJGW_ConfigIni.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_LossToolConfig::CCJGW_LossToolConfig(void)
    {
    }


    CCJGW_LossToolConfig::~CCJGW_LossToolConfig(void)
    {
    }

    bool CCJGW_LossToolConfig::LoadLossToolConfig(const std::wstring strLossConfigIniPath,std::vector<S_EQUIP_PROJECT_CONFIG>& vsEquipProjetConfig)
    {
        CCJGW_ConfigIni equipToolConfig;
        if (!equipToolConfig.InitIniFilePath(strLossConfigIniPath.c_str())) return false;

        int nProjectCount = equipToolConfig.GetIniKeyIntValue(L"PROJECT",L"PROJECT_COUNT");
        S_EQUIP_PROJECT_CONFIG sEquipProjectConfig;
        std::wstring strTemp;

        for (int i = 1;i <= nProjectCount;i ++)
        {
            JGW_FormatWString(strTemp,L"%d",i);
            sEquipProjectConfig.mstrConfigFileName = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"ConfigName",L"equipconfig.ini");
            sEquipProjectConfig.mstrGroupName = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"GroupName");
            sEquipProjectConfig.mstrModuleFolder = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"ModuleFolder");
            sEquipProjectConfig.mstrTestName = equipToolConfig.GetIniKeyValue(strTemp.c_str(),L"TestName");
            sEquipProjectConfig.mcEquipType = (E_EQUIP_TYPE)equipToolConfig.GetIniKeyIntValue(strTemp.c_str(),L"EquipConfigType",E_StationCalDataFile);
            vsEquipProjetConfig.push_back(sEquipProjectConfig);
        }

        return true;
    }
}

