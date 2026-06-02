#include "StdAfx.h"
#include "DuiLossConfig.h"
#include "../ConfigFile.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#define lsitContainerItem_height 40

namespace JGW
{
    CDuiLossConfig::CDuiLossConfig(CConfigFile* lpConfigFileClass):m_hParentHwnd(NULL)
        ,m_pCancelbtn(NULL)
        ,m_pSaveBtn(NULL)
        ,m_pListCtrl(NULL)
        ,m_pRfConfigInfo(NULL)
        ,m_lpConfigFileClass(lpConfigFileClass)
    {
        if( m_lpConfigFileClass ) m_pRfConfigInfo = &m_lpConfigFileClass->m_sRfConfigInfo;
    }


    CDuiLossConfig::~CDuiLossConfig(void)
    {
    }

    int CDuiLossConfig::DoModal( HWND hParentHwnd /* = NULL */ )
    {
        m_hParentHwnd = hParentHwnd;
#ifdef USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateDuiDialogWindow(m_hParentHwnd,_T(""),_T("configskin.xml"),UI_WNDSTYLE_FRAME/*UI_CLASSSTYLE_CHILD,NULL*/);
#else
        CreateDuiDialogWindow(m_hParentHwnd,_T("skin\\Login"),_T("configskin.xml"),UI_WNDSTYLE_FRAME/*UI_CLASSSTYLE_CHILD*/ ,/*WS_EX_STATICEDGE | */NULL ,_T("CDuiLossConfig"));
#endif


        SetFocus(m_hParentHwnd);
        CenterWindow();

        return ShowModal();
    }

    LPCTSTR CDuiLossConfig::GetWindowClassName() const
    {
        return L"CDuiLossConfig";
    }

    // UINT CDuiLossConfig::GetClassStyle() const
    // {
    // 	return UI_CLASSSTYLE_CHILD;
    // }

