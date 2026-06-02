#include "StdAfx.h"
#include "CJGW_ResolveQdartHtml.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_ResolveQdartHtml::CCJGW_ResolveQdartHtml(void)
    {
        mszReadBuf = (char*)calloc(HTML_STRING_LINE_MAX_LENGTH + 1,1);
    }


    CCJGW_ResolveQdartHtml::~CCJGW_ResolveQdartHtml(void)
    {
        if (mszReadBuf)
        {
            free(mszReadBuf);
            mszReadBuf = NULL;
        }
    }

    std::string CCJGW_ResolveQdartHtml::GetErrorMsg()
    {
        return mstrErrorMsg;
    }

    std::string CCJGW_ResolveQdartHtml::GetTestResult()
    {
        return mstrPassTestList.str();
    }

    std::string CCJGW_ResolveQdartHtml::GetErrorTestResult()
    {
        return mstrFailTestList.str();
    }

    bool CCJGW_ResolveQdartHtml::LoadResolveQdartHtml(const std::string& strQdartHtmlFilePath,const S_HTML_QDART_RESOLVE_CONFIG_INFO& sHtmlQdartResolveConfigInfo,bool bIsParsePassProject /* = true */)
    {
        std::ifstream htmlStream;
        htmlStream.imbue(std::locale(""));
        htmlStream.open(strQdartHtmlFilePath);
        if (!htmlStream.is_open())
        {
            mstrErrorMsg = "open qdart html fail";
            mstrErrorMsg += strQdartHtmlFilePath;
            return false;
        }
        mstrPassTestList.clear();
        mstrFailTestList.clear();
        //! 解析测试项目结果,并且定位到指定HREF
        S_HTML_TABLE_INFO sHtmlTableInfo;
        {
            std::string strAllTestInfoTitleTableContent = GetTableFileContentToHtml(htmlStream,sHtmlQdartResolveConfigInfo.mstrAllTestInfoTitle,sHtmlQdartResolveConfigInfo.mstrStartTableFlag,sHtmlQdartResolveConfigInfo.mstrEndTableFlag);
            if (strAllTestInfoTitleTableContent.empty())
            {
                mstrErrorMsg = "get test project result content fail";
                return false;
            }
            ParseTableQdartHtml(strAllTestInfoTitleTableContent,sHtmlTableInfo);
        }
        //! 解析指定测试项目下的测试内容
        ParseTestProjectHtml(sHtmlQdartResolveConfigInfo,sHtmlTableInfo,htmlStream,bIsParsePassProject);

        return true;
    }

    std::string CCJGW_ResolveQdartHtml::GetTableFileContentToHtml(std::ifstream& htmlStream,const std::string& strTableTitle,const std::string& strBegin /* = "<table border/* =1>" */,const std::string& strEnd /* = "</table>" */)
    {
        std::string strRead;
        bool bReadTitle = false,bReadStart = false;
        memset(mszReadBuf,0x00,HTML_STRING_LINE_MAX_LENGTH);
        std::streamoff pos = htmlStream.tellg();
        while (htmlStream.getline(mszReadBuf,HTML_STRING_LINE_MAX_LENGTH))
        {
            if (strTableTitle.empty())
            {
                bReadTitle = true;
            }

            if (!bReadTitle)
            {
                if (NULL != JGW_StrCaseStr(mszReadBuf,strTableTitle.c_str()))
                {
                    if (strBegin.empty() && strEnd.empty())
                    {
                        strRead = mszReadBuf;
                        return strRead;
                    }
                    bReadTitle = true;
                    memset(mszReadBuf,0x00,HTML_STRING_LINE_MAX_LENGTH);
                    continue;
                }
            }
            if (!bReadStart)
            {
                if (NULL != JGW_StrCaseStr(mszReadBuf,strBegin.c_str()))
                {
                    bReadStart = true;
                    strRead = mszReadBuf;
                    memset(mszReadBuf,0x00,HTML_STRING_LINE_MAX_LENGTH);
                    continue;
                }
            }
            if (bReadTitle && bReadStart)
            {
                strRead += mszReadBuf;
                if (NULL != JGW_StrCaseStr(mszReadBuf,strEnd.c_str()))
                {
                    return strRead;
                }
            }
        }
        htmlStream.seekg(pos);
        return "";
    }

    bool CCJGW_ResolveQdartHtml::ParseTableQdartHtml(const std::string& strTableContent,S_HTML_TABLE_INFO& sHtmlTableInfo)
    {
        //! <table><tr><td></td></tr></table>
        std::string strStartTR = "<tr";
        std::string strEndTR = "</tr>";
        std::string strTr,strTableTemp (strTableContent);
        bool bHeader = true;

        while (std::string::npos != strTableTemp.find(strEndTR))
        {
            size_t nEndTRPos = strTableTemp.find(strEndTR);
            strTr = JGW_GetSubStrToNotEraseStartEndStr(strTableTemp,"<tr","</tr>");
            if (bHeader)
            {
                bHeader = false; 
                PareseTableTr(strTr,sHtmlTableInfo.mvstrTableHeader);
                //sHtmlTableInfo.mvvstrTableContents.push_back(vsHtmlTableColumn);
            }
            else
            {
                std::vector<S_HTML_TABLE_COLUMN> vsHtmlTableColumn;
                PareseTableTr(strTr,vsHtmlTableColumn);
                sHtmlTableInfo.mvvstrTableContents.push_back(vsHtmlTableColumn);
            }
            if (nEndTRPos + strEndTR.length() < strTableTemp.length())
            {
                strTableTemp = strTableTemp.substr(nEndTRPos + strEndTR.length());
            }
            else
            {
                break;
            }
        }
        return true;
    }

    bool CCJGW_ResolveQdartHtml::PareseTableTr(const std::string& strTRContent,std::vector<S_HTML_TABLE_COLUMN>& vsHtmlTableColumn)
    {
        std::string strStartTD = "<td",strEndTD = "</td>",strContentTemp(strTRContent),strTestFailFlag("bgcolor=\"red\"");
        while (std::string::npos != strContentTemp.find(strEndTD))
        {
            std::string strTDContent = JGW_GetSubStrToNotEraseStartEndStr(strContentTemp,strStartTD,strEndTD);
            S_HTML_TABLE_COLUMN sHtmlTableColumn;
            sHtmlTableColumn.mbTestResult = true;
            if (std::string::npos != strTDContent.find(strTestFailFlag))
            {
                sHtmlTableColumn.mbTestResult = false;
            }
            sHtmlTableColumn.mstrTestValue = GetHtmlSectionContent(strTDContent);
            vsHtmlTableColumn.push_back(sHtmlTableColumn);


            size_t nEndTDPos = strContentTemp.find(strEndTD);
            if (nEndTDPos + strEndTD.length() < strContentTemp.length())
            {
                strContentTemp = strContentTemp.substr(nEndTDPos + strEndTD.length());
            }
            else
            {
                break;
            }
            //strTDContent = JGW_GetSubStrToNotEraseStartEndStr(strContentTemp,strStartTD,strEndTD);
        }
        return true;
    }

    std::string CCJGW_ResolveQdartHtml::GetHtmlSectionContent(const std::string& strContent)
    {
        //! bgcolor="red"
        std::string strTemp (strContent);
        JGW_EraseLastAndFristTwoChars(strTemp);
        JGW_EraseLastAndFristTwoChars(strTemp,' ',' ');

        while (!strTemp.empty() && strTemp[0] == '<')
        {
            size_t pos = strTemp.find_first_of('>');
            strTemp.erase(0,pos + 1);
            JGW_EraseLastAndFristTwoChars(strTemp);
            JGW_EraseLastAndFristTwoChars(strTemp,' ',' ');
        }

        while (!strTemp.empty() && strTemp[strTemp.length() - 1] == '>')
        {
            size_t pos = strTemp.find_last_of('<');
            strTemp.erase(pos);
            JGW_EraseLastAndFristTwoChars(strTemp);
            JGW_EraseLastAndFristTwoChars(strTemp,' ',' ');
        }

        return strTemp;
    }
    
    bool CCJGW_ResolveQdartHtml::ParseTestProjectHtml(const S_HTML_QDART_RESOLVE_CONFIG_INFO& sHtmlQdartResolveConfigInfo,S_HTML_TABLE_INFO& sHtmlTableInfo,std::ifstream& htmlStream,bool bIsParsePassProject /* = true */)
    {
        //! 默认第一项是测试项目 =>  Test Name
        for (size_t i = 0;i < sHtmlTableInfo.mvvstrTableContents.size();i ++)
        {
            if (sHtmlTableInfo.mvvstrTableContents[i].size() < 2)
            {
                return false;
            }
            std::string strTestName = sHtmlTableInfo.mvvstrTableContents[i][0].mstrTestValue;
            //! 如果不解析PASS测试项目与当前测试项目结果为PASS则直接返回
            if (!bIsParsePassProject && sHtmlTableInfo.mvvstrTableContents[i][1].mbTestResult)
            {
                continue;
            }
            for (size_t index = 0;index < sHtmlQdartResolveConfigInfo.mvsResolveTestProjectInfo.size();index ++)
            {
                //! 如果相等，则开始查找当前测试项目测试结果
                if (NULL == JGW_StrComparenoCaseStr(strTestName.c_str(),sHtmlQdartResolveConfigInfo.mvsResolveTestProjectInfo[index].mstrTestName.c_str()))
                {
                    std::string strHref,strTableContent;//! <a name="%d" xmlns:msxsl="urn:schemas-microsoft-com:xslt">
                    JGW_FormatString(strHref,sHtmlQdartResolveConfigInfo.mstrTableHrefFormat.c_str(),i + 1);
                    //! HTML文件跳转
                    std::string strProjectContent = GetTableFileContentToHtml(htmlStream,strHref,"","");
                    std::streamoff hrefPos = htmlStream.tellg();
                    for (size_t testIndex = 0;testIndex < sHtmlQdartResolveConfigInfo.mvsResolveTestProjectInfo[index].mvsHtmlQdartTableInfo.size();testIndex ++)
                    {
                        //! 找到当前测试项目
                        if (NULL != JGW_StrCaseStr(strProjectContent.c_str(),sHtmlQdartResolveConfigInfo.mvsResolveTestProjectInfo[index].mvsHtmlQdartTableInfo[testIndex].mstrTableContentHtmlFlag.c_str()))
                        {
                            strTableContent = GetTableFileContentToHtml(htmlStream,"",sHtmlQdartResolveConfigInfo.mstrStartTableFlag,sHtmlQdartResolveConfigInfo.mstrEndTableFlag);
                        }
                        else
                        {
                            strTableContent = GetTableFileContentToHtml(htmlStream,sHtmlQdartResolveConfigInfo.mvsResolveTestProjectInfo[index].mvsHtmlQdartTableInfo[testIndex].mstrTableContentHtmlFlag,sHtmlQdartResolveConfigInfo.mstrStartTableFlag,sHtmlQdartResolveConfigInfo.mstrEndTableFlag);
                        }
                        //! TABLE不为空则开始解析TABLE
                        if (!strTableContent.empty())
                        {
                            ParseTestItemTableHtml(sHtmlQdartResolveConfigInfo.mvsResolveTestProjectInfo[index].mvsHtmlQdartTableInfo[testIndex],strTableContent);
                        }
                        htmlStream.seekg(hrefPos);
                    }
                }
            }
        }

        return false;
    }

    bool CCJGW_ResolveQdartHtml::ParseTestItemTableHtml(S_HTML_QDART_TABLE_CONFIG_INFO sHtmlQdartTableInfo,const std::string& strTableContent)
    {
        S_HTML_TABLE_INFO sHtmlTableInfo;
        ParseTableQdartHtml(strTableContent,sHtmlTableInfo);
        std::vector<int> mnColumnIndex;
        //! 遍历测试项列
        for (size_t i = 0;i < sHtmlQdartTableInfo.mvstrTableColumnName.size();i ++)
        {
            //! 遍历需要记录的测试列
            for (size_t index = 0;index < sHtmlTableInfo.mvstrTableHeader.size();index ++)
            {
                //! 比较测试列是否与需要记录的测试列相同
                if (NULL == JGW_StrComparenoCaseStr(sHtmlQdartTableInfo.mvstrTableColumnName[i].c_str(),sHtmlTableInfo.mvstrTableHeader[index].mstrTestValue.c_str()))
                {
                    mnColumnIndex.push_back(index);
                    break;
                    /*for (size_t listIndex = 0;listIndex < sHtmlTableInfo.mvvstrTableContents.size();listIndex ++)
                    {
                    if (sHtmlTableInfo.mvvstrTableContents[listIndex].size() > index)
                    {

                    }
                    }*/
                }
            }
            if (mnColumnIndex.size() < i + 1)
            {
                for (size_t index = 0;index < sHtmlTableInfo.mvstrTableHeader.size();index ++)
                {
                    //! 查找测试列是否与需要记录的测试列相同
                    if (NULL != JGW_StrCaseStr(sHtmlQdartTableInfo.mvstrTableColumnName[i].c_str(),sHtmlTableInfo.mvstrTableHeader[index].mstrTestValue.c_str()))
                    {
                        mnColumnIndex.push_back(index);
                        break;
                    }
                }
                if (mnColumnIndex.size() < i + 1)
                {
                    mnColumnIndex.push_back(-1);
                } 
            }
        }
        for (size_t listIndex = 0;listIndex < sHtmlTableInfo.mvvstrTableContents.size();listIndex ++)
        {
            bool bResult = true;
            std::string strResult (sHtmlQdartTableInfo.mstrTableContentShow);
            for (size_t i = 0;i < mnColumnIndex.size();i ++)
            {
                std::string strIndex;
                JGW_FormatString(strIndex,"{%d}",i);
                if (-1 == mnColumnIndex[i])
                {
                    continue;
                }
                if (sHtmlTableInfo.mvvstrTableContents[listIndex].size() > (size_t)mnColumnIndex[i])
                {
                    bResult &= sHtmlTableInfo.mvvstrTableContents[listIndex][mnColumnIndex[i]].mbTestResult;
                    //if (sHtmlTableInfo.mvvstrTableContents[listIndex][mnColumnIndex[i]].mbTestResult)
                    //{

                    //}
                    //else
                    //{
                        JGW_ReplaceStringA(strResult,strIndex.c_str(),sHtmlTableInfo.mvvstrTableContents[listIndex][mnColumnIndex[i]].mstrTestValue.c_str());
                    //}   
                }  
            }
            
            if (bResult)
            {
                mstrPassTestList << strResult << "\tPASS" << std::endl;
            }
            else
            {
                mstrFailTestList << strResult << "\tFAIL" << std::endl;
            }
        }
        return true;
    }
}
