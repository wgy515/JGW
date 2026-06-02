#include "StdAfx.h"
#include "UIVerticalLayoutUIProjectBtn.h"

namespace UiLib
{
	CUIVerticalLayoutUIProjectBtn::CUIVerticalLayoutUIProjectBtn(void):m_nProjectHeight(30),m_nLastHeight(0),m_nLastWeight(0),mpsUIProjectInfo(NULL)
	{
		//SetDelayedDestroy(false);
	}

	CProjectListUI* CUIVerticalLayoutUIProjectBtn::GetProjectListUI(LPCTSTR strGroupName)
	{
		CProjectListUI* pTemp = NULL;
		for (int i = 0;i < m_aProjectListUI.GetSize();++i)
		{
			pTemp = (CProjectListUI*)m_aProjectListUI.GetAt(i);
			if( _tcsicmp(pTemp->GetGroup(), strGroupName) == 0 )
			{
				return pTemp;
			}
		}
		return NULL;
	}

	CUIVerticalLayoutUIProjectBtn::~CUIVerticalLayoutUIProjectBtn(void)
	{
	
	}

	void CUIVerticalLayoutUIProjectBtn::RemoveAll()
	{
		m_aProjectListUI.Empty();
		m_aProjectItems.Empty();
        m_aSrcProjectItems.Empty();
		CVerticalLayoutUI::RemoveAll();
	}

	LPCTSTR CUIVerticalLayoutUIProjectBtn::GetClass() const
	{
		return _T("UIVerticalLayoutProjectBtn");
	}

