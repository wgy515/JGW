#include "StdAfx.h"
#include "VincentyGeodesics.h"
#include <JGW_FoundationFunc/JGW_StringFunc.h>
#include <JGW_FoundationFunc/JGW_StringFuncTemplate.hpp>
namespace JGW
{

	/**
	* 3.1415926535897931
	* Let denote the ratio of the circumference of a circle to its diameter, specified by a constant π
	*/
	static double PI = 3.14159265358979;
	/**
	* 1E-12 epsilon
	*/
	static double EPSILON12 = 0.000000000001;
	/**
	* 1E-24 epsilon
	* 1 * 10^(-24)
	* 0.000000000000000000000001
	*/
	static double EPSILON24 = 1e-24;
	/**
	* ~2.2E-16 2.22044604925031E-16
	*/
	static double EPSILON16 = 2.22044604925031E-16;
	/**
	* WGS84 ellipsiod
	* 6378137.0
	* Equatorial radius:
	* Semi-major axis a
	*/
	static double low_a = 6378137;
	/**
	* Semi-minor axis b
	*/
	static double low_b = 6356752.3142;
	/**
	* flattening
	*/
	static double f = 1 / 298.257223563;
	/**
	* max iterations count
	*/
	static int MaxIterations = 100;
	/**
	* Converts angles to radians
	* 将角度转弧度
	* @param degrees angles
	* @return
	*/
	static double ToRadians(double degrees) 
	{
		return degrees * (PI / 180);
	}

	/**
	* Converts radians to angles
	* 将弧度转角度
	* @param radians radians
	* @return
	*/
	static double ToDegrees(double radians) 
	{
		return (radians * 180) / PI;
	}


	/**
	* 计算两点方向偏航角 第二点到第一点偏航角
	*/
	double VincentyGeodesics::calcRevAzimuth = INVALID_YAW_F;
	/**
	* 计算两点的俯仰角===根据海拔高度差
	*/
	double VincentyGeodesics::calcPitch = INVALID_YAW_F;
	/**
	* 计算两点的俯仰角===根据海拔高度差
	*/
	double VincentyGeodesics::calcRevPitch = INVALID_YAW_F;
	/**
	* 计算两点之间距离
	*/
	double VincentyGeodesics::calcDistance = INVALID_DISTANCE;
	/**
	* 计算两点的偏航角,第一点到第二点偏航角
	* 真航向角
	*/
	double VincentyGeodesics::calcAzimuth = INVALID_YAW_F;

	VincentyGeodesics::VincentyGeodesics(void)
	{
	}


	VincentyGeodesics::~VincentyGeodesics(void)
	{
	}

	double VincentyGeodesics::VincentyDirLat(double lat, double lon, double azimuth, double distance)
	{
		DirParams p = VincentyDir(lat, lon, azimuth, distance);

		double x = p.sinU1 * p.sinSigma - p.cosU1 * p.cosSigma * p.cosAlpha1;
		double phi2 = atan2(p.sinU1 * p.cosSigma + p.cosU1 * p.sinSigma * p.cosAlpha1, (1 - f) * sqrt(pow(p.sinAlpha, 2) + pow(x, 2)));

		return ToDegrees(phi2);
	}

	double VincentyGeodesics::VincentyDirLon(double lat, double lon, double azimuth, double distance)
	{
		DirParams p = VincentyDir(lat, lon, azimuth, distance);

		double lambda = atan2(p.sinSigma * p.sinAlpha1, p.cosU1 * p.cosSigma - p.sinU1 * p.sinSigma * p.cosAlpha1);
		double C = f / 16 * p.cosSqAlpha * (4 + f * (4 - 3 * p.cosSqAlpha));
		double fix1 = p.cos2sigmaM + C * p.cosSigma * (-1 + 2 * pow(p.cos2sigmaM, 2));
		double L = lambda - (1 - C) * f * p.sinAlpha * (p.sigma + C * p.sinSigma * fix1);

		double lambda2 = p.lambda1 + L;

		if (lambda2 == PI)
		{
			return 180;
		}

		return NormalizeLon(ToDegrees(lambda2));
	}

