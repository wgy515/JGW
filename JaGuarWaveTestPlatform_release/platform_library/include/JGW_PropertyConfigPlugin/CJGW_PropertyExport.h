#pragma once
#include <JGW_PropertyConfigPlugin/CJGW_PropertyConfig_Define.h>
#include <JGW_PropertyConfigPlugin/CJGW_PropertyImpl.h>
#ifdef __cplusplus
extern "C" 
{
#endif
    //! 
    JGW_PROPERTYCONFIGPLUGIN_API JGW::CCJGW_PropertyImpl* GetMemoryProperty();
    //! 
    JGW_PROPERTYCONFIGPLUGIN_API JGW::CCJGW_PropertyImpl* GetFileProperty();
    //! 
    JGW_PROPERTYCONFIGPLUGIN_API JGW::CCJGW_PropertyImpl* GetFilePropertyToFileName(const wchar_t* strPropertyFileName);
    //!
    JGW_PROPERTYCONFIGPLUGIN_API void DestroyFilePropertyToPropertyImplPtr(JGW::CCJGW_PropertyImpl* pPropertyImpl);
    //!
    JGW_PROPERTYCONFIGPLUGIN_API void DestroyFilePropertyToFileName(const wchar_t* strPropertyFileName);
#ifdef __cplusplus
};
#endif