#include "StdAfx.h"
#include "CJGW_WlanHandleThread.h"
#include "CJGW_WlanServiceThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include "CJGW_RequestBaseJson.h"
#include "CJGW_ResponseScanWlanJSON.h"
#include "CJGW_RequestConnectWlanJson.h"
#include "CJGW_ReqPumasCommadJson.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include <JGW_FoundationFunc/jgw_single_application_instance.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/jgw_timer.hpp>


namespace JGW
{
    //! connectionMode : manual
    const std::wstring gstrWlanProFile = L"<?xml version=\"1.0\"?>\
                                          <WLANProfile xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v1\">\
                                          <name>%s</name>\
                                          <SSIDConfig>\
                                          <SSID>\
                                          <name>%s</name>\
                                          </SSID>\
                                          </SSIDConfig>\
                                          <connectionType>ESS</connectionType>\
                                          <connectionMode>manual</connectionMode>\
                                          <MSM>\
                                          <security>\
                                          <authEncryption>\
                                          <authentication>WPA2PSK</authentication>\
                                          <encryption>AES</encryption>\
                                          <useOneX>false</useOneX>\
                                          </authEncryption>\
                                          <sharedKey>\
                                          <keyType>passPhrase</keyType>\
                                          <protected>false</protected>\
                                          <keyMaterial>%s</keyMaterial>\
                                          </sharedKey>\
                                          </security>\
                                          </MSM>\
                                          <MacRandomization xmlns=\"http://www.microsoft.com/networking/WLAN/profile/v3\">\
                                          <enableRandomization>false</enableRandomization>\
                                          </MacRandomization>\
                                          </WLANProfile>";
#if 0
    <?xml version="1.0" encoding="US-ASCII"?>
        <WLANProfile xmlns="https://www.microsoft.com/networking/WLAN/profile/v1">
        <name>SampleWPAPSK</name>
        <SSIDConfig>
        <SSID>
        <name>SampleWPAPSK</name>
        </SSID>
        </SSIDConfig>
        <connectionType>ESS</connectionType>
        <connectionMode>auto</connectionMode>
        <autoSwitch>false</autoSwitch>
        <MSM>
        <security>
        <authEncryption>
        <authentication>WPA2PSK</authentication>
        <encryption>AES</encryption>
        <useOneX>false</useOneX>
        </authEncryption>
        <sharedKey>
        <keyType>passPhrase</keyType>
        <protected>false</protected>
        <keyMaterial> <!-- insert key here --> </keyMaterial>
        </sharedKey>
        </security>
        </MSM>
        </WLANProfile>


        <?xml version="1.0" encoding="US-ASCII"?>
        <WLANProfile xmlns="https://www.microsoft.com/networking/WLAN/profile/v1">
        <name>SampleWPA2PSK</name>
        <SSIDConfig>
        <SSID>
        <name>SampleWPA2PSK</name>
        </SSID>
        </SSIDConfig>
        <connectionType>ESS</connectionType>
        <connectionMode>auto</connectionMode>
        <autoSwitch>false</autoSwitch>
        <MSM>
        <security>
        <authEncryption>
        <authentication>WPA2PSK</authentication>
        <encryption>AES</encryption>
        <useOneX>false</useOneX>
        </authEncryption>
        <sharedKey>
        <keyType>passPhrase</keyType>
        <protected>false</protected>
        <keyMaterial> <!-- insert key here --> </keyMaterial>
        </sharedKey>
        </security>
        </MSM>
        </WLANProfile>
#endif
    CCJGW_WlanHandleThread::CCJGW_WlanHandleThread(void) : mDonglesComPort(mSerialComPort)
    {
    }