	double VincentyGeodesics::VincentyDirRevAzimuth(double lat, double lon, double azimuth, double distance)
	{
		return VincentyDirRevAzimuth(lat,lon,azimuth,distance,true);
	}

	double VincentyGeodesics::VincentyDirRevAzimuth(double lat, double lon, double azimuth, double distance, bool returnAzimuth)
	{
		DirParams p = VincentyDir(lat, lon, azimuth, distance);

		double x = p.sinU1 * p.sinSigma - p.cosU1 * p.cosSigma * p.cosAlpha1;
		double alpha2 = atan2(p.sinAlpha, -x);

		if (returnAzimuth)
		{
			return NormalizeAzimuth(ToDegrees(alpha2) + 180, true);
		}

		return NormalizeAzimuth(ToDegrees(alpha2), true);
	}

	double VincentyGeodesics::VincentyInvDistance(double lat1, double lon1, double lat2, double lon2)
	{
		InvParams p = VincentyInv(lat1, lon1, lat2, lon2);

		if (abs(p.s) < EPSILON16)
		{
			return 0.00;
		}

		return p.s;
	}

	double VincentyGeodesics::VincentyInvFwdAzimuth(double lat1, double lon1, double lat2, double lon2)
	{
		InvParams p = VincentyInv(lat1, lon1, lat2, lon2);

		if (abs(p.s) < EPSILON16)
		{
			throw "The distance is too short";
		}

		double fwdAz = 0.00;
		if (abs(p.sinSqSigma) < EPSILON16)//special handling of exactly antipodal points where sinSigma = 0
		{
			fwdAz = 0.00;
		}
		else
		{
			fwdAz = atan2(p.cosU2 * p.sinLambda, p.cosU1 * p.sinU2 - p.sinU1 * p.cosU2 * p.cosLambda);
		}

		return NormalizeAzimuth(ToDegrees(fwdAz), true);
	}

	double VincentyGeodesics::VincentyInvFwdAzimuth(InvParams invParams)
	{
		if (abs(invParams.s) < EPSILON16)
		{
			throw ("Abs(p.s) < EPSILON16");
		}

		double fwdAz = 0.00;

		if (abs(invParams.sinSqSigma) < EPSILON16)
		{
			fwdAz = 0;
		}
		else
		{
			fwdAz = atan2(invParams.cosU2 * invParams.sinLambda, invParams.cosU1 * invParams.sinU2 - invParams.sinU1 * invParams.cosU2 * invParams.cosLambda);
		}

		return NormalizeAzimuth(ToDegrees(fwdAz), true);
	}

	double VincentyGeodesics::VincentyInvRevAzimuth(InvParams p)
	{
		bool returnAzimuth = true;
		if (abs(p.s) < EPSILON16)
		{
			throw ("The distance is too short");
		}

		double revAz;
		if (abs(p.sinSqSigma) < EPSILON16)
		{
			// special handling of exactly antipodal points where sinSigma = 0
			revAz = PI;
		}
		else
		{
			revAz = atan2(p.cosU1 * p.sinLambda, -p.sinU1 * p.cosU2 + p.cosU1 * p.sinU2 * p.cosLambda);
		}

		if (returnAzimuth)
		{
			return NormalizeAzimuth(ToDegrees(revAz) + 180, true);
		}
		return NormalizeAzimuth(ToDegrees(revAz), true);
	}

	double VincentyGeodesics::VincentyInvRevAzimuth(double lat1, double lon1, double lat2, double lon2)
	{
		return VincentyInvRevAzimuth(lat1,lon1,lat2,lon2,true);
	}

	double VincentyGeodesics::VincentyInvRevAzimuth(double lat1, double lon1, double lat2, double lon2, bool returnAzimuth)
	{
		InvParams p = VincentyInv(lat1, lon1, lat2, lon2);
		if (abs(p.s) < EPSILON16)
		{
			throw ("The distance is too short");
		}

		double revAz;
		if (abs(p.sinSqSigma) < EPSILON16)
		{
			// special handling of exactly antipodal points where sinSigma = 0
			revAz = PI;
		}
		else
		{
			revAz = atan2(p.cosU1 * p.sinLambda, -p.sinU1 * p.cosU2 + p.cosU1 * p.sinU2 * p.cosLambda);
		}

		if (returnAzimuth)
		{
			return NormalizeAzimuth(ToDegrees(revAz) + 180, true);
		}
		return NormalizeAzimuth(ToDegrees(revAz), true);
	}

