#include "StdAfx.h"
#include "CJGW_LossConfigToolDlg.h"
#include "CJGW_LoadCalDBLossConfig.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include "CJGW_LossEdtDlg.h"
#include "CJGW_LossToolConfig.h"
#include <JGW_FoundationFunc/JGW_FilePath.h>
#include "CJGW_LoadStationCalData.h"

namespace JGW
{
    CCJGW_LossConfigToolDlg::CCJGW_LossConfigToolDlg(void) : mbIsChange(false) , mpsEquipProjectConfig(NULL),mpLoadLossConfigImpl(NULL)
    {
        S_EQUIP_PROJECT_CONFIG sEquipProjectConfig;
        sEquipProjectConfig.mstrGroupName = L"";
        sEquipProjectConfig.mstrModuleFolder = L"..\\Databases";
        sEquipProjectConfig.mstrConfigFileName = L"CalDB_NET.xml";
        sEquipProjectConfig.mstrTestName = L"Qualcomm Calibrate";
        sEquipProjectConfig.mcEquipType = E_QCOMM_CAL;
        mvsEquipProjectConfig.push_back(sEquipProjectConfig);
    }


    CCJGW_LossConfigToolDlg::~CCJGW_LossConfigToolDlg(void)
    {
        if (mpLoadLossConfigImpl)
        {
            delete mpLoadLossConfigImpl;
            mpLoadLossConfigImpl = NULL;
        }
    }

    void CCJGW_LossConfigToolDlg::OnInitWindow()
    {
        LoadControlUiClassPtr();
        LoadTargetProjectView();
    }

    void CCJGW_LossConfigToolDlg::LoadTargetProjectView()
    {
        mpTargetCombo->RemoveAll();
        CCJGW_LossToolConfig equipToolConfig;
        std::wstring strTemp = JGW_GetApplicationFolder();
        strTemp += L"lossconfigtool.ini";
        equipToolConfig.LoadLossToolConfig(strTemp,mvsEquipProjectConfig);

        for (std::vector<S_EQUIP_PROJECT_CONFIG>::iterator it = mvsEquipProjectConfig.begin();it != mvsEquipProjectConfig.end(); ++it)
        {
            CListLabelElementUI* pListLabelEUi = new CListLabelElementUI();
            strTemp = L"";
            JGW_FormatWString(strTemp,L"%s %s Loss Editor(%s)",it->mstrGroupName.c_str(),it->mstrTestName.c_str(),it->mstrConfigFileName.c_str());
            pListLabelEUi->SetText(strTemp.c_str());
            pListLabelEUi->SetDate(&it[0]);
            mpTargetCombo->Add(pListLabelEUi);
        }
        if (mpTargetCombo->GetCount() >= 1) mpTargetCombo->SelectItem(0);
    }

    void CCJGW_LossConfigToolDlg::OnUpdateLossView()
    {
        mpLossList->GetHeader()->RemoveAll();
        mpLossList->GetList()->RemoveAll();
        msCalDBLossConfig.mvCalConfigNode.clear();
        if (mpsEquipProjectConfig && mpLoadLossConfigImpl)
        {
            mpLoadLossConfigImpl->LoadLossConfig(mpsEquipProjectConfig,msCalDBLossConfig);
            mpLossFilePathEdt->SetText(mpLoadLossConfigImpl->GetLossConfigXmlPath().c_str());
        }
        UpdateTreeView();
    }

    void CCJGW_LossConfigToolDlg::OnNotify(TNotifyUI& msg)
    {
        if (msg.sType == DUI_MSGTYPE_ITEMSELECT)
        {
            if (msg.pSender == mpTreeView)
            {
                CTreeNodeUI* pTreeNodeUi =  (CTreeNodeUI*)mpTreeView->GetItemAt(msg.wParam);
                PS_CAL_PATH_NODE psCalPathNode = (PS_CAL_PATH_NODE)pTreeNodeUi->GetDate();
                SaveLossListChange();
                UpdateLossList(psCalPathNode);
            }
            else if (msg.pSender == mpTargetCombo)
            {
                mpsEquipProjectConfig = NULL;
                int curSel = mpTargetCombo->GetCurSel();
                if (curSel >= 0 && curSel < static_cast<int>(mvsEquipProjectConfig.size()))
                {
                    mpsEquipProjectConfig = &mvsEquipProjectConfig[curSel];
                    delete mpLoadLossConfigImpl;
                    mpLoadLossConfigImpl = NULL;
                    switch (mpsEquipProjectConfig->mcEquipType)
                    {
                    case E_QCOMM_CAL:
                        mpLoadLossConfigImpl = new CCJGW_LoadCalDBLossConfig();
                        break;
                    case E_StationCalDataFile:
                        mpLoadLossConfigImpl = new CCJGW_LoadStationCalData();
                        break;
                    }
                }
                OnUpdateLossView();
            }  
        }
        else if (msg.sType == DUI_MSGTYPE_CLICK)
        {
            if (msg.pSender == mpSetLossBtn)
            {
                OnSetLossClick();
            }
            else if (msg.pSender == mpAddLossBtn)
            {
                OnAddLossClick();
            }
            else if (msg.pSender == mpSaveLossBtn)
            {
                SaveLossListChange();
                OnSaveLossClick();
            }
        }
    }

