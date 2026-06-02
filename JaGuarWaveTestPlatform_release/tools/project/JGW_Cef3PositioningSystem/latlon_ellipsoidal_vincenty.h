/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
/* Vincenty Direct and Inverse Solution of Geodesics on the Ellipsoid (c) Chris Veness 2002-2022  */
/*                                                                                   MIT Licence  */
/* www.ngs.noaa.gov/PUBS_LIB/inverse.pdf                                                          */
/* www.movable-type.co.uk/scripts/latlong-vincenty.html                                           */
/* www.movable-type.co.uk/scripts/geodesy-library.html#latlon-ellipsoidal-vincenty                */
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -  */
/**
 * Distances & bearings between points, and destination points given start points & initial bearings,
 * calculated on an ellipsoidal earth model using ‘direct and inverse solutions of geodesics on the
 * ellipsoid’ devised by Thaddeus Vincenty.
 *
 * From: T Vincenty, "Direct and Inverse Solutions of Geodesics on the Ellipsoid with application of
 * nested equations", Survey Review, vol XXIII no 176, 1975. www.ngs.noaa.gov/PUBS_LIB/inverse.pdf.
 *
 * example: 获取两点的磁方位角
 * 
 * VincentyInverseInfos inverseInfo3 = VincentyInverse(22.580899,113.941030,22.980769,113.840966);
 * wmm_init();
 * float wmmTimeDate = wmm_get_date(25,7,15);
 * float dec = 0.00f;
 * E0000(22.580899,113.941030,wmmTimeDate,&dec);
 * double dfMagneticHeading = getMagneticHeading(inverseInfo3.initialBearing,dec);
 * 
 * 
 * 
 * @module latlon-ellipsoidal-vincenty
 */
#ifndef LATLON_ELIPSOIDAL_VINCENTY_H__
#define LATLON_ELIPSOIDAL_VINCENTY_H__
#include <stdint.h>
/**
 * Pi. This is the ratio of the circumference of a circle to its diameter.
 */
#define PI 3.141592653589793
/**
 * The value of Number.EPSILON is the difference between 1 and the smallest value greater than 1 that is representable as a Number value,
 * which is approximately: 2.2204460492503130808472633361816 x 10‍−‍16. 
 */
#define EPSILON 2.220446049250313e-16
/**
 * 无效距离
 */
#define INVALID_DISTANCE -999.0f
/**
 * 无效偏航角
 */
#define INVALID_YAW -999.0f
/**
 * 无效纬度信息
 */
#define INVALID_LATITUDE -99999.0f
/**
 * 无效经度信息
 */
#define INVALID_LONGITUDE -99999.0f
/**
 * 大气折射修正系数
 * 考虑地球曲率和大气折射修正（默认修正值0.15）
 */
//#define REFRACTION 0.15
#define REFRACTION 0.0

typedef struct
{
    double distance; // 两点之间距离
    double initialBearing; // ‌初始方位角 (α₁)‌ 表示从起点出发、沿大地线（最短路径）向终点前进时，在起点的前进方向。这是旅程开始时的方向
    double finalBearing; // ‌最终方位角 (α₂)‌表示当沿着同一条大地线从起点到达终点时，在终点的前进方向（即从终点看，指向起点的方向的反方向）
    double iterations; // 迭代次数
} VincentyInverseInfos;

/**
 * 经纬度结构体
 */
typedef struct 
{
    double longitude; // 经度
    double latitude; // 纬度
} LngLat;

typedef struct 
{
    LngLat point; // 经纬度
    double finalBearing; // ‌最终方位角 (α₂)‌表示当沿着同一条大地线从起点到达终点时，在终点的前进方向（即从终点看，指向起点的方向的反方向）
    double iterations; // 迭代次数
}VincentyDirectInfos;



typedef struct
{
	double a; // 赤道半径，长轴A
	double b; // 赤道半径，短轴B
	double f; // 椭圆体扁平率
}ellipsiod;

typedef struct
{
	char* name; // 坐标系名称
	ellipsiod e; // 坐标系对应的椭圆体信息
} ellipsiod_info;



/**
 * 是否是有效纬度（-90 ~ 90）
 * @param lat 
 * @return 0 ： 无效 1 : 有效
 * @example 
 *      IsValidLatitude(-180.0)
 */