	DirParams VincentyGeodesics::VincentyDir(double lat, double lon, double azimuth, double distance)
	{
		DirParams p = {0};

		double phi1 = ToRadians(lat);
		p.lambda1 = ToRadians(lon);
		double alpha1 = ToRadians(azimuth);
		double s = distance;

		double fix1 = 0.00;// temp variable to prevent "formula too complex.." error
		double fix2 = 0.00;// temp variable to prevent "formula too complex.." error

		p.sinAlpha1 = sin(alpha1);
		p.cosAlpha1 = cos(alpha1);

		double tanU1 = (1 - f) * tan(phi1);
		p.cosU1 = 1 / sqrt((1 + pow(tanU1, 2)));
		p.sinU1 = tanU1 * p.cosU1;
		double sigma1 = atan2(tanU1, p.cosAlpha1);// sigma1 = angular distance on the sphere from the equator to P1
		p.sinAlpha = p.cosU1 * p.sinAlpha1;// Alpha = azimuth of the geodesic at the equator
		p.cosSqAlpha = 1 - pow(p.sinAlpha, 2);
		fix1 = pow(low_a, 2) - pow(low_b, 2);
		double uSq = p.cosSqAlpha * fix1 / pow(low_b, 2);
		fix1 = -768 + uSq * (320 - 175 * uSq);
		double A = 1 + uSq / 16384 * (4096 + uSq * fix1);
		fix1 = -128 + uSq * (74 - 47 * uSq);
		double B = uSq / 1024 * (256 + uSq * fix1);

		p.sigma = s / (low_b * A);// sigma = angular distance P1-P2 on the sphere
		double deltaSigma = 0.00;
		double sigma2 = 0.00;
		int iterationCount = 0;

		do
		{
			p.cos2sigmaM = cos(2 * sigma1 + p.sigma);// sigmaM = angular distance on the sphere from the equator to the midpoint of the line
			p.sinSigma = sin(p.sigma);
			p.cosSigma = cos(p.sigma);
			fix1 = (-1 + 2 * pow(p.cos2sigmaM, 2));
			fix2 = (-3 + 4 * pow(p.sinSigma, 2)) * (-3 + 4 * pow(p.cos2sigmaM, 2));
			deltaSigma = B * p.sinSigma * (p.cos2sigmaM + B / 4 * (p.cosSigma * fix1 - B / 6 * p.cos2sigmaM * fix2));
			sigma2 = p.sigma;
			p.sigma = s / (low_b * A) + deltaSigma;
			iterationCount = iterationCount + 1;
		} while (abs(p.sigma - sigma2) > EPSILON12 && iterationCount < MaxIterations);// iterate until negligible change in lambdaP (~0.006mm)

		if (iterationCount >= MaxIterations)
		{
			throw ("failed to converge");
		}

		return p;
	}

