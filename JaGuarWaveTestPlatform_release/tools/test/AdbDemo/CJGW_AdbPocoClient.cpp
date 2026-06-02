#include "StdAfx.h"
#include "CJGW_AdbPocoClient.h"
#include <poco/Timespan.h>

namespace JGW
{
    CCJGW_AdbPocoClient::CCJGW_AdbPocoClient(void) : mnConnectTimeOut(10 * 1000),mnSendRecvTimeOut(10 * 1000),mstrPort(DEFAULT_ADB_PORT_CHAR),mstrServerName("127.0.0.1"),mbConnected(false)
    {
    }


    CCJGW_AdbPocoClient::~CCJGW_AdbPocoClient(void)
    {
    }

    void CCJGW_AdbPocoClient::SetConnectAdbHostInfo(int connectTimeOut /* = 10 * 1000 */,const char* server_name /* = "127.0.0.1" */,const char* port /* = DEFAULT_ADB_PORT_CHAR */,int send_recv_timeout /* = 10 * 1000 */)
    {
        mnConnectTimeOut = connectTimeOut;
        mnSendRecvTimeOut = send_recv_timeout;
        mstrServerName = server_name ? server_name : "";
        mstrPort = port ? port : "";
    }

    bool CCJGW_AdbPocoClient::ConnectAdbHost(int connectTimeOut /* = 10 * 1000 */,const char* server_name /* = "127.0.0.1" */,const char* port /* = DEFAULT_ADB_PORT_CHAR */,int send_recv_timeout /* = 10 * 1000 */)
    {
        mStreamSocket.close();
        Poco::Timespan timeout(connectTimeOut);
        try
        {
            mStreamSocket.connect(Poco::Net::SocketAddress(server_name,static_cast<unsigned short>(atoi(port))),timeout);
            Poco::Timespan sendRecvTimeout(send_recv_timeout);
            mStreamSocket.setReceiveTimeout(sendRecvTimeout);
            mStreamSocket.setSendTimeout(sendRecvTimeout);
            mbConnected = true;
            return true;
        }
        catch (Poco::Exception& e)
        {
            mstrErrorMsg = e.message();
        }
        return false;
    }

    void CCJGW_AdbPocoClient::DisConnectAdbHost()
    {
        mStreamSocket.close();
        mbConnected = false;
    }

    std::string CCJGW_AdbPocoClient::FormatHostSerialCommand(const std::string& cmd,transport_type ttype /* = kTransportAny */, const char* serial /* = NULL */)
    {
        if (serial)
        {
            return JGW_GetFormatString("host-serial:%s:%s",serial,cmd.c_str());
        }
        const char* prefix = "host";
        if (ttype == kTransportUsb)
            prefix = "host-usb";
        else if (ttype == kTransportLocal)
            prefix = "host-local";
        else if (ttype == kTransportRaw)
            return cmd;
        return JGW_GetFormatString("%s:%s",prefix,cmd.c_str());
    }

    //static void format_host_command(char* buffer, size_t  buflen, const char* command, transport_type ttype, const char* serial)
    //{
    //    if (serial) {
    //        _snprintf(buffer, buflen, "host-serial:%s:%s", serial, command);
    //    } else {
    //        const char* prefix = "host";
    //        if (ttype == kTransportUsb)
    //            prefix = "host-usb";
    //        else if (ttype == kTransportLocal)
    //            prefix = "host-local";

    //        _snprintf(buffer, buflen, "%s:%s", prefix, command);
    //    }
    //}

    std::string CCJGW_AdbPocoClient::FormatAdbSendProtocol(const std::string& cmd)
    {
        std::string formatCmd;
        JGW_FormatString(formatCmd,"%04x%s",cmd.length(),cmd.c_str());
        return formatCmd;
    }

    bool CCJGW_AdbPocoClient::GetAdbHostVersion(int& version)
    {
        std::string recv;
        std::string cmd = FormatHostSerialCommand("version");

        if (!SendAdbProtocolCommand(cmd) || !AdbQuery(recv))
        {
            return false;
        }

        if (sscanf(recv.c_str(), "%04x", &version) != 1)
        {
            mstrErrorMsg = "unknown host version";
            return false;
        }
        return true;
    }

