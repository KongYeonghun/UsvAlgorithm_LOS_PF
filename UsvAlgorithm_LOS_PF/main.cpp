
#include "main.hpp"

time_t timer;
struct tm* t;

using namespace std;

ofstream AlgLoggingFile;

int main(void)
{
	cout << "Potential Field Start.."<<endl; 

	char* cBuf = NULL; 
	cBuf = (char*)malloc(sizeof(char) * 256);
	/**************************************************************************************************************************/
	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString("IP_ADDRESS", "SystemCtrl_IP ", "-", cBuf, 256, SETTING_PATH);
	string SystemCtrl_IP = cBuf;
	cout << "SystemCtrl_IP   : "<< SystemCtrl_IP   << endl; 

	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString("IP_ADDRESS", "SystemCtrl_PORT_UA", "-", cBuf, 256, SETTING_PATH);
	int  SystemCtrl_PORT_UA = atoi(cBuf);
	cout << "SystemCtrl_PORT_UA   : "<< SystemCtrl_PORT_UA   << endl; 

	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString("IP_ADDRESS", "UsvAlgorithm_IP", "-", cBuf, 256, SETTING_PATH);
	string UsvAlgorithm_IP = cBuf;
	cout << "UsvAlgorithm_IP : "<< UsvAlgorithm_IP   << endl; 

	memset(cBuf, 0x00, sizeof(cBuf));
	GetPrivateProfileString("IP_ADDRESS", "UsvAlgorithm_PORT  ", "-", cBuf, 256, SETTING_PATH);
	int  UsvAlgorithm_PORT   = atoi(cBuf);
	cout << "UsvAlgorithm_PORT : "<< UsvAlgorithm_PORT   << endl; 
	
	/**************************************************************************************************************************/
	

	ServerSystemCtrl_UsvAlgorithm_Socket.UDP_CLIENT_OPEN(SystemCtrl_IP, UsvAlgorithm_IP, SystemCtrl_PORT_UA, UsvAlgorithm_PORT);

	if( ::bind(ServerSystemCtrl_UsvAlgorithm_Socket.ClientSocket, (struct sockaddr*)&ServerSystemCtrl_UsvAlgorithm_Socket.ClientInfo, sizeof(ServerSystemCtrl_UsvAlgorithm_Socket.ClientInfo)) < 0)
    {
        cout<<"ServerSystemCtrl_UsvAlgorithm_Socket() bind error."<<endl;
        closesocket( ServerSystemCtrl_UsvAlgorithm_Socket.ClientSocket );
        WSACleanup();
        exit(0);
    }

	char Buffer[BUFFER_SIZE];

	Header						    mHeader;
	ROSStatusInfo				    mROSStatusInfo;
	USVModeCommand				    mUSVModeCommand;
	MissionRecordInfo			    mMissionRecordInfo;
	PowerControl				    mPowerControl;
	EOCameraControlCommand		    mEOCameraControlCommand;
	PlatformControlCommand		    mPlatformControlCommand;
	USVStatusInfo				    mUSVStatusInfo;
	NavigationStatusInfo		    mNavigationStatusInfo;
	NavigationInfo				    mNavigationInfo;
	AutoNaviStatusInfo			    mAutoNaviStatusInfo;
	RadarTargetInfo				    mRadarTargetInfo;
	LidarTargetInfo				    mLidarTargetInfo;
	FlexTargetInfo				    mFlexTargetInfo;
	PlatformControlState		    mPlatformControlState;
	PlatformStatusInfo			    mPlatformStatusInfo;
	PowerControlResult			    mPowerControlResult;
	AcousticCommInfo			    mAcousticCommInfo;

	memset(&mUSVModeCommand, 0, sizeof(mUSVModeCommand));

	while(1)
	{
		ServerSystemCtrl_UsvAlgorithm_Socket.ClientToServer_Recv(Buffer, ServerSystemCtrl_UsvAlgorithm_Socket);

		memcpy(&mHeader,(Header*)&Buffer, sizeof(mHeader));

		switch(mHeader.Message_ID)
		{
			case 0x1401:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mUSVModeCommand);
				cout << "mUSVModeCommand.ModeDetail : " << mUSVModeCommand.ModeDetail << endl;
				cout << "mUSVModeCommand.AlgSelect : " << (int)mUSVModeCommand.AlgSelect << endl;
			}
			break;
			case 0x1402:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mMissionRecordInfo);
				cout << "/**************** MissionPointCount ******************/: " << mMissionRecordInfo.MissionPointCount << endl;
			}
			break;
			case 0x1403:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mNavigationInfo);

				cout << "Navi.Latitude : [ " << (double) mNavigationInfo.Latitude	 	
				<< " ] , Navi.Longitude : [ " << (double) mNavigationInfo.Longitude 		
				<< " ] , Navi.Heading : [ " << mNavigationInfo.Heading 		
				<< " ] , Navi.Velocity : " << mNavigationInfo.Velocity	 	<< endl;
			}
			break;
			case 0x1404:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mFlexTargetInfo);

				cout << "mFlexTargetInfo.TargetCount : [ " << mFlexTargetInfo.TargetCount << " ] "<<endl;
			}
			break;
		}// switch

		memset(&mAutoNaviStatusInfo , 0 ,sizeof(mAutoNaviStatusInfo));
		mAutoNaviStatusInfo = PotentailField(mUSVModeCommand, mMissionRecordInfo, mNavigationInfo, mFlexTargetInfo );

		if (mUSVModeCommand.ModeDetail == 3 && mUSVModeCommand.AlgSelect == 2) // 운용모드 확인 후 자율 운항 시작
		{
			cout << "LOS_PF_heading : [ " << mAutoNaviStatusInfo.Heading << " ] , LOS_PF_velocity : [ " << mAutoNaviStatusInfo.Velocity << " ] " << endl;

			ServerSystemCtrl_UsvAlgorithm_Socket.HeaderSetting(&mHeader, 0x27, 0x21, 0x1501, sizeof(mAutoNaviStatusInfo), 0);
			ServerSystemCtrl_UsvAlgorithm_Socket.MessageCombination(Buffer, mHeader, mAutoNaviStatusInfo);
			ServerSystemCtrl_UsvAlgorithm_Socket.ClientToServer_Send(Buffer, ServerSystemCtrl_UsvAlgorithm_Socket, COMM_HEADER_SIZE + mHeader.Payload_Length);
		}
	}//while
	return 0;
}

