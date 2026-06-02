#include "StdAfx.h"
#include "CJGW_ReportStationStatusThread.h"
#include <JGW_Wlan/CJGW_WlanInterfaceImpl.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/URI.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#define TX_SECTOR_COMMAND "mib q b 12-8-1-c"
#define RX_SECTOR_COMMAND "mib q b 12-8-1-d"

namespace JGW
{
    CCJGW_ReportStationStatusThread::CCJGW_ReportStationStatusThread(std::string& strStationServerAddress) : mDonglesComPort(mCSerialComPort),mstrStationServerAddress(strStationServerAddress),mstrContentType("application/json"),mpAsyncWndMessage(NULL),mbGateWayServer(true),mnWEBRequestErrorCount(0)
    {
    }

    CCJGW_ReportStationStatusThread::~CCJGW_ReportStationStatusThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_ReportStationStatusThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_ReportStationStatusThread::OnReportStationStatus)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_ReportStationStatusThread::OnReportStationStatus(WPARAM wParam,LPARAM lParam)
    {
        mpsStationMonitorUiInfo = (PS_STATION_MONITOR_UI_INFO)wParam;
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)lParam;
        InitStationMonitorUIInfo();

        //! 检查PERASO设备是否已经插入
        if (!OpenPerasoComPort())
        {
            mpsStationMonitorUiInfo->mstrIPAddress = L"";
            mpsStationMonitorUiInfo->mstrMacAddress = L"";
            goto ON_REPORT_END;
        }
#if 0
        //! 检查PERASO WIFI是否已经连接
        if (!CheckPerasoWiFiIsConnected())
        {
            mbGateWayServer = true;
            mnWEBRequestErrorCount = 0;
            mpsStationMonitorUiInfo->mstrIPAddress = L"";
            goto ON_REPORT_END;
        }
#endif
        //! 获取peraso wifi ip address
        if (!GetPerasoWiFiIPAddress())
        {
            mbGateWayServer = true;
            mnWEBRequestErrorCount = 0;
            mpsStationMonitorUiInfo->mstrIPAddress = L"";
            goto ON_REPORT_END;
        }

        //! 获取peraso设备相关信息
        if (!GetPerasoStatusInfo())
        {
            goto ON_REPORT_END;
        }
        //! 赋值
        {
            CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mMacAddressCriticalSectionLock);
            mRepStatusJson.mac = mpsStationMonitorUiInfo->mstrAnsiMacAddress;
        }
        {
            CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mTrafficSpeedCriticalSectionLock);
            mRepStatusJson.tx_speed = mpsStationMonitorUiInfo->mstrUploadSpeed;
            mRepStatusJson.rx_speed = mpsStationMonitorUiInfo->mstrDownloadSpeed;
        }
        mRepStatusJson.rx_drop_rate = mpsStationMonitorUiInfo->mfRxDropRate;
        mRepStatusJson.tx_drop_rate = mpsStationMonitorUiInfo->mfTxDropRate;

        //! 上报station 设备信息
        if (!ReportStationDeviceInfo())
        {
            goto ON_REPORT_END;
        }
ON_REPORT_END: 
        mDonglesComPort.CloseDonglesComPort();
        mpAsyncWndMessage->PutAsyncMessage(WM_HWND_REPORT_INFO_MSG);
    }

    bool CCJGW_ReportStationStatusThread::OpenPerasoComPort()
    {
        std::wstring strComPort;

        strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
        if (strComPort.empty()) return false;
        return mDonglesComPort.OpenDonglesComPort(strComPort.c_str());
    }
#if 0
    bool CCJGW_ReportStationStatusThread::CheckPerasoWiFiIsConnected()
    {

        if (!GetWiFiInterfaceImpl()->OpenWlanHandle())
        {
            return false;
        }

        if (!GetWiFiInterfaceImpl()->EnumWlanInterface(L"Peraso"))
        {
            Log4WI(L"Not Find Peraso Net Card");
            GetWiFiInterfaceImpl()->CloseWlanHandle();
            return false;
        }

        if (!GetWiFiInterfaceImpl()->CheckWlanIsConnected(""))
        {
            Log4WI(L"Peraso Net Card Not Connect Wlan");
            GetWiFiInterfaceImpl()->CloseWlanHandle();
            return false;
        }
        GetWiFiInterfaceImpl()->CloseWlanHandle();
        return true;
    }
