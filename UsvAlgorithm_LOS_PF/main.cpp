
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

	// thread CommunicationUDP(mCommunicationUDP);
	thread PotentialFieldAlgorithm(mPotentialFieldAlgorithm);

	// CommunicationUDP.join();
	PotentialFieldAlgorithm.join();

	return 0;
}

void mCommunicationUDP()
{
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

	while(1)
	{
		Sleep(100);//0.1s

		/******************************************************************/
		//WP,USV communication
		/******************************************************************/
		memset(Buffer, 0, BUFFER_SIZE);
		ServerSystemCtrl_UsvAlgorithm_Socket.ClientToServer_Recv(Buffer, ServerSystemCtrl_UsvAlgorithm_Socket);

		memcpy(&mHeader,(Header*)&Buffer, sizeof(mHeader));

		switch(mHeader.Message_ID)
		{
			case 0x1401:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mUSVModeCommand);

				m_mutex.lock();
				memcpy(&gUSVModeCommand, &mUSVModeCommand, sizeof(mUSVModeCommand));
				m_mutex.unlock();

				cout << "mUSVModeCommand.ModeDetail : " << mUSVModeCommand.ModeDetail << endl;
			}
			break;
			case 0x1402:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mMissionRecordInfo);

				m_mutex.lock();
				memcpy(&gMissionRecordInfo, &mMissionRecordInfo, sizeof(gMissionRecordInfo));
				m_mutex.unlock();

				cout << "mMissionRecordInfo.MissionPointCount : " << mMissionRecordInfo.MissionPointCount << endl;
			}
			break;
			case 0x1403:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mNavigationInfo);

				m_mutex.lock();
				memcpy(&gNavigationInfo, &mNavigationInfo, sizeof(gNavigationInfo));
				m_mutex.unlock();

				cout << "mNavigationInfo.Latitude : " << mNavigationInfo.Latitude << endl;
			}
			break;
			case 0x1404:
			{
				ServerSystemCtrl_UsvAlgorithm_Socket.MessageClassification(Buffer, &mFlexTargetInfo);

				m_mutex.lock();
				memcpy(&gFlexTargetInfo, &mFlexTargetInfo, sizeof(gFlexTargetInfo));
				m_mutex.unlock();

				cout << "mFlexTargetInfo.TargetCount : " << mFlexTargetInfo.TargetCount << endl;
			}
			break;
		}
		/******************************************************************/
		//WP,USV communication
		/******************************************************************/
		
		/******************************************************************/
		//WP,USV communication
		/******************************************************************/

		memset(Buffer, 0, BUFFER_SIZE);

		// m_mutex.lock();
		// ServerSystemCtrl_UsvAlgorithm_Socket.HeaderSetting(&mHeader, 0xff, 0xff, 0x1501, sizeof(AutoNaviStatusInfo), 0xff);
		// ServerSystemCtrl_UsvAlgorithm_Socket.MessageCombination(Buffer, mHeader, gAutoNaviStatusInfo);
		// ServerSystemCtrl_UsvAlgorithm_Socket.ClientToServer_Send(Buffer, ServerSystemCtrl_UsvAlgorithm_Socket, COMM_HEADER_SIZE + mHeader.Payload_Length);
		// m_mutex.unlock();
	}

	ServerSystemCtrl_UsvAlgorithm_Socket.UDP_CLIENT_CLOSE();
}

void mPotentialFieldAlgorithm()
{
	PotentailField();
}

