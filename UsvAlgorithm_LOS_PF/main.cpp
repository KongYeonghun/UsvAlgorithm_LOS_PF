
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
	
	Setting_Init();

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
	CurrentTime						mCurrentTime;

	memset(&mUSVModeCommand, 0, sizeof(mUSVModeCommand));
	memset(&mFlexTargetInfo, 0, sizeof(mFlexTargetInfo));

	timer = time(NULL);
	t = localtime(&timer);

	string test = ".\\SystemCtrl_Log\\LOS_PF_DataLogging";
	test += std::to_string(t->tm_year + 1900);
	test += "_";
	test += std::to_string(t->tm_mon + 1);
	test += "_";
	test += std::to_string(t->tm_mday);
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
	"Time"					<< "	" <<
	"USVMotion.stdUTME" 	<< "	" << 
	"USVMotion.stdUTMN" 	<< "	" <<
	"USVMotion.stdCOG" 		<< "	" << 
	"USVMotion.stdSOG" 		<< "	" <<
	"UsvGoalHnV.heading" 	<< "	" << 
	"UsvGoalHnV.velocity" 	<< "	" <<
	"WPInfo.stnWPnum" 		<< "	" << 
	"distance_usv2wp" 			<< "	" << 
	"Obs_avoidance_flag" 		<< "	" <<
	endl;

	int count = 0;

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
				m_mutex.lock();

				// memcpy(&gMissionRecordInfo, &mMissionRecordInfo, sizeof(mMissionRecordInfo));

				m_mutex.unlock();
				// cout << "/**************** MissionPointCount ******************/: " << mMissionRecordInfo.MissionPointCount << endl;
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