uint8_t IsValidLatitude(double lat);
/**
 * 是否是有效经度（-180 ~ 180）
 * @param lat 
 * @return 0 ： 无效 1 : 有效
 * @example 
 *      IsValidLongitude(-180.0)
 */
uint8_t IsValidLongitude(double lon);
/**
 * 是否是有效经纬度（-180 ~ 180）
 * @param lat 
 * @param lon 
 * @return 0 ： 无效 1 : 有效
 * @example 
 *      IsValidLongitudeLatitude(-91,-180.0)
 */
uint8_t IsValidLatitudeLongitude(double lat,double lon);
/**
 * Returns the distance between ‘this’ point and destination point along a geodesic on the
 * surface of the ellipsoid, using Vincenty inverse solution.
 * 计算AB两点之间距离，A点纬度、经度。B点纬度、经度
 *
 * @param   {LatLon} point - Latitude/longitude of destination point.
 * @returns {number} Distance in metres between points or NaN if failed to converge.
 *
 * @example
 *   const p1 = new LatLon(50.06632, -5.71475);
 *   const p2 = new LatLon(58.64402, -3.07009);
 *   const d = p1.distanceTo(p2); // 969,954.166 m
 */
float VincentyDistance(double lat1, double lon1, double lat2, double lon2);
/**
* Vincenty inverse calculation.
*
* Ellipsoid parameters are taken from datum of 'this' point. Height is ignored.
*
* @private
* @param   {LatLon} point - Latitude/longitude of destination point.
* @returns {Object} Object including distance, initialBearing, finalBearing.
* @throws  {TypeError}  Invalid point.
* @throws  {RangeError} Points must be on surface of ellipsoid.
* @throws  {EvalError}  Formula failed to converge.
*/
VincentyInverseInfos VincentyInverse(double lat1, double lon1, double lat2, double lon2);
/**
 * Returns the initial bearing to travel along a geodesic from ‘this’ point to the given point,
 * using Vincenty inverse solution.
 * 使用Vincenty逆解，返回沿测地线从“this”点到给定点*的初始方位。(点1到点2的初始方向)
 *
 * @param   {LatLon} point - Latitude/longitude of destination point.
 * @returns {number} Initial bearing in degrees from north (0°..360°) or NaN if failed to converge.
 *
 * @example
 *   const p1 = new LatLon(50.06632, -5.71475);
 *   const p2 = new LatLon(58.64402, -3.07009);
 *   const b1 = p1.initialBearingTo(p2); // 9.1419°
 */
double VincentyInitialBearing(double lat1, double lon1, double lat2, double lon2);
/**
 * Returns the final bearing having travelled along a geodesic from ‘this’ point to the given
 * point, using Vincenty inverse solution.
 * 使用文森特逆解，返回从“this”点到给定*点沿测地线行进的最终方位。
 *
 * @param   {LatLon} point - Latitude/longitude of destination point.
 * @returns {number} Final bearing in degrees from north (0°..360°) or NaN if failed to converge.
 *
 * @example
 *   const p1 = new LatLon(50.06632, -5.71475);
 *   const p2 = new LatLon(58.64402, -3.07009);
 *   const b2 = p1.finalBearingTo(p2); // 11.2972°
 */
double VincentyFinalBearing(double lat1, double lon1, double lat2, double lon2);
/**
 * Returns the destination point having travelled the given distance along a geodesic given by
 * initial bearing from ‘this’ point, using Vincenty direct solution.
 * 使用文森特直接解，返回从“this”点出发，沿*初始方位给出的测地线行进给定距离的目的点。
 *
 * @param   {number} distance - Distance travelled along the geodesic in metres.
 * @param   {number} initialBearing - Initial bearing in degrees from north.
 * @returns {LatLon} Destination point.
 *
 * @example
 *   const p1 = new LatLon(-37.95103, 144.42487);
 *   const p2 = p1.destinationPoint(54972.271, 306.86816); // 37.6528°S, 143.9265°E
 */
LngLat VincentyDestinationPoint(double lat1, double lon1,double distance, double initialBearing);
/**
 * Vincenty direct calculation.
 *
 * Ellipsoid parameters are taken from datum of 'this' point. Height is ignored.
 *
 * @private
 * @param   {number} distance - Distance along bearing in metres.
 * @param   {number} initialBearing - Initial bearing in degrees from north.
 * @returns (Object} Object including point (destination point), finalBearing.
 * @throws  {RangeError} Point must be on surface of ellipsoid.
 * @throws  {EvalError}  Formula failed to converge.
 */