AutoNaviStatusInfo PotentailField(USVModeCommand mUSVModeCommand, MissionRecordInfo mMissionRecordInfo, NavigationInfo mNavigationInfo,FlexTargetInfo mFlexTargetInfo )
{
	//ICD message
	//AutoSailing -> UsvAlgorithm
	Header						    mHeader;
	
	//UsvAlgorithm -> AutoSailing
	AutoNaviStatusInfo			    mAutoNaviStatusInfo;

	stWPInfo		m_stWPInfo;
	stUSVMotion		m_stUSVMotion;
	stTargetMotion	m_stTargetMotion;
	stUsvGoalHnV	m_stUsvGoalHnV;
	stUsvGoalHnV	pre_m_stUsvGoalHnV;

	CCoordinateConv 				mCCoordinateConv;

	memset(&m_stWPInfo		,0, sizeof(m_stWPInfo)		);
	memset(&m_stTargetMotion		,0, sizeof(m_stTargetMotion)		);
	memset(&m_stUsvGoalHnV		,0, sizeof(m_stWPInfo)		);
	memset(&pre_m_stUsvGoalHnV		,0, sizeof(pre_m_stUsvGoalHnV)		);
	memset(&m_stUSVMotion		,0, sizeof(m_stUSVMotion)		);

	DataLogging_Init();

	char Buffer[BUFFER_SIZE];

	memset(&pre_m_stUsvGoalHnV, 0, sizeof(pre_m_stUsvGoalHnV));
	memset(&m_stUsvGoalHnV, 0, sizeof(m_stUsvGoalHnV));


	//Usv Setting
	//this code is typed for real test

	m_stUSVMotion.stdSOG	= 	mNavigationInfo.Velocity;
	m_stUSVMotion.stdCOG	=	mNavigationInfo.Heading;
	mCCoordinateConv.WGS2UTM(mNavigationInfo.Latitude, mNavigationInfo.Longitude);
	m_stUSVMotion.stdUTMN	=	mCCoordinateConv.getUTMN();
	m_stUSVMotion.stdUTME	=	mCCoordinateConv.getUTME();

	m_mutex.lock();

	for(int i = 0; i < DEF_NUM_WP; i++)
	{
		mCCoordinateConv.WGS2UTM(mMissionRecordInfo.Waypoint[i].Latitude, mMissionRecordInfo.Waypoint[i].Longitude);
		m_stWPInfo.stnWPsize			= (int) mMissionRecordInfo.MissionPointCount		;
		m_stWPInfo.stdWPUTMN[i]			= mCCoordinateConv.getUTMN()				;
		m_stWPInfo.stdWPUTME[i]			= mCCoordinateConv.getUTME()				;
		m_stWPInfo.stdWPMaxVel[i]		= mMissionRecordInfo.Waypoint[i].Velocity	;
		m_stWPInfo.stdWPAcceptRadius[i]	= mMissionRecordInfo.PathRange				;
	}
	m_mutex.unlock();

	cout << "/********/ PF_MissionPointCount : " << mMissionRecordInfo.MissionPointCount<< endl;
	// for(int i = 0; i < DEF_NUM_TARGET; i++)
	// {
	// 	gCCoordinateConv.WGS2UTM(mMissionRecordInfo.Waypoint[i].Latitude, mMissionRecordInfo.Waypoint[i].Longitude);
	// 	m_stTargetMotion.stdUTMN[i] = mCCoordinateConv.getUTMN();
	// 	m_stTargetMotion.stdUTME[i] = mCCoordinateConv.getUTME();
	// 	m_stTargetMotion.stdCOG[i] 	= mFlexTargetInfo.TargetInfo[i].Course;
	// 	m_stTargetMotion.stdSOG[i] 	= mFlexTargetInfo.TargetInfo[i].Velocity;
	// 	m_stTargetMotion.stdDISTANCE[i] = sqrt(pow(m_stUSVMotion.stdUTME - m_stTargetMotion.stdUTME[i], 2) + pow(m_stUSVMotion.stdUTMN - m_stTargetMotion.stdUTMN[i], 2));
	// }

	/******************************************************************/
	//check wp
	/******************************************************************/
	
	if(mUSVModeCommand.ModeDetail == 3 && mUSVModeCommand.AlgSelect == 2) // 운용모드 확인 후 자율 운항 시작
	{
		double dutme_pow = pow( (m_stUSVMotion.stdUTME - m_stWPInfo.stdWPUTME[m_stWPInfo.stnWPnum]), 2);
		double dutmn_pow = pow( (m_stUSVMotion.stdUTMN - m_stWPInfo.stdWPUTMN[m_stWPInfo.stnWPnum]), 2);
		double distance_usv2wp = sqrt(dutme_pow + dutmn_pow);

		cout <<"distance_usv2wp : " << distance_usv2wp << endl;
		cout <<"m_stWPInfo.stdWPAcceptRadius[m_stWPInfo.stnWPnum] : " << m_stWPInfo.stdWPAcceptRadius[m_stWPInfo.stnWPnum] << endl;
		cout <<"m_stWPInfo.stnWPnum : " << m_stWPInfo.stnWPnum << endl;

		if (m_stWPInfo.stdWPAcceptRadius[m_stWPInfo.stnWPnum] > distance_usv2wp)//check AcceptRadius
		{
		m_stWPInfo.stnWPnum++;
		if (m_stWPInfo.stnWPnum >= m_stWPInfo.stnWPsize)//check last wp.
			exit(0);
		}

		/******************************************************************/
		//LOS, Obstacle avoidance
		/******************************************************************/

		stUsvGoalHnV temp_stUsvGoalHnV;
		int obs_flag=0;

		if(obs_flag == 0)
		{
		temp_stUsvGoalHnV = LOS_Tracking(m_stWPInfo, m_stUSVMotion);
		Obs_avoidance_flag = 0;
		}

		memcpy(&m_stUsvGoalHnV, &temp_stUsvGoalHnV, sizeof(m_stUsvGoalHnV));

		DataLogging(m_stWPInfo, m_stUSVMotion, m_stTargetMotion, m_stUsvGoalHnV, distance_usv2wp);

		memset(&Buffer, 0 , sizeof(Buffer));
		//this code is typed for real test
		mAutoNaviStatusInfo.DestinationPathID	= m_stWPInfo.stnWPnum;
		if(obs_flag == 0)
		mAutoNaviStatusInfo.AutoNaviStatus	= 0;
		else if(obs_flag == 1)
		mAutoNaviStatusInfo.AutoNaviStatus	= 3;
		mAutoNaviStatusInfo.Heading				= (unsigned short) m_stUsvGoalHnV.heading * 100;
		mAutoNaviStatusInfo.Velocity			= (unsigned short) m_stUsvGoalHnV.velocity * 100;
		mAutoNaviStatusInfo.FailStatus			= 0;	
	}//if(mUSVModeCommand.ModeDetail == 3) // 운용모드 확인 후 자율 운항 시작
	
	return mAutoNaviStatusInfo;
}