	InvParams VincentyGeodesics::VincentyInv(double lat1, double lon1, double lat2, double lon2)
	{
		InvParams p = {0};
		double sinSigma = 0.00;
		double cosSigma;
		double sinAlpha;
		double cosSqAlpha;
		double cos2sigmaM;
		double C;
		double uSq;
		double upper_B;
		double fix2;// temp variable to prevent "formula too complex.." error
		double fix1;// temp variable to prevent "formula too complex.." error
		int iterationCount = 0;

		lat1 = ToRadians(lat1);
		lat2 = ToRadians(lat2);

		double L = ToRadians(lon2 - lon1); // L = difference in longitude, U = reduced latitude, defined by tan U=(1-f)*Tan(lat)

		double tanU1 = (1 - f) * tan(lat1);
		p.cosU1 = 1 / sqrt(1 + pow(tanU1, 2));
		p.sinU1 = tanU1 * p.cosU1;

		double tanU2 = (1 - f) * tan(lat2);
		p.cosU2 = 1 / sqrt(1 + pow(tanU2, 2));
		p.sinU2 = tanU2 * p.cosU2;

		bool antipodal = (abs(L) > PI / 2) || (abs(lat2 - lat1) > PI / 2);

		double lambda = L;// lambda = difference in longitude on an auxiliary sphere
		p.sigma = antipodal ? PI : 0;
		cosSigma = antipodal ? -1 : 1;
		cos2sigmaM = 1;// cos2sigmaM = angular distance on the sphere from the equator to the midpoint of the line
		cosSqAlpha = 1; // cosSqAlpha = azimuth of the geodesic at the equator
		double lambdaP = 0.00;

		do
		{
			p.sinLambda = sin(lambda);
			p.cosLambda = cos(lambda);
			p.sinSqSigma = (pow((p.cosU2 * p.sinLambda), 2)) + (pow((p.cosU1 * p.sinU2 - p.sinU1 * p.cosU2 * p.cosLambda), 2));
			if (abs(p.sinSqSigma) < EPSILON24)
			{
				break;// ("co-incident/antipodal points (Sigma < ~0.006mm)");
			}

			sinSigma = sqrt(p.sinSqSigma);
			cosSigma = p.sinU1 * p.sinU2 + p.cosU1 * p.cosU2 * p.cosLambda;
			p.sigma = atan2(sinSigma, cosSigma);
			sinAlpha = p.cosU1 * p.cosU2 * p.sinLambda / sinSigma;
			cosSqAlpha = 1 - pow(sinAlpha, 2);


			if (cosSqAlpha != 0)
			{
				cos2sigmaM = cosSigma - 2 * p.sinU1 * p.sinU2 / cosSqAlpha;
			}
			else
			{
				cos2sigmaM = 0; // on equatorial line cosSqAlpha = 0 (par 6)
			}

			C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
			lambdaP = lambda;

			fix1 = cos2sigmaM + C * cosSigma * (-1 + 2 * pow(cos2sigmaM, 2));
			lambda = L + (1 - C) * f * sinAlpha * (p.sigma + C * sinSigma * fix1);

			double iterationCheck = antipodal ? abs(lambda) - PI : abs(lambda);
			if (iterationCheck > PI)
			{
				throw ("iterationCheck > PI");
			}

			iterationCount = iterationCount + 1;
		} while (abs(lambda - lambdaP) > EPSILON12 && iterationCount < MaxIterations);//  iterate until negligible change in lambdaP (~0.006mm)


		if (iterationCount >= MaxIterations)
		{
			throw ("failed to converge");
		}

		uSq = cosSqAlpha * (pow(low_a, 2) - pow(low_b, 2)) / pow(low_b, 2);

		fix1 = -768 + uSq * (320 - 175 * uSq);
		p.upper_A = 1 + uSq / 16384 * (4096 + uSq * fix1);

		fix1 = -128 + uSq * (74 - 47 * uSq);
		upper_B = uSq / 1024 * (256 + uSq * fix1);

		fix1 = cosSigma * (-1 + 2 * pow(cos2sigmaM, 2));
		fix2 = upper_B / 6 * cos2sigmaM * (-3 + 4 * pow(sinSigma, 2)) * (-3 + 4 * pow(cos2sigmaM, 2));


		p.deltaSigma = upper_B * sinSigma * (cos2sigmaM + upper_B / 4 * (fix1 - fix2));
		p.s = low_b * p.upper_A * (p.sigma - p.deltaSigma); // s = length of the geodesic

		return p;
	}

