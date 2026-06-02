#pragma once
#include "CJGW_DonglesComPort.h"
#include "CCJGW_PerasoSocImp.h"

namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_PerasoSocUsb : public CCJGW_PerasoSocImp
    {
    public:
        CCJGW_PerasoSocUsb(CCJGW_CSerialComPort& cSerialComPort);
        ~CCJGW_PerasoSocUsb(void);

    public:
        //! 打开peraso设备
        virtual bool OpenPerasoSocImpToUsb(const std::wstring& strComPort);
        //! 
        virtual bool OpenPerasoSocImpToSSH(const std::string& strHost,const std::string& strSrcAddr,size_t sshHostPort = 22,const std::string& strUserName = "root",const std::string& strPassword = "lion");
        //!
        virtual void ClosePerasoSocImpToUsb();
        //!
        virtual void ClosePerasoSocImpToSSH();
        //! 读取MAC地址
        virtual bool ReadMacAddress(std::wstring& strMacAddress);
        //! 读取SN
        virtual bool ReadSerialNumber(std::wstring& strSerialNumber);
        //! 读取FW
        virtual bool ReadFirmwareVersion(std::wstring& strFirmwareVersion);
        //! USB 协议版本
        virtual bool ReadUsbSpeed(std::wstring& strUsbType);
        //! = xx-256
        virtual bool ReadOmniRssi(int& rssi);
        //!
        virtual bool ReadRssiToCommand(const std::string& strCommand,int& rssi);
        //! 
        virtual bool ReadMcs(int& mcs);
        //!
        virtual bool ReadWriteCommands(const std::string& strCommand,std::string& strRead);
        //!
        virtual bool ReadRawWriteCommands(const char* strCommand,std::string& strRead);
        //!
        virtual bool ReadWriteCommands(const char* strCommand,std::string& strRead);
        //! 
        virtual bool IsOpenPerasoSocImpToUsb();
        //!
        virtual bool IsOpenPerasoSocImpToSSH();

        //! 根据TX SECTOR读取TXsector返回值
        virtual bool ReadSectorStructToTxSectorCommand(std::wstring& strTxSector);
        //! 根据RX SECTOR读取RXsector返回值
        virtual bool ReadSectorStructToRxSectorCommand(std::wstring& strRxSector);
        //! 解析DONGLES 块缓冲区
        virtual bool PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount);
    private:
        //! 
        virtual bool FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf);
        //!
        virtual std::string SubDonglesComPortString(const std::string& strRead,const char* pSub,const char* pDelimiter);
    private:
        CCJGW_DonglesComPort mcDonglesComPort;
        //CCJGW_CSerialComPort& mcSerialComPort;
    };
}

