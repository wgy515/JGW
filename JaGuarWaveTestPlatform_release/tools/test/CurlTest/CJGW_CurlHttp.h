#pragma once
#include <string>
namespace JGW
{
    class CCJGW_CurlHttp
    {
    public:
        CCJGW_CurlHttp(void);
        ~CCJGW_CurlHttp(void);
    public:
        //! strHeader = Content-Type:application/json;charset=UTF-8
        bool SendCurlHttpPostData(const std::wstring& strUrl,const std::wstring& strPostData,std::wstring& strResponce,std::wstring& strErrorMsg,const std::wstring& strHeader = L"Content-Type:text/html;charset=UTF-8");
        //! field name
        bool UploadCurlHttpPostFile(const std::wstring& strUrl,const std::wstring& strLocalFilePath,std::wstring& strResponce,std::wstring& strErrorMsg,const std::wstring& strFieldName = L"fileName");
    };
}


