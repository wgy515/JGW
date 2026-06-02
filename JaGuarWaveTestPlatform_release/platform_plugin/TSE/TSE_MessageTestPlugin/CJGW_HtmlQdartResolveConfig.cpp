#include "StdAfx.h"
#include "CJGW_HtmlQdartResolveConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_HtmlQdartResolveConfig::CCJGW_HtmlQdartResolveConfig(void)
    {
    }


    CCJGW_HtmlQdartResolveConfig::~CCJGW_HtmlQdartResolveConfig(void)
    {
    }

    bool CCJGW_HtmlQdartResolveConfig::LoadHtmlQdartResolveConfig(const std::wstring& strHtmlQdartResolveConfigPath,S_HTML_QDART_RESOLVE_CONFIG_INFO& sHtmlQdartResolveInfo)
    {
        CCJGW_ConfigIni cConfigIni;
        cConfigIni.InitIniFilePath(strHtmlQdartResolveConfigPath.c_str());

        sHtmlQdartResolveInfo.mstrAllTestInfoTitle = cConfigIni.GetIniKeyValueA(L"MAIN",L"AllTestInfoTitle",L"Table of Contents");
        sHtmlQdartResolveInfo.mstrStartTableFlag = cConfigIni.GetIniKeyValueA(L"MAIN",L"StartTableFlag",L"<table border=\"1\">");
        sHtmlQdartResolveInfo.mstrEndTableFlag = cConfigIni.GetIniKeyValueA(L"MAIN",L"EndTableFlag",L"</table>");
        sHtmlQdartResolveInfo.mstrTableHrefFormat = cConfigIni.GetIniKeyValueA(L"MAIN",L"TableHrefFormat",L"<a name=\"%d\" xmlns:msxsl=\"urn:schemas-microsoft-com:xslt\">");
        sHtmlQdartResolveInfo.mstrResolveTestProject = cConfigIni.GetIniKeyValueA(L"MAIN",L"ResolveTestProject");
        if (sHtmlQdartResolveInfo.mstrResolveTestProject.empty())
        {
            return true;
        }

        std::vector<std::string> vTempTestProjectName;
        JGW_ParserStrA(sHtmlQdartResolveInfo.mstrResolveTestProject.c_str(),",",vTempTestProjectName);
        for (size_t i = 0;i < vTempTestProjectName.size();i ++)
        {
            S_HTML_QDART_PROJECT_CONFIG_INFO sHtmlQdartProjectInfo;
            sHtmlQdartProjectInfo.mstrTestName = vTempTestProjectName[i];
            if (LoadTestProjectConfig(vTempTestProjectName[i],sHtmlQdartProjectInfo,cConfigIni))
            {
                sHtmlQdartResolveInfo.mvsResolveTestProjectInfo.push_back(sHtmlQdartProjectInfo);
            }
        }
        return true;
    }

    bool CCJGW_HtmlQdartResolveConfig::LoadTestProjectConfig(const std::string& strTestProjectName,S_HTML_QDART_PROJECT_CONFIG_INFO& sHtmlQdartProjectInfo,CCJGW_ConfigIni& configIni)
    {
        std::wstring wstrTestProjectName = JGW_A2W(strTestProjectName);
        std::string strTableTitles = configIni.GetIniKeyValueA(wstrTestProjectName.c_str(),L"TableTitles");
        if (strTableTitles.empty())
        {
            return false;
        }
        //std::vector<std::string> vTempTableTitle;
        std::wstring strTableTitle;
        std::string strTableColumnNames,strTestProjectHtmlFlag;
        JGW_ParserStrA(strTableTitles.c_str(),",",sHtmlQdartProjectInfo.mvTableTitleName);
        for (size_t i = 0;i < sHtmlQdartProjectInfo.mvTableTitleName.size();i ++)
        {
            S_HTML_QDART_TABLE_CONFIG_INFO sHtmlQdartTableInfo;
            sHtmlQdartTableInfo.mstrTableTitleName = sHtmlQdartProjectInfo.mvTableTitleName[i];
            strTableTitle = JGW_A2W(sHtmlQdartTableInfo.mstrTableTitleName);
            
            strTableColumnNames = configIni.GetIniKeyValueA(wstrTestProjectName.c_str(),strTableTitle.c_str());

            strTestProjectHtmlFlag = configIni.GetIniKeyValueA(wstrTestProjectName.c_str(),(strTableTitle + L"_HtmlFlag").c_str());
            sHtmlQdartTableInfo.mstrTableContentHtmlFlag = strTestProjectHtmlFlag.empty() ?sHtmlQdartTableInfo.mstrTableTitleName : strTestProjectHtmlFlag;
            sHtmlQdartTableInfo.mstrTableContentShow = configIni.GetIniKeyValueA(wstrTestProjectName.c_str(),(strTableTitle + L"_Show").c_str());
            JGW_ParserStrA(strTableColumnNames.c_str(),",",sHtmlQdartTableInfo.mvstrTableColumnName);
            sHtmlQdartProjectInfo.mvsHtmlQdartTableInfo.push_back(sHtmlQdartTableInfo);
        }
        return true;
    }

}
