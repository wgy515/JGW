#pragma once
#include <TSE_FoundationLib/TSE_TestBase.h>
#include <stdint.h>

namespace JGW
{
    typedef enum 
    {
        E_MicroAmmeterReadOperation,
        E_MicroAmmeterWriteSingleOperation,
        E_MicroAmmeterWriteMultiOperation
    }EMicroAmmeterOperationType;

    class CTSE_MicroAmmeterRW : public CTSE_TestBase
    {
    public:
        CTSE_MicroAmmeterRW(void);
        ~CTSE_MicroAmmeterRW(void);
    private:
        const wchar_t* TSE_GetParamDescription();
        bool TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue);
        bool TSE_Run();
    private:
        int16_t MicroAmmeterCRC16(uint8_t *puchMsg,uint8_t usDataLen);
        int FormatMicroAmmeterReadCommand();
        int FormatMicroAmmeterWriteSingleCommand();
        int FormatMicroAmmeterMultiSingleCommand();
        // 0x01 0x03 0x0A 0x30 0xF2 0x00 0x00 0x2D 0x55 0x00 0x89 0x00 0x01  0x23 0x76
        // 设备站号 命令  数据大小 有效数据 CRC校验
        bool CheckMicroAmmeterReadRspVaild(uint8_t* szReadBuf,uint8_t uBufLen);
        bool CheckMicroAmmeterWriteSingleRspVaild(uint8_t* szReadBuf,uint8_t uBufLen);
        bool CheckMicroAmmeterMultiSingleRspVaild(uint8_t* szReadBuf,uint8_t uBufLen);
    private:
        // 0 : 读取寄存器 2 : 写单个寄存器 3 : 写多个寄存器
        EMicroAmmeterOperationType meOperationType;
        // Equipment station number
        int mnEquipmentStationNumber;
        int mnStartAddress;
        int mnReadRegisterCount;
        std::vector<uint16_t> mvnWriteContents;
        uint8_t* mszTxBuf;
        std::wstring mstrSaveReadRegContentEnvironment;
    };
}


