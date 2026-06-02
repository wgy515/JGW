#pragma once
#include "CJGW_Cef3PositioningSystemUDPScokect.h"

namespace JGW
{
	class CCJGW_PositioningSystemMemberVariable;
	class CCJGW_Cef3BrowserEventHelp;

	class CCJGW_PositioningSystemProtocol : public Poco::Runnable
	{
	public:
		CCJGW_PositioningSystemProtocol(CCJGW_Cef3BrowserEventHelp* pCef3BrowserEventHelp,CCJGW_Cef3PositioningSystemUDPScokect* pCef3PositioningSystemUDPScokect);
		~CCJGW_PositioningSystemProtocol(void);
	public:
		// 接收定位系统协议消息
		static void OnRecvPositioningSystemProtocolMsg(const std::string& strMessage);
		//! 接收RABBITMQ消息
		static void OnRecvRabbitMQMsg(const std::string& strMessage);
	public:
		//! 开启定位系统协议线程
		bool StartPositioningSystemProtocolThread(void);
		//! 停止定位系统协议线程
		void StopPositioningSystemProtocolThread(void);
		//! 获取错误消息
		std::wstring GetErrorMessage();
	private:
		void OnHandlePositioningSystemProtocolMsg(int nID,const std::string& strMessage);
		// 处理DXCS ID消息
		void OnHandleDxcsOperationMsg(const std::string& strMessage);
	public: //! 注册函数给HTML调用
		// 重新校准地磁
		static const wchar_t* OnClickRecalibrationMagnetic(const wchar_t* szMessage);
		// 设置指南针角度
		static const wchar_t* OnSettingCompassAngle(const wchar_t* szMessage);
		// 手动更新经纬度
		static const wchar_t* OnManualUpdateLatLng(const wchar_t* szMessage);
		// 取消手动更新指南针角度
		static const wchar_t* OnCancelManualUpdateCompassAngle(const wchar_t* szMessage);
		// 重新连接DXCS
		static const wchar_t* OnReConnectDxcs(const wchar_t* szMessage);
		// 登录RabbitMQ
		static const wchar_t* OnLoginRabbitMQ(const wchar_t* szMessage);
		// 连接BLE设备
		static const wchar_t* OnConnectBleDevice(const wchar_t* szMessage);
	private:
		void run();
		/// Does the work.
	private: // 发送指令到BLE UWP,获取命令
		//! 获取UWP扫描BLE命令
		static std::string GetStartScanUWPCommand();
		//! 获取UWP停止扫描BLE命令
		static std::string GetStopScanUWPCommand();
		//! 获取UWP连接BLE命令
		static std::string GetConnectBleUWPCommand(const std::string& strBleName,const std::string& strBleId);
		// 获取UWP断开连接BLE命令
		static std::string GetDisConnectBleUWPCommand();
		//! 读取经纬度
		static std::string GetLongitudeLatitudeUWPCommand();
		//! 重新校准地磁
		static std::string GetReCalibrationMagneticUWPCommand();
		//! 磁偏角
		//static std::string& GetMagneticDecUWPCommand();
		//! MFX
		static std::string GetMfxUWPCommand();
		//! 获取GNSS原始经纬度信息
		static std::string GetGNSSRawInfoUWPCommand();
	private: // RabbitMQ 消息格式化
		// 发送经纬度信息
		static std::string GetLongitudeLatitudeRabbitMQCommand();
		// 发送高度信息 暂时不使用
		// 发送欧拉角
		static std::string GetEulerAngleRabbitMQCommand();
	private:
		bool mbConnectBLE;
		bool mbIsStartThread;
		//int mnPositioningSystemID;
		Poco::Net::DatagramSocket* _socket;
		CCJGW_Cef3PositioningSystemUDPScokect* mpCef3PositioningSystemUDPScokect;
		CCJGW_Cef3BrowserEventHelp* mpCef3BrowserEventHelp;
		Poco::Thread _thread;
		std::string mstrBleId;
		std::wstring mstrErrorMessage;
	};

}