    CCJGW_WlanHandleThread::~CCJGW_WlanHandleThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCJGW_WlanHandleThread,CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCJGW_WlanHandleThread::OnStartHandleRequest)
    END_MYTHREAD_MESSAGE_MAP()

    void CCJGW_WlanHandleThread::OnStartHandleRequest(WPARAM wParam,LPARAM lParam)
    {
        PS_WLAN_SOCKET_PARAM psWlanSocketParam = (PS_WLAN_SOCKET_PARAM)wParam;
        if (!psWlanSocketParam) return ;
        CCJGW_WlanServiceThread* pWlanServiceThread = (CCJGW_WlanServiceThread*)psWlanSocketParam->mpMessageThread;

		Log4WI_F(L"On Start Handle Request (%s)",JGW_A2W(psWlanSocketParam->mstrJson).c_str());
        std::string strRsp = HandleRequest(psWlanSocketParam->mstrJson);
        pWlanServiceThread->SendJsonCommand(strRsp,psWlanSocketParam->mClientSock);
    }

    std::string CCJGW_WlanHandleThread::HandleRequest(const std::string& strRequest)
    {
        CCJGW_RequestBaseJson requestBaseJson;
        requestBaseJson.FromJosn(strRequest);

        switch (requestBaseJson.requestType)
        {
        case E_SCAN_WLAN:
            return HandleResopnseScanWiFi(strRequest);
        case E_CONNECT_WLAN:
            return HandleResopnseConnectWiFi(strRequest);
        case E_DISCONNECT_WLAN:
            return HandleResponseDisConnectWiFi(strRequest);
        case E_CONNECT_PUMAS:
            return HandleRspConnectPumas(strRequest);
        case E_COMMAND_PUMAS:
            return HandleRspCommandPumas(strRequest);
        case E_DISCONNECT_PUMAS:
            return HandleRspDisConnectPumas(strRequest);
        case E_CHECK_WLAN_CONNECT:
            return HandleRspCheckWlanConnectStatus(strRequest);
        case E_SHELL_EXECUTE:
            return HandleRspShellExecute(strRequest);
        }

        return "{\"responseType\":-1,\"bStatus\":false,\"returnMessage\":\"未知命令类型\"}";
    }

    std::string CCJGW_WlanHandleThread::HandleResopnseScanWiFi(const std::string& strRequest)
    {
        mapBss.clear();
        CCJGW_ResponseScanWlanJSON responseScanJson;
        responseScanJson.responseType = E_SCAN_WLAN;

		Log4WI(L"Handle Scan WiFi Request");
        if (!GetWiFiInterfaceImpl()->OpenWlanHandle())
        {
            responseScanJson.returnMessage = "OpenWlanHandle Fail";
            return responseScanJson.BuildScanWlanJson(mapBss);
        }

        if (!GetWiFiInterfaceImpl()->EnumWlanInterface(L"Peraso"))
        {
            responseScanJson.returnMessage = "未发现Peraso网卡设备插入";
            return responseScanJson.BuildScanWlanJson(mapBss);
        }

        if (!GetWiFiInterfaceImpl()->GetVisibleNetworkList(mapBss))
        {
            responseScanJson.returnMessage = "GetVisibleNetworkList Fail";
            return responseScanJson.BuildScanWlanJson(mapBss);
        }
        //! GetWiFiInterfaceImpl()->CloseWlanHandle();
        responseScanJson.bStatus = true;
        return responseScanJson.BuildScanWlanJson(mapBss);
    }

    bool CCJGW_WlanHandleThread::ConnectWlanToSSID(const std::string& strSSID,const std::string& strPassword,int nTimeOutSec /* = 30 */)
    {
        jgw_timer jt;
        bool bStatus = false;
        bool bOnceInit = true;
        DOT11_BSS_TYPE dot11BSSType = dot11_BSS_type_infrastructure;
        mstrConnectSSID = JGW_A2W(strSSID);
        if (mapBss.end() != mapBss.find(strSSID)) dot11BSSType = (DOT11_BSS_TYPE)mapBss[strSSID];

        while (jt.elapsed() <= nTimeOutSec)        
        {
            if (strPassword.empty())
            {
                bStatus = GetWiFiInterfaceImpl()->ConnectOpenWlanToSSID(strSSID,dot11BSSType);
            }
            else
            {
                if (bOnceInit)
                {
                    std::wstring strWlanProFile;
                    JGW_FormatWString(strWlanProFile,gstrWlanProFile.c_str(),mstrConnectSSID.c_str(),mstrConnectSSID.c_str(),JGW_A2W(strPassword).c_str());
                    if (!GetWiFiInterfaceImpl()->SetWlanConnectProfile(strWlanProFile)) 
                    {
                        Sleep(200);
                        continue;
                    }
                    bOnceInit = false;
                }

                bStatus = GetWiFiInterfaceImpl()->ConnectWlanToSSID(strSSID,mstrConnectSSID,dot11BSSType);
            }
            //! 连接失败
            if (!bStatus)
            {
                Sleep(2 * 1000);
                continue;
            }
            //! 检查WIFI是否已经连接
            for (size_t i = 0;i < 20;i ++)
            {
                if (GetWiFiInterfaceImpl()->CheckWlanIsConnected(strSSID)) return true;
                Sleep(1 * 1000);
            }
        }

        return false;
    }

    std::string CCJGW_WlanHandleThread::HandleResopnseConnectWiFi(const std::string& strRequest)
    {
        CCJGW_FTM_Json ftmJson;
        mScanTime = 1000;
        CCJGW_RequestConnectWlanJson requestConnectWlanJson;
        CCJGW_ResponseBaseJson responseJson;
        bool bOnceInit = true;
        ftmJson.FromJsonToString(strRequest);
        requestConnectWlanJson.FromJosn(ftmJson);

		Log4WI(L"Handle Connect WiFi Request");
        responseJson.responseType = E_CONNECT_WLAN;
        responseJson.bStatus = ConnectWlanToSSID(requestConnectWlanJson.ssidname,requestConnectWlanJson.passwrod,30);
        if (!responseJson.bStatus) responseJson.returnMessage = "WiFi 连接失败";


        return responseJson.BuildResponseBaseJson();
    }

    std::string CCJGW_WlanHandleThread::HandleResponseDisConnectWiFi(const std::string& strRequest)
    {
        CCJGW_ResponseBaseJson responseJson;
        responseJson.responseType = E_DISCONNECT_WLAN;
    
		Log4WI(L"Handle DisConnect WiFi Request");
        GetWiFiInterfaceImpl()->DeleteWlanConnectProfile(mstrConnectSSID);
        responseJson.bStatus = GetWiFiInterfaceImpl()->DisWlanConnect();
        GetWiFiInterfaceImpl()->CloseWlanHandle();

        return responseJson.BuildResponseBaseJson();
    }

    std::string CCJGW_WlanHandleThread::HandleRspConnectPumas(const std::string& strRequest)
    {
        CCJGW_ResponseBaseJson responseJson;
        std::wstring strComPort;
        responseJson.responseType = E_CONNECT_PUMAS;

		Log4WI(L"Handle Connect Pumas Request");
        strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
        if (strComPort.empty()) 
        {
            responseJson.returnMessage = "Not Find Peraso Device";
            return responseJson.BuildResponseBaseJson();
        }
        responseJson.bStatus = mDonglesComPort.OpenDonglesComPort(strComPort);
        if (!responseJson.bStatus) responseJson.returnMessage = "Open Peraso Device Fail";
        return responseJson.BuildResponseBaseJson();
    }

    std::string CCJGW_WlanHandleThread::HandleRspCommandPumas(const std::string& strRequest)
    {
        CCJGW_FTM_Json ftmJson;
        CCJGW_ReqPumasCommadJson puamsCommandJson;
        CCJGW_ResponseBaseJson responseJson;
        responseJson.responseType = E_COMMAND_PUMAS;
        ftmJson.FromJsonToString(strRequest);
        puamsCommandJson.FromJosn(ftmJson);

		Log4WI(L"Handle Write Pumas Command Request");
        //! 未打开Dongles
        if (!mDonglesComPort.IsOpenDonglesComPort())
        {
            puamsCommandJson.command = "Peraso Device Is Not Open";
            return responseJson.BuildResponseBaseJson();
        }

        std::string strRead;
        responseJson.bStatus = mDonglesComPort.ReadWriteCommands(puamsCommandJson.command.c_str(),strRead);
        if (!responseJson.bStatus) responseJson.returnMessage = "Read Write Peraso Device Fail";
        else responseJson.returnMessage = strRead;

        return responseJson.BuildResponseBaseJson();
    }

    std::string CCJGW_WlanHandleThread::HandleRspDisConnectPumas(const std::string& strRequest)
    {
        CCJGW_ResponseBaseJson responseJson;
        responseJson.responseType = E_DISCONNECT_PUMAS;
        responseJson.bStatus = true;
		Log4WI(L"Handle DisConnect Pumas Request");
        mDonglesComPort.CloseDonglesComPort();
        return responseJson.BuildResponseBaseJson();
    }

    std::string CCJGW_WlanHandleThread::HandleRspShellExecute(const std::string& strRequest)
    {
        CCJGW_ResponseBaseJson responseJson;
        responseJson.responseType = E_SHELL_EXECUTE;
        responseJson.bStatus = true;

        CCJGW_FTM_Json ftmJson;
        CCJGW_ReqPumasCommadJson puamsCommandJson;
        ftmJson.FromJsonToString(strRequest);
        puamsCommandJson.FromJosn(ftmJson);
        //! 返回值大于32表示执行成功
        responseJson.bStatus = (32 < (int)ShellExecute(NULL,L"open",JGW_A2W(puamsCommandJson.command).c_str(),NULL,JGW_GetApplicationFolder(),SW_NORMAL));
        if (!responseJson.bStatus)
        {
            //responseJson.returnMessage = JGW_GetSystemErrorStringA(GetLastError());
            JGW_FormatString(responseJson.returnMessage,"Command : %s Error : %s",puamsCommandJson.command.c_str(),JGW_GetSystemErrorStringA(GetLastError()).c_str());
        }

        return responseJson.BuildResponseBaseJson();
    }

    //! 如果WLAN检查连接失败，则断开WIFI，重新连接wifi
    //! 包含WLAN 网关IP地址(待检测IP地址)，远程主机IP地址(=>以排除本地USB网口IP地址)
    std::string CCJGW_WlanHandleThread::HandleRspCheckWlanConnectStatus(const std::string& strRequest)
    {
        //! 检查网关IP地址
        CCJGW_FTM_Json ftmJson;
        CCJGW_ReqCheckWlanConnectJson checkWlanJson;
        CCJGW_ResponseCheckWlanConnectJson responseJson;
        std::string strScanRsp;
        jgw_timer jt;
        bool bStatus = true;

		Log4WI(L"Handle Check Wlan Connect Request");
        ftmJson.FromJsonToString(strRequest);
        checkWlanJson.FromJosn(ftmJson);
        responseJson.responseType = E_CHECK_WLAN_CONNECT;

        int i = 2;

        while (jt.elapsed() <= 60)
        {
            //! 先关闭当前WIFI连接，然后扫描WIFI连接
            HandleResponseDisConnectWiFi("");
            //! 扫描WIFI
            strScanRsp = HandleResopnseScanWiFi(""); 
            ftmJson.FromJsonToString(strScanRsp);
            ftmJson.GetJsonValueToKey<bool>("bStatus",bStatus);

            //! 扫描失败
            if (!bStatus)
            {
                ftmJson.GetJsonValueToKey<std::string>("returnMessage",responseJson.returnMessage);
                continue;
            }
            //! 检查WIFI是否已经扫描到,未扫描到
            if (mapBss.end() == mapBss.find(checkWlanJson.ssidname))
            {
                responseJson.returnMessage = "未扫描到当前SSID";
                Sleep(2 * 1000);
                continue;
            }

            //! 
            if (!ConnectWlanToSSID(checkWlanJson.ssidname,checkWlanJson.passwrod,30))
            {
                responseJson.returnMessage = "连接WIFI失败";
                continue;
            }
            //! 检查WIFI的网络连接
            responseJson.wlanAddress = GetWlanIpAddress(checkWlanJson.wlangateway,checkWlanJson.localaddress);
            if (!responseJson.wlanAddress.empty())
            {
                //! 检查内网连接
                for (size_t i = 0;i < 10;i ++)
                {
                    //! 如果能PING通，说明网络连接是OK的，否则重新断开WIFI连接
                    if (mIcmpSocket.Ping(checkWlanJson.wlangateway,responseJson.wlanAddress.c_str()))
                    {
                        responseJson.bStatus = true;
                        return responseJson.BuildResponseCheckWlanConnectJson();
                    }
                }
                JGW_FormatString(responseJson.returnMessage,"ping %s(wlan地址: %s)失败",checkWlanJson.wlangateway.c_str(),responseJson.wlanAddress.c_str());
            }  
        }

        return responseJson.BuildResponseCheckWlanConnectJson();
    }

    std::string CCJGW_WlanHandleThread::GetWlanIpAddress(const std::string& strWlanGateWay,const std::string& strLocalAddress)
    {
        std::vector<std::string> vipAddress;
        msyncSocket.GetHostIP(std::string(""),vipAddress);
        //! 优先取与WLAN网关同一个局域网的IP
        std::string strWlanGateWaySegment = strWlanGateWay.substr(0,strWlanGateWay.rfind('.') + 1);
        for (size_t i = 0;i < vipAddress.size();i ++)
        {
            if (NULL == JGW_StrComparenoCaseStr(strLocalAddress.c_str(),vipAddress[i].c_str())) continue;/*return vipAddress[i];*/
            if (NULL == _strnicmp(strWlanGateWaySegment.c_str(),vipAddress[i].c_str(),strWlanGateWaySegment.length())) return vipAddress[i];
        }
        //! 否则默认取与本地地址不相同的第一个IP
        for (size_t i = 0;i < vipAddress.size();i ++)
        {
            if (NULL == JGW_StrComparenoCaseStr(strLocalAddress.c_str(),vipAddress[i].c_str())) continue;/*return vipAddress[i];*/
            return vipAddress[i];
        }
        return "";
    }