    void CCJGW_LossConfigToolDlg::OnCloseWindow()
    {

    }

    bool CCJGW_LossConfigToolDlg::LoadControlUiClassPtr()
    {
        bool bFindControlOk = true;

        mpTreeView = static_cast<CTreeViewUI*>(m_PaintManager.FindControl(L"tree"));
        mpDragHorizontalLayoutUI = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(L"drag"));
        mpLossList = static_cast<CListTestCtrlUI*>(m_PaintManager.FindControl(L"losslist"));
        mpLossFilePathEdt = static_cast<CRichEditUI*>(m_PaintManager.FindControl(L"lossFilePathEdt"));
        mpTargetCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(L"targetCombo"));
        bFindControlOk &= (NULL != mpTreeView);
        bFindControlOk &= (NULL != mpDragHorizontalLayoutUI);
        bFindControlOk &= (NULL != mpLossList);
        bFindControlOk &= (NULL != mpLossFilePathEdt);
        bFindControlOk &= (NULL != mpTargetCombo);

        mpSetLossBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"setLossBtn"));
        mpAddLossBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"addLossBtn"));
        mpSaveLossBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"saveLossBtn"));
        bFindControlOk &= (NULL != mpSetLossBtn);
        bFindControlOk &= (NULL != mpAddLossBtn);
        bFindControlOk &= (NULL != mpSaveLossBtn);

        if(!bFindControlOk) Close(IDCANCEL);
        //! mpTreeView->m_pDragingCtrl = mpDragHorizontalLayoutUI;
        return bFindControlOk;
    }

    bool CCJGW_LossConfigToolDlg::UpdateTreeView()
    {
        mpTreeView->RemoveAll();
        for (std::vector<S_CAL_CONFIG_NODE>::iterator it = msCalDBLossConfig.mvCalConfigNode.begin(); it != msCalDBLossConfig.mvCalConfigNode.end();++ it)
        {
            CTreeNodeUI* pTreeNodeUi = new CTreeNodeUI;
            pTreeNodeUi->SetItemText(it->mstrCalConfigName.c_str());
            pTreeNodeUi->SetAttribute(L"folderattr",L"padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16' \" hotimage=\"file='treeview_b.png' source='36,0,72,16' \" selectedimage=\"file='treeview_a.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_a.png' source='36,0,72,16' \"");
            for (std::vector<S_CAL_PATH_NODE>::iterator itPath = it->mvCalPathNode.begin();
                itPath !=  it->mvCalPathNode.end();
                ++ itPath)
            {
                CTreeNodeUI* pSubTreeNodeUi = new CTreeNodeUI;
                if (E_QCOMM_CAL == mpsEquipProjectConfig->mcEquipType)
                {
                    pSubTreeNodeUi->SetItemText(CalPathNumberToCalDBLossString(itPath->mnNumber));
                }
                else
                {
                    pSubTreeNodeUi->SetItemText(itPath->mstrCalConfigName.c_str());
                }

                pSubTreeNodeUi->SetAttribute(L"folderattr",L"padding=\"0,0,5,0\" width=\"36\" height=\"16\" normalimage=\"file='treeview_b.png' source='0,0,36,16' \" hotimage=\"file='treeview_b.png' source='36,0,72,16' \" selectedimage=\"file='treeview_a.png' source='0,0,36,16' \" selectedhotimage=\"file='treeview_a.png' source='36,0,72,16' \"");
                pSubTreeNodeUi->SetDate(&itPath[0]);
                pTreeNodeUi->AddChildNode(pSubTreeNodeUi);      
            }
            pTreeNodeUi->GetFolderButton()->SetCheck(true);
            mpTreeView->Add(pTreeNodeUi);
            mpTreeView->SetItemExpand(false,pTreeNodeUi);
        }
        return true;
    }

    const wchar_t* CCJGW_LossConfigToolDlg::CalPathNumberToCalDBLossString(int number)
    {
        switch (number)
        {
        case 5:
            return L"UplinkCP1";
        case 4:
            return L"DownlinkCP1";
        case 14:
            return L"DownlinkAuxCP1";
        }
        return L"UnknowLink";
    }

    void CCJGW_LossConfigToolDlg::UpdateLossList(PS_CAL_PATH_NODE psCalPathNode)
    {
        mpLossList->GetHeader()->RemoveAll();
        mpLossList->GetList()->RemoveAll();
        mpLossList->SetDate(psCalPathNode);
        if (!psCalPathNode) return ;

        mpLossList->InsertColumn(0,_T("Frequency(MHz)"),0);
        mpLossList->InsertColumn(1,_T("Loss(dB)"),0);
        mpLossList->GetHeader()->GetItemAt( 0 )->SetScale( 48 );
        mpLossList->GetHeader()->GetItemAt( 1 )->SetScale( 48 );

        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CEditUI* lpEditui = NULL;
        std::wstring strTemp = L"",strChannel,strLoss;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;

        for (std::vector<S_CAL_POINT_NODE>::iterator it = psCalPathNode->mvCalPoint.begin();
            it != psCalPathNode->mvCalPoint.end();
            ++ it)
        {
            CDialogBuilder builder1;
            int i = 0;
            lplsitContainerItem = (CListContainerElementUIEx*)(builder1.Create(_T("sigle_list_loss.xml"),(UINT)0,NULL,&m_PaintManager));
            if( !lplsitContainerItem )continue;
            if(lplsitContainerItem != NULL) mpLossList->InsertItem(mpLossList->GetCount(), 40, lplsitContainerItem);
            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(0));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(it->mfFreq.c_str());

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            pHorizontalLayoutUi->GetItemAt(i)->SetText(it->mstrLoss.c_str());
        }
    }

    void CCJGW_LossConfigToolDlg::SaveLossListChange()
    {
        PS_CAL_PATH_NODE psCalPathNode = (PS_CAL_PATH_NODE)mpLossList->GetDate();
        if (!psCalPathNode || 0 == mpLossList->GetList()->GetCount()) return ;

        CListContainerElementUIEx* lplsitContainerItem = NULL;
        CEditUI* lpEditui = NULL;
        CHorizontalLayoutUI* pHorizontalLayoutUi = NULL;

        for ( int i = 0;i < mpLossList->GetCount();i ++ )
        {
            lplsitContainerItem = static_cast<CListContainerElementUIEx*>(mpLossList->GetItemAt(i));
            if( !lplsitContainerItem )continue;

            pHorizontalLayoutUi = static_cast<CHorizontalLayoutUI*>(lplsitContainerItem->GetItemAt(1));
            if( !pHorizontalLayoutUi )continue;
            lpEditui = static_cast<CEditUI*>(pHorizontalLayoutUi->GetItemAt(0));
            if( !lpEditui )continue;
            if (NULL == JGW_WStrComparenoCaseWStr(psCalPathNode->mvCalPoint.at(i).mstrLoss.c_str(),lpEditui->GetText().GetData())) continue;
            mbIsChange = true;
            psCalPathNode->mvCalPoint.at(i).mstrLoss = lpEditui->GetText().GetData();
        }
    }

    void CCJGW_LossConfigToolDlg::OnSaveLossClick()
    {
        if (NULL == mpsEquipProjectConfig || msCalDBLossConfig.mvCalConfigNode.empty())
        {
            ::MessageBox(m_PaintManager.GetPaintWindow(),L"Loss Config File Save Fail",L"Save Loss File",MB_ICONERROR);
            return ;
        }
        if (!mpLoadLossConfigImpl || !mpLoadLossConfigImpl->SaveLossConfig(mpsEquipProjectConfig,msCalDBLossConfig))
        {
            ::MessageBox(m_PaintManager.GetPaintWindow(),L"Loss Config File Save Fail",L"Save Loss File",MB_ICONERROR);
        }
        mbIsChange = false;
    }

    void CCJGW_LossConfigToolDlg::OnSetLossClick()
    {
        PS_CAL_PATH_NODE psCalPathNode = (PS_CAL_PATH_NODE)mpLossList->GetDate();
        if (!psCalPathNode || 0 == mpLossList->GetList()->GetCount()) return ;

        CCJGW_LossEdtDlg lossEdtDlg;
        if (IDCANCEL == lossEdtDlg.DoModal(m_PaintManager.GetPaintWindow())) return ;

        std::wstring strTemp = lossEdtDlg.GetInputLoss().c_str();
        for (size_t i = 0;i < psCalPathNode->mvCalPoint.size();i ++ )
        {
            psCalPathNode->mvCalPoint.at(i).mstrLoss = strTemp;
        }
        UpdateLossList(psCalPathNode); 
    }

    void CCJGW_LossConfigToolDlg::OnAddLossClick()
    {
        PS_CAL_PATH_NODE psCalPathNode = (PS_CAL_PATH_NODE)mpLossList->GetDate();
        if (!psCalPathNode || 0 == mpLossList->GetList()->GetCount()) return ;

        CCJGW_LossEdtDlg lossEdtDlg;
        if (IDCANCEL == lossEdtDlg.DoModal(m_PaintManager.GetPaintWindow())) return ;

        std::wstring strTemp;
        float fLoss = 0.00f,fAddLoss = static_cast<float>(_ttof(lossEdtDlg.GetInputLoss().c_str()));
        for (size_t i = 0;i < psCalPathNode->mvCalPoint.size();i ++ )
        {
            fLoss = static_cast<float>(_ttof(psCalPathNode->mvCalPoint.at(i).mstrLoss.c_str())) + fAddLoss;
            JGW_FormatWString(strTemp,L"%.2f",fLoss);
            psCalPathNode->mvCalPoint.at(i).mstrLoss = strTemp;
        }
        UpdateLossList(psCalPathNode); 
    }
}