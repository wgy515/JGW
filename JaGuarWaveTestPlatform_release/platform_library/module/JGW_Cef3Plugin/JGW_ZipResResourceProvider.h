#pragma once
#include <string>

#include <include/cef_resource_handler.h>
#include <include/cef_stream.h>
#include <include/wrapper/cef_zip_archive.h>

#if defined(OS_WIN)
#include <include/wrapper/cef_resource_manager.h>
#endif
//! Origin
#define ZIP_URL_ORIGIN "https://zipres.com/"

namespace JGW
{
	//! ZIP资源文件服务提供
	class JGW_ZipResResourceProvider : public CefResourceManager::Provider
	{
	public:
		JGW_ZipResResourceProvider();
		~JGW_ZipResResourceProvider();
	public:
		static bool SetZipResResource(HINSTANCE hModuleHandle, DWORD dwResourceNameID, const std::wstring& strResourceType,const wchar_t* szPassword);
        static bool SetZipFilePath(const std::wstring& strZipFilePath,const wchar_t* szPassword);
	protected:
		bool OnRequest(scoped_refptr<CefResourceManager::Request> request) OVERRIDE;
	private:
		CefRefPtr<CefStreamReader> GetZipResResourceReader(const std::string& resource_path);
		bool GetZipResBinaryResource(const std::wstring& strResourceName, DWORD& dwSize, LPBYTE& pData);
	private:
		static bool LoadZipResResource(HINSTANCE hModuleHandle, DWORD zipResourceId, const std::wstring& strResourceType);
	private:
		static HINSTANCE ghModuleHandle;
		//! IDR_ZIPRES1
		static DWORD gdwResourceNameID;
		//! _T("ZIPRES")
		static std::wstring gstrResourceType;
        static CefRefPtr<CefStreamReader> gptrCefStreamReader;
        static CefRefPtr<CefZipArchive> gptrCefZipArchive;
	};
}
