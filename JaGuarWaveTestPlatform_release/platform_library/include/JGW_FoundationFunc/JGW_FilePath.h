// 导出文件操作函数

#ifndef JGW_FOUNDATIONFUNCLIB_FILEPATH_H__
#define JGW_FOUNDATIONFUNCLIB_FILEPATH_H__

#include <Windows.h>
#include <vector>
#include <string>
#include <CommDlg.h>
#include "JGW_FoundationFuncLib_Define.h"

namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    extern "C" 
    {
#endif
        //! 获取应用程序当前目录C:\JGWStart\JaGuarWaveTestPlatform_V20210506\exec\    
        JGW_FOUNDATIONFUNC_LIB_API const wchar_t* JGW_GetApplicationFolder();
        //! 获取UseFilterPortConfig.ini文件路径
        JGW_FOUNDATIONFUNC_LIB_API const wchar_t* JGW_GetUseFilterPortConfigFilePath();
        //! 
        JGW_FOUNDATIONFUNC_LIB_API const wchar_t* JGW_GetUIConfigFilePath();
        //!
        JGW_FOUNDATIONFUNC_LIB_API const wchar_t* JGW_GetPluginSubFolder();
        //! 获取模块当前工作目录下的子目录 如c:/123/subfolder/ 
        JGW_FOUNDATIONFUNC_LIB_API size_t		JGW_GetExecSubFolder_C(wchar_t* path,size_t size,const wchar_t* subfolder,HMODULE hModule = NULL);
        //! 获取当前模块的工作目录 如c:/123/
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetModuleWorkPath_C(char* path,size_t size,HMODULE hModule = NULL);
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetModuleWorkPathA_C(wchar_t* path,size_t size,HMODULE hModule = NULL);
        //! 获取模块当前工作目录下的子目录 如c:/123/subfolder/ 
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetModuleWorkPath(std::wstring& path,HMODULE hModule = NULL);
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetModuleWorkPathA(std::string& path,HMODULE hModule = NULL);
        //! 获取当前模块的工作目录 如c:/123/
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetExecSubFolder(std::wstring& path,const wchar_t* subfolder,HMODULE hModule = NULL);
        //! 获取当前模块的工作目录 如c:/cache/123/
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetCacheExecSubFolder(std::wstring& path,const wchar_t* subfolder,HMODULE hModule = NULL);
        //! 获取当前模块的工作目录 如c:/cache/123/
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetConfigExecSubFolder(std::wstring& path,const wchar_t* subfolder,HMODULE hModule = NULL);
        //! 创建路径 bIsPath 是否路径,路径是目录的意思
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_CreateDirectory( const wchar_t* filename, bool bIsPath );
        //! 创建路径 bIsPath 是否路径
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_CreateDirectoryA( const char* filename, bool bIsPath );
        //! 斜杠
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_IsPathSlash( wchar_t c );
        //! 斜杠
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_IsPathSlashA( char c );
        //! 是否为空
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_IsNotNull( const wchar_t* pszText );
        //! 是否为空
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_IsNotNullA( const char* pszText );
        //!  将/替换为\         //
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_ReplaceSlash( wchar_t* path );

        JGW_FOUNDATIONFUNC_LIB_API void			JGW_ReplaceSlashA( char* path );

        JGW_FOUNDATIONFUNC_LIB_API void         JGW_PathAddBackslash(wchar_t* path);

        JGW_FOUNDATIONFUNC_LIB_API void         JGW_PathAddBackslashA(char* path);
        //! 确定一个文件或目录的文件系统对象的路径是否是有效的。
        JGW_FOUNDATIONFUNC_LIB_API bool         JGW_PathFileExistsA(const char* path);
        //! 确定一个文件或目录的文件系统对象的路径是否是有效的。
        JGW_FOUNDATIONFUNC_LIB_API bool         JGW_PathFileExists(const wchar_t* path);
        //! 将路径转化为WINDOWS路径  "C:\\EXEC"
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_PathReplaceWindowsPathRemoveBackslash(const std::wstring& strPath);
        //! 将路径转化为WINDOWS路径  "C:\\EXEC"
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_PathReplaceWindowsPathRemoveBackslashA(const std::string& strPath);

        //! 将路径转化为WINDOWS目录  "C:\\EXEC\\" 末尾添加Backslash
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_FolderReplaceWindowsFolderAddBackslash(const std::wstring& strPath);
        //! 将路径转化为WINDOWS路径  "C:\\EXEC\\" 末尾添加Backslash
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_FolderReplaceWindowsFolderAddBackslashA(const std::string& strPath);

        //! ERROR code转换成系统错误字符串
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetSystemErrorString(unsigned long errorcode);
        //!
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetSystemErrorStringA(unsigned long errorcode);
        //! 获取当前路径的扩展文件名
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetPathExtensionName(const char *filePath, char ext[], const size_t extLen);
        //! 获取当前路径的扩展文件名
        JGW_FOUNDATIONFUNC_LIB_API void			JGW_GetPathExtensionNameW(const wchar_t *filePath, wchar_t ext[], const size_t extLen);
        //! 遍历单个目录下文件 C库实现
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_TraverseFiles_C(std::string& path,int& numFile,const char* character = "\\*.*",void (*scanfile)(const char* filepath,const char* filename,void* lpData) = NULL ,void* lpData = NULL);
        //! 遍历所有目录
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_TraverseFolder_C(std::string& path,int& numFile,const char* character = "/*.*",void (*scanfile)(const char* filepath,const char* filename,void* lpData) = NULL,void* lpData = NULL);
        //! 遍历当前路径下所有目录
        //JGW_FOUNDATIONFUNC_LIB_API bool			JGW_TraverseFolder_VC(std::string& path,//! 路径
        //    int& numFile,//! 文件数量
        //    const char* character ,//! 分隔符 *.*
        //    void (*scanfile)(const char* filepath,const char* filename,void* lpData) = NULL,//! 回调函数
        //    void* lpData = NULL //! 传参
        //    );
        //! 遍历当前路径下所有目录
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_TraverseFolder_VC(std::wstring& path,int& numFile,
            const wchar_t* character,
            void (*scanfile)(const wchar_t* filepath,const wchar_t* filename,void* lpData),
            void* lpData);
        //! 遍历单个目录下文件 C库实现
        JGW_FOUNDATIONFUNC_LIB_API bool	JGW_TraverseFile_VC(std::wstring& path,int& numFile,
            const wchar_t* character,
            void (*scanfile)(const wchar_t* filepath,const wchar_t* filename,void* lpData),
            void* lpData);

        //! 当前路径是否是相对路径
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_PathIsRelativeA( const char* filePath );
        //! 当前路径是否是相对路径
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_PathIsRelativeW( const wchar_t* filePath );
        //! 选择文件
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_ChooseFile(std::wstring& strFile,const wchar_t* strFilter,HWND hwnd = NULL,DWORD Flags = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_LONGNAMES|OFN_PATHMUSTEXIST,const wchar_t* lpstrInitialDir = NULL);
        //! 多选
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_ChooseMultiSelectFileNames(std::vector<std::wstring> &_vMultiFiles,const wchar_t* strFilter,HWND hwnd = NULL,DWORD Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT ,const wchar_t* lpstrInitialDir = NULL );
        //! 
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_ChooseMultiSelectFileNamesFolder(std::wstring& folder,std::vector<std::wstring> &_vMultiFiles,const wchar_t* strFilter,HWND hwnd = NULL,DWORD Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT ,const wchar_t* lpstrInitialDir = NULL );
        //! 多选
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_ChooseMultiSeleJGWile(std::vector<std::wstring> &_vMultiFiles,const wchar_t* strFilter,HWND hwnd = NULL,DWORD Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT ,const wchar_t* lpstrInitialDir = NULL );
        //!
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_ChooseFolderSetFolderPath(std::wstring& strFile,const wchar_t* lpstrInitialDir,HWND hwnd = NULL,int iImage = 0);
        //! 选择路径
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_ChooseFolder(std::wstring& strFile,HWND hwnd = NULL,int iImage = 0);
        //! 保存文件名 
        JGW_FOUNDATIONFUNC_LIB_API bool			JGW_SaveFileName(std::wstring& strFile,const wchar_t* strFilter = L"excel File(*.xls,*.xlsx)\0*.xlsx;*.xls\0all file(*.*)\0*.*\0\0",HWND hwnd = NULL,const wchar_t* strext = L"txt",DWORD Flags = OFN_EXPLORER|OFN_ENABLEHOOK|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST);
        //!
        //! JGW_FOUNDATIONFUNC_LIB_API bool			JGW_SaveFileNameToFileName(std::wstring& strFile,const wchar_t* strFilter = L"excel File(*.xls,*.xlsx)\0*.xlsx;*.xls\0all file(*.*)\0*.*\0\0",HWND hwnd = NULL,const wchar_t* strext = L"xls",const wchar_t* ,DWORD Flags = OFN_EXPLORER|OFN_ENABLEHOOK|OFN_HIDEREADONLY|OFN_NOCHANGEDIR|OFN_PATHMUSTEXIST);

        //! 
        JGW_FOUNDATIONFUNC_LIB_API std::wstring	JGW_GetFileNameToFilePath( const wchar_t* filename );

        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetFileNameToFilePathA( const char* filename );


        JGW_FOUNDATIONFUNC_LIB_API std::wstring	JGW_GetPathOfFile(const wchar_t* filename);

        JGW_FOUNDATIONFUNC_LIB_API std::wstring	JGW_GetFileFolderToFilePath( const wchar_t* szFilePath );

        JGW_FOUNDATIONFUNC_LIB_API std::string	JGW_GetFileFolderToFilePathA( const char* szFilePath );

        JGW_FOUNDATIONFUNC_LIB_API void JGW_CloseFilePtr_C(FILE* fp);

        JGW_FOUNDATIONFUNC_LIB_API size_t JGW_GetFileSize_C(const char* szFilePath,const char * mode = "rb");

        //! JGW_FOUNDATIONFUNC_LIB_API FILE* JGW_GetFilePtrToPath_C(const char* szFilePath,const char * mode = "rb");
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_WriteFileToBuf(const char* szFilePath,const char* buf,size_t buf_size,const char * mode = "wb");

        JGW_FOUNDATIONFUNC_LIB_API bool JGW_WriteFileToBufW(const wchar_t* szFilePath,const char* buf,size_t buf_size,const wchar_t * mode = L"wb");

        JGW_FOUNDATIONFUNC_LIB_API size_t JGW_GetFilePtrFileSize_C(FILE* fp);

        JGW_FOUNDATIONFUNC_LIB_API char* JGW_GetFileBufToPath_C(const char* szFilePath,size_t& file_size,const char * mode = "rb");

        JGW_FOUNDATIONFUNC_LIB_API bool JGW_GetFileBufToPath_BufLen_C(const char* szFilePath,char* file_buf,size_t& buf_size,const char * mode = "rb");

        JGW_FOUNDATIONFUNC_LIB_API size_t JGW_GetFileBufToFilePtr_C(FILE* fp,char* file_buf,size_t read_len);
        //! true : 文件存在
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_FileExistsToFilePath(const wchar_t* strFilePath);
        //! true : 文件存在
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_FileExistsToFilePathA(const char* strFilePath);
        //! 相对路径转绝对路径
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_RealativePathToAbsPath(const wchar_t* strPath);
        //!
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_RealPathToAbsPathBasePath(const wchar_t* strPath,const wchar_t* strBaseFolder);
        //!
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_RealativePathToAbsPathA(const std::string& strPath,const std::string& strBaseFolder = "");

        //! 获取TSE配置文件
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetTSEConfigFilePath(const std::wstring& strModuleFolderName,const std::wstring& strSuiteFileName);
        //! cache 
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetCacheConfigFilePath(const std::wstring& strModuleFolderName,const std::wstring& strConfigFileName);
        //! cache folder
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetCacheConfigFolder(const std::wstring& strModuleFolderName);
        //! config
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetConfigConfigFilePath(const std::wstring& strModuleFolderName,const std::wstring& strConfigFileName);
        //! config folder
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetConfigConfigFolder(const std::wstring& strModuleFolderName);

        //! 
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetExecConfigFilePath(const std::wstring& strConfigFileName);
        //!
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetSkinUIConfigFilePath(const wchar_t* strDefaultFileName = L"skin\\uiconfig.xml");
        //!
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_DeleteFile(const std::wstring& strFilePath);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_DeleteDirectoryW(const std::wstring& path);

        // true 目录存在
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_IsFolderExist(const std::wstring& strFolderPath);
        JGW_FOUNDATIONFUNC_LIB_API bool JGW_IsFolderExistA(const std::string& strFolderPath);
        //! 获取文件最后访问时间，时间戳秒为单位
        JGW_FOUNDATIONFUNC_LIB_API __int64 JGW_GetFileLastAccessTimeTimeStampSec(const std::wstring& strFilePath);
        //! 获取文件最后修改时间，时间戳秒为单位
        JGW_FOUNDATIONFUNC_LIB_API __int64 JGW_GetFileLastWriteTimeTimeStampSec(const std::wstring& strFilePath);



















        //! 获取应用程序路径目录 如c:/123/
        //const wchar_t* GetExecWorkPath();
        //! 获取当前工作目录下的子目录 如c:/123/subfolder/  需要释放内存
        //const wchar_t* _GetExecSubFolder(const wchar_t* subfolder);
        //! 释放获取当前工作目录下的内存
        //void FreeFilePathMem(const wchar_t* buf);

#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
    }
#endif
}

#endif

