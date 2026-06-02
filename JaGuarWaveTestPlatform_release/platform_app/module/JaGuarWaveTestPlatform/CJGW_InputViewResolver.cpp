#include "StdAfx.h"
#include "CJGW_InputViewResolver.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/CJGW_NumberRule.h>
#include <LanguageTranslationPlugin/LanguageTranslationPluginDefine.h>
#include "../../../platform_include/JGW_MSG_ID_Define.h"
namespace JGW
{
    CCJGWInputViewResolver::CCJGWInputViewResolver(LPS_ALL_INPUT_UI_ATTR_SETTING psAllControlAttrSetting) : mpsAllControlAttrSetting(psAllControlAttrSetting),m_pCloseBtn(NULL),mnMaxChar(9999),mnMinChar(0),meStringUpperLowCaseType(E_STRING_UPPERCASE_TYPE)
    {
        _ASSERT(mpsAllControlAttrSetting);
    }


    CCJGWInputViewResolver::~CCJGWInputViewResolver(void)
    {
    }

    LPCTSTR CCJGWInputViewResolver::GetWindowClassName() const
    {
        return _T("CCTFViewResolver_UiLib");
    }

    std::wstring CCJGWInputViewResolver::GetControlStringAttr( LPCTSTR name,LPCTSTR attr_name )
    {
        CControlUI* pControlUi = m_PaintManager.FindControl( name );
        if ( !pControlUi ) L"";

        return pControlUi->GetAttribute( attr_name ).pcwchar;
    }

    unUserData CCJGWInputViewResolver::GetControlAttr( LPCTSTR name,LPCTSTR attr_name )
    {
        unUserData nRetData = {{0,0,0,0}};
        CControlUI* pControlUi = m_PaintManager.FindControl( name );
        if ( !pControlUi ) nRetData;

        return pControlUi->GetAttribute( attr_name );
    }


    bool CCJGWInputViewResolver::InitWindow()
    {
        SetTimer(GetHWND(),1,10,NULL);

        //closebtn
        m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(L"closebtn"));

        m_PaintManager.GetEventSource() += MakeDelegate(this,&CCJGWInputViewResolver::OnEventCustomMessage,0);

