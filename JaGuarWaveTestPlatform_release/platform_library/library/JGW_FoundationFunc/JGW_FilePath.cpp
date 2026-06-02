// 导出文件操作函数
#include "stdafx.h"

#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <JGW_FoundationFunc\win32impl.h>

#include <tchar.h>
#include <sstream>
#include <Shlwapi.h>
#include <ShlObj.h>
#include <io.h>

#pragma comment(lib,"Shlwapi.lib")

#define FILTERINICONFIGNAME _T( "\\UseFilterPortConfig.ini")

namespace JGW
{
    const wchar_t* JGW_GetApplicationFolder()
    {
        static std::wstring gstrApplicationFolder;
        if (!gstrApplicationFolder.empty()) return gstrApplicationFolder.c_str();

        JGW_GetModuleWorkPath(gstrApplicationFolder);
        return gstrApplicationFolder.c_str();
    }

    const wchar_t* JGW_GetUseFilterPortConfigFilePath()
    {
        static std::wstring gstrUsbFilterPortConfigFilePath;
        if (!gstrUsbFilterPortConfigFilePath.empty()) return gstrUsbFilterPortConfigFilePath.c_str();
        gstrUsbFilterPortConfigFilePath = JGW_GetApplicationFolder();
        gstrUsbFilterPortConfigFilePath += FILTERINICONFIGNAME;
        return gstrUsbFilterPortConfigFilePath.c_str();
    }

    const wchar_t* JGW_GetPluginSubFolder()
    {
        static std::wstring gstrPluginSubFolder;
        if (!gstrPluginSubFolder.empty()) return gstrPluginSubFolder.c_str();
        JGW_GetExecSubFolder(gstrPluginSubFolder,L"plugin");
        return gstrPluginSubFolder.c_str();
    }

    const wchar_t* JGW_GetUIConfigFilePath()
    {
        static std::wstring gstrUiConfigFilePath;
        if (!gstrUiConfigFilePath.empty()) return gstrUiConfigFilePath.c_str();
        JGW_GetExecSubFolder( gstrUiConfigFilePath,L"skin");
        gstrUiConfigFilePath += _T("uiconfig.xml");
        gstrUiConfigFilePath.c_str();
        return gstrUiConfigFilePath.c_str();
    }

    void JGW_GetModuleWorkPath_C(wchar_t* path,size_t size,HMODULE hModule /* = NULL */)
    {
        if ( !path || !size ) return;

        GetModuleFileName(hModule, path, size);
        PathRemoveFileSpec(path);
        PathAddBackslash(path);
    }

    void JGW_GetModuleWorkPathA_C(char* path,size_t size,HMODULE hModule /* = NULL */)
    {
        if ( !path || !size ) return;

        GetModuleFileNameA(hModule, path, size);
        PathRemoveFileSpecA(path);
        PathAddBackslashA(path);
    }

    size_t JGW_GetExecSubFolder_C( wchar_t* path,size_t size,const wchar_t* subfolder,HMODULE hModule/* = NULL*/ )
    {
        if ( !path || !subfolder )
        {
            return -1;
        }
        JGW_GetModuleWorkPath_C(path,size,hModule);
        PathAppend(path,subfolder);
        PathAddBackslash(path);
        return _tcslen(path);
    }

    void JGW_GetModuleWorkPath(std::wstring& path,HMODULE hModule /* = NULL */)
    {
        path.resize(512);
        JGW_GetModuleWorkPath_C(const_cast<wchar_t*>(path.data()),512,hModule);
        path.resize(wcslen(path.c_str()));
    }

    void JGW_GetModuleWorkPathA(std::string& path,HMODULE hModule /* = NULL */)
    {
        path.resize(512);
        JGW_GetModuleWorkPathA_C(const_cast<char*>(path.data()),512,hModule);
        path.resize(strlen(path.c_str()));
    }

    void JGW_GetExecSubFolder(std::wstring& path,const wchar_t* subfolder,HMODULE hModule /* = NULL */)
    {
        //JGW_GetModuleWorkPath(path,hModule);
        path.resize(512);
        JGW_GetModuleWorkPath_C(const_cast<wchar_t*>(path.data()),512,hModule);
        PathAppend(const_cast<wchar_t*>(path.data()),subfolder);
        PathAddBackslash(const_cast<wchar_t*>(path.data()));
        path.resize(wcslen(path.c_str()));
    }

    void JGW_GetCacheExecSubFolder(std::wstring& path,const wchar_t* subfolder,HMODULE hModule /* = NULL */)
    {
        path.resize(512);
        JGW_GetModuleWorkPath_C(const_cast<wchar_t*>(path.data()),512,hModule);
        PathAppend(const_cast<wchar_t*>(path.data()),L"cache\\");
        PathAppend(const_cast<wchar_t*>(path.data()),subfolder);
        PathAddBackslash(const_cast<wchar_t*>(path.data()));
        path.resize(wcslen(path.c_str()));
    }

    void JGW_GetConfigExecSubFolder(std::wstring& path,const wchar_t* subfolder,HMODULE hModule /* = NULL */)
    {
        path.resize(512);
        JGW_GetModuleWorkPath_C(const_cast<wchar_t*>(path.data()),512,hModule);
        PathAppend(const_cast<wchar_t*>(path.data()),L"config\\");
        PathAppend(const_cast<wchar_t*>(path.data()),subfolder);
        PathAddBackslash(const_cast<wchar_t*>(path.data()));
        path.resize(wcslen(path.c_str()));
    }

    inline bool JGW_IsNotNull( const wchar_t* pszText )
    {
        return pszText != NULL && *pszText != 0;
    }

    inline bool JGW_IsNotNullA( const char* pszText )
    {
        return pszText != NULL && *pszText != 0;
    }

    inline bool JGW_IsPathSlash( wchar_t c )
    {
        return L'\\' == c || L'/' == c;
    }

    inline bool JGW_IsPathSlashA( char c )
    {
        return '\\' == c || '/' == c;
    }

    void JGW_ReplaceSlash(wchar_t* path)
    {
        for (; *path; path++)
        {
            if (L'/' == *path)
            {
                *path = L'\\';
            }
        }
    }

    void JGW_ReplaceSlashA( char* path )
    {
        for (; *path; path++)
        {
            if ('/' == *path)
            {
                *path = '\\';
            }
        }
    }

