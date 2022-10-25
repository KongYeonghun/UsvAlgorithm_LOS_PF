#include "CoordinateConv.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCoordinateConv::CCoordinateConv(void)
{
	OnInitializationParam() ;
}

CCoordinateConv::~CCoordinateConv(void)
{
}

void CCoordinateConv::OnInitializationParam()
{
	m_dUTM_X = 0.0 ;
	m_dUTM_Y = 0.0 ;

//	m_dUTM_X = 353114.82;
//	m_dUTM_Y = 4026086.41;

	m_dUTM2WGS_DDLAT = 0.0 ;
	m_dUTM2WGS_DDLON = 0.0 ;
	m_dUTM2WGS_DMLAT = 0.0 ;
	m_dUTM2WGS_DMLON = 0.0 ;
	m_dWGS84_Lat = 0.0 ;
	m_dWGS84_Lon = 0.0 ;

	m_nUTM_Zone = 0 ;
}

void CCoordinateConv::Coordinate_UTM2WGS(double dUTM2WGS_X, double dUTM2WGS_Y)
{
	UTM2WGS(dUTM2WGS_X, dUTM2WGS_Y, UTM_ZON) ;

	m_dUTM2WGS_DDLAT = m_dWGS84_Lat ;
	m_dUTM2WGS_DDLON = m_dWGS84_Lon ;

	GPSWGS84_DM2DD(m_dWGS84_Lat, m_dWGS84_Lon) ;
}


void CCoordinateConv::GPSWGS84_DD2DM(double dLatitude, double dLongitude)
{
	double dLat_deg, dLon_deg;
	double dLat_degdec, dLon_degdec;

	dLat_deg = floor(dLatitude) ;
	dLon_deg = floor(dLongitude) ;

	dLat_degdec = (dLatitude - dLat_deg) * 60 ;
	dLon_degdec = (dLongitude - dLon_deg) * 60 ;

	dLat_deg = dLat_deg * 100 ;
	dLon_deg = dLon_deg * 100 ;

	m_dUTM2WGS_DMLAT = dLat_deg + dLat_degdec ;
	m_dUTM2WGS_DMLON = dLon_deg + dLon_degdec ;	
}

void CCoordinateConv::GPSWGS84_DM2DD(double dLatitude, double dLongitude)
{
	double dLat_deg, dLon_deg;
	double dLat_degdec, dLon_degdec;
	double dLat_min, dLon_min;

	// ddxx.xxxxxx , dddxx.xxxxx	
	dLat_deg = floor(dLatitude/100.0) ;
	dLon_deg = floor(dLongitude/100.0) ;
	// xxmm.mmmmmm , xxxmm.mmmmm
	dLat_min = dLatitude - (dLat_deg*100.0) ;
	dLon_min = dLongitude - (dLon_deg*100.0) ;

	dLat_degdec = (dLat_min/60.0) ;
	dLon_degdec = (dLon_min/60.0) ;

	m_dWGS84_Lat = dLat_deg + dLat_degdec ;
	m_dWGS84_Lon = dLon_deg + dLon_degdec ;
}

