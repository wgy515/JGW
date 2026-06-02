#pragma once
#include <string>
#include "JGW_CurlPluginDefine.h"
namespace JGW
{

    class JGW_CURLPLUGIN_CLASS CCJGW_CurlHttp
    {
    public:
        CCJGW_CurlHttp(const std::string& strSourceAddress);
        ~CCJGW_CurlHttp(void);
    public:
        void SetSourceAddress(const std::string& strSourceAddress);
		// 默认get
		bool SendCurlHttp(const std::wstring& strUrl,bool bSaveCookieFile,bool bUseCookieFile,const std::wstring& strCookiePath,std::wstring& strResponce,std::wstring& strErrorMsg);
		// 默认get URL以及文件路径
		bool SendCurlHttpA(const std::string& strUrl,bool bSaveCookieFile,bool bUseCookieFile,const std::string& strCookiePath,std::string& strResponceUtf8,std::string& strErrorMsgUtf8);

        //! strHeader = Content-Type:application/json;charset=UTF-8
        bool SendCurlHttpPostData(const std::wstring& strUrl,const std::wstring& strPostData,std::wstring& strResponce,std::wstring& strErrorMsg,const std::wstring& strHeader = L"Content-Type:text/html;charset=UTF-8");
        //! field name
        bool UploadCurlHttpPostFile(const std::wstring& strUrl,const std::wstring& strLocalFilePath,std::wstring& strResponce,std::wstring& strErrorMsg,const std::wstring& strFieldName = L"fileName");
    private:
        char mszSourceAddress[128];
    };
}