stUsvGoalHnV Heading_LimitForSimulator(stUsvGoalHnV m_stUsvGoalHnV, stUsvGoalHnV pre_m_stUsvGoalHnV)
{
	stUsvGoalHnV result_stUsvGoalHnV;

	double heading_temp = m_stUsvGoalHnV.heading;
	double heading_diff = 0;

	if( heading_temp < pre_m_stUsvGoalHnV.heading)
		heading_temp = heading_temp + 360.0;

	heading_diff = heading_temp - pre_m_stUsvGoalHnV.heading;

	if( ( heading_diff > 180.0) && (heading_diff < (360.0 - m_pf_init.HEADING_LIMIT) ) )
	{
		result_stUsvGoalHnV.heading = pre_m_stUsvGoalHnV.heading - m_pf_init.HEADING_LIMIT;
	}
	else if( ( heading_diff < 180.0 ) && ( heading_diff > m_pf_init.HEADING_LIMIT) )
	{
		result_stUsvGoalHnV.heading = pre_m_stUsvGoalHnV.heading + m_pf_init.HEADING_LIMIT;
	}
	else
	{
		result_stUsvGoalHnV.heading = m_stUsvGoalHnV.heading;
	}

	if(result_stUsvGoalHnV.heading >= 360.0)
		result_stUsvGoalHnV.heading = result_stUsvGoalHnV.heading - 360.0;
	else if(result_stUsvGoalHnV.heading < 0.0)
		result_stUsvGoalHnV.heading = result_stUsvGoalHnV.heading + 360.0;


	result_stUsvGoalHnV.velocity = m_stUsvGoalHnV.velocity;

	return result_stUsvGoalHnV;
}

