#pragma once
#include <string>
#include <vector>
namespace JGW
{
    typedef struct
    {
        std::wstring mstrVersion;
        std::wstring mstrUpdatePack;
        std::wstring mstrUpdatePackMD5;
    }S_JGW_HELP_UPDATE,*LPS_JGW_HELP_UPDATE;

    typedef struct  
    {
        size_t mnTimeStamp; //! 编译时间
        std::wstring mstrVersion; //! 
        std::wstring mstrFileName;
        std::wstring mstrMD5;
    }S_JGW_HELP_INSTALL,*LPS_JGW_HELP_INSTALL;

    typedef struct  
    {
        std::wstring mstrInstallVersion;
        S_JGW_HELP_UPDATE msUpdateInfo;
    }S_JGW_HELP_UPDATE_PACKAGE,*LPS_JGW_HELP_UPDATE_PACKAGE;

    typedef struct
    {
        //! 版本限制
        bool mbVersieLimiet; //! 是否限制版本 默认不限制版本，则每次更新的时候，最新版本默认都满足生产要求
        std::wstring mstrUpdateVersion; //! 需要更新的版本号
        std::vector<S_JGW_HELP_UPDATE_PACKAGE> mvsHelpUpdatePackage;
    }S_JGW_HELP_UPDATE_XML_CONFIG,*LPS_JGW_HELP_UPDATE_XML_CONFIG;

    typedef struct  
    {
        S_JGW_HELP_INSTALL msNewInstallConfig; //! 最新的安装版本信息
        std::vector<S_JGW_HELP_INSTALL> mvstrInstallInfo;
    }S_JGW_HELP_INSTALL_XML_CONFIG,*LPS_JGW_HELP_INSTALL_XML_CONFIG;

}