int PotentailField()
{
	//ICD message
	//AutoSailing -> UsvAlgorithm
	Header						    mHeader;
	MissionRecordInfo			    mMissionRecordInfo;
	NavigationInfo				    mNavigationInfo;
	RadarTargetInfo				    mRadarTargetInfo;
	LidarTargetInfo				    mLidarTargetInfo;
	FlexTargetInfo				    mFlexTargetInfo;
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


	//this code is typed for simulator
	// m_stWPInfo.stnWPnum = 0;
	// m_stWPInfo.stnWPsize = 5;
	// // rhomsbus waypoint
	// m_stWPInfo.stdWPUTME[0] = 300;
	// m_stWPInfo.stdWPUTMN[0] = 100;

	// m_stWPInfo.stdWPUTME[1] = 300;
	// m_stWPInfo.stdWPUTMN[1] = 300;

	// m_stWPInfo.stdWPUTME[2] = 100;
	// m_stWPInfo.stdWPUTMN[2] = 300;

	// m_stWPInfo.stdWPUTME[3] = 100;
	// m_stWPInfo.stdWPUTMN[3] = 100;

	// m_stWPInfo.stdWPUTME[4] = 300;
	// m_stWPInfo.stdWPUTMN[4] = 300;

	// // m_stWPInfo.stdWPUTME[0] = 100;
	// // m_stWPInfo.stdWPUTMN[0] = 300;

	// // m_stWPInfo.stdWPUTME[1] = 300;
	// // m_stWPInfo.stdWPUTMN[1] = 500;

	// // m_stWPInfo.stdWPUTME[2] = 500;
	// // m_stWPInfo.stdWPUTMN[2] = 300;

	// // m_stWPInfo.stdWPUTME[3] = 300;
	// // m_stWPInfo.stdWPUTMN[3] = 100;

	// // m_stWPInfo.stdWPUTME[4] = 100;
	// // m_stWPInfo.stdWPUTMN[4] = 300;

	// // usv, obs HardCoding.
	// // m_stWPInfo.stdWPUTME[0] = 100;
	// // m_stWPInfo.stdWPUTMN[0] = 300;

	// // m_stWPInfo.stdWPUTME[1] = 300;
	// // m_stWPInfo.stdWPUTMN[1] = 300;

	// // m_stWPInfo.stdWPUTME[2] = 300;
	// // m_stWPInfo.stdWPUTMN[2] = 100;

	// // m_stWPInfo.stdWPUTME[3] = 100;
	// // m_stWPInfo.stdWPUTMN[3] = 100;

	// m_stWPInfo.stdWPMaxVel[0] = 10;//m/s
	// m_stWPInfo.stdWPMaxVel[1] = 10;//m/s
	// m_stWPInfo.stdWPMaxVel[2] = 10;//m/s
	// m_stWPInfo.stdWPMaxVel[3] = 10;//m/s
	// m_stWPInfo.stdWPMaxVel[4] = 10;//m/s

	// m_stWPInfo.stdWPAcceptRadius[0] = 20;
	// m_stWPInfo.stdWPAcceptRadius[1] = 20;
	// m_stWPInfo.stdWPAcceptRadius[2] = 20;
	// m_stWPInfo.stdWPAcceptRadius[3] = 20;
	// m_stWPInfo.stdWPAcceptRadius[4] = 20;

	// m_stUSVMotion.stdUTME = 100;
	// m_stUSVMotion.stdUTMN = 100;
	// m_stUSVMotion.stdSOG = 1;//m/s
	// m_stUSVMotion.stdCOG = 90;//degree

	// memset(&m_stTargetMotion, 0 ,sizeof(m_stTargetMotion));
	// m_stTargetMotion.stnNumObj = 20;

	// for(int i=0; i<10; i++)
	// {
	// 	m_stTargetMotion.stdUTME[i] = 100 + i*3;
	// 	m_stTargetMotion.stdUTMN[i] = 200;
	// }
	// for(int i=10; i<20; i++)
	// {
		
	// 	m_stTargetMotion.stdUTME[i] = 100 - i*3;
	// 	m_stTargetMotion.stdUTMN[i] = 200;
	// }
	//this code is typed for simulator	

	DataLogging_Init();


	char Buffer[BUFFER_SIZE];

	memset(&pre_m_stUsvGoalHnV, 0, sizeof(pre_m_stUsvGoalHnV));
	memset(&m_stUsvGoalHnV, 0, sizeof(m_stUsvGoalHnV));

	while(1)
	{
		// Sleep(1);//10ms

		//Usv Setting
		//this code is typed for real test
		m_mutex.lock();

		memcpy(&mNavigationInfo, &gNavigationInfo, sizeof(mNavigationInfo));
		mCCoordinateConv.WGS2UTM(mNavigationInfo.Latitude, mNavigationInfo.Longitude);
		m_stUSVMotion.stdUTMN	=	mCCoordinateConv.getUTMN();
		m_stUSVMotion.stdUTME	=	mCCoordinateConv.getUTME();
		memcpy(&mMissionRecordInfo, &gMissionRecordInfo, sizeof(mMissionRecordInfo));

		for(int i = 0; i < DEF_NUM_WP; i++)
		{
			mCCoordinateConv.WGS2UTM(mMissionRecordInfo.Waypoint[i].Latitude, mMissionRecordInfo.Waypoint[i].Longitude);
			// m_stWPInfo.stnWPsize			= (int) mMissionRecordInfo.MissionPointCount		;
			// m_stWPInfo.stdWPUTMN[i]			= mCCoordinateConv.getUTMN()				;
			// m_stWPInfo.stdWPUTME[i]			= mCCoordinateConv.getUTME()				;
			// m_stWPInfo.stdWPMaxVel[i]		= mMissionRecordInfo.Waypoint[i].Velocity	;
			// m_stWPInfo.stdWPAcceptRadius[i]	= mMissionRecordInfo.PathRange				;
		}
		
		// for(int i = 0; i < DEF_NUM_TARGET; i++)
		// {
		// 	gCCoordinateConv.WGS2UTM(mMissionRecordInfo.Waypoint[i].Latitude, mMissionRecordInfo.Waypoint[i].Longitude);
		// 	m_stTargetMotion.stdUTMN[i] = mCCoordinateConv.getUTMN();
		// 	m_stTargetMotion.stdUTME[i] = mCCoordinateConv.getUTME();
		// 	m_stTargetMotion.stdCOG[i] 	= mFlexTargetInfo.TargetInfo[i].Course;
		// 	m_stTargetMotion.stdSOG[i] 	= mFlexTargetInfo.TargetInfo[i].Velocity;
		// 	m_stTargetMotion.stdDISTANCE[i] = sqrt(pow(m_stUSVMotion.stdUTME - m_stTargetMotion.stdUTME[i], 2) + pow(m_stUSVMotion.stdUTMN - m_stTargetMotion.stdUTMN[i], 2));
		// }

		m_mutex.unlock();
		//obs info setting
		//this code is typed for real test

	/***************************************************************************************/
	//obstacle is the one.
	/***************************************************************************************/

		// /******************************************************************/
		// //WP,USV communication
		// /******************************************************************/
		//obs info setting
		//******************* This code is typed for simulator. ***************************//
		//For Potential field code
		// double dx = m_stTargetMotion.stdUTME[0] - m_stUSVMotion.stdUTME;
		// double dy = m_stTargetMotion.stdUTMN[0] - m_stUSVMotion.stdUTMN;
		// double distance_usv2obs = sqrt(dx * dx + dy * dy);
		// if (dy > 0)
		// {
		// 	// double theta_obs_global = atan2(dx, dy) * DEF_RAD2DEG;//atan2
		// 	double theta_obs_global = atan2(dy, dx) * DEF_RAD2DEG;//atan2
		// 	// if (theta_obs_global <= 0)
		// 	// 	theta_obs_global = -theta_obs_global;
		// 	// else
		// 	// 	theta_obs_global = 360 - theta_obs_global;

		// 	theta_obs_global = 90 - theta_obs_global;

		// 	// if(theta_obs_global > 0)
		// 	// 	theta_obs_global = -theta_obs_global;

		// 	// double theta_temp2=0;

		// 	// theta_temp2 = 360 - m_stUSVMotion.stdCOG + theta_obs_global;
		// 	// if (theta_temp2 >= 360)
		// 	// 	theta_temp2 = theta_temp2 - 360;
		// 	// else if(theta_temp2 < 0)
		// 	// 	theta_temp2 = theta_temp2 + 360;
		// 	// //change global coordinate to local coordinate(usv_heading)
		// 	// if (theta_temp2 > 180)
		// 	// 	theta_temp2 = -(360 - theta_temp2);

		// 	m_stTargetMotion.stdCOG[0] = theta_obs_global;
		// 	//set local coordinate(usv_heading), Clockwise is angle of minus, counterclockwise is angle of plus at local coordinate
		// }

		// cout << "m_stTargetMotion.stdCOG[0] : "<< m_stTargetMotion.stdCOG[0] << endl;

		// m_stTargetMotion.stdDISTANCE[0] = sqrt(pow(m_stUSVMotion.stdUTME - m_stTargetMotion.stdUTME[0], 2) + pow(m_stUSVMotion.stdUTMN - m_stTargetMotion.stdUTMN[0], 2));
		// m_stTargetMotion.stdDISTANCE[1] = sqrt(pow(m_stUSVMotion.stdUTME - m_stTargetMotion.stdUTME[1], 2) + pow(m_stUSVMotion.stdUTMN - m_stTargetMotion.stdUTMN[1], 2));
		//******************* Code written so far is typed for simulator. ***************************//

	/***************************************************************************************/
	//obstacle is the one.
	/***************************************************************************************/
		//usv info setting
		// //******************* This code is typed for simulator. ***************************//
		// double m_distance = m_stUsvGoalHnV.velocity * mt;
		// double m_theta = 0;

		// if(m_stUSVMotion.stdCOG >= 360)
		// 	m_stUSVMotion.stdCOG = m_stUSVMotion.stdCOG - 360;
		// else if(m_stUSVMotion.stdCOG < 0)
		// 	m_stUSVMotion.stdCOG = m_stUSVMotion.stdCOG + 360;
		
		// //시계 방향
		// if (m_stUsvGoalHnV.heading < 90 && m_stUsvGoalHnV.heading >= 0)//1 quadrant
		// {
		// 	m_theta = m_stUsvGoalHnV.heading * DEF_DEG2RAD;
		// 	m_stUSVMotion.stdUTMN = m_stUSVMotion.stdUTMN + m_distance * cos(m_theta);
		// 	m_stUSVMotion.stdUTME = m_stUSVMotion.stdUTME + m_distance * sin(m_theta);
		// }
		// else if (m_stUsvGoalHnV.heading < 180 && m_stUsvGoalHnV.heading >= 90)//4 quadrant
		// {
		// 	m_theta = (180 - m_stUsvGoalHnV.heading) * DEF_DEG2RAD;
		// 	m_stUSVMotion.stdUTME = m_stUSVMotion.stdUTME + m_distance * sin(m_theta);
		// 	m_stUSVMotion.stdUTMN = m_stUSVMotion.stdUTMN - m_distance * cos(m_theta);
		// }
		// else if (m_stUsvGoalHnV.heading < 270 && m_stUsvGoalHnV.heading >= 180)//3 quadrant
		// {
		// 	m_theta = (m_stUsvGoalHnV.heading - 180) * DEF_DEG2RAD;
		// 	m_stUSVMotion.stdUTMN = m_stUSVMotion.stdUTMN - m_distance * cos(m_theta);
		// 	m_stUSVMotion.stdUTME = m_stUSVMotion.stdUTME - m_distance * sin(m_theta);
		// }
		// else if (m_stUsvGoalHnV.heading < 360 && m_stUsvGoalHnV.heading >= 270)//2 quadrant
		// {
		// 	m_theta = (360 - m_stUsvGoalHnV.heading) * DEF_DEG2RAD;
		// 	m_stUSVMotion.stdUTMN = m_stUSVMotion.stdUTMN + m_distance * cos(m_theta);
		// 	m_stUSVMotion.stdUTME = m_stUSVMotion.stdUTME - m_distance * sin(m_theta);
		// }

		// m_stUSVMotion.stdSOG = m_stUsvGoalHnV.velocity + velocity_noise();//m/s
		// m_stUSVMotion.stdCOG = m_stUsvGoalHnV.heading + degree_noise();//degree

		// cout <<"m_stUSVMotion.stdSOG : " << m_stUSVMotion.stdSOG << endl;
		// cout <<"m_stUSVMotion.stdCOG : " << m_stUSVMotion.stdCOG << endl;

		// if(m_stUSVMotion.stdCOG >= 360)
		// 	m_stUSVMotion.stdCOG = m_stUSVMotion.stdCOG - 360;
		// else if(m_stUSVMotion.stdCOG < 0)
		// 	m_stUSVMotion.stdCOG = m_stUSVMotion.stdCOG + 360;

		//******************* Code written so far is typed for simulator. ***************************//

		/******************************************************************/
		//check wp
		/******************************************************************/
		
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
				return 0;
		}
	
		/******************************************************************/
		//LOS, Obstacle avoidance
		/******************************************************************/

		stUsvGoalHnV temp_stUsvGoalHnV;
		 int obs_flag=0;
		// for(int i=0; i<m_stTargetMotion.stnNumObj;i++)
		// {
		// 	if (m_pf_init.AVOIDANCE_RADIUS > m_stTargetMotion.stdDISTANCE[0] && m_stTargetMotion.stdCOG[0] < 30 && m_stTargetMotion.stdCOG[0] > -30 )// check to detect obstacle in the front
		// 	{
		// 		temp_stUsvGoalHnV = ObstacleAvoidance(m_stUSVMotion, m_stTargetMotion);
		// 		Obs_avoidance_flag = 1;
		// 		obs_flag = 1;
		// 	}
		// }
		if(obs_flag == 0)
		{
			temp_stUsvGoalHnV = LOS_Tracking(m_stWPInfo, m_stUSVMotion);
			Obs_avoidance_flag = 0;
		}
		/***************************************************************************************/
		//obstacle is the one.
		/***************************************************************************************/
		// stUsvGoalHnV temp_stUsvGoalHnV;
		// if (m_pf_init.avoidance_radius > m_stTargetMotion.stdDISTANCE[0] && m_stTargetMotion.stdCOG[0] < 30 && m_stTargetMotion.stdCOG[0] > -30 )// check to detect obstacle in the front
		// {
		// 	temp_stUsvGoalHnV = ObstacleAvoidance(m_stUSVMotion, m_stTargetMotion);
		// 	Obs_avoidance_flag = 1;
		// 	obs_flag = 1;
		// }
		// else
		// {
		// 	temp_stUsvGoalHnV = LOS_Tracking(m_stWPInfo, m_stUSVMotion);
		// 	Obs_avoidance_flag = 0;
		// }
		/***************************************************************************************/
		//obstacle is the one.
		/***************************************************************************************/
		// m_stUsvGoalHnV = Heading_LimitForSimulator(temp_stUsvGoalHnV, pre_m_stUsvGoalHnV);
		// memcpy(&pre_m_stUsvGoalHnV, &m_stUsvGoalHnV, sizeof(m_stUsvGoalHnV));

		memcpy(&m_stUsvGoalHnV, &temp_stUsvGoalHnV, sizeof(m_stUsvGoalHnV));

		DataLogging(m_stWPInfo, m_stUSVMotion, m_stTargetMotion, m_stUsvGoalHnV, distance_usv2wp);
		
		cout << "m_stUsvGoalHnV.heading : "		<< m_stUsvGoalHnV.heading 	<< endl;
		cout << "m_stUsvGoalHnV.velocity : "	<< m_stUsvGoalHnV.velocity 	<< endl;

		//this code is typed for real test
		m_mutex.lock();
		gAutoNaviStatusInfo.DestinationPathID	= m_stWPInfo.stnWPnum;
		if(obs_flag == 0)
			gAutoNaviStatusInfo.AutoNaviStatus	= 0;
		else if(obs_flag == 1)
			gAutoNaviStatusInfo.AutoNaviStatus	= 3;

		gAutoNaviStatusInfo.Heading				= m_stUsvGoalHnV.heading;
		gAutoNaviStatusInfo.Velocity			= m_stUsvGoalHnV.velocity;
		gAutoNaviStatusInfo.FailStatus			= 0;	

		ServerSystemCtrl_UsvAlgorithm_Socket.HeaderSetting(&mHeader, 0xff, 0xff, 0x1501, sizeof(AutoNaviStatusInfo), 0xff);
		ServerSystemCtrl_UsvAlgorithm_Socket.MessageCombination(Buffer, mHeader, gAutoNaviStatusInfo);
		ServerSystemCtrl_UsvAlgorithm_Socket.ClientToServer_Send(Buffer, ServerSystemCtrl_UsvAlgorithm_Socket, COMM_HEADER_SIZE + mHeader.Payload_Length);
		m_mutex.unlock();
		//this code is typed for real test
		
	}
	return 0;
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

	string test = "Vo_Alg_";
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
	test += ".csv";

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