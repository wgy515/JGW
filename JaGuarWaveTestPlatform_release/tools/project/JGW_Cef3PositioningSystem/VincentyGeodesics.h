#pragma once
#include <string>
#include "JGW_Cef3PositioningSystem.h"
namespace JGW
{
	typedef struct
	{
		double sinU1;
		double cosU1;
		double sigma;
		double sinSigma;
		double cosSigma;
		double cosAlpha1;
		double cosSqAlpha;
		double cos2sigmaM;
		double sinAlpha;
		double sinAlpha1;
		double lambda1;
	} DirParams;

	typedef struct
	{
		double upper_A;
		double sigma;
		double sinSqSigma;
		double deltaSigma;
		double sinU1;
		double cosU1;
		double sinU2;
		double cosU2;
		double cosLambda;
		double sinLambda;
		/// <summary>
		/// distance
		/// </summary>
		double s;
	} InvParams;



	class VincentyGeodesics
	{
	public:
		VincentyGeodesics(void);
		~VincentyGeodesics(void);
	public:
		/**
		* Calculates geodesic latitude (in degrees) based on one point, bearing (in degrees) and distance (in m) using Vincenty's direct formula for ellipsoids
		* 计算B点纬度，根据A点的纬度、经度、方位角、距离
		* @param lat latitude point one
		* @param lon longitude point one
		* @param azimuth bearing (in degrees)=>point one
		* @param distance distance
		* @return latitude point two
		*/
		static double VincentyDirLat(double lat, double lon, double azimuth, double distance);
		/**
		* 计算B点经度，根据A点的纬度、经度、方位角、距离
		* @param lat latitude point one
		* @param lon longitude point one
		* @param azimuth bearing (in degrees)=>point one
		* @param distance distance
		* @return longitude point two
		*/
		static double VincentyDirLon(double lat, double lon, double azimuth, double distance);

		/**
		* Calculates geodesic reverse azimuth (in degrees) based on one point, bearing (in degrees) and distance (in m) using Vincenty's direct formula for ellipsoids.
		* 计算反向方位角，根据纬度、经度、方位角、距离，
		* @param lat latitude point one
		* @param lon longitude point one
		* @param azimuth bearing (in degrees)=>point one
		* @param distance distance
		* @return bearing (in degrees)=>point two
		*/
		static double VincentyDirRevAzimuth(double lat, double lon, double azimuth, double distance);
		/**
		* Calculates geodesic reverse azimuth (in degrees) based on one point, bearing (in degrees) and distance (in m) using Vincenty's direct formula for ellipsoids.
		* 计算反向方位角，根据纬度、经度、方位角、距离，是否反向(否 : 反向方位角+180)
		* @param lat latitude point one
		* @param lon longitude point one
		* @param azimuth bearing (in degrees)=>point one
		* @param distance distance
		* @param returnAzimuth bearing (in degrees)=>point two
		* @return bearing (in degrees)=>point two
		*/
		static double VincentyDirRevAzimuth(double lat, double lon, double azimuth, double distance, bool returnAzimuth);
		/**
		* Calculates geodesic distance (in m) between two points specified by latitude/longitude (in numeric degrees) using Vincenty's inverse formula for ellipsoids.
		* 计算AB两点之间距离，A点纬度、经度。B点纬度、经度
		* @param lat1 latitude point one
		* @param lon1 longitude point one
		* @param lat2 latitude point two
		* @param lon2 longitude point two
		* @return distance
		*/
		static double VincentyInvDistance(double lat1, double lon1, double lat2, double lon2);
		/**
		* 计算第一点到第二点的方位角，
		* Calculates geodesic forward azimuth in degrees clockwise from north between two points specified by latitude/longitude using Vincenty's inverse formula for ellipsoids.
		* 计算A点到B点的方位角，A点纬度、经度，B点纬度、经度
		* @param lat1 latitude point one
		* @param lon1 longitude point one
		* @param lat2 latitude point two
		* @param lon2 longitude point two
		* @return bearing (in degrees)=>point one
		*/
		static double VincentyInvFwdAzimuth(double lat1, double lon1, double lat2, double lon2);
		/**
		* 计算A点到B点的方位角，InvParams
		* @param invParams
		* @return
		*/
		static double VincentyInvFwdAzimuth(InvParams invParams);
		/**
		* 计算A点到B点的反向方位角，InvParams
		* @param invParams
		* @return
		*/
		static double VincentyInvRevAzimuth(InvParams p);

