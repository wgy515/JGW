#include "StdAfx.h"
#include <JGW_SSHPlugin/CJGW_DonglesSSH.h>
#include <JGW_WindowsFuncPlugin/PerasoDefine.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

namespace JGW
{
    CCJGW_DonglesSSH::CCJGW_DonglesSSH(CCJGW_SSHShell& sshShell,const std::string& strBinPath /* = "/userdata/bin/prs_serial " */):mstrBinPath(strBinPath),msshShell(sshShell)
    {
    }


    CCJGW_DonglesSSH::~CCJGW_DonglesSSH(void)
    {
    }

    bool CCJGW_DonglesSSH::OpenPerasoSocImpToSSH(const std::string& strHost,const std::string& strSrcAddr = "",size_t sshHostPort /* = 22 */,const std::string& strUserName /* = "root" */,const std::string& strPassword /* = "lion" */)
    {
        std::string strRead;
        if (!msshShell.ConnectSSHShell(strHost,strUserName,strPassword,strSrcAddr.empty()?NULL:strSrcAddr.c_str(),sshHostPort))
        {
            Log4WE_F(L"Connect %s SSH Fail",JGW_A2W(strHost).c_str());
            return false;
        }

        if (!msshShell.RecvSSHShell(strRead))
        {
            Log4WE_F(L"Recv SSH Fail");
            return false;
        }
        return true;
    }

    bool CCJGW_DonglesSSH::OpenPerasoSocImpToUsb(const std::wstring& strComPort)
    {
        return false;
    }

    void CCJGW_DonglesSSH::ClosePerasoSocImpToUsb()
    {
        
    }

    void CCJGW_DonglesSSH::ClosePerasoSocImpToSSH()
    {
        msshShell.DisConnectSSHShell();
    }

