#include "StdAfx.h"
#include "TSE_MicroAmmeterRW.h"
#include "TSE_DevicesGlobalResource.h"
/* CRC 高位字节值表 */ 
const uint8_t auchCRCHi[] = { 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
    0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
    0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
} ; 

/* CRC低位字节值表*/ 
const uint8_t auchCRCLo[] = { 
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
    0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
    0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
    0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
    0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
    0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
    0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
    0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
    0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
    0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
    0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
    0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
    0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
    0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
    0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
    0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
    0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
    0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
    0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
    0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
} ;

#define MICRO_AMM_TX_DATA_SIZE 128
#define MICRO_AMM_RX_DATA_SIZE 128

namespace JGW
{
    // 0 : 读取寄存器(0x03) 1 : 写单个寄存器(0x06) 2 : 写多个寄存器(0x16)
    int gnRegisterCommands[3] = {0x03,0x06,0x10};
    
    CTSE_MicroAmmeterRW::CTSE_MicroAmmeterRW(void) : meOperationType(E_MicroAmmeterReadOperation),mnReadRegisterCount(1),mnStartAddress(0x00),mnEquipmentStationNumber(1)
    {
        mszTxBuf = (uint8_t*)malloc(128);
        memset(mszTxBuf,0x00,MICRO_AMM_TX_DATA_SIZE);
    }


    CTSE_MicroAmmeterRW::~CTSE_MicroAmmeterRW(void)
    {
        free(mszTxBuf);
        mszTxBuf = NULL;
    }

    const wchar_t*  CTSE_MicroAmmeterRW::TSE_GetParamDescription()
    {
        return L"{\
                \"RealName\": \"CTSE_MicroAmmeterRW\",\
                \"TestName\": \"Micro Ammeter Read Write\",\
                }";
    }
#if 0
    /******************************************************************************
    * Function Name  :  crc16
    * Input          :  数据缓冲区指针：puchMsg ， 数据长度：usDataLen
    * Return         :  16 位CRC校验码
    * Description     :  产生 16 位CRC校验码
    *******************************************************************************/
    INT16U crc16(INT8U *puchMsg, INT8U usDataLen) 
    { 
        INT8U  uchCRCHi=0xFF ; /* 高CRC字节初始化 */ 
        INT8U  uchCRCLo=0xFF ; /* 低CRC字节初始化 */ 
        INT16U uIndex;          /* CRC循环中的索引 */ 

        while(usDataLen--)        /* 传输消息缓冲区  */ 
        { uIndex  =uchCRCHi^*puchMsg++ ; /* 计算CRC */ 
        uchCRCHi=uchCRCLo^auchCRCHi[uIndex] ; 
        uchCRCLo=auchCRCLo[uIndex]; 
        } 
        return (uchCRCHi<<8|uchCRCLo); 
    }
#endif
    int16_t CTSE_MicroAmmeterRW::MicroAmmeterCRC16(uint8_t *puchMsg,uint8_t usDataLen)
    {
        uint8_t uchCRCHi = 0xFF;
        uint8_t uchCRCLo = 0xFF;
        int_least16_t uIndex;

        while (usDataLen--)
        {
             uIndex = uchCRCHi^ (*(puchMsg++)) ;
             uchCRCHi = uchCRCLo^auchCRCHi[uIndex]; 
             uchCRCLo = auchCRCLo[uIndex]; 
        }

        return (uchCRCHi<< 8 | uchCRCLo); 
    }

