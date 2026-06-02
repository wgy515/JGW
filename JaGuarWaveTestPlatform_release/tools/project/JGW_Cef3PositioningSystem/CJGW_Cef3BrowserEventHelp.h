#pragma once
#include <JGW_Cef3Plugin/JGW_Cef3PluginExport.h>
namespace JGW
{
	class CCJGW_Cef3BrowserEventHelp : public CCJGW_Cef3BrowserEventDelegate
	{
	public:
		CCJGW_Cef3BrowserEventHelp(void);
		~CCJGW_Cef3BrowserEventHelp(void);
	public:
		int GetBrowserID();
	public: //! Execute Jave Script
		// 设置本机经纬度信息
		// fLatitude 纬度
		// fLongitude 经度
		// fDistance 本机和远程GPS距离
		void SetLocalGpsInfoToMap(double fLatitude,double fLongitude,float fDistance);
		// 设置远程经纬度信息
		// fLatitude 纬度
		// fLongitude 经度
		void SetRemoteGpsInfoToMap(double fLatitude,double fLongitude);
		//! 设置本机偏航角信息 cal_yaw, yaw, true_yaw
		// fCalYaw 计算的偏航角（磁偏航角）
		// fYaw 本机偏航角（磁偏航角）
		// fTrueYaw 本机真航向角
		void SetLocalYawInfoToMap(float fCalYaw,float fYaw,float fTrueYaw);
		//! 设置远程偏航角信息 cal_yaw, yaw, true_yaw
		// fCalYaw 计算的偏航角（磁偏航角）
		// fYaw 本机偏航角（磁偏航角）
		// fTrueYaw 本机真航向角
		void SetRemoteYawInfoToMap(float fCalYaw,float fYaw,float fTrueYaw);
		// 设置GPS详细信息 set_local_gps_des_param
		// positionFixIndicator ： GPS质量 === 0=未定位，1=非差分定位，2=差分定位，3=PPS模式
		// SatellitesUsed 卫星数量
		// HDOP 水平精度因子，表示位置精度，值越小精度越高
		// 海拔高度，单位为米
		// GeoidalSeparation 大地水准面高度，单位为米 大地水准位间距 地球椭球面相对于海平面的高度
		void SetLocalGpsDesToMap(int nPositionFixIndicator, int nSatellitesUsed,float fHdop, const std::string& strMslAltitude, float fGeoidalSeparation);
		// 添加连接消息
		void AddConnectMessageToMap(const std::string& strMessage);
		//! 关闭连接MODAL
		void CloseConnectModalToMap();
		// 重新连接DXCS
		void OnReConnectDXCSToMap();
		//! 更新高德地图显示
		void UpdateAmapWebView();
		//! addBLEDeviceInfo 添加BLE设备信息
		void AddBLEDeviceInfoToMap(const std::string& strBleName,const std::string& strBleID);
		//! showEcompassNotCalibratePrompt 显示地磁未校准提示
		void ShowEcompassNotCalibratePromptToMap();
		//! dismissEcompassNotCalibratePrompt 取消显示地磁未校准提示
		void DismissEcompassNotCalibratePromptToMap();
		// disable false 取消禁用重新扫描按钮
		void DisableFalseReScanBleBtnToMap();
	protected:
		 void OnBrowserCreate(int nBrowserID,const std::wstring& strUrl) ;
	private:
		int mnBrowserID;
		bool mbIsShowEcompassNotCalibratePrompt;
	};
}

