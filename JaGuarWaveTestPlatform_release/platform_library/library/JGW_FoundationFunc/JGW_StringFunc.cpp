// 字符串操作导出函数
#include "stdafx.h"

#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include <JGW_FoundationFunc\JGW_FilePath.h>
#include <Windows.h>
#include <algorithm>
#include <time.h>
#include <WinSock.h>

namespace JGW
{
    inline long JGW_WHashKey(const wchar_t* str)
    {
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;

        while (*str)
        {
            hash = hash * seed + (*str++);
        }

        long i = (hash & 0x7FFFFFFF);
        return i;
    }

    inline long _hashkey(const char *str)
    {
        unsigned int seed = 131; // 31 131 1313 13131 131313 etc..
        unsigned int hash = 0;

        while (*str)
        {
            hash = hash * seed + (*str++);
        }

        return (hash & 0x7FFFFFFF);
    }

    inline void JGW_FreeStringBufA( const char* s )
    {
        if ( s )
        {
            delete[] s;
            s = NULL;
        }
    }

    inline void JGW_FreeStringBufW( const wchar_t* s )
    {
        if ( s )
        {
            delete[] s;
            s = NULL;
        }
    }


    //转码
    //ASCII to Unicode
    std::wstring AtoW(const std::string &str)
    {
        //unicode的长度
        int nwLen = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, nullptr, 0);
        //分配内存
        wchar_t *pUnicode = (wchar_t *)malloc(sizeof(wchar_t) * nwLen);

        if (pUnicode)
        {
            MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pUnicode, nwLen);
            std::wstring wstr = pUnicode;
            free(pUnicode);
            return wstr;
        }

        return NULL;
    }

    //Unicode to ASCII
    std::string WtoA(const std::wstring &wstr)
    {
        int naLen = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);

        char *pAssii = (char *)malloc(sizeof(char) * naLen);

        if (pAssii)
        {
            WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, pAssii, naLen, nullptr, nullptr);
            std::string str = pAssii;
            free(pAssii);
            return str;
        }

        return NULL;
    }

    //utf8 to Unicode
    std::wstring U2W(const std::string &str)
    {
        int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        wchar_t *pUnicode = (wchar_t *)malloc(sizeof(wchar_t) * nwLen);

        if (pUnicode)
        {
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, pUnicode, nwLen);
            std::wstring wstr = pUnicode;
            free(pUnicode);
            return wstr;
        }

        return NULL;
    }

    //Unicode to utf8
    std::string W2U(const std::wstring &wstr)
    {
        int naLen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        char *pAssii = (char *)malloc(sizeof(char) * naLen);

        if (pAssii)
        {
            WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, pAssii, naLen, nullptr, nullptr);
            std::string str = pAssii;
            free(pAssii);
            return str;
        }

        return NULL;
    }

    //ASCII to utf8
    std::string A2U(const std::string &str)
    {
        return W2U(AtoW(str));
    }

    //utf8 to ASCII
    std::string U2A(const std::string &str)
    {
        return WtoA(U2W(str));
    }

    inline std::string JGW_W2A_W(const wchar_t* s, int codepage/* = 0*/)
    {
        std::string str;
        int wlen = ( NULL == s ) ? 0 : (int)wcslen(s);

        if ( wlen > 0)
        {
            int len = WideCharToMultiByte(codepage, 0, s, wlen, NULL, 0, NULL, NULL);
            str.resize(len);
            WideCharToMultiByte(codepage, 0, s, wlen,
                const_cast<char*>(str.data()), len, NULL, NULL);
        }

        return str;
    }

    inline std::string JGW_W2A( const std::wstring& s,int codepage /* = 0 */ )
    {
        return JGW_W2A_W(s.c_str(),codepage);
    }

    inline std::wstring JGW_A2W_A(const char* s, int codepage/* = 0*/)
    {
        std::wstring wstr;
        int len = (NULL == s) ? 0 : (int)strlen(s);

        if ( len > 0)
        {
            int wlen = MultiByteToWideChar(codepage, 0, s, len, NULL, 0);
            wstr.resize(wlen);
            MultiByteToWideChar(codepage, 0, s, len,
                const_cast<wchar_t*>(wstr.data()), wlen);
        }

        return wstr;
    }
#if 0
	// UTF-8转本地编码示例
	std::string UTF8ToLocal(const std::string& utf8) {
		int len = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
		wchar_t* wstr = new wchar_t[len];
		MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wstr, len);
		len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
		char* str = new char[len];
		WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
		std::string result(str);
		delete[] wstr; delete[] str;
		return result;
	}
