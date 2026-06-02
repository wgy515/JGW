#pragma once
#include <tinyxml/tinyxml.h>
#include "CTSE_ConfigEditTool_Define.h"
namespace JGW
{
    class CTSE_LoadTSESuiteConfig
    {
    public:
        CTSE_LoadTSESuiteConfig(S_TSE_SequenceConfig& sTSESequenceConfig);
        ~CTSE_LoadTSESuiteConfig(void);
    public:
        bool PareseTSESuiteConfig(const std::string& strConfigXml);

        bool SaveAsTSESuiteConfig(const std::string& strSaveXmlFilePath);

        bool SaveTSESuiteConfig();
    private:
        //! 解析所有测试节点
        bool PareseAllTestProjectNode(const TiXmlElement* xmlRoot);
        //! 解析Common节点参数
        void PareseCommonNode(const TiXmlElement* xmlRoot);
        //! 解析单个Item节点
        void PareseItemNode(const TiXmlElement* xmlNode,_vsuitetestconfig& vsuitetTestConfig);
        //! 解析config节点
        void PareseConfigNode(const TiXmlElement* xmlRoot);
        //! 解析auto
        void PareseAutoNode(const TiXmlElement* xmlRoot);
        //! 解析SuiteProject
        bool PareseSuiteProject(const TiXmlElement* xmlRoot);
    public:
        S_TSE_SequenceConfig& msTSESequenceConfig;
        std::unique_ptr<TiXmlDocument> mptrXmlDoc;
    };
}