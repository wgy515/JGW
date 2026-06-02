#include "stdafx.h"
#include "latlon_ellipsoidal_vincenty.h"
#include <stdlib.h>
#include <math.h>

/**
 * Ellipsoid parameters; exposed through static getter below.
 * 不同坐标系，默认使用WGS84坐标系
 */
const ellipsiod_info ellipsoids[] = {
		{"WGS84" ,		 { 6378137,      6356752.314245,  1/298.257223563 }},
		{"Airy1830",      {  6377563.396,  6356256.909,     1/299.3249646   }},
		{"AiryModified",  {  6377340.189,  6356034.448,     1/299.3249646   }},
		{"Bessel1841",    {  6377397.155,  6356078.962822,  1/299.15281285  }},
		{"Clarke1866",    {  6378206.4,    6356583.8,       1/294.978698214 }},
		{"Clarke1880IGN", {  6378249.2,    6356515.0,       1/293.466021294 }},
		{"GRS80",         {  6378137,      6356752.314140,  1/298.257222101 }},
		{"Intl1924",      {  6378388,      6356911.946128,  1/297           }}, // aka Hayford
		{"WGS72",         {  6378135,      6356750.52,      1/298.26        }}
};

/**
* WGS84 ellipsiod
* 6378137.0
* Equatorial radius: 赤道半径，长轴A
* Semi-major axis a
*/
static double low_a = ellipsoids[0].e.a;
/**
 * 赤道半径，短轴B
* Semi-minor axis b
*/
static double low_b = ellipsoids[0].e.b;
/**
* flattening 椭圆体扁平率
*/
static double ellipsiod_flattening = ellipsoids[0].e.f;



uint8_t IsValidLatitude(double lat)
{
    return (lat >= -90.0 && lat <= 90.0);
}

uint8_t IsValidLongitude(double lon)
{
    return (lon >= -180.0 && lon <= 180.0);
}

uint8_t IsValidLatitudeLongitude(double lat,double lon)
{
    return IsValidLatitude(lat) && IsValidLongitude(lon);
}

double toRadians(double degrees)
{
    return degrees * PI / 180;
}

double toDegrees(double radians)
{
    return radians * 180 / PI;
}

double wrap360(double degrees)
{
    if (0<=degrees && degrees<360) return degrees; // avoid rounding due to arithmetic ops if within range

    // bearing wrapping requires a sawtooth wave function with a vertical offset equal to the
    // amplitude and a corresponding phase shift; this changes the general sawtooth wave function from
    //     f(x) = (2ax/p - p/2) % p - a
    // to
    //     f(x) = (2ax/p) % p
    // where a = amplitude, p = period, % = modulo; however, JavaScript '%' is a remainder operator
    // not a modulo operator - for modulo, replace 'x%n' with '((x%n)+n)%n'
    double x = degrees, a = 180.0, p = 360.0;
    return fmod((fmod((2*a*x/p),p)+p),p);
}

double wrap180(double degrees)
{
    if (-180<=degrees && degrees<=180) return degrees; // avoid rounding due to arithmetic ops if within range

    // longitude wrapping requires a sawtooth wave function; a general sawtooth wave is
    //     f(x) = (2ax/p - p/2) % p - a
    // where a = amplitude, p = period, % = modulo; however, JavaScript '%' is a remainder operator
    // not a modulo operator - for modulo, replace 'x%n' with '((x%n)+n)%n'
    double x = degrees, a = 180, p = 360;
    return fmod((fmod((2*a*x/p - p/2),p)+p),p) - a;
}

double wrap90(double degrees)
{
    if (-90<=degrees && degrees<=90) return degrees; // avoid rounding due to arithmetic ops if within range

    // latitude wrapping requires a triangle wave function; a general triangle wave is
    //     f(x) = 4a/p ⋅ | (x-p/4)%p - p/2 | - a
    // where a = amplitude, p = period, % = modulo; however, JavaScript '%' is a remainder operator
    // not a modulo operator - for modulo, replace 'x%n' with '((x%n)+n)%n'
    double x = degrees, a = 90.0, p = 360.0;
    //return 4*a/p * fabs((((x-p/4)%p)+p)%p - p/2) - a;
    return 4*a/p * fabs(fmod((fmod((x-p/4),p)+p),p) - p/2) - a;
}