/********************************************potential field start*******************************************************/
		stWPInfo		m_stWPInfo;
		stUSVMotion		m_stUSVMotion;
		stTargetMotion	m_stTargetMotion;
		stUsvGoalHnV	m_stUsvGoalHnV;
		stUsvGoalHnV	pre_m_stUsvGoalHnV;

		CCoordinateConv 				mCCoordinateConv;
		CCoordinateConv 				target_mCCoordinateConv;

		memset(&m_stTargetMotion	,0, sizeof(m_stTargetMotion)	);
		memset(&m_stUsvGoalHnV		,0, sizeof(m_stUsvGoalHnV)		);
		memset(&m_stUSVMotion		,0, sizeof(m_stUSVMotion)		);

		//Usv Setting
		//this code is typed for real test

		m_stUSVMotion.stdSOG	= 	mNavigationInfo.Velocity * 0.01;
		m_stUSVMotion.stdCOG	=	mNavigationInfo.Heading * 0.01;
		mCCoordinateConv.WGS2UTM(mNavigationInfo.Latitude, mNavigationInfo.Longitude);
		m_stUSVMotion.stdUTMN	=	mCCoordinateConv.getUTMN();
		m_stUSVMotion.stdUTME	=	mCCoordinateConv.getUTME();

		for(int i = 0; i < DEF_NUM_WP; i++)
		{
			mCCoordinateConv.WGS2UTM(mMissionRecordInfo.Waypoint[i].Latitude, mMissionRecordInfo.Waypoint[i].Longitude);
			m_stWPInfo.stnWPsize			= (int) mMissionRecordInfo.MissionPointCount;
			m_stWPInfo.stdWPUTMN[i]			= mCCoordinateConv.getUTMN()				;
			m_stWPInfo.stdWPUTME[i]			= mCCoordinateConv.getUTME()				;
			m_stWPInfo.stdWPMaxVel[i]		= mMissionRecordInfo.Waypoint[i].Velocity	;
			m_stWPInfo.stdWPAcceptRadius[i]	= mMissionRecordInfo.PathRange				;
		}

		cout << "/********/ PF_MissionPointCount : " << mMissionRecordInfo.MissionPointCount<< endl;
		
		m_stTargetMotion.stnNumObj = mFlexTargetInfo.TargetCount;
		cout << "m_stTargetMotion.stnNumObj : [ " << m_stTargetMotion.stnNumObj << " ] "<<endl;
		double dx[DEF_NUM_TARGET] , dy[DEF_NUM_TARGET];
		double tan2_value[DEF_NUM_TARGET];
		memset(&dx, 0, sizeof(dx));
		memset(&dy, 0 ,sizeof(dy));

		for(int i = 0; i < mFlexTargetInfo.TargetCount; i++)
		{
			target_mCCoordinateConv.WGS2UTM(mFlexTargetInfo.TargetInfo[i].Latitude, mFlexTargetInfo.TargetInfo[i].Longitude);
			m_stTargetMotion.stdUTMN[i] = target_mCCoordinateConv.getUTMN();
			m_stTargetMotion.stdUTME[i] = target_mCCoordinateConv.getUTME();

			dx[i] =  m_stTargetMotion.stdUTME[i]-m_stUSVMotion.stdUTME ;
			dy[i] =  m_stTargetMotion.stdUTMN[i]-m_stUSVMotion.stdUTMN ;

			tan2_value[i] = atan2(dy[i] , dx[i])  * DEF_RAD2DEG;

			if(tan2_value[i] >= 0 && tan2_value[i] < 90) // 1 quadrant
				m_stTargetMotion.stdCOG[i] = (90 - tan2_value[i]);
			else if(tan2_value[i] >= 90 && tan2_value[i] <= 180)//2 quadrant
				m_stTargetMotion.stdCOG[i] = 360 - (tan2_value[i] - 90);
			else if(tan2_value[i] < 0 && tan2_value[i] >= -180) // 3, 4 quadrant, 음의 각도 출력됨.
				m_stTargetMotion.stdCOG[i] = 90 - tan2_value[i];

			m_stTargetMotion.stdDISTANCE[i] = sqrt( pow(m_stUSVMotion.stdUTME - m_stTargetMotion.stdUTME[i], 2) + pow(m_stUSVMotion.stdUTMN - m_stTargetMotion.stdUTMN[i], 2) );
			cout << "m_stTargetMotion.stdDISTANCE[i] : " << (double) m_stTargetMotion.stdDISTANCE[i] << endl;
			cout << "m_stTargetMotion.stdCOG[i] : " << (double) m_stTargetMotion.stdCOG[i] << endl;
		}

		/******************************************************************/
		//check wp
		/******************************************************************/

		double distance_usv2wp = 0;
		if(mUSVModeCommand.ModeDetail == 3 && mUSVModeCommand.AlgSelect == 2) // 운용모드 확인 후 자율 운항 시작
		{
			double dutme_pow = pow( (m_stUSVMotion.stdUTME - m_stWPInfo.stdWPUTME[m_stWPInfo.stnWPnum]), 2);
			double dutmn_pow = pow( (m_stUSVMotion.stdUTMN - m_stWPInfo.stdWPUTMN[m_stWPInfo.stnWPnum]), 2);
			distance_usv2wp = sqrt(dutme_pow + dutmn_pow);

			cout <<"Distance_usv2wp : [ " << distance_usv2wp  <<" ] , Waypoint Number : [ " << m_stWPInfo.stnWPnum << " ]"<< endl;

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
			int obs_flag = 0;

			for (int ii = 0; ii < m_stTargetMotion.stnNumObj; ii++ )
			{
				if( m_stTargetMotion.stdDISTANCE[ii] < m_pf_init.AVOIDANCE_RADIUS)//회피를 시작하는 반경에 들어오는 장애물이 있다면 장애물 회피 시작
				{
					obs_flag = 1;
					break;
				}
			}
			
			if(obs_flag == 0)
			{
				m_stUsvGoalHnV = LOS_Tracking(m_stWPInfo, m_stUSVMotion);
				Obs_avoidance_flag = 0;
			}
			else
			{
				cout << "ObstacleAvoidance Start" << endl;
				m_stUsvGoalHnV = ObstacleAvoidance(m_stWPInfo, m_stUSVMotion, m_stTargetMotion);
			}
			
			//this code is typed for real test

			memset(&mAutoNaviStatusInfo , 0 ,sizeof(mAutoNaviStatusInfo));
			mAutoNaviStatusInfo.DestinationPathID	= m_stWPInfo.stnWPnum;
			
			if(obs_flag == 0)
				mAutoNaviStatusInfo.AutoNaviStatus	= 0;
			else if(obs_flag == 1)
				mAutoNaviStatusInfo.AutoNaviStatus	= 3;

			mAutoNaviStatusInfo.Heading				= (unsigned short) m_stUsvGoalHnV.heading * 100;
			mAutoNaviStatusInfo.Velocity			= (unsigned short) m_stUsvGoalHnV.velocity * 100;
			mAutoNaviStatusInfo.FailStatus			= 0;	
		}//if(mUSVModeCommand.ModeDetail == 3) // 운용모드 확인 후 자율 운항 시작