    bool CCJGW_DonglesSSH::ReadMacAddress(std::wstring& strMacAddress)
    {
        std::string strSerialNumberCommand = mstrBinPath + "\"mib q b 60-1-1-1\"\n",strRead;
        if (!msshShell.RWSSHShell(strSerialNumberCommand,strRead)) return false;
        JGW_ReplaceStringA(strRead," ","");
        if (MAC_ADDRESS_MAX_LEN != strRead.length()) return false;
        strMacAddress = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_DonglesSSH::ReadSerialNumber(std::wstring& strSerialNumber)
    {
        std::string strSerialNumberCommand = mstrBinPath + "\"mib q l 60-1-1-2\"\n",strRead;
        if (!msshShell.RWSSHShell(strSerialNumberCommand,strRead)) return false;
        unsigned char szBuf[MAX_SERIAL_NUMBER_FALSH_BIT + 1] = {0};
        if (!PareseDonglesReadBlockBuf(szBuf,strRead.c_str(),MAX_SERIAL_NUMBER_FALSH_BIT / 4)) return false;
        strSerialNumber = (wchar_t*)szBuf;
        return true;
    }

    bool CCJGW_DonglesSSH::ReadFirmwareVersion(std::wstring& strFirmwareVersion)
    {
        std::string strCommand = mstrBinPath + "\"version\"\n",strRead;
        if (!msshShell.RWSSHShell(strCommand,strRead)) return false;
        std::string firmware_version = SubDonglesComPortString(strRead,"Falcon","\n");
        if (firmware_version.empty()) return false;
        JGW_ReplaceStringA(firmware_version," ","");
        JGW_ReplaceStringA(firmware_version,"Falcon","");
        JGW_ReplaceStringA(firmware_version,"UMAC","");
        JGW_ReplaceStringA(firmware_version,"Windows","");
        strFirmwareVersion = JGW_A2W(firmware_version);
        return true;
    }

    bool CCJGW_DonglesSSH::ReadUsbSpeed(std::wstring& strUsbType)
    {
        std::string strCommand = mstrBinPath + "\"status\"\n",strRead;
        if (!msshShell.RWSSHShell(strCommand,strRead)) return false;
        std::string firmware_version = SubDonglesComPortString(strRead,"speed","\n");
        if (firmware_version.empty()) return false;
        JGW_ReplaceStringA(firmware_version," ","");
        JGW_ReplaceStringA(firmware_version,"speed","");
        JGW_ReplaceStringA(firmware_version,":","");
        strUsbType = JGW_A2W(firmware_version);
        return true;
    }

    bool CCJGW_DonglesSSH::ReadOmniRssi(int& rssi)
    {
        std::string strCommand = mstrBinPath + "\"mib q b 12-1-1-d\"\n",strRead;
        if (!msshShell.RWSSHShell(strCommand,strRead)) return false;
        if (strRead.empty() || 2 != strRead.length()) return false;
        sscanf_s(strRead.c_str(),"%X",&rssi);
        rssi -= 256;
        return true;
    }

    bool CCJGW_DonglesSSH::ReadRssiToCommand(const std::string& strCommand,int& rssi)
    {
        std::string strSSHCommand,strRead;
        JGW_FormatString(strSSHCommand,"%s\"%s\"\n",mstrBinPath.c_str(),strCommand);
        if (!msshShell.RWSSHShell(strSSHCommand,strRead)) return false;
        if (strRead.empty() || 2 != strRead.length()) return false;
        sscanf_s(strRead.c_str(),"%X",&rssi);
        rssi -= 256;
        return true;
    }

    bool CCJGW_DonglesSSH::ReadMcs(int& mcs)
    {
        std::string strCommand = mstrBinPath + "\"mib q b 12-8-1-a\"\n",strRead;
        if (!msshShell.RWSSHShell(strCommand.c_str(),strRead)) return false;
        if (strRead.empty() || 2 != strRead.length()) return false;
        sscanf_s(strRead.c_str(),"%X",&mcs);
        return true;
    }

    bool CCJGW_DonglesSSH::ReadWriteCommands(const std::string& strCommand,std::string& strRead)
    {
        return ReadWriteCommands(strCommand.c_str(),strRead);
    }

    bool CCJGW_DonglesSSH::ReadWriteCommands(const char* strCommand,std::string& strRead)
    {
        //!std::string strSSHCommand = mstrBinPath + strCommand;
        std::string strSSHCommand;
        JGW_FormatString(strSSHCommand,"%s\"%s\"\n",mstrBinPath.c_str(),strCommand);
        if (!msshShell.RWSSHShell(strSSHCommand,strRead)) return false;
        return true;
    }

    bool CCJGW_DonglesSSH::ReadRawWriteCommands(const char* strCommand,std::string& strRead)
    {
        return ReadWriteCommands(strCommand,strRead);
    }

    bool CCJGW_DonglesSSH::IsOpenPerasoSocImpToUsb()
    {
        return false;
    }

    bool CCJGW_DonglesSSH::IsOpenPerasoSocImpToSSH()
    {
        return msshShell.IsSSHConnected();
    }

    bool CCJGW_DonglesSSH::ReadSectorStructToTxSectorCommand(std::wstring& strTxSector)
    {
        std::string strSSHCommand = mstrBinPath + "\"mib q b 12-8-1-c\"\n",strRead;
        if (!msshShell.RWSSHShell(strSSHCommand,strRead)) return false;
        strTxSector = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_DonglesSSH::ReadSectorStructToRxSectorCommand(std::wstring& strRxSector)
    {
        std::string strSSHCommand = mstrBinPath + "\"mib q b 12-8-1-d\"\n",strRead;
        if (!msshShell.RWSSHShell(strSSHCommand,strRead)) return false;
        strRxSector = JGW_A2W(strRead);
        return true;
    }

    bool CCJGW_DonglesSSH::PareseDonglesReadBlockBuf(unsigned char* output_buf,const char* read_buf,size_t mnMaxBlockCount)
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

    std::string CCJGW_DonglesSSH::SubDonglesComPortString(const std::string& strRead,const char* pSub,const char* pDelimiter)
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

    bool CCJGW_DonglesSSH::FormattedReadLittleEndianBlock(unsigned char* file_buf,const char* read_buf)
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