    bool CDuiLossConfig::InitWindow()
    {
        m_pCancelbtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("cancelbtn")));
        m_pSaveBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("savebtn")));
        m_pListCtrl = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(_T("configlist")));

        if( !m_pCancelbtn || !m_pSaveBtn || !m_pListCtrl || !m_lpConfigFileClass)
        {
            Close(IDCANCEL);
            return false;
        }
        InitShowConfigContent();
        return true;
    }

    void CDuiLossConfig::Notify(TNotifyUI& msg)
    {
        if ( msg.sType == DUI_MSGTYPE_CLICK )
        {
            if ( m_pCancelbtn == msg.pSender )
            {
                Close(IDCANCEL);//PostQuitMessage(0);		
                return ;
            }
            if ( m_pSaveBtn == msg.pSender )
            {
                if( SaveConfig() )
                {
                    ::MessageBox(GetHWND(),L"保存配置文件成功",L"保存配置文件",MB_OK);
                    Close(IDCANCEL);
                }
                return ;
            }
        }
    }

    CEditUI* CDuiLossConfig::GetListSubEditUi()
    {
        CEditUI* editUi = new CEditUI;
        if( !editUi ) return NULL;

        editUi->SetAttribute(L"bordersize",L"1");
        editUi->SetAttribute(L"height",L"30");
        editUi->SetAttribute(L"bordercolor",L"#FF4775CC");
        editUi->SetAttribute(L"textpadding",L"5,5,5,5");

        return editUi;
    }

    void CDuiLossConfig::GetVectortBandChannelInfo(std::vector<S_BAND_CHANNEL_INFO>& vBandChannelInfo,std::wstring& strChannel,std::wstring& strLoss)
    {
        TCHAR szChannelBuffer[260] = {0};
        TCHAR szLossBuffer[260] = {0};
        std::wstring strPTxLoss,strPRxLoss,strDRxLoss,strTemp;
        strChannel = L"";
        strLoss = L"";

        for (std::vector<S_BAND_CHANNEL_INFO>::iterator it = vBandChannelInfo.begin();
            it != vBandChannelInfo.end();
            it ++)
        {
            JGW_FormatWString(strChannel,L"%d,",it->m_nUpLinkChannel);
            _tcscat_s(szChannelBuffer,strChannel.c_str());

            JGW_FormatWString(strTemp,L"%s%.2f,",strPTxLoss.c_str(),it->m_fPTxLoss);
            strPTxLoss = strTemp;
            JGW_FormatWString(strTemp,L"%s%.2f,",strPRxLoss.c_str(),it->m_fPRxLoss);
            strPRxLoss = strTemp;
            JGW_FormatWString(strTemp,L"%s%.2f,",strDRxLoss.c_str(),it->m_fDRxLoss);
            strDRxLoss = strTemp;
        }

        strChannel = szChannelBuffer;
        if ( -1 != strChannel.find(L',') )
        {
            strChannel.erase(strChannel.find_last_of(L','));
        }

        if ( -1 != strPTxLoss.find(L',') )
        {
            strPTxLoss.erase(strPTxLoss.find_last_of(L','));
        }

        if ( -1 != strPRxLoss.find(L',') )
        {
            strPRxLoss.erase(strPRxLoss.find_last_of(L','));
        }

        if ( -1 != strDRxLoss.find(L',') )
        {
            strDRxLoss.erase(strDRxLoss.find_last_of(L','));
        }
        strLoss  = strPTxLoss;
        strLoss += L";";
        strLoss += strPRxLoss;
        strLoss += L";";
        strLoss += strDRxLoss;
    }

    // void CDuiLossConfig::ShowTestBandInfo(S_SYS_INFO& sysInfo,LPCTSTR strFristCol/* = L"LTE_B"*/)
    // {
    // 	CListContainerElementUIEx* lplsitContainerItem = NULL;
    // 	CEditUI* lpEditui = NULL;
    // 	std::wstring strTemp = L"",strChannel,strLoss;
    // 
    // 	for ( std::vector<S_BAND_INFO>::iterator it = sysInfo.m_vBandInfo.begin();
    // 		it != sysInfo.m_vBandInfo.end();
    // 		it ++)
    // 	{
    // 		lplsitContainerItem = new CListContainerElementUIEx;
    // 		if( !lplsitContainerItem ) break;
    // 		lplsitContainerItem->SetFixedHeight(40);
    // 
    // 		CHorizontalLayoutUI* lphui1 = new CHorizontalLayoutUI;
    // 		lpEditui = GetListSubEditUi();
    // 		if( !lpEditui ) break;
    // 		FormatWString(strTemp,L"%s%d",strFristCol,it->m_nBand);
    // 		lpEditui->SetText(strTemp.c_str());
    // 		lpEditui->SetAttribute(L"align",L"center");
    // 		lpEditui->SetAttribute(L"tooltip",L"不允许修改,修改也无效 O(∩_∩)O哈哈~");
    // 		lphui1->Add(lpEditui);
    // 
    // 		CHorizontalLayoutUI* lphui2 = new CHorizontalLayoutUI;
    // 		lpEditui = GetListSubEditUi();
    // 		if( !lpEditui ) break;
    // 		GetVectortBandChannelInfo(it->m_vBandChannelInfo,strChannel,strLoss);
    // 		lpEditui->SetText(strChannel.c_str());
    // 		lpEditui->SetAttribute(L"tooltip",L"请输入当前频段上行信道 已','隔开");
    // 		m_pListCtrl->InsertContainerSubItem(lplsitContainerItem,1,lpEditui);
    // 		lphui2->Add(lpEditui);
    // 
    // 		CHorizontalLayoutUI* lphui3 = new CHorizontalLayoutUI;
    // 
    // 		lpEditui = GetListSubEditUi();
    // 		if( !lpEditui ) break;
    // 		FormatWString(strTemp,L"%d",(int)it->m_ePort);
    // 		lpEditui->SetText(strTemp.c_str());
    // 		lpEditui->SetAttribute(L"align",L"center");
    // 		lpEditui->SetAttribute(L"tooltip",L"请输入测试端口,1表示CMW500最下面的RF口(RFAC),2 中间(RFAO),3 最上面(RFBC)");
    // 		m_pListCtrl->InsertContainerSubItem(lplsitContainerItem,2,lpEditui);
    // 		lphui3->Add(lpEditui);
    // 
    // 		CHorizontalLayoutUI* lphui4 = new CHorizontalLayoutUI;
    // 		lpEditui = GetListSubEditUi();
    // 		if( !lpEditui ) break;
    // 		FormatWString(strTemp,L"%d",(int)it->m_eDrxPort);
    // 		lpEditui->SetText(strTemp.c_str());
    // 		lpEditui->SetAttribute(L"align",L"center");
    // 		lpEditui->SetAttribute(L"tooltip",L"请输入测试端口,1表示CMW500最下面的RF口(RFAC),2 中间(RFAO),3 最上面(RFBC)");
    // 		m_pListCtrl->InsertContainerSubItem(lplsitContainerItem,3,lpEditui);
    // 		lphui4->Add(lpEditui);
    // 
    // 		CHorizontalLayoutUI* lphui5 = new CHorizontalLayoutUI;
    // 		lpEditui = GetListSubEditUi();
    // 		if( !lpEditui ) break;
    // 		lpEditui->SetText(strLoss.c_str());
    // 		lpEditui->SetAttribute(L"align",L"center");
    // 		lpEditui->SetAttribute(L"tooltip",L"线损依次对应信道,已','隔开");
    // 		m_pListCtrl->InsertContainerSubItem(lplsitContainerItem,4,lpEditui);
    // 		lphui5->Add(lpEditui);
    // 
    // 		lplsitContainerItem->Add(lphui1);
    // 		lplsitContainerItem->Add(lphui2);
    // 		lplsitContainerItem->Add(lphui3);
    // 		lplsitContainerItem->Add(lphui4);
    // 		lplsitContainerItem->Add(lphui5);
    // 		m_pListCtrl->Add(lplsitContainerItem);
    // 	}
    // }
    /*
    CDialogBuilder builder;
    CListContainerElementUIEx* pLine = (CListContainerElementUIEx*)(builder.Create(_T("sigle_list_item_column.xml"),(UINT)0,NULL,&m_PaintManager));
    pLine->SetFixedHeight(60);
    if( pLine != NULL ) 
    {
    m_pListCtrl->InsertItem(m_pListCtrl->GetCount(), 40, pLine);
    }

    CDialogBuilder builder1;
    CListContainerElementUIEx* pLine1 = (CListContainerElementUIEx*)(builder1.Create(_T("sigle_list_item_column.xml"),(UINT)0,NULL,&m_PaintManager));
    pLine1->SetFixedHeight(60);
    if( pLine1 != NULL ) 
    {
    m_pListCtrl->InsertItem(m_pListCtrl->GetCount(), 40, pLine1);
    }
    */
    void CDuiLossConfig::ShowTestBandInfo(S_SYS_INFO& sysInfo,LPCTSTR strFristCol/* = L"LTE_B"*/)
    {
        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CEditUI* lpEditui = NULL;
        std::wstring strTemp = L"",strChannel,strLoss;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;

        for ( std::vector<S_BAND_INFO>::iterator it = sysInfo.m_vBandInfo.begin();
            it != sysInfo.m_vBandInfo.end();
            it ++)
        {
            CDialogBuilder builder1;
            int i = 0;
            lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("sigle_list_item_column.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem )continue;
            if( lplsitContainerItem != NULL ) 
            {
                m_pListCtrl->InsertItem(m_pListCtrl->GetCount(), 40, lplsitContainerItem);
            }
            
            if (NULL == JGW_WStrComparenoCaseWStr(strFristCol,L"WIFI_"))
                JGW_FormatWString(strTemp,L"%s%s",strFristCol,gszWlanModeName[it->m_nBand - 1]);
            else
                JGW_FormatWString(strTemp,L"%s%d",strFristCol,it->m_nBand);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(strTemp.c_str());

            GetVectortBandChannelInfo(it->m_vBandChannelInfo,strChannel,strLoss);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(strChannel.c_str());

            JGW_FormatWString(strTemp,L"%d",(int)it->m_ePort);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(2));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(strTemp.c_str());

            JGW_FormatWString(strTemp,L"%d",(int)it->m_eDrxPort);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(3));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(strTemp.c_str());

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(4));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(strLoss.c_str());
        }

    }

    void CDuiLossConfig::InitShowConfigContent()
    {
        m_pListCtrl->RemoveAll();

        if ( m_pRfConfigInfo->m_sModeSelecter.lte )
        {
            ShowTestBandInfo(m_pRfConfigInfo->m_sLteInfo.m_sTestBandInfo,L"LTE_B");
        }
        if ( m_pRfConfigInfo->m_sModeSelecter.wcdma )
        {
            ShowTestBandInfo(m_pRfConfigInfo->m_sWcdmaInfo.m_sTestBandInfo,L"WCDMA_B");
        }
        if ( m_pRfConfigInfo->m_sModeSelecter.tds )
        {
            ShowTestBandInfo(m_pRfConfigInfo->m_sTdscdmaInfo.m_sTestBandInfo,L"TDSCDMA_B");
        }
        if ( m_pRfConfigInfo->m_sModeSelecter.cdma )
        {
            ShowTestBandInfo(m_pRfConfigInfo->m_sCdmaInfo.m_sTestBandInfo,L"CDMA_B");
        }
        if ( m_pRfConfigInfo->m_sModeSelecter.gsm )
        {
            ShowTestBandInfo(m_pRfConfigInfo->m_sGsmInfo.m_sTestBandInfo,L"GSM_B");
        }
        if ( m_pRfConfigInfo->m_sModeSelecter.wlan )
        {
            ShowTestBandInfo(m_pRfConfigInfo->m_sWlanInfo.m_sTestBandInfo,L"WIFI_");
        }
    }