		/**
		* Calculates geodesic reverse azimuth (in degrees) between two points specified by latitude/longitude (in numeric degrees) using Vincenty's inverse formula for ellipsoids.
		* 计算第二点到第一点的方位角
		* 计算B点到A点的方位角，A点纬度、经度，B点纬度、经度
		* @param lat1 latitude point one
		* @param lon1 longitude point one
		* @param lat2 latitude point two
		* @param lon2 longitude point two
		* @return
		* @throws Exception
		*/
		static double VincentyInvRevAzimuth(double lat1, double lon1, double lat2, double lon2);
		/**
		* Calculates geodesic reverse azimuth (in degrees) between two points specified by latitude/longitude (in numeric degrees) using Vincenty's inverse formula for ellipsoids.
		* 计算B点到A点的方位角，A点纬度、经度，B点纬度、经度、返回方位角
		* @param lat1 latitude point one
		* @param lon1 longitude point one
		* @param lat2 latitude point two
		* @param lon2 longitude point two
		* @param returnAzimuth For return azimuth (2->1) use returnAzimuth = true.
		* @return bearing (in degrees)=>point true
		*/
		static double VincentyInvRevAzimuth(double lat1, double lon1, double lat2, double lon2, bool returnAzimuth);
		/**
		* get DirParams A点纬度、经度、方位角、距离
		* @param lat latitude point one
		* @param lon longitude point one
		* @param azimuth bearing (in degrees)=>point one
		* @param distance distance
		* @return DirParams
		*/
		static DirParams VincentyDir(double lat, double lon, double azimuth, double distance);
		/**
		* get InvParams A点纬度、经度、B点纬度、经度
		* @param lat1 latitude point one
		* @param lon1 longitude point one
		* @param lat2 latitude point two
		* @param lon2 longitude point two
		* @return InvParams
		*/
		static InvParams VincentyInv(double lat1, double lon1, double lat2, double lon2);

		/**
		* Converts decimal latitude, longitude or azimuth value to degrees/minutes/seconds string format.
		* 十进制角度转换成字符串、是否经度
		* @param decimalDeg decimal latitude, longitude or azimuth value
		* @param isLongitude is longitude,1 : is longitude 0 : is latitude,-1 : azimuth
		* @return
		*/
		static std::wstring ConvertDegrees(double decimalDeg, int isLongitude);
		/**
		* Converts latitude, longitude or azimuth in degrees/minutes/seconds format to decimal value.
		* 将时分秒角度转成为十进制角度
		* @param degreeDeg 37°57′03.72030″S
		* @return
		*/
		static double ConvertDecimal(std::wstring degreeDeg);
		/**
		*
		* @param val
		* @return
		*/
		static int Sign(double val);
		/// <summary>
		/// http://en.wikipedia.org/wiki/Modulo_operation
		/// </summary>
		/// <param name="dividend"></param>
		/// <param name="divisor"></param>
		/// <param name="sameSignAsDivisor"></param>
		/// <returns></returns>
		static double ModDouble(double dividend, double divisor, bool sameSignAsDivisor);
		/// <summary>
		/// Normalizes latitude to -90..+90 range.
		/// </summary>
		/// <param name="lat">latitude</param>
		/// <returns></returns>
		static double NormalizeLat(double lat);

		/// <summary>
		/// Normalizes longitude to -180..+180 range
		/// </summary>
		/// <param name="lon">longitude</param>
		/// <returns></returns>
		static double NormalizeLon(double lon);
		/// <summary>
		/// Normalizes azimuth to 0..360 range. Note: by default input and return values have the same sign. To obtain only positive values pass positiveOnly = true
		/// </summary>
		/// <param name="azimuth">azimuth</param>
		/// <param name="positiveOnly">To obtain only positive values pass positiveOnly = true</param>
		/// <returns></returns>
		static double NormalizeAzimuth(double azimuth, bool positiveOnly);
		/// <summary>
		/// source: http://en.wikibooks.org/wiki/Programming:Visual_Basic_Classic/Simple_Arithmetic#Trigonometrical_Functions
		/// note: x & y are in reverse order to match JavaScript Math.atan2() params order
		/// </summary>
		/// <param name="y"></param>
		/// <param name="x"></param>
		/// <returns></returns>
		static double Atan2(double y, double x);

		/**
		* 计算偏航角俯仰角以及距离
		* @param latitude1 第一点纬度
		* @param longitude1 第一点经度
		* @param latitude2 第二点纬度
		* @param longitude2 第二点经度
		* @param altitude1 第一点海拔高度
		* @param altitude2 第二点海拔高度
		*/
		static void CalcVincentyAzimuthPitch(double latitude1,double longitude1,double latitude2,double longitude2,double altitude1,double altitude2);
		//! 是否是有效GPS
		static bool IsValidGPS(double lat,double lng);
	public:
		/**
		* 计算两点方向偏航角 第二点到第一点偏航角
		*/
		static double calcRevAzimuth;
		/**
		* 计算两点的俯仰角===根据海拔高度差
		*/
		static double calcPitch;
		/**
		* 计算两点的俯仰角===根据海拔高度差
		*/
		static double calcRevPitch;
		/**
		* 计算两点之间距离
		*/
		static double calcDistance;
		/**
		* 计算两点的偏航角,第一点到第二点偏航角
		*/
		static double calcAzimuth;

		/**
		* 初始化偏航角俯仰角以及距离参数
		*/
		static void InitVincentyAzimuthPitch();
	};
}
