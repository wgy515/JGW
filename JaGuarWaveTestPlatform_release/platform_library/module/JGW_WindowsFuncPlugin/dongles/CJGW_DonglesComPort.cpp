#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_DonglesComPort.h>
#include <JGW_WindowsFuncPlugin/PerasoDefine.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
namespace JGW
{
    CCJGW_DonglesComPort::CCJGW_DonglesComPort(CCJGW_CSerialComPort& cSerialComPort) : mcSerialComPort(cSerialComPort)
    {
    }

    CCJGW_DonglesComPort::~CCJGW_DonglesComPort(void)
    {
    }

    bool CCJGW_DonglesComPort::OpenDonglesComPort(const std::wstring& strComPort)
    {
        return mcSerialComPort.OpenSerialComPort(strComPort);
    }

    void CCJGW_DonglesComPort::CloseDonglesComPort()
    {
        mcSerialComPort.CloseSerialComPort();
    }

    bool CCJGW_DonglesComPort::ConfigureChannel(int channel)
    {
        std::string strCommand,strRead;

        JGW_FormatString(strCommand,"mib s 62-1-1-2 %02d\n",channel);
        bool result = ReadWriteCommands(strCommand.c_str(),strRead);

        return result && strRead.empty();
    }

    bool CCJGW_DonglesComPort::ReadConfigPdbVersion(std::wstring& strConfigPdbVersion)
    {
        std::string strRead,strRsp;

        bool result = ReadWriteCommands("mib q s 7d-2-1-1\n",strRead);
        if (!result || std::string::npos == strRead.find("Config "))
        {
            return false;
        }
        strRsp = JGW_GetSubStrToStartEndStr(strRead,"Config "," ");
        strConfigPdbVersion = JGW_A2W(strRsp);
        
        return true;
    }

    bool CCJGW_DonglesComPort::ReadOtpPdbVersion(std::wstring& strOtpPdbVersion)
    {
        std::string strRead,strRsp;

        bool result = ReadWriteCommands("mib q s 7d-1-1-1\n",strRead);
        if (!result || std::string::npos == strRead.find("OTP "))
        {
            return false;
        }
        strRsp = JGW_GetSubStrToStartEndStr(strRead,"OTP "," ");
        strOtpPdbVersion = JGW_A2W(strRsp);

        return true;
    }

    bool CCJGW_DonglesComPort::ReadSerialNumberAndMacAddress(std::wstring& strSerialNumber,std::wstring& strMacAddress)
    {
        std::string strRead,strTemp;

        mcSerialComPort.ReadRespCommandToWriteCommand("bdi\n",strRead);
        mcSerialComPort.EraseCliString(strRead);
        strRead += "\n";
        strTemp = JGW_GetSubStrToStartEndStr(strRead,"serial_number =");
        strSerialNumber = JGW_A2W(strTemp);
        strTemp = JGW_GetSubStrToStartEndStr(strRead,"mac_addr =");
        strMacAddress = JGW_A2W(strTemp);
        JGW_ReplaceStringW(strMacAddress,L":",L"");

        return true;
    }