#define LTE_FLAGS_LENGTH 5
#define WCDMA_FLAGS_LENGTH 7
#define TDSCDMA_FLAGS_LENGTH 9
#define CDMA_FLAGS_LENGTH 6
#define GSM_FLAGS_LENGTH 5
#define WLAN_FLAGS_LENGTH 5

    int CDuiLossConfig::GetSys(LPCTSTR strBand)
    {
        switch(strBand[0])
        {
        case L'L':
            return TECHNOLOGY_LTE;
        case L'T':
            return TECHNOLOGY_TD_SCDMA;
        case L'W':
            if (L'C' == strBand[1])
                return TECHNOLOGY_WCDMA;
            else
                return TECHNOLOGY_WLAN;
        case L'C':
            return TECHNOLOGY_CDMA;
        case L'G':
            return GSM;
        default:
            return TECHNOLOGY_UNKNOW;
        }
    }

    int CDuiLossConfig::GetBandInfo(int nSys,LPCTSTR strBand)
    {
        switch(nSys)
        {
        case TECHNOLOGY_LTE:
            return _ttoi(strBand + LTE_FLAGS_LENGTH);
        case TECHNOLOGY_TD_SCDMA:
            return _ttoi( strBand + TDSCDMA_FLAGS_LENGTH );
        case TECHNOLOGY_CDMA:
            return _ttoi( strBand + CDMA_FLAGS_LENGTH );
        case TECHNOLOGY_WCDMA:
            return _ttoi( strBand + WCDMA_FLAGS_LENGTH );
        case GSM:
            return _ttoi( strBand + GSM_FLAGS_LENGTH);
        case TECHNOLOGY_WLAN:
            if ('B' == strBand[WLAN_FLAGS_LENGTH])
            {
                return 1;
            }
            else if ('G' == strBand[WLAN_FLAGS_LENGTH])
            {
                return 2;
            }
            else if ('N' == strBand[WLAN_FLAGS_LENGTH])
            {
                return 3;
            }
            else if ('A' == strBand[WLAN_FLAGS_LENGTH])
            {
                return 4;
            }
           // return _ttoi( strBand + WLAN_FLAGS_LENGTH);
        }
        return -1;
    }

    bool CDuiLossConfig::SaveConfig()
    {
        S_CHANGE_Chanel_INFO sChannelInfo;
        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CEditUI* lpEditui = NULL;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;
        std::vector<S_CHANGE_Chanel_INFO> vBandChannelInfo;

        for ( int i = 0;i < m_pListCtrl->GetCount();i ++ )
        {
            lplsitContainerItem = static_cast<CListContainerElementUIEx*>(m_pListCtrl->GetItemAt(i));
            if( !lplsitContainerItem )continue;

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
            if( !pHorizontalLayoutUi )continue;
            lpEditui = static_cast<CEditUI*>(pHorizontalLayoutUi->GetItemAt(0));
            if( !lpEditui )continue;
            sChannelInfo.m_esys = (COMMUNICATIONSSYS)GetSys(lpEditui->GetText());
            sChannelInfo.m_nBand = GetBandInfo(sChannelInfo.m_esys,lpEditui->GetText());

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            lpEditui = static_cast<CEditUI*>(pHorizontalLayoutUi->GetItemAt(0));
            if( !lpEditui )continue;
            sChannelInfo.m_strChannel = lpEditui->GetText();

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(2));
            if( !pHorizontalLayoutUi )continue;
            lpEditui = static_cast<CEditUI*>(pHorizontalLayoutUi->GetItemAt(0));
            if( !lpEditui )continue;
            sChannelInfo.m_strPrx = lpEditui->GetText();

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(3));
            if( !pHorizontalLayoutUi )continue;
            lpEditui = static_cast<CEditUI*>(pHorizontalLayoutUi->GetItemAt(0));
            if( !lpEditui )continue;
            sChannelInfo.m_strDrx = lpEditui->GetText();

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(4));
            if( !pHorizontalLayoutUi )continue;
            lpEditui = static_cast<CEditUI*>(pHorizontalLayoutUi->GetItemAt(0));
            if( !lpEditui )continue;
            sChannelInfo.m_strLoss = lpEditui->GetText();

            vBandChannelInfo.push_back(sChannelInfo);
        }

        if( m_lpConfigFileClass )
        {
            m_lpConfigFileClass->ChangeChannelInfo(vBandChannelInfo);
        }

        return true;
    }
}