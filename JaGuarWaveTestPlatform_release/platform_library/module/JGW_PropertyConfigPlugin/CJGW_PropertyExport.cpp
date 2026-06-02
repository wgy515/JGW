#include "StdAfx.h"
#include <JGW_PropertyConfigPlugin/CJGW_PropertyExport.h>
#include <JGW_FoundationFunc/CJGW_CriticalSectionLock.h>
#include "CJGW_PropertyMemoryConfig.h"
#include "CJGW_PropertyFileConfig.h"
#include <map>

std::map<std::wstring,std::shared_ptr<JGW::CCJGW_PropertyFileConfig>> gmapPropertyFileConfig;

JGW::CCJGW_PropertyImpl* GetMemoryProperty()
{
    static JGW::CCJGW_PropertyMemoryConfig gPropertyMemoryConfig;
    return &gPropertyMemoryConfig;
}
JGW::CCJGW_CriticalSectionLock mcs;

JGW::CCJGW_PropertyImpl* GetFileProperty()
{
#if 0
    JGW::CCJGW_CriticalSectionAutoLock autoLock(mcs);
    //! 
    if (gmapPropertyFileConfig.find(GLOBAL_PROPERTY_CONFIG_INI) == gmapPropertyFileConfig.end())
    {
         std::shared_ptr<JGW::CCJGW_PropertyFileConfig> ptrPropertyFileConfig (new JGW::CCJGW_PropertyFileConfig(GLOBAL_PROPERTY_CONFIG_INI));
        gmapPropertyFileConfig[GLOBAL_PROPERTY_CONFIG_INI] = ptrPropertyFileConfig;
    }
    return gmapPropertyFileConfig[GLOBAL_PROPERTY_CONFIG_INI].get();
#else
    static JGW::CCJGW_PropertyFileConfig gPropertyFileConfig(GLOBAL_PROPERTY_CONFIG_INI);
    return &gPropertyFileConfig;
#endif
}



JGW::CCJGW_PropertyImpl* GetFilePropertyToFileName(const wchar_t* strPropertyFileName)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(mcs);
    //! 
    if (gmapPropertyFileConfig.find(strPropertyFileName) == gmapPropertyFileConfig.end())
    {
        std::shared_ptr<JGW::CCJGW_PropertyFileConfig> ptrPropertyFileConfig (new JGW::CCJGW_PropertyFileConfig(strPropertyFileName));
        gmapPropertyFileConfig[strPropertyFileName] = ptrPropertyFileConfig;
    }
    return gmapPropertyFileConfig[strPropertyFileName].get();
}

void DestroyFilePropertyToPropertyImplPtr(JGW::CCJGW_PropertyImpl* pPropertyImpl)
{
    JGW::CCJGW_CriticalSectionAutoLock autoLock(mcs);
    std::map<std::wstring,std::shared_ptr<JGW::CCJGW_PropertyFileConfig>>::iterator it = gmapPropertyFileConfig.begin();
    while (it != gmapPropertyFileConfig.end())
    {
        if (pPropertyImpl == it->second.get())
        {
            it = gmapPropertyFileConfig.erase(it);
        }
        else
        {
            ++ it;
        }
    }
}

void DestroyFilePropertyToFileName(const wchar_t* strPropertyFileName)
{
    JGW::CCJGW_PropertyImpl* pPropertyImpl = NULL;
    {
        JGW::CCJGW_CriticalSectionAutoLock autoLock(mcs);
        if (gmapPropertyFileConfig.find(strPropertyFileName) == gmapPropertyFileConfig.end()) return;
        pPropertyImpl = gmapPropertyFileConfig[strPropertyFileName].get();
    }
    DestroyFilePropertyToPropertyImplPtr(pPropertyImpl);
}