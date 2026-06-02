#pragma once
#include "CJGW_AdbClient.h"
#include <poco/Net/StreamSocket.h>
namespace JGW
{
    class CCJGW_AdbPocoClient
    {
    public:
        CCJGW_AdbPocoClient(void);
        ~CCJGW_AdbPocoClient(void);
    public:
        void SetConnectAdbHostInfo(int connectTimeOut = 10 * 1000,const char* server_name = "127.0.0.1",const char* port = DEFAULT_ADB_PORT_CHAR,int send_recv_timeout = 10 * 1000);
    private:
        bool ConnectAdbHost(int connectTimeOut = 10 * 1000,const char* server_name = "127.0.0.1",const char* port = DEFAULT_ADB_PORT_CHAR,int send_recv_timeout = 10 * 1000);
        void DisConnectAdbHost();
    public:
        //! adb version
        bool GetAdbHostVersion(int& version);
        //! adb devices
        bool GetAdbDevices(std::string& recv);
        //! adb root remount reboot reboot-bootloader tcpip usb
        //! root: remount: reboot: reboot-bootloader: tcpip: usb:
        //! root:argv[1] remount:argv[1] reboot:argv[1] reboot-bootloader: tcpip:argv[1] usb:argv[1]
        bool AdbDevicesCommandRequest(const std::string& cmd,std::string& recv,const char* argv = NULL,transport_type ttype = kTransportAny, const char* serial = NULL);
        //! start shell
        bool StartAdbShell(transport_type ttype = kTransportAny, const char* serial = NULL);
        //!
        bool SendAdbShellCommand(const std::string& command);
        //!
        bool RecvAdbShellCommand(std::string& recv);
        //! 
        void StopAdbShell();
    public:
        //! 
        bool SendAdbProtocolCommand(const std::string& cmd,transport_type ttype = kTransportAny, const char* serial = NULL);
        //! 
        void AdbRead(std::string& recv);
        //! 3. As a special exception, for 'host:version', a 4-byte hex string corresponding to the server's internal version number
        bool AdbQuery(std::string& recv);
    private:
        std::string FormatHostSerialCommand(const std::string& cmd,transport_type ttype = kTransportAny, const char* serial = NULL);
        //! 1.A 4-byte hexadecimal string giving the length of the payload
        //! 2.Followed by the payload itself.
        std::string FormatAdbSendProtocol(const std::string& cmd);
        //! 1. For success, the 4-byte "OKAY" string
        //! 2. For failure, the 4-byte "FAIL" string, followed by a 4-byte hex length, followed by a string giving the reason for failure.
        bool GetAdbStatus();

        //!
        bool BindAdbSerialTransport(transport_type ttype = kTransportAny, const char* serial = NULL);
        //! 
        bool CheckSocketConnect();
    private:
        bool mbConnected;
        int mnConnectTimeOut;
        int mnSendRecvTimeOut;
        std::string mstrPort;
        std::string mstrServerName;
        std::string mstrErrorMsg;
        Poco::Net::StreamSocket mStreamSocket;
    };
}