void CCoordinateConv::WGS2UTM(double dLatitude, double dLongitude)
{
	double dLat ;
	double dLon ;

	// coordinates in radians
	dLat = dLatitude * PI/180 ;
	dLon = dLongitude * PI/180 ;

	// UTM parameters
	double lon0_f = floor(dLongitude/6)*6+3 ;	// reference longitude in degrees
	double lon0 = lon0_f*PI/180 ;				// in radians
	double k0 = 0.9996 ;						// scale on central meridian

	int FE = 500000 ;							// false easting
	int FN = (dLatitude < 0) * 10000000 ;		// false northing 

	// Equations parameters
	// N: radius of curvature of the earth perpendicular to meridian plane
	// Also, distance from point to polar axis
	double WN = Wa/sqrt( 1 - pow(We,2)*pow(sin(dLat),2)) ; 
	double WT = pow(tan(dLat),2) ;                
	double WC = (pow(We,2)/(1 - pow(We,2)))*pow(cos(dLat),2) ;
	double WLA = (dLon - lon0)*cos(dLat) ;                            

	// M: true distance along the central meridian from the equator to lat
	double WM = Wa*((1 - pow(We,2)/4 - 3*pow(We,4)/64 - 5*pow(We,6)/256)*dLat
		- (3*pow(We,2)/8 + 3*pow(We,4)/32 + 45*pow(We,6)/1024)*sin(2*dLat) 
		+ (15*pow(We,4)/256 + 45*pow(We,6)/1024)*sin(4*dLat) - (35*pow(We,6)/3072)*sin(6*dLat)) ;

	// easting
	m_dUTM_X = FE + k0*WN*(WLA + (1 - WT + WC)*pow(WLA,3)/6	+ (5 - 18*WT + pow(WT,2) + 72*WC - 58*Weps)*pow(WLA,5)/120) ;

	// northing 
	// M(lat0) = 0 so not used in following formula
	m_dUTM_Y = FN + k0*WM + k0*WN*tan(dLat)*(pow(WLA,2)/2 + (5 - WT + 9*WC + 4*pow(WC,2))*pow(WLA,4)/24
		+ (61 - 58*WT + pow(WT,2) + 600*WC - 330*Weps)*pow(WLA,6)/720);

	// UTM zone
	m_nUTM_Zone = (int)(floor(lon0_f/6)+31);
	// qDebug() << "m_nUTM_Zone : %d" << m_nUTM_Zone;
}

void CCoordinateConv::UTM2WGS(double dUTM_X, double dUTM_Y, int nUTM_Zone)
{
	double sa = 6378137.000000 ; 
	double sb = 6356752.314245 ;

	double e2 = sqrt((sa*sa) - (sb*sb))*0.0000001573130351105668 ;
	double e2cuadrada = e2*e2 ;
	double cc = (sa*sa)*0.0000001573130351105668 ;

	// Utmzone > 'M' => Y = y, Utmzone < 'M' => Y = y - 10000000
	dUTM_X = dUTM_X - 500000 ;
	dUTM_Y = dUTM_Y ;

	int Szt = ((nUTM_Zone*6) - 183) ; 
	double latr =  dUTM_Y*0.0000001571424896673576 ;
	double vv = (cc/sqrt((1 + (e2cuadrada*(cos(latr))*(cos(latr))))))*0.9996 ;
	double aa = dUTM_X/vv ;
	double a1 = sin(2*latr) ;
	double a2 = a1*(cos(latr))*(cos(latr)) ;
	double j2 = latr + (a1*0.5) ;
	double j4 = ((3*j2) + a2)*0.25 ;
	double j6 = ((5*j4) + (a2*(cos(latr))*(cos(latr))))*0.333333333333333 ;
	double alfa = 0.75*e2cuadrada ;
	double beta = 1.666666666666667*alfa*alfa ;
	double gama = 1.296296296296296*alfa*alfa*alfa ;
	double Bm = 0.9996*cc*(latr - alfa*j2 + beta*j4 - gama*j6) ;
	double bb = (dUTM_Y - Bm)/vv ;
	double Epsi = ((e2cuadrada*aa*aa)*0.5)*(cos(latr))*(cos(latr)) ;
	double Eps = aa*(1 - (Epsi*0.333333333333333)) ;
	double nab = (bb*(1 - Epsi)) + latr ;
	double senoheps = (exp(Eps) - exp(-Eps))*0.5 ;
	double Delt = atan(senoheps/(cos(nab))) ;
	double TaO = atan(cos(Delt) * tan(nab)) ;

	m_dWGS84_Lon = (Delt*(180/PI)) + Szt ;
	m_dWGS84_Lat = (latr + (1 + e2cuadrada*(cos(latr)*cos(latr)) - 1.5*e2cuadrada*sin(latr)*cos(latr)*(TaO - latr))*(TaO - latr))*(180/PI) ;
}

double CCoordinateConv::getUTME(void){
	return m_dUTM_X;
}

double CCoordinateConv::getUTMN(void){
	return m_dUTM_Y;
}

double CCoordinateConv::getWGS84Lon(void){
	return m_dWGS84_Lon;
}

double CCoordinateConv::getWGS84Lat(void){
	return m_dWGS84_Lat;
}