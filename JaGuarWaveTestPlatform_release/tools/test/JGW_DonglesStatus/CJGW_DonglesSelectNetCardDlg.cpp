#include "StdAfx.h"
#include "CJGW_DonglesSelectNetCardDlg.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <pcap.h>
namespace JGW
{
    pcap_if_t *alldevs = NULL;
    CCJGW_DonglesSelectNetCardDlg::CCJGW_DonglesSelectNetCardDlg(std::wstring& strNetCardInfo):mstrNetCardInfo(strNetCardInfo)
    {
    }


    CCJGW_DonglesSelectNetCardDlg::~CCJGW_DonglesSelectNetCardDlg(void)
    {
        if (alldevs) {pcap_freealldevs(alldevs);alldevs = NULL;}
    }

    void CCJGW_DonglesSelectNetCardDlg::OnCloseWindow()
    {
        if (alldevs) {pcap_freealldevs(alldevs);alldevs = NULL;}
    }

    void CCJGW_DonglesSelectNetCardDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (0 < mpSelNetCardCombo->GetCurSel())
            {
                mstrNetCardInfo = JGW_A2W_A((const char*)mpSelNetCardCombo->GetItemAt(mpSelNetCardCombo->GetCurSel())->GetDate());
                Close(IDOK);
            }
        }
    }

    void CCJGW_DonglesSelectNetCardDlg::OnInitWindow()
    {
        mpSelNetCardCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"selNetCombo"));
        if (!mpSelNetCardCombo)
        {        
            MessageBox(m_PaintManager.GetPaintWindow(),_T("Æ¤·ô¿Ø¼þÈ±Ê§"),_T("³õÊ¼»¯Ê§°Ü"),MB_ICONERROR);
            Close(IDCANCEL);
            return;
        }
#if 1
        pcap_if_t *d = NULL;
        char errbuf[PCAP_ERRBUF_SIZE * 4+ 1] = {0};
        if (-1 == pcap_findalldevs(&alldevs, errbuf))
        {
             MessageBoxA(m_PaintManager.GetPaintWindow(),errbuf,"WinPcap³õÊ¼»¯Ê§°Ü",MB_ICONERROR);
            return;
        }

        for(d = alldevs; d; d = d->next)
        {
            CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
            pListLabelEle->SetDate(d->name);
            pListLabelEle->SetText(JGW_A2W_A(d->description).c_str());
            mpSelNetCardCombo->Add(pListLabelEle);
        }	
#else
        CListLabelElementUI* pListLabelEle = new CListLabelElementUI;
        pListLabelEle->SetDate("13131");
        pListLabelEle->SetText(L"13131");
        mpSelNetCardCombo->Add(pListLabelEle);
#endif
    }
}