/********************************************potential field start*******************************************************/
		memset(&Buffer, 0 , sizeof(Buffer));
		cout << "mUSVModeCommand.AlgSelect : "   << (int)mUSVModeCommand.AlgSelect << endl;

		if (mUSVModeCommand.ModeDetail == 3 && mUSVModeCommand.AlgSelect == 2) // 운용모드 확인 후 자율 운항 시작
		{
			cout << "LOS_PF_heading : [ " << mAutoNaviStatusInfo.Heading << " ] , LOS_PF_velocity : [ " << mAutoNaviStatusInfo.Velocity << " ] " << endl;
			count++;
			if(count > 10)
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.HeaderSetting(&mHeader, 0x27, 0x21, 0x1501, sizeof(mAutoNaviStatusInfo), 0);
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageCombination(Buffer, mHeader, mAutoNaviStatusInfo);
				ServerSystemCtrl_UsvAlgorithm_Socket.ClientToServer_Send(Buffer, ServerSystemCtrl_UsvAlgorithm_Socket, COMM_HEADER_SIZE + mHeader.Payload_Length);
				count = 0 ;

				timer = time(NULL);
				t = localtime(&timer);

				mCurrentTime.year 	= t->tm_year + 1900;
				mCurrentTime.month 	= t->tm_mon + 1;
				mCurrentTime.day 	= t->tm_mday;
				mCurrentTime.hour 	= t->tm_hour;
				mCurrentTime.day 	= t->tm_min;
				mCurrentTime.sec 	= t->tm_sec;

				AlgLoggingFile 						<<
				(int)		mCurrentTime.year 		<< "-" << 			
				(int)		mCurrentTime.month 		<< "-" << 		
				(int)		mCurrentTime.day 		<< "_" << 		
				(int)		mCurrentTime.hour 		<< ":" << 		
				(int)		mCurrentTime.day 		<< ":" << 		
				(int)		mCurrentTime.sec 		<< "	" << 			
				(double)	m_stUSVMotion.stdUTME 	<< "	" << 
				(double)	m_stUSVMotion.stdUTMN 	<< "	" <<
				(double)	m_stUSVMotion.stdCOG 	<< "	" << 
				(double)	m_stUSVMotion.stdSOG 	<< "	" <<
				(double)	m_stUsvGoalHnV.heading 	<< "	" << 
				(double)	m_stUsvGoalHnV.velocity << "	" <<
				(int)		m_stWPInfo.stnWPnum 	<< "	" <<
				(double)	distance_usv2wp 		<< "	" << 
				(int)		Obs_avoidance_flag 		<<
				endl;
			}
		}
	}//while
	AlgLoggingFile.close();
	return 0;
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

	double m_heading = 0;

	// 시계 방향
	if(tan2_value >= 0 && tan2_value < 90) // 1 quadrant
		m_heading = (90 - tan2_value);
	else if(tan2_value >= 90 && tan2_value <= 180)//2 quadrant
		m_heading = 360 - (tan2_value - 90);
	else if(tan2_value < 0 && tan2_value >= -180) // 3, 4 quadrant, 음의 각도 출력됨.
		m_heading = 90 - tan2_value;

	if(m_heading >= 360)
		m_heading = m_heading - 360;
	else if(m_heading < 0)
		m_heading = m_heading + 360;

	m_stUsvGoalHnV.heading = m_heading;
	m_stUsvGoalHnV.velocity = m_stWPInfo.stdWPMaxVel[m_stWPInfo.stnWPnum];

	return m_stUsvGoalHnV;
}

