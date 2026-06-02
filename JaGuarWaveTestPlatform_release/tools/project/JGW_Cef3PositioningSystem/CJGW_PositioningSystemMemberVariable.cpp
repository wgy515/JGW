#include "StdAfx.h"
#include "CJGW_PositioningSystemMemberVariable.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <time.h>
#include "WMM_2025/wmm.h"
//#include "VincentyGeodesics.h"
#include "CJGW_PositioningSystemProtocol.h"
#include "latlon_ellipsoidal_vincenty.h"
namespace JGW
{
	CCJGW_PositioningSystemMemberVariable::CCJGW_PositioningSystemMemberVariable(void) : mbIsManualUpdateLocalLatLng(false),mbIsManualUpdateLocalYaw(false),mbIsManualUpdateRemoteLatLng(false)
	{ 
		mfLocalMagneticDec = INVALID_YAW_F;
		mfRemoteMagneticDec = INVALID_YAW_F;

		mfDistance = INVALID_DISTANCE;
		//! 本机经纬度
		msLocalGpsLngLat.mdLatitude = INVALID_LATITUDE;
		msLocalGpsLngLat.mdLongitude = INVALID_LONGITUDE;
		//! 远程样机经纬度
		msRemoteGpsLngLat.mdLatitude = INVALID_LATITUDE;
		msRemoteGpsLngLat.mdLongitude = INVALID_LONGITUDE;
		//! 本机偏航角信息
		msLocalYaws.mfCalYaw = INVALID_YAW_F;
		msLocalYaws.mfYaw = INVALID_YAW_F;
		msLocalYaws.mfTrueYaw = INVALID_YAW_F;
		//! 远程偏航角信息
		msRemoteYaws.mfCalYaw = INVALID_YAW_F;
		msRemoteYaws.mfYaw = INVALID_YAW_F;
		msRemoteYaws.mfTrueYaw = INVALID_YAW_F;
		//!
		msLocalGpsDes.mfGeoidalSeparation = 0.00f;
		msLocalGpsDes.mfHdop = 0.00f;
		msLocalGpsDes.mnPositionFixIndicator = 0;
		msLocalGpsDes.mnSatellitesUsed = 0;
	}


	CCJGW_PositioningSystemMemberVariable::~CCJGW_PositioningSystemMemberVariable(void)
	{
	}

	CCJGW_PositioningSystemMemberVariable& CCJGW_PositioningSystemMemberVariable::GetPSMemberVariable()
	{
		static CCJGW_PositioningSystemMemberVariable gPositioningSystemMemberVariable;
		return gPositioningSystemMemberVariable;
	}

	bool CCJGW_PositioningSystemMemberVariable::IsValidYaw(float fYaw)
	{
		if (fYaw < -360.0f || fYaw > 720.0f)
		{
			return false;
		}
		return true;
	}

	void CCJGW_PositioningSystemMemberVariable::UpdateGpsDes(int nPositionFixIndicator,int nSatellitesUse,float fHdop,float fGeoidalSeparation,const std::string& strMslAltitude)
	{
		if (!mbIsManualUpdateLocalLatLng)
		{
			msLocalGpsDes.mfGeoidalSeparation = fGeoidalSeparation;
			msLocalGpsDes.mfHdop = fHdop;
			msLocalGpsDes.mnPositionFixIndicator = nPositionFixIndicator;
			msLocalGpsDes.mnSatellitesUsed = nSatellitesUse;
			msLocalGpsDes.mstrMslAltitude = strMslAltitude;
		}
	}

	void CCJGW_PositioningSystemMemberVariable::UpdateLocalGpsLngLat(double dLongitude,double dLatitude)
	{
		if (!mbIsManualUpdateLocalLatLng)
		{
			msLocalGpsLngLat.mdLatitude = dLatitude;
			msLocalGpsLngLat.mdLongitude = dLongitude;
		}
	}

