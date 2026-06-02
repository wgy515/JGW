// stdafx.cpp : 只包括标准包含文件的源文件
// PTP4100_IperfTest.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <Poco/UnicodeConverter.h>
#include <poco/Path.h>
#include <Poco/Buffer.h>
// TODO: 在 STDAFX.H 中
// 引用任何所需的附加头文件，而不是在此文件中引用

Poco::SingletonHolder<JGW::ILog> gLog;
namespace JGW
{
    std::wstring UTF8ToLocal(const std::string& utf8String)
    {
        return A2W(utf8String,CP_UTF8);
    }

    std::wstring A2W(const std::string& localString,int codepage /* = CP_ACP */)
    {
        std::wstring wstr;
        if ( localString.length() > 0)
        {
            int wlen = MultiByteToWideChar(codepage, 0, localString.c_str(), localString.size(), NULL, 0);
            wstr.resize(wlen);
            MultiByteToWideChar(codepage, 0, localString.c_str(), localString.size(),const_cast<wchar_t*>(wstr.data()), wlen);
        }
        return wstr;
    }

    std::string LocalToUTF8(const std::wstring& localString)
    {
        return W2A(localString,CP_UTF8);
    }

    std::string W2A(const std::wstring& localString,int codepage /* = CP_ACP */)
    {
        std::string str;

        if ( localString.length() > 0)
        {
            int len = WideCharToMultiByte(codepage, 0, localString.c_str(), localString.length(), NULL, 0, NULL, NULL);
            str.resize(len);
            WideCharToMultiByte(codepage, 0, localString.c_str(), localString.length(),const_cast<char*>(str.data()), len, NULL, NULL);
        }

        return str;
    }

    std::wstring GetModuleFolderW(HMODULE hModule)
    {
        DWORD len = MAX_PATH + 1;
        std::wstring buffer;
        buffer.resize(len);
        DWORD n = GetModuleFileNameW(hModule, const_cast<wchar_t*>(buffer.data()),len);
        if (n > 0 && n <= len)
        {
            while (!buffer.empty() && buffer[buffer.size() - 1] != '\\') buffer.erase(buffer.size() - 1);
            return buffer;
        }
        return L"";
    }

    std::wstring GetApplicationFolderW()
    {
        return GetModuleFolderW(NULL);
    }

    std::string GetApplicationFolder()
    {
        return W2A(GetApplicationFolderW());
    }

    std::wstring GetAbsolutePath(const std::wstring& relativePath,const wchar_t* strBaseFolder)
    {
        std::string relativePathUTF8;
        std::wstring relativePathGB2312 (relativePath);
        relativePathUTF8 = LocalToUTF8(relativePathGB2312);
        //Poco::UnicodeConverter::convert(relativePathGB2312, relativePathUTF8);
        Poco::Path path(relativePathUTF8);
        if (path.isRelative())
        {
            relativePathGB2312 = NULL == strBaseFolder ? GetApplicationFolderW() : strBaseFolder;
            relativePathGB2312 += relativePath;
        }
        return relativePathGB2312;
    }

    void EraseChars(std::string& sSource,std::vector<char> vRemoveChars)
    {
        while ( !sSource.empty() && IsFindCharVector(sSource[0],vRemoveChars) )
        {
            sSource.erase(sSource.begin());
        }
        while ( !sSource.empty() && IsFindCharVector(sSource[sSource.length() - 1],vRemoveChars))
        {
            sSource.erase(sSource.end()-1);
        }
    }

    inline std::string FormatString(std::string& strFormat,const char *szFormat,...)
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

    inline void FormatWString(std::wstring& strFormat,const wchar_t *szFormat,...)
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


    size_t GetInsertUsbDeviceCountToServicesName(const wchar_t* strServicesName)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\services\\") ;
        DWORD			dwType = REG_DWORD;
        DWORD			byData = 0;
        DWORD			dwSize = sizeof(DWORD);
        HKEY			m_hKey = NULL;

