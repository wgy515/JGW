#pragma once

#include <string>
#include <stdint.h>
#include "JGW_WindowsFuncPlugin_Define.h"
#ifdef _UNICODE
typedef std::wstring _tstring;
//using _tstring = std::wstring;
#else
//using _tstring = std::string;
typedef std::string _tstring;
#endif

//1   秒       second = 1000 毫秒 millisecond
//1 毫秒  millisecond = 1000 微妙 microsecond
//1 微妙  microsecond = 1000 纳秒 nanosecond
namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CTimeUtils
    {
    public:

        //
        // @brief: 获取当前时间戳
        // @param: void
        // @ret: 时间戳(单位: 毫秒)    如: 1697017380617
        static int64_t GetCurrentTimestamp();

        //
        // @brief: 获取当前日期字符串
        // @param: void
        // @ret: 时间戳字符串          如: 2023-10-11
        static _tstring GetCurrentDateString();

        //
        // @brief: 获取当前时间戳字符串
        // @param: bHasDate            是否包含日期
        // @ret: 时间戳字符串          如: 
        //                              包含日期: 2023-10-11 17:43:00.617
        //                              不包含日期: 17:43:00.617
        static _tstring GetCurrentTimeString(bool bHasDate = true);

        //
        // @brief: 时间戳转字符串  
        // @param: timestamp    时间戳 如: 1697017380617
        // @param: bHasDate            是否包含日期
        // @ret: 时间字符串            如: 2023-10-11 17:43:00.617
        static _tstring TimestampToString(int64_t timestamp, bool bHasDate = true);

        //
        // @brief: 字符串转时间戳
        // @param: timeString   时间字符串, 如: 2023-10-11 17:43:00.617
        // @ret: 时间戳(单位: 毫秒)         如: 1697017380617
        static int64_t StringToTimestamp(const _tstring& timeString);

        //
        // @brief: 获取系统启动以来经过的毫秒数
        // @param: void
        // @ret: 毫秒数
        static uint64_t GetCurrentTickCount();
    };
}
