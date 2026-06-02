#pragma once
#include <string>
#include <vector>
#include <map>

namespace JGW
{
    struct S_FTM_COMPARE_STRING_ATTR
    {
        //! 是否是错误字符串信息
        bool mbIsErrorStringInfo;
        std::wstring mstrAttrName;
        std::wstring mstrAttrVal;
        std::wstring mstrErrorMsg;
    };

    struct S_FTM_COMPARE_Int_ATTR
    {
        //! 是否是错误整数信息
        bool mbIsErrorIntInfo;
        int mnMinAttrVal;
        int mnMaxAttrVal;   
        std::wstring mstrAttrName;
        std::wstring mstrErrorMsg;
    };
    //////////////////////////////////////////////////////////////////////////
    /// 检查测试结果有效性
    //////////////////////////////////////////////////////////////////////////
    class CTSE_CheckTestResult
    {
    public:
        CTSE_CheckTestResult(void);
        ~CTSE_CheckTestResult(void);
    public:
        //! 添加需要比对的相关测试
        void AddCompareParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        //! 测试参数未改变的情况默认只能初始化一次
        void InitCompareParam();
        //! 检查需要比对的相关测试结果 默认按照环境变量数据来比对
        bool CheckCompareResult(const std::wstring& strRead);
        //! 是否需要比较结果
        bool IsCompareResult();
    protected:
        //! 获取比对参数相关描述
        const wchar_t* GetCompareParamDescription();
        //！比较测试结果
        bool CompareStringResultFile(const std::wstring& strRead);
        //！比较测试数值范围
        bool CompareIntResultFile(const std::wstring& strRead);
        //! 查找当前字符串是否在结果范围内
        bool FindStringResultFile(const std::wstring& strRead);
    protected:
        //! 是否已经初始化相关测试参数
        bool mbInitCompareParam;
        size_t mnCompareStringCount;
        size_t mnCompareIntCount;
        size_t mnFindStringCount;
        std::vector<S_FTM_COMPARE_STRING_ATTR> mvCompareStringAttrs;
        std::vector<S_FTM_COMPARE_STRING_ATTR> mvFindStringAttrs;
        std::vector<S_FTM_COMPARE_Int_ATTR> mvCompareIntAttrs;
        std::map<std::wstring,std::wstring> mmapAttrNameVal;  
    };
}