        strSubKey += strServicesName;
        strSubKey += _T("\\Enum");

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return 0;
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, _T("Count"), NULL, &dwType,(LPBYTE) (&byData), &dwSize) )
        {
            RegCloseKey(m_hKey);
            return 0;
        }

        RegCloseKey(m_hKey);

        return byData;
    }

    std::wstring GetUsbEnumPidVidToServiceName(const wchar_t* strServicesName,DWORD dwEnumID)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\services\\") ;
        DWORD			dwType = REG_DWORD;
        TCHAR			byData [MAX_PATH ]= {0};
        TCHAR			lpValueName[9] = {0};
        DWORD			dwSize = 260;
        HKEY			m_hKey = NULL;

        strSubKey += strServicesName;
        strSubKey += _T("\\Enum");

        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return _T("");
        }
        _stprintf_s(lpValueName,_T("%ld"),dwEnumID);
        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, lpValueName, NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return _T("");
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }

    std::wstring GetRegUsbInfoToTag(const wchar_t* strRegUsbPidVid,const wchar_t* tag)
    {
        std::wstring	strSubKey = _T("SYSTEM\\CurrentControlSet\\Enum\\");
        DWORD			dwType = REG_SZ;
        TCHAR			byData [512 ]= {0};
        DWORD			dwSize = 512;
        HKEY			m_hKey = NULL;

        strSubKey += strRegUsbPidVid;
        if( ERROR_SUCCESS != RegOpenKeyEx(HKEY_LOCAL_MACHINE, strSubKey.c_str(), 0,KEY_QUERY_VALUE, &m_hKey) )
        {
            return L"";
        }

        if ( ERROR_SUCCESS != RegQueryValueEx(m_hKey, tag, NULL, &dwType,(LPBYTE) byData, &dwSize) )
        {
            RegCloseKey(m_hKey);
            return L"";
        }

        RegCloseKey(m_hKey);

        return std::wstring(byData);
    }

    std::string GetSubStrToStartEndStr(const std::string& strSource,const std::string& strStart,const std::string& strEnd /* = L"" */,const std::string& strDef /* = L"" */)
    {
        std::string strRsp = strDef;
        size_t index = strSource.find(strStart);
        size_t endIndex = strSource.find(strEnd,index + strStart.length());

        if (std::string::npos != index && std::string::npos != endIndex)
        {
            strRsp = strSource.substr(index + strStart.length(),endIndex - index - strStart.length());
        }
        return strRsp;
    }
}

typedef  unsigned short uint16_t;
typedef  unsigned char uint8_t;

//! reg_values[6] = reg_values[47:0]
void spi_serial_command_format(uint16_t reg_address,uint8_t reg_values[6],uint8_t* spi_datas) 
{
    spi_datas[0] = 0x00;
    spi_datas[0] = (uint8_t)((reg_address & 0x3FF) >> 4);
    spi_datas[1] = (uint8_t)((reg_address & 0xF) << 4);
    spi_datas[1] |= reg_values[0] >> 4;
    spi_datas[2] = (reg_values[0] & 0xF) << 4;
    spi_datas[2] |= reg_values[1] >> 4;
    spi_datas[3] = (reg_values[1] & 0xF) << 4;
    spi_datas[3] |= reg_values[2] >> 4;
    spi_datas[4] = (uint8_t)((reg_values[2] & 0xF) << 4);
    spi_datas[4] |= reg_values[3] >> 4;
    spi_datas[5] = (uint8_t)((reg_values[3] & 0xF) << 4);
    spi_datas[5] |= reg_values[4] >> 4;
    spi_datas[6] = (uint8_t)((reg_values[4] & 0xF) << 4);
    spi_datas[6] |= reg_values[5] >> 4;
    spi_datas[7] = (uint8_t)((reg_values[5] & 0xF) << 4);
}

void dual_spi_serial_command_format(uint16_t reg_address_one,uint8_t reg_values_one[6],uint16_t reg_address_two,uint8_t reg_values_two[6],uint8_t* spi_datas)
{
    spi_serial_command_format(reg_address_one,reg_values_one,spi_datas);
    spi_datas[7] &= 0xF3;
    spi_datas[7] |= (uint8_t)((reg_address_two & 0x300) >> 8);
    spi_datas[8] = (uint8_t)(reg_address_two & 0xFF);
    memcpy(&spi_datas[9],reg_values_two,6);
}

#define RING_BUFFER_SIZE 2048 * 4
#define RING_MAX_BLOCK_SIZE 0xFF
typedef void (* InCallBackFunc)(void);
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef int int32_t;

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#define LOGGER_ENTRY_MAX_PAYLOAD RING_MAX_BLOCK_SIZE
#define LOGGER_ENTRY_STRUCT_SIZE 4
#define size_t uint32_t


typedef struct
{
    uint8_t* buffer; //! The actual ring buffer
    uint32_t w_off; //! The current write head offset  基于头的偏移
    uint32_t head; //! The head, or location that readers start reading at.
    uint32_t size; //! The size of the log buffer 大小
    //struct list_head	readers;
}logger_log;