VincentyDirectInfos VincentyDirect(double lat1, double lon1,double distance,double initialBearing);
/**
 * Returns the final bearing having travelled along a geodesic given by initial bearing for a
 * given distance from ‘this’ point, using Vincenty direct solution.
 * TODO: arg order? (this is consistent with destinationPoint, but perhaps less intuitive)
 *
 * @param   {number} distance - Distance travelled along the geodesic in metres.
 * @param   {LatLon} initialBearing - Initial bearing in degrees from north.
 * @returns {number} Final bearing in degrees from north (0°..360°).
 *
 * @example
 *   const p1 = new LatLon(-37.95103, 144.42487);
 *   const b2 = p1.finalBearingOn(54972.271, 306.86816); // 307.1736°
 */
double VincentyFinalBearingOn(double lat1, double lon1,double distance, double initialBearing);

/**
 * Returns the point at given fraction between ‘this’ point and given point.
 * 返回‘ this ’点和给定点之间的给定分数点。
 *
 * @param   {LatLon} point - Latitude/longitude of destination point.
 * @param   {number} fraction - Fraction between the two points (0 = this point, 1 = specified point).
 * @returns {LatLon} Intermediate point between this point and destination point.
 *
 * @example
 *   const p1 = new LatLon(50.06632, -5.71475);
 *   const p2 = new LatLon(58.64402, -3.07009);
 *   const pInt = p1.intermediatePointTo(p2, 0.5); // 54.3639°N, 004.5304°W
 */
LngLat VincentyIntermediatePointTo(double lat1, double lon1, double lat2, double lon2,double fraction);
/// @brief 将（B->A最终方位）转化为B -> A初始方位
/// @param finalBearing 最终方位（B->A最终方位）
/// @return 返回B -> A 初始方位
double getInitialBearingToFinalBearing(double finalBearing);
/// @brief 获取磁航向角
/// @param initialBearing 初始方位角（真航向角）
/// @param magneticDec 磁偏角
/// @return 
double getMagneticHeading(double initialBearing,double magneticDec);
/// @brief 获取初始方位角 （真航向角）
/// @param magneticHeading 磁航向角
/// @param magneticDec 磁偏角
/// @return 
double getInitialBearingToMagneticHeading(double magneticHeading,double magneticDec);
/// @brief 计算俯仰角
/// @param distance AB两点之间距离
/// @param alt1 A点海拔高度
/// @param alt2 B点海拔高度
/// @return 
double calculateElevation(double distance,double alt1,double alt2);
/// @brief Parses representing degrees/minutes into numeric degrees.
/// @param dddmm_mmmmm 度分格式角度，如TAU1103经纬度格式dddmm.mmmmm
/// @return 返回十进制角度
double parseDm(double dddmm_mmmmm);
/// @brief 度数转弧度
/// @param degrees 度数
/// @return 
double toRadians(double degrees);
/// @brief 弧度转度数
/// @param radians 弧度
/// @return 
double toDegrees(double radians);
/**
 * Constrain degrees to range 0..360 (for bearings); e.g. -1 => 359, 361 => 1.
 * 将角度限制在0…360范围内(轴承);例如-1 => 359,361 => 1
 *
 * @private
 * @param {number} degrees
 * @returns degrees within range 0..360.
 */
double wrap360(double degrees);
/**
 * Constrain degrees to range -180..+180 (for longitude); e.g. -181 => 179, 181 => -179.
 * 将角度限制在-180…+180（经度）；例如：-181 => 179,181 => -179。
 * 
 * @private
 * @param {number} degrees
 * @returns degrees within range -180..+180.
 */
double wrap180(double degrees);
/**
 * Constrain degrees to range -90..+90 (for latitude); e.g. -91 => -89, 91 => 89.
 * 将度数限制在-90…+90（纬度）；例如-91 => -89,91 => 89。
 *
 * @private
 * @param {number} degrees
 * @returns degrees within range -90..+90.
 */
double wrap90(double degrees);

#endif //LATLON_ELIPSOIDAL_VINCENTY_H__
