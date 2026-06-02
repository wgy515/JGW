#include "StdAfx.h"
#include <JGW_WindowsFuncPlugin/CJGW_PerasoSocUsb.h>

namespace JGW
{
    CCJGW_PerasoSocUsb::CCJGW_PerasoSocUsb(CCJGW_CSerialComPort& cSerialComPort) : mcDonglesComPort(cSerialComPort)
    {
    }


    CCJGW_PerasoSocUsb::~CCJGW_PerasoSocUsb(void)
    {
    }

    bool CCJGW_PerasoSocUsb::OpenPerasoSocImpToSSH(const std::string& strHost,const std::string& strSrcAddr,size_t sshHostPort /* = 22 */,const std::string& strUserName /* = "root" */,const std::string& strPassword /* = "lion" */)
    {
        return false;
    }

    bool CCJGW_PerasoSocUsb::OpenPerasoSocImpToUsb(const std::wstring& strComPort)
    {
        return mcDonglesComPort.OpenDonglesComPort(strComPort);
    }

    void CCJGW_PerasoSocUsb::ClosePerasoSocImpToUsb()
    {
        mcDonglesComPort.CloseDonglesComPort();
    }

    void CCJGW_PerasoSocUsb::ClosePerasoSocImpToSSH()
    {

    }

    bool CCJGW_PerasoSocUsb::ReadMacAddress(std::wstring& strMacAddress)
    {
        return mcDonglesComPort.ReadMacAddress(strMacAddress);
    }

    bool CCJGW_PerasoSocUsb::ReadSerialNumber(std::wstring& strSerialNumber)
    {
        return mcDonglesComPort.ReadSerialNumber(strSerialNumber);
    }

    bool CCJGW_PerasoSocUsb::ReadFirmwareVersion(std::wstring& strFirmwareVersion)
    {
        return mcDonglesComPort.ReadFirmwareVersion(strFirmwareVersion);
    }

    bool CCJGW_PerasoSocUsb::ReadUsbSpeed(std::wstring& strUsbType)
    {
        return mcDonglesComPort.ReadUsbSpeed(strUsbType);
    }

    bool CCJGW_PerasoSocUsb::ReadOmniRssi(int& rssi)
    {
        return mcDonglesComPort.ReadOmniRssi(rssi);
    }

    bool CCJGW_PerasoSocUsb::ReadRssiToCommand(const std::string& strCommand,int& rssi)
    {
        return mcDonglesComPort.ReadRssiToCommand(strCommand,rssi);
    }

    bool CCJGW_PerasoSocUsb::ReadMcs(int& mcs)
    {
        return mcDonglesComPort.ReadMcs(mcs);
    }

    bool CCJGW_PerasoSocUsb::ReadWriteCommands(const std::string& strCommand,std::string& strRead)
    {
        return mcDonglesComPort.ReadWriteCommands(strCommand,strRead);
    }

    bool CCJGW_PerasoSocUsb::ReadRawWriteCommands(const char* strCommand,std::string& strRead)
    {
        return mcDonglesComPort.ReadRawWriteCommands(strCommand,strRead);
    }

    bool CCJGW_PerasoSocUsb::ReadWriteCommands(const char* strCommand,std::string& strRead)
    {
        return mcDonglesComPort.ReadWriteCommands(strCommand,strRead);
    }

    bool CCJGW_PerasoSocUsb::IsOpenPerasoSocImpToSSH()
    {
        return false;
    }

    bool CCJGW_PerasoSocUsb::IsOpenPerasoSocImpToUsb()
    {
        return mcDonglesComPort.IsOpenDonglesComPort();
    }

    bool CCJGW_PerasoSocUsb::ReadSectorStructToTxSectorCommand(std::wstring& strTxSector)
    {
        return mcDonglesComPort.ReadSectorStructToTxSectorCommand(strTxSector);
    }

    bool CCJGW_PerasoSocUsb::ReadSectorStructToRxSectorCommand(std::wstring& strRxSector)
    {
        return mcDonglesComPort.ReadSectorStructToRxSectorCommand(strRxSector);
    }

    bool CCJGW_PerasoSocUsb::PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount)
    {
        return mcDonglesComPort.PareseDonglesReadBlockBuf(output_buf,read_buf,mnMaxBlockCount);
    }

    bool CCJGW_PerasoSocUsb::FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf)
    {
        size_t a,b,c,d;
        sscanf_s(read_buf,"%02X%02X%02X%02X",&a,&b,&c,&d);
        file_buf[3] = a;
        file_buf[2] = b;
        file_buf[1] = c;
        file_buf[0] = d;
        return true;
    }

    std::string CCJGW_PerasoSocUsb::SubDonglesComPortString(const std::string& strRead,const char* pSub,const char* pDelimiter)
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
}