	void CCJGW_PositioningSystemMemberVariable::UpdateLocalYaw(float fLocalYaw)
	{
		if (!mbIsManualUpdateLocalYaw)
		{
			msLocalYaws.mfYaw = fLocalYaw;
			if (IsValidYaw(msLocalYaws.mfYaw)/*INVALID_YAW != msLocalYaws.mfYaw*/)
			{
				msLocalYaws.mfTrueYaw = msLocalYaws.mfYaw + mfLocalMagneticDec;
				if (msLocalYaws.mfTrueYaw > 360.00) msLocalYaws.mfTrueYaw -= 360.0;
				else if (msLocalYaws.mfTrueYaw < 0.00) msLocalYaws.mfTrueYaw += 360.0;
			}
		}
	}

	void CCJGW_PositioningSystemMemberVariable::UpdateRemoteGpsLngLat(double dLongitude,double dLatitude)
	{
		if (!mbIsManualUpdateRemoteLatLng)
		{
			msRemoteGpsLngLat.mdLatitude = dLatitude;
			msRemoteGpsLngLat.mdLongitude = dLongitude;
		}	
	}

	void CCJGW_PositioningSystemMemberVariable::UpdateRemoteYaw(float fRemoteYaw)
	{
		msRemoteYaws.mfYaw = fRemoteYaw;
		if (IsValidYaw(msRemoteYaws.mfYaw)/*INVALID_YAW != msRemoteYaws.mfYaw*/)
		{
			msRemoteYaws.mfTrueYaw = msRemoteYaws.mfYaw + mfRemoteMagneticDec;
			if (msRemoteYaws.mfTrueYaw > 360.00) msRemoteYaws.mfTrueYaw -= 360.0;
			else if (msRemoteYaws.mfTrueYaw < 0.00) msRemoteYaws.mfTrueYaw += 360.0;
		}
	}

	void CCJGW_PositioningSystemMemberVariable::ManualUpdateLocalLatLng(double dLongitude,double dLatitude)
	{
		mbIsManualUpdateLocalLatLng = true;
		msLocalGpsLngLat.mdLongitude = dLongitude;
		msLocalGpsLngLat.mdLatitude = dLatitude;
		msLocalGpsDes.mfGeoidalSeparation = 0.00f;
		msLocalGpsDes.mfHdop = 0.00f;
		msLocalGpsDes.mnPositionFixIndicator = 0;
		msLocalGpsDes.mnSatellitesUsed = 0;
	}

	void CCJGW_PositioningSystemMemberVariable::ManualUpdateLocalYaw(float fLocalYaw)
	{
		mbIsManualUpdateLocalYaw = true;
		msLocalYaws.mfYaw = fLocalYaw;
	}

	void CCJGW_PositioningSystemMemberVariable::ManualUpdateRemoteLatLng(double dLongitude,double dLatitude)
	{
		mbIsManualUpdateRemoteLatLng = true;
		msRemoteGpsLngLat.mdLatitude = dLatitude;
		msRemoteGpsLngLat.mdLongitude = dLongitude;
		//msLocalGpsDes.mfGeoidalSeparation = 0.00f;
		//msLocalGpsDes.mfHdop = 0.00f;
		//msLocalGpsDes.mnPositionFixIndicator = 0;
		//msLocalGpsDes.mnSatellitesUsed = 0;
	}

	void CCJGW_PositioningSystemMemberVariable::CancelManualUpdateLocalLatLng()
	{
		mbIsManualUpdateLocalLatLng = false;
		msRemoteGpsLngLat.mdLatitude = INVALID_LATITUDE;
		msRemoteGpsLngLat.mdLongitude = INVALID_LONGITUDE;
	}

	void CCJGW_PositioningSystemMemberVariable::CancelManualUpdateLocalYaw()
	{
		mbIsManualUpdateLocalYaw = false;
		msLocalYaws.mfYaw = INVALID_YAW_F;
	}

	void CCJGW_PositioningSystemMemberVariable::CancelManualUpdateRemoteLatLng()
	{
		mbIsManualUpdateRemoteLatLng = false;
		msRemoteGpsLngLat.mdLatitude = INVALID_LATITUDE;
		msRemoteGpsLngLat.mdLongitude = INVALID_LONGITUDE;
	}

