#include "StdAfx.h"
#include "CJGW_EquipConfigImpl.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_EquipConfigImpl::CCJGW_EquipConfigImpl(void) : mpsEquipProjectConfig(NULL)
    {
    }


    CCJGW_EquipConfigImpl::~CCJGW_EquipConfigImpl(void)
    {
    }

    void CCJGW_EquipConfigImpl::GetEquipConfigXmlPath()
    {
        std::wstring strTemp(mpsEquipProjectConfig->mstrModuleFolder);
        strTemp += L"\\";;
        strTemp += mpsEquipProjectConfig->mstrConfigFileName;
        mstrConfigFilePath = JGW_W2A(JGW_RealativePathToAbsPath(strTemp.c_str()));
    }

    bool CCJGW_EquipConfigImpl::LoadEquipConfigXml(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo)
    {
        mpsEquipProjectConfig = psEquipProjectConfig;
        GetEquipConfigXmlPath();

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(mstrConfigFilePath.c_str())) return false;
        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !PareseRootNode(xmlRoot,sMonitorInfo))  return false;
        return true;
    }

    bool CCJGW_EquipConfigImpl::SaveEquipConfigXml(LPS_EQUIP_PROJECT_CONFIG psEquipProjectConfig,S_MONITOR_INFO& sMonitorInfo)
    {
        mpsEquipProjectConfig = psEquipProjectConfig;
        GetEquipConfigXmlPath();

        std::unique_ptr<TiXmlDocument> ptrXmlDoc( new TiXmlDocument );
        TiXmlElement* xmlRoot = NULL;

        if (!ptrXmlDoc->LoadFile(mstrConfigFilePath.c_str())) return false;
        if( !( xmlRoot = ptrXmlDoc->RootElement() ) || !ChangeRootNode(xmlRoot,sMonitorInfo))  return false;
        ptrXmlDoc->SaveFile(mstrConfigFilePath);
        return true;
    }

    bool CCJGW_EquipConfigImpl::PareseRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        return true;
    }

    bool CCJGW_EquipConfigImpl::ChangeRootNode(TiXmlElement* xmlRoot,S_MONITOR_INFO& sMonitorInfo)
    {
        return true;
    }
}