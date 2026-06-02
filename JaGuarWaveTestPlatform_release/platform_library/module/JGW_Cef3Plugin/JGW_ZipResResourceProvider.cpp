#include "stdafx.h"
#include "JGW_ZipResResourceProvider.h"
#include <include/wrapper/cef_stream_resource_handler.h>
#include <include/wrapper/cef_byte_read_handler.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <include/wrapper/cef_zip_archive.h>

namespace JGW
{
	std::wstring JGW_ZipResResourceProvider::gstrResourceType = L"ZIPRES";
	HINSTANCE JGW_ZipResResourceProvider::ghModuleHandle = NULL;
	DWORD JGW_ZipResResourceProvider::gdwResourceNameID = 129;

    CefRefPtr<CefStreamReader> JGW_ZipResResourceProvider::gptrCefStreamReader;
    CefRefPtr<CefZipArchive> JGW_ZipResResourceProvider::gptrCefZipArchive;
	DWORD mhZipResourceSize = 0;
	LPBYTE mpZipResourceByte = NULL;
    std::wstring gstrPassWord;

	JGW_ZipResResourceProvider::JGW_ZipResResourceProvider()
	{

	}

	JGW_ZipResResourceProvider::~JGW_ZipResResourceProvider()
	{
        if (gptrCefStreamReader.get())
        {
            gptrCefStreamReader = NULL;
        }
		if (NULL != mpZipResourceByte)
		{
			mhZipResourceSize = NULL;
			delete[] mpZipResourceByte;
			mpZipResourceByte = NULL;
		}	
	}

	bool JGW_ZipResResourceProvider::SetZipResResource(HINSTANCE hModuleHandle, DWORD dwResourceNameID, const std::wstring& strResourceType,const wchar_t* szPassword)
	{
        if (gptrCefStreamReader.get())
        {
            gptrCefStreamReader = NULL;
        }
        if (NULL != mpZipResourceByte)
        {
            mhZipResourceSize = 0;
            delete[] mpZipResourceByte;
            mpZipResourceByte = NULL;
        }	
        gstrPassWord = NULL != szPassword ? szPassword : L"";
		ghModuleHandle = hModuleHandle;
		gdwResourceNameID = dwResourceNameID;
		gstrResourceType = strResourceType;
		return LoadZipResResource(ghModuleHandle, gdwResourceNameID, gstrResourceType);
	}

    bool JGW_ZipResResourceProvider::SetZipFilePath(const std::wstring& strZipFilePath,const wchar_t* szPassword)
    {
        if (gptrCefStreamReader.get())
        {
            gptrCefStreamReader = NULL;
        }
        if (NULL != mpZipResourceByte)
        {
            mhZipResourceSize = 0;
            delete[] mpZipResourceByte;
            mpZipResourceByte = NULL;
        }	
        gstrPassWord = NULL != szPassword ? szPassword : L"";
        if (JGW_FileExistsToFilePath(strZipFilePath.c_str()))
        {
            gptrCefStreamReader = CefStreamReader::CreateForFile(strZipFilePath);
            if (NULL != gptrCefStreamReader.get())
            {
                gptrCefZipArchive = new CefZipArchive();
                gptrCefZipArchive->Load(gptrCefStreamReader,gstrPassWord.c_str(),false);
            } 
        }
        return NULL != gptrCefStreamReader.get() && NULL != gptrCefZipArchive.get();
    }

	bool JGW_ZipResResourceProvider::OnRequest(scoped_refptr<CefResourceManager::Request> request)
	{
		CEF_REQUIRE_IO_THREAD();

		const std::string& url = request->url();
		const std::string root_url_(ZIP_URL_ORIGIN);
		if (url.find(root_url_) != 0L) {
			// Not handled by this provider.
			return false;
		}

		CefRefPtr<CefResourceHandler> handler;

		const std::string& relative_path = url.substr(root_url_.length());
		if (!relative_path.empty()) {
			CefRefPtr<CefStreamReader> stream = GetZipResResourceReader(relative_path.data());
			if (stream.get()) {
				handler = new CefStreamResourceHandler(
					request->mime_type_resolver().Run(url), stream);
			}
		}

		request->Continue(handler);
		return true;
	}

	CefRefPtr<CefStreamReader> JGW_ZipResResourceProvider::GetZipResResourceReader(const std::string& resource_path)
	{
		DWORD dwSize;
		LPBYTE pBytes;
		CefString strResourcePath(resource_path);

		if (GetZipResBinaryResource(strResourcePath.ToWString(), dwSize, pBytes)) {
            CefRefPtr<CefZipArchive::File> ptrFile = gptrCefZipArchive->GetFile(strResourcePath);
            if (NULL != ptrFile.get())
            {
                return ptrFile->GetStreamReader();
            }
			//return CefStreamReader::CreateForHandler(new CefByteReadHandler(pBytes, dwSize, NULL));
		}

		NOTREACHED();  // The resource should be found.
		return NULL;
	}


	bool JGW_ZipResResourceProvider::LoadZipResResource(HINSTANCE hModuleHandle, DWORD zipResourceId, const std::wstring& strResourceType)
	{
		if (NULL == hModuleHandle)
		{
			hModuleHandle = GetModuleHandle(NULL);
		}
		ghModuleHandle = hModuleHandle;

		HRSRC hResource = ::FindResource(hModuleHandle, MAKEINTRESOURCE(zipResourceId), strResourceType.c_str());
		if (NULL != hResource)
		{
			HGLOBAL hGlobal = ::LoadResource(hModuleHandle, hResource);
			if (hGlobal == NULL)
			{
				::FreeResource(hResource);
				return 0L;
			}

			mhZipResourceSize = ::SizeofResource(hModuleHandle, hResource);
			if (mhZipResourceSize == 0)
				return 0L;

			mpZipResourceByte = new BYTE[mhZipResourceSize];
			if (mpZipResourceByte != NULL)
			{
				::CopyMemory(mpZipResourceByte, (LPBYTE)::LockResource(hGlobal), mhZipResourceSize);
			}

			::FreeResource(hResource);

            gptrCefStreamReader = CefStreamReader::CreateForData((void*)mpZipResourceByte,mhZipResourceSize);
            if (NULL != gptrCefStreamReader.get())
            {
                gptrCefZipArchive = new CefZipArchive();
                gptrCefZipArchive->Load(gptrCefStreamReader,gstrPassWord.c_str(),false);
            }
            
			//g_hz = OpenZip(mpZipResourceByte, mhZipResourceSize, 3);
		}

		return NULL != gptrCefStreamReader.get() && NULL != gptrCefZipArchive.get();
	}

	bool JGW_ZipResResourceProvider::GetZipResBinaryResource(const std::wstring& strResourceName, DWORD& dwSize, LPBYTE& pData)
	{
		if (NULL == gptrCefStreamReader.get() || NULL == gptrCefZipArchive.get())
		{
			if (!LoadZipResResource(ghModuleHandle, gdwResourceNameID, gstrResourceType))
				return false;
		}

        if (!gptrCefZipArchive->HasFile(strResourceName.c_str()))
        {
            return false;
        }
		//ZIPENTRY ze;
		//int i;
		//if (FindZipItem(g_hz, strResourceName.c_str(), true, &i, &ze) != 0)
		//	return false;

		//dwSize = ze.unc_size;
		//if (dwSize == 0)
		//	return false;

		//pData = new BYTE[dwSize];
		//int res = UnzipItem(g_hz, i, pData, dwSize, 3);
		//if (res != 0x00000000 && res != 0x00000600) {
		//	delete[] pData;
		//	pData = NULL;
		//	return false;;
		//}
		return true;
	}
}