stUsvGoalHnV LOS_Tracking(stWPInfo m_stWPInfo, stUSVMotion m_stUSVMotion)
{
	stUsvGoalHnV m_stUsvGoalHnV;
	m_stUsvGoalHnV.heading = 0;
	m_stUsvGoalHnV.velocity = 0;

	double dx = m_stWPInfo.stdWPUTME[m_stWPInfo.stnWPnum] - m_stUSVMotion.stdUTME;
	double dy = m_stWPInfo.stdWPUTMN[m_stWPInfo.stnWPnum] - m_stUSVMotion.stdUTMN;

	double distance = sqrt((dx * dx) + (dy * dy));
	double height = dy / distance;

	double sin_value = asin( height ) * DEF_RAD2DEG;
	double tan_value = atan(dy/ dx) * DEF_RAD2DEG;
	double tan2_value = atan2(dy, dx) * DEF_RAD2DEG;
	
	cout << "tan2_value : " << tan2_value << endl;

	double m_heading = 0;

	// 반시계 방향
	// if (sin_value > 0 && tan_value > 0) //1 quadrant
	// 	m_heading = sin_value + 270;
	// else if (sin_value > 0 && tan_value < 0) // 2 quadrant
	// 	m_heading = 90 - sin_value;
	// else if (sin_value < 0 && tan_value > 0) // 3 quadrant
	// 	m_heading = - sin_value + 90;
	// else if (sin_value < 0 && tan_value < 0) // 4 quadrant
	// 	m_heading = sin_value + 270;

	// 시계 방향

	if(tan2_value >= 0 && tan2_value < 90) // 1 quadrant
		m_heading = (90 - tan2_value);
	else if(tan2_value >= 90 && tan2_value <= 180)//2 quadrant
		m_heading = 360 - (tan2_value - 90);
	else if(tan2_value < 0 && tan2_value >= -180) // 4 quadrant, 음의 각도 출력됨.
		m_heading = 90 - tan2_value;

	if(m_heading >= 360)
		m_heading = m_heading - 360;
	else if(m_heading < 0)
		m_heading = m_heading + 360;

	m_stUsvGoalHnV.heading = m_heading;
	m_stUsvGoalHnV.velocity = m_stWPInfo.stdWPMaxVel[m_stWPInfo.stnWPnum];


	return m_stUsvGoalHnV;
}

