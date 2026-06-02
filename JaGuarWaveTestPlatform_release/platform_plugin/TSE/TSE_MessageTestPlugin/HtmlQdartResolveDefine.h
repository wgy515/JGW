#pragma once
#include <string>
#include <vector>

namespace JGW
{
    /************************************************************************/
    /*          CONFIG INI                                                  */
    /************************************************************************/
    typedef struct
    {
        //! TableTitles=LTE_MaxPower
        //! LTE_MaxPower 测试项目名称
        std::string mstrTableTitleName;
        //! LTE_MaxPower_HtmlFlag=<b xmlns:msxsl="urn:schemas-microsoft-com:xslt">LTE_MaxPower</b>
        std::string mstrTableContentHtmlFlag;
        //! LTE_MaxPower_Show=Band-{0},Channel-{1},{2},{3},{4},TxPwr{5}({6}-{7})
        std::string mstrTableContentShow;
        //! LTE_MaxPower=Band (),Channel (),Ch_BW (MHz),UL_Start_RB (),UL_Num_RB (),AvgTxPwr (dBm)
        std::vector<std::string> mvstrTableColumnName;   
    }S_HTML_QDART_TABLE_CONFIG_INFO;

    typedef struct
    {
        //! std::string mstrTableTitles;
        //! Table of Contents  Test Name
        std::string mstrTestName;
        std::vector<std::string> mvTableTitleName;
        std::vector<S_HTML_QDART_TABLE_CONFIG_INFO> mvsHtmlQdartTableInfo;
    }S_HTML_QDART_PROJECT_CONFIG_INFO;

    typedef struct
    {
        std::string mstrAllTestInfoTitle;
        std::string mstrTableHrefFormat;
        std::string mstrStartTableFlag;
        std::string mstrEndTableFlag;
        std::string mstrResolveTestProject;
        std::vector<S_HTML_QDART_PROJECT_CONFIG_INFO> mvsResolveTestProjectInfo;
    }S_HTML_QDART_RESOLVE_CONFIG_INFO;
    /* example HTML_QDART_Resolve.ini
    [MAIN]
    AllTestInfoTitle=Table of Contents
    TableHrefFormat=<a name="%d" xmlns:msxsl="urn:schemas-microsoft-com:xslt">
    ResolveTestProject=LTE_MaxPower,LTE_TxQuality,LTE_OccupiedBandwidth


    [LTE_MaxPower]
    TableTitles=LTE_MaxPower
    LTE_MaxPower=Band (),Channel (),Ch_BW (MHz),UL_Start_RB (),UL_Num_RB (),AvgTxPwr (dBm)
    LTE_MaxPower_Show=Band-{0},Channel-{1},{2},{3},{4},TxPwr{5}({6}-{7})

    [LTE_TxQuality]
    TableTitles=LTE_TxQuality,Freq Error,EVM,Origin Offset,Spectrum Flatness
    LTE_TxQuality=Band (),Channel (),Ch_BW (MHz),UL_Start_RB (),UL_Num_RB (),AvgTxPwr (dBm),AvgTxPwr Min,AvgTxPwr Max
    LTE_TxQuality_Show=Band-{0},Channel-{1},{2},{3},{4},TxPwr{5}({6}-{7})*/

    typedef struct
    {
        bool mbTestResult;
        std::string mstrTestValue;
    }S_HTML_TABLE_COLUMN;

    typedef struct  
    {
        //! HTML 表头
        std::vector<S_HTML_TABLE_COLUMN> mvstrTableHeader;
        //! HTML 表内容
        std::vector<std::vector<S_HTML_TABLE_COLUMN>> mvvstrTableContents;
    }S_HTML_TABLE_INFO;


}