float VincentyDistance(double lat1, double lon1, double lat2, double lon2)
{
    VincentyInverseInfos inverseInfos = VincentyInverse(lat1,lon1,lat2,lon2);
    if (INVALID_YAW == inverseInfos.initialBearing)
    {
        return INVALID_DISTANCE;
    }
    return inverseInfos.distance;
}

VincentyInverseInfos VincentyInverse(double lat1, double lon1, double lat2, double lon2)
{
    VincentyInverseInfos inverseInfos = {INVALID_DISTANCE,INVALID_YAW,INVALID_YAW,0};
    if (!IsValidLatitudeLongitude(lat1,lon1) || ! IsValidLatitudeLongitude(lat2,lon2))
    {
        return inverseInfos;
    }

	double φ1 = toRadians(lat1);
	double φ2 = toRadians(lat2);
	double λ1 = toRadians(lon1);
	double λ2 = toRadians(lon2);
	// allow alternative ellipsoid to be specified
	double a = low_a;
	double b = low_b;
	double f = ellipsiod_flattening;

	double L = λ2 - λ1; // L = difference in longitude, U = reduced latitude, defined by tan U = (1-f)·tanφ.
	double tanU1 = (1-f) * tan(φ1), cosU1 = 1 / sqrt((1 + tanU1*tanU1)), sinU1 = tanU1 * cosU1;
	double tanU2 = (1-f) * tan(φ2), cosU2 = 1 / sqrt((1 + tanU2*tanU2)), sinU2 = tanU2 * cosU2;

	double π = PI;
	double ε = EPSILON;
	double antipodal = fabs(L) > π/2 || fabs(φ2-φ1) > π/2;

	double λ = L, sinλ = 0.00, cosλ = 0.00; // λ = difference in longitude on an auxiliary sphere
	double σ = antipodal ? π : 0, sinσ = 0, cosσ = antipodal ? -1 : 1, sinSqσ = 0.00; // σ = angular distance P₁ P₂ on the sphere
	double cos2σₘ = 1;                      // σₘ = angular distance on the sphere from the equator to the midpoint of the line
	double cosSqα = 1;                      // α = azimuth of the geodesic at the equator
	

	double λʹ = 0.00;
	uint16_t iterations = 0;
	do {
		sinλ = sin(λ);
		cosλ = cos(λ);
		sinSqσ = pow((cosU2*sinλ),2) + pow((cosU1*sinU2-sinU1*cosU2*cosλ),2);
		if (fabs(sinSqσ) < 1e-24) break;  // co-incident/antipodal points (σ < ≈0.006mm)
		sinσ = sqrt(sinSqσ);
		cosσ = sinU1*sinU2 + cosU1*cosU2*cosλ;
		σ = atan2(sinσ, cosσ);
		double sinα = cosU1 * cosU2 * sinλ / sinσ;
		cosSqα = 1 - sinα*sinα;
		cos2σₘ = (cosSqα != 0) ? (cosσ - 2*sinU1*sinU2/cosSqα) : 0; // on equatorial line cos²α = 0 (§6)
		double C = f/16*cosSqα*(4+f*(4-3*cosSqα));
		λʹ = λ;
		λ = L + (1-C) * f * sinα * (σ + C*sinσ*(cos2σₘ+C*cosσ*(-1+2*cos2σₘ*cos2σₘ)));
		double iterationCheck = antipodal ? fabs(λ)-π : fabs(λ);
		if (iterationCheck > π) return inverseInfos;//throw new EvalError('λ > π');
	} while (fabs(λ-λʹ) > 1e-12 && ++iterations<1000); // TV: 'iterate until negligible change in λ' (≈0.006mm)

	if (iterations >= 1000) return inverseInfos;//throw new EvalError('Vincenty formula failed to converge');

	double uSq = cosSqα * (a*a - b*b) / (b*b);
	double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
	double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
	double Δσ = B*sinσ*(cos2σₘ+B/4*(cosσ*(-1+2*cos2σₘ*cos2σₘ)-B/6*cos2σₘ*(-3+4*sinσ*sinσ)*(-3+4*cos2σₘ*cos2σₘ)));

	double s = b*A*(σ-Δσ); // s = length of the geodesic

	// note special handling of exactly antipodal points where sin²σ = 0 (due to discontinuity
	// atan2(0, 0) = 0 but atan2(ε, 0) = π/2 / 90°) - in which case bearing is always meridional,
	// due north (or due south!)
	// α = azimuths of the geodesic; α2 the direction P₁ P₂ produced
	double α1 = fabs(sinSqσ) < ε ? 0 : atan2(cosU2*sinλ,  cosU1*sinU2-sinU1*cosU2*cosλ);
	double α2 = fabs(sinSqσ) < ε ? π : atan2(cosU1*sinλ, -sinU1*cosU2+cosU1*sinU2*cosλ);

	inverseInfos.distance = s;
	inverseInfos.initialBearing = fabs(s) < ε ? INVALID_YAW : wrap360(toDegrees(α1));
	inverseInfos.finalBearing = fabs(s) < ε ? INVALID_YAW : wrap360(toDegrees(α2));
	inverseInfos.iterations = iterations;

	return inverseInfos;
}