stUsvGoalHnV ObstacleAvoidance(stUSVMotion m_stUSVMotion, stTargetMotion m_stTargetMotion)
{
	/***************************************************************************************/
		// need to update about 
		// F_usv == sog of usv, unit : meter
		// F_obs == sog of usv + sog of usv, unit : meter
		// usv_theta == ?
	/***************************************************************************************/
	stUsvGoalHnV m_stUsvGoalHnV;
	m_stUsvGoalHnV.heading = 0;
	m_stUsvGoalHnV.velocity = 0;


	/***************************************************************************************/
	//y coordinate is usv_heading
	//obs mark needs to change
	/***************************************************************************************/
	double dx[DEF_NUM_TARGET];
	double dy[DEF_NUM_TARGET];
	double distance_usv2obs[DEF_NUM_TARGET];

	double F_obs_x[DEF_NUM_TARGET];
	double F_obs_y[DEF_NUM_TARGET];
	
	// for( int i = 0; i < m_stTargetMotion.stnNumObj; i++)
	// {
	// 	if(m_stTargetMotion.stdCOG[i] < m_pf_init.COLEGS_RULE_RADIUS)
	// 	{
	// 		m_stTargetMotion.stdCOG[i] = - m_stTargetMotion.stdCOG[i]; 
	// 	}
	// }

	for( int i = 0; i < m_stTargetMotion.stnNumObj; i++)
	{
		dx[i] = m_stTargetMotion.stdUTME[i] - m_stUSVMotion.stdUTME;
		dy[i] = m_stTargetMotion.stdUTMN[i] - m_stUSVMotion.stdUTMN;
		distance_usv2obs[i] = sqrt(dx[i] * dx[i] + dy[i] * dy[i]);

		F_obs_x[i] = -(m_stUSVMotion.stdSOG * mt * sin(m_stTargetMotion.stdCOG[i] * DEF_DEG2RAD)  / distance_usv2obs[i]) * m_pf_init.PF_CONSTANT_PARAMETER;
		F_obs_y[i] = -(m_stUSVMotion.stdSOG * mt * cos(m_stTargetMotion.stdCOG[i] * DEF_DEG2RAD)  / distance_usv2obs[i]) * m_pf_init.PF_CONSTANT_PARAMETER;
	}

	double F_usv_x = 0;
	double F_usv_y = m_stUSVMotion.stdSOG * mt;
	double F_usv_new[2];

	F_usv_new[0] = F_usv_x;
	F_usv_new[1] = F_usv_y;

	for( int i =0; i < m_stTargetMotion.stnNumObj; i++)
	{
		F_usv_new[0] += F_obs_x[i];
		F_usv_new[1] += F_obs_y[i];
	}

	// // atan2 default is  atan2(y,x);
	double theta_usv_next = atan2(F_usv_new[0], F_usv_new[1]) * DEF_RAD2DEG;
	double heading_temp = m_stUSVMotion.stdCOG + theta_usv_next;

	if(heading_temp > 360)
		heading_temp = heading_temp - 360;
	else if(heading_temp < 0)
		heading_temp = heading_temp + 360;
		
	m_stUsvGoalHnV.heading = heading_temp;
	m_stUsvGoalHnV.velocity = m_stUSVMotion.stdSOG;

	return m_stUsvGoalHnV;


	/***************************************************************************************/
	//obstacle is the one.
	/***************************************************************************************/
	// double dx = m_stTargetMotion.stdUTME[0] - m_stUSVMotion.stdUTME;
	// double dy = m_stTargetMotion.stdUTMN[0] - m_stUSVMotion.stdUTMN;
	// double distance_usv2obs = sqrt(dx * dx + dy * dy);


	// double F_obs_x = -(m_stUSVMotion.stdSOG * mt * sin(m_stTargetMotion.stdCOG[0] * DEF_DEG2RAD)  / distance_usv2obs) * m_pf_init.PF_CONSTANT_PARAMETER;
	// double F_obs_y = -(m_stUSVMotion.stdSOG * mt * cos(m_stTargetMotion.stdCOG[0] * DEF_DEG2RAD)  / distance_usv2obs) * m_pf_init.PF_CONSTANT_PARAMETER;
	// double F_usv_x = 0;
	// double F_usv_y = m_stUSVMotion.stdSOG * mt;
	// double F_usv_new[2] = { F_obs_x + F_usv_x, F_obs_y + F_usv_y };


	// // atan2 default is  atan2(y,x);
	// double theta_usv_next = atan2(F_usv_new[0], F_usv_new[1]) * DEF_RAD2DEG;
	// double heading_temp = m_stUSVMotion.stdCOG + theta_usv_next;

	// if(heading_temp > 360)
	// 	heading_temp = heading_temp - 360;
	// else if(heading_temp < 0)
	// 	heading_temp = heading_temp + 360;
		
	// m_stUsvGoalHnV.heading = heading_temp;
	// m_stUsvGoalHnV.velocity = m_stUSVMotion.stdSOG;

	// return m_stUsvGoalHnV;
	/***************************************************************************************/
	//obstacle is the one.
	/***************************************************************************************/

}