    bool CCJGW_AdbPocoClient::GetAdbDevices(std::string& recv)
    {
        std::string cmd = FormatHostSerialCommand("devices");
        if (!SendAdbProtocolCommand(cmd) || AdbQuery(recv))
        {
            return false;
        }
        return true;
    }

    bool CCJGW_AdbPocoClient::GetAdbStatus()
    {
        char buf[5] = {0};
        size_t len = 0;

        if (4 != mStreamSocket.receiveBytes(buf,4))
        {
            mstrErrorMsg = "protocol fault (no status)";
            return false;
        }

        if (memcpy(buf,ADB_OKAY,4))
        {
            return true;
        }

        if(memcmp(buf, "FAIL", 4)) 
        {
            JGW_FormatString(mstrErrorMsg,
                "protocol fault (status %02x %02x %02x %02x?!)",
                buf[0], buf[1], buf[2], buf[3]);
            return false;
        }

        if (4 != mStreamSocket.receiveBytes(buf,4))
        {
            mstrErrorMsg = "protocol fault (status len)";
            return false;
        }

        buf[4] = 0;
        len = strtoul((char*)buf, 0, 16);
        if(len > 255) len = 255;

        mstrErrorMsg.resize(len);
        if (len != mStreamSocket.receiveBytes(const_cast<char*>(mstrErrorMsg.data()),len))
        {
            mstrErrorMsg = "protocol fault (status read)";
            return false;
        }
        return false;
    }

    bool CCJGW_AdbPocoClient::AdbQuery(std::string& recv)
    {
        char buf[5] = {0};
        if (4 != mStreamSocket.receiveBytes(buf,4))
        {
            mstrErrorMsg = "protocol fault (recv len)";
            return false;
        }

        buf[4] = 0;
        size_t len = strtoul(buf, 0, 16);
        if(len > 1024)
        {
            mstrErrorMsg = "recv len out fo range";
            return false;
        }

        recv.resize(len);
        if (len != mStreamSocket.receiveBytes(const_cast<char*>(recv.data()),len))
        {
            mstrErrorMsg = "recv fault (read)";
            return false;
        }
        return true;
    }

    //static int switch_socket_transport(int fd)
    //{
    //    char service[64];
    //    char tmp[5];
    //    int len;

    //    if (__adb_serial)
    //        snprintf(service, sizeof service, "host:transport:%s", __adb_serial);
    //    else {
    //        char* transport_type = "???";

    //        switch (__adb_transport) {
    //        case kTransportUsb:
    //            transport_type = "transport-usb";
    //            break;
    //        case kTransportLocal:
    //            transport_type = "transport-local";
    //            break;
    //        case kTransportAny:
    //            transport_type = "transport-any";
    //            break;
    //        case kTransportHost:
    //            // no switch necessary
    //            return 0;
    //            break;
    //        }

    //        snprintf(service, sizeof service, "host:%s", transport_type);
    //    }
    //    len = strlen(service);
    //    snprintf(tmp, sizeof tmp, "%04x", len);

    //    if(writex(fd, tmp, 4) || writex(fd, service, len)) {
    //        strcpy(__adb_error, "write failure during connection");
    //        adb_close(fd);
    //        return -1;
    //    }
    //    D("Switch transport in progress\n");

    //    if(adb_status(fd)) {
    //        adb_close(fd);
    //        D("Switch transport failed\n");
    //        return -1;
    //    }
    //    D("Switch transport success\n");
    //    return 0;
    //}

    bool CCJGW_AdbPocoClient::BindAdbSerialTransport(transport_type ttype /* = kTransportAny */, const char* serial /* = NULL */)
    {
        std::string cmd;
        if (serial)
        {
            JGW_FormatString(cmd,"host:transport:%s",serial);
        }
        else
        {
            char* transport_type = "???";

            switch (ttype) 
            {
            case kTransportUsb:
                transport_type = "transport-usb";
                break;
            case kTransportLocal:
                transport_type = "transport-local";
                break;
            case kTransportAny:
                transport_type = "transport-any";
                break;
            case kTransportHost:
                // no switch necessary
                return true;
            }
            JGW_FormatString(cmd,"host:%s",transport_type);
        }
        std::string command = FormatAdbSendProtocol(cmd);
        if (!CheckSocketConnect() || command.length() != mStreamSocket.sendBytes(command.c_str(),command.length()))
        {
            mstrErrorMsg = "write failure during connection";
            return false;
        }

        if (!GetAdbStatus())
        {
            return false;
        }
        return true;
    }