double VincentyInitialBearing(double lat1, double lon1, double lat2, double lon2)
{
    VincentyInverseInfos inverseInfos = VincentyInverse(lat1,lon1,lat2,lon2);
    return inverseInfos.initialBearing;
}

double VincentyFinalBearing(double lat1, double lon1, double lat2, double lon2)
{
    VincentyInverseInfos inverseInfos = VincentyInverse(lat1,lon1,lat2,lon2);
    return inverseInfos.finalBearing;
}

LngLat VincentyDestinationPoint(double lat1, double lon1,double distance, double initialBearing)
{
    VincentyDirectInfos direct = VincentyDirect(lat1,lon1,distance,initialBearing);
    return direct.point;
}

double VincentyFinalBearingOn(double lat1, double lon1,double distance, double initialBearing)
{
    VincentyDirectInfos direct = VincentyDirect(lat1,lon1,distance,initialBearing);
    return direct.finalBearing;
}

VincentyDirectInfos VincentyDirect(double lat1, double lon1,double distance,double initialBearing)
{
    VincentyDirectInfos directInfo = {{INVALID_LONGITUDE,INVALID_LATITUDE},INVALID_YAW,0};
    if (INVALID_DISTANCE == distance || distance < 0)
    {
        return directInfo;
    }
    if (0 == distance)
    {
        directInfo.point.longitude = lon1;
        directInfo.point.latitude = lat1;
        return directInfo;
    }
    if (INVALID_YAW == initialBearing || initialBearing < 0 || initialBearing > 360.0)
    {
        return directInfo;
    }

    double φ1 = toRadians(lat1), λ1 = toRadians(lon1);
    double α1 = toRadians(initialBearing);
    double s = distance;

    // allow alternative ellipsoid to be specified
    double a = low_a;
    double b = low_b;
    double f = ellipsiod_flattening;

    double sinα1 = sin(α1);
    double cosα1 = cos(α1);

    double tanU1 = (1-f) * tan(φ1), cosU1 = 1 / sqrt((1 + tanU1*tanU1)), sinU1 = tanU1 * cosU1;
    double σ1 = atan2(tanU1, cosα1); // σ1 = angular distance on the sphere from the equator to P1
    double sinα = cosU1 * sinα1;          // α = azimuth of the geodesic at the equator
    double cosSqα = 1 - sinα*sinα;
    double uSq = cosSqα * (a*a - b*b) / (b*b);
    double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
    double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));

    double σ = s / (b*A), sinσ = 0.00, cosσ = 0.00; // σ = angular distance P₁ P₂ on the sphere
    double cos2σₘ = 0.00; // σₘ = angular distance on the sphere from the equator to the midpoint of the line

    double σʹ = 0.00;
    uint16_t iterations = 0;
    do {
        cos2σₘ = cos(2*σ1 + σ);
        sinσ = sin(σ);
        cosσ = cos(σ);
        double Δσ = B*sinσ*(cos2σₘ+B/4*(cosσ*(-1+2*cos2σₘ*cos2σₘ)-B/6*cos2σₘ*(-3+4*sinσ*sinσ)*(-3+4*cos2σₘ*cos2σₘ)));
        σʹ = σ;
        σ = s / (b*A) + Δσ;
    } while (fabs(σ-σʹ) > 1e-12 && ++iterations<100); // TV: 'iterate until negligible change in λ' (≈0.006mm)
    if (iterations >= 100) return directInfo;//throw new EvalError('Vincenty formula failed to converge'); // not possible?

    double x = sinU1*sinσ - cosU1*cosσ*cosα1;
    double φ2 = atan2(sinU1*cosσ + cosU1*sinσ*cosα1, (1-f)*sqrt(sinα*sinα + x*x));
    double λ = atan2(sinσ*sinα1, cosU1*cosσ - sinU1*sinσ*cosα1);
    double C = f/16*cosSqα*(4+f*(4-3*cosSqα));
    double L = λ - (1-C) * f * sinα * (σ + C*sinσ*(cos2σₘ+C*cosσ*(-1+2*cos2σₘ*cos2σₘ)));
    double λ2 = λ1 + L;

    double α2 = atan2(sinα, -x);


    directInfo.point.latitude = toDegrees(φ2);
    directInfo.point.longitude = toDegrees(λ2);
    directInfo.iterations = iterations;
    directInfo.finalBearing = wrap360(toDegrees(α2));

    return directInfo;
}

