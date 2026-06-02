#include "StdAfx.h"
#include "CWriteNumberThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JaGuarWaveTestPlatform/CJGW_InputViewResolver_Define.h>

namespace JGW
{
    CCWriteNumberThread::CCWriteNumberThread(void) : mcDownloadPDBImage(mcSerialComPort)
    {
    }


    CCWriteNumberThread::~CCWriteNumberThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCWriteNumberThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCWriteNumberThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCWriteNumberThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCWriteNumberThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsDownloadPDBParam = (LPS_DOWNLOAD_PDB_PARAM)lParam;
        mbInitLoadPDBImage = mcDownloadPDBImage.LoadPDBImageFile(mpsDownloadPDBParam->mstrFolder);
    }

    void CCWriteNumberThread::OnRunThread( WPARAM wParam,LPARAM lParam )
    {
        SendListMsgToWnd(L"Start Test@---@W");
        if (!mbInitLoadPDBImage)
        {
            SendListMsgToWnd(L"Load PDB Image@FAIL@F");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return;
        }

        if (!OpenPerasoComPortView()
            || !InitDongleView()
            || !GetSerialNumberView()
            || !GetMacAddressView()
            || !WriteSerialNumberAndMacAddressView()
            || !ResetDongleView()
            )
        {
            mcDownloadPDBImage.CloseDongleComPort();
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return ;
        }
        mcDownloadPDBImage.CloseDongleComPort();
        mpAsyncWndMessage->PutAsyncMessage(WM_TEST_PASS_MSG);
        SendListMsgToWnd(L"Test Finished@OK@P");
    }

    void CCWriteNumberThread::SendListMsgToWnd(const wchar_t* strMsg,bool bInsert /* = true */)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(bInsert?1:0));
    }

    bool CCWriteNumberThread::OpenPerasoComPortView()
    {
        std::wstring strComPort;
        SendListMsgToWnd(L"Find Device@---@W",true);

        for (int i = 0;i < 50;i ++)
        {
            if (0 == (i % 3))
            {
                SendListMsgToWnd(L"Find Device@.@W",false);
            }
            else if (1 == (i % 3))
            {
                SendListMsgToWnd(L"Find Device@..@W",false);
            }
            else
            {
                SendListMsgToWnd(L"Find Device@...@W",false);
            }
            strComPort = JGW_GetComPortToUsbServiceName(L"usbser",L"VID_2932");
            if (strComPort.empty()) { Sleep(1000); continue; }
            SendListMsgToWnd(L"Opening  Port@....@W");
            if (mcDownloadPDBImage.OpenDongleComPort(strComPort))
            {
                SendListMsgToWnd(L"Opening  Port@OK@P",false);
                return true;
            }
            SendListMsgToWnd(L"Opening  Port@FAIL@F",false);
            Sleep(500);
        }
        return false;
    }

    bool CCWriteNumberThread::InitDongleView()
    {
        SendListMsgToWnd(L"Init Device@---@W",true);
        if (mcDownloadPDBImage.InitPDBInfoToDongle())
        {
            SendListMsgToWnd(L"Init Device@OK@P",false);
            return true;
        }
        SendListMsgToWnd(L"Init Device@FAIL@F",false);

        return false;
    }

    bool CCWriteNumberThread::GetInputNumber(size_t nMaxChar,const wchar_t* strTipLabel,const wchar_t* strInputTip)
    {
        if ( !IsWindow(mpsDownloadPDBParam->mhMainWnd) ) return false;

        S_ALL_CONTROL_ATTR_SETTING				m_sAllControlAttrSetting;
        std::wstring strTemp;
        std::vector<LPS_CONTROL_ATTR_SETTING>	m_vGetControlAttrSetting;
        m_sAllControlAttrSetting.m_sWindowAttr.mhParentHwnd = mpsDownloadPDBParam->mhMainWnd;
        m_sAllControlAttrSetting.m_sWindowAttr.mstrSkinFolder = _T("skin\\JaGuarWave");
        m_sAllControlAttrSetting.m_sWindowAttr.mstrSkinFile = _T("CommInputUiSkin.xml");
        m_sAllControlAttrSetting.m_sWindowAttr.dwStyle = UI_WNDSTYLE_DIALOG;
        m_sAllControlAttrSetting.m_sWindowAttr.dwExStyle = WS_EX_STATICEDGE | WS_EX_APPWINDOW ;
        m_sAllControlAttrSetting.m_sWindowAttr.mstrWindowName = _T("CCTF_Input_Common_Window_Duilib");

        S_ATTR_NAME_VALUE sAttrNameValue;
        S_CONTROL_ATTR_SETTING sControlAttrSetting,sControlAttrSetting1,sControlAttrSetting2;
        S_CONTROL_ATTR_SETTING sGetControlAttrSetting;
        {
            sControlAttrSetting.name = _T("inputEdit");

            sAttrNameValue.pstrName = _T("focus");
            sAttrNameValue.pstrValue = _T("");
            sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

            sAttrNameValue.pstrName = _T("SetSelAll");
            sAttrNameValue.pstrValue = _T("");
            sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

            //
            sAttrNameValue.pstrName = _T("maxchar");
            JGW_FormatWString(sAttrNameValue.pstrValue,L"%d",nMaxChar);
            sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
        }

        {
            sControlAttrSetting1.mvsAttrNameValue.clear();
            sControlAttrSetting1.name = _T("tipLabel");
            sAttrNameValue.pstrName = _T("text");
            sAttrNameValue.pstrValue = strTipLabel;
            sControlAttrSetting1.mvsAttrNameValue.push_back(sAttrNameValue);
        }
        {
            sControlAttrSetting2.mvsAttrNameValue.clear();
            sControlAttrSetting2.name = _T("inputTip");
            sAttrNameValue.pstrName = _T("text");
            sAttrNameValue.pstrValue = strInputTip;
            sControlAttrSetting2.mvsAttrNameValue.push_back(sAttrNameValue);
        }
        m_sAllControlAttrSetting.m_vControlAttrSetting.push_back( &sControlAttrSetting );
        m_sAllControlAttrSetting.m_vControlAttrSetting.push_back( &sControlAttrSetting1 );
        m_sAllControlAttrSetting.m_vControlAttrSetting.push_back( &sControlAttrSetting2 );
        //! »ñÈ¡¿ØÖÆ×Ö·û´®
        {
            sGetControlAttrSetting.name = _T("inputEdit");
            sAttrNameValue.pstrName = _T("text");
            sAttrNameValue.pstrValue = _T("");
            sGetControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
        }
        m_vGetControlAttrSetting.push_back( &sGetControlAttrSetting );

        if ( IDOK == SendMessage(mpsDownloadPDBParam->mhMainWnd,WM_HWND_INPUT_COMMON_WINDOW_MSG,WPARAM(&m_sAllControlAttrSetting),LPARAM(&m_vGetControlAttrSetting)) && !sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue.empty() )
        {
            mstrInput = sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue;
            return true;
        }
        return false;
    }

    bool CCWriteNumberThread::ResetDongleView()
    {
        mcDownloadPDBImage.ResetDongleDevice();
        SendListMsgToWnd(L"Reset Devices@OK@P");
        return true;
    }

    bool CCWriteNumberThread::GetSerialNumberView()
    {
        std::wstring strTemp;
        SendListMsgToWnd(L"Read SerialNumber@---@W");
        if (!mcDownloadPDBImage.ReadSNMacToDongleFlash(mstrSerialNumber,mstrMacAddress))
        {
            SendListMsgToWnd(L"Read SerialNumber@FAIL@F",false);
            return false;
        }
        mwstrSerialNumber = JGW_A2W(mstrSerialNumber);
        JGW_FormatWString(strTemp,L"Read SerialNumber@%s@P",mwstrSerialNumber.c_str());
        SendListMsgToWnd(strTemp.c_str(),false);

        return true;
    }

    bool CCWriteNumberThread::GetMacAddressView()
    {
        std::wstring strTemp;
        SendListMsgToWnd(L"Input MacAddress@---@W");
        if (!GetInputNumber(17,L"MacAddress",L"Please input 12/17-bit MAC Address Number")
            || !(12 == mstrInput.length() || 17 == mstrInput.length()))
        {
            SendListMsgToWnd(L"Input MacAddress@Input Length Error@F",false);
            return false;
        }
        if (17 == mstrInput.length()) JGW_ReplaceStringW(mstrInput,L":",L"");
        if (!ValidationMacAddressEffectiveness())
        {
            SendListMsgToWnd(L"Input MacAddress@Input MAC Address Number Unlawfulness@F",false);
            return false;
        }
        mstrMacAddress = JGW_W2A(mstrInput);
        JGW_FormatWString(strTemp,L"Input MacAddress@%s@P",mstrInput.c_str());
        SendListMsgToWnd(strTemp.c_str(),false);

        return true;
    }

    bool CCWriteNumberThread::WriteSerialNumberAndMacAddressView()
    {
        SendListMsgToWnd(L"Write Number@---@W");
        if (!mcDownloadPDBImage.DownloadSNMacToDongleFlash(mstrSerialNumber.c_str(),mstrMacAddress.c_str()))
        {
            SendListMsgToWnd(L"Write Number@FAIL@F",false);
            return false;
        }
        SendListMsgToWnd(L"Write Number@OK@P",false);

        return true;
    }

    bool CCWriteNumberThread::ValidationMacAddressEffectiveness()
    {
        if (12 != mstrInput.length()) return false;
        for (int i = 0;i < 12;i ++)
        {
            wchar_t c = mstrInput.at(i);
            if (c >= '0' && c <= '9') continue;
            if (c >= 'A' && c <= 'F') continue;  
            if (c >= 'a' && c <= 'f') continue;
            return false;
        }
        return true;
    }
}