	LPVOID CUIVerticalLayoutUIProjectBtn::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_VERTICALLAYOUT_PROJECT_BUTTON) == 0 ) return static_cast<CUIVerticalLayoutUIProjectBtn*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

    void CUIVerticalLayoutUIProjectBtn::InitPluginGroupView(PS_UI_PROJECT_INFO psUIProjectInfo)
    {
        mpsUIProjectInfo = psUIProjectInfo;
        if (!mpsUIProjectInfo) return ;
        RemoveAll();
        m_nLastWeight = 0;
        CProjectListUI* pProjectListUi = NULL;
        for ( std::vector<S_PLUGIN_GROUP_INFO>::iterator itgroup = mpsUIProjectInfo->mvAllPluginInfo.begin();
            itgroup != mpsUIProjectInfo->mvAllPluginInfo.end();
            ++ itgroup )
        {
            this->Add(GenOptionUi(&itgroup[0]));
            pProjectListUi = GenProjectListUI(&itgroup[0]);
            GenProjectList(pProjectListUi,itgroup->mvSubWinInfo);
            this->Add(pProjectListUi);
        }
    }

    COptionUI* CUIVerticalLayoutUIProjectBtn::GenOptionUi( S_PLUGIN_GROUP_INFO* pPluginGroupInfo )
    {
        COptionUI* pOptionUI = new COptionUI;

        pOptionUI->SetGroup( pPluginGroupInfo->mstrGroupID.c_str() );
        pOptionUI->SetText( pPluginGroupInfo->mstrGroupName.c_str() );
        pOptionUI->SetAttribute(_T("disabledtextcolor"),_T("#FFA7A6AA"));
        pOptionUI->SetAttribute(_T("align"),_T("center"));
        pOptionUI->SetFloat();
        //! mpsUIProjectInfo->msGroupButtonLayout.
        pOptionUI->SetAttribute(_T("font"),mpsUIProjectInfo->msGroupButtonLayout.m_strFontID.c_str());
        pOptionUI->SetAttribute(_T("height"),mpsUIProjectInfo->msGroupButtonLayout.m_strHeight.c_str());
        pOptionUI->SetAttribute(_T("textcolor"),mpsUIProjectInfo->msGroupButtonLayout.m_strTextColor.c_str());
        pOptionUI->SetNormalImage( mpsUIProjectInfo->msGroupButtonLayout.mSButtonImage.mstrNormalImage.c_str() );
        pOptionUI->SetHotImage( mpsUIProjectInfo->msGroupButtonLayout.mSButtonImage.mstrHotImage.c_str() );
        pOptionUI->SetPushedImage( mpsUIProjectInfo->msGroupButtonLayout.mSButtonImage.mstrPushedImage.c_str() );
        pOptionUI->SetFocusedImage( mpsUIProjectInfo->msGroupButtonLayout.mSButtonImage.mstrFocusedImage.c_str() );
        pOptionUI->SetDisabledImage( mpsUIProjectInfo->msGroupButtonLayout.mSButtonImage.mstrDisabledImage.c_str() );

        return pOptionUI;
    }

    CProjectListUI* CUIVerticalLayoutUIProjectBtn::GenProjectListUI( S_PLUGIN_GROUP_INFO* pPluginGroupInfo )
    {
        CProjectListUI* pProjectListUI = new CProjectListUI;
        pProjectListUI->SetManager(m_pManager,NULL);
        pProjectListUI->SetGroup( pPluginGroupInfo->mstrGroupID.c_str() );
        pProjectListUI->SetText( pPluginGroupInfo->mstrGroupName.c_str() );
        pProjectListUI->SetAttribute(_T("float"),_T("true"));
        pProjectListUI->SetAttribute(_T("vscrollbar"),_T("true"));

        return pProjectListUI;
    }

    void CUIVerticalLayoutUIProjectBtn::GenProjectList( CProjectListUI* pProjectListUi,std::vector<S_PLUGIN_INFO>& psSubWindowInfos )
    {
        for ( std::vector<S_PLUGIN_INFO>::iterator it = psSubWindowInfos.begin() ;
            it != psSubWindowInfos.end();
            ++it )
        {
            if ( !it[0].mstrPluginName.empty() )
                pProjectListUi->Add( GenProjectBtnUI(&it[0]) );
        }
    }

    CProjectBtnUI* CUIVerticalLayoutUIProjectBtn::GenProjectBtnUI( S_PLUGIN_INFO* pPluginInfo )
    {
        CProjectBtnUI* pProjectBtnUI = new CProjectBtnUI;
        LPS_BUTTON_IMAGE_ pButtonImages = &mpsUIProjectInfo->mvButtonImage[pPluginInfo->mImageGroupID];
        
        pProjectBtnUI->SetAttribute(_T("text"), pPluginInfo->mstrStationName.c_str() );
        pProjectBtnUI->SetAttribute(_T("height"),mpsUIProjectInfo->msProjectButtonLayout.m_strHeight.c_str());
        pProjectBtnUI->SetAttribute(_T("font"),mpsUIProjectInfo->msProjectButtonLayout.m_strFontID.c_str());
        pProjectBtnUI->SetAttribute(_T("imageheight"),mpsUIProjectInfo->msProjectButtonLayout.m_strImageHeight.c_str());
        pProjectBtnUI->SetAttribute(_T("imageweight"),mpsUIProjectInfo->msProjectButtonLayout.m_strImageWeight.c_str());
        pProjectBtnUI->SetAttribute(_T("labelheight"),mpsUIProjectInfo->msProjectButtonLayout.m_strLabelHeight.c_str());
        pProjectBtnUI->SetAttribute(_T("textcolor"),mpsUIProjectInfo->msProjectButtonLayout.m_strLabelTextColor.c_str());
        pProjectBtnUI->SetAttribute(_T("disabledtextcolor"),_T("#FFA7A6AA"));
        pProjectBtnUI->SetAttribute(_T("align"),_T("center"));
        //pProjectBtnUI->SetAttribute(_T("float"),_T("false"));
        //pProjectBtnUI->SetAttribute( _T("tooltip"),_T("点击此按钮，将打开对应的测试程序插件(已经打开的测试程序插件，将重新打开)"));
        pProjectBtnUI->SetAttribute( _T("tooltip"),_T("Click this button to open the corresponding test program plug-in (test program plug-in that has been opened will be reopened)"));
        pProjectBtnUI->SetAttribute(_T("normalimage"),pButtonImages->mstrNormalImage.c_str()/*_T("file='button_nor.bmp' corner='5,5,5,5'")*/);
        pProjectBtnUI->SetAttribute(_T("hotimage"),pButtonImages->mstrHotImage.c_str()/*_T("file='button_down.bmp' corner='5,5,5,5'")*/);
        pProjectBtnUI->SetAttribute(_T("pushedimage"),pButtonImages->mstrPushedImage.c_str()/*_T("file='button_down.bmp' corner='5,5,5,5'")*/);
        pProjectBtnUI->SetAttribute(_T("focusedimage"),pButtonImages->mstrFocusedImage.c_str()/*_T("file='button_over.bmp' corner='5,5,5,5'")*/);
        pProjectBtnUI->SetAttribute(_T("disabledimage"),pButtonImages->mstrDisabledImage.c_str()/*_T("file='disable_btn.png' corner='5,5,5,5'")*/);

        pProjectBtnUI->GetImageButton()->SetDate(pPluginInfo);

        return pProjectBtnUI;
    }

	bool CUIVerticalLayoutUIProjectBtn::IsPrjectButtonClick(CControlUI* pControl)
	{
        if (0 == m_aProjectItems.GetSize()) return false;
		CProjectListUI* pHorizontalLayoutUI = GetProjectListUI(((COptionUI*)(m_aProjectItems.GetAt(0)))->GetGroup());
		if( pHorizontalLayoutUI )
		{
			return pHorizontalLayoutUI->IsProjectButton(pControl);
		}
		return false;
	}

	void CUIVerticalLayoutUIProjectBtn::InitControlPos()
	{
		if ( m_aProjectItems.IsEmpty() ) return;
		CDuiString strTemp;
		RECT rect;
		for (int nIndex = 0;nIndex < 1;++nIndex)
		{
			rect.top = nIndex * m_nProjectHeight + (nIndex+1)*5;
			rect.bottom = rect.top + m_nProjectHeight;
			rect.left = 8;
			rect.right = GetWidth() - 8;
			strTemp.Format(_T("%d,%d,%d,%d"),rect.left,rect.top,rect.right,rect.bottom);
			((COptionUI*)(m_aProjectItems.GetAt(nIndex)))->SetAttribute(_T("pos"),strTemp);
		}

		for (int nIndex = m_aProjectItems.GetSize()-1;nIndex > 0;--nIndex)
		{
			rect.top = GetHeight() - nIndex * m_nProjectHeight - (nIndex+1)*5;
			rect.bottom = rect.top + m_nProjectHeight;
			rect.left = 8;
			rect.right = GetWidth() - 8;
			strTemp.Format(_T("%d,%d,%d,%d"),rect.left,rect.top,rect.right,rect.bottom);
			((COptionUI*)(m_aProjectItems.GetAt(nIndex)))->SetAttribute(_T("pos"),strTemp);
		}
		rect.top = m_nProjectHeight + 10;
		rect.bottom = GetHeight() - (m_aProjectItems.GetSize()-1) * m_nProjectHeight - (m_aProjectItems.GetSize())*5 - 5;
		rect.left = 8;
		rect.right = GetWidth() - 8;
		CProjectListUI* pHorizontalLayoutUI = GetProjectListUI(((COptionUI*)(m_aProjectItems.GetAt(0)))->GetGroup());
		if( pHorizontalLayoutUI )
		{
			strTemp.Format(_T("%d,%d,%d,%d"),rect.left,rect.top,rect.right,rect.bottom);
			pHorizontalLayoutUI->SetAttribute(_T("pos"),strTemp);
			pHorizontalLayoutUI->SetVisible(true);
		}
		NeedUpdate();
	}

	void CUIVerticalLayoutUIProjectBtn::DoPaint(HDC hDC, const RECT& rcPaint)
	{		
 		if( m_nLastWeight != GetWidth() || m_nLastHeight != GetHeight())
 		{
 			m_nLastHeight = GetHeight();
 			m_nLastWeight = GetWidth();
			InitControlPos();
		}
		CVerticalLayoutUI::DoPaint(hDC,rcPaint);
	}
	
	void CUIVerticalLayoutUIProjectBtn::DoEvent(TEventUI& event)
	{
		if( event.Type == UIEVENT__PROJECT_SEL_INDEX_CLEAR )
		{
			for (int nIndex = 0;nIndex < m_aProjectItems.GetSize();++nIndex)
			{
				CProjectListUI* pHorizontalLayoutUI = GetProjectListUI(((COptionUI*)(m_aProjectItems.GetAt(nIndex)))->GetGroup());
				if( pHorizontalLayoutUI ) pHorizontalLayoutUI->SetSelIndex(-1);
			}
		}
		else
		{
			CVerticalLayoutUI::DoEvent(event);
		}

	}

    int CUIVerticalLayoutUIProjectBtn::GetSelProjectIndex()
    {
        for (int index = 0;index < m_aSrcProjectItems.GetSize();++ index)
        {
            if (m_pCurretProjectItems == m_aSrcProjectItems.GetAt(index)) return index;
        }
        return 0;
    }

    void CUIVerticalLayoutUIProjectBtn::SetSelProjectIndex(int nIndex)
    {
        if (0 == m_aProjectItems.GetSize()) return;

        if (nIndex < 0 || nIndex >= m_aProjectItems.GetSize()) nIndex = 0;

        LPVOID pSend = m_aProjectItems.GetAt(nIndex);
		m_pCurretProjectItems = pSend;
        CProjectListUI* pHorizontalLayoutUI = GetProjectListUI(((COptionUI*)(m_aProjectItems.GetAt(0)))->GetGroup());
        if( pHorizontalLayoutUI ) pHorizontalLayoutUI->SetVisible(false);
        LPVOID pData = m_aProjectItems.GetAt(0);
        m_aProjectItems.SetAt(0,pSend);
        m_aProjectItems.SetAt(nIndex,pData);
        InitControlPos();
    }

	bool CUIVerticalLayoutUIProjectBtn::Add(CControlUI* pControl)
	{
		if (_tcsicmp(pControl->GetClass(), _T("OptionUI")) == 0)
		{
			pControl->OnNotify += MakeDelegate(this,&CUIVerticalLayoutUIProjectBtn::OnDBClickProject,_T(""));
			m_aProjectItems.Add(pControl);
            m_aSrcProjectItems.Add(pControl);
		}
		else if (_tcsicmp(pControl->GetClass(), _T("UIProjectList")) == 0)
		{
			pControl->SetVisible(false);
			m_aProjectListUI.Add(pControl);
		}
		pControl->SetAttribute(_T("float"),_T("true"));
		return CVerticalLayoutUI::Add(pControl);
	}

	bool CUIVerticalLayoutUIProjectBtn::OnDBClickProject(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
	{
		if ( pTNotifyUI->sType == DUI_MSGTYPE_CLICK )
		{
			for (int nIndex = 0;nIndex < m_aProjectItems.GetSize();++nIndex)
			{
				if ( pTNotifyUI->pSender == m_aProjectItems.GetAt(nIndex) )
				{
                    m_pCurretProjectItems = pTNotifyUI->pSender;
					CProjectListUI* pHorizontalLayoutUI = GetProjectListUI(((COptionUI*)(m_aProjectItems.GetAt(0)))->GetGroup());
					if( pHorizontalLayoutUI ) pHorizontalLayoutUI->SetVisible(false);
					LPVOID pData = m_aProjectItems.GetAt(0);
					m_aProjectItems.SetAt(0,pTNotifyUI->pSender);
					m_aProjectItems.SetAt(nIndex,pData);           
					//! m_nSelPorjectIndex = nIndex;
					InitControlPos();
					return true;
				}
			}
		}
		return false;
	}

	/************************************************************************/
	/*      CProjectListUI                                                  */
	/************************************************************************/

	CProjectListUI::CProjectListUI():m_nSelIndex(-1)
	{
	//	SetDelayedDestroy(false);
	}

	CProjectListUI::~CProjectListUI()
	{

	}

	void CProjectListUI::RemoveAll()
	{
		m_aProjectList.Empty();
		CVerticalLayoutUI::RemoveAll();
	}

	void CProjectListUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		this->GetCount();
		m_aProjectList.GetSize();
		CVerticalLayoutUI::DoPaint(hDC,rcPaint);
	}

	LPCTSTR CProjectListUI::GetClass() const
	{
		return _T("UIProjectList");
	}

	LPVOID CProjectListUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_PROJECTLIST) == 0 ) return static_cast<CProjectListUI*>(this);
		return CVerticalLayoutUI::GetInterface(pstrName);
	}

	LPCTSTR CProjectListUI::GetGroup() const
	{
		return m_sGroupName;
	}

	void CProjectListUI::SetGroup(LPCTSTR pStrGroupName)
	{
		if( pStrGroupName == NULL ) {
			if( m_sGroupName.IsEmpty() ) return;
			m_sGroupName.Empty();
		}
		else {
			if( m_sGroupName == pStrGroupName ) return;
			m_sGroupName = pStrGroupName;
		}
	}

	bool CProjectListUI::Add(CControlUI* pControl)
	{
		if (_tcsicmp(pControl->GetClass(), _T("UIProjectBtn")) == 0)
		{
			m_aProjectList.Add(pControl);
			((CProjectBtnUI*)pControl)->GetImageButton()->OnNotify += MakeDelegate(this,&CProjectListUI::OnDBClickItem,_T(""));
			pControl->SetFloat(false);
		}
		return CVerticalLayoutUI::Add(pControl);
	}

	bool CProjectListUI::OnDBClickItem(TNotifyUI* pTNotifyUI,LPARAM lParam,WPARAM wParam)
	{
		if ( pTNotifyUI->sType == DUI_MSGTYPE_CLICK )
		{
			for (int nIndex = 0;nIndex < m_aProjectList.GetSize();++nIndex)
			{
				if ( pTNotifyUI->pSender == ((CProjectBtnUI*)m_aProjectList.GetAt(nIndex))->GetImageButton() )
				{
					if( -1 != m_nSelIndex )
						((CProjectBtnUI*)m_aProjectList.GetAt(m_nSelIndex))->GetImageButton()->SetNormalImage(m_strNormalImage);
					TEventUI eventUi = {0};
					eventUi.Type = UIEVENT__PROJECT_SEL_INDEX_CLEAR;
					this->GetParent()->DoEvent(eventUi);
					SetSelIndex(nIndex);
					m_strNormalImage = ((CProjectBtnUI*)m_aProjectList.GetAt(m_nSelIndex))->GetImageButton()->GetNormalImage();
					((CProjectBtnUI*)m_aProjectList.GetAt(m_nSelIndex))->GetImageButton()->SetNormalImage(((CProjectBtnUI*)m_aProjectList.GetAt(m_nSelIndex))->GetImageButton()->GetFocusedImage());
					return false;
				}
			}
		}
		return false;
	}

	void CProjectListUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if(_tcscmp(pstrName, _T("group")) == 0 )
			SetGroup(pstrValue);
		else if (  _tcscmp(pstrName, _T("float")) == 0 )
		{
			CVerticalLayoutUI::SetAttribute(_T("float"),_T("true"));
		}
		else
			CVerticalLayoutUI::SetAttribute(pstrName,pstrValue);
	}

	void CProjectListUI::SetSelIndex(int nSelIndex)
	{
		if( -1 != m_nSelIndex ) 
		{
			((CProjectBtnUI*)m_aProjectList.GetAt(m_nSelIndex))->GetImageButton()->SetNormalImage(m_strNormalImage);
		}
		m_nSelIndex = nSelIndex;
	}

	int CProjectListUI::GetSelIndex()
	{
		return m_nSelIndex;
	}

	bool CProjectListUI::IsProjectButton(CControlUI* pControl)
	{
		if ( m_nSelIndex < 0 || m_nSelIndex >= m_aProjectList.GetSize() ) return false;
		return ((CProjectBtnUI*)m_aProjectList.GetAt(m_nSelIndex))->GetImageButton() == pControl;
	}


	/************************************************************************/
	/*      CProjectBtnUI                                                   */
	/************************************************************************/


	CProjectBtnUI::CProjectBtnUI():m_pImageBtn(NULL),m_pLabelUI(NULL)
		,m_nImageHeight(48)
		,m_nImageWeight(48)
		,m_nLabelHeight(20)
		,m_bInit(true)
		,m_nLabelAndImageInterval(10)
	{
		m_pImageBtn = new CButtonUI;
		m_pLabelUI = new CLabelUI;
		m_pLabelUI->SetEnabled(true);
		m_pImageBtn->SetEnabled(true);
		m_pLabelUI->SetAttribute(_T("height"),_T("20"));
		//m_pLabelUI->SetAttribute(_T("autocalcwidth"),_T("true"));
		m_pImageBtn->SetAttribute(_T("height"),_T("48"));
		m_pImageBtn->SetAttribute(_T("weight"),_T("48"));
		m_pImageBtn->SetAttribute(_T("align"),_T("center"));
		Add(m_pImageBtn);
		Add(m_pLabelUI);
	}

	CProjectBtnUI::~CProjectBtnUI()
	{

	}

	LPCTSTR CProjectBtnUI::GetClass() const
	{
		return _T("UIProjectBtn");
	}

	LPVOID CProjectBtnUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_PROJECTBTN) == 0 ) return static_cast<CProjectBtnUI*>(this);
		return CContainerUI::GetInterface(pstrName);
	}
	
	void CProjectBtnUI::SetPos(RECT rc)
	{
		if( m_bInit )
		{
			InitControlPos(rc.right - rc.left);
			m_bInit = false;
		}
		CVerticalLayoutUI::SetPos(rc);
	}

	void CProjectBtnUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		m_nLastHeight = GetHeight();
		m_nLastWeight = GetWidth();

