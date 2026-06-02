#pragma once
#include <string>
namespace JGW
{
    class /*JGWWINDOWSFUNCPLUGIN_CLASS*/ CCJGW_PerasoSocImp
    {
    public:
        //! 打开peraso设备
        virtual bool OpenPerasoSocImpToUsb(const std::wstring& strComPort) = 0;
        //! 
        virtual bool OpenPerasoSocImpToSSH(const std::string& strHost,const std::string& strSrcAddr,size_t sshHostPort = 22,const std::string& strUserName = "root",const std::string& strPassword = "lion") = 0;
        //!
        virtual void ClosePerasoSocImpToUsb() = 0;
        //!
        virtual void ClosePerasoSocImpToSSH() = 0;
        //! 读取MAC地址
        virtual bool ReadMacAddress(std::wstring& strMacAddress) = 0;
        //! 读取SN
        virtual bool ReadSerialNumber(std::wstring& strSerialNumber) = 0;
        //! 读取FW
        virtual bool ReadFirmwareVersion(std::wstring& strFirmwareVersion) = 0;
        //! USB 协议版本
        virtual bool ReadUsbSpeed(std::wstring& strUsbType) = 0;
        //! = xx-256
        virtual bool ReadOmniRssi(int& rssi) = 0;
        //!
        virtual bool ReadRssiToCommand(const std::string& strCommand,int& rssi) = 0;
        //! 
        virtual bool ReadMcs(int& mcs) = 0;
        //!
        virtual bool ReadWriteCommands(const std::string& strCommand,std::string& strRead) = 0;
        //!
        virtual bool ReadRawWriteCommands(const char* strCommand,std::string& strRead) = 0;
        //!
        virtual bool ReadWriteCommands(const char* strCommand,std::string& strRead) = 0;
        //! 
        virtual bool IsOpenPerasoSocImpToUsb() = 0;
        //!
        virtual bool IsOpenPerasoSocImpToSSH() = 0;

        //! 根据TX SECTOR读取TXsector返回值
        virtual bool ReadSectorStructToTxSectorCommand(std::wstring& strTxSector) = 0;
        //! 根据RX SECTOR读取RXsector返回值
        virtual bool ReadSectorStructToRxSectorCommand(std::wstring& strRxSector) = 0;
        //! 解析DONGLES 块缓冲区
        virtual bool PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount) = 0;
    protected:
        //! 
        virtual bool FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf) = 0;
        //!
        virtual std::string SubDonglesComPortString(const std::string& strRead,const char* pSub,const char* pDelimiter) = 0;

        ////! 打开dongles端口
        //virtual bool OpenDonglesComPort(const std::wstring& strComPort) = 0;
        ////! 关闭dongles端口
        //virtual void CloseDonglesComPort() = 0;
        ////! 读取MAC地址
        //virtual bool ReadMacAddress(std::wstring& strMacAddress) = 0;
        ////! 读取SN
        //virtual bool ReadSerialNumber(std::wstring& strSerialNumber) = 0;
        ////! 读取FW
        //virtual bool ReadFirmwareVersion(std::wstring& strFirmwareVersion) = 0;
        ////! USB 协议版本
        //virtual bool ReadUsbSpeed(std::wstring& strUsbType) = 0;
        ////! = xx-256
        //virtual bool ReadOmniRssi(int& rssi) = 0;
        ////!
        //virtual bool ReadMcs(int& mcs) = 0;
        ////!
        //virtual bool ReadWriteCommands(const std::string& strCommand,std::string& strRead) = 0;
        ////!
        //virtual bool ReadRawWriteCommands(const char* strCommand,std::string& strRead) = 0;
        ////!
        //virtual bool ReadWriteCommands(const char* strCommand,std::string& strRead) = 0;
        ////!
        //virtual bool IsOpenDonglesComPort() = 0;
        ////! 根据TX SECTOR读取TXsector返回值
        //virtual bool ReadSectorStructToTxSectorCommand(std::wstring& strTxSector) = 0;
        ////! 根据RX SECTOR读取RXsector返回值
        //virtual bool ReadSectorStructToRxSectorCommand(std::wstring& strRxSector) = 0;
        ////! 解析DONGLES 块缓冲区
        //virtual bool PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount) = 0;
    };
}