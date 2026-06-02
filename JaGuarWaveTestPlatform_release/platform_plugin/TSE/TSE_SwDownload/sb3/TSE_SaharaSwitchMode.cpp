#include "StdAfx.h"
#include "TSE_SaharaSwitchMode.h"
#include <stdint.h>
#include "CJGW_QcommDownloadSerialPort.h"
#include <JGW_QMSLFuncPlugin/jgw_device_api.h>
#pragma pack(push, 1)
typedef struct
{
    uint32_t command;                 // command ID
    uint32_t length;                  // packet length incl command and length
} sahara_packet_header;

typedef struct
{
    sahara_packet_header header;
    uint32_t version;                 // target protocol version number
    uint32_t version_supported;       // minimum protocol version number supported
    // on target
    uint32_t cmd_packet_length;       // maximum packet size supported for command
    // packets
    uint32_t mode;                    // expected mode of target operation
    uint32_t reserved0;               // reserved field
    uint32_t reserved1;               // reserved field
    uint32_t reserved2;               // reserved field
    uint32_t reserved3;               // reserved field
    uint32_t reserved4;               // reserved field
    uint32_t reserved5;               // reserved field
} sahara_packet_hello;
#pragma pack(pop)

namespace JGW
{
    CTSE_SaharaSwitchMode::CTSE_SaharaSwitchMode(void)
    {
    }


    CTSE_SaharaSwitchMode::~CTSE_SaharaSwitchMode(void)
    {
    }

    const wchar_t* CTSE_SaharaSwitchMode::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_SaharaSwitchMode\",\
                \"TestName\": \"switch sahara mode\",\
                \"ResourceContextEnv\": \"get port handle environment\",\
                \"SaharaSerialClassEnv\": \"get sahara serial class environment\",\
                \"Mode\": \"Sahara hello Packet mode, when mode = 0, this indicates a SAHARA_MODE_IMAGE_TX_PENDING mode when mode = 2, this indicates a SAHARA_MODE_MEMORY_DEBUG mode\"\
                }";
    }

    bool CTSE_SaharaSwitchMode::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"ResourceContextEnv"))
        {
            mstrResourceContextEnv = strParamValue;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"Mode"))
        {
            mnMode = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaharaSerialClassEnv"))
        {
            mstrSaharaSerialClassEnv = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }

    bool CTSE_SaharaSwitchMode::TSE_Run()
    {
        sahara_packet_hello sahara = {0};
        sahara.header.command = 0x01/*SAHARA_HELLO_ID*/;
        sahara.header.length = sizeof(sahara_packet_hello);;
        sahara.version = /*sahara.sahara_hello->version*/2;
        sahara.version_supported = 4;
        //sahara.cmd_packet_length = 1024;
        sahara.mode = 0x02/*SAHARA_MODE_MEMORY_DEBUG*/;

        DWORD nNumberOfBytesToWrite = sizeof(sahara_packet_hello),nNumberOfBytesWritten = 0;
        LPOVERLAPPED pOverlapped = NULL;
        if (!mstrResourceContextEnv.empty())
        {
            //£¡ ERROR
#if 0
            HANDLE hResourceContext = (HANDLE)GetGlobalEnvironment()->GetInt(mstrResourceContextEnv);
            if (NULL == hResourceContext)
            {
                LogE_F(L"Could not connect to the Resource Context");
                return false;
            }
            short rsplen = 0;
            unsigned char szBuf[260] = {0};
            unsigned long version = 2;
            BOOL result = QLIB_QPHONEMS_Sahara_SetImageTransferMode(hResourceContext,&version,1000);

            // QLIB_SendSync(hResourceContext,nNumberOfBytesToWrite,(unsigned char*)&sahara,&rsplen,szBuf,5000);
            return true;
#else 
            return false;
#endif
            //return (TRUE == WriteFile(hResourceContext,(LPCVOID)&sahara,nNumberOfBytesToWrite,&nNumberOfBytesWritten,pOverlapped));
        }
        else
        {
            if (mstrSaharaSerialClassEnv.empty())
            {
                LogE_F(L"Could not connect to the Resource Context");
                return false;
            }

            CCJGW_QcommDownloadSerialPort* pCSerialPort = (CCJGW_QcommDownloadSerialPort*)GetGlobalEnvironment()->GetInt(mstrSaharaSerialClassEnv);
            if (NULL == pCSerialPort)
            {
                LogE_F(L"Could not connect to the Resource Context");
                return false;
            }
            size_t writen_len = 0;
            return pCSerialPort->WriteBufferToDownloadPort((const char*)&sahara,nNumberOfBytesToWrite,writen_len);
        }
    }
}
