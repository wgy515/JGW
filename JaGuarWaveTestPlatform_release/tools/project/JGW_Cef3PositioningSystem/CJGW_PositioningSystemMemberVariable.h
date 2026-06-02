#pragma once
#include "JGW_Cef3PositioningSystem.h"

namespace JGW
{
	typedef struct  
	{
		// 纬度
		double mdLatitude;
		// 经度
		double mdLongitude;
	} S_LNGLAT;

	typedef struct  
	{
		// 计算的偏航角（磁偏航角）
		float mfCalYaw;
		// 本机偏航角（磁偏航角）
		float mfYaw;
		// 本机真航向角
		float mfTrueYaw;
	}S_YAW;

	//positionFixIndicator ： GPS质量 === 0=未定位，1=非差分定位，2=差分定位，3=PPS模式
	// SatellitesUsed 卫星数量
	// HDOP 水平精度因子，表示位置精度，值越小精度越高
	// 海拔高度，单位为米
	// GeoidalSeparation 大地水准面高度，单位为米 大地水准位间距 地球椭球面相对于海平面的高度
	//! detailed information
	typedef struct
	{
		// GPS质量 === 0=未定位，1=非差分定位，2=差分定位，3=PPS模式
		int mnPositionFixIndicator;
		// 卫星数量
		int mnSatellitesUsed;
		// HDOP 水平精度因子，表示位置精度，值越小精度越高
		float mfHdop;
		// 大地水准面高度，单位为米 大地水准位间距 地球椭球面相对于海平面的高度
		float mfGeoidalSeparation;
		//! 海拔高度M
		std::string mstrMslAltitude;
	} S_GPS_DES;

	class CCJGW_PositioningSystemProtocol;
	class CCJGW_PositioningSystemMemberVariable
	{
	private:
		CCJGW_PositioningSystemMemberVariable(void);
		~CCJGW_PositioningSystemMemberVariable(void);
	public:
		static CCJGW_PositioningSystemMemberVariable& GetPSMemberVariable();
		//! 是否是有效偏航角
		static bool IsValidYaw(float fYaw);
	public: // setting
		void UpdateLocalGpsLngLat(double dLongitude,double dLatitude);
		void UpdateRemoteGpsLngLat(double dLongitude,double dLatitude);
		void UpdateLocalYaw(float fLocalYaw);
		void UpdateRemoteYaw(float fRemoteYaw);
		void UpdateGpsDes(int nPositionFixIndicator,int nSatellitesUse,float fHdop,float fGeoidalSeparation,const std::string& strMslAltitude);
		void ManualUpdateLocalLatLng(double dLongitude,double dLatitude);
		void ManualUpdateRemoteLatLng(double dLongitude,double dLatitude);
		void ManualUpdateLocalYaw(float fLocalYaw);
		void CancelManualUpdateLocalYaw();
		void CancelManualUpdateLocalLatLng();
		void CancelManualUpdateRemoteLatLng();
	public://CalcVincentyAzimuthPitch
		void CalculateVincentyAzimuthPitchAndMagneticDec();
		//! 计算磁偏角
		static float CalcMagneticDec(float fLongitude,float fLatitude);
		friend CCJGW_PositioningSystemProtocol;
	public:	// get
		float GetLocalMagneticDec();
		float GetRemoteMagneticDec();
		float GetDistance();
		void GetLocalGpsLngLat(S_LNGLAT& sLocalGpsLngLat);
		void GetRemoteGpsLngLat(S_LNGLAT& sRemoteGpsLngLat);
		void GetLocalYaws(S_YAW& sLocalYaws);
		void GetRemoteYaws(S_YAW& sRemoteYaws);
		void GetLocalGpsDes(S_GPS_DES& sLocalGpsDes);
	private:
		//! 是否手动更新偏航角
		bool mbIsManualUpdateLocalYaw;
		//！是否手动更新经纬度
		bool mbIsManualUpdateLocalLatLng;
		//! 是否手动更新远程经纬度
		bool mbIsManualUpdateRemoteLatLng;
		//! 本机磁偏角
		float mfLocalMagneticDec;
		//! 远程磁偏角
		float mfRemoteMagneticDec;
		//! 距离
		float mfDistance;
		//! 本机经纬度
		S_LNGLAT msLocalGpsLngLat;
		//! 远程样机经纬度
		S_LNGLAT msRemoteGpsLngLat;
		//! 本机偏航角信息
		S_YAW msLocalYaws;
		//! 远程偏航角信息
		S_YAW msRemoteYaws;
		//! 本机GPS描述信息
		S_GPS_DES msLocalGpsDes;
	};
}

