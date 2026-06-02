#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/TimeUtils.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

namespace JGW
{
    int64_t CTimeUtils::GetCurrentTimestamp()
    {
        int64_t timeStamp = 0;
        (void)::GetSystemTimeAsFileTime((FILETIME*)&timeStamp);
        return (timeStamp - 116444736000000000) / 10000;
    }

    _tstring CTimeUtils::GetCurrentDateString()
    {
        TCHAR szBuf[MAX_PATH] = { 0 };

        SYSTEMTIME st = { 0 };
        (void)::GetLocalTime(&st);

        ::StringCchPrintf(szBuf, _countof(szBuf),
            _T("%04d-%02d-%02d"),
            st.wYear, st.wMonth, st.wDay
            );

        return szBuf;
    }

    _tstring CTimeUtils::GetCurrentTimeString(bool bHasDate/* = true*/)
    {
        TCHAR szBuf[MAX_PATH] = { 0 };

        SYSTEMTIME st = { 0 };
        (void)::GetLocalTime(&st);

        if (bHasDate)
        {
            ::StringCchPrintf(szBuf, _countof(szBuf),
                _T("%04d-%02d-%02d %02d:%02d:%02d.%d"),
                st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
                );
        }
        else
        {
            ::StringCchPrintf(szBuf, _countof(szBuf),
                _T("%02d:%02d:%02d.%d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
                );
        }

        return szBuf;
    }

    _tstring CTimeUtils::TimestampToString(int64_t timestamp, bool bHasDate/* = true*/)
    {
        TCHAR szBuf[MAX_PATH] = { 0 };
        SYSTEMTIME st = { 0 };
        FILETIME ftFile = { 0 };
        FILETIME ftLocal = { 0 };

        timestamp = timestamp * 10000 + 116444736000000000;

        ftFile.dwLowDateTime = timestamp & 0xFFFFFFFF;
        ftFile.dwHighDateTime = timestamp >> 32;

        ::FileTimeToLocalFileTime(&ftFile, &ftLocal);
        ::FileTimeToSystemTime(&ftLocal, &st);

        if (bHasDate)
        {
            ::StringCchPrintf(szBuf, _countof(szBuf),
                _T("%04d-%02d-%02d %02d:%02d:%02d.%d"),
                st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
                );
        }
        else
        {
            ::StringCchPrintf(szBuf, _countof(szBuf),
                _T("%02d:%02d:%02d.%d"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds
                );
        }

        return szBuf;
    }

    int64_t CTimeUtils::StringToTimestamp(const _tstring& timeString)
    {
        FILETIME ftFile = { 0 };
        FILETIME ftLocal = { 0 };
        SYSTEMTIME st = { 0 };

        ::_stscanf_s(timeString.c_str(), _T("%hd-%hd-%hd %hd:%hd:%hd.%hd"),
            &st.wYear,
            &st.wMonth,
            &st.wDay,
            &st.wHour,
            &st.wMinute,
            &st.wSecond,
            &st.wMilliseconds
            );

        ::SystemTimeToFileTime(&st, &ftLocal);
        ::LocalFileTimeToFileTime(&ftLocal, &ftFile);

        int64_t timeStamp = ((int64_t)ftFile.dwHighDateTime << 32) | ftFile.dwLowDateTime;
        return (timeStamp - 116444736000000000) / 10000;
    }

    uint64_t CTimeUtils::GetCurrentTickCount()
    {
        return ::GetTickCount64();
    }
}
