#pragma once
#include <string>
#include <vector>

namespace JGW
{
    typedef struct
    {
        std::wstring mfFreq;
        std::wstring mstrLoss;
    }S_CAL_POINT_NODE;

    typedef struct  
    {
        int mnNumber;
        std::wstring mstrCalConfigName;
        std::vector<S_CAL_POINT_NODE> mvCalPoint;
    }S_CAL_PATH_NODE,*PS_CAL_PATH_NODE;

    typedef struct  
    {
        std::wstring mstrCalConfigName;
        std::vector<S_CAL_PATH_NODE> mvCalPathNode;
    }S_CAL_CONFIG_NODE;

    typedef struct
    {
        std::vector<S_CAL_CONFIG_NODE> mvCalConfigNode;
    }S_CAL_DB_LOSS_CONFIG,*PS_CAL_DB_LOSS_CONFIG;

    enum E_EQUIP_TYPE
    {
        E_QCOMM_CAL = 0, //! 高通校准XML配置
        E_StationCalDataFile
        //E_QSEQ, //! 高通QSEQ校准配置方式
        //E_EQUIP_CONFIG //! 自定义校准
    };

    typedef struct 
    {
        E_EQUIP_TYPE mcEquipType;
        std::wstring mstrGroupName;
        std::wstring mstrTestName;
        std::wstring mstrModuleFolder;
        std::wstring mstrConfigFileName;
    }S_EQUIP_PROJECT_CONFIG,*LPS_EQUIP_PROJECT_CONFIG;
}