typedef struct 
{
    uint16_t len; //! The length of the payload
    uint16_t hdr_size; //! sizeof(struct logger_entry_v2)
    uint8_t msg[1]; //! The message that is to be logged
}logger_entry;

typedef struct 
{
    logger_log* log;
    uint32_t r_off;//! The current read head offset.
    uint8_t	r_all;//! Reader can read all entries
}logger_reader;

//logger_reader *reader;

/* logger_offset - returns index 'n' into the log via (optimized) modulus */
uint32_t logger_offset(logger_log *log, uint32_t n)
{
    return n & (log->size - 1);
}
/*
* get_entry_header - returns a pointer to the logger_entry header within
* 'log' starting at offset 'off'. A temporary logger_entry 'scratch' must
* be provided. Typically the return value will be a pointer within
* 'logger->buf'.  However, a pointer to 'scratch' may be returned if
* the log entry spans the end and beginning of the circular buffer.
*/
logger_entry *get_entry_header(logger_log *log,
    size_t off, logger_entry *scratch)
{
    size_t len = min(LOGGER_ENTRY_STRUCT_SIZE, log->size - off);
    if (len != LOGGER_ENTRY_STRUCT_SIZE) {
        memcpy(((void *) scratch), log->buffer + off, len);
        memcpy(((char *) scratch) + len, log->buffer,
            LOGGER_ENTRY_STRUCT_SIZE - len);
        return scratch;
    }

    return (logger_entry *) (log->buffer + off);
}

/*
* get_entry_msg_len - Grabs the length of the message of the entry
* starting from from 'off'.
*
* An entry length is 2 bytes (16 bits) in host endian order.
* In the log, the length does not include the size of the log entry structure.
* This function returns the size including the log entry structure.
*
* Caller needs to hold log->mutex.
*/
uint32_t get_entry_msg_len(logger_log *log, size_t off)
{
    logger_entry scratch;
    logger_entry *entry;

    entry = get_entry_header(log, off, &scratch);
    return entry->len;
}

uint8_t is_between(uint32_t a, uint32_t b, uint32_t c)
{
    if (a < b) {
        /* is c between a and b? */
        if (a < c && c <= b)
            return 1;
    } else {
        /* is c outside of b through a? */
        if (c <= b || a < c)
            return 1;
    }

    return 0;
}

/*
* get_next_entry - return the offset of the first valid entry at least 'len'
* bytes after 'off'.
*
* Caller must hold log->mutex.
*/
static size_t get_next_entry(logger_log *log, size_t off, size_t len)
{
    size_t count = 0;

    do {
        size_t nr = LOGGER_ENTRY_STRUCT_SIZE +
            get_entry_msg_len(log, off);
        off = logger_offset(log, off + nr);
        count += nr;
    } while (count < len);

    return off;
}
/*
* do_write_log - writes 'len' bytes from 'buf' to 'log'
*
* The caller needs to hold log->mutex.
*/
void do_write_log(logger_log *log, uint8_t *buf, uint8_t count)
{
    uint32_t len;

    len = min(count, log->size - log->w_off);
    memcpy(log->buffer + log->w_off, buf, len);
    //!memcpy(log->buffer[log->w_off], buf, len);

    if (count != len)
        memcpy(log->buffer, buf + len, count - len);

    log->w_off = logger_offset(log, log->w_off + count);
}

/*
* do_write_log_user - writes 'len' bytes from the user-space buffer 'buf' to
* the log 'log'
*
* The caller needs to hold log->mutex.
*
* Returns 'count' on success, negative error code on failure.
*/
int32_t do_write_log_from_user(logger_log *log,uint8_t *buf, size_t count)
{
    uint32_t len;

    len = min(count, log->size - log->w_off);
    if (len && !memcpy(log->buffer + log->w_off, buf, len))
        return -1;

    if (count != len)
        if (!memcpy(log->buffer, buf + len, count - len))
            /*
            * Note that by not updating w_off, this abandons the
            * portion of the new entry that *was* successfully
            * copied, just above.  This is intentional to avoid
            * message corruption from missing fragments.
            */
            return -1;

    log->w_off = logger_offset(log, log->w_off + count);

    return count;
}

/*
* fix_up_readers - walk the list of all readers and "fix up" any who were
* lapped by the writer; also do the same for the default "start head".
* We do this by "pulling forward" the readers and start head to the first
* entry after the new write head.
*
* The caller needs to hold log->mutex.
*/
void fix_up_readers(logger_log *log, size_t len,logger_reader *reader)
{
    size_t old = log->w_off;
    size_t new1 = logger_offset(log, old + len);
	
	if (is_between(old, new1, log->head))
        log->head = get_next_entry(log, log->head, len);

    if (is_between(old, new1, reader->r_off))
        reader->r_off = get_next_entry(log, reader->r_off, len);

    /*
    list_for_each_entry(reader, &log->readers, list)
    if (is_between(old, new1, reader->r_off))
    reader->r_off = get_next_entry(log, reader->r_off, len);*/
}


