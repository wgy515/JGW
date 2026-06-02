#include "StdAfx.h"
#include "CDownloadPDBThread.h"
#include "../../../platform_include/JGW_MSG_ID_Define.h"
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>

#include <JaGuarWaveTestPlatform/CJGW_InputViewResolver_Define.h>

namespace JGW
{
    CCDownloadPDBThread::CCDownloadPDBThread(void):mbInitLoadPDBImage(false),mcDownloadPDBImage(mcSerialComPort)
    {
    }


    CCDownloadPDBThread::~CCDownloadPDBThread(void)
    {
    }

    BEGIN_MYTHREAD_MESSAGE_MAP(CCDownloadPDBThread, CCMessageThread)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_INIT,&CCDownloadPDBThread::OnInitWnd)
        ON_MYTHREAD_MESSGAE(WM_TEST_THREAD_MSG_RUN,&CCDownloadPDBThread::OnRunThread)
    END_MYTHREAD_MESSAGE_MAP()

    void CCDownloadPDBThread::OnInitWnd(WPARAM wParam,LPARAM lParam)
    {
        mpAsyncWndMessage = (CCJGW_AsyncWndMessage*)wParam;
        mpsDownloadPDBParam = (LPS_DOWNLOAD_PDB_PARAM)lParam;
        /* std::string strFolder = (const char*)lParam;*/
        mbInitLoadPDBImage = mcDownloadPDBImage.LoadPDBImageFile(mpsDownloadPDBParam->mstrFolder);
    }

    void CCDownloadPDBThread::OnRunThread( WPARAM wParam,LPARAM lParam )
    {
        SendListMsgToWnd(L"Start Test@---@W");
        if (!mbInitLoadPDBImage)
        {
            SendListMsgToWnd(L"Load PDB Image@FAIL@F");
            mpAsyncWndMessage->PutAsyncMessage(WM_TEST_FAIL_MSG);
            return;
        }

        if (!InputSN()
			|| !OpenPerasoComPort()
            || !InitDongle()
            || !WriteSerialNumber() 
            || !BuildOTPBin()
            || !DownloadPDBImageFiles()
            || !ResetDongle()
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

    void CCDownloadPDBThread::SendListMsgToWnd(const wchar_t* strMsg,bool bInsert /* = true */)
    {
        mpAsyncWndMessage->PutAsyncStringMessage(WM_HWND_TEST_LIST_MSG,strMsg,LPARAM(bInsert?1:0));
    }

    bool CCDownloadPDBThread::OpenPerasoComPort()
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
            SendListMsgToWnd(L"Opening  Port@....@W",false);
            if (mcDownloadPDBImage.OpenDongleComPort(strComPort))
            {
                SendListMsgToWnd(L"Opening  Port@OK@P",false);
                return true;
            }
            SendListMsgToWnd(L"Opening  Port@FAIL@F",false);
            Sleep(200);
        }
        return false;
    }

    bool CCDownloadPDBThread::InitDongle()
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

	bool CCDownloadPDBThread::InputSN()
	{
		SendListMsgToWnd(L"Input Serial Number@---@W",false);

		int i = 0;
		for (;i < 2;i ++)
		{
			mwstrSerialNumber = L"";
			if (GetInputSerialNumber() && mwstrSerialNumber.length() == SERIALNUMBER_MAX_LEN)break;
		}
		if (2 == i)
		{
			SendListMsgToWnd(L"Input Serial Number@Input Length Error@F",false);
			return false;
		}
		std::wstring strTemp;
		JGW_FormatWString(strTemp,L"Input Serial Number@%s@P",mwstrSerialNumber.c_str());
		SendListMsgToWnd(strTemp.c_str(),false);
		return true;
	}

    bool CCDownloadPDBThread::WriteSerialNumber()
    {
        SendListMsgToWnd(L"Write Serial Number@---@W");
        
        std::string strSN(mwstrSerialNumber.begin(),mwstrSerialNumber.end());
        std::wstring strTemp;
        mcDownloadPDBImage.ChangeSNToImageFileBuf(strSN.c_str());
        JGW_FormatWString(strTemp,L"Write Serial Number@%s@P",mwstrSerialNumber.c_str());
        SendListMsgToWnd(strTemp.c_str(),false);

        return true;
    }

    bool CCDownloadPDBThread::BuildOTPBin()
    {
        SendListMsgToWnd(L"Build OTP Bin@---@W");
        if (mcDownloadPDBImage.BuildOTPCRC32())
        {
            SendListMsgToWnd(L"Build OTP Bin@OK@P",false);
            return true;
        }
        SendListMsgToWnd(L"Build OTP Bin@FAIL@F",false);
        return false;
    }

    bool CCDownloadPDBThread::DownloadPDBImageFiles()
    {
        SendListMsgToWnd(L"Download PDB Image@---@W");
        if (mcDownloadPDBImage.DonwloadAllPDBImageFileToDongleFlash())
        {
            SendListMsgToWnd(L"Download PDB Image@OK@P",false);
            return true;
        }
        SendListMsgToWnd(L"Download PDB Image@FAIL@F",false);
        return false;
    }

    bool CCDownloadPDBThread::ResetDongle()
    {
        mcDownloadPDBImage.ResetDongleDevice();
        SendListMsgToWnd(L"Reset Devices@OK@P");
        return true;
    }

    bool CCDownloadPDBThread::GetInputSerialNumber()
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
            JGW_FormatWString(sAttrNameValue.pstrValue,L"%d",SERIALNUMBER_MAX_LEN);
            sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);

//             sAttrNameValue.pstrName = _T("inputOKLen");
//             JGW_FormatWString(sAttrNameValue.pstrValue,L"%d",SERIALNUMBER_MAX_LEN);
//             sControlAttrSetting.mvsAttrNameValue.push_back(sAttrNameValue);
        }

        {
            sControlAttrSetting1.mvsAttrNameValue.clear();
            sControlAttrSetting1.name = _T("tipLabel");
            sAttrNameValue.pstrName = _T("text");
            sAttrNameValue.pstrValue = L"SN";
            sControlAttrSetting1.mvsAttrNameValue.push_back(sAttrNameValue);
        }
        {
            sControlAttrSetting2.mvsAttrNameValue.clear();
            sControlAttrSetting2.name = _T("inputTip");
            sAttrNameValue.pstrName = _T("text");
            JGW_FormatWString(sAttrNameValue.pstrValue,L"Please input %d-bit SN Number",SERIALNUMBER_MAX_LEN);
            //! sAttrNameValue.pstrValue = L"ÇëÊäÈë16Î»SNºÅ¶Î";
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
            //! mstrEnvironmentStr
            mwstrSerialNumber = sGetControlAttrSetting.mvsAttrNameValue[0].pstrValue;
            return true;
        }
        return false;
    }
}