    bool CCJGW_AdbPocoClient::SendAdbProtocolCommand(const std::string& cmd,transport_type ttype /* = kTransportAny */, const char* serial /* = NULL */)
    {
#if 0
        std::string command = FormatHostSerialCommand(cmd,ttype,serial);
        command = FormatAdbSendProtocol(command);
#else
        std::string command = FormatAdbSendProtocol(cmd);
#endif

        if (!ConnectAdbHost(mnConnectTimeOut,mstrServerName.empty() ? NULL : mstrServerName.c_str(),mstrPort.empty() ? NULL : mstrPort.c_str(),mnSendRecvTimeOut))
        {
            mstrErrorMsg = "Connet Adb Host Fail";
            return false;
        }

        if (memcmp(command.c_str(),"host",4) != 0 && !BindAdbSerialTransport(ttype,serial)) 
        {
            return false;
        }

        if (!CheckSocketConnect() || command.length() != mStreamSocket.sendBytes(command.c_str(),command.length()))
        {
            mstrErrorMsg = "Send Adb Command Fail";
            return false;
        }

        if (!GetAdbStatus())
        {
            return false;
        }

        return true;
    }

    bool CCJGW_AdbPocoClient::CheckSocketConnect()
    {
        //struct tcp_info info; 
        //int len=sizeof(info); 
        //getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len);
        //if((info.tcpi_state==TCP_ESTABLISHED)) 
        return mbConnected;
    }

    void CCJGW_AdbPocoClient::AdbRead(std::string& recv)
    {
        /*setsockopt(rs, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive));
        setsockopt(rs, SOL_TCP, TCP_KEEPIDLE, (void*)&keepIdle, sizeof(keepIdle));
        setsockopt(rs, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
        setsockopt(rs, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));*/

        char buf[4097] = {0};
        recv = "";
        while (CheckSocketConnect())
        {
            int len = mStreamSocket.receiveBytes(buf,4096);
            if (len > 0)
            { 
                recv += buf;
                memset(buf,0x00,4097);
            }
            else if (len < 0 && errno != EINTR)
            {
                break;
            }
            else if (len == 0)
            {
                break;
            }
        }
        DisConnectAdbHost();
    }

    bool CCJGW_AdbPocoClient::AdbDevicesCommandRequest(const std::string& cmd,std::string& recv,const char* argv /* = NULL */,transport_type ttype /* = kTransportAny */, const char* serial /* = NULL */)
    {
        std::string command (cmd); 
        if (NULL != argv)
        {
            command += argv;
        }
        if (!SendAdbProtocolCommand(command))
        {
            return false;
        }
        AdbRead(recv);

        return true;
    }

    bool CCJGW_AdbPocoClient::StartAdbShell(transport_type ttype /* = kTransportAny */, const char* serial /* = NULL */)
    {
        std::string command ("shell:"); 
        if (!SendAdbProtocolCommand(command))
        {
            return false;
        }
        return true;
    }

    void CCJGW_AdbPocoClient::StopAdbShell()
    {
        DisConnectAdbHost();
    }

    bool CCJGW_AdbPocoClient::SendAdbShellCommand(const std::string& command)
    {
        if (!CheckSocketConnect() || command.length() != mStreamSocket.sendBytes(command.c_str(),command.length()))
        {
            mstrErrorMsg = "Send Adb Command Fail";
            return false;
        }
        return true;
    }

    bool CCJGW_AdbPocoClient::RecvAdbShellCommand(std::string& recv)
    {
        char buf[4097] = {0};
        if (!CheckSocketConnect() || mStreamSocket.receiveBytes(buf,4096) < 0)
        {
            mstrErrorMsg = "Recv Adb Shell Fail";
            recv = "";
            return false;
        }
        recv = buf;
        return true;
    }
}