        return true;
    }

	void CCJGWInputViewResolver::OnCloseWindow()
	{
		m_PaintManager.GetEventSource() -= MakeDelegate(this,&CCJGWInputViewResolver::OnEventCustomMessage,0);
	}

    void CCJGWInputViewResolver::Notify( TNotifyUI& msg )
    {
        if ( msg.sType == DUI_MSGTYPE_CLICK)
        {
            if ( m_pCloseBtn == msg.pSender )
            {
                Close(IDCANCEL);
                return;
            }
        }
    }

    void CCJGWInputViewResolver::CloseInputWindow()
    {
        //! inputTip
        std::wstring strTemp;

        CTextUI* pControlUi = static_cast<CTextUI*>(m_PaintManager.FindControl(L"inputTip"));
        if ( !pControlUi ) { Close(IDOK); return ;}
        CEditUI* pEditUI = static_cast<CEditUI*>(m_PaintManager.FindControl(L"inputEdit"));
        if (!pEditUI){ Close(IDOK); return ;}

        if (mnMaxChar < pEditUI->GetText().GetLength() || mnMinChar > pEditUI->GetText().GetLength())
        {       
            JGW_FormatWString(strTemp,GetString(E_INPUT_GRETERTHAN_EQUAL_LESSTHAN_CHARACTERS),mnMinChar,mnMaxChar);
            pControlUi->SetText(strTemp.c_str());
            pControlUi->SetTextColor(FAIL_BK_COLOR);
            return ;
        }

        switch (meStringUpperLowCaseType)
        {
        case E_STRING_NOT_IGNORE_CASE_TYPE:
            if (-1 == pEditUI->GetText().Find(mstrInputPrefix.c_str()))
            {
                JGW_FormatWString(strTemp,GetString(E_INPUT_PREFIX_NOTMATCH_PLEASE_REINPUT),mstrInputPrefix.c_str());
                pControlUi->SetText(strTemp.c_str());
                pControlUi->SetTextColor(FAIL_BK_COLOR);
                return ;
            }
            break;
        case E_STRING_UPPERCASE_TYPE:
        case E_STRING_LOWCASE_TYPE:
            if (!mstrInputPrefix.empty() && NULL == JGW_WStrCaseWStr(pEditUI->GetText().GetData(),mstrInputPrefix.c_str()))
            {
                JGW_FormatWString(strTemp,GetString(E_INPUT_PREFIX_NOTMATCH_PLEASE_REINPUT),mstrInputPrefix.c_str());
                pControlUi->SetText(strTemp.c_str());
                pControlUi->SetTextColor(FAIL_BK_COLOR);
                return ;
            }
            break;
        }

        switch (mnNumberType)
        {
        case E_IMEI:
        case E_IMEI2:
            {
                std::wstring strIMEI;
                if (!CCJGW_NumberRule::GetIMEIChecksum(pEditUI->GetText().GetData(),strIMEI))
                {
                    pControlUi->SetText(GetString(E_INPUT_IMEI_VERIFICATION_FAIL));
                    pControlUi->SetTextColor(FAIL_BK_COLOR);
                    return ;
                }
                else
                {
                    if (14 != pEditUI->GetText().GetLength() 
                        && NULL != JGW_WStrComparenoCaseWStr(pEditUI->GetText().GetData(),strIMEI.c_str())) 
                    {  
                        JGW_FormatWString(strTemp,GetString(E_INPUT_IMEI_CHECKCODE),strIMEI.c_str());
                        pControlUi->SetText(strTemp.c_str());
                        pControlUi->SetTextColor(FAIL_BK_COLOR);
                        return ;
                    }
                }
            }
            break;
        default:
            break;
        }
        Close(IDOK);
    }

    int CCJGWInputViewResolver::DoModal()
    {
#if defined USE_ZIP_RESOURCE_ENABLE
        SetResourceType(UILIB_ZIPRESOURCE,IDR_ZIPRES1);
        CreateDuiDialogWindow( mpsAllControlAttrSetting->m_sWindowAttr.mhParentHwnd,mpsAllControlAttrSetting->m_sWindowAttr.mstrSkinFolder.c_str(),mpsAllControlAttrSetting->m_sWindowAttr.mstrSkinFile.c_str(),mpsAllControlAttrSetting->m_sWindowAttr.dwStyle,mpsAllControlAttrSetting->m_sWindowAttr.dwExStyle,mpsAllControlAttrSetting->m_sWindowAttr.mstrWindowName.c_str() );
#elif defined USE_ZIP_FILE_ENABLE
        SetResourceType(UILIB_ZIP,0,L"skin\\JaGuarWave.zip");
        CreateDuiDialogWindow( mpsAllControlAttrSetting->m_sWindowAttr.mhParentHwnd,mpsAllControlAttrSetting->m_sWindowAttr.mstrSkinFolder.c_str(),mpsAllControlAttrSetting->m_sWindowAttr.mstrSkinFile.c_str(),mpsAllControlAttrSetting->m_sWindowAttr.dwStyle,mpsAllControlAttrSetting->m_sWindowAttr.dwExStyle,mpsAllControlAttrSetting->m_sWindowAttr.mstrWindowName.c_str() );
#else
        
        CreateDuiDialogWindow(mpsAllControlAttrSetting->m_sWindowAttr.mhParentHwnd,mpsAllControlAttrSetting->m_sWindowAttr.mstrSkinFolder.c_str(),mpsAllControlAttrSetting->m_sWindowAttr.mstrSkinFile.c_str(),mpsAllControlAttrSetting->m_sWindowAttr.dwStyle,mpsAllControlAttrSetting->m_sWindowAttr.dwExStyle,mpsAllControlAttrSetting->m_sWindowAttr.mstrWindowName.c_str() );
#endif
        //! SetFocus( m_hParentHwnd );
        CenterWindow();

        return ShowModal();
    }

    LRESULT CCJGWInputViewResolver::ResponseDefaultKeyEvent(WPARAM wParam)
    {
        if (wParam == VK_RETURN)
        {
            CloseInputWindow();
            return TRUE;
        }
        else if (wParam == VK_ESCAPE)
        {
            Close(IDCANCEL);
            return TRUE;
        }

        return FALSE;
    }

    bool CCJGWInputViewResolver::OnEventCustomMessage(TEventUI* pTEventUI,LPARAM lParam,WPARAM wParam)
    {
        if (WM_TIMER == pTEventUI->Type && 1 == pTEventUI->wParam)
        {
            KillTimer(GetHWND(),1);
            SettingControlAttrToWindow();
        }
        return true;
    }
    //! inputOKLen
    void CCJGWInputViewResolver::SettingControlAttrToWindow()
    {
        for ( std::vector<S_CONTROL_ATTR_SETTING>::iterator it = mpsAllControlAttrSetting->m_vControlAttrSetting.begin();
            it != mpsAllControlAttrSetting->m_vControlAttrSetting.end();
            ++ it)
        {
            CControlUI* pControlUi = m_PaintManager.FindControl( it[0].name.c_str() );
            if (!pControlUi)
            {
                //! check name is GlobalEnvironment
                if (NULL != JGW_WStrComparenoCaseWStr(it[0].name.c_str(),L"GlobalEnvironment")) continue;
                for ( std::vector<S_ATTR_NAME_VALUE>::iterator itvec = it[0].mvsAttrNameValue.begin();
                    itvec != it[0].mvsAttrNameValue.end();
                    ++itvec )
                {
                    if(NULL == JGW_WStrComparenoCaseWStr(itvec->pstrName.c_str(),L"minchar"))
                    {
                        mnMinChar = _ttoi(itvec->pstrValue.c_str());
                    }
                    else if (NULL == JGW_WStrComparenoCaseWStr(itvec->pstrName.c_str(),L"maxchar"))
                    {
                        mnMaxChar = _ttoi(itvec->pstrValue.c_str());
                    }
                    else if (NULL == JGW_WStrComparenoCaseWStr(itvec->pstrName.c_str(),L"inputprefix"))
                    {
                        mstrInputPrefix = itvec->pstrValue;
                    }
                    else if (NULL == JGW_WStrComparenoCaseWStr(itvec->pstrName.c_str(),L"numbertype"))
                    {
                        if (!itvec->pstrValue.empty()) mnNumberType = _ttoi(itvec->pstrValue.c_str());
                    }
                    else if (NULL == JGW_WStrComparenoCaseWStr(itvec->pstrName.c_str(),L"StringUpperLowCaseType"))
                    {
                        meStringUpperLowCaseType = (E_STRING_UPPERLOWCASE_TYPE)_ttoi(itvec->pstrValue.c_str());
                    }
                }
                continue;
            }

            for ( std::vector<S_ATTR_NAME_VALUE>::iterator itvec = it[0].mvsAttrNameValue.begin();
                itvec != it[0].mvsAttrNameValue.end();
                ++itvec )
            {
                if( NULL == itvec->pstrName.compare( _T("focus") ) )
                    pControlUi->SetFocus();
                else if ( NULL == itvec->pstrName.compare(_T("SetSelAll")) )
                {
                    ((CEditUI*)pControlUi)->SetSelAll();
                    ((CEditUI*)pControlUi)->SetSel(0,((CEditUI*)pControlUi)->GetText().GetLength());
                }
                else
                    pControlUi->SetAttribute(itvec->pstrName.c_str(),itvec->pstrValue.c_str());
            }
        }
    }
}