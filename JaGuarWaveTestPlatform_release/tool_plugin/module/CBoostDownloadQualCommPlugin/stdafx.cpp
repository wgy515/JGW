// stdafx.cpp : 只包括标准包含文件的源文件
// CBoostDownloadQualCommPlugin.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include "CBoostDownload_Define.h"
#include "download_define.h"
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
    // TODO: 在 STDAFX.H 中
    // 引用任何所需的附加头文件，而不是在此文件中引用
    SIZE_T_64 CopyString (char *Dest, char *Source, SIZE_T_64  Dstart, SIZE_T_64  Sstart, SIZE_T_64  length, SIZE_T_64 DestSize, SIZE_T_64 SourceSize)
    {
        SIZE_T_64  i = Dstart + length;
        char Temp;

        // WARNING, don't use dbg(LOG_INFO,etc) in this function or you get infinite recurrsion!!
        // WARNING, don't use dbg(LOG_INFO,etc) in this function or you get infinite recurrsion!!
        // WARNING, don't use dbg(LOG_INFO,etc) in this function or you get infinite recurrsion!!

        //if(length>=6000)
        //  printf("\nlength=%"SIZE_T_FORMAT" CopyString String of length %"SIZE_T_FORMAT" and DESTINATION Array of length %"SIZE_T_FORMAT,length,Dstart+length-1,DestSize);

        if (length == 0)
        {
            // This is a case like filename="", and so user is copying a null string
            Dest[Dstart] = '\0';    // NULL
            return 1;
        }

        if (Dest == '\0')
        {
            dbg (LOG_ERROR,0,L"CopyString Dest is NULL");  // Dest is null
            return -1;
        }

        if (Source == '\0')
        {
            dbg (LOG_ERROR,0,L"CopyString Source is NULL");  // Source is null
            return -1;
        }

        if (Sstart + length > SourceSize)
        {
            dbg (LOG_ERROR,0,_T("CopyString Range of %lld exceeds Array of length %lld"), Sstart + length, SourceSize);
            return 0;
        } // range to copy is beyond source string

        if (i > 0 && (i - 1) > DestSize)
        {
            dbg (LOG_ERROR,0,_T("\ni=%lld, (i-1)=%lld and DestSize=%lld"), i, i - 1, DestSize);
            dbg (LOG_ERROR,0,_T("\nCopyString String of length %lld at offset %lld of *dest will exceed Array of length %lld by %lld bytes"), length, Dstart, DestSize, (length + Dstart - 1) - DestSize);
            //ExitAndShowLog(1);
            return 0;
        } // string to copy over is too big for destination

        //printf("CopyString(0x%X, 0x%X, %"SIZE_T_FORMAT", %"SIZE_T_FORMAT", %"SIZE_T_FORMAT", %"SIZE_T_FORMAT", %"SIZE_T_FORMAT")",Dest,Source,Dstart,Sstart,length,DestSize,SourceSize);
        if (Source == Dest)
            return length;  // user passed same buffer as source and destination, therefore nothing to copy

        for (i = 0; i < length; i++)
        {
            Temp = * (Source + Sstart + i);

            * (Dest + Dstart + i) = Temp;
        } // end i

        * (Dest + Dstart + i) = '\0'; // NULL

        return length;
    }
}
