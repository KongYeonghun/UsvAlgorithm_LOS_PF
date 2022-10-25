#pragma once

#include <math.h>

// WGS84 parameters
#define UTM_ZON			52
#define Wa				6378137
#define Wb				6356752.314245
#define We				0.081819190842965
#define Weps			0.006739496742333
#define	PI				3.1415926535897931


class CCoordinateConv
{
public:
	CCoordinateConv(void) ;
	~CCoordinateConv(void) ;

public:
	void OnInitializationParam() ;
	void WGS2UTM(double dLatitude, double dLongitude) ;
	void UTM2WGS(double dUTM_X, double dUTM_Y, int nUTM_Zone) ;
	void GPSWGS84_DD2DM(double dLatitude, double dLongitude) ;
	void GPSWGS84_DM2DD(double dLatitude, double dLongitude) ;
	void Coordinate_UTM2WGS(double dUTM2WGS_X, double dUTM2WGS_Y) ;
	double getUTME(void);
	double getUTMN(void);
	double getWGS84Lon(void);
	double getWGS84Lat(void);

	double m_dUTM_X ;
	double m_dUTM_Y ;
	double m_dUTM2WGS_DDLAT ;
	double m_dUTM2WGS_DDLON ;
	double m_dUTM2WGS_DMLAT ;
	double m_dUTM2WGS_DMLON ;
	double m_dWGS84_Lat ;
	double m_dWGS84_Lon ;

	int m_nUTM_Zone ;
};
