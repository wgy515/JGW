#pragma once
#include <JGW_WindowsFuncPlugin/CMessageThread.h>
#include <JGW_QMSLFuncPlugin/JGW_wr_nv_api.h>
#include <JGW_WindowsFuncPlugin/CJGW_CPeekNamePide.h>
#include <JGW_DevicePool/DevicePortDefine.h>

namespace JGW
{
	class CSwitchModeThread :
		public CCMessageThread
	{
	public:
		CSwitchModeThread(void);
		~CSwitchModeThread(void);

		void			OnDialogSwitchEdl(WPARAM wParam,LPARAM lParam);
		bool			CheckPidVidIsDialog();
		bool			GetDialogComPort(int& nPort);
		bool			ConnectServer(int nPort);
		bool			SwitchEdl();
		/**切入到FastBoot模式**/
		void			OnFactoryModeMsg(WPARAM wParam,LPARAM lParam);
		bool			CheckUsbVidIsAdbMsg();
		bool			SwitchFastboot(/*CString strSerialNumber*/);
		bool			CheckAdbPort(/*CString &strSerialNumber*/);
		/**切入到9006**/
		void			OnFastbootModeMsg(WPARAM wParam,LPARAM lParam);
		bool			SwitchDownLoadMode();
		bool			CheckDeviceState();
		bool			GetFastbootDeviceState();
		DECLARE_MYTHREAD_MESSAGE_MAP();
	private:
		CCJGW_PeekNamePide		m_cPeekNamePide;
		const wchar_t*			m_strRegUsbPidVid;
		//STRUCT_DEVICE_MSG_INFO *m_lpDeviceMsgInfo;
		wchar_t					m_szSerialNumber[260];
		size_t					m_nSerialNumberSize;
		std::wstring			m_strRead;

		jgw_wr_nv_api			m_cwrnv;
	};
}