stUsvGoalHnV ObstacleAvoidance(stWPInfo m_stWPInfo, stUSVMotion m_stUSVMotion, stTargetMotion m_stTargetMotion) //Colregs 룰은 제외되어 있음.
{
	stUsvGoalHnV m_stUsvGoalHnV;
	memset(&m_stUsvGoalHnV, 0 ,sizeof(m_stUsvGoalHnV));

	double dx[DEF_NUM_TARGET];
	double dy[DEF_NUM_TARGET];
	double distance_usv2obs[DEF_NUM_TARGET];

	double PF_obs_x = 0 , PF_obs_y = 0;

	double RelativeBearing_dev = 360.0 - m_stUSVMotion.stdCOG;

	double RelativeBearing[DEF_NUM_TARGET];
	memset(&RelativeBearing, 0 ,sizeof(RelativeBearing));

	for( int i = 0; i < m_stTargetMotion.stnNumObj; i++)//USV의 헤딩을 기준으로 한 상대 방위로 타겟의 방위를 변경.
	{	
		dx[i] = m_stTargetMotion.stdUTME[i] - m_stUSVMotion.stdUTME;
		dy[i] = m_stTargetMotion.stdUTMN[i] - m_stUSVMotion.stdUTMN;

		double tan2_value = atan2(dy[i], dx[i]) * DEF_RAD2DEG;
		//tan2_value 가 양수, 1,4 사분면, 1 <-> 3 사분면 / 2 < - > 4 사분면

		if(tan2_value >= 0)// USV의 전방에 있는 장애물만 식별 후 Potential Field 적용 및 상대 방위 출력_221114
		{
			RelativeBearing[i] = m_stTargetMotion.stdCOG[i] + RelativeBearing_dev;
		
			if(RelativeBearing[i] >= 360)
				RelativeBearing[i] = RelativeBearing[i] - 360.0;

			cout << "RelativeBearing[i] : " << (double) RelativeBearing[i] << endl;
			distance_usv2obs[i] = sqrt(dx[i] * dx[i] + dy[i] * dy[i]);
			PF_obs_x += m_pf_init.AVOIDANCE_RADIUS / distance_usv2obs[i] * cos(RelativeBearing[i] * DEF_DEG2RAD); // 장애물회피를 시작하는 거리 / usv와 장애물간 거리 * cosine (상대방위)
			PF_obs_y += m_pf_init.AVOIDANCE_RADIUS / distance_usv2obs[i] * sin(RelativeBearing[i] * DEF_DEG2RAD); // 장애물회피를 시작하는 거리 / usv와 장애물간 거리 * sine (상대방위)
		}
	}

	cout << "PF_obs_x : " << PF_obs_x << " , PF_obs_y : " << PF_obs_y << endl;

	double PF_obs_tan2_value = atan2(PF_obs_y, PF_obs_x) * DEF_RAD2DEG; // PF가 적용되어 하나가 된 장애물의 방위 확인
	cout << "PF_obs_tan2_value : " << (double) PF_obs_tan2_value << endl;
	double PF_heading_dev = 0;
	
	if(PF_obs_tan2_value >= 90.0)
		PF_heading_dev = m_pf_init.AVOIDANCE_ANGLE - (PF_obs_tan2_value - 90);
	else if(PF_obs_tan2_value < 90.0)
		PF_heading_dev =  - m_pf_init.AVOIDANCE_ANGLE + (90 - PF_obs_tan2_value );

	m_stUsvGoalHnV.heading = m_stUSVMotion.stdCOG + PF_heading_dev;

	if(m_stUsvGoalHnV.heading >= 360)	
		m_stUsvGoalHnV.heading = m_stUsvGoalHnV.heading - 360.0;
	else if(m_stUsvGoalHnV.heading < 0)
		m_stUsvGoalHnV.heading = m_stUsvGoalHnV.heading + 360.0;

	cout << "PF heading : " << m_stUsvGoalHnV.heading << endl;

	m_stUsvGoalHnV.velocity = m_stWPInfo.stdWPMaxVel[m_stWPInfo.stnWPnum];

	return m_stUsvGoalHnV;
}

void Setting_Init(void)
{
	//char* cBuf = NULL; cBuf = (char*)malloc(sizeof(char) * 256);
	//memset(cBuf, 0x00, sizeof(cBuf));
	//GetPrivateProfileString("ALGORITHM_SETTING", "avoidance_radius", "-", cBuf, 256, "C:\Users\User\Documents\Visual Studio 2015\Projects\USV_AS_OA\setting.ini");

	//printf("%s \n", cBuf);
	//m_pf_init.avoidance_radius = stod(cBuf);
	//printf("%f \n", m_pf_init.avoidance_radius);

	m_pf_init.AVOIDANCE_RADIUS = 150.0; //탐지거리 AVOIDANCE_RADIUS m부터는 장애물 회피 시작하게 하는 파라미터
	m_pf_init.AVOIDANCE_ANGLE = 40.0;  // 장애물 회피시 회피하는 각도 ex) usv가 장애물을 정면으로 바로볼때 40도로 우측 회피
	m_pf_init.HEADING_LIMIT = 30.0;
	
	
	m_pf_init.COLEGS_RULE_RADIUS = -2.5;//우회전(콜렉스 룰)을 반영하기 위한 각도, -2.5도 보다 작은 각도는 우회전으로 회피
	// 오른쪽이 우회전(+), 왼쪽이 좌회전(-)

	//Default COLEGS RULE is Right turn the vehicle avoid obstacle.
	//m_pf_init.COLEGS_RULE_RADIUS is the angle limit when that vehicle avoid obstacle turn right
}

