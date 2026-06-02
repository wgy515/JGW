// 字符串操作导出函数

#ifndef JGW_FOUNDATIONFUNC_FUNCLIB_STRINGFUNC_H__
#define JGW_FOUNDATIONFUNC_FUNCLIB_STRINGFUNC_H__
#include <string>
#include <vector>
#include <map>
#include "JGW_FoundationFuncLib_Define.h"



namespace JGW
{
#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	extern "C" 
	{
#endif
		//! 将unicode码转为assic码
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_W2A_W( const wchar_t* s, int codepage = 0);
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_W2A( const std::wstring& s,int codepage = 0 );
		//! 将assic码转unicode码
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_A2W_A( const char* s, int codepage = 0 );
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_A2W( const std::string& s, int codepage = 0 );
		// window ANSI编码 转UTF-8编码
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_LocalToUTF8(const std::string& strLocal);
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_LocalToUTF8_C(const char* strLocal);
		// UTF-8编码转window ANSI编码
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_UTF8ToLocal(const std::string& strUtf8);
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_UTF8ToLocal_C(const char* strUtf8);
		//! C实现方式
		//JGW_FOUNDATIONFUNC_LIB_API const char* JGW_W2A_W_C( const wchar_t* s, int codepage = 0 );
		//JGW_FOUNDATIONFUNC_LIB_API const wchar_t* JGW_A2W_A_C( const char* s, int codepage = 0 );

		JGW_FOUNDATIONFUNC_LIB_API void JGW_FreeStringBufW( const wchar_t* s );
		JGW_FOUNDATIONFUNC_LIB_API void JGW_FreeStringBufA( const char* s );
		//! 将类名称改为hashkey码
		JGW_FOUNDATIONFUNC_LIB_API long _hashkey( const char *str );
		//!
		JGW_FOUNDATIONFUNC_LIB_API long JGW_WHashKey( const wchar_t* str );
		//! 转为大写
		JGW_FOUNDATIONFUNC_LIB_API void JGW_MakeStringToUpper( std::string& strSource );
		//! 转为小写
		JGW_FOUNDATIONFUNC_LIB_API void JGW_MakeStringToLower( std::string& strSource );
		//! 倒转
		JGW_FOUNDATIONFUNC_LIB_API void JGW_MakeStringReverse(std::string& strSource);

		//! 转为大写
		JGW_FOUNDATIONFUNC_LIB_API void JGW_MakeWstringToUpper( std::wstring& strSource );
		//! 转为小写
		JGW_FOUNDATIONFUNC_LIB_API void JGW_MakeWstringToLower( std::wstring& strSource );
		//! 倒转
		JGW_FOUNDATIONFUNC_LIB_API void JGW_MakeWstringReverse(std::wstring& strSource);

		//! 忽略大小写查找子串在父串位置
		//! 查找到则返回父串指针位置 否则返回NULL
		JGW_FOUNDATIONFUNC_LIB_API const char* JGW_StrCaseStr(const char* str, const char* subStr);
		//! 忽略大小写查找子串在父串位置
		//! 查找到则返回父串指针位置 否则返回NULL
		JGW_FOUNDATIONFUNC_LIB_API const wchar_t* JGW_WStrCaseWStr(const wchar_t* str, const wchar_t* subStr);
		//! 比较两个字符串大小  相等返回0  第一个字符串大于 则返回1  否则返回-1
		JGW_FOUNDATIONFUNC_LIB_API int JGW_StrComparenoCaseStr(const char* friststr, const char* secodertr);
		//! 比较两个字符串大小  相等返回0  第一个字符串大于 则返回1  否则返回-1
		JGW_FOUNDATIONFUNC_LIB_API int JGW_WStrComparenoCaseWStr(const wchar_t* str, const wchar_t* subStr);
		//! 解析字符串  按照指定格式将字符串解析
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserIntA(const char *ListStr,const char * delimiter,std::vector<int>& _vectorInt);
        //! 解析字符串  按照指定格式将字符串解析
        JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserIntAToHex(const char *ListStr,const char * delimiter,std::vector<size_t>& _vectorInt);
		//! 解析字符串  按照指定格式将字符串解析
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserFloatA(const char *ListStr,const char * delimiter,std::vector<float>& _vectorFloat);
		//! 解析字符串  按照指定格式将字符串解析
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserStrA(const char *ListStr,const char * delimiter,std::vector<std::string>& _vectorStr);
		//! 替换字符串
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ReplaceStringA(std::string& str, const std::string& src, const std::string& des);
		//! 替换字符
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ReplaceCharA(std::string& str,const char src, const char des);
		//! 解析字符串  按照指定格式将字符串解析
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserIntW(const wchar_t *ListStr,const wchar_t * delimiter,std::vector<int>& _vectorInt);
        
		//! 解析字符串  按照指定格式将字符串解析
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserFloatW(const wchar_t *ListStr,const wchar_t * delimiter,std::vector<float>& _vectorFloat);
		//! 解析字符串  按照指定格式将字符串解析
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ParserStrW(const wchar_t *ListStr,const wchar_t * delimiter,std::vector<std::wstring>& _vectorStr);
		//! 替换字符串
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ReplaceStringW(std::wstring& str, const std::wstring& src, const std::wstring& des);
		//! 替换字符
		JGW_FOUNDATIONFUNC_LIB_API void JGW_ReplaceCharW(std::wstring& str,const wchar_t src, const wchar_t des);
		//! 将整形转换成16进制字符  0 <= nHex < 16
		JGW_FOUNDATIONFUNC_LIB_API char JGW_HexIntToChar(int nHex);
		//! 将16进制字符转换成整形
		JGW_FOUNDATIONFUNC_LIB_API int JGW_HexCharToInt(char c);
		//! 格式化字符串
		JGW_FOUNDATIONFUNC_LIB_API void JGW_FormatWString(std::wstring& strFormat,const wchar_t* szFormat,...);
        //! 获取格式化后的字符串
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetFormatWString(const wchar_t* szFormat,...);
		//! 格式化字符串
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_FormatString(std::string& strFormat,const char* szFormat,...);
        //! 格式化字符串
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetFormatString(const char* szFormat,...);

		//! 从起始开始查找
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetSubStrToStartEndCharArray(const std::string& strSource,const std::string& strStart,const char* strEnd = "\n",const std::string& strDef = "");
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetICaseSubStrToStartEndCharArray(const std::string& strSource,const std::string& strStart,const char* strEnd = "\n",const std::string& strDef = "");
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetSubStrToStartEndCharArrayW(const std::wstring& strSource,const std::wstring& strStart,const wchar_t* strEnd = L"\n",const std::wstring& strDef = L"");
		// 忽略大小写查找
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetICaseSubStrToStartEndCharArrayW(const std::wstring& strSource,const std::wstring& strStart,const wchar_t* strEnd = L"\n",const std::wstring& strDef = L"");
        //! 从起始开始查找
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd = "\n",const std::string& strDef = "");
        //! 从起始开始查找 不删除起始字串
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetSubStrToNotEraseStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd = "\n",const std::string& strDef = "");
        //! 从起始开始查找
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetSubStrToStartEndStrW(const std::wstring& strSource,const std::wstring& strStart,const std::wstring& strEnd = L"\n",const std::wstring& strDef = L"");
		//! 从起始开始查找(忽略大小写)
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetICaseSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd = "\n",const std::string& strDef = "");
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetICaseSubStrToStartEndStrW(const std::wstring& strSource,const std::wstring& strStart,const std::wstring& strEnd = L"\n",const std::wstring& strDef = L"");
		// 查找忽略大小写
		JGW_FOUNDATIONFUNC_LIB_API size_t JGW_FindICase(const std::string& strSource,const std::string& strFind);
		JGW_FOUNDATIONFUNC_LIB_API size_t JGW_FindICaseW(const std::wstring& strSource,const std::wstring& strFind);
		// 查找忽略大小写
		JGW_FOUNDATIONFUNC_LIB_API size_t JGW_FindICaseIndex(const std::string& strSource,size_t uSourceStartIndex,const std::string& strFind);
		JGW_FOUNDATIONFUNC_LIB_API size_t JGW_FindICaseIndexW(const std::wstring& strSource,size_t uSourceStartIndex,const std::wstring& strFind);
        //! 从末尾开始查找
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetRSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd = "\n",const std::string& strDef = "");
        //! 从末尾开始查找
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetRSubStrToStartEndStrW(const std::wstring& strSource,const std::wstring& strStart,const std::wstring& strEnd = L"\n",const std::wstring& strDef = L"");


		//! 删除指定字符串
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_EraseChars( const std::string& sSource, const std::string& toRemove );
		//! 删除指定字符
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_EraseChar( const std::string& sSource, const char toRemove );
        //! 删除最后一个字符串后的指定字符串 不包含toRemove
        JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseAfterToRemoveChar(std::string& sSource,const char toRemove);
        //! 删除第一个指定字符串前的字符串包含toRemove
        JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseFristToRemoveChar(std::string& sSource,const char toRemove);

		//! 删除最后一个"\""后的字符串
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_EraseAfterLastQuote( std::string sSource);
		//! 删除最后一个"\n"后的字符串
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_EraseAfterNewLineChar( std::string sSource);
		//! 删除指定字符串
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_EraseCharsW( const std::wstring& sSource, const std::wstring& toRemove );
		//! 删除指定字符
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_EraseCharW( const std::wstring& sSource,const wchar_t toRemove);
        //! 删除最后一个字符串后的指定字符串 不包含toRemove
        JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseAfterToRemoveCharW(std::wstring& sSource,const wchar_t toRemove);
        //! 删除第一个指定字符串前的字符串 包含toRemove
        JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseFristToRemoveCharW(std::wstring& sSource,const wchar_t toRemove);
		//! 删除最后一个"\""后的字符串
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_EraseAfterLastQuoteW( std::wstring sSource);
		//! 删除最后一个"\n"后的字符串
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_EraseAfterNewLineCharW( std::wstring sSource);
		//! 删除首尾,
		JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseLastAndFristChars( std::wstring& sSource,const char toRemove = ',' );
		JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseLastAndFristCharsA( std::string& sSource,const char toRemove = ',' );
        //! 删除首尾 \r \n
        JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseLastAndFristTwoChars( std::string& sSource,const char toRemoveOne = '\r',const char toRemoveTwo = '\n');
        //! 删除首尾 \r \n
		JGW_FOUNDATIONFUNC_LIB_API void JGW_EraseLastAndFristTwoCharsW( std::wstring& sSource,const char toRemoveOne = '\r',const char toRemoveTwo = '\n');
        //! 年_月_日 2018-8-2
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetTimeFolder();

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetCurrentTimeStampYMDHMS(const std::wstring& strFormate = _T("%04d%02d%02d%02d%02d%02d"));
        //! 
        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetCurrentTimeStampHMS(const std::wstring& strFormate = _T("%02d%02d%02d"));
        //! 
        JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetCurrentTimeStampYMDHMSA(const std::string& strFormate = "%04d%02d%02d%02d%02d%02d");

		//!
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetTimeStringToTimestampW(__int64 dw64TimeStampMSec,const std::wstring& strFormat = L"%04d-%02d-%02d %02d:%02d:%02d.%03d");

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetCPKTestStartTime();

		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetCurrentTimeStamp();

        JGW_FOUNDATIONFUNC_LIB_API SYSTEMTIME JGW_TimeT2SystemTime(const time_t& t);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API time_t JGW_SystemTime2TimeT(const SYSTEMTIME& st);
        //! 修改系统时钟
        JGW_FOUNDATIONFUNC_LIB_API void JGW_SetWindowTime(long timeStamp);
        //! 获取系统时间戳
        JGW_FOUNDATIONFUNC_LIB_API time_t JGW_GetWindowTime();

        //! JGW_FOUNDATIONFUNC_LIB_API time_t JGW_GetTimetToTimeStamp(size_t timeStamp);
        //! 如果为正  timeStamp1 > timeStamp2  为0则相等，为负 timeStamp1 < timeStamp2
        JGW_FOUNDATIONFUNC_LIB_API double JGW_DiffTime(size_t timeStamp1,size_t timeStamp2);

        JGW_FOUNDATIONFUNC_LIB_API size_t JGW_GetTimeOfDay();

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetFileNameToCurrentTimeStamp();

        JGW_FOUNDATIONFUNC_LIB_API std::wstring& JGW_URLEncodeW(const std::wstring& str,  std::wstring& encodedStr,const std::wstring& reserved = L"\\/");

        JGW_FOUNDATIONFUNC_LIB_API std::wstring& JGW_URLDecodeW(const std::wstring& str,  std::wstring& decodedStr,bool plusAsSpace = false);

        JGW_FOUNDATIONFUNC_LIB_API std::string& JGW_URLEncodeA(const std::string& str,  std::string& encodedStr,const std::string& reserved = "\\/");

        JGW_FOUNDATIONFUNC_LIB_API std::string& JGW_URLDecodeA(const std::string& str,  std::string& decodedStr,bool plusAsSpace = false);

        JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_BuildLog4cplusLogFilePath(const std::wstring& strLoggerName);

        JGW_FOUNDATIONFUNC_LIB_API void JGW_PareserConfigString(const std::wstring& strConfig,std::map<std::wstring,std::wstring>& mapProperty);
        //! csvline_populate
        JGW_FOUNDATIONFUNC_LIB_API void JGW_PopulateCSVLine(std::vector<std::wstring> &record, const std::wstring& line, wchar_t delimiter = L',');
        JGW_FOUNDATIONFUNC_LIB_API void JGW_PopulateCSVLineA(std::vector<std::string> &record, const std::string& line, char delimiter = ',');


        //! 
        JGW_FOUNDATIONFUNC_LIB_API void ParseStringA(const std::string& inputString, char delimiter,std::vector<std::string>& _vectorStr);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API void ParseStringW(const std::wstring& inputString, wchar_t delimiter,std::vector<std::wstring>& _vectorStr);
        //! 
        JGW_FOUNDATIONFUNC_LIB_API void ParseMultiStringA(const std::string& inputString, std::vector<char>& delimiterVec,std::vector<std::string>& _vectorStr);
        //!
        JGW_FOUNDATIONFUNC_LIB_API void ParseMultiStringW(const std::wstring& inputString, std::vector<wchar_t>& delimiterVec,std::vector<std::wstring>& _vectorStr);
		// 获取KEY-VALUE对应KEY 的value整数值
		JGW_FOUNDATIONFUNC_LIB_API int JGW_GetJsonIntICaseFieldDef(const std::string& strNotif,const std::string& strField,int def);
		JGW_FOUNDATIONFUNC_LIB_API int JGW_GetJsonIntICaseFieldDefW(const std::wstring& strNotif,const std::wstring& strField,int def);
		// 获取KEY-VALUE对应KEY 的value 字符串
		JGW_FOUNDATIONFUNC_LIB_API std::string JGW_GetJsonICaseStringFieldDef(const std::string& strNotif,const std::string& strField,const std::string& strDes = "NaN");
		JGW_FOUNDATIONFUNC_LIB_API std::wstring JGW_GetJsonICaseStringFieldDefW(const std::wstring& strNotif,const std::wstring& strField,const std::wstring& strDes = L"NaN");


#ifdef JGW_FOUNDATIONFUNC_DLL_EXTERN_
	}
#endif
}

#endif
