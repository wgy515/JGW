// stdafx.cpp : 只包括标准包含文件的源文件
// JGW_PumaDownLoad.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <iostream>
#include "PumaDownload_Define.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用
namespace JGW
{
    void MyLog (int log_level,int nIndex, const char *func_name, int line_number, const char *format, ...)
    {
#if 0
        //! if (log_level >= LOG_DEBUG) return ;
        std::string strLog("");
        FORMAT_STRING(strLog,format);

        g_strDownloadDubugLog[nIndex] += strLog.c_str();

        if (log_level <= LOG_ERROR) 
        {
            std::string strTemp;
            JGW::JGW_FormatString(strTemp,"[%d] %s",nIndex,strLog.c_str());
            Log4AE(strTemp.c_str());
        }
#else
        std::string strLog("");
        FORMAT_STRING(strLog,format);
        std::cout << strLog;
#endif
    }
}
