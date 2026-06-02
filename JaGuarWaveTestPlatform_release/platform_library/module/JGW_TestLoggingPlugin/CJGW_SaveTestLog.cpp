#include "StdAfx.h"
#include <JGW_TestLoggingPlugin/CJGW_SaveTestLog.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <fstream>
#include <regex>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <sstream>

namespace JGW
{
    ///
    const wchar_t g_szBarcodeStartFlag[] = L"TSE_BARCODE: ";
    ///TSE_BARCODE:@123456789@@
    const wchar_t g_szBarcodeEndFlag[] = L"  ";

    CCJGW_SaveTestLog::CCJGW_SaveTestLog(void)
    {
    }


    CCJGW_SaveTestLog::~CCJGW_SaveTestLog(void)
    {
    }

    std::wstring CCJGW_SaveTestLog::GetBarcodeToTestLog(const std::wstring& strTestLog,const std::wstring& strStart,const std::wstring& strEnd,const std::wstring& strDefaultBarcode /* = L"" */)
    {
        std::wstring strRsp = L"";
        size_t index = strTestLog.length(),endIndex = strTestLog.length() + 1,startIndex = 0;

        startIndex = index = strTestLog.rfind(strStart.c_str(),index - 1);
        endIndex = strTestLog.find(strEnd,index + strStart.length());
        while (std::wstring::npos != index && std::wstring::npos != endIndex && index < endIndex)
        {
            strRsp = strTestLog.substr(index + strStart.length(),endIndex - index - strStart.length());
            JGW::JGW_EraseLastAndFristTwoCharsW(strRsp,L' ');
            std::wregex partten(L"[0-9A-Za-z]{8,25}");
            if (regex_match(strRsp,partten))
            {
                return strRsp;
            }
            if (index <= 0)
            {
                break;
            }
            index = strTestLog.rfind(strStart.c_str(),index - 1);
            endIndex = strTestLog.find(strEnd,index + strStart.length());
            if (startIndex == index)
            {
                break;
            }
        }
        return L"";
        /*size_t index = strTestLog.rfind(g_szBarcodeStartFlag);
        size_t endIndex = strTestLog.find(g_szBarcodeEndFlag,index + strStart.length());

        if (std::wstring::npos != index && std::wstring::npos != endIndex && index < endIndex)
        {
        strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
        JGW_EraseLastAndFristTwoCharsW(strRsp,L' ');
        }
        return strRsp;

        std::wregex partten(it->second);
        if (!regex_match(strValue,partten))
        {
        LogE_F(L"param:%s,rule:%s,value:%s,regex_match fail",vstrEnvName[i].c_str(),it->second.c_str(),strValue.c_str());
        return false;
        }*/


        //return JGW_GetRSubStrToStartEndStrW(strTestLog,g_szBarcodeStartFlag,g_szBarcodeEndFlag,strDefaultBarcode);
    }

    bool CCJGW_SaveTestLog::SaveTestLogGetFilePath(const std::wstring& strBaseFolder,const std::wstring& strBarcode,bool bTestResult,const std::wstring& strTestLog,std::wstring& strTestLogPath)
    {
        std::wofstream fileStream;
        std::wstring strSaveLogPath,strBarcodeVal (strBarcode);

        if (strBarcode.empty())
        {
            strBarcodeVal = GetBarcodeToTestLog(strTestLog,g_szBarcodeStartFlag,g_szBarcodeEndFlag);
        }
        JGW_FormatWString(strSaveLogPath,L"%s\\%s\\%s_%s_%u.log",strBaseFolder.c_str(),JGW_GetTimeFolder().c_str(),bTestResult?L"P":L"F",strBarcodeVal.c_str(),JGW_GetTimeOfDay());
        //! GetTickCount
        JGW_CreateDirectory(strSaveLogPath.c_str(),false);
        strTestLogPath = strSaveLogPath;
#if 0
        fileStream.imbue(std::locale(""));
        fileStream.open(strSaveLogPath.c_str(),std::ios_base::out | std::ios_base::trunc );
        if (!fileStream.is_open()) return false;

        fileStream << strTestLog;
        fileStream.flush();
        fileStream.close();
#else
        std::string filePath = JGW_W2A(strSaveLogPath,CP_UTF8);
        Poco::FileOutputStream fos (filePath);
        std::stringstream ss (JGW_W2A(strTestLog,CP_UTF8));
        Poco::StreamCopier::copyStream(ss, fos);
        fos.flush();
#endif
        return true;
    }

    bool CCJGW_SaveTestLog::SaveTestLog(const std::wstring& strBaseFolder,const std::wstring& strBarcode,bool bTestResult,const std::wstring& strTestLog)
    {
        std::wstring strSaveLogPath;
        return SaveTestLogGetFilePath(strBaseFolder,strBarcode,bTestResult,strTestLog,strSaveLogPath);
#if 0
        std::wofstream fileStream;
        std::wstring strSaveLogPath,strBarcodeVal (strBarcode);

        if (strBarcode.empty())
        {
            strBarcodeVal = GetBarcodeToTestLog(strTestLog,g_szBarcodeStartFlag,g_szBarcodeEndFlag);
        }
        JGW_FormatWString(strSaveLogPath,L"%s\\%s\\%s_%s_%u.log",strBaseFolder.c_str(),JGW_GetTimeFolder().c_str(),bTestResult?L"P":L"F",strBarcodeVal.c_str(),JGW_GetTimeOfDay());
        //! GetTickCount
        JGW_CreateDirectory(strSaveLogPath.c_str(),false);
#if 0
        fileStream.imbue(std::locale(""));
        fileStream.open(strSaveLogPath.c_str(),std::ios_base::out | std::ios_base::trunc );
        if (!fileStream.is_open()) return false;

        fileStream << strTestLog;
        fileStream.flush();
        fileStream.close();
#else
        std::string filePath = JGW_W2A(strSaveLogPath,CP_UTF8);
        Poco::FileOutputStream fos (filePath);
        std::stringstream ss (JGW_W2A(strTestLog,CP_UTF8));
        Poco::StreamCopier::copyStream(ss, fos);
        fos.flush();
#endif
        return true;
#endif
    }
}