	std::wstring VincentyGeodesics::ConvertDegrees(double decimalDeg, int isLongitude)
	{
		std::wstring convertDegrees;
		if (1 == isLongitude)
		{
			decimalDeg = NormalizeLon(decimalDeg);
		}
		else if (0 == isLongitude)
		{
			decimalDeg = NormalizeLat(decimalDeg);
		}
		else
		{
			decimalDeg = NormalizeAzimuth(decimalDeg, false);
		}

		int s = Sign(decimalDeg);
		decimalDeg = abs(decimalDeg);
		int degrees = (int)(decimalDeg);
		int minutes = (int)((decimalDeg - degrees) * 60);
		//double fl = (decimalDeg - degrees - (minutes / 60)) * 60 * 60;
		double seconds = (decimalDeg - degrees - (minutes / 60.00)) * 60 * 60;// 4 digit precision corresponds to ~3mm

		if (0 == isLongitude)
		{
			//convertDegrees = "";
			//convertDegrees = String.Format("{0:D2}{1}{2:D2}{3}{4:00.0000}{5}", degrees, Convert.ToChar(176), minutes, Convert.ToChar(8242), seconds, Convert.ToChar(8243));
			//convertDegrees += String.format("%02d", degrees);
			//convertDegrees += String.format("%c", (char)176);
			//convertDegrees += String.format("%d", minutes);
			//convertDegrees += String.format("%c", (char)8242);
			//convertDegrees += String.format("%2.4f", seconds);
			//convertDegrees += String.format("%c", (char)8243);

			convertDegrees = JGW_GetFormatWString(L"%02d%c%02d%c%2.4f%c",degrees,(wchar_t)176,minutes,(wchar_t)8242,seconds,(wchar_t)8243);
			//return string.Format$(degrees, "00") & Chr$(176) & Format$(minutes, "00") & "'" & Format$(seconds, "00.0000") + Chr$(34)
		}
		else
		{
			//convertDegrees = String.Format("{0:D3}{1}{2:D2}{3}{4:00.0000}{5}", degrees, Convert.ToChar(176), minutes, Convert.ToChar(8242), seconds, Convert.ToChar(8243));

			convertDegrees = JGW_GetFormatWString(L"%03d%c%02d%c%2.4f%c",degrees,(wchar_t)176,minutes,(wchar_t)8242,seconds,(wchar_t)8243);
		}

		if (decimalDeg == 0)
		{

		}
		else if (-1 == isLongitude)
		{
			if (s == -1)
			{
				convertDegrees = std::wstring(L"-") + convertDegrees;
			}
		}
		else if (1 == isLongitude)
		{
			if (s == 1)
			{
				convertDegrees += L"E";
			}
			else if (s == -1)
			{
				convertDegrees += L"W";
			}
		}
		else
		{
			if (s == 1)
			{
				convertDegrees += L"N";
			}
			else if (s == -1)
			{
				convertDegrees += L"S";
			}
		}
		return convertDegrees;
	}

