/******************************************************************************
* $Header: //depot/HTE/QDART/SUITE/X-Functional/Drivers/ieee488/NetworkInstrumentProtocol.h#10 $
* $DateTime: 2016/04/04 19:04:53 $
*
*
******************************************************************************
*
* Copyright (c) 2008-2016 Qualcomm Technologies, Inc.
* All rights reserved.
* Qualcomm Technologies, Inc. Confidential and Proprietary.
*
******************************************************************************
*/

#ifndef _NETWORK_INSTRUMENT_PROTOCOL_H_
#define _NETWORK_INSTRUMENT_PROTOCOL_H_

#pragma once

#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <string>
/*#include "Utilities\UI\IUIManager.h"*/

//#define _VISA32_SUPPORT_
#ifdef _VISA32_SUPPORT_
// Note VISA32.lib is also required
#include "C:\Program Files\National Instruments\VXIPNP\WinNT\include\visa.h"
#include "C:\Program Files\National Instruments\VXIPNP\WinNT\include\visatype.h"
#endif

// VXI-11 Types (Appendix C)
typedef long Device_Link;

// used by interrupts
enum Device_AddrFamily
{
    DEVICE_TCP,
    DEVICE_UDP
};

typedef long Device_Flags;

// Error types
typedef long Device_ErrorCode;
struct Device_Error
{
    Device_ErrorCode error;
};

struct Create_LinkParms
{
    long clientId; /* implementation specific value */
    bool lockDevice; /* attempt to lock the device */
    unsigned long lock_timeout; /* time to wait on a lock */
    std::string device; /* name of device */
    int inst ;     /* maps to the corresponding sub-instrument to be used */
	std::string sourceAddress;
};

struct Create_LinkResp
{
    Device_ErrorCode error;
    Device_Link lid;
    unsigned short abortPort; /* for the abort RPC */
    unsigned long maxRecvSize; /* specifies max data size in bytes device will accept on a write */
};

struct Device_WriteParms
{
    Device_Link lid; /* link id from create_link */
    unsigned long io_timeout; /* time to wait for I/O */
    unsigned long lock_timeout; /* time to wait for lock */
    Device_Flags flags;
    unsigned char*data; /* the data length and the data itself */
};

struct Device_WriteResp
{
    Device_ErrorCode error;
    unsigned long size; /* Number of bytes written */
};

struct Device_ReadParms
{
    Device_Link lid; /* link id from create_link */
    unsigned long requestSize; /* Bytes requested */
    unsigned long io_timeout; /* time to wait for I/O */
    unsigned long lock_timeout; /* time to wait for lock */
    Device_Flags flags;
    char termChar; /* valid if flags & termchrset */
};

struct Device_ReadResp
{
    Device_ErrorCode error;
    long reason; /* Reason(s) read completed */
    unsigned char*data; /* data.len and data.val */
};

struct Device_ReadStbResp
{
    Device_ErrorCode error; /* error code */
    unsigned char stb; /* the returned status byte */
};

struct Device_GenericParms
{
    Device_Link lid; /* Device_Link id from connect call */
    Device_Flags flags; /* flags with options */
    unsigned long lock_timeout; /* time to wait for lock */
    unsigned long io_timeout; /* time to wait for I/O */
};

struct Device_RemoteFunc
{
    unsigned long hostAddr; /* Host servicing Interrupt */
    unsigned short hostPort; /* valid port # on client */
    unsigned long progNum; /* DEVICE_INTR */
    unsigned long progVers; /* DEVICE_INTR_VERSION */
    Device_AddrFamily progFamily; /* DEVICE_UDP | DEVICE_TCP */
};

struct Device_EnableSrqParms
{
    Device_Link lid;
    bool enable; /* Enable or disable interrupts */
    unsigned char handle[40]; /* Host specific data */
};

struct Device_LockParms
{
    Device_Link lid; /* link id from create_link */
    Device_Flags flags; /* Contains the waitlock flag */
    unsigned long lock_timeout; /* time to wait to acquire lock */
};

struct Device_DocmdParms
{
    Device_Link lid; /* link id from create_link */
    Device_Flags flags; /* flags specifying various options */
    unsigned long io_timeout; /* time to wait for I/O to complete */
    unsigned long lock_timeout; /* time to wait on a lock */
    long cmd; /* which command to execute */
    bool network_order; /* client's byte order */
    long datasize; /* size of individual data elements */
    unsigned char*data_in; /* docmd data parameters */
};