//     bool CCJGW_WlanHandleThread::ConnectWlan(CCJGW_ReqCheckWlanConnectJson repJson,CCJGW_ResponseCheckWlanConnectJson rspJson,int nTimeOut)
//     {
//         jgw_timer jt;
//         bool bFindSSID = false;
//         while (jt.elapsed() <= nTimeOut)
//         {
//             mstrGUID = wlan.FindWalnGUIDToInterfaceDescriptionKeyword(JGW_A2W(repJson.InterfaceDescription));
//             if (mstrGUID.empty()) 
//             {
//                 Sleep(200);
//                 rspJson.returnMessage = "未识别到WLAN网卡设备，请检查WLAN网卡是否插入";
//                 continue;
//             }
//             wlan.DisConnectWlan(mstrGUID);
//             //! scan wifi
//             rspJson.bStatus = wlan.GetWlanBssList(mstrGUID,mapBss,mScanTime);
//             if (!rspJson.bStatus)
//             {
//                 rspJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg()); 
//                 if (mScanTime < 5000) mScanTime += 500;
//                 Sleep(200);
//                 continue;
//             }
//             //! 检查SSID是否扫描到
//             bFindSSID = false;
//             mstrSSID = JGW_A2W(repJson.ssidname);
//             for (std::map<std::wstring,int>::iterator it = mapBss.begin();
//                 it != mapBss.end();
//                 ++ it)
//             {
//                 if (NULL == JGW_WStrComparenoCaseWStr(mstrSSID.c_str(),it->first.c_str()))
//                 { 
//                     bFindSSID = true;
//                     break ;
//                 }
//             }
// 
//             if (!bFindSSID)
//             {
//                 Sleep(200);
//                 rspJson.returnMessage = "未查到到当前SSID";
//                 rspJson.returnMessage= repJson.ssidname;
//                 continue;
//             }
//             //! 连接11AD
//             if (repJson.passwrod.empty())
//             {
//                 rspJson.bStatus = wlan.ConnectOpenWlan(mstrGUID,mstrSSID,mapBss[mstrSSID]);
//             }
//             else
//             {
//                 std::wstring strWlanProFile;
//                 JGW_FormatWString(strWlanProFile,gstrWlanProFile.c_str(),mstrSSID.c_str(),mstrSSID.c_str(),JGW_A2W(repJson.passwrod).c_str());
//                 rspJson.bStatus = wlan.SetWlanProfile(mstrGUID,strWlanProFile);
//                 if (rspJson.bStatus )
//                     rspJson.bStatus = wlan.ConnectWlan(mstrGUID,mstrSSID,mstrSSID,mapBss[mstrSSID]);
//             }
// 
//             if (!rspJson.bStatus)
//             {
//                 rspJson.returnMessage = JGW_W2A(wlan.GetWlanErrorMsg());
//             }
//             else
//             {
//                 return true;
//             }
//         }
//         return false;
//     }
}