// 		m_pLabelUI->m_hAlign = DT_CENTER;
// 		m_pLabelUI->m_vAlign = DT_CENTER;
		if( _tcscmp(pstrName, _T("align")) == 0 
			|| _tcscmp(pstrName, _T("text")) == 0 
			|| _tcscmp(pstrName, _T("textcolor")) == 0
			|| _tcscmp(pstrName, _T("font")) == 0
			|| _tcscmp(pstrName, _T("autocalcwidth")) == 0
			)
		{
			m_pLabelUI->SetAttribute(pstrName,pstrValue);
		}
		else if ( _tcscmp(pstrName, _T("hotimage")) == 0 
			|| _tcscmp(pstrName, _T("pushedimage")) == 0 
			|| _tcscmp(pstrName, _T("normalimage")) == 0 
			|| _tcscmp(pstrName, _T("focusedimage")) == 0 
			|| _tcscmp(pstrName, _T("disabledimage")) == 0 
			|| _tcscmp(pstrName, _T("tooltip")) == 0
			)
		{
			m_pImageBtn->SetAttribute(pstrName,pstrValue);
		}
		else if ( _tcscmp(pstrName, _T("imageweight")) == 0 )
		{
			m_nImageWeight = _ttoi(pstrValue);
			m_pImageBtn->SetAttribute(_T("weight"),pstrValue);
		}
		else if ( _tcscmp(pstrName, _T("imageheight")) == 0)
		{
			m_nImageHeight = _ttoi(pstrValue);
			m_pImageBtn->SetAttribute(_T("height"),pstrValue);
		}
		else if (  _tcscmp(pstrName, _T("labelheight")) == 0)
		{
			m_nLabelHeight = _ttoi(pstrValue);
			m_pLabelUI->SetAttribute(_T("height"),pstrValue);
		}
		else if (  _tcscmp(pstrName, _T("interval")) == 0 )
		{
			m_nLabelAndImageInterval = _ttoi(pstrValue);
		}
		else if (  _tcscmp(pstrName, _T("float")) == 0 )
		{
			m_pLabelUI->SetAttribute(_T("float"),_T("false"));
			m_pImageBtn->SetAttribute(_T("float"),_T("false"));
		}
		else
			CVerticalLayoutUI::SetAttribute(pstrName,pstrValue);
	}

	CButtonUI* CProjectBtnUI::GetImageButton()
	{
		return m_pImageBtn;
	}

	CLabelUI* CProjectBtnUI::GetLabelUI()
	{
		return m_pLabelUI;
	}

	void CProjectBtnUI::DoPaint(HDC hDC, const RECT& rcPaint)
	{
		if( m_nLastWeight != GetWidth() || m_nLastHeight != GetHeight())
		{
			m_nLastHeight = GetHeight();
			m_nLastWeight = GetWidth();
			InitControlPos(m_nLastWeight);
		}
		CVerticalLayoutUI::DoPaint(hDC,rcPaint);
	}

	void CProjectBtnUI::InitControlPos(int width)
	{
		CDuiString strTemp;
		RECT rect;
// 
// 		rect.top = 5;
// 		rect.bottom = rect.top + m_nImageHeight;
		rect.left = ( width - m_nImageWeight )/2;
// 		rect.right = rect.left + m_nImageWeight;
		//strTemp.Format(_T("%d,%d,%d,%d"),rect.left,rect.top,rect.right,rect.bottom);
		//m_pImageBtn->SetAttribute(_T("pos"),strTemp);
		strTemp.Format(_T("%d,5,%d,0"),rect.left,rect.left);
		m_pImageBtn->SetAttribute(_T("padding"),strTemp);
		//strTemp.Format(_T("0,%d,0,0"),m_nImageHeight + 5 + m_nLabelAndImageInterval );
		//strTemp.Format(_T("0,%d,%d,%d"),m_pImageBtn->GetPos().bottom + 5 + m_nLabelAndImageInterval,GetPos().right,m_pImageBtn->GetPos().bottom + 5 + m_nLabelAndImageInterval + m_nLabelHeight);
		//m_pLabelUI->SetAttribute(_T("pos"),strTemp);
		//strTemp.Format(_T("0,%d,0,0"),m_nImageHeight + 5 + m_nLabelAndImageInterval);
		//m_pLabelUI->SetAttribute(_T("padding"),strTemp);

// 		rect.top += m_nLabelAndImageInterval;
// 		rect.bottom = rect.top + m_nLabelHeight;
// 		rect.left = 0;
// 		rect.right = GetWidth();
// 		strTemp.Format(_T("%d,%d,%d,%d"),rect.left,rect.top,rect.right,rect.bottom);
// 		m_pLabelUI->SetAttribute(_T("pos"),strTemp);

	}
}