    bool CTSE_MicroAmmeterRW::TSE_AddParam(const wchar_t* strParamName,const wchar_t* strParamValue)
    {
        if (CTSE_TestBase::TSE_AddParam(strParamName,strParamValue)) return true;
        if (TSE_PARAM_NAME_EQUAL(strParamName,L"OperationType"))
        {
            meOperationType = (EMicroAmmeterOperationType)_ttoi(strParamValue);
            if (meOperationType < E_MicroAmmeterReadOperation || meOperationType > E_MicroAmmeterWriteMultiOperation) meOperationType = E_MicroAmmeterReadOperation;
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"EquipmentStationNumber"))
        {
            mnEquipmentStationNumber = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"StartAddress"))
        {
            mnStartAddress = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"ReadRegisterCount"))
        {
            mnReadRegisterCount = _ttoi(strParamValue);
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"WriteContents"))
        {
            std::vector<std::wstring> vstrTemp;
            JGW_ParserStrW(strParamValue,L" ",vstrTemp);
            for (std::vector<std::wstring>::iterator it = vstrTemp.begin();it != vstrTemp.end();++it)
            {
                mvnWriteContents.push_back((int16_t)(_tcstol(it->c_str(),NULL,16) & 0xFF));
            }
        }
        else if (TSE_PARAM_NAME_EQUAL(strParamName,L"SaveReadRegContentEnvironment"))
        {
            mstrSaveReadRegContentEnvironment = strParamValue;
        }
        else
        {
            return false;
        }
        return true;
    }
    // 设备站号  命令   开始地址   需要读取数目    CRC校验
    // 发出 0x01 0x03 0x00 0x00 0x00 0x05 0x85 0xC9
    int CTSE_MicroAmmeterRW::FormatMicroAmmeterReadCommand()
    {
        int index = 0;

        memset(mszTxBuf,0x00,MICRO_AMM_TX_DATA_SIZE);
        mszTxBuf[index++] = (mnEquipmentStationNumber & 0xFF);
        mszTxBuf[index++] = gnRegisterCommands[E_MicroAmmeterReadOperation];
        mszTxBuf[index++] = ((mnStartAddress >> 8) & 0xFF);
        mszTxBuf[index++] = (mnStartAddress & 0xFF);
        mszTxBuf[index++] = ((mnReadRegisterCount >> 8) & 0xFF);
        mszTxBuf[index++] = (mnReadRegisterCount & 0xFF);
        int16_t crc16 = MicroAmmeterCRC16(mszTxBuf,index);
        mszTxBuf[index++] = ((crc16 >> 8) & 0xFF);
        mszTxBuf[index++] = (crc16 & 0xFF);

        return index;
    }
    // 设备站号   命令   地址   数据    CRC校验
    // 01 06 00 04 00 01  09 CB
    int CTSE_MicroAmmeterRW::FormatMicroAmmeterWriteSingleCommand()
    {
        int index = 0;

        memset(mszTxBuf,0x00,MICRO_AMM_TX_DATA_SIZE);
        mszTxBuf[index++] = (mnEquipmentStationNumber & 0xFF);
        mszTxBuf[index++] = gnRegisterCommands[E_MicroAmmeterWriteSingleOperation];;
        mszTxBuf[index++] = ((mnStartAddress >> 8) & 0xFF);
        mszTxBuf[index++] = (mnStartAddress & 0xFF);
        if (mvnWriteContents.size() != 2) return 0;
        mszTxBuf[index++] = (mvnWriteContents[0] & 0xFF);
        mszTxBuf[index++] = (mvnWriteContents[1] & 0xFF);
        int16_t crc16 = MicroAmmeterCRC16(mszTxBuf,index);
        mszTxBuf[index++] = ((crc16 >> 8) & 0xFF);
        mszTxBuf[index++] = (crc16 & 0xFF);

        return index;
    }
    // 设备站号  命令   开始地址  N个寄存器     字节数  　数据   CRC校验
    // 0x01 0x10 0x00 0x09 0x00 0x02 0x04 0x00 0x0F 0x42 0x40 0x32 0x96
    /*
    0x01设备站号
    0x10命令
    0x00 0x09	开始地址
    0x00 0x02	数据长度		
    0x04  		字节数 

    0x00 0x0F   设置电流报警上限 (高16位)
    0x42 0x40   设置电流报警上限 (低16位)      ->  0x000F4240  = 1000000 = 1.000A 

    0x32 0x96  CRC校验
    */
    int CTSE_MicroAmmeterRW::FormatMicroAmmeterMultiSingleCommand()
    {
        int index = 0;

        memset(mszTxBuf,0x00,MICRO_AMM_TX_DATA_SIZE);
        mszTxBuf[index++] = (mnEquipmentStationNumber & 0xFF);
        mszTxBuf[index++] = gnRegisterCommands[E_MicroAmmeterWriteMultiOperation];;
        mszTxBuf[index++] = ((mnStartAddress >> 8) & 0xFF);
        mszTxBuf[index++] = (mnStartAddress & 0xFF);
        //if (mvnWriteContents.size() != 2) return 0;
        size_t length = mvnWriteContents.size() / 2;
        mszTxBuf[index++] = ((length >> 8) & 0xFF);
        mszTxBuf[index++] = (length & 0xFF);
        mszTxBuf[index++] = (length * 2 & 0xFF);
        for (size_t pos = 0;pos < length;pos++)
        {
            mszTxBuf[index++] = ((uint8_t)mvnWriteContents[pos * 2] & 0xFF);
            mszTxBuf[index++] = ((uint8_t)mvnWriteContents[pos * 2 + 1] & 0xFF);
        }
        int16_t crc16 = MicroAmmeterCRC16(mszTxBuf,index);
        mszTxBuf[index++] = ((crc16 >> 8) & 0xFF);
        mszTxBuf[index++] = (crc16 & 0xFF);

        return index;
    }
    // 设备站号  命令   数据大小       有效数据   CRC校验
    /*
    接收 0x01 0x03 0x0A 0x30 0xF2 0x00 0x00 0x2D 0x55 0x00 0x89 0x00 0x01  0x23 0x76
    (0x01设备站号	0x03命令		0x0A数据大小)
    (0x30 0xF2) (0x00 0x00 0x2D 0x55) (0x00 0x89) (0x00 0x01) 有效数据 
    测量电压(Reg0)				=  0x30F2/1000  = 12.530V
    测量电流(Reg1和Reg2)		=  0x00002D55/1000  = 11.605mA 
    测量功率(Reg3)				=  0x0089/1000  = 0.137W 			
    开机画面(Reg4)	　			=  (0x00 0x01) = 1  (0:关闭	1:打开)
    0x23 0x76　 CRC校验
    */
    bool CTSE_MicroAmmeterRW::CheckMicroAmmeterReadRspVaild(uint8_t* szReadBuf,uint8_t uBufLen)
    {
        // 检查读取长度是否异常
        if (uBufLen < 5 || uBufLen != 2 + 1 + 2 + (uint8_t)szReadBuf[2])
        {
            LogE(L"read length exception...");
            return false;
        }
        //
        if (mnReadRegisterCount * 2 != (uint8_t)szReadBuf[2])
        {
            LogE(L"Some register contents are lost...");
            return false;
        }
        //
        if (mnEquipmentStationNumber != szReadBuf[0] || gnRegisterCommands[E_MicroAmmeterReadOperation] != szReadBuf[1])
        {
            LogE(L"Protocol header error");
            return false;
        }
        // CRC校验
        uint16_t crc16 = (int16_t)(szReadBuf[uBufLen - 1] & 0xFF) | (int16_t)(szReadBuf[uBufLen - 2] << 8);
        if (crc16 != (uint16_t)MicroAmmeterCRC16(szReadBuf,uBufLen - 2))
        {
            LogE(L"CRC check failure");
            return false;
        }

        return true;
    }
    // 设备站号  命令   地址   数据    CRC校验
    bool CTSE_MicroAmmeterRW::CheckMicroAmmeterWriteSingleRspVaild(uint8_t* szReadBuf,uint8_t uBufLen)
    {
        // 检查读取长度是否异常
        if (uBufLen != 2 + 2 + 2 + 2)
        {
            LogE(L"read length exception...");
            return false;
        }

        //
        if (mnEquipmentStationNumber != szReadBuf[0] || gnRegisterCommands[E_MicroAmmeterWriteSingleOperation] != szReadBuf[1])
        {
            LogE(L"Protocol header error");
            return false;
        }

        // CRC校验
        uint16_t crc16 = (int16_t)(szReadBuf[uBufLen - 1] & 0xFF) | (int16_t)(szReadBuf[uBufLen - 2] << 8);
        if (crc16 != (uint16_t)MicroAmmeterCRC16(szReadBuf,uBufLen - 2))
        {
            LogE(L"CRC check failure");
            return false;
        }

        // mnStartAddress
        uint16_t addr = (uint16_t)(szReadBuf[3] & 0xFF) | (uint16_t)(szReadBuf[2] << 8);
        if (addr != mnStartAddress)
        {
            LogE(L"Failed to write the register address");
            return false;
        }

        if (2 == mvnWriteContents.size())
        {
            if (mvnWriteContents[0] != szReadBuf[4] || mvnWriteContents[1] != szReadBuf[5])
            {
                LogE(L"Description Failed to write register content");
                return false;
            }
        }

        return true;
    }
    // 设备站号  命令   开始地址       N个寄存器    CRC校验
    // 0x01 0x10 0x00 0x09 0x00 0x02 0x91 0xC9
    bool CTSE_MicroAmmeterRW::CheckMicroAmmeterMultiSingleRspVaild(uint8_t* szReadBuf,uint8_t uBufLen)
    {
        // 检查读取长度是否异常
        if (uBufLen != 2 + 2 + 2 + 2)
        {
            LogE(L"read length exception...");
            return false;
        }

        //
        if (mnEquipmentStationNumber != szReadBuf[0] || gnRegisterCommands[E_MicroAmmeterWriteMultiOperation] != szReadBuf[1])
        {
            LogE(L"Protocol header error");
            return false;
        }

        // CRC校验
        uint16_t crc16 = (int16_t)(szReadBuf[uBufLen - 1] & 0xFF) | (int16_t)(szReadBuf[uBufLen - 2] << 8);
        if (crc16 != (uint16_t)MicroAmmeterCRC16(szReadBuf,uBufLen - 2))
        {
            LogE(L"CRC check failure");
            return false;
        }

        // mnStartAddress
        uint16_t addr = (uint16_t)(szReadBuf[3] & 0xFF) | (uint16_t)(szReadBuf[2] << 8);
        if (addr != mnStartAddress)
        {
            LogE(L"Failed to write the register address");
            return false;
        }

        uint16_t dataCount = (uint16_t)(szReadBuf[5] & 0xFF) | (uint16_t)(szReadBuf[4] << 8);
        if (dataCount != mvnWriteContents.size() / 2)
        {
            LogE(L"Write register values are inconsistent");
            return false;
        }
        return true;
    }

    bool CTSE_MicroAmmeterRW::TSE_Run()
    {
        //! check serial com port
        if (!CTSE_DevicesGlobalResource::GetSerialComPort().IsOpenComPort())
        {
            GetLogServices()->ErrorLog(L"Serial Com Port Is Not Open .");
            return false;
        }

        int length = 0;
        switch (meOperationType)
        {
        case E_MicroAmmeterReadOperation:
            length = FormatMicroAmmeterReadCommand();
            break;
        case E_MicroAmmeterWriteSingleOperation:
            length = FormatMicroAmmeterWriteSingleCommand();
            break;
        case E_MicroAmmeterWriteMultiOperation:
            length = FormatMicroAmmeterMultiSingleCommand();
            break;
        }
        if (0 == length)
        {
            LogE(L"Abnormal formatting of written data");
            return false;
        }
        std::wstring strTemp;
        for (int index = 0;index < length;index++)
        {
            strTemp += JGW_GetFormatWString(L"%02X ",(uint8_t)(mszTxBuf[index] & 0xFF));
        }
        CTSE_DevicesGlobalResource::GetSerialComPort().SetEndString("##########");
        if (!CTSE_DevicesGlobalResource::GetSerialComPort().WriteBinaryFileBufferToComPort((const char *)mszTxBuf,length, 4 * 1000))
        {
            GetLogServices()->ErrorLogFormat(L"Write Serial Failed .(%s)",strTemp.c_str());
            return false;
        }
        //Sleep(100);
        char buf[MICRO_AMM_RX_DATA_SIZE] = {0};
        int readLength = CTSE_DevicesGlobalResource::GetSerialComPort().ReadAsyncCommand(buf,128,200);
        if (0 >= readLength)
        {
            return false;
        }
        strTemp = L"";
        for (int index = 0;index < readLength;index++)
        {
            strTemp += JGW_GetFormatWString(L"%02X ",(uint8_t)(buf[index]));
        }
        LogU(E_TSE_SHOW_CONTENT_INFO,strTemp.c_str());
        GetGlobalEnvironment()->PutString(mstrSaveReadRegContentEnvironment,strTemp);

        if (meOperationType == E_MicroAmmeterReadOperation)
        {
            return CheckMicroAmmeterReadRspVaild((uint8_t *)buf,readLength);
        }
        else if (meOperationType == E_MicroAmmeterWriteSingleOperation)
        {
            return CheckMicroAmmeterWriteSingleRspVaild((uint8_t *)buf,readLength);
        }
        else if (meOperationType == E_MicroAmmeterWriteMultiOperation)
        {
            return CheckMicroAmmeterMultiSingleRspVaild((uint8_t *)buf,readLength);
        }

        return false;
    }

}