    bool CCJGW_DonglesComPort::ReadMacAddress(std::wstring& strMacAddress)
    {
        std::string strSerialNumberCommand = "mib q b 60-1-1-1\n",strRead;
        mcSerialComPort.ReadRespCommandToWriteCommand(strSerialNumberCommand.c_str(),strRead);
        mcSerialComPort.EraseCliString(strRead);
        JGW_ReplaceStringA(strRead," ","");
        if (MAC_ADDRESS_MAX_LEN != strRead.length()) return false;
        strMacAddress = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_DonglesComPort::ReadSerialNumber(std::wstring& strSerialNumber)
    {
        std::string strSerialNumberCommand = "mib q l 60-1-1-2\n",strRead;
        mcSerialComPort.ReadRespCommandToWriteCommand(strSerialNumberCommand.c_str(),strRead);
        mcSerialComPort.EraseCliString(strRead);

        unsigned char szBuf[MAX_SERIAL_NUMBER_FALSH_BIT + 1] = {0};
        if (!PareseDonglesReadBlockBuf(szBuf,strRead.c_str(),MAX_SERIAL_NUMBER_FALSH_BIT / 4)) return false;
        strSerialNumber = (wchar_t*)szBuf;

        return true;
    }

    bool CCJGW_DonglesComPort::ReadRawWriteCommands(const char* strCommand,std::string& strRead)
    {
        return mcSerialComPort.ReadRespCommandToWriteCommand(strCommand,strRead);
    }

    bool CCJGW_DonglesComPort::ReadWriteCommands(const char* strCommand,std::string& strRead)
    {
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand,strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        return true;
    }

    bool CCJGW_DonglesComPort::ReadWriteCommands(const std::string& strCommand,std::string& strRead)
    {
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        return true;
    }


    std::string CCJGW_DonglesComPort::SubDonglesComPortString(const std::string& strRead,const char* pSub,const char* pDelimiter)
    {
        const char *pStart= strRead.c_str(),*pTemp,*pEnd;
        pTemp = strstr(strRead.c_str(),pSub);
        if (!pTemp) return "";
        size_t start_pos = pTemp - pStart;
        pEnd = strstr(pTemp,pDelimiter);
        if (!pEnd) pEnd = pStart + strRead.length();
        std::string serial_number = strRead.substr(start_pos,pEnd - pTemp);

        return serial_number;
    }

    bool CCJGW_DonglesComPort::ReadFirmwareVersion(std::wstring& strFirmwareVersion)
    {
        std::string strFirmwareVersionCommand = "version\n",strRead;
        mcSerialComPort.ReadRespCommandToWriteCommand(strFirmwareVersionCommand.c_str(),strRead);
        mcSerialComPort.EraseCliString(strRead);
    
        std::string firmware_version = SubDonglesComPortString(strRead,"Falcon","\n");
        if (firmware_version.empty()) return false;

        JGW_ReplaceStringA(firmware_version," ","");
        JGW_ReplaceStringA(firmware_version,"Falcon","");
        JGW_ReplaceStringA(firmware_version,"UMAC","");
        JGW_ReplaceStringA(firmware_version,"Windows","");
        strFirmwareVersion = JGW_A2W(firmware_version);

        return true;
    }

    bool CCJGW_DonglesComPort::ReadUsbSpeed(std::wstring& strUsbType)
    {
        std::string strFirmwareVersionCommand = "status\n",strRead;
        mcSerialComPort.ReadRespCommandToWriteCommand(strFirmwareVersionCommand.c_str(),strRead);
        mcSerialComPort.EraseCliString(strRead);

        std::string firmware_version = SubDonglesComPortString(strRead,"speed","\n");
        if (firmware_version.empty()) return false;

        JGW_ReplaceStringA(firmware_version," ","");
        JGW_ReplaceStringA(firmware_version,"speed","");
        JGW_ReplaceStringA(firmware_version,":","");
        strUsbType = JGW_A2W(firmware_version);

        return true;
    }

    bool CCJGW_DonglesComPort::ReadRssiToCommand(const std::string& strCommand,int& rssi)
    {
        std::string /*strCommand = "mib q b 12-1-1-d\n",*/strRead;
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        if (strRead.empty() || 2 != strRead.length()) return false;
        sscanf_s(strRead.c_str(),"%X",&rssi);
        rssi -= 256;
        return true;
    }

    bool CCJGW_DonglesComPort::ReadOmniRssi(int& rssi)
    {
        std::string strCommand = "mib q b 12-1-1-d\n",strRead;
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        if (strRead.empty() || 2 != strRead.length()) return false;
        sscanf_s(strRead.c_str(),"%X",&rssi);
        rssi -= 256;
        return true;
    }

    bool CCJGW_DonglesComPort::ReadMcs(int& mcs)
    {
        std::string strCommand = "mib q b 12-8-1-a\n",strRead;
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        if (strRead.empty() || 2 != strRead.length()) return false;
        sscanf_s(strRead.c_str(),"%X",&mcs);
        return true;
    }
    /*
    #define TX_SECTOR_COMMAND "mib q b 12-8-1-c"
    #define RX_SECTOR_COMMAND "mib q b 12-8-1-d"
    */
    bool CCJGW_DonglesComPort::ReadSectorStructToTxSectorCommand(std::wstring& strTxSector)
    {
        std::string strCommand = "mib q b 12-8-1-c\n",strRead;
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        strTxSector = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_DonglesComPort::ReadSectorStructToRxSectorCommand(std::wstring& strRxSector)
    {
        std::string strCommand = "mib q b 12-8-1-d\n",strRead;
        if (!mcSerialComPort.ReadRespCommandToWriteCommand(strCommand.c_str(),strRead)) return false;
        mcSerialComPort.EraseCliString(strRead);
        strRxSector = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_DonglesComPort::PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount)
    {
        std::vector<std::string> vstrRead;
        JGW_ParserStrA(read_buf," ",vstrRead);
        if (vstrRead.size() != mnMaxBlockCount)
        {
            Log4WE(L"read buf error !!!");
            return false;
        }

        for (size_t i = 0;i < vstrRead.size();i ++)
        {
            FormattedReadLittleEndianBlock((unsigned char*)(output_buf + i * 4),vstrRead.at(i).c_str());
        }
        return true;
    }

    bool CCJGW_DonglesComPort::FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf)
    {
        size_t a,b,c,d;
        sscanf_s(read_buf,"%02X%02X%02X%02X",&a,&b,&c,&d);
        file_buf[3] = a;
        file_buf[2] = b;
        file_buf[1] = c;
        file_buf[0] = d;
        return true;
    }
}

