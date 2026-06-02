#pragma once
//! QIA XTT测试
//! 主要用于WIFI校准综测， 以及其他XTT测试
#include <string>
#include <map>
namespace JGW
{
    class CCJGW_QIATest
    {
    public:
        CCJGW_QIATest(void);
        ~CCJGW_QIATest(void);
    public:
        void SetTestXttPath(const std::string& strXttFilePath);

        void SetGlobalVariable(const std::string& gvName,const std::string& value);

        std::string GetGlobalVariable(const std::string& gvName);

        void ClearGlobalVariable();
    public:
        bool ExecuteXttTest();
    private:
        bool mbOpenTestTree;
        //! com 接口
        ITestTreePtr mptrITestTreePtr;
        //! XTT 文件路径
        std::string mstrXttFilePath;
        std::map<std::string,std::string> mmapGlobalVariable;
    };
}