	double VincentyGeodesics::ConvertDecimal(std::wstring degreeDeg)
	{
		double convertDec = 0.00;
		// double single quote
		JGW_ReplaceStringW(degreeDeg,L"''", L" ");
		// double quote
		JGW_ReplaceStringW(degreeDeg,L"\"\"", L" ");
		// single quote
		JGW_ReplaceStringW(degreeDeg,L"'", L" ");
		// Prime
		JGW_ReplaceCharW(degreeDeg,(wchar_t)8242, L' ');
		// Double Prime
		JGW_ReplaceCharW(degreeDeg,(wchar_t)8243, L' ');
		// Section Sign
		JGW_ReplaceCharW(degreeDeg,(wchar_t)167, L' ');
		// Degree Sign
		JGW_ReplaceCharW(degreeDeg,(wchar_t)176, L' ');
		// Masculine Ordinal Indicator
		JGW_ReplaceCharW(degreeDeg,(wchar_t)186, L' ');
		// Latin Small Letter O With Stroke
		JGW_ReplaceCharW(degreeDeg,(wchar_t)248, L' ');
		JGW_ReplaceStringW(degreeDeg,L":",L" ");
		JGW_ReplaceStringW(degreeDeg,L"*",L" ");
		JGW_StringFunc<std::wstring>::Ltrim(degreeDeg);
		JGW_StringFunc<std::wstring>::Rtrim(degreeDeg);
		//degreeDeg = degreeDeg.trim();


		wchar_t lc = degreeDeg.at(degreeDeg.length() - 1);
		wchar_t fc = degreeDeg.at(0);
		int sign = 1;

		if (lc < L'0' || lc > L'9')
		{
			if (lc == L'W' || lc == L'w' || lc == L'S' || lc == L's')
			{
				sign = -1;
			}
			else if (lc == L'E' || lc == L'e' || lc == L'N' || lc == L'n')
			{
				sign = 1;
			}
			else
			{
				return INVALID_LATITUDE;
			}
		}
		else if (fc < L'0' || fc > L'9')
		{
			if (fc == L'W' || fc == L'w' || fc == L'S' || fc == L's')
			{
				sign = -1;
			}
			else if (fc == L'E' || fc == L'e' || fc == L'N' || fc == L'n')
			{
				sign = 1;
			}
			else
			{
				return INVALID_LATITUDE;
			}
		}

		degreeDeg = degreeDeg.substr(0,degreeDeg.length() - 1);
		JGW_StringFunc<std::wstring>::Ltrim(degreeDeg);
		JGW_StringFunc<std::wstring>::Rtrim(degreeDeg);
		JGW_ReplaceStringW(degreeDeg,L"  ", L" ");
		std::vector<std::wstring> contents;
		JGW_ParserStrW(degreeDeg.c_str(),L" ",contents);
		//String[] contents = degreeDeg.split( " " );
		double degrees = _ttof(contents[0].c_str());
		double minutes = 0.00f;
		if (contents.size() > 0)
		{
			minutes = _ttof(contents[1].c_str());
			minutes = minutes / 60.0;
		}
		double seconds = 0.00f;
		if (contents.size() > 1)
		{
			seconds = _ttof(contents[2].c_str());
			seconds = seconds / 3600.0;
		}
		convertDec = (degrees + (Sign(degrees) * minutes) + (Sign(degrees) * Sign(minutes) * seconds)) * sign;

		return convertDec;
	}

	int VincentyGeodesics::Sign(double val) 
	{
		return (val >= 0) ? 1 : -1;
	}

	double VincentyGeodesics::ModDouble(double dividend, double divisor, bool sameSignAsDivisor) 
	{
		double modDouble = 0.00;
		if (sameSignAsDivisor)
		{
			modDouble = dividend - (divisor * floor(dividend / divisor));
		}
		else
		{
			modDouble = dividend - (divisor * (int)(dividend / divisor));
		}
		// this function can only be accurate when (a / b) is outside [-2.22E-16,+2.22E-16]
		// without this correction, ModDouble(.66, .06) = 5.55111512312578E-17 when it should be 0
		// http://en.wikipedia.org/wiki/Machine_epsilon
		if (modDouble >= -EPSILON16 && modDouble <= EPSILON16)
		{
			modDouble = 0.00;
		}
		return modDouble;
	}


	/// <summary>
	/// Normalizes latitude to -90..+90 range.
	/// </summary>
	/// <param name="lat">latitude</param>
	/// <returns></returns>
	double VincentyGeodesics::NormalizeLat(double lat)
	{
		return abs(ModDouble(lat - 90, 360, true) - 180) - 90;
	}
	/// <summary>
	/// Normalizes longitude to -180..+180 range
	/// </summary>
	/// <param name="lon">longitude</param>
	/// <returns></returns>
	double VincentyGeodesics::NormalizeLon(double lon)
	{
		return 2 * ModDouble((lon / 2) + 90, 180, true) - 180;
	}
	/// <summary>
	/// Normalizes azimuth to 0..360 range. Note: by default input and return values have the same sign. To obtain only positive values pass positiveOnly = true
	/// </summary>
	/// <param name="azimuth">azimuth</param>
	/// <param name="positiveOnly">To obtain only positive values pass positiveOnly = true</param>
	/// <returns></returns>
	double VincentyGeodesics::NormalizeAzimuth(double azimuth, bool positiveOnly)
	{
		return ModDouble(azimuth, 360, positiveOnly);
	}
	/// <summary>
	/// source: http://en.wikibooks.org/wiki/Programming:Visual_Basic_Classic/Simple_Arithmetic#Trigonometrical_Functions
	/// note: x & y are in reverse order to match JavaScript atan2() params order
	/// </summary>
	/// <param name="y"></param>
	/// <param name="x"></param>
	/// <returns></returns>
	double VincentyGeodesics::Atan2(double y, double x)
	{
		if (y > 0)
		{
			if (x >= y)
			{
				return atan(y / x);
			}
			else if (x <= -y)
			{
				return atan(y / x) + PI;
			}
			else
			{
				return PI / 2 - atan(x / y);
			}
		}

		else
		{
			if (x >= -y)
			{
				return atan(y / x);
			}
			else if (x <= y)
			{
				return atan(y / x) - PI;
			}
			else
			{
				return -atan(x / y) - PI / 2;
			}
		}
	}

