#pragma once
#include "CJGW_CSerialComPort.h"

namespace JGW
{
    class JGWWINDOWSFUNCPLUGIN_CLASS CCJGW_DonglesComPort
    {
    public:
        CCJGW_DonglesComPort(CCJGW_CSerialComPort& cSerialComPort);
        ~CCJGW_DonglesComPort(void);

    public:
        //! 打开dongles端口
        bool OpenDonglesComPort(const std::wstring& strComPort);
        //! 关闭dongles端口
        void CloseDonglesComPort();
        //！
        bool ReadSerialNumberAndMacAddress(std::wstring& strSerialNumber,std::wstring& strMacAddress);
        //!
        bool ReadConfigPdbVersion(std::wstring& strConfigPdbVersion);
        //!
        bool ReadOtpPdbVersion(std::wstring& strOtpPdbVersion);
        //!
        bool ConfigureChannel(int channel);
        //! 读取MAC地址
        bool ReadMacAddress(std::wstring& strMacAddress);
        //! 读取SN
        bool ReadSerialNumber(std::wstring& strSerialNumber);
        //! 读取FW
        bool ReadFirmwareVersion(std::wstring& strFirmwareVersion);
        //! USB 协议版本
        bool ReadUsbSpeed(std::wstring& strUsbType);
        //! = xx-256
        bool ReadOmniRssi(int& rssi);
        //!
        bool ReadRssiToCommand(const std::string& strCommand,int& rssi);
        //!
        bool ReadMcs(int& mcs);
        //!
        bool ReadWriteCommands(const std::string& strCommand,std::string& strRead);
        //!
        bool ReadRawWriteCommands(const char* strCommand,std::string& strRead);
        //!
        bool ReadWriteCommands(const char* strCommand,std::string& strRead);
        //!
        bool IsOpenDonglesComPort(){ return mcSerialComPort.IsOpenComPort();}
        //! 根据TX SECTOR读取TXsector返回值
        bool ReadSectorStructToTxSectorCommand(std::wstring& strTxSector);
        //! 根据RX SECTOR读取RXsector返回值
        bool ReadSectorStructToRxSectorCommand(std::wstring& strRxSector);
        //! 解析DONGLES 块缓冲区
        bool PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount);
    private:     
        //! 
        bool FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf);
        //!
        std::string SubDonglesComPortString(const std::string& strRead,const char* pSub,const char* pDelimiter);
    private:
        CCJGW_CSerialComPort& mcSerialComPort;
    };
}