    std::string JGW_GetSystemErrorStringA(unsigned long errorcode)
    {
        return JGW_W2A(JGW_GetSystemErrorString(errorcode));
    }

    std::wstring JGW_GetSystemErrorString( unsigned long errorcode )
    {
        std::wstring retstr;
        wchar_t buf[21] = { 0 };

        if (errorcode != 0)
        {
#ifdef JGW_FormatMessage
            wchar_t* p = NULL;

            ::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL, errorcode,
                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (wchar_t*)&p, 0, NULL);

            if (p != NULL)
            {
                retstr = p;
                ::LocalFree((HLOCAL)p);
            }
#endif

            // define _NEED_STDIO before include PluginInc.h for vc6 or gcc.
            swprintf_s(buf, _countof(buf), L"[0x%x]", errorcode);
            retstr = buf + retstr;
        }

        return retstr;
    }

    bool JGW_CreateDirectoryA( const char* filename, bool bIsPath )
    {
        char path[MAX_PATH];
        size_t i, nLen;
        char cSaveChar;

        if ( !JGW_IsNotNullA(filename) )
        {
            g_strErrorMsg = L"The file path cannot be empty";
            return false;
        }

        strcpy_s(path, MAX_PATH, filename);
        JGW_ReplaceSlashA(path);

        if ( bIsPath )
        {
            PathRemoveBackslashA( path );
        }
        else
        {
            PathRemoveFileSpecA(path);
        }

        PathAddBackslashA(path);
        nLen = strlen(path);

#ifdef _WIN32
        if ( access(path, 0) == 0 )
#else
        if (x3CheckFileAttributes(path, NULL, NULL))
#endif
        {
            return true;
        }

        for (i = 2; i < nLen; i++)
        {
            if (JGW_IsPathSlashA(path[i]) && path[i-1] != L':')
            {
                cSaveChar = path[i];
                path[i] = 0;
                ::CreateDirectoryA(path, NULL);
                ::x3SetFileAttributesNormalA(path);
                path[i] = cSaveChar;
            }
        }

        DWORD dwError = GetLastError();
#ifdef _WIN32
        if ( access(path, 0) != 0 )
#else
        if ( !x3CheckFileAttributes(path, NULL, NULL) )
#endif
        {
            g_strErrorMsg = JGW_GetSystemErrorString(dwError);
            g_strErrorMsg += L", ";
            g_strErrorMsg += JGW_A2W(filename);
            return false;
        }

        return true;
    }

    void JGW_PathAddBackslash(wchar_t* path)
    {
        PathAddBackslash(path);
    }

    void JGW_PathAddBackslashA(char* path)
    {
        PathAddBackslashA(path);
    }

    bool JGW_PathFileExistsA(const char* path)
    {
        return TRUE == PathFileExistsA(path);
    }

    bool JGW_PathFileExists(const wchar_t* path)
    {
        return TRUE == PathFileExistsW(path);
    }

    std::wstring JGW_PathReplaceWindowsPathRemoveBackslash(const std::wstring& strPath)
    {
        wchar_t toRemoveOne = L'\\';
        std::wstring strPathWindows(strPath);
        JGW_ReplaceStringW(strPathWindows,L"/",L"\\");
        JGW_ReplaceStringW(strPathWindows,L"\\\\",L"\\");
        while (!strPathWindows.empty() && toRemoveOne == strPathWindows.at(strPathWindows.length()-1))
        {
            strPathWindows.erase(strPathWindows.end()-1);
        }
        return strPathWindows;
    }

    std::string JGW_PathReplaceWindowsPathRemoveBackslashA(const std::string& strPath)
    {
        char toRemoveOne = L'\\';
        std::string strPathWindows(strPath);
        JGW_ReplaceStringA(strPathWindows,"/","\\");
        JGW_ReplaceStringA(strPathWindows,"\\\\","\\");
        while (!strPathWindows.empty() && toRemoveOne == strPathWindows.at(strPathWindows.length()-1))
        {
            strPathWindows.erase(strPathWindows.end()-1);
        }
        return strPathWindows;
    }

    std::wstring JGW_FolderReplaceWindowsFolderAddBackslash(const std::wstring& strPath)
    {
        return JGW_PathReplaceWindowsPathRemoveBackslash(strPath) + L"\\";
    }

    std::string JGW_FolderReplaceWindowsFolderAddBackslashA(const std::string& strPath)
    {
        return JGW_PathReplaceWindowsPathRemoveBackslashA(strPath) + "\\";
    }


    bool JGW_CreateDirectory( const wchar_t* filename, bool bIsPath )
    {
        wchar_t path[MAX_PATH];
        size_t i, nLen;
        wchar_t cSaveChar;

        if ( !JGW_IsNotNull(filename) )
        {
            g_strErrorMsg = L"The file path cannot be empty";
            ///			Log4WE( g_strErrorMsg.c_str() );
            return false;
        }

        wcscpy_s(path, MAX_PATH, filename);
        JGW_ReplaceSlash(path);
        if ( bIsPath )
        {
            PathRemoveBackslashW( path );
        }
        else
        {
            PathRemoveFileSpecW(path);
        }
        PathAddBackslashW(path);
        nLen = wcslen(path);

#ifdef _WIN32
        if ( _waccess(path, 0) == 0 )
#else
        if (x3CheckFileAttributes(path, NULL, NULL))
#endif
        {
            return true;
        }

        for (i = 2; i < nLen; i++)
        {
            if (JGW_IsPathSlash(path[i]) && path[i-1] != L':')
            {
                cSaveChar = path[i];
                path[i] = 0;
                ::CreateDirectoryW(path, NULL);
                ::x3SetFileAttributesNormal(path);
                path[i] = cSaveChar;
            }
        }

        DWORD dwError = GetLastError();
#ifdef _WIN32
        if ( _waccess(path, 0) != 0 )
#else
        if ( !x3CheckFileAttributes(path, NULL, NULL) )
#endif
        {
            g_strErrorMsg = JGW_GetSystemErrorString(dwError);
            g_strErrorMsg += L", ";
            g_strErrorMsg += filename;
            //	Log4WE( g_strErrorMsg.c_str() );
            return false;
        }

        return true;
    }

    void JGW_GetPathExtensionName(const char *filePath, char ext[], const size_t extLen)
    {
        if( !filePath || !ext ) return;
        size_t len = strlen(filePath), lastSep = 0, m = 0;
        for (lastSep = len - 1; lastSep >= 0; --lastSep)
        {
            if (filePath[lastSep] == '.' || filePath[lastSep] == '\\' || filePath[lastSep] == '/')
            {
                break;
            }
        }
        if (lastSep < 0)
        {
            lastSep = 0;
        }
        if( len-lastSep >= extLen)
        {
            return;
        }
        for (; lastSep < len; ++lastSep)
        {
            ext[m++] = filePath[lastSep];
        }
        ext[m] = '\0';
    }

    void JGW_GetPathExtensionNameW(const wchar_t *filePath, wchar_t ext[], const size_t extLen)
    {
        if( !filePath || !ext ) return;
        size_t len = wcslen(filePath), lastSep = 0, m = 0;
        for (lastSep = len - 1; lastSep >= 0; --lastSep)
        {
            if (filePath[lastSep] == '.' || filePath[lastSep] == '\\' || filePath[lastSep] == '/')
            {
                break;
            }
        }
        if (lastSep < 0)
        {
            lastSep = 0;
        }
        if( len-lastSep >= extLen)
        {
            return;
        }
        for (; lastSep < len; ++lastSep)
        {
            ext[m++] = filePath[lastSep];
        }
        ext[m] = '\0';
    }

    bool JGW_TraverseFiles_C(std::string& path,int& numFile,const char* character ,void (*scanfile)(const char* filepath,const char* filename,void* lpData) ,void* lpData)
    {
        _finddata_t file_info;
        std::string current_file = path;
        current_file += (NULL != character)?character:"/*.*";

        int handle=_findfirst(current_file.c_str(),&file_info);
        if( -1 == handle )
            return false;
        do
        {
            //! 判断是否子目录
            if( !(_A_SUBDIR & file_info.attrib) )
            {
                if( NULL != scanfile )
                {
                    scanfile(path.c_str(),file_info.name,lpData);
                }
                numFile ++;
            }

        }while(!_findnext(handle,&file_info));  //返回0则遍历完
        //关闭文件句柄
        _findclose(handle);

        return true;
    }

    bool JGW_TraverseFolder_C(std::string& path,int& numFile,const char* character ,void (*scanfile)(const char* filepath,const char* filename,void* lpData) ,void* lpData)
    {
        _finddata_t file_info;
        std::string current_file = path;
        current_file += (NULL != character)?character:"\\*.*";

        int handle=_findfirst(current_file.c_str(),&file_info);
        if( -1 == handle )
            return false;
        do
        {
            if( !(_A_SUBDIR & file_info.attrib) )
            {
                if( NULL != scanfile )
                {
                    scanfile(path.c_str(),file_info.name,lpData);
                }
                numFile ++;
            }
            else
            {
                if( strcmp(file_info.name,"..") != 0 && strcmp(file_info.name,".") != 0 )  
                {
                    JGW_TraverseFolder_C(path + "\\" + file_info.name,numFile,character,scanfile,lpData);
                }
            }
        }while(!_findnext(handle,&file_info));  
        _findclose(handle);

        return true;
    }

    bool JGW_PathIsRelativeA( const char* filePath )
    {
        return (TRUE == PathIsRelativeA(filePath));
    }

    bool JGW_PathIsRelativeW( const wchar_t* filePath )
    {
        return (TRUE == PathIsRelativeW(filePath));
    }

    bool JGW_TraverseFolder_VC(std::wstring& path,int& nFileCount,
        const wchar_t* character,
        void (*scanfile)(const wchar_t* filepath,const wchar_t* filename,
        void* lpData),
        void* lpData) 
    {
#if 0
        WIN32_FIND_DATA	findFileData;
        wchar_t szFind[MAX_PATH] = {_T("\0")};

        _tcscpy_s(szFind,MAX_PATH,path.c_str() );
        PathAddBackslashW(szFind);
        _tcscat_s( szFind, character );     //这里一定要指明通配符，不然不会读取所有文件和目录

        HANDLE hFindFile = FindFirstFile( szFind,&findFileData );
        if ( INVALID_HANDLE_VALUE == hFindFile )
        {
            return false;
        }

        while(true)
        {
            if ( findFileData.cFileName[0] != _T('.') )
            {

                //这是一个普通目录
                if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                {	
                    //遍历该目录
                    JGW_TraverseFolder_VC ( path + L"\\" + findFileData.cFileName,numFile,character,scanfile ,lpData);
                }
                else
                {
                    // 如果是文件，则计算之
                    if( NULL != scanfile ) scanfile(path.c_str(),findFileData.cFileName,lpData); 

                    numFile++;
                }
            }		
            //如果是当前路径或者父目录的快捷方式，或者是普通目录，则寻找下一个目录或者文件
            bRet = ::FindNextFile(hFindFile, &findFileData);
            //函数调用失败
            if (!bRet)
            {
                break;
            }
        }
        ::FindClose(hFindFile);

        return true;
#else
        std::wstring strFindFolder(path);
        strFindFolder += L"\\";
        strFindFolder += character;
        //! 文件数目归零计算
        nFileCount = 0;
        //! 开始进行查找
        WIN32_FIND_DATA	findFileData = {0};
        HANDLE hFindFile = FindFirstFile(strFindFolder.c_str(),&findFileData );
        if (INVALID_HANDLE_VALUE == hFindFile)
        {
            return false;
        }
        do
        {
            if (findFileData.cFileName[0] != L'.')
            {
                //这是一个普通目录
                if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                {	
                    //遍历该目录
                    JGW_TraverseFolder_VC ( path + L"\\" + findFileData.cFileName,nFileCount,character,scanfile,lpData);
                }
                else
                {
                    if( NULL != scanfile ) scanfile(path.c_str(),findFileData.cFileName,lpData);
                    nFileCount++;
                }
            }
        } while (TRUE == ::FindNextFile(hFindFile, &findFileData));

        ::FindClose(hFindFile);
        return true;
#endif
    }

    bool JGW_TraverseFile_VC(std::wstring& path,int& numFile, const wchar_t* character, void (*scanfile)(const wchar_t* filepath,const wchar_t* filename,void* lpData), void* lpData)
    {
        WIN32_FIND_DATA		findFileData;
        TCHAR				szFind[MAX_PATH] = {_T("\0")};
        BOOL				bRet = FALSE;
        _tcscpy_s( szFind, MAX_PATH, path.c_str() );
        PathAddBackslashW(szFind);
        _tcscat_s( szFind, character );     //这里一定要指明通配符，不然不会读取所有文件和目录

        HANDLE hFindFile = FindFirstFile(szFind,&findFileData);
        if (INVALID_HANDLE_VALUE == hFindFile)
        {
            return false;
        }

        while(true)
        {
            if ( findFileData.cFileName[0] != _T('.') )
            {
                //设置下一个将要扫描的文件夹路径
                //_tcscpy_s(szFind, MAX_PATH, strfolderPath.c_str());    
                //_tcscat_s(szFind, _T("\\"));    
                //_tcscat_s(szFind, findFileData.cFileName);
                //_tprintf(_T("%s\\%s\n"), strfolderPath.c_str(), findFileData.cFileName);
                //不是当前路径或者父目录的快捷方式
                //这是一个普通目录
                if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                {	
                    //遍历该目录
                    //JGW_TraverseFolder_VC ( path + L"\\" + findFileData.cFileName,numFile,character,scanfile ,lpData);
                }
                else
                {
                    // 如果是文件，则计算之
                    //QueryFilterStrToFile(strFilter,std::wstring(szFind));
                    if( NULL != scanfile )
                    {
                        scanfile(path.c_str(),findFileData.cFileName,lpData);
                    }
                }
            }		
            //如果是当前路径或者父目录的快捷方式，或者是普通目录，则寻找下一个目录或者文件
            bRet = ::FindNextFile(hFindFile, &findFileData);
            //函数调用失败
            if (!bRet)
            {
                break;
            }
        }
        ::FindClose(hFindFile);

        return true;
    }


    bool JGW_ChooseFile(std::wstring& strFile,const wchar_t* strFilter,HWND hwnd /* = NULL */,DWORD Flags /* = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST */,const wchar_t* lpstrInitialDir /* = NULL */)
    {
        OPENFILENAME fopt = {sizeof(OPENFILENAME)};
        wchar_t fileName[MAX_PATH]={0};

        fopt.hwndOwner = hwnd;
        fopt.lpstrFilter = strFilter;
        fopt.lpstrFile = fileName;
        fopt.nMaxFile = MAX_PATH;
        fopt.Flags = Flags;
        if (NULL != lpstrInitialDir) fopt.lpstrInitialDir = lpstrInitialDir;
        int nResult = ::GetOpenFileName(&fopt) > 0?true:false;
        strFile = fileName;

        return nResult > 0;
    }

    bool JGW_ChooseMultiSelectFileNames(std::vector<std::wstring> &_vMultiFiles,const wchar_t* strFilter,HWND hwnd /* = NULL */,DWORD Flags /* = OFN_EXPLORER | OFN_ALLOWMULTISELECT  */,const wchar_t* lpstrInitialDir /* = NULL */ )
    {
        OPENFILENAME fopt = {sizeof(OPENFILENAME)};
        wchar_t szPath[ MAX_PATH ] = {0};
        TCHAR szOpenFileNames[MAX_PATH*MAX_PATH] = {0};
        TCHAR szFileName[MAX_PATH] = {0};

        //if( lpstrInitialDir )_tcscpy_s(szOpenFileNames,MAX_PATH*MAX_PATH,lpstrInitialDir);
        fopt.hwndOwner = hwnd;
        fopt.lpstrFilter = strFilter;
        fopt.lpstrFile = szOpenFileNames/*fileName*/;
        fopt.nMaxFile = sizeof(szOpenFileNames);
        fopt.Flags = Flags;
        if (NULL != lpstrInitialDir) fopt.lpstrInitialDir = lpstrInitialDir;
        bool nResult = ::GetOpenFileName(&fopt) > 0?true:false;

        //
        lstrcpyn( szPath, szOpenFileNames, fopt.nFileOffset );
        const wchar_t* p = szOpenFileNames + fopt.nFileOffset;
        //         szPath[ fopt.nFileOffset ] = '/0';
        // 
        //         size_t nLen = lstrlen(szPath);
        //         if( szPath[nLen-1] != '\\' )   //如果选了多个文件,则必须加上'//'
        //         {
        //             lstrcat(szPath, TEXT("\\"));
        //         }

        while( *p )
        {
            memset(szFileName,0x00,MAX_PATH);
            lstrcat(szFileName, p);    //加上文件名
            _vMultiFiles.push_back(szFileName);
            p += lstrlen(p) +1;     //移至下一个文件
        }

        return nResult;
    }

    bool JGW_ChooseMultiSelectFileNamesFolder(std::wstring& folder,std::vector<std::wstring> &_vMultiFiles,const wchar_t* strFilter,HWND hwnd /*= NULL*/,DWORD Flags /*= OFN_EXPLORER | OFN_ALLOWMULTISELECT */,const wchar_t* lpstrInitialDir/* = NULL*/)
    {
        OPENFILENAME fopt = {sizeof(OPENFILENAME)};
        wchar_t szPath[ MAX_PATH ] = {0};
        TCHAR szOpenFileNames[MAX_PATH*MAX_PATH] = {0};
        TCHAR szFileName[MAX_PATH] = {0};

        //if( lpstrInitialDir )_tcscpy_s(szOpenFileNames,MAX_PATH*MAX_PATH,lpstrInitialDir);
        fopt.hwndOwner = hwnd;
        fopt.lpstrFilter = strFilter;
        fopt.lpstrFile = szOpenFileNames/*fileName*/;
        fopt.nMaxFile = sizeof(szOpenFileNames);
        fopt.Flags = Flags;
        if (NULL != lpstrInitialDir) fopt.lpstrInitialDir = lpstrInitialDir;
        bool nResult = ::GetOpenFileName(&fopt) > 0?true:false;

        //
        lstrcpyn( szPath, szOpenFileNames, fopt.nFileOffset );
        const wchar_t* p = szOpenFileNames + fopt.nFileOffset;
        szPath[fopt.nFileOffset] = '/0';

        size_t nLen = lstrlen(szPath);
        if( szPath[nLen-1] != '\\' )   //如果选了多个文件,则必须加上'//'
        {
            lstrcat(szPath, TEXT("\\"));
        }
        folder = szPath;

        while( *p )
        {
            memset(szFileName,0x00,MAX_PATH);
            lstrcat(szFileName, p);    //加上文件名
            _vMultiFiles.push_back(szFileName);
            p += lstrlen(p) +1;     //移至下一个文件
        }

        return nResult;
    }

    bool JGW_ChooseMultiSeleJGWile(std::vector<std::wstring>& _vMultiFiles,const wchar_t* strFilter,HWND hwnd /* = NULL */,DWORD Flags /* = OFN_EXPLORER | OFN_ALLOWMULTISELECT */,const wchar_t* lpstrInitialDir/* = NULL */)
    {
        OPENFILENAME fopt = {sizeof(OPENFILENAME)};
        wchar_t szPath[ MAX_PATH ] = {0};
        TCHAR szOpenFileNames[MAX_PATH*MAX_PATH] = {0};
        TCHAR szFileName[MAX_PATH] = {0};

        //if( lpstrInitialDir )_tcscpy_s(szOpenFileNames,MAX_PATH*MAX_PATH,lpstrInitialDir);
        fopt.hwndOwner = hwnd;
        fopt.lpstrFilter = strFilter;
        fopt.lpstrFile = szOpenFileNames/*fileName*/;
        fopt.nMaxFile = sizeof(szOpenFileNames);
        fopt.Flags = Flags;
        if (NULL != lpstrInitialDir) fopt.lpstrInitialDir = lpstrInitialDir;
        int nResult = ::GetOpenFileName(&fopt) > 0?true:false;
        //
        lstrcpyn( szPath, szOpenFileNames, fopt.nFileOffset );
        const wchar_t* p = szOpenFileNames + fopt.nFileOffset;
        szPath[ fopt.nFileOffset ] = '/0';

        size_t nLen = lstrlen(szPath);
        if( szPath[nLen-1] != '\\' )   //如果选了多个文件,则必须加上'//'
        {
            lstrcat(szPath, TEXT("\\"));
        }

        while( *p )
        {
            memset(szFileName,0x00,MAX_PATH);
            lstrcat(szFileName, szPath);  //给文件名加上路径
            lstrcat(szFileName, p);    //加上文件名
            _vMultiFiles.push_back(szFileName);
            p += lstrlen(p) +1;     //移至下一个文件
        }

        return nResult > 0;
    }

    bool JGW_SaveFileName(std::wstring& strFile,const wchar_t* strFilter,HWND hwnd/* = NULL*/,const wchar_t* strext/* = L"txt"*/,DWORD Flags/* = OFN_EXPLORER|OFN_ENABLEHOOK|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST*/ )
    {
        OPENFILENAME fopt = {sizeof(OPENFILENAME)};
        wchar_t fileName[MAX_PATH * 2]={0};
        wcscpy_s(fileName,512,strFile.c_str());

        fopt.hwndOwner = hwnd;
        fopt.lpstrFilter = strFilter;
        fopt.lpstrFile = fileName;
        fopt.nMaxFile = MAX_PATH;
        fopt.Flags = Flags;
        fopt.lpstrDefExt = strext;
        int nResult = ::GetSaveFileName(&fopt) > 0?true:false;
        strFile = fileName;

        return nResult > 0;
    }

    static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM , LPARAM lpData)   
    {
        if(uMsg == BFFM_INITIALIZED)
        {
            HTREEITEM	hItemSel;
            ::SendMessage(hWnd, BFFM_SETSELECTION, TRUE, lpData);
            hItemSel = (HTREEITEM)::SendMessage(hWnd, TVM_GETNEXTITEM, TVGN_CARET, 0);
            ::SendMessage(hWnd, TVM_EXPAND, TVE_COLLAPSE, (LPARAM)hItemSel);
        } 
        return 0;  
    }

    bool JGW_ChooseFolderSetFolderPath(std::wstring& strFile,const wchar_t* lpstrInitialDir,HWND hwnd /* = NULL */,int iImage /* = 0 */)
    {
        wchar_t fileName[MAX_PATH * 2]={0};
        wcscpy_s(fileName,512,strFile.c_str());
        BROWSEINFO bi = {0};
        LPITEMIDLIST pIDList = NULL;

        bi.hwndOwner = hwnd;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = const_cast<wchar_t*>(strFile.data());//此参数如为NULL则不能显示对话框
        bi.lpszTitle = _T("选择文件夹");
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        bi.lpfn = BrowseCallbackProc;
        bi.iImage = iImage;
        bi.lParam = (LPARAM)lpstrInitialDir;

        pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
        if( !pIDList )
        {
            strFile = L"";
            return false;
        }
        SHGetPathFromIDList(pIDList, fileName);
        strFile = fileName;
        return true;
    }



    bool JGW_ChooseFolder(std::wstring& strFile,HWND hwnd,int iImage)
    {
        wchar_t fileName[MAX_PATH * 2]={0};
        wcscpy_s(fileName,512,strFile.c_str());
        BROWSEINFO bi = {0};
        LPITEMIDLIST pIDList = NULL;

        bi.hwndOwner = hwnd;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = const_cast<wchar_t*>(fileName);//此参数如为NULL则不能显示对话框
        bi.lpszTitle = _T("Select Folder");
        bi.ulFlags = BIF_RETURNONLYFSDIRS;
        bi.lpfn = NULL;
        bi.iImage = iImage;
        bi.lParam = 0;

        pIDList = SHBrowseForFolder(&bi);//调用显示选择对话框
        if( !pIDList )
        {
            strFile = L"";
            return false;
        }
        SHGetPathFromIDList(pIDList, fileName);
        strFile = fileName;
        return true;
    }

    std::string JGW_GetFileNameToFilePathA( const char* filename )
    {
        if (NULL == filename || 0 == filename[0])
            return "";

        char szTemp[MAX_PATH] = {0};
        strcpy_s(szTemp, MAX_PATH, PathFindFileNameA(filename));
        PathRemoveBackslashA(szTemp);
        szTemp[strlen(szTemp)] = '\0';

        return szTemp;
    }

    std::wstring JGW_GetFileNameToFilePath( const wchar_t* filename )
    {
        if (NULL == filename || 0 == filename[0])
            return L"";

        wchar_t szTemp[MAX_PATH];
        wcscpy_s(szTemp, MAX_PATH, PathFindFileNameW(filename));
        PathRemoveBackslashW(szTemp);
        szTemp[wcslen(szTemp)] = L'\0';

        return szTemp;
    }

    std::wstring JGW_GetPathOfFile(const wchar_t* filename)
    {
        wchar_t szPath[MAX_PATH] = { 0 };

        if (filename != NULL)
        {
            wcscpy_s(szPath, MAX_PATH, filename);
            JGW_ReplaceSlash(szPath);
            PathRemoveBackslashW(szPath);
            PathRemoveFileSpecW(szPath);
            if (szPath[0] != 0)
            {
                PathAddBackslashW(szPath);
            }
        }

        return szPath;
    }

    std::wstring JGW_GetFileFolderToFilePath( const wchar_t* szFilePath )
    {
        wchar_t szPath[512] = {0};
        int len = wcslen(szFilePath);
        if (szFilePath && len > 4 && len < 512)
        {
            wcscpy_s(szPath,512,szFilePath);
            JGW_ReplaceSlash(szPath);
            PathRemoveBackslash(szPath);
            PathRemoveFileSpec(szPath);
            PathAddBackslash(szPath);
        }
        return szPath;
    }

    std::string JGW_GetFileFolderToFilePathA( const char* szFilePath )
    {
        std::wstring strFolder,strFilePath(JGW_A2W(szFilePath));
        strFolder = JGW_GetFileFolderToFilePath(strFilePath.c_str());
        return JGW_W2A(strFolder);
    }

    void JGW_CloseFilePtr_C(FILE* fp)
    {
        if (fp)
        {
            fclose(fp);
        }
    }
    FILE* JGW_GetFilePtrToPath_C(const char* szFilePath,const char * mode = "rb");

    FILE* JGW_GetFilePtrToPath_C(const char* szFilePath,const char * mode/* = "rb"*/)
    {
        return fopen(szFilePath,mode);
    }

    bool JGW_WriteFileToBufW(const wchar_t* szFilePath,const char* buf,size_t buf_size,const wchar_t * mode /*= L"wb"*/)
    {
        std::shared_ptr<FILE> fptr(_wfopen(szFilePath,mode),JGW_CloseFilePtr_C);
        if (!fptr) return false;
        return (buf_size != fwrite(buf,buf_size,1,fptr.get()));
    }

    bool JGW_WriteFileToBuf(const char* szFilePath,const char* buf,size_t buf_size,const char * mode /* = "wb" */)
    {
        std::shared_ptr<FILE> fptr(JGW_GetFilePtrToPath_C(szFilePath,mode),JGW_CloseFilePtr_C);
        if (!fptr) return false;
        return (buf_size != fwrite(buf,buf_size,1,fptr.get()));
    }

    size_t JGW_GetFileSize_C(const char* szFilePath,const char * mode /* = "rb" */)
    {
        std::shared_ptr<FILE> fptr(JGW_GetFilePtrToPath_C(szFilePath,mode),JGW_CloseFilePtr_C);
        return JGW_GetFilePtrFileSize_C(fptr.get());
    }

    size_t JGW_GetFilePtrFileSize_C(FILE* fp)
    {
        if (!fp) return 0;
        size_t file_size = 0;

        fseek (fp, 0, SEEK_END);  // seek to end of file
        file_size = ftell (fp); // get current file pointer
        fseek (fp, 0, SEEK_SET);  // seek back to beginning of file
        return (size_t) file_size;
    }

    char* JGW_GetFileBufToPath_C(const char* szFilePath,size_t& file_size,const char * mode /* = "rb" */)
    {
        file_size = 0;
        std::shared_ptr<FILE> fptr(JGW_GetFilePtrToPath_C(szFilePath),JGW_CloseFilePtr_C);
        //FILE* fp = JGW_GetFilePtrToPath_C(szFilePath);
        if (!fptr) return NULL;

        file_size = JGW_GetFilePtrFileSize_C(fptr.get());

        char* file_buf = (char*)calloc(file_size + 1, 1);
        if (!file_buf) return NULL;
        if (1 != fread (file_buf, file_size, 1,fptr.get()))
        {
            free(file_buf);
            return NULL;
        }

        return file_buf;
    }

    bool JGW_GetFileBufToPath_BufLen_C(const char* szFilePath,char* file_buf,size_t& buf_size,const char * mode /* = "rb" */)
    {
        std::shared_ptr<FILE> fptr(JGW_GetFilePtrToPath_C(szFilePath),JGW_CloseFilePtr_C);
        //! FILE* fp = JGW_GetFilePtrToPath_C(szFilePath);
        if (!fptr) return false;

        size_t file_size_tmp = JGW_GetFilePtrFileSize_C(fptr.get());
        //! 如果传递过来缓冲区大小大于文件大小 那么读取整个文件
        if (buf_size >= file_size_tmp)
        {
            buf_size = file_size_tmp;
            return (1 == fread (file_buf, file_size_tmp, 1,fptr.get()));
        }

        return (1 == fread (file_buf, buf_size, 1,fptr.get()));
    }

    size_t JGW_GetFileBufToFilePtr_C(FILE* fp,char* file_buf,size_t read_len)
    {
        if (!fp)  return 0;
        return fread(file_buf,sizeof(char),read_len,fp);
    }

    bool JGW_FileExistsToFilePath(const wchar_t* strFilePath)
    {
        return (INVALID_FILE_ATTRIBUTES != GetFileAttributes(strFilePath));
    }

    bool JGW_FileExistsToFilePathA(const char* strFilePath)
    {
        return (INVALID_FILE_ATTRIBUTES != GetFileAttributesA(strFilePath));
    }


    std::string JGW_RealativePathToAbsPathA(const std::string& strPath,const std::string& strBaseFolder /*= ""*/)
    {
        std::string strAbsPath(strPath);
        //! 
        if (PathIsRelativeA(strPath.c_str()))
        {
            if (strBaseFolder.empty())
                strAbsPath = JGW_W2A(JGW_GetApplicationFolder());
            else
                strAbsPath = strBaseFolder;
            if ('\\' != strAbsPath[strAbsPath.length() - 1]) strAbsPath += '\\';
            strAbsPath += strPath;
        }
        return strAbsPath;
    }

    std::wstring JGW_RealativePathToAbsPath(const wchar_t* strPath)
    {
        std::wstring strAbsPath(strPath);
        //! 
        if (PathIsRelative(strPath))
        {
            strAbsPath = JGW_GetApplicationFolder();
            strAbsPath += strPath;
        }
        return strAbsPath;
    }

    std::wstring JGW_RealPathToAbsPathBasePath(const wchar_t* strPath,const wchar_t* strBaseFolder)
    {
        std::wstring strAbsPath(strPath);
        if (PathIsRelative(strPath))
        {
            if (NULL == strBaseFolder)
            {
                strAbsPath = JGW_GetApplicationFolder();
                strAbsPath += strPath;
            }
            else
            {
                strAbsPath = strBaseFolder;
                if ('\\' != strBaseFolder[wcslen(strBaseFolder) - 1] && '/' != strBaseFolder[wcslen(strBaseFolder) - 1])
                {
                    strAbsPath += L"\\";
                }
                strAbsPath += strPath;
            } 
        }
        return strAbsPath;
    }

    void ScanFileNameFunc(const char* filepath,const char* filename,void* lpData)
    {
        std::vector<std::string>* vFileName = (std::vector<std::string>*)lpData;
        if (NULL != vFileName)
        {
            vFileName->push_back(filename);
        }
    }

    void ScanFileFunc(const wchar_t* filepath,const wchar_t* filename,void* lpData)
    {
        std::vector<std::wstring>* vFilePath = (std::vector<std::wstring>*)lpData;
        if (NULL != vFilePath)
        {
            std::wstring strFilePath(filepath);
            strFilePath += L"\\";
            strFilePath += filename;
            vFilePath->push_back(strFilePath);
        }
    }

    std::wstring JGW_GetTSEConfigFilePath(const std::wstring& strModuleFolderName,const std::wstring& strSuiteFileName)
    {
        std::wstring strTSEConfigFilePath,strTSEConfigFolder;

        JGW_GetCacheExecSubFolder(strTSEConfigFilePath,strModuleFolderName.c_str());
        strTSEConfigFolder = strTSEConfigFilePath;
        strTSEConfigFilePath += strSuiteFileName;
        if (JGW_FileExistsToFilePath(strTSEConfigFilePath.c_str()))
        {
            return strTSEConfigFilePath;
        }
        else
        {
            int numFile;
            std::vector<std::string> vFileName;
            JGW_TraverseFiles_C(JGW_W2A(strTSEConfigFolder),numFile,"\\*.xml",ScanFileNameFunc,&vFileName);
            ///JGW_TraverseFolder_VC(strTSEConfigFolder,numFile,L"*.xml",ScanFileFunc,&vFilePath);
            if (1 == vFileName.size())
            {
                strTSEConfigFolder += L"\\";
                strTSEConfigFolder += JGW_A2W(vFileName[0]);
                return strTSEConfigFolder;
            }
            else if (1 < vFileName.size())
            {
                for (size_t i = 0;i < vFileName.size();i ++)
                {
                    if (NULL != JGW_StrCaseStr(vFileName[i].c_str(),"suite"))
                    {
                        strTSEConfigFolder += L"\\";
                        strTSEConfigFolder += JGW_A2W(vFileName[i]);
                        return strTSEConfigFolder;
                    }
                }
            }
        }
        return strTSEConfigFilePath;
    }

    std::wstring JGW_GetCacheConfigFilePath(const std::wstring& strModuleFolderName,const std::wstring& strConfigFileName)
    {
        std::wstring strCacheConfigFilePath;
        JGW_GetCacheExecSubFolder(strCacheConfigFilePath,strModuleFolderName.c_str());
        strCacheConfigFilePath += strConfigFileName;
        return strCacheConfigFilePath;
    }

    std::wstring JGW_GetCacheConfigFolder(const std::wstring& strModuleFolderName)
    {
        std::wstring strCacheConfigFilePath;
        JGW_GetCacheExecSubFolder(strCacheConfigFilePath,strModuleFolderName.c_str());
        return strCacheConfigFilePath;
    }

    std::wstring JGW_GetConfigConfigFilePath(const std::wstring& strModuleFolderName,const std::wstring& strConfigFileName)
    {
        std::wstring strCacheConfigFilePath;
        JGW_GetConfigExecSubFolder(strCacheConfigFilePath,strModuleFolderName.c_str());
        strCacheConfigFilePath += strConfigFileName;
        return strCacheConfigFilePath;
    }

    std::wstring JGW_GetConfigConfigFolder(const std::wstring& strModuleFolderName)
    {
        std::wstring strCacheConfigFilePath;
        JGW_GetConfigExecSubFolder(strCacheConfigFilePath,strModuleFolderName.c_str());
        return strCacheConfigFilePath;
    }

    //std::wstring JGW_GetConfigFilePathToCacheFilePath(const std::wstring& strCacheConfigFilePath)
    //{
    //    std::wstring strTempFilePath(strCacheConfigFilePath);
    //    if (std::wstring::npos != strCacheConfigFilePath.find('/'))
    //    {
    //        JGW_ReplaceStringW(strTempFilePath,L"/",L"\\");
    //    }
    //}

    std::wstring JGW_GetExecConfigFilePath(const std::wstring& strConfigFileName)
    {
        std::wstring strCacheConfigFilePath (JGW_GetApplicationFolder());
        strCacheConfigFilePath += strConfigFileName;
        return strCacheConfigFilePath;
    }

    std::wstring JGW_GetSkinUIConfigFilePath(const wchar_t* strDefaultFileName/* = L"skin\\uiconfig.xml"*/)
    {
        std::wstring strUIConfigXmlPath (JGW_GetApplicationFolder());
        strUIConfigXmlPath += strDefaultFileName;
        return strUIConfigXmlPath;
    }

    bool JGW_DeleteFile(const std::wstring& strFilePath)
    {
        if (!JGW_FileExistsToFilePath(strFilePath.c_str())) return true;
        return (TRUE == DeleteFile(strFilePath.c_str()));
    }

    bool JGW_DeleteDirectoryW(const std::wstring& path)
    {
        WIN32_FIND_DATA		findFileData;
        TCHAR				szFind[MAX_PATH] = {_T("\0")};
        BOOL				bRet = FALSE;
        _tcscpy_s( szFind, MAX_PATH, path.c_str() );
        PathAddBackslashW(szFind);
        _tcscat_s( szFind,L"\\*.*");     //这里一定要指明通配符，不然不会读取所有文件和目录

        HANDLE hFindFile = FindFirstFile( szFind,&findFileData );
        if ( INVALID_HANDLE_VALUE == hFindFile )
        {
            return false;
        }

        while(true)
        {
            if (findFileData.cFileName[0] != _T('.'))
            {
                //设置下一个将要扫描的文件夹路径
                //_tcscpy_s(szFind, MAX_PATH, strfolderPath.c_str());    
                //_tcscat_s(szFind, _T("\\"));    
                //_tcscat_s(szFind, findFileData.cFileName);
                //_tprintf(_T("%s\\%s\n"), strfolderPath.c_str(), findFileData.cFileName);
                //不是当前路径或者父目录的快捷方式
                //这是一个普通目录
                if ( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
                {	
                    //遍历该目录
                    JGW_DeleteDirectoryW ( path + L"\\" + findFileData.cFileName);
                }
                else
                {
                    // 如果是文件，则计算之
                    //QueryFilterStrToFile(strFilter,std::wstring(szFind));
                    std::wstring strTmep (path);
                    strTmep += L"/";
                    strTmep += findFileData.cFileName;
                    JGW_DeleteFile(strTmep);
                }
            }		
            //如果是当前路径或者父目录的快捷方式，或者是普通目录，则寻找下一个目录或者文件
            bRet = ::FindNextFile(hFindFile, &findFileData);
            //函数调用失败
            if (!bRet)
            {
                break;
            }
        }
        ::FindClose(hFindFile);
        if (0 == RemoveDirectory(path.c_str()))
        {
            return false;
        }
        return true;
    }

    bool JGW_IsFolderExist(const std::wstring& strFolderPath)
    {
        bool bFolderExist = false;

        if (strFolderPath.size() == 0) return true;

        DWORD folder_atr = GetFileAttributes(strFolderPath.c_str());

        bFolderExist = (folder_atr != 0xffffffff && (folder_atr & FILE_ATTRIBUTE_DIRECTORY));

        return bFolderExist;
    }

    bool JGW_IsFolderExistA(const std::string& strFolderPath)
    {
        bool bFolderExist = false;

        if (strFolderPath.size() == 0) return true;

        DWORD folder_atr = GetFileAttributesA(strFolderPath.c_str());

        bFolderExist = (folder_atr != 0xffffffff && (folder_atr & FILE_ATTRIBUTE_DIRECTORY));

        return bFolderExist;
    }

    __int64 JGW_GetFileLastAccessTimeTimeStampSec(const std::wstring& strFilePath)
    {
        HANDLE hDestHandle = CreateFileW(strFilePath.c_str(),GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_EXISTING,NULL,NULL);
        if (hDestHandle == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        FILETIME destcreationTime,destlastWriteTime;
        __int64 destlastAccessTime ;
        if ( FALSE == GetFileTime(hDestHandle,&destcreationTime,(FILETIME*)&destlastAccessTime,&destlastWriteTime))
        {
            CloseHandle(hDestHandle);
            return 0;
        }
        CloseHandle(hDestHandle);
        // 需要从100纳秒转到1秒，所以要除以10^7
        //__int64 temp = ((__int64)destlastAccessTime - 116444736000000000LL) / 10000000;
        return ((__int64)destlastAccessTime - 116444736000000000LL) / 10000000;
    }

    __int64 JGW_GetFileLastWriteTimeTimeStampSec(const std::wstring& strFilePath)
    {
        HANDLE hDestHandle = CreateFileW(strFilePath.c_str(),GENERIC_READ | GENERIC_WRITE,NULL,NULL,OPEN_EXISTING,NULL,NULL);
        if (hDestHandle == INVALID_HANDLE_VALUE)
        {
            return 0;
        }

        FILETIME destcreationTime,destlastAccessTime;
        __int64 destlastWriteTime;
        if ( FALSE == GetFileTime(hDestHandle,&destcreationTime,&destlastAccessTime,(FILETIME*)&destlastWriteTime))
        {
            CloseHandle(hDestHandle);
            return 0;
        }
        CloseHandle(hDestHandle);
        // 需要从100纳秒转到1秒，所以要除以10^7
        //__int64 temp = ((__int64)destlastWriteTime - 116444736000000000LL) / 10000000;
        return ((__int64)destlastWriteTime - 116444736000000000LL) / 10000000;
    }











    // 	const wstring& GetExecApplicationPath()
    // 	{
    // 		static wstring strExecutePath = _T("");
    // 		if( strExecutePath.empty() )
    // 		{
    // 			wchar_t path[512] = {0};
    // 			GetModuleFileName(NULL, path, 512);
    // 			PathRemoveFileSpec(path);
    // 			PathAddBackslash(path);
    // 			strExecutePath = path;
    // 		}
    // 		return strExecutePath;
    // 	}
    // 	const wchar_t* GetExecWorkPath()
    // 	{
    // 		static wchar_t path[512] = {0};
    // 		if( '\0' == path[0] )
    // 		{
    // 			GetModuleFileName(NULL, path, 512);
    // 			PathRemoveFileSpec(path);
    // 			PathAddBackslash(path);
    // 		}
    // 		return path;
    // 	}

    // 	const wchar_t* _GetExecSubFolder(const wchar_t* subfolder)
    // 	{
    // 		wchar_t* path = new wchar_t[512];
    // 
    // 		memset(path,0x00,512*sizeof(wchar_t));
    // 		_tcscat_s(path,512,GetExecWorkPath());
    // 		PathAppend(path,subfolder);
    // 		PathAddBackslash(path);
    // 
    // 		return path;
    // 	}

    // 	void FreeFilePathMem(const wchar_t* buf)
    // 	{
    // 		if(buf)
    // 		{
    // 			delete[] buf;
    // 			buf = NULL;
    // 		}
    // 	}

}