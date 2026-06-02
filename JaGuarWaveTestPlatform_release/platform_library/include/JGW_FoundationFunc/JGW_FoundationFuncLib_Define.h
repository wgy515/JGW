#ifndef JGW_FOUNDATIONFUNCLIB_DEFINE_H__
#define JGW_FOUNDATIONFUNCLIB_DEFINE_H__

#if (defined JGW_FOUNDATIONFUNC_LIB_EXPORT) || (defined JGW_FOUNDATIONFUNC_LIB_IMPORT) || (defined JGW_FOUNDATIONFUNC_LIB_DLL_EXPORTS) || (defined  JGW_FOUNDATIONFUNC_LIB_DLL_IMPORT)
#else
#define JGW_FOUNDATIONFUNC_LIB_IMPORT
#endif

#if (defined JGW_FOUNDATIONFUNC_LIB_EXPORT) || (defined JGW_FOUNDATIONFUNC_LIB_IMPORT)
#	define JGW_FOUNDATIONFUNC_LIB_API extern
#	define JGW_FOUNDATIONFUNC_LIB_CLASS
#endif


#ifdef JGW_FOUNDATIONFUNC_LIB_DLL_EXPORTS
#	define JGW_FOUNDATIONFUNC_DLL_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_FOUNDATIONFUNC_LIB_API __declspec(dllexport)
#		define JGW_FOUNDATIONFUNC_LIB_CLASS __declspec(dllexport)
#	else
#		define JGW_FOUNDATIONFUNC_LIB_API 
#		define JGW_FOUNDATIONFUNC_LIB_CLASS 
#	endif
#endif

// #ifdef JGW_FOUNDATIONFUNC_LIB_IMPORT
// #		define JGW_FOUNDATIONFUNC_LIB_API extern
// #		define JGW_FOUNDATIONFUNC_LIB_CLASS 
// #endif

#ifdef JGW_FOUNDATIONFUNC_LIB_DLL_IMPORT
#	define JGW_FOUNDATIONFUNC_DLL_EXTERN_
#	if _MSC_VER >= 1500
#		define JGW_FOUNDATIONFUNC_LIB_API __declspec(dllimport)
#		define JGW_FOUNDATIONFUNC_LIB_CLASS __declspec(dllimport)
#	else
#		define JGW_FOUNDATIONFUNC_LIB_API 
#		define JGW_FOUNDATIONFUNC_LIB_CLASS 
#	endif
// #	if _DEBUG
// #		pragma comment(lib,"JGW_FOUNDATIONFUNCFuncLib_d.lib")
// #	else
// #		pragma comment(lib,"JGW_FOUNDATIONFUNCFuncLib.lib")
// #	endif
#endif

#if (defined JGW_FOUNDATIONFUNC_LIB_IMPORT) || (defined JGW_FOUNDATIONFUNC_LIB_DLL_IMPORT)
#	if _DEBUG
#		pragma comment(lib,"JGW_FoundationFunc_d.lib")
#	else
#		pragma comment(lib,"JGW_FoundationFunc.lib")
#	endif
#endif

#pragma warning(disable:4996)

#define JGW_FormatMessage

#define FORMAT_WSTRING(strLog,szFormat)\
	int nListCount = 0;\
{\
	va_list pArgList;\
	va_start(pArgList,szFormat);\
	int nLength = _vscwprintf(szFormat,pArgList) + 1;\
	strLog.resize(nLength);\
	nListCount +=_vsnwprintf_s(const_cast<wchar_t *>(strLog.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);\
	va_end(pArgList);\
	}\
	strLog.reserve(nListCount + 1);

#define FORMAT_STRING(strLog,szFormat)\
	int nListCount = 0;\
{\
	va_list pArgList;\
	va_start(pArgList,szFormat);\
	int nLength = _vscprintf(szFormat,pArgList) + 1;\
	strLog.resize(nLength);\
	nListCount +=_vsnprintf_s(const_cast<char *>(strLog.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);\
	va_end(pArgList);\
	}\
	strLog.reserve(nListCount + 1);

#define SafeCall(p, f)                          if (p) p->f
#define SafeCallIf(p, f, v)                     ((p) ? (p->f) : (v))
#define SafeCallWithReturn(p, f, v)             if (p) return p->f; else return (v);
#define WndSafeCall(p, f)                       if (p && ::IsWindow(p->GetSafeHwnd())) p->f
#define WndSafeCallWithReturn(p, f, v)          if (p && ::IsWindow(p->GetSafeHwnd())) return p->f; else return (v);
#define InterfaceSafeCall(p, f)                 if (p.IsNotNull()) p->f
#define InterfaceSafeCallIf(p, f, v)            (p.IsNotNull() ? (p->f) : (v))
#define InterfaceSafeCallWithReturn(p, f, v)    if (p.IsNotNull()) return p->f; else return (v);

#define a2w_a					JGW_A2W_A 
#define wstrcomparenocasewstr	JGW_WStrComparenoCaseWStr
#define a2w						JGW_A2W 
#define	MakestringToUpper		JGW_MakeStringToUpper
#define ReplaceStringA			JGW_ReplaceStringA
#define hexIntToChar			JGW_HexIntToChar
#define hexCharToInt			JGW_HexCharToInt
#define w2a_w					JGW_W2A_W
#define FormatWString			JGW_FormatWString
#define w2a						JGW_W2A
#define wstrcasewstr			JGW_WStrCaseWStr
#define ParserStrA				JGW_ParserStrA
#define eraseChar				JGW_EraseChar
#define ReplaceCharW			JGW_ReplaceCharW

enum EWINDOWS_VERSION
{
	UNKNOW_WINDOWS_VERSION = -1,
	Windows_3 = 0,
	Windows_95,
	Windows_NT_3,
	Windows_NT_4,
	WINDOWS_2000 ,
	WINDOWS_XP,
	WINDOWS_VISTA,
	WINDOWS_WIN7,//Windows Server 2008 R2
	WINDOWS_WIN8,//Windows Server 2012
	WINDOWS_WIN8_1,//Windows Server 2012 R2
	WINDOWS_WIN10
};
#include <tchar.h>
#include <Windows.h>
//! 05C6-901D
//解决USB Driver重复注册问题 USB\VID_1DBC&PID_0316   IgnoreHWSerNum1DBC0316
const TCHAR g_UsbFlagsKeyName[][60] = {_T("GlobalDisableSerNumGen"),_T("IgnoreHWSerNum1DBC0312"),_T("IgnoreHWSerNum05C69025"),_T("IgnoreHWSerNum05C69091"),_T("IgnoreHWSerNum05C69092"),_T("IgnoreHWSerNum05C69008"),_T("IgnoreHWSerNum18D1D00D"),_T("IgnoreHWSerNum05C6901D")};

extern const wchar_t* JGW_GetFOUNDATIONFUNCFuncErrorMsg();

#endif