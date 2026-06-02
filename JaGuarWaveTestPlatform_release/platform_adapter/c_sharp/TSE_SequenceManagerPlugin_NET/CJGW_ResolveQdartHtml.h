#pragma once
#include <fstream>
#include <sstream>
#include "HtmlQdartResolveDefine.h"
#define HTML_STRING_LINE_MAX_LENGTH     1024 * 10
namespace JGW
{
    class CCJGW_ResolveQdartHtml
    {
    public:
        CCJGW_ResolveQdartHtml(void);
        ~CCJGW_ResolveQdartHtml(void);
    public:
        bool LoadResolveQdartHtml(const std::string& strQdartHtmlFilePath,const S_HTML_QDART_RESOLVE_CONFIG_INFO& sHtmlQdartResolveConfigInfo,bool bIsParsePassProject = true);
        std::string GetErrorMsg();
        std::string GetTestResult();
        std::string GetErrorTestResult();
    private:
        //! 解析HTML测试结果
        bool ParseTestProjectHtml(const S_HTML_QDART_RESOLVE_CONFIG_INFO& sHtmlQdartResolveConfigInfo,S_HTML_TABLE_INFO& sHtmlTableInfo,std::ifstream& htmlStream,bool bIsParsePassProject = true);
        //! 解析HTML Table
        bool ParseTableQdartHtml(const std::string& strTableContent,S_HTML_TABLE_INFO& sHtmlTableInfo);
        //! 从文件中获取HTML TABLE内容
        //! strTableTitle 为空的时候，直接从当前位置开始解析
        //! strBegin 和 strEnd为空，直接返回strTableTitle对应位置行内容
        std::string GetTableFileContentToHtml(std::ifstream& htmlStream,const std::string& strTableTitle,const std::string& strBegin = "<table border=\"1\">",const std::string& strEnd = "</table>");
        //! 解析HTML TABLE TR
        bool PareseTableTr(const std::string& strTRContent,std::vector<S_HTML_TABLE_COLUMN>& vsHtmlTableColumn);
        //! 解析测试项的TABLE
        bool ParseTestItemTableHtml(S_HTML_QDART_TABLE_CONFIG_INFO sHtmlQdartTableInfo,const std::string& strTableContent);
        //! 获取HTML段值
        std::string GetHtmlSectionContent(const std::string& strContent);
    private:
        char* mszReadBuf;
        std::string mstrErrorMsg;
        //! std::string mstrErrorTestResult;
        std::stringstream mstrPassTestList;
        std::stringstream mstrFailTestList;
    };

}