uint8_t RingBufferInit(logger_log *logger_log_p, uint32_t buf_size,InCallBackFunc callBack)
{
#if 0
    logger_log_p->buffer = (uint8_t *)malloc(buf_size);
    if (logger_log_p->buffer) {
        memset(logger_log_p->buffer, 0, buf_size);
    } else {
        return 0;
    }
    logger_log_p->size = buf_size;
    logger_log_p->head = 0;
    logger_log_p->w_off = 0;
    // logger_log_p->InCallBack = callBack;
    return 1;
#else
    // buf size 必须是4的倍数
    logger_log_p->size = buf_size - buf_size % 4;
    logger_log_p->buffer = (uint8_t *)malloc(logger_log_p->size);
    if (logger_log_p->buffer) {
        memset(logger_log_p->buffer, 0, logger_log_p->size);
    } else {
        return 0;
    } 
    logger_log_p->head = 0;
    logger_log_p->w_off = 0;
    //logger_log_p->InCallBack = callBack;
    return 1;
#endif
}

uint8_t RingBufferPut(logger_log *logger_log_p, uint8_t *buf, uint8_t len)
{
	logger_entry header;
	logger_reader reader = {0};
	//! 初始化log 参数
	header.hdr_size = LOGGER_ENTRY_STRUCT_SIZE;
	header.len = min(len,LOGGER_ENTRY_MAX_PAYLOAD);

	fix_up_readers(logger_log_p,LOGGER_ENTRY_STRUCT_SIZE + header.len,&reader);
	do_write_log(logger_log_p, (uint8_t*)(&header), LOGGER_ENTRY_STRUCT_SIZE);
	do_write_log_from_user(logger_log_p,buf, header.len);
	return 1;
}

uint8_t RingBufferGet(logger_log *logger_log_p, uint8_t *buf, uint8_t buf_len)
{
	logger_reader reader = {0};
	reader.r_off = logger_log_p->head;

	if (logger_log_p->w_off == reader.r_off) return 0;

	logger_entry scratch;
    size_t msg_start,len,count;
    logger_entry *entry = NULL;
    /*
    * First, copy the header to userspace, using the version of
    * the header requested
    */
    entry = get_entry_header(logger_log_p, reader.r_off, &scratch);
    count = entry->len;

    msg_start = logger_offset(logger_log_p,reader.r_off + LOGGER_ENTRY_STRUCT_SIZE);

    len = min(count, logger_log_p->size - msg_start);
    if (!memcpy(buf, (void*)&(logger_log_p->buffer[msg_start]), len))
        return 0;

    /*
    * Second, we read any remaining bytes, starting back at the head of
    * the log.
    */
    if (count != len)
        if (!memcpy(buf + len, logger_log_p->buffer, count - len))
            return -1;

    reader.r_off = logger_offset(logger_log_p, reader.r_off +
        LOGGER_ENTRY_STRUCT_SIZE + count);
    logger_log_p->head = reader.r_off;
    buf[count] = '\0';

    return len;


	//fix_up_readers(logger_log_p,LOGGER_ENTRY_STRUCT_SIZE + reader.r_off,&reader);
	#if 0
    logger_log* log = reader->log;
    if (log->w_off == reader->r_off) return 0;

    logger_entry scratch;
    size_t msg_start,len,count;
    logger_entry *entry = NULL;
    char* buf = (char*)log_buf;

    /*
    * First, copy the header to userspace, using the version of
    * the header requested
    */
    entry = get_entry_header(log, reader->r_off, &scratch);
    count = entry->len;

    msg_start = logger_offset(log,reader->r_off + LOGGER_ENTRY_STRUCT_SIZE);

    len = min(count, log->size - msg_start);
    if (!memcpy(buf, (void*)&(log->buffer[msg_start]), len))
        return 0;

    /*
    * Second, we read any remaining bytes, starting back at the head of
    * the log.
    */
    if (count != len)
        if (!memcpy(buf + len, log->buffer, count - len))
            return -EFAULT;

    reader->r_off = logger_offset(log, reader->r_off +
        LOGGER_ENTRY_STRUCT_SIZE + count);

    log_buf[len/2] = L'\0';

    return len/2;
	#endif
}