LngLat VincentyIntermediatePointTo(double lat1, double lon1, double lat2, double lon2,double fraction)
{
    LngLat lnglat = {lat1,lon1};
    if (0 == fraction)
    {
        return lnglat;
    }
    if (1 == fraction)
    {
        lnglat.latitude = lat2;
        lnglat.longitude = lon2;
        return lnglat;
    }

    VincentyInverseInfos inverse = VincentyInverse(lat1,lon1,lat2,lon2);
    if (INVALID_YAW == inverse.initialBearing)
    {
        return lnglat;
    }
    return VincentyDestinationPoint(lat1,lon1,inverse.distance * fraction,inverse.initialBearing);
}

double parseDm(double dddmm_mmmmm)
{
    double val = dddmm_mmmmm / 100.00;
    uint16_t dec = (uint16_t)val;
    return dec + (val - dec) * 100.00/60.00;
    //return (uint16_t)(dddmm_mmmmm / 100.00) + fmod(dddmm_mmmmm,100.0) / 60.0;
}

double getInitialBearingToFinalBearing(double finalBearing)
{
    return wrap360(finalBearing + 180.0);
}

double getMagneticHeading(double initialBearing,double magneticDec)
{
    /*
    纬度(-90 ~ 90 北纬为正数，南纬为负数) north + south -
    经度(-180~180 东经正数，西经为负数)east+  west-

    ‌真航向角（True Heading, TH）‌
    以地理真北（经线北端）为基准，计算公式：
    ‌TH = MH + δ‌
    其中 MH 为磁航向角，δ 为磁偏角（东偏取正，西偏取负）‌。
    真北方向通过天文观测或高精度GNSS定位确定，不受磁场干扰‌。

    ‌磁航向角（Magnetic Heading, MH）‌
    以磁北（地磁北极方向）为基准，由磁罗盘直接测得‌28。
    与真航向的转换关系：
    ‌MH = TH - δ‌（或 MH = TH + |δ|，需根据磁偏角方向调整符号）‌
    */
    return wrap360(initialBearing - magneticDec);
}

double getInitialBearingToMagneticHeading(double magneticHeading,double magneticDec)
{
    return wrap360(magneticHeading + magneticDec);
}

double calculateElevation(double distance,double alt1,double alt2)
{
    // 计算高度差和水平距离
    double h = alt2 - alt1;
    double d = sqrt(distance*distance - h*h);
    
    // 俯仰角计算（含折射修正）
    double elevation = toDegrees(atan2(h, d)) - REFRACTION;
    return elevation;
}