	float CCJGW_PositioningSystemMemberVariable::GetDistance()
	{
		return mfDistance;
	}

	float CCJGW_PositioningSystemMemberVariable::GetLocalMagneticDec()
	{
		return mfLocalMagneticDec;
	}

	float CCJGW_PositioningSystemMemberVariable::GetRemoteMagneticDec()
	{
		return mfRemoteMagneticDec;
	}

	void CCJGW_PositioningSystemMemberVariable::GetLocalGpsLngLat(S_LNGLAT& sLocalGpsLngLat)
	{
		sLocalGpsLngLat.mdLatitude = msLocalGpsLngLat.mdLatitude;
		sLocalGpsLngLat.mdLongitude = msLocalGpsLngLat.mdLongitude;
	}

	void CCJGW_PositioningSystemMemberVariable::GetRemoteGpsLngLat(S_LNGLAT& sRemoteGpsLngLat)
	{
		sRemoteGpsLngLat.mdLatitude = msRemoteGpsLngLat.mdLatitude;
		sRemoteGpsLngLat.mdLongitude = msRemoteGpsLngLat.mdLongitude;
	}

	void CCJGW_PositioningSystemMemberVariable::GetLocalYaws(S_YAW& sLocalYaws)
	{
		sLocalYaws.mfCalYaw = msLocalYaws.mfCalYaw;
		sLocalYaws.mfTrueYaw = msLocalYaws.mfTrueYaw;
		sLocalYaws.mfYaw = msLocalYaws.mfYaw;
	}

	void CCJGW_PositioningSystemMemberVariable::GetRemoteYaws(S_YAW& sRemoteYaws)
	{
		sRemoteYaws.mfCalYaw = msRemoteYaws.mfCalYaw;
		sRemoteYaws.mfTrueYaw = msRemoteYaws.mfTrueYaw;
		sRemoteYaws.mfYaw = msRemoteYaws.mfYaw;
	}

	void CCJGW_PositioningSystemMemberVariable::GetLocalGpsDes(S_GPS_DES& sLocalGpsDes)
	{
		sLocalGpsDes.mfGeoidalSeparation = msLocalGpsDes.mfGeoidalSeparation;
		sLocalGpsDes.mfHdop = msLocalGpsDes.mfHdop;
		sLocalGpsDes.mnPositionFixIndicator = msLocalGpsDes.mnPositionFixIndicator;
		sLocalGpsDes.mnSatellitesUsed = msLocalGpsDes.mnSatellitesUsed;
	}

	float CCJGW_PositioningSystemMemberVariable::CalcMagneticDec(float fLongitude,float fLatitude)
	{
		time_t timeCur;
		struct tm tmTmp ;
		std::wstring strStamp;

		time(&timeCur);
		localtime_s(&tmTmp,&timeCur);

		float fTimerYears = wmm_get_date(tmTmp.tm_year - 100,tmTmp.tm_mon,tmTmp.tm_mday);
		float fMagneticDec = 0.00f;
		E0000(fLatitude,fLongitude,fTimerYears,&fMagneticDec);

		return fMagneticDec;
	}