void DataLogging(stWPInfo m_stWPInfo, stUSVMotion m_stUSVMotion, stTargetMotion m_stTargetMotion, stUsvGoalHnV m_stUsvGoalHnV ,double distance_usv2wp)
{

	AlgLoggingFile <<

	m_stUSVMotion.stdUTME << "," << m_stUSVMotion.stdUTMN << "," <<
	m_stUSVMotion.stdCOG << "," << m_stUSVMotion.stdSOG << "," <<
	m_stUsvGoalHnV.heading << "," << m_stUsvGoalHnV.velocity << "," <<
	m_stWPInfo.stnWPnum << "," << m_stWPInfo.stdWPMaxVel[m_stWPInfo.stnWPnum] << "," <<
	m_stTargetMotion.stdCOG[0] << "," << m_stTargetMotion.stdSOG[0] << "," <<
	distance_usv2wp << "," << Obs_avoidance_flag <<

	"\n";
}

void DataLogging_Init(void)
{
	timer = time(NULL);
	t = localtime(&timer);

	string test = ".\\SystemCtrl_Log\\LOS_PF_";
	test += std::to_string(t->tm_year + 1900);
	test += "_";
	test += std::to_string(t->tm_mon + 1);
	test += "_";
	test += std::to_string(t->tm_mday + 1);
	test += "_";
	test += std::to_string(t->tm_hour);
	test += "_";
	test += std::to_string(t->tm_min);
	test += "_";
	test += std::to_string(t->tm_sec);
	test += ".txt";

	AlgLoggingFile.open(test);
	AlgLoggingFile.setf(ios::fixed);
	AlgLoggingFile.setf(ios::showpoint);
	AlgLoggingFile.precision(6);

	AlgLoggingFile <<

	"m_stUSVMotion.stdUTME" << "," << "m_stUSVMotion.stdUTMN" << "," <<
	"m_stUSVMotion.stdCOG" << "," << "m_stUSVMotion.stdSOG" << "," <<
	"m_stUsvGoalHnV.heading" << "," << "m_stUsvGoalHnV.velocity" << "," <<
	"m_stWPInfo.stnWPnum" << "," << "m_stUSVMotion.stdWPMaxVel" << "," <<

	"distance_usv2wp" << "," << "Obs_avoidance_flag" << "," <<
	"m_stTargetMotion.stdCOG[0]" << "," << "m_stTargetMotion.stdSOG[0]" << "," <<
	"\n";

}
void Setting_Init(void)
{
	//char* cBuf = NULL; cBuf = (char*)malloc(sizeof(char) * 256);
	//memset(cBuf, 0x00, sizeof(cBuf));
	//GetPrivateProfileString("ALGORITHM_SETTING", "avoidance_radius", "-", cBuf, 256, "C:\Users\User\Documents\Visual Studio 2015\Projects\USV_AS_OA\setting.ini");

	//printf("%s \n", cBuf);
	//m_pf_init.avoidance_radius = stod(cBuf);
	//printf("%f \n", m_pf_init.avoidance_radius);

	m_pf_init.AVOIDANCE_RADIUS = 100.0;//탐지거리 100m부터는 장애물 회피 시작하게 하는 파라미터
	m_pf_init.PF_CONSTANT_PARAMETER = 20.0; // 장애물 탐지 반경?
	m_pf_init.HEADING_LIMIT = 30.0;
	
	
	m_pf_init.COLEGS_RULE_RADIUS = -2.5;//우회전(콜렉스 룰)을 반영하기 위한 각도, -2.5도 보다 작은 각도는 우회전으로 회피
	// 오른쪽이 우회전(+), 왼쪽이 좌회전(-)

	//Default COLEGS RULE is Right turn the vehicle avoid obstacle.
	//m_pf_init.COLEGS_RULE_RADIUS is the angle limit when that vehicle avoid obstacle turn right
}


float velocity_noise(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-1, 1);
	return dis(gen);
}

float degree_noise(void)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(-3, 3);
	return dis(gen);
}
