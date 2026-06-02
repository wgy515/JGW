// stdafx.cpp : 只包括标准包含文件的源文件
// CTSEAPSMultiUpgradePlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <string>
#include "CTSEAPS_UpgradeDefine.h"

#include <JGW_FoundationFunc/JGW_StringFunc.h>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
namespace JGW
{
    std::wstring g_strDownloadDubugLog[DOWNLOAD_THREAD_MAX];


    void close_file(FILE* fd)
    {
        if (fd) fclose (fd);
    }

    void MyLog (int log_level,int nIndex, const wchar_t *func_name, int line_number, const wchar_t *format, ...)
    {
        //! if (log_level >= LOG_DEBUG) return ;
        std::wstring strLog;
        FORMAT_WSTRING(strLog,format);

        g_strDownloadDubugLog[nIndex] += strLog.c_str();

        if (log_level <= LOG_ERROR) 
        {
            std::wstring strTemp;
            JGW::JGW_FormatWString(strTemp,L"[%d] %s",nIndex,strLog.c_str());
            Log4WE(strTemp.c_str());
        }
    }
}