struct Device_DocmdResp
{
    Device_ErrorCode error; /* returned status */
    unsigned char*data_out; /* returned data parameter */
};

// VXI-11 Error codes (B.5.2)
typedef enum
{
    VXI11Error_NoError=0,
    VXI11Error_SyntaxError=1,
    VXI11Error_DeviceNotAccessible=3,
    VXI11Error_InvalidLinkID=4,
    VXI11Error_ParameterError=5,
    VXI11Error_ChannelNotEstablished=6,
    VXI11Error_OperationNotSupported=8,
    VXI11Error_OutOfResources=9,
    VXI11Error_DeviceLockedByAnotherLink=11,
    VXI11Error_NoLockHeldByThisLink=12,
    VXI11Error_IOTimeout=15,
    VXI11Error_IOError=17,
    VXI11Error_InvalidAddress=21,
    VXI11Error_Abort=23,
    VXI11Error_ChannelAlreadyEstablished=29
} VXI11ErrorCodes;

// VXI-11 RPC Procedure Numbers (Appendix C)
// Except where noted all procedures are Core Channel procedures
typedef enum
{
    VXI11Procedure_device_abort=1, /* Abort Channel Procedure */
    VXI11Procedure_create_link=10,
    VXI11Procedure_device_write=11,
    VXI11Procedure_device_read=12,
    VXI11Procedure_device_readstb=13,
    VXI11Procedure_device_trigger=14,
    VXI11Procedure_device_clear=15,
    VXI11Procedure_device_remote=16,
    VXI11Procedure_device_local=17,
    VXI11Procedure_device_lock=18,
    VXI11Procedure_device_unlock=19,
    VXI11Procedure_device_enable_srq=20,
    VXI11Procedure_device_docmd=22,
    VXI11Procedure_destroy_link=23,
    VXI11Procedure_create_intr_chan=25,
    VXI11Procedure_destroy_intr_chan=26
} VXI11Procedures;

class CNetworkInstrumentProtocol
{
public:
    CNetworkInstrumentProtocol(void);
    ~CNetworkInstrumentProtocol(void);

    // VXI-11 Core Channel Procedures
    Create_LinkResp create_link(const Create_LinkParms parms);
    Device_WriteResp device_write(const Device_WriteParms parms);
    Device_ReadResp device_read (const Device_ReadParms parms);
    Device_Error destroy_link (const Device_Link lid);

    // Unimplemented VXI-11 functions
    /*   // VXI-11 Abort Channel Procedures
    Device_Error device_abort(const Device_Link lid);

    // VXI-11 Core Channel Procedures
    Device_ReadStbResp device_readstb(const Device_GenericParms parms);
    Device_Error device_trigger(const Device_GenericParms parms);
    Device_Error device_clear(const Device_GenericParms parms);
    Device_Error device_remote(const Device_GenericParms parms);
    Device_Error device_local(const Device_GenericParms parms);
    Device_Error device_lock(const Device_LockParms parms);
    Device_Error device_unlock(const Device_Link parms);
    Device_Error device_enable_srq(const Device_EnableSrqParms parms);
    Device_DocmdResp device_docmd(const Device_DocmdParms parms);
    Device_Error create_intr_chan(const Device_RemoteFunc parms);
    Device_Error destroy_intr_chan(void);
    */
private:
#ifdef _VISA32_SUPPORT_
    ViSession m_defaultRM;
    ViSession m_instr;
#endif

    // This must be at least 1024 (VXI 11 Rule B.6.3)
    static const unsigned long maxBufferSize=0x100100;
    static const unsigned long maxCmdBlockSize=0x40000;
    // Minimum Windows Sockets version
    static const unsigned short MinWSAMajorVers=2;
    static const unsigned short MinWSAMinorVers=2;
    // Channel constants (VXI 11 Rule B.6.1)
    static const unsigned int abortChannel_prog=395184;
    static const unsigned int abortChannel_prot=IPPROTO_TCP;
    static const unsigned int abortChannel_vers=1;
    static const unsigned int coreChannel_prog=395183;
    static const unsigned int coreChannel_prot=IPPROTO_TCP;
    static const unsigned int coreChannel_vers=1;
    static const unsigned int interruptChannel_prog=395185;
    static const unsigned int interruptChannel_prot=IPPROTO_TCP;
    static const unsigned int interruptChannel_vers=1;
    // Portmapper constants (RFC1833)
    static const unsigned int portmapper_prog=100000;
   static const unsigned int portmapper_port=111;
    static const unsigned int portmapper_prot=IPPROTO_TCP;
    static const unsigned int portmapper_vers=2;
    static const unsigned int RPC_Version=2;

