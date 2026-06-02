#include "stdafx.h"
#include "JGW_ZipResResourceProvider.h"
#include <include/wrapper/cef_stream_resource_handler.h>
#include <include/wrapper/cef_byte_read_handler.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>

///////////////////////////////////////////////////////////////////////////////////////
DECLARE_HANDLE(HZIP);	// An HZIP identifies a zip file that has been opened
typedef DWORD ZRESULT;
typedef struct
{
	int index;                 // index of this file within the zip
	char name[MAX_PATH];       // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRY;
typedef struct
{
	int index;                 // index of this file within the zip
	TCHAR name[MAX_PATH];      // filename within the zip
	DWORD attr;                // attributes, as in GetFileAttributes.
	FILETIME atime, ctime, mtime;// access, create, modify filetimes
	long comp_size;            // sizes of item, compressed and uncompressed. These
	long unc_size;             // may be -1 if not yet known (e.g. being streamed in)
} ZIPENTRYW;
#define OpenZip OpenZipU
#define CloseZip(hz) CloseZipU(hz)
extern HZIP OpenZipU(void *z, unsigned int len, DWORD flags);
extern ZRESULT CloseZipU(HZIP hz);
#ifdef _UNICODE
#define ZIPENTRY ZIPENTRYW
#define GetZipItem GetZipItemW
#define FindZipItem FindZipItemW
#else
#define GetZipItem GetZipItemA
#define FindZipItem FindZipItemA
#endif
extern ZRESULT GetZipItemA(HZIP hz, int index, ZIPENTRY *ze);
extern ZRESULT GetZipItemW(HZIP hz, int index, ZIPENTRYW *ze);
extern ZRESULT FindZipItemA(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRY *ze);
extern ZRESULT FindZipItemW(HZIP hz, const TCHAR *name, bool ic, int *index, ZIPENTRYW *ze);
extern ZRESULT UnzipItem(HZIP hz, int index, void *dst, unsigned int len, DWORD flags);
///////////////////////////////////////////////////////////////////////////////////////

namespace JGW
{
	std::wstring JGW_ZipResResourceProvider::gstrResourceType = L"ZIPRES";
	HINSTANCE JGW_ZipResResourceProvider::ghModuleHandle = NULL;
	DWORD JGW_ZipResResourceProvider::gdwResourceNameID = 129;
	HZIP g_hz = NULL;
	DWORD mhZipResourceSize = 0;
	LPBYTE mpZipResourceByte = NULL;

	JGW_ZipResResourceProvider::JGW_ZipResResourceProvider()
	{

	}

	JGW_ZipResResourceProvider::~JGW_ZipResResourceProvider()
	{
		if (NULL != g_hz)
		{
			CloseZipU(g_hz);
		}
		if (NULL != mpZipResourceByte)
		{
			mhZipResourceSize = 0;
			delete[] mpZipResourceByte;
			mpZipResourceByte = NULL;
		}	
	}

	bool JGW_ZipResResourceProvider::SetZipResResource(HINSTANCE hModuleHandle, DWORD dwResourceNameID, const std::wstring& strResourceType)
	{
		ghModuleHandle = hModuleHandle;
		gdwResourceNameID = dwResourceNameID;
		gstrResourceType = strResourceType;
		return LoadZipResResource(ghModuleHandle, gdwResourceNameID, gstrResourceType);
	}

    bool JGW_ZipResResourceProvider::SetZipFilePath(const std::wstring& strZipFilePath)
    {
        if (NULL != g_hz)
        {
            CloseZipU(g_hz);
        }
        if (NULL != mpZipResourceByte)
        {
            mhZipResourceSize = 0;
            delete[] mpZipResourceByte;
            mpZipResourceByte = NULL;
        }	

        if (JGW_FileExistsToFilePath(strZipFilePath.c_str()))
        {
            g_hz = OpenZip(mpZipResourceByte, mhZipResourceSize, /*ZIP_FILENAME*/2);
        }
        return NULL != g_hz;
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
			return CefStreamReader::CreateForHandler(
				new CefByteReadHandler(pBytes, dwSize, NULL));
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

			g_hz = OpenZip(mpZipResourceByte, mhZipResourceSize, 3);
		}

		return NULL != g_hz;
	}

	bool JGW_ZipResResourceProvider::GetZipResBinaryResource(const std::wstring& strResourceName, DWORD& dwSize, LPBYTE& pData)
	{
		if (g_hz == NULL)
		{
			if (!LoadZipResResource(ghModuleHandle, gdwResourceNameID, gstrResourceType))
				return false;
		}

		ZIPENTRY ze;
		int i;
		if (FindZipItem(g_hz, strResourceName.c_str(), true, &i, &ze) != 0)
			return false;

		dwSize = ze.unc_size;
		if (dwSize == 0)
			return false;

		pData = new BYTE[dwSize];
		int res = UnzipItem(g_hz, i, pData, dwSize, 3);
		if (res != 0x00000000 && res != 0x00000600) {
			delete[] pData;
			pData = NULL;
			return false;;
		}
		return true;
	}
}
