#pragma once
#include <string>
#include <unordered_map>
namespace JGW
{
    class CCJGW_PropertyImpl
    {
    public:
        //! 从全局环境变量中Get对应itemName 或者 double类型itemValue
        virtual double GetDouble(const std::wstring& itemName) = 0;
        //! 从全局环境变量中Get对应itemName 或者 int类型itemValue
        virtual int GetInt(const std::wstring& itemName) = 0;
        //! 从全局环境变量中Get对应itemName 或者 int类型itemValue
        virtual __int64 GetInt64(const std::wstring& itemName) = 0;
        //! 从全局环境变量中Get对应itemName 或者 string类型itemValue
        virtual std::wstring GetString(const std::wstring& itemName) = 0;
        //! 从全局环境变量中Get对应itemName 或者 float类型itemValue
        virtual float GetFloat(const std::wstring& itemName) = 0;
        //! 从全局环境变量中Get对应itemName 或者 bool类型itemValue
        virtual bool GetBool(const std::wstring& itemName) = 0;

        //! put itemName ,itemValue 到全局环境变量
        virtual void PutDouble(const std::wstring& itemName,double itemValue) = 0;
        //! put itemName ,itemValue 到全局环境变量
        virtual void PutInt(const std::wstring& itemName,int itemValue) = 0;
        //! put itemName ,itemValue 到全局环境变量
        virtual void PutInt64(const std::wstring& itemName,__int64 itemValue) = 0;
        //! put itemName ,itemValue 到全局环境变量
        virtual void PutString(const std::wstring& itemName,const std::wstring& itemValue) = 0;
        //! put itemName ,itemValue 到全局环境变量
        virtual void PutFloat(const std::wstring& itemName,float itemValue) = 0;
        //! put itemName ,itemValue 到全局环境变量
        virtual void PutBool(const std::wstring& itemName,bool itemValue) = 0;
    public:
        //! 
        virtual void CopyProperty(std::unordered_map<std::wstring,std::wstring>& mapProperty) = 0;
        //!
        virtual const std::unordered_map<std::wstring,std::wstring>::iterator Begin() = 0;
        //! 
        virtual const std::unordered_map<std::wstring,std::wstring>::iterator End() = 0;
        //!
        virtual void SetPropertyAppName(const std::wstring& strPropertyAppName = L"propety") = 0;
        //!
        virtual void ReloadPropertyFile() = 0;
    };
}