#endif

    inline std::wstring JGW_A2W( const std::string& s, int codepage /* = 0 */ )
    {
        return JGW_A2W_A(s.c_str(),codepage);
    }

	std::string JGW_LocalToUTF8_C(const char* strLocal)
	{
		std::wstring strLocalW = JGW_A2W_A(strLocal);
		return JGW_W2A(strLocalW,CP_UTF8);
	}

	std::string JGW_LocalToUTF8(const std::string& strLocal)
	{
		return JGW_LocalToUTF8_C(strLocal.c_str());
	}

	std::string JGW_UTF8ToLocal_C(const char* strUtf8)
	{
		std::wstring strUtf8W = JGW_A2W_A(strUtf8,CP_UTF8);
		return JGW_W2A(strUtf8W);
	}

	std::string JGW_UTF8ToLocal(const std::string& strUtf8)
	{
		return JGW_UTF8ToLocal_C(strUtf8.c_str());
	}

	

    //const char* JGW_W2A_W_C( const wchar_t* s, int codepage /* = 0 */ )
    //{
    //    int wlen = (NULL == s) ? 0 : (int)wcslen(s);
    //    char* str = "";

    //    if ( wlen > 0)
    //    {
    //        int len = WideCharToMultiByte(codepage, 0, s, wlen, NULL, 0, NULL, NULL);
    //        str = new char[len+1];
    //        WideCharToMultiByte(codepage, 0, s, wlen,str, len, NULL, NULL);
    //        str[len] = '\0';
    //    }
    //    return str;
    //}

    //const wchar_t* JGW_A2W_A_C( const char* s, int codepage /* = 0 */ )
    //{
    //    wchar_t* wstr = L"";
    //    int len = (NULL == s) ? 0 : (int)strlen(s);

    //    if ( len > 0)
    //    {
    //        int wlen = MultiByteToWideChar(codepage, 0, s, len, NULL, 0);
    //        wstr = new wchar_t[wlen+1];
    //        MultiByteToWideChar(codepage, 0, s, len,wstr, wlen);
    //        wstr[wlen] = '\0'; 
    //    }

    //    return wstr;
    //}

    void JGW_MakeStringToUpper( std::string& strSource )
    {
        std::transform(strSource.begin(), strSource.end(), strSource.begin(), toupper);
    }

    void JGW_MakeStringToLower( std::string& strSource )
    {
        std::transform(strSource.begin(), strSource.end(), strSource.begin(), tolower);
    }

    void JGW_MakeStringReverse(std::string& strSource)
    {
        reverse(strSource.begin(),strSource.end());
    }

    void JGW_MakeWstringToUpper( std::wstring& strSource )
    {
        std::transform(strSource.begin(), strSource.end(), strSource.begin(), towupper);
    }

    void JGW_MakeWstringToLower( std::wstring& strSource )
    {
        std::transform(strSource.begin(), strSource.end(), strSource.begin(), towlower);
    }

    void JGW_MakeWstringReverse(std::wstring& strSource)
    {
        reverse(strSource.begin(),strSource.end());
    }

    void JGW_ParserIntA(const char *ListStr,const char * delimiter,std::vector<int>& _vectorInt)
    {
        if (!ListStr || !delimiter)
        {
            return;
        }

        size_t length = strlen(ListStr)+1;
        char* pSource = new char[length];
        strcpy(pSource,ListStr);
        //strcpy_s(pSource,length,ListStr);
        //int temp = 0;

        for (char *pSubStr = strtok(pSource,delimiter);
            pSubStr;
            pSubStr = strtok(NULL,delimiter))
        {
            _vectorInt.push_back(atoi(pSubStr));
        }
        // 		for (char *pSubStr = strtok_s(pSource,delimiter,&szBuffer);
        // 			pSubStr;
        // 			pSubStr = strtok_s(NULL,delimiter,&szBuffer))
        // 		{
        // 			temp = atoi(pSubStr);
        // 			_vectorInt.push_back(temp);
        // 		}

        if(pSource != NULL)
        {
            delete [] pSource;
            pSource = NULL;
        }
    }

    void JGW_ParserIntAToHex(const char *ListStr,const char * delimiter,std::vector<size_t>& _vectorInt)
    {
        if (!ListStr || !delimiter)
        {
            return;
        }

        size_t length = strlen(ListStr) + 1;
        char* pSource = new char[length];
        size_t nTemp = 0;
        strcpy(pSource,ListStr);

        for (char *pSubStr = strtok(pSource,delimiter);
            pSubStr;
            pSubStr = strtok(NULL,delimiter))
        {
            sscanf_s(pSubStr,"%02X",&nTemp);
            _vectorInt.push_back(nTemp);
        }

        if(pSource != NULL)
        {
            delete [] pSource;
            pSource = NULL;
        }
    }

    void JGW_ParserFloatA(const char *ListStr,const char * delimiter,std::vector<float>& _vectorFloat)
    {
        if (!ListStr || !delimiter)
        {
            return;
        }

        size_t length = strlen(ListStr)+1;
        char *pSource = new char[length];
        strcpy(pSource,ListStr);
        //strcpy_s(pSource,length,ListStr);
        //char *szBuffer = NULL;
        //	_vectorFloat.clear();
        //float temp = 0;

        for (char *pSubStr = strtok(pSource,delimiter);
            pSubStr;
            pSubStr = strtok(NULL,delimiter))
        {
            _vectorFloat.push_back(static_cast<float>(atof(pSubStr)));
        }
        // 		for (char *pSubStr = strtok_s(pSource,delimiter,&szBuffer);
        // 			pSubStr;
        // 			pSubStr = strtok_s(NULL,delimiter,&szBuffer))
        // 		{
        // 			temp = static_cast<float>(atof(pSubStr));
        // 			_vectorFloat.push_back(temp);
        // 		}

        if(pSource != NULL)
        {
            delete [] pSource;
            pSource = NULL;
        }
    }

    void JGW_ParserStrA(const char *ListStr,const char * delimiter,std::vector<std::string>& _vectorStr)
    {
        if (!ListStr || !delimiter || 0 == strlen(ListStr))
        {
            return;
        }

        size_t length = strlen(ListStr)+1;
        char *pSource = new char[length];
        strcpy(pSource,ListStr);
        //strcpy_s(pSource,length,ListStr);
        //char *szBuffer = NULL;
        //	_vectorStr.clear();
        for (char *pSubStr = strtok(pSource,delimiter);
            pSubStr;
            pSubStr = strtok(NULL,delimiter))
        {
            _vectorStr.push_back(pSubStr);
        }
        // 		for (char *pSubStr = strtok_s(pSource,delimiter,&szBuffer);
        // 			pSubStr;
        // 			pSubStr = strtok_s(NULL,delimiter,&szBuffer))
        // 		{
        // 			_vectorStr.push_back(pSubStr);
        // 		}

        if ( pSource != NULL )
        {
            delete [] pSource;
            pSource = NULL;
        }
    }

    void JGW_ReplaceStringA(std::string& str, const std::string& src, const std::string& des)
    {
        int pos = -1;
        int curPos = 0;
        if ( 0 == src.size() )
        {
            return;
        }

        while ( -1 != (pos = str.find(src, curPos)) )
        {
            str.replace(pos, src.size(), des);
            curPos = pos + des.size();
        }
    }

    void JGW_ReplaceCharA(std::string& str,const char src, const char des)
    {
        int pos = -1;
        int curPos = 0;

        while ( -1 != (pos = str.find(src, curPos)) )
        {
            str.replace(pos, 1,1, des);
            curPos = pos + 1;
        }
    }

    void JGW_ParserFloatW(const wchar_t* ListStr,const wchar_t* delimiter,std::vector<float>& _vectorFloat)
    {
        if (!ListStr || !delimiter)
        {
            return;
        }

        size_t length = wcslen(ListStr)+1;
        wchar_t *pSource = new wchar_t[length];
        wcscpy(pSource,ListStr);
        //wcscpy_s(pSource,length,ListStr);
        //wchar_t *szBuffer = NULL;
        //float temp = 0;

        for (wchar_t *pSubStr = wcstok(pSource,delimiter);
            pSubStr;
            pSubStr = wcstok(NULL,delimiter))
        {
            //temp = ;
            _vectorFloat.push_back(static_cast<float>(_wtof(pSubStr)));
        }
        // 		for (wchar_t *pSubStr = wcstok_s(pSource,delimiter,&szBuffer);
        // 			pSubStr;
        // 			pSubStr = wcstok_s(NULL,delimiter,&szBuffer))
        // 		{
        // 			temp = static_cast<float>(_wtof(pSubStr));
        // 			_vectorFloat.push_back(temp);
        // 		}

        if(pSource != NULL)
        {
            delete [] pSource;
            pSource = NULL;
        }
    }

    void JGW_ParserIntW(const wchar_t *ListStr,const wchar_t * delimiter,std::vector<int> &_vectorInt)
    {
        if (!ListStr || !delimiter)
        {
            return;
        }

        size_t length = wcslen(ListStr)+1;
        wchar_t *pSource = new wchar_t[length];
        wcscpy(pSource,ListStr);
        //wcscpy_s(pSource,length,ListStr);
        //wchar_t *szBuffer = NULL;
        //	_vectorInt.clear();
        //int temp = 0;

        for (wchar_t *pSubStr = wcstok(pSource,delimiter);
            pSubStr;
            pSubStr = wcstok(NULL,delimiter))
        {
            _vectorInt.push_back(_wtoi(pSubStr));
        }

        // 		for (wchar_t *pSubStr = wcstok_s(pSource,delimiter,&szBuffer);
        // 			pSubStr;
        // 			pSubStr = wcstok_s(NULL,delimiter,&szBuffer))
        // 		{
        // 			_vectorInt.push_back(_wtoi(pSubStr));
        // 		}

        if(pSource != NULL)
        {
            delete [] pSource;
            pSource = NULL;
        }
    }

    void JGW_ParserStrW(const wchar_t *ListStr,const wchar_t * delimiter,std::vector<std::wstring> &_vectorStr)
    {
        if (!ListStr || !delimiter)
        {
            return;
        }

        size_t length = wcslen(ListStr)+1;
        wchar_t *pSource = new wchar_t[length];
        wcscpy(pSource,ListStr);
        //wcscpy_s(pSource,length,ListStr);
        //wchar_t *szBuffer = NULL;
        //	_vectorStr.clear();

        for (wchar_t *pSubStr = wcstok(pSource,delimiter);
            pSubStr;
            pSubStr = wcstok(NULL,delimiter))
        {
            _vectorStr.push_back(pSubStr);
        }

        // 		for (wchar_t *pSubStr = wcstok_s(pSource,delimiter,&szBuffer);
        // 			pSubStr;
        // 			pSubStr = wcstok_s(NULL,delimiter,&szBuffer))
        // 		{
        // 			_vectorStr.push_back(pSubStr);
        // 		}

        if(pSource != NULL)
        {
            delete [] pSource;
            pSource = NULL;
        }
    }


    void JGW_ReplaceStringW(std::wstring& str, const std::wstring& src, const std::wstring& des)
    {
        int pos = -1;
        int curPos = 0;

        while ( -1 != (pos = str.find(src, curPos)) )
        {
            str.replace(pos, src.size(), des);
            curPos = pos + des.size();
        }
    }

    void JGW_ReplaceCharW(std::wstring& str,const wchar_t src, const wchar_t des)
    {
        int pos = -1;
        int curPos = 0;

        while (-1 != (pos = str.find(src, curPos)))
        {
            str.replace(pos, 1,1, des);
            curPos = pos + 1;
        }
    }

    char JGW_HexIntToChar(int nHex)
    {
        if ( nHex >= 0 && nHex <= 9 ) return nHex + '0';
        if ( nHex >= 10 && nHex<= 15) return nHex + 'A' - 10;

        return '0';
    }

    int JGW_HexCharToInt(char c)  
    {   
        if (c >= '0' && c <= '9') return (c - '0');  
        if (c >= 'A' && c <= 'F') return (c - 'A' + 10);  
        if (c >= 'a' && c <= 'f') return (c - 'a' + 10);  
        return 0;  
    }

    inline std::wstring JGW_GetFormatWString(const wchar_t* szFormat,...)
    {
        int nListCount = 0;
        std::wstring strFormat;

        {
            va_list pArgList;
            va_start(pArgList,szFormat);
            int nLength = _vscwprintf(szFormat,pArgList) + 1;
            strFormat.resize(nLength);
            nListCount += _vsnwprintf_s(const_cast<wchar_t *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

            if( -1 == nListCount ) throw L"buffer too small";

            va_end(pArgList);
        }

        strFormat.resize(nListCount);

        return strFormat;
    }

    inline void JGW_FormatWString(std::wstring& strFormat,const wchar_t *szFormat,...)
    {
        int nListCount = 0;

        {
            va_list pArgList;
            va_start(pArgList,szFormat);
            int nLength = _vscwprintf(szFormat,pArgList) + 1;
            strFormat.resize(nLength);
            nListCount += _vsnwprintf_s(const_cast<wchar_t *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

            if( -1 == nListCount ) throw L"buffer too small";

            va_end(pArgList);
        }

        strFormat.resize(nListCount);
    }

    inline std::string JGW_GetFormatString(const char *szFormat,...)
    {
        int nListCount = 0;
        std::string strFormat;
        {
            va_list pArgList;
            va_start(pArgList,szFormat);
            int nLength = _vscprintf(szFormat,pArgList) + 1;
            strFormat.resize(nLength);
            nListCount += _vsnprintf_s(const_cast<char *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

            if( -1 == nListCount ) return "buffer too small";

            va_end(pArgList);
        }

        strFormat.resize(nListCount);

        return strFormat;
    }

    inline std::string JGW_FormatString(std::string& strFormat,const char *szFormat,...)
    {
        int nListCount = 0;

        {
            va_list pArgList;
            va_start(pArgList,szFormat);
            int nLength = _vscprintf(szFormat,pArgList) + 1;
            strFormat.resize(nLength);
            nListCount += _vsnprintf_s(const_cast<char *>(strFormat.data())+ nListCount,nLength-  nListCount,_TRUNCATE,szFormat,pArgList);

            if( -1 == nListCount ) return "buffer too small";

            va_end(pArgList);
        }

        strFormat.resize(nListCount);

        return strFormat;
    }

    inline std::wstring towstr(const std::wstring& str)
    {
        return str.empty() ? L"" : str.c_str();
    }

    int JGW_StrComparenoCaseStr(const char* str, const char* subStr)
    {
        size_t nlen1 = strlen(str);
        size_t nlen2 = strlen(subStr);

        if ( nlen1 < nlen2 )
        {
            return -1;
        }
        else if( nlen1 > nlen2 )
        {
            return 1;
        }

        return _strnicmp(str, subStr, nlen1);
    }

    int JGW_WStrComparenoCaseWStr(const wchar_t* str, const wchar_t* subStr)
    {
        size_t nlen1 = wcslen(str);
        size_t nlen2 = wcslen(subStr);

        if ( nlen1 < nlen2 )
        {
            return -1;
        }
        else if( nlen1 > nlen2 )
        {
            return 1;
        }

        return _wcsnicmp(str, subStr, nlen1);
    }

    const char* JGW_StrCaseStr(const char* str, const char* subStr)
    {
        size_t len = strlen(subStr);
        if(len == 0) return NULL;          

        while(*str)
        {
            if(_strnicmp(str, subStr, len) == 0)       /* 这里使用了可限定比较长度的strncasecmp */
            {
                return str;
            }
            str++;
        }
        return NULL;
    }

    const wchar_t* JGW_WStrCaseWStr(const wchar_t* str, const wchar_t* subStr)
    {
        size_t len = wcslen(subStr);
        if(len == 0) return NULL;          

        while(*str)
        {
            if(_wcsnicmp(str, subStr, len) == 0)       /* 这里使用了可限定比较长度的strncasecmp */
            {
                return str;
            }
            str++;
        }
        return NULL;
    }

    std::string JGW_GetSubStrToNotEraseStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd /* = "" */,const std::string& strDef /* = "" */)
    {
        std::string strRsp = strDef;
        size_t index = strSource.find(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());

        if (std::string::npos != index && std::string::npos != endIndex)
        {
            strRsp = strSource.substr(index,endIndex - index + strEnd.length());
            JGW_EraseLastAndFristTwoChars(strRsp,' ');
        }
        return strRsp;
    }

	std::string JGW_GetSubStrToStartEndCharArray(const std::string& strSource,const std::string& strStart,const char* strEnd /* = "" */,const std::string& strDef /* = "" */)
	{
		std::string strRsp = strDef;
		size_t index = strSource.find(strStart);
		size_t endIndex = std::string::npos;
		
		const char* pEndChar = strEnd;
		while (*pEndChar != '\0' && endIndex == std::string::npos)
		{
			endIndex = strSource.find(*pEndChar,index + strStart.length());
			pEndChar++;
		}
		//strlen()
			

		if (std::string::npos != index && std::string::npos != endIndex)
		{
			strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
			JGW_EraseLastAndFristTwoChars(strRsp,' ');
		}
		return strRsp;

	}

	std::string JGW_GetICaseSubStrToStartEndCharArray(const std::string& strSource,const std::string& strStart,const char* strEnd /* = "" */,const std::string& strDef /* = "" */)
	{
		std::string strRsp = strDef;
		//size_t index = strSource.find(strStart);
		size_t index = JGW_FindICase(strSource,strStart);
		size_t endIndex = std::string::npos;

		const char* pEndChar = strEnd;
		while (*pEndChar != '\0' && endIndex == std::string::npos)
		{
			//endIndex = strSource.find(*pEndChar,index + strStart.length());
			endIndex = JGW_FindICaseIndex(strSource,index + strStart.length(),std::string("") + (*pEndChar));
			pEndChar++;
		}

		if (std::string::npos != index && std::string::npos != endIndex)
		{
			strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
			JGW_EraseLastAndFristTwoChars(strRsp,' ');
		}
		return strRsp;
	}

	std::wstring JGW_GetICaseSubStrToStartEndCharArrayW(const std::wstring& strSource,const std::wstring& strStart,const wchar_t* strEnd /* = L"" */,const std::wstring& strDef /* = L"" */)
	{
		std::wstring strRsp = strDef;
		//size_t index = strSource.find(strStart);
		size_t index = JGW_FindICaseW(strSource,strStart);
		size_t endIndex = std::wstring::npos;

		const wchar_t* pEndChar = strEnd;
		while (*pEndChar != '\0' && endIndex == std::wstring::npos)
		{
			//endIndex = strSource.find(*pEndChar,index + strStart.length());
			endIndex = JGW_FindICaseIndexW(strSource,index + strStart.length(),std::wstring(L"") + (*pEndChar));
			pEndChar++;
		}

		if (std::wstring::npos != index && std::wstring::npos != endIndex)
		{
			strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
			JGW_EraseLastAndFristTwoCharsW(strRsp,' ');
		}
		return strRsp;
	}

	std::wstring JGW_GetSubStrToStartEndCharArrayW(const std::wstring& strSource,const std::wstring& strStart,const wchar_t* strEnd /* = L"" */,const std::wstring& strDef /* = L"" */)
	{
		std::wstring strRsp = strDef;
		size_t index = strSource.find(strStart);
		size_t endIndex = std::wstring::npos;

		const wchar_t* pEndChar = strEnd;
		while (*pEndChar != '\0' && endIndex == std::wstring::npos)
		{
			endIndex = strSource.find(*pEndChar,index + strStart.length());
			pEndChar++;
		}
		//strlen()


		if (std::wstring::npos != index && std::wstring::npos != endIndex)
		{
			strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
			JGW_EraseLastAndFristTwoCharsW(strRsp,' ');
		}
		return strRsp;
	}

    std::string JGW_GetSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd /* = "" */,const std::string& strDef /* = "" */)
    {
        std::string strRsp = strDef;
        size_t index = strSource.find(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());
        
        if (std::string::npos != index && std::string::npos != endIndex)
        {
            strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
            JGW_EraseLastAndFristTwoChars(strRsp,' ');
        }
        return strRsp;
    }

    std::string JGW_GetRSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd /* = "" */,const std::string& strDef /* = "" */)
    {
        std::string strRsp = strDef;
        size_t index = strSource.rfind(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());

        if (std::string::npos != index && std::string::npos != endIndex && index < endIndex)
        {
            strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
            JGW_EraseLastAndFristTwoChars(strRsp,' ');
        }
        return strRsp;
    }

	size_t JGW_FindICaseIndex(const std::string& strSource,size_t uSourceStartIndex,const std::string& strFind)
	{
		const char* pstrStart = strSource.c_str();
		const char* pstrFind = JGW_StrCaseStr(pstrStart + uSourceStartIndex,strFind.c_str());
		if (NULL == pstrFind)
		{
			return std::string::npos;
		}
		return pstrFind - pstrStart;
	}

	size_t JGW_FindICaseIndexW(const std::wstring& strSource,size_t uSourceStartIndex,const std::wstring& strFind)
	{
		const wchar_t* pstrStart = strSource.c_str();
		const wchar_t* pstrFind = JGW_WStrCaseWStr(pstrStart + uSourceStartIndex,strFind.c_str());
		if (NULL == pstrFind)
		{
			return std::wstring::npos;
		}
		return pstrFind - pstrStart;
	}

	size_t JGW_FindICase(const std::string& strSource,const std::string& strFind)
	{
		const char* pstrStart = strSource.c_str();
		const char* pstrFind = JGW_StrCaseStr(pstrStart,strFind.c_str());
		if (NULL == pstrFind)
		{
			return std::string::npos;
		}
		return pstrFind - pstrStart;
	}

	size_t JGW_FindICaseW(const std::wstring& strSource,const std::wstring& strFind)
	{
		const wchar_t* pstrStart = strSource.c_str();
		const wchar_t* pstrFind = JGW_WStrCaseWStr(pstrStart,strFind.c_str());
		if (NULL == pstrFind)
		{
			return std::wstring::npos;
		}
		return pstrFind - pstrStart;
	}

	std::string JGW_GetICaseSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd /* = "" */,const std::string& strDef /* = "" */)
	{
		std::string strRsp = strDef;
		//size_t index = strSource.find(strStart);
		//size_t endIndex = strSource.find(strEnd,index + strStart.length());
		size_t index = JGW_FindICase(strSource,strStart);
		size_t endIndex = JGW_FindICaseIndex(strSource,index + strStart.length(),strEnd);

		if (std::string::npos != index && std::string::npos != endIndex)
		{
			strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
			JGW_EraseLastAndFristTwoChars(strRsp,' ');
		}
		return strRsp;
	}

	std::wstring JGW_GetICaseSubStrToStartEndStrW(const std::wstring& strSource,const std::wstring& strStart,const std::wstring& strEnd /* = L"" */,const std::wstring& strDef /* = L"" */)
	{
		std::wstring strRsp = strDef;
		//size_t index = strSource.find(strStart);
		//size_t endIndex = strSource.find(strEnd,index + strStart.length());
		size_t index = JGW_FindICaseW(strSource,strStart);
		size_t endIndex = JGW_FindICaseIndexW(strSource,index + strStart.length(),strEnd);

		if (std::wstring::npos != index && std::wstring::npos != endIndex)
		{
			strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
			JGW_EraseLastAndFristTwoCharsW(strRsp,L' ');
		}
		return strRsp;
	}

    std::wstring JGW_GetSubStrToStartEndStrW(const std::wstring& strSource,const std::wstring& strStart,const std::wstring& strEnd /* = L"" */,const std::wstring& strDef /* = L"" */)
    {
        std::wstring strRsp = strDef;
        size_t index = strSource.find(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());

        if (std::wstring::npos != index && std::wstring::npos != endIndex)
        {
            strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
            JGW_EraseLastAndFristTwoCharsW(strRsp,L' ');
        }
        return strRsp;
    }

    std::wstring JGW_GetRSubStrToStartEndStrW(const std::wstring& strSource,const std::wstring& strStart,const std::wstring& strEnd /* = L"" */,const std::wstring& strDef /* = L"" */)
    {
        std::wstring strRsp = strDef;
        size_t index = strSource.rfind(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());

        if (std::wstring::npos != index && std::wstring::npos != endIndex && index < endIndex)
        {
            strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
            JGW_EraseLastAndFristTwoCharsW(strRsp,L' ');
        }
        return strRsp;
    }

    std::string JGW_EraseChar( const std::string& sSource, const char toRemove )
    {
        std::string retVal = sSource;
        while(retVal.find(toRemove) != std::string::npos)
            retVal.erase(retVal.find(toRemove),1);

        return retVal;
    }

    std::wstring JGW_EraseCharW(const std::wstring& sSource,const wchar_t toRemove)
    {
        std::wstring retVal = sSource;
        while(retVal.find(toRemove) != std::wstring::npos)
            retVal.erase(retVal.find(toRemove),1);

        return retVal;
    }

    std::string JGW_EraseChars( const std::string& sSource, const std::string& toRemove )
    {
        std::string retVal = sSource;
        while(retVal.find(toRemove) != std::string::npos)
            retVal.erase(retVal.find(toRemove),toRemove.size());

        return retVal;
    }

    void JGW_EraseAfterToRemoveChar(std::string& sSource,const char toRemove)
    {
        sSource.erase(sSource.find_last_of(toRemove) + 1);
    }

    void JGW_EraseFristToRemoveChar(std::string& sSource,const char toRemove)
    {
        sSource.erase(0,sSource.find(toRemove) + 1);
    }

    std::string JGW_EraseAfterLastQuote( std::string sSource)
    {
        sSource.erase(sSource.find_last_of("\"")+1);
        return sSource;
    }

    std::string JGW_EraseAfterNewLineChar( std::string sSource)
    {
        sSource.erase(sSource.find_last_of("\n")+1);
        return sSource;
    }

    std::wstring JGW_EraseAfterLastQuoteW( std::wstring sSource)
    {
        sSource.erase(sSource.find_last_of(L"\"")+1);
        return sSource;
    }

    std::wstring JGW_EraseCharsW( const std::wstring& sSource, const std::wstring& toRemove )
    {
        std::wstring retVal = sSource;
        while(retVal.find(toRemove) != std::string::npos)
            retVal.erase(retVal.find(toRemove),toRemove.size());

        return retVal;
    }

    void JGW_EraseAfterToRemoveCharW(std::wstring& sSource,const wchar_t toRemove)
    {
        sSource.erase(sSource.find_last_of(toRemove) + 1);
    }

    void JGW_EraseFristToRemoveCharW(std::wstring& sSource,const wchar_t toRemove)
    {
        sSource.erase(0,sSource.find(toRemove) + 1);
    }

	void JGW_EraseLastAndFristCharsA( std::string& sSource,const char toRemove /* = ' ,' */)
	{
		while ( !sSource.empty() && toRemove == sSource.at(0) )
		{
			sSource.erase(sSource.begin());
		}
		while ( !sSource.empty() && toRemove == sSource.at(sSource.length()-1) )
		{
			sSource.erase(sSource.end()-1);
		}
	}

    void JGW_EraseLastAndFristChars( std::wstring& sSource,const char toRemove /*= L','*/ )
    {
        while ( !sSource.empty() && toRemove == sSource.at(0) )
        {
            sSource.erase(sSource.begin());
        }
        while ( !sSource.empty() && toRemove == sSource.at(sSource.length()-1) )
        {
            sSource.erase(sSource.end()-1);
        }
    }

    void JGW_EraseLastAndFristTwoChars(std::string& sSource,const char toRemoveOne /* = '' */,const char toRemoveTwo /* = '' */)
    {
        while ( !sSource.empty() && (toRemoveOne == sSource.at(0) || toRemoveTwo == sSource.at(0)) )
        {
            sSource.erase(sSource.begin());
        }
        while ( !sSource.empty() && (toRemoveOne == sSource.at(sSource.length()-1) || toRemoveTwo == sSource.at(sSource.length()-1)) )
        {
            sSource.erase(sSource.end()-1);
        }
    }

    void JGW_EraseLastAndFristTwoCharsW(std::wstring& sSource,const char toRemoveOne /* = '\r' */,const char toRemoveTwo /* = '\n' */)
    {
        while ( !sSource.empty() && (toRemoveOne == sSource.at(0) || toRemoveTwo == sSource.at(0)) )
        {
            sSource.erase(sSource.begin());
        }
        while ( !sSource.empty() && (toRemoveOne == sSource.at(sSource.length()-1) || toRemoveTwo == sSource.at(sSource.length()-1)) )
        {
            sSource.erase(sSource.end()-1);
        }
    }

    std::wstring JGW_GetTimeFolder()
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::wstring strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        FormatWString(strStamp,_T("%4d-%02d-%02d"),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday);

        return strStamp;
    }

    std::string JGW_GetCurrentTimeStampYMDHMSA(const std::string& strFormate /* = "%04d%02d%02d%02d%02d%02d" */)
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::string strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        JGW_FormatString(strStamp,strFormate.c_str(),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday,tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec);

        return strStamp;
    }

    std::wstring JGW_GetCurrentTimeStampHMS(const std::wstring& strFormate /*= _T("02d%02d%02d")*/)
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::wstring strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        FormatWString(strStamp,strFormate.c_str(),tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec);

        return strStamp;
    }

    std::wstring JGW_GetCurrentTimeStampYMDHMS(const std::wstring& strFormate /* = _T("%04d%02d%02d%02d%02d%02d") */)
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::wstring strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        FormatWString(strStamp,strFormate.c_str(),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday,tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec);

        return strStamp;
    }

    std::wstring JGW_GetCPKTestStartTime()
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::wstring strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        FormatWString(strStamp,_T("%d-%d-%d %d:%d:%d"),tmTmp.tm_mon + 1,tmTmp.tm_mday,tmTmp.tm_year+1900,tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec);

        return strStamp;
    }

	std::wstring JGW_GetTimeStringToTimestampW(__int64 dw64TimeStampMSec,const std::wstring& strFormat /* = L"%04d-%02d-%02d %02d:%02d:%02d.%03d" */)
	{
		time_t t = (dw64TimeStampMSec / 1000);
		SYSTEMTIME st = JGW_TimeT2SystemTime(t);
		st.wMilliseconds = (WORD)(dw64TimeStampMSec % 1000);
		return JGW_GetFormatWString(strFormat.c_str(),st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond,st.wMilliseconds);
	}


    int gettimeofday(struct timeval *tp, void *tzp)
    {
        time_t clock;
        struct tm tm;
        SYSTEMTIME wtm;
        GetLocalTime(&wtm);
        tm.tm_year = wtm.wYear - 1900;
        tm.tm_mon = wtm.wMonth - 1;
        tm.tm_mday = wtm.wDay;
        tm.tm_hour = wtm.wHour;
        tm.tm_min = wtm.wMinute;
        tm.tm_sec = wtm.wSecond;
        tm.tm_isdst = -1;
        clock = mktime(&tm);
        tp->tv_sec = (long)clock;
        tp->tv_usec = wtm.wMilliseconds * 1000;
        return (0);
    }

    size_t JGW_GetTimeOfDay()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec;
    }