#endif

    bool CCJGW_ReportStationStatusThread::GetPerasoWiFiIPAddress()
    {
        std::vector<NetWorkConection>& vConnections = mAdapterCommonInfo.GetNetAdaptersInfo();

        for (size_t i = 0;i < vConnections.size();i ++)
        {
            if (NULL != JGW_StrCaseStr(vConnections[i].description.c_str(),"Peraso"))
            {
                mpsStationMonitorUiInfo->mstrNetCardConnectName = vConnections[i].description;
                if (!mpsStationMonitorUiInfo->mbConnectPeraso) return false;
                mRepStatusJson.ip = JGW_W2A(vConnections[i].ip_address);
                if (mpsStationMonitorUiInfo->mstrIPAddress.empty() || NULL != vConnections[i].ip_address.compare(mpsStationMonitorUiInfo->mstrIPAddress))
                {
                    CCJGW_CriticalSectionAutoLock lock(mpsStationMonitorUiInfo->mIPAddressCriticalSectionLock);
                    mpsStationMonitorUiInfo->mstrIPAddress = vConnections[i].ip_address;
                }
                mpsStationMonitorUiInfo->mstrGatewayAddress = vConnections[i].default_gateway;
                if (mpsStationMonitorUiInfo->mstrGatewayAddress.empty()) mpsStationMonitorUiInfo->mstrGatewayAddress = L"192.168.213.1";
                return true;
            }
        }

        //mpsStationMonitorUiInfo->mstrNetCardConnectName = JGW_W2A(GetWiFiInterfaceImpl()->GetWlanInterfaceDescriptionToEnumKey());
      
        //for (size_t i = 0;i < vConnections.size();i ++)
        //{
        //    if (NULL == JGW_StrComparenoCaseStr(mpsStationMonitorUiInfo->mstrNetCardConnectName.c_str(),vConnections[i].description.c_str()))
        //    {
        //        mRepStatusJson.ip = JGW_W2A(vConnections[i].ip_address);
        //        if (mpsStationMonitorUiInfo->mstrIPAddress.empty() || NULL != vConnections[i].ip_address.compare(mpsStationMonitorUiInfo->mstrIPAddress))
        //        {
        //            mpsStationMonitorUiInfo->mstrIPAddress = vConnections[i].ip_address;
        //        }
        //        mpsStationMonitorUiInfo->mstrGatewayAddress = vConnections[i].default_gateway;
        //        if (mpsStationMonitorUiInfo->mstrGatewayAddress.empty()) mpsStationMonitorUiInfo->mstrGatewayAddress = L"192.168.213.1";
        //        return true;
        //    }
        //}
        return false;
    }

    bool CCJGW_ReportStationStatusThread::GetPerasoStatusInfo()
    {
        std::wstring strCurrentTxSector,strCurrentRxSector;

        mRepStatusJson.tx_mcs = -1;
        mDonglesComPort.ReadMcs(mRepStatusJson.tx_mcs);
        JGW_FormatWString(mpsStationMonitorUiInfo->mstrTxMcs,L"%d",mRepStatusJson.tx_mcs);
        mpsStationMonitorUiInfo->mstrRxMcs = mpsStationMonitorUiInfo->mstrTxMcs;
        mRepStatusJson.rx_mcs = mRepStatusJson.tx_mcs;
        mRepStatusJson.rssi = -128;
        mRepStatusJson.rx_sector = -1;
        mRepStatusJson.tx_sector = -1;

        if (mDonglesComPort.ReadSectorStructToTxSectorCommand(strCurrentTxSector) && std::wstring::npos != strCurrentTxSector.find(L"00 "))
        {
            JGW_ReplaceStringW(strCurrentTxSector,L"00 ",L"");
            mpsStationMonitorUiInfo->mstrTxSector = strCurrentTxSector;
            _stscanf_s(strCurrentTxSector.c_str(),L"%X",&mRepStatusJson.tx_sector);
        }

        if (mDonglesComPort.ReadSectorStructToTxSectorCommand(strCurrentRxSector) && std::wstring::npos != strCurrentRxSector.find(L"00 "))
        {
            JGW_ReplaceStringW(strCurrentRxSector,L"00 ",L"");
            _stscanf_s(strCurrentRxSector.c_str(),L"%X",&mRepStatusJson.rx_sector);
        }

        mpsStationMonitorUiInfo->mstrTxSector = strCurrentTxSector;
        mpsStationMonitorUiInfo->mstrRxSector = strCurrentRxSector;
        mDonglesComPort.ReadOmniRssi(mRepStatusJson.rssi);
        JGW_FormatWString(mpsStationMonitorUiInfo->mstrRssi,L"%d",mRepStatusJson.rssi);

        return true;
    }

    std::string gulp(std::istream &in)
    {
        std::string ret;
        char buffer[4096];
        while (in.read(buffer, sizeof(buffer)))
            ret.append(buffer, sizeof(buffer));
        ret.append(buffer, (size_t)in.gcount());
        return ret;
    }

    bool CCJGW_ReportStationStatusThread::ReportStationDeviceInfo()
    {
        std::string strUrl,strSourceAddr = mRepStatusJson.ip,strRead;

        {
            CCJGW_CriticalSectionAutoLock autoLock(mpsStationMonitorUiInfo->mStationStatusJsonCriticalSectionLock);
            mpsStationMonitorUiInfo->mstrStationStatusJson =  mRepStatusJson.BuildStatusJson();
        }

        if (mbGateWayServer)
        {
            JGW_FormatString(strUrl,mstrStationServerAddress.c_str(),JGW_W2A(mpsStationMonitorUiInfo->mstrGatewayAddress).c_str());
        }
        else
        {
            strUrl = "http://mmwlogin.net/cgi-bin/leopard.fcgi";
        }
        try
        {
            Poco::URI url(strUrl);
            Poco::Net::HTTPClientSession session(url.getHost(),url.getPort());
            Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST,url.getPath(),Poco::Net::HTTPRequest::HTTP_1_1);

            request.setChunkedTransferEncoding(false);
            request.setContentType(mstrContentType);
            request.setContentLength(mpsStationMonitorUiInfo->mstrStationStatusJson.length());
            //! 如果非空则绑定源地址
            if (!strSourceAddr.empty())
            {
                sockaddr_in sin;
                sin.sin_family = AF_INET;
                sin.sin_port = 0;
                sin.sin_addr.S_un.S_addr = inet_addr(strSourceAddr.c_str());
                Poco::Net::SocketAddress sourceAddress((LPSOCKADDR)&sin, sizeof(sin));
                session.socket().impl()->bind(sourceAddress);
                session.setKeepAlive(true);
                Poco::Timespan time(60*60,0);
                session.setKeepAliveTimeout(time);

                Poco::Net::SocketAddress addr(url.getHost(),url.getPort());
                session.socket().connect(addr,60000000);
                session.socket().setReceiveTimeout(60000000);
                session.socket().setNoDelay(true);
            }
            session.sendRequest(request) << mpsStationMonitorUiInfo->mstrStationStatusJson;

            Poco::Net::HTTPResponse resp;
            std::istream & is = session.receiveResponse(resp);
            strRead = gulp(is);

            Log4AI_F("w: %s",mpsStationMonitorUiInfo->mstrStationStatusJson.c_str());

            return true;
        }
        catch(Poco::Net::NetException & ex)
        {
            Log4AE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
        }
        catch(Poco::Exception& ex)
        {
            Log4AE_F("Post HTTP Fail(%s)",ex.displayText().c_str()); 
        }

        if (++mnWEBRequestErrorCount >= WEB_MAX_REQUEST_ERROR_COUNT) 
        {
            mnWEBRequestErrorCount = 0;
            mbGateWayServer = !mbGateWayServer;
        }
        return false;
    }

    void CCJGW_ReportStationStatusThread::InitStationMonitorUIInfo()
    {
        //         mpsStationMonitorUiInfo->mfRxDropRate = 0.00;
        //         mpsStationMonitorUiInfo->mfTxDropRate = 0.00;
        //         mpsStationMonitorUiInfo->mulDownloadSpeed = 0;
        //         mpsStationMonitorUiInfo->mulUploadSpeed = 0;
        //！mpsStationMonitorUiInfo->mstrIPAddress = L"";
        //! mpsStationMonitorUiInfo->mstrMacAddress = L"";
        mpsStationMonitorUiInfo->mstrRssi = L"";
        mpsStationMonitorUiInfo->mstrRxMcs = L"";
        mpsStationMonitorUiInfo->mstrRxSector = L"";
        mpsStationMonitorUiInfo->mstrTxMcs = L"";
        mpsStationMonitorUiInfo->mstrTxSector = L"";
        mpsStationMonitorUiInfo->mstrGatewayAddress = L"";
    }
}