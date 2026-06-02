#pragma once
#include <JGW_MSVCFunc/CMessageThread.h>
#include <vector>
#include "DevicePortDefine.h"
#include "RegDevicePorts.h"
#define AUTO_ID_USB_EXPORTS
#include "Auto_ID_Usb.h"
#include "DevicePoolImpl.h"
//RegisterDeviceNotification
namespace JGW
{
	class CCDevicePool :
		public CCMessageThread,public JGW_DevicePoolImpl
	{
	public:
		bool StartDevicePool(HWND hWnd);

		bool RegDevicePoolMsg(CRegMsgType* lpRegMsgType);

		bool RegDevicePoolMsg(UINT nMsgID,HWND hwnd,GUID guid,E_DEVICE_MSG guidType,DeviceModeEnum eDeviceModeEnum);

		bool SendDeviceMsgToDevicePool(SDeviceMsg_Info* lpDeviceMsg);

		unsigned int GetDevicePoolThreadID();

		bool IsDevicePoolThreadRun();

		void CloseDevicePoolThread();
	public:
		~CCDevicePool(void);
		//! 设置响应窗口句柄
		void	SetResponceHwnd(HWND hWnd);
		//! 响应设备消息 wParam guid 消息类型拔出还是拔入 lParam dbcc_name
		void	OnAddDeviceMsg(WPARAM wParam,LPARAM lParam);
		//! 注册响应窗体消息  消息接收后像窗体发送线程消息
		//! wParam 窗体句柄 lParam 响应消息类型   允许重复注册
		void	OnRegisterHwnd(WPARAM wParam,LPARAM lParam);
		//! 自动移除注册窗体消息 wParam 需要移除窗口句柄 lParam 参数未定义为NULL即可
		void	OnRemoveHwnd(WPARAM wParam,LPARAM lParam);
		//! 获取下载模式USB索引
		int		GetDownModeUsbSortIndex(const wchar_t* strInstanceID);
		//! 获取FASTBOOT模式索引
		int		GetFastbootModeUsbSortIndex(const wchar_t* strInstanceID,std::wstring& strSeralNumber);
		//! 获取LAN排序索引
		int		GetLanModeUsbSortIndex(const wchar_t* strInstanceID);
		//! 获取工厂模式排序索引
		int		GetFactoryModeSortIndex(const wchar_t* strInstanceID);
		//! 响应清除排序的索引
		void	OnClearSortIndex(WPARAM wParam,LPARAM lParam);

		DECLARE_MYTHREAD_MESSAGE_MAP();

		static CCDevicePool* GetDevicePoolInstance()
		{
			static CCDevicePool dev;
			return &dev;
		}
	private:
		CCDevicePool(void);
		void	DispatchDeviceMsg(SDeviceMsg_Info* lpdevice);
		void	RunThread();
		void	ClearDeviceMsg();
		void	ClearRegMsgType();
		bool	RegisterNotification(GUID guid);
		//static CCDevicePool* m_lpDevicePool;
		std::vector<S_REG_PIDVID>		m_vRegDownModeUsbPidVid;//! 已经注册排序的PID VID 集合
		std::vector<S_REG_PIDVID>		m_vRegLanUsbPidVid;//! 网卡模式排序集合
		std::vector<S_REG_PIDVID>		m_vRegFastbootModeUsbPidVid;//! fastboot PID VID排序集合
		std::vector<S_REG_PIDVID>		m_vRegFactoryModeUsbPidVid;//! 工厂模式PID VID排序集合
	private:
		std::vector<std::shared_ptr<CRegMsgType>> m_vRegHwnd;
		std::vector<SDeviceMsg_Info*> m_vSDeviceMsg;
		std::vector<GUID>			m_vRegGuid;
		HWND						m_hWnd;
		CRegDevicePorts				m_regPort;
	};
}