	void VincentyGeodesics::InitVincentyAzimuthPitch()
	{
		calcAzimuth = INVALID_YAW_F;
		calcPitch = INVALID_YAW_F;
		calcDistance = INVALID_DISTANCE;
		calcRevAzimuth = INVALID_YAW_F;
		calcRevPitch = INVALID_YAW_F;
	}

	bool VincentyGeodesics::IsValidGPS(double lat,double lng)
	{
		if (INVALID_LATITUDE == (lat) || INVALID_LONGITUDE == (lng)) 
		{
			return false;
		}

		if ((lat >= -90.0 && lat <= 90.0) && (lng >= -180.0 && lng <= 180.0)) 
		{
			return true;
		}

		return false;
	}

	void VincentyGeodesics::CalcVincentyAzimuthPitch(double latitude1,double longitude1,double latitude2,double longitude2,double altitude1,double altitude2)
	{
		InitVincentyAzimuthPitch();
		// 检查经纬度参数有效性
		Log4WD_F(L"lat1 : %f,lon1 : %f,lat2 : %f,lon2 : %f\n",latitude1,longitude1,latitude2,longitude2);

		if (IsValidGPS(latitude1,longitude1) && IsValidGPS(latitude2,longitude2)) 
		{
			try 
			{
				InvParams invParams = VincentyInv(latitude1,longitude1,latitude2,longitude2);
				calcAzimuth = VincentyInvFwdAzimuth(invParams);
				calcRevAzimuth = VincentyInvRevAzimuth(invParams);
				calcDistance = invParams.s;
				Log4WD_F(L"VincentyAzimuthPitch Azimuth : %.3f,RevAzimuth : %.3f,Distance : %f\r\n",calcAzimuth,calcRevAzimuth,calcDistance);
			} 
			catch (char* szErrorMsg) 
			{
				Log4WE_F(L"Calc VincentyAzimuthPitch Error : %s",JGW_A2W_A(szErrorMsg).c_str());
			} 
			catch (...)
			{

			}
		}
		Log4WD_F(L"VincentyAzimuthPitch altitude1 : %f,altitude2 : %f\r\n",altitude1,altitude2);

		if (INVALI_ALTITUDE != altitude1 && INVALI_ALTITUDE != (altitude2)) {
			double height = abs(altitude1 - altitude2);
			// 直线距离
			double distance = sqrt(calcDistance * calcDistance - height * height);
			double pitch = ToDegrees(atan(height / distance));
			// 默认往上俯仰是正数，往下俯仰是负数
			// 天线与PCBA平行
			if (altitude1 > altitude2) {
				calcPitch = -1.0 * pitch;
				calcRevPitch = pitch;
			} else if (altitude2 > altitude1) {
				calcPitch = pitch;
				calcRevPitch = -1.0 * pitch;
			} else {
				calcPitch = 0;
				calcRevPitch = 0;
			}

			//!!!!!!!! 天线与PCBA有90度夹角
			if (altitude1 > altitude2) {
				calcPitch = 90.0 - pitch;
				calcRevPitch = pitch + 90.0;
			} else if (altitude2 > altitude1) {
				calcPitch = pitch + 90.0;
				calcRevPitch = 90.0 - pitch;
			} else {
				calcPitch = 90.0;
				calcRevPitch = 90.0;
			}
		}
	}
}