//     time_t JGW_GetTimetToTimeStamp(size_t timeStamp)
//     {
//         struct tm tmTmp;
//         time_t clock = timeStamp;
//         localtime_s(&tmTmp,&clock);
//         return clock;
//     }

    double JGW_DiffTime(size_t timeStamp1,size_t timeStamp2)
    {
        return difftime(timeStamp1,timeStamp2);
    }

    std::wstring JGW_GetCurrentTimeStamp()
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::wstring strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        FormatWString(strStamp,_T("%4d-%02d-%02d %02d:%02d:%02d"),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday,tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec);

        return strStamp;
    }

    std::wstring JGW_GetFileNameToCurrentTimeStamp()
    {
        time_t timeCur;
        struct tm tmTmp ;
        std::wstring strStamp;

        time(&timeCur);
        localtime_s(&tmTmp,&timeCur);

        FormatWString(strStamp,_T("%4d_%02d_%02d_%02d_%02d_%02d"),tmTmp.tm_year+1900,tmTmp.tm_mon+1,tmTmp.tm_mday,tmTmp.tm_hour,tmTmp.tm_min,tmTmp.tm_sec);

        return strStamp;
    }

    SYSTEMTIME JGW_TimeT2SystemTime(const time_t& t)
    {
        tm temptm = *localtime(&t);
        SYSTEMTIME st = { 1900 + temptm.tm_year
            , 1 + temptm.tm_mon 
            , temptm.tm_wday 
            , temptm.tm_mday 
            , temptm.tm_hour 
            , temptm.tm_min 
            , temptm.tm_sec 
            , 0};
        return st;
    }

    //SYSTEMTIME转time_t
    time_t JGW_SystemTime2TimeT(const SYSTEMTIME& st)
    {
        struct tm gm = {st.wSecond, st.wMinute, st.wHour, st.wDay, st.wMonth-1, st.wYear-1900, st.wDayOfWeek, 0, 0};
        return mktime(&gm);
    }

    void JGW_SetWindowTime(long timeStamp)
    {
        //! 东八区
        time_t t = timeStamp + 8 * 60 * 60;
        SYSTEMTIME st = JGW_TimeT2SystemTime(t);
        SetLocalTime(&st);
    }

    time_t JGW_GetWindowTime()
    {
        SYSTEMTIME st ={0};
        GetLocalTime(&st);
        return JGW_SystemTime2TimeT(st);
    }

    std::wstring& JGW_URLEncodeW(const std::wstring& str, std::wstring& encodedStr,const std::wstring& reserved /* = L"/" */)
    {
        std::string encodedStrA,strUrl = JGW_W2A(str),reservedA = JGW_W2A(reserved);
        JGW_URLEncodeA(strUrl,encodedStrA,reservedA);
        encodedStr = JGW_A2W(encodedStrA);
        return encodedStr;
    }

    std::wstring& JGW_URLDecodeW(const std::wstring& str, std::wstring& decodedStr,bool plusAsSpace /* = false */)
    {
        std::string decodedStrA,strUrl = JGW_W2A(str);
        JGW_URLDecodeA(strUrl,decodedStrA,plusAsSpace);
        decodedStr = JGW_A2W(decodedStrA);
        return decodedStr;
    }

    std::string& JGW_URLEncodeA(const std::string& str,  std::string& encodedStr,const std::string& reserved/* = "\\/"*/)
    {       
        const std::string ILLEGAL = "%<>{}|\\\"^`!*'()$,[]";
        for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
        {
            unsigned char c = *it;
            if ((c >= 'a' && c <= 'z') || 
                (c >= 'A' && c <= 'Z') || 
                (c >= '0' && c <= '9') ||
                c == '-' || c == '_' || 
                c == '.' || c == '~')
            {
                encodedStr += c;
            }
            else if (std::string::npos != reserved.find(c))
            {
                encodedStr += c;
            }
            else if (c <= 0x20 || c >= 0x7F || ILLEGAL.find(c) != std::string::npos)
            {
                encodedStr += JGW_GetFormatString("%%%02X",(unsigned char)c);
            }
            else encodedStr += c;
        }
        return encodedStr;
    }

    std::string& JGW_URLDecodeA(const std::string& str, std::string& decodedStr,bool plusAsSpace /* = false */)
    {
        bool inQuery = false;
        std::string::const_iterator it  = str.begin();
        std::string::const_iterator end = str.end();
        while (it != end)
        {
            char c = *it++;
            if (c == '?') inQuery = true;
            // spaces may be encoded as plus signs in the query
            if (inQuery && plusAsSpace && c == '+') c = ' ';
            else if (c == '%')
            {
                if (it == end)
                {
                    g_strErrorMsg = JGW_GetFormatWString(L"URI encoding: no hex digit following percent sign:%s",JGW_A2W(str).c_str());
                    return decodedStr;
                }
                    
                    
                char hi = *it++;
                if (it == end)
                {
                    g_strErrorMsg = JGW_GetFormatWString(L"URI encoding: two hex digits must follow percent sign:%s",JGW_A2W(str).c_str());
                    return decodedStr;
                }

                char lo = *it++;
                if (hi >= '0' && hi <= '9')
                    c = hi - '0';
                else if (hi >= 'A' && hi <= 'F')
                    c = hi - 'A' + 10;
                else if (hi >= 'a' && hi <= 'f')
                    c = hi - 'a' + 10;
                else {g_strErrorMsg = _T("URI encoding: not a hex digit"); return decodedStr;}
                c *= 16;
                if (lo >= '0' && lo <= '9')
                    c += lo - '0';
                else if (lo >= 'A' && lo <= 'F')
                    c += lo - 'A' + 10;
                else if (lo >= 'a' && lo <= 'f')
                    c += lo - 'a' + 10;
                else {g_strErrorMsg = _T("URI encoding: not a hex digit"); return decodedStr;}
            }
            decodedStr += c;
        }
        return decodedStr;
    }

    void JGW_PareserConfigString(const std::wstring& strConfig,std::map<std::wstring, std::wstring>& mapProperty)
    {
        int temp = 0,start = 0;
        bool isValue = false;
        std::wstring strKey,strValue;

        for (size_t i = 0;i < strConfig.length();i ++)
        {
            if (i == strConfig.length() - 1 && isValue)
            {
                strValue += strConfig[i];
                mapProperty[strKey] = strValue;
                continue;
            }
            //! 是否是换行
            if (L'\n'== strConfig[i])
            {
                if (strKey.empty() && strValue.empty()) 
                    continue;

                if (isValue) 
                {
                    {
                        mapProperty[strKey] = strValue;
                        strKey = L"";
                        strValue = L"";
                        isValue=false;
                        start = 0;
                        continue;
                    }
                }
            }

            //! 判断属性头
            if (0 == start)
            {
                if (L';'== strConfig[i] || L'#' == strConfig[i]) 
                {
                    while (L'\n' != strConfig[i] && i < strConfig.length()) i++;
                    i --;
                    continue;
                }

                if (L' ' == strConfig[i])
                {
                    continue;
                }
            }

            if (!isValue)
            {
                if (L'=' == strConfig[i])
                {
                    isValue = true;
                    start = 0;
                    continue;
                }
                strKey += strConfig[i];
                start ++;
            }
            else 
            {
                if (L'\r' == strConfig[i] || L'\n' == strConfig[i]) continue;
                strValue += strConfig[i];
            }
        }
    }

    std::wstring JGW_BuildLog4cplusLogFilePath(const std::wstring& strLoggerName)
    {
        std::wstring strLogFilePath;

        JGW_GetExecSubFolder(strLogFilePath,L"log",NULL);
        JGW_FormatWString(strLogFilePath,L"%s%s\\%s\\%s.log",strLogFilePath.c_str(),strLoggerName.c_str(),JGW_GetTimeFolder().c_str(),strLoggerName.c_str());
        JGW_CreateDirectory(strLogFilePath.c_str(),false);

        return strLogFilePath;
    }

    void JGW_PopulateCSVLine(std::vector<std::wstring> &record, const std::wstring& line, wchar_t delimiter/* = L','*/)
    {
        int linepos=0;
        int inquotes=false;
        wchar_t c;
        int linemax=line.length();
        std::wstring curstring;
        record.clear();

        while(line[linepos] != 0 && linepos < linemax)
        {
            c = line[linepos];
            if (!inquotes && curstring.length()==0 && c==L'"')
            {
                //beginquotechar
                inquotes=true;
            }
            else if (inquotes && c==L'"')
            {
                //quotechar
                if ( (linepos+1 <linemax) && (line[linepos+1]==L'"') )
                {
                    //encountered 2 double quotes in a row (resolves to 1 double quote)
                    curstring.push_back(c);
                    linepos++;
                }
                else
                {
                    //endquotechar
                    inquotes=false;
                }
            }
            else if (!inquotes && c==delimiter)
            {
                //end of field
                record.push_back( curstring );
                curstring=L"";
            }
            else if (!inquotes && (c==L'\r' || c==L'\n') )
            {
                record.push_back( curstring );
                return;
            }
            else
            {
                curstring.push_back(c);
            }
            linepos++;
        }
        record.push_back(curstring);
    }

    void JGW_PopulateCSVLineA(std::vector<std::string> &record, const std::string& line, char delimiter /* = ' */)
    {
        int linepos=0;
        int inquotes=false;
        char c;
        int linemax=line.length();
        std::string curstring;
        record.clear();

        while(line[linepos] != 0 && linepos < linemax)
        {
            c = line[linepos];
            if (!inquotes && curstring.length()==0 && c=='"')
            {
                //beginquotechar
                inquotes=true;
            }
            else if (inquotes && c=='"')
            {
                //quotechar
                if ( (linepos+1 <linemax) && (line[linepos+1]=='"') )
                {
                    //encountered 2 double quotes in a row (resolves to 1 double quote)
                    curstring.push_back(c);
                    linepos++;
                }
                else
                {
                    //endquotechar
                    inquotes=false;
                }
            }
            else if (!inquotes && c==delimiter)
            {
                //end of field
                record.push_back( curstring );
                curstring="";
            }
            else if (!inquotes && (c=='\r' || c=='\n') )
            {
                record.push_back( curstring );
                return;
            }
            else
            {
                curstring.push_back(c);
            }
            linepos++;
        }
        record.push_back(curstring);
    }


    void ParseStringA(const std::string& inputString, char delimiter,std::vector<std::string>& retValue)
    {
        int position = 0;
        int tokenStartPosition;
        int tokenLength;
        int delimiterPosition;
        std::string tok;
        long inLen = inputString.length();

        retValue.clear();

        while( position < inLen )
        {
            tokenStartPosition = 0;
            tokenLength = 0;
            delimiterPosition = 0;

            delimiterPosition = inputString.find_first_of( delimiter, position );

            if( delimiterPosition < 0 ) // no more delimiters
            {
                tokenStartPosition = position;
                tokenLength = inLen - tokenStartPosition;
                position += tokenLength;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition > position ) // more tokens / delims left
            {
                tokenStartPosition = position;
                tokenLength = delimiterPosition - tokenStartPosition;
                position = delimiterPosition;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition == position )    // back-to-back delimiters
            {
                tok = "";
            }

            retValue.push_back( tok );
            ++position;
        }

        // handle delimiter in the last position
        // if the last position is a delimiter as is the case in back-to-back delimiters( ex ",,,," )
        // then the position will equal the inLen
        // if position = 0 then the input string is empty and has already been handled
        if( position == inLen && position != 0 )
        {
            tok = "";
            retValue.push_back( tok );
        }
    }

    void ParseStringW(const std::wstring& inputString, wchar_t delimiter,std::vector<std::wstring>& retValue)
    {
        int position = 0;
        int tokenStartPosition;
        int tokenLength;
        int delimiterPosition;
        std::wstring tok;
        long inLen = inputString.length();

        retValue.clear();

        while( position < inLen )
        {
            tokenStartPosition = 0;
            tokenLength = 0;
            delimiterPosition = 0;

            delimiterPosition = inputString.find_first_of( delimiter, position );

            if( delimiterPosition < 0 ) // no more delimiters
            {
                tokenStartPosition = position;
                tokenLength = inLen - tokenStartPosition;
                position += tokenLength;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition > position ) // more tokens / delims left
            {
                tokenStartPosition = position;
                tokenLength = delimiterPosition - tokenStartPosition;
                position = delimiterPosition;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition == position )    // back-to-back delimiters
            {
                tok = L"";
            }

            retValue.push_back( tok );
            ++position;
        }

        // handle delimiter in the last position
        // if the last position is a delimiter as is the case in back-to-back delimiters( ex ",,,," )
        // then the position will equal the inLen
        // if position = 0 then the input string is empty and has already been handled
        if( position == inLen && position != 0 )
        {
            tok = L"";
            retValue.push_back( tok );
        }
    }

    void ParseMultiStringA(const std::string& inputString, std::vector<char>& delimiterVec,std::vector<std::string>& retValue)
    {
        int position = 0;
        int tokenStartPosition = 0;
        int tokenLength = 0;
        int delimiterPosition = 0,delimiterPosition_itr = 0;
        std::string tok;
        long inLen = inputString.length();
        std::vector<char>::iterator delimiterIter;
        retValue.clear();
        int minPosition = 0,count = 0;
        while( position < inLen )
        {
            tokenStartPosition = 0;
            tokenLength = 0;
            delimiterPosition = 0;
            count = 0;
            for( delimiterIter = delimiterVec.begin(); delimiterIter!= delimiterVec.end(); delimiterIter++,count++)
            {

                delimiterPosition_itr = inputString.find_first_of( *delimiterIter, position );
                // Making sure that the first delimiter becomes the min position.
                if(count == 0)
                {
                    minPosition = delimiterPosition_itr;
                }
                if((minPosition > 0) && (delimiterPosition_itr > 0) && (delimiterPosition_itr <  minPosition))
                {
                    minPosition = delimiterPosition_itr;
                }
                else if ( (minPosition < 0) && (delimiterPosition_itr > 0)) // This condition needs to be checked since the minPosition can be equal to -1 is not valid.
                {
                    minPosition = delimiterPosition_itr;
                }
            }

            delimiterPosition = minPosition;
            if( delimiterPosition < 0 ) // no more delimiters
            {
                tokenStartPosition = position;
                tokenLength = inLen - tokenStartPosition;
                position += tokenLength;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition > position ) // more tokens / delims left
            {
                tokenStartPosition = position;
                tokenLength = delimiterPosition - tokenStartPosition;
                position = delimiterPosition;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition == position )    // back-to-back delimiters
            {
                tok = "";
            }

            retValue.push_back( tok );
            ++position;

            // handle delimiter in the last position
            // if the last position is a delimiter as is the case in back-to-back delimiters( ex ",,,," )
            // then the position will equal the inLen
            // if position = 0 then the input string is empty and has already been handled
            if( position == inLen && position != 0 )
            {
                tok = "";
                retValue.push_back( tok );
            }
        }
    }

    void ParseMultiStringW(const std::wstring& inputString, std::vector<wchar_t>& delimiterVec,std::vector<std::wstring>& retValue)
    {
        int position = 0;
        int tokenStartPosition = 0;
        int tokenLength = 0;
        int delimiterPosition = 0,delimiterPosition_itr = 0;
        std::wstring tok;
        long inLen = inputString.length();
        std::vector<wchar_t>::iterator delimiterIter;
        retValue.clear();
        int minPosition = 0,count = 0;
        while( position < inLen )
        {
            tokenStartPosition = 0;
            tokenLength = 0;
            delimiterPosition = 0;
            count = 0;
            for( delimiterIter = delimiterVec.begin(); delimiterIter!= delimiterVec.end(); delimiterIter++,count++)
            {

                delimiterPosition_itr = inputString.find_first_of( *delimiterIter, position );
                // Making sure that the first delimiter becomes the min position.
                if(count == 0)
                {
                    minPosition = delimiterPosition_itr;
                }
                if((minPosition > 0) && (delimiterPosition_itr > 0) && (delimiterPosition_itr <  minPosition))
                {
                    minPosition = delimiterPosition_itr;
                }
                else if ( (minPosition < 0) && (delimiterPosition_itr > 0)) // This condition needs to be checked since the minPosition can be equal to -1 is not valid.
                {
                    minPosition = delimiterPosition_itr;
                }
            }

            delimiterPosition = minPosition;
            if( delimiterPosition < 0 ) // no more delimiters
            {
                tokenStartPosition = position;
                tokenLength = inLen - tokenStartPosition;
                position += tokenLength;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition > position ) // more tokens / delims left
            {
                tokenStartPosition = position;
                tokenLength = delimiterPosition - tokenStartPosition;
                position = delimiterPosition;
                tok = inputString.substr( tokenStartPosition, tokenLength );
            }
            else if( delimiterPosition == position )    // back-to-back delimiters
            {
                tok = L"";
            }

            retValue.push_back( tok );
            ++position;

            // handle delimiter in the last position
            // if the last position is a delimiter as is the case in back-to-back delimiters( ex ",,,," )
            // then the position will equal the inLen
            // if position = 0 then the input string is empty and has already been handled
            if( position == inLen && position != 0 )
            {
                tok = L"";
                retValue.push_back( tok );
            }
        }
    }

	int JGW_GetJsonIntICaseFieldDef(const std::string& strNotif,const std::string& strField,int def)
	{
		std::string strFloat = JGW_GetICaseSubStrToStartEndCharArray(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),",}\n");
		if (strFloat.empty()) return def;
		if (NULL != JGW_StrCaseStr(strFloat.c_str(),"NaN")) return def;
		return atoi(strFloat.c_str());
	}

	int JGW_GetJsonIntICaseFieldDefW(const std::wstring& strNotif,const std::wstring& strField,int def)
	{
		std::wstring strFloat = JGW_GetICaseSubStrToStartEndCharArrayW(strNotif,JGW_GetFormatWString(L"\"%s\":",strField.c_str()),L",}\n");
		if (strFloat.empty()) return def;
		if (NULL != JGW_WStrCaseWStr(strFloat.c_str(),L"NaN")) return def;
		return _ttoi(strFloat.c_str());
	}

	std::string JGW_GetJsonICaseStringFieldDef(const std::string& strNotif,const std::string& strField,const std::string& strDes /* = "NaN" */)
	{
		std::string strValue = JGW_GetICaseSubStrToStartEndStr(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),"\",");
		if (strValue.empty()) strValue = JGW_GetICaseSubStrToStartEndStr(strNotif,JGW_GetFormatString("\"%s\":",strField.c_str()),"}");
		JGW_EraseLastAndFristCharsA(strValue,'"');
		if (strValue.empty()) strValue = strDes;
		return strValue;
	}

	std::wstring JGW_GetJsonICaseStringFieldDefW(const std::wstring& strNotif,const std::wstring& strField,const std::wstring& strDes /* = L"NaN" */)
	{
		std::wstring strValue =  JGW_GetICaseSubStrToStartEndStrW(strNotif,JGW_GetFormatWString(L"\"%s\":",strField.c_str()),L"\",");
		//JGW_ReplaceStringW(strValue,L"\"",L"");
		JGW_EraseLastAndFristChars(strValue,'"');
		if (strValue.empty()) strValue = strDes;
		return strValue;
	}
}

double VoltsTodB( double volts )
{
    double dB = -999;
    if(volts > 0)
        dB = 20.0*log10( volts );
    return dB;
}


double dBToVolts( double dB )
{
    double volts = 0;
    volts = pow(10,dB/20.00);
    return volts;
}
