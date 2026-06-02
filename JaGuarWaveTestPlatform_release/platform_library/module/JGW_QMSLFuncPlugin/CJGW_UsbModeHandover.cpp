#include "StdAfx.h"
#include <JGW_QMSLFuncPlugin/CJGW_UsbModeHandover.h>
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_UsbRegedit.h>

namespace JGW
{
    const wchar_t g_szSerialNumberDevice[] = L"adb -s %s get-state\n";
    const wchar_t g_szGetSerialNumber[] = L"adb devices\n";
    const wchar_t g_szGetState[] = L"adb -s %s get-state\n";
    const wchar_t g_szADBDevices[] = L"adb -s %s devices\n";
    const wchar_t g_szRebootbooloader[] = L"adb -s %s reboot-bootloader\n";
    const wchar_t g_szFastbootReboot[] = L"fastboot -s %s reboot\n";
    const wchar_t g_szFastbootDevices[] = L"fastboot -s %s devices\n";
    const wchar_t g_szSelectOemInfo[] = L"fastboot -s %s oem device-info\n";
    const wchar_t g_szOemUnLock[] = L"fastboot -s %s oem unlock-fdl\n";
    const wchar_t g_szFastBootDownCmd[] = L"fastboot flash %s \"%s\" -s %s\n";
    const wchar_t g_szFasterasePartion[] = L"fastboot erase %s -s %s\n";
    const wchar_t g_szADBSwitchEDL[] = L"adb -s %s reboot edl\n";

    CCJGW_UsbModeHandover::CCJGW_UsbModeHandover(void) : mpDeviceMsgInfo(NULL)
    {
    }


    CCJGW_UsbModeHandover::~CCJGW_UsbModeHandover(void)
    {
    }

    void ClearDevice(SDeviceMsg_Info* pDeviceMsgInfo)
    {
        pDeviceMsgInfo->Dec();
    }

    bool CCJGW_UsbModeHandover::QcommUsbModeHandoverEdl(SDeviceMsg_Info* pDeviceMsgInfo)
    {
        if (!pDeviceMsgInfo) return false;
        mpDeviceMsgInfo = pDeviceMsgInfo;
        std::shared_ptr<SDeviceMsg_Info> ps(pDeviceMsgInfo,ClearDevice);

        if (E_DIAG_MODE == pDeviceMsgInfo->m_nDeviceMode)
        {
            //! return QcommDialogSwitchEdl();
            return AdbSwitchEdl();
        }
        else if (E_ADB_MODE == pDeviceMsgInfo->m_nDeviceMode)
        {
            return AdbSwitchEdl();
        }
        return false;
    }

    bool CCJGW_UsbModeHandover::QcommDialogSwitchEdl()
    {
        if (!ConnectQcommDialog())
        {
            Log4WD_F(L"ConnectQcommDialog Fail(%s)",mpDeviceMsgInfo->m_strRegUsbPidVid.c_str());
            return false;
        }

        return m_cwrnv.SwitchPhoneEDL();
    }

    bool CCJGW_UsbModeHandover::AdbSwitchEdl()
    {
        //! if (mpDeviceMsgInfo->m_strSeralNumber.empty()) return false;
        PDLOG4WW(L"CCJGW_UsbModeHandover::AdbSwitchEdl");
        if (!InitCmdPide())
        {
            Log4WD_F(L"AdbSwitchEdl -> InitCmdPide Fail(%s)",mpDeviceMsgInfo->m_strRegUsbPidVid.c_str());
            return false;
        }

		if (mpDeviceMsgInfo->m_strSeralNumber.length() <= 2)
		{
			for (int i = 0;i < 10;i ++)
			{
				mpDeviceMsgInfo->m_strSeralNumber = GetADBSeralNumber();
				if (!mpDeviceMsgInfo->m_strSeralNumber.empty()) break;
				Sleep(200);
			}
			if (mpDeviceMsgInfo->m_strSeralNumber.empty()) return false;
		}
        
        std::wstring strTemp;
        JGW_FormatWString(strTemp,g_szADBSwitchEDL,mpDeviceMsgInfo->m_strSeralNumber.c_str());
        PDLOG4WW(strTemp.c_str());
        return m_cPeekNamePide.WrPideFile(strTemp.c_str());
    }

    bool CCJGW_UsbModeHandover::ConnectQcommDialog()
    {
        int nComPort = 0;

        if (!JGW_GetComportToUsbPidVid(mpDeviceMsgInfo->m_strRegUsbPidVid.c_str(),nComPort)) return false;
        return m_cwrnv.ConnectDevice(nComPort);
    }

    bool CCJGW_UsbModeHandover::InitCmdPide()
    {
        if (m_cPeekNamePide.CheckProcessIsRun()) return true;
        std::wstring strTemp;
        return m_cPeekNamePide.InitPideCMD() && m_cPeekNamePide.ReadPideFile(strTemp);
    }

	std::wstring CCJGW_UsbModeHandover::GetADBSeralNumber()
	{
		std::wstring strRead,strTemp,strDownloadCmd;
		if (!m_cPeekNamePide.WRPideFile(g_szGetSerialNumber,wcslen(g_szGetSerialNumber),strRead,L"\tdevice")) return L"";
		PDLOG4WW(strRead.c_str());
		JGW_EraseFristToRemoveCharW(strRead,L'\n');
		JGW_EraseAfterToRemoveCharW(strRead,L'\n'); 
		JGW_EraseLastAndFristTwoCharsW(strRead);
		//! É¾³ýList of devices attached
		//! JGW_EraseFristToRemoveCharW(strRead,L'\n');
		PDLOG4WW(strRead.c_str());
		JGW_ReplaceStringW(strRead,L"\r",L"");
		JGW_ReplaceStringW(strRead,L"\n\n",L"");
		std::vector<std::wstring> vstrTemp;
		JGW_ParserStrW(strRead.c_str(),L"\n",vstrTemp);

		for (size_t i = 0;i < vstrTemp.size();i ++)
		{
			if (NULL == JGW_WStrCaseWStr(vstrTemp[i].c_str(),L"\tdevice")) continue;
			JGW_ReplaceStringW(vstrTemp[i],L"\tdevice",L"");
			JGW_EraseLastAndFristTwoCharsW(vstrTemp[i]);
			strTemp = vstrTemp[i];
            
            JGW_FormatWString(strDownloadCmd,g_szADBSwitchEDL,mpDeviceMsgInfo->m_strSeralNumber.c_str());
            m_cPeekNamePide.WrPideFile(strDownloadCmd.c_str());
			//! break;
		}
		/*size_t i = strRead.find_last_of(L"\tdevice");
		if (std::wstring::npos == i) return L"";
		size_t n = strRead.find_last_of(L'\n',i);
		if (n == std::wstring::npos) n = 0; 
		i -= wcslen(L"\tdevice");
		strTemp = strRead.substr(n,i - n + 1);*/

		return strTemp;
	}
}