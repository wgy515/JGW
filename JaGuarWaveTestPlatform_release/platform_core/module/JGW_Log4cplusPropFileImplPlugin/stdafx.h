// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>



// TODO: 在此处引用程序需要的其他头文件
typedef struct
{
    char* buffer; //! The actual ring buffer
    boost::mutex mutex; //! The mutex that protects the @buffer
    size_t w_off; //! The current write head offset  基于头的偏移
    size_t head; //! The head, or location that readers start reading at.
    size_t size; //! The size of the log buffer 大小
    //struct list_head	readers;
}logger_log,*plogger_log;

//!struct logger_entry;

struct logger_entry
{
    UINT16 len; //! The length of the payload
    UINT16 hdr_size; //! sizeof(struct logger_entry_v2)
    INT32 pid; //! The generating process' process ID
    INT32 tid; //! The generating process' thread ID
    INT32 sec; //! The number of seconds that have elapsed since the Epoch
    INT32 nsec; //! The number of nanoseconds that have elapsed since @sec
    //UINT32 next;
    wchar_t msg[1]; //! The message that is to be logged
};

struct logger_reader 
{
    logger_log* log;
    size_t	r_off;//! The current read head offset.
    bool	r_all;//! Reader can read all entries
    //! std::wstring strReaderBuf;
};
// struct logger_entry_test
// {
//     UINT16 len; //! The length of the payload
//     UINT16 hdr_size; //! sizeof(struct logger_entry_v2)
//     INT32 pid; //! The generating process' process ID
//     INT32 tid; //! The generating process' thread ID
//     INT32 sec; //! The number of seconds that have elapsed since the Epoch
//     INT32 nsec; //! The number of nanoseconds that have elapsed since @sec
//     logger_entry* next;
// };

#define LOGGER_ENTRY_MAX_PAYLOAD 4076
#define LOGGER_ENTRY_STRUCT_SIZE 20