    // From RFC 1831 Section 8
    typedef enum
    {
        CALL=0,
        REPLY=1
    } RPC_MSG_TYPE;
    // From RFC 1833
    typedef enum
    {
        PMAPPROC_NULL=0,
        PMAPPROC_SET=1,
        PMAPPROC_UNSET=2,
        PMAPPROC_GETPORT=3,
        PMAPPROC_DUMP=4,
        PMAPPROC_CALLIT=5
    } portmapper_procedure;

    typedef struct
    {
        unsigned int port;
        unsigned int prog;
        unsigned int proc;
        unsigned int prot;
        unsigned int vers;
        SOCKET socket;
    } channel;
    typedef struct
    {
        unsigned long clientID;
        unsigned long lockDevice;
        unsigned long lockTimeout;
        unsigned long stringLength;
        unsigned char device[sizeof(long)*2];
    } XDR_createLinkParms;
    typedef struct
    {
        unsigned long lid;
        unsigned long io_timeout;
        unsigned long lock_timeout;
        unsigned long flags;
        unsigned long stringLength;
        unsigned char command[maxCmdBlockSize*sizeof(long)];
    } XDR_deviceWriteParms;
    typedef struct
    {
        unsigned long lid;
        unsigned long requestSize;
        unsigned long io_timeout;
        unsigned long lock_timeout;
        unsigned long flags;
        unsigned long termChar;
    } XDR_deviceReadParms;
    typedef struct
    {
        unsigned long error;
        unsigned long reason;
        unsigned char response[maxCmdBlockSize*sizeof(long)];
    } XDR_deviceReadResp;
    typedef struct
    {
        unsigned long prog;
        unsigned long vers;
        unsigned long prot;
        unsigned long port;
    } portmapper_mapping;
    typedef struct
    {
        union
        {
            struct
            {
                unsigned int length:31;
                unsigned int lastFragment:1;
            } bits;
            unsigned long word;
        } fragInfo;
        unsigned long xid;
        unsigned long callType;
        unsigned long rpcVersion;
        unsigned long program;
        unsigned long version;
        unsigned long procedure;
        unsigned long authentication[4];
    } RPC_CallHeader;
    typedef struct
    {
        union
        {
            struct
            {
                unsigned int length:31;
                unsigned int lastFragment:1;
            } bits;
            unsigned long word;
        } fragInfo;
    } RPC_FragHeader;
    typedef struct
    {
        unsigned long xid;
        unsigned long replyType;
        unsigned long authentication[4];
    } RPC_ReplyHeader;

    // XDR Functions
    unsigned int XDR_formatString(const char*sourceString,const unsigned int maxStringLength,char*destinationBuffer,unsigned long*stringLength,unsigned long*dataSize);
    unsigned int XDR_getString(const char*sourceBuffer,const unsigned int maxStringLength,char*destinationBuffer);
    // RPC Functions
    unsigned int RPC_Call(const channel chan,unsigned long dataSize,unsigned char*data);
    unsigned long RPC_PMAPPROC_GETPORT(const std::string ipAddress,const unsigned int prog,const unsigned int vers,const unsigned int prot,const unsigned int port=0,const char* source_addr = NULL);
    unsigned int RPC_Reply(const channel chan,unsigned long dataSize,unsigned char*data);
    // TCP Functions
    void TCP_Cleanup(void);
    void TCP_CloseSocket(const SOCKET socket);
    long TCP_Init(void);
    SOCKET TCP_OpenSocket(const std::string ipAddress,const long port,const char* source_addr/* = NULL*/);
    unsigned int TCP_Receive(const SOCKET sock,char*buffer,const int maxSize);
    unsigned int TCP_Send(const SOCKET sock,const char*buffer,const int size);

    channel m_abortChannel;
    channel m_coreChannel;
    channel m_portmapper;
    unsigned char m_buffer[maxBufferSize+sizeof(long)+1];
    Device_Link m_lid;
    //! IUIManager*m_pUIError;
    long m_xid;
    unsigned long m_maxRecvSize;
};
#endif
