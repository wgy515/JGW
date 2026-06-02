#include "StdAfx.h"
#include <JGW_QMSLFuncPlugin\CJGWWiFiQMSL.h>
#include <JGW_QMSLFuncPlugin\WiFiTestDefine.h>
#include <JGW_FoundationFunc\JGW_StringFunc.h>
#include "../../../platform_include/JGW_WIFI_Define.h"

#define _OP_TX 1
#define _OP_RX 2
#define _OP_RX_STATUS 11
namespace JGW
{
    CCJGWWiFiQMSL::CCJGWWiFiQMSL(void* hResourceContent/* = NULL*/,bool bUseQPST /*= false*/):jgw_wr_nv_api(hResourceContent,bUseQPST)
    {
    }

    CCJGWWiFiQMSL::~CCJGWWiFiQMSL(void)
    {
    }

    bool CCJGWWiFiQMSL::LoadWiFiDut(const char *boardDataFName /* = "" */, const char *DevDLLname /* = "qc6174" */,int iNVMem /* = 0 */,unsigned int ssid /* = 62 */)
    {
#if 0
        unsigned char szboardDataFName[260] = {0};
        memcpy(szboardDataFName,boardDataFName,strlen(boardDataFName));
        unsigned char result = TRUE;

        result = QLIB_FTM_WLAN_SetModuleType(m_hResourceContext,FTM_WLAN_ModuleType_Atheros);
        result = QLIB_FTM_WLAN_Atheros_LoadDUT(m_hResourceContext,(unsigned char*)boardDataFName,(unsigned char*)"c:\\wlanConfig.bin"/*szboardDataFName*/,iNVMem,ssid);
        return result/*(TRUE == QLIB_FTM_WLAN_SetModuleType(m_hResourceContext,FTM_WLAN_ModuleType_Atheros) && TRUE == QLIB_FTM_WLAN_Atheros_LoadDUT(m_hResourceContext,(unsigned char*)DevDLLname,szboardDataFName,iNVMem,ssid))*/;
#else
        return (TRUE == QLIB_FTM_WLAN_SetModuleType(m_hResourceContext,FTM_WLAN_ModuleType_Atheros) && TRUE == QLIB_FTM_WLAN_Atheros_LoadDUT(m_hResourceContext,(unsigned char*)DevDLLname,(unsigned char*)boardDataFName,5/*iNVMem*/,ssid));
#endif
    }

    bool CCJGWWiFiQMSL::UnLoadWiFiDut()
    {
        return (TRUE == QLIB_FTM_WLAN_Atheros_UNLoadDUT(m_hResourceContext));
    }

    bool CCJGWWiFiQMSL::DisableWiFiTx()
    {
        bool bResulte = true;

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Create(m_hResourceContext,_OP_TX));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"txMode","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"channel","2412"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Complete(m_hResourceContext));

        return bResulte;
    }

    bool CCJGWWiFiQMSL::StartWiFiTx(int nTxChain,float fPower,int nChannel,int nHtMode,int nDataRate,int nTxMode /* = 3 */,int nTpcm /* = 0 */)
    {
       // unsigned char opCode = '1';
        bool bResulte = true;
        std::string strTemp;

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Create(m_hResourceContext,_OP_TX));

        JGW_FormatString(strTemp,"%.2f",fPower);
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"txPower0",(char*)strTemp.c_str()));
        JGW_FormatString(strTemp,"%d",nChannel);
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"channel",(char*)strTemp.c_str()));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"txMode",(char*)gvTxModeParamInfo[nTxMode].szIndex));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"wlanMode",(char*)gvHTModeInfo[nHtMode].szIndex));

        JGW_FormatString(strTemp,"%d",nDataRate);
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"rateBitIndex0",(char*)strTemp.c_str()));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"tpcm",(char*)gvTxPowerControlInfo[nTpcm].szIndex));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"txChain0",(char*)gvTxChain0Info[nTxChain].szIndex));
        bResulte &= DefaultWiFiTxParam();

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Complete(m_hResourceContext));
        return bResulte;
    }


    bool CCJGWWiFiQMSL::DefaultWiFiTxParam()
    {
        bool bResulte = true;

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"txPattern","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"shortGuard","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"agg","1"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"bandwidth","0"));
        //!bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"ifs","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"numPackets","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"scramblerOff","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"aifsn","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"pktLen0","1500"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"antenna","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"broadcast",(char*)gvBroadCaseInfo[0].szIndex));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"flags","24"));
       

        return bResulte;
    }

    bool CCJGWWiFiQMSL::StartWiFiRx(int nRxChain,int nChannel,int nHtMode,int nDataRate)
    {
        bool bResulte = true;
        std::string strTemp;

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Create(m_hResourceContext,_OP_RX));
        JGW_FormatString(strTemp,"%d",nChannel);
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"channel",(char*)strTemp.c_str()));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"rxMode",(char*)gvRxModeParamInfo[0].szIndex));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"wlanMode",(char*)gvHTModeInfo[nHtMode].szIndex));

        if (E_WLAN_CCK == nHtMode)
        {
            bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"bandwidth","51"));
        }
        else
        {
            bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"bandwidth","0"));
        }

        JGW_FormatString(strTemp,"%d",nDataRate);
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"rateMask",(char*)strTemp.c_str()));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"rxChain",(char*)gvTxChain0Info[nRxChain].szIndex));

        //! Default:
        //! QLIB_FTM_WLAN_TLV_AddParam(bandwidth,0)
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"bandwidth","0"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"enANI","Off"));
        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_AddParam(m_hResourceContext,"antenna","0"));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Complete(m_hResourceContext));
        return bResulte;
    }

    bool CCJGWWiFiQMSL::GetWiFiRxData(float& rssi,int& txbTotalPackets,int& txbGoodPackets)
    {
        bool bResulte = true;
        char szParam[260] = {0};

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Create(m_hResourceContext,_OP_RX_STATUS));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_Complete(m_hResourceContext));

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(m_hResourceContext,"totalPkt",szParam));
        txbTotalPackets = atoi(szParam);
        memset(szParam,0x00,260);

//         bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(m_hResourceContext,"totalPkt",szParam));
//         txbTotalPackets = atoi(szParam);
//         memset(szParam,0x00,260);

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(m_hResourceContext,"goodPackets",szParam));
        txbGoodPackets = atoi(szParam);
        memset(szParam,0x00,260);

        bResulte &= (TRUE == QLIB_FTM_WLAN_TLV_GetRspParam(m_hResourceContext,"rssi",szParam));
        rssi = static_cast<float>(atof(szParam));

        return bResulte;
    }
}