	void CCJGW_PositioningSystemMemberVariable::CalculateVincentyAzimuthPitchAndMagneticDec()
	{
		float fLocalMagneticDec = INVALID_YAW_F,fRemoteMagneticDec = INVALID_YAW_F;
		S_LNGLAT sLocalLngLat = {INVALID_LATITUDE,INVALID_LONGITUDE};
		S_LNGLAT sRemoteLngLat = {INVALID_LATITUDE,INVALID_LONGITUDE};
		GetLocalGpsLngLat(sLocalLngLat);
		GetRemoteGpsLngLat(sRemoteLngLat);

		bool bIsValidLocalGps = 1 == IsValidLatitudeLongitude(sLocalLngLat.mdLatitude,sLocalLngLat.mdLongitude);
		bool bIsValidRemoteGps = 1 == IsValidLatitudeLongitude(sRemoteLngLat.mdLatitude,sRemoteLngLat.mdLongitude);

		//! 计算本机磁偏角
		if (bIsValidLocalGps)
		{
			fLocalMagneticDec = CalcMagneticDec(static_cast<float>(sLocalLngLat.mdLongitude),static_cast<float>(sLocalLngLat.mdLatitude));
			//Log4WI_F(L"distance : %lf,initialBearing : %lf,finalBearing : %lf,iterations : %d",inverseInfo3.distance,inverseInfo3.initialBearing,inverseInfo3.finalBearing,inverseInfo3.iterations);
		}
		//! 计算远程主机磁偏角
		if (bIsValidRemoteGps)
		{
			fRemoteMagneticDec = CalcMagneticDec(static_cast<float>(sRemoteLngLat.mdLongitude),static_cast<float>(sRemoteLngLat.mdLatitude));
			//Log4WI_F(L"Longitude : %s,Latitude : %s,finalBearing : %lf,iterations : %d",inverseInfo3.distance,inverseInfo3.initialBearing,inverseInfo3.finalBearing,inverseInfo3.iterations);
		}

		//VincentyGeodesics::InitVincentyAzimuthPitch();
		//! 计算距离、偏航角、俯仰角
		if (bIsValidLocalGps && bIsValidRemoteGps)
		{
			//VincentyGeodesics::CalcVincentyAzimuthPitch(sLocalLngLat.mdLatitude,sLocalLngLat.mdLongitude,sRemoteLngLat.mdLatitude,sRemoteLngLat.mdLongitude,15,10);
			VincentyInverseInfos inverseInfo3 = VincentyInverse(sLocalLngLat.mdLatitude,sLocalLngLat.mdLongitude,sRemoteLngLat.mdLatitude,sRemoteLngLat.mdLongitude);
			Log4WI_F(L"distance : %lf,initialBearing : %lf,finalBearing : %lf,iterations : %d",inverseInfo3.distance,inverseInfo3.initialBearing,inverseInfo3.finalBearing,inverseInfo3.iterations);

			//! 填充结构体
			mfLocalMagneticDec = fLocalMagneticDec;
			mfRemoteMagneticDec = fRemoteMagneticDec;
			mfDistance = static_cast<float>(inverseInfo3.distance);
			// 磁偏角 declination
			// magnetic heading 磁航向角
			// magnetic heading + declination = true heading
			// 真航向计算公式是：真航向（TH） = 磁航向（MH） ± 磁偏角（VAR）。
			// 磁航向(MC) = 真航向(TC) - 磁差(Var)。
			// 真航向角的计算公式：真航向角=磁航向角+磁偏角或真航向角=磁航向角-磁偏角（取决于磁偏角的方向）。
			// (True heading + variation = magnetic heading.)
			// 罗航向+东罗经差=真航向
			// 罗航向-西罗经差=真航向
			// 罗经差=磁差+自差
			// 其中磁差为磁北与真北之间相差的角度，自差为罗北与磁北两者相差的角度。
			// （自差的符号和大小与船上钢铁磁化的性质和大小有关，而船磁又与航向和地磁方向的相对位置有关，即船磁的大小和方向是随着航向的不同而变化的，因此自差也是随着航向改变而变化的。除此之外，自差还可能因船舶所在的地磁纬度不同、船舶倾斜和船上铁器位置的变化而变化。 [3]）
			// true heading
			// https://blog.csdn.net/wujingmintop/article/details/132078528
			// https://blog.csdn.net/luckpl/article/details/50964233
			// https://www.magnetic-declination.com/#
			// https://www.ngdc.noaa.gov/geomag/calculators/magcalc.shtml#declination
			/*
			磁偏角补偿方法：
			对目标方位做磁补偿是向西减磁偏角或向东加磁偏角
			例1：磁偏角向西偏23度，罗盘指针指向323度。
			真正的方位角=磁方位角（323度）-磁偏角（23度）=300度
			例2：磁偏角向东偏22度，罗盘指针指向278度
			真正的方位角=磁方位角（278度）+磁偏角（22度）=300度
			FT808专业手持户外罗盘设有专门的磁偏角补偿设定，你只要根据当地磁偏角数值输入，其他计算由FT808来完成，用以满足专业人事对于高精度方向导航的要求；同时他的防水，防尘，防震设计也能满专业户外及野外生存的苛刻环境。
			328 = 331-23

			航向角描述运动载体（如飞机、车辆）纵轴与参考北向的夹角，根据基准不同分为三类：

			‌真航向角（True Heading, TH）‌
			以地理真北（经线北端）为基准，计算公式：
			‌TH = MH + δ‌
			其中 MH 为磁航向角，δ 为磁偏角（东偏取正，西偏取负）‌。
			真北方向通过天文观测或高精度GNSS定位确定，不受磁场干扰‌。

			‌磁航向角（Magnetic Heading, MH）‌
			以磁北（地磁北极方向）为基准，由磁罗盘直接测得‌28。
			与真航向的转换关系：
			‌MH = TH - δ‌（或 MH = TH + |δ|，需根据磁偏角方向调整符号）‌。

			‌罗航向角（Compass Heading, RH）‌
			受载体磁场干扰的磁罗盘读数，需修正罗差（Deviation, d）：
			‌RH = MH + d‌
			罗差 d 由载体铁磁物质引起，需定期校准‌。
			*/
#if 0
			double trueHeading = VincentyGeodesics::calcAzimuth;
			// (True heading + variation = magnetic heading.)
			double magneticHeading = trueHeading - mfLocalMagneticDec;
			if (magneticHeading > 360.00) magneticHeading -= 360.0;
			else if (magneticHeading < 0.00) magneticHeading += 360.0;
			//! 计算的磁航向角
			msLocalYaws.mfCalYaw = static_cast<float>(magneticHeading);	
#else
			msLocalYaws.mfCalYaw = getMagneticHeading(inverseInfo3.initialBearing,mfLocalMagneticDec);
#endif
#if 0

			trueHeading = VincentyGeodesics::calcRevAzimuth;
			// (True heading + variation = magnetic heading.)
			magneticHeading = trueHeading - mfRemoteMagneticDec;
			if (magneticHeading > 360.00) magneticHeading -= 360.0;
			else if (magneticHeading < 0.00) magneticHeading += 360.0;
			//! 计算的磁航向角
			msRemoteYaws.mfCalYaw = static_cast<float>(magneticHeading);	
#else
			msRemoteYaws.mfCalYaw = getMagneticHeading(getInitialBearingToFinalBearing(inverseInfo3.finalBearing),mfRemoteMagneticDec);
#endif

			//! 331.188567, 327.908567
			// 将本机磁航向角（地磁显示的数据）转换为本机真航向角
			if (IsValidYaw(msLocalYaws.mfYaw)/*INVALID_YAW != msLocalYaws.mfYaw*/)
			{
#if 0
				msLocalYaws.mfTrueYaw = msLocalYaws.mfYaw + mfLocalMagneticDec;
				if (msLocalYaws.mfTrueYaw > 360.00) msLocalYaws.mfTrueYaw -= 360.0;
				else if (msLocalYaws.mfTrueYaw < 0.00) msLocalYaws.mfTrueYaw += 360.0;
#else
				msLocalYaws.mfTrueYaw = getInitialBearingToMagneticHeading(msLocalYaws.mfYaw,mfLocalMagneticDec);
#endif
			}

			if (IsValidYaw(msRemoteYaws.mfYaw)/*INVALID_YAW != msRemoteYaws.mfYaw*/)
			{
#if 0
				msRemoteYaws.mfTrueYaw = msRemoteYaws.mfYaw + mfRemoteMagneticDec;
				if (msRemoteYaws.mfTrueYaw > 360.00) msRemoteYaws.mfTrueYaw -= 360.0;
				else if (msRemoteYaws.mfTrueYaw < 0.00) msRemoteYaws.mfTrueYaw += 360.0;
#else
				msRemoteYaws.mfTrueYaw = getInitialBearingToMagneticHeading(msRemoteYaws.mfYaw,mfRemoteMagneticDec);
#endif
			}
		}

	}
}
