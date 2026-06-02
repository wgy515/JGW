#pragma once
#include <JGW_DevicePool/DevicePortDefine.h>
#include <JGW_QMSLFuncPlugin/JGW_wr_nv_api.h>
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>
#include <JGW_QMSLFuncPlugin\JGW_QMSLFuncPlugin_Define.h>

namespace JGW
{
    class JGWQMSLFUNCPLUGIN_API CCJGW_UsbModeHandover
    {
    public:
        CCJGW_UsbModeHandover(void);
        ~CCJGW_UsbModeHandover(void);
    public:
        //! 高通USB切换到EDL
        bool QcommUsbModeHandoverEdl(SDeviceMsg_Info* pDeviceMsgInfo);
    private:
        //! 高通DIAG端口切换9008下载模式端口
        bool QcommDialogSwitchEdl();
        //! ADB 模式端口切换到9008下载模式端口
        bool AdbSwitchEdl();
    private: //! diag
        //! bool CheckUsbMode(const wchar_t* tag,const wchar_t* strFlag);
        bool ConnectQcommDialog();
    private:    //! adb
        bool InitCmdPide();
		std::wstring GetADBSeralNumber();
    private:
        SDeviceMsg_Info* mpDeviceMsgInfo;
        jgw_wr_nv_api m_cwrnv;
        CCJGW_PeekNamePide m_cPeekNamePide;
    };
}