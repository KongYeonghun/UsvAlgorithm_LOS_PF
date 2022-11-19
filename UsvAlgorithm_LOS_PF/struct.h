/*******************************************************************************************/
/*******************************************************************************************/

#pragma once

#include <stdint.h>
#include <thread>
#include <WinSock2.h>

#define COMM_HEADER_SIZE 12
#define COMM_BODY_SIZE 1300
#define BUFFER_SIZE COMM_HEADER_SIZE+COMM_BODY_SIZE

#define DEF_NUM_WP 15
#define DEF_NUM_TARGET 20
#define LOOKUP_TABLE_NUM 20
#define SETTING_PATH ".\\setting.txt" 
#define LOGGING_PATH ".\\SystemCtrl_Logging.bin"
// #define ROS_MISSIONEQUIP ".\\Ros_MissionEquip.exe"

#pragma pack(push,1)

/*******************************************************************************************/
//Header
/*******************************************************************************************/

typedef struct stHeader
{
	unsigned char Source_Application_ID;
	unsigned char Destination_Application_ID;
	unsigned short Message_ID;
	unsigned int Payload_Length;// body_length
	unsigned int Sequence_Number;
}Header;

typedef struct stCurrentTime
{
	unsigned short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char min;
	unsigned char sec;
}CurrentTime;

/*******************************************************************************************/
//ROS -> SystemCtrl
/*******************************************************************************************/

typedef struct stROSStatusInfo
{
	double ROS_Latitude;
	double ROS_Longitude;
}ROSStatusInfo;


typedef struct stUSVModeCommand
{
	unsigned char	Mode;
	unsigned short	ModeDetail;
	unsigned char	AlgSelect;
	unsigned short	NavigationStartRecordID;
	double			Emergency_Return_Latitude;
	double			Emergency_Return_Longitude;
	unsigned char	reserved;

}USVModeCommand;

typedef struct stWaypoint
{
	unsigned short 	RecordID			;
	unsigned short 	Velocity			;
	double 			Latitude			;
	double 			Longitude			;
	unsigned char 	EngineSelect		;
	unsigned char 	AuvLauncher			;
	unsigned char 	SonarLauncher		;
	unsigned char 	AcousticCommLauncher;
}mWaypoint;

typedef struct stMissionRecordInfo
{
	char 			MissionName[50];
	unsigned short 	LateralError;
	unsigned short 	PathRange;	
	unsigned short 	MissionPointCount;
	mWaypoint 		Waypoint[DEF_NUM_WP];
}MissionRecordInfo;

typedef struct stMissionRecordInfoRes
{
	char			MissionName[50];
	unsigned short	MissionPointCount;
	unsigned short	Result;
}MissionRecordInfoRes;

typedef struct stEOCameraControlRes
{
	unsigned char	EOMode				;
	unsigned char	PanControl			;
	unsigned short	PanSpeed			;
	unsigned char	TiltControl			;
	unsigned short	TiltSpeed			;
	short			PanLocationControl	;
	unsigned char	PanMode				;
	short			TiltLocationControl	;
	unsigned char	TiltMode			;
	unsigned char	LRFControl			;
	unsigned char	TrakingControl		;
	unsigned char	TrakingID			;
	unsigned char	ZoomMode			;
	unsigned char	FocusMode			;
	unsigned char	EONightMode			;
	unsigned short	D_ZoomControl		;
	unsigned short	DISControl			;
}EOCameraControlRes;


typedef struct stIRCameraControlRes
{
	unsigned short	D_ZoomControl;
	unsigned short	NUCControl	;
	unsigned short	DISControl	;
	unsigned short	MIDEControl	;
}IRCameraControlRes;

typedef struct stSaeronEOTrackingCommandRes
{
	unsigned short	TrackingResult;
}SaeronEOTrackingCommandRes;

typedef struct stLRFDisdanceRes
{
	unsigned short LrfModeInfo;		
	unsigned short LrfMeasureDistanceInfo;		
}LRFDisdanceRes;

typedef struct stWaveInfo
{
	float WaveHeight;
	float Wavelength;
	float WaveFrequency;
	float WaveDirection;
}WaveInfo;

typedef struct stPowerControl
{
	unsigned char AutoSailingSBC1_2;
	unsigned char AutoSailingSBC3;
	unsigned char GPS_INS;
	unsigned char EthernetSwitch;
	unsigned char DigitalCompass;
	unsigned char CCTV;
	unsigned char Radar;
	unsigned char LiDAR;
	unsigned char EO;
	unsigned char MissionEquip1_24V	;
	unsigned char MissionEquip2_24V	;
	unsigned char MissionEquip3_24V	;
	unsigned char Platform1_24V		;
	unsigned char Platform2_24V		;
	unsigned char Platform3_24V		;
	unsigned char Platform4_24V		;
}PowerControl;

typedef struct stEOCameraControlCommand
{
	unsigned char	EOMode;
	unsigned char	PanControl;
	unsigned short	PanSpeed;
	unsigned char	TiltControl;
	unsigned short	TiltSpeed;
	short			PanLocationControl;
	unsigned char	PanMode;
	short			TiltLocationControl;
	unsigned char	TiltMode;
	unsigned char	LRFControl;
	unsigned char	TrakingControl;
	unsigned char	TrakingID;
	unsigned char	ZoomMode;
	unsigned char	FocusMode;
	unsigned char	EONightMode;
	unsigned short	D_ZoomControl;
	unsigned short	DISControl;
}EOCameraControlCommand;

typedef struct stIRCameraControlCommand
{
	unsigned short	D_ZoomControl;
	unsigned short	NUCControl	;
	unsigned short	DISControl	;
	unsigned short	MIDEControl	;
}IRCameraControlCommand;

typedef struct stSaeronEOTrackingCommand
{
	unsigned short TrackingCameraType;
	unsigned short PointX;
	unsigned short PointY;
	unsigned short Width;
	unsigned short Height;
	unsigned short TrackingMode;
	unsigned short TrackingAssist;
	unsigned short TrackingStartFlag;
}SaeronEOTrackingCommand;

typedef struct stPlatformControlCommand
{
	unsigned short 	ControlType;
	unsigned short 	RPMValue;
	short 			HandleAngle;
	unsigned short 	Heading;
	unsigned short 	Velocity;
	unsigned short 	Gear;
	unsigned char 	EngineSelect;
}PlatformControlCommand;

typedef struct stPcHeartBeat
{
	unsigned char	HeartBeat;
}PcHeartBeat;

typedef struct stUsvEmergencyStop
{
	unsigned char	E_Stop;
	unsigned char	FireAerosol;
}UsvEmergencyStop;

// typedef struct stCurrentTime
// {
// 	unsigned short	Year;
// 	unsigned char	Month;
// 	unsigned char	Day;
// 	unsigned char	Hour;
// 	unsigned char	Min;
// 	unsigned char	Sec;
// }CurrentTime;

typedef struct stSteeringNozzlePidGain
{
	unsigned char	LutNum;
	double			Kp;
	double			Ki;
	double			Kd;
}SteeringNozzlePidGain;


typedef struct stRpmPidGain
{
	double	Kp;
	double	Ki;
	double	Kd;
}RpmPidGain;

typedef struct stGainLutCheckCommand
{
	unsigned char	GainCheck;
}GainLutCheckCommand;



typedef struct stPlatformTempMessage
{
	unsigned char	Temp1	;
	unsigned char	Temp2	;	
	unsigned char	Temp3	;	
	unsigned char	Temp4	;	
	unsigned char	Temp5	;	
	unsigned char	Temp6	;	
	unsigned char	Temp7	;	
	unsigned char	Temp8	;	
	unsigned short	Temp9	;	
	unsigned short	Temp10;		
	unsigned short	Temp11;		
	unsigned short	Temp12;		
	double			Temp13;
	double			Temp14;
	double			Temp15;
	double			Temp16;
}PlatformTempMessage;


typedef struct stPlatformTempMessageRes
{
	unsigned char	Temp1	;
	unsigned char	Temp2	;
	unsigned char	Temp3	;
	unsigned char	Temp4	;
	unsigned char	Temp5	;
	unsigned char	Temp6	;
	unsigned char	Temp7	;
	unsigned char	Temp8	;
	unsigned short	Temp9	;
	unsigned short	Temp10	;
	unsigned short	Temp11	;
	unsigned short	Temp12	;
	double			Temp13	;
	double			Temp14	;
	double			Temp15	;
	double			Temp16	;
}PlatformTempMessageRes;

/*******************************************************************************************/
// SystemCtrl -> ROS
/*******************************************************************************************/

typedef struct stUSVStatusInfo
{
	unsigned char 	Mode;
	unsigned short 	ModeDetail;
	unsigned char 	NavigationConnectFlag;
	unsigned char 	PlatformConnectFlag;
	unsigned char 	EoCamDevConnectFlag;
	unsigned char 	reserved1;
	unsigned char 	reserved2;
}USVStatusInfo;

typedef struct stUSVStatusInfoConnectionFlag
{
	char AutoSailingConnectFlag;
	char PlatformConnectFlag;
	char EoCamDevConnectFlag;
	char reserved1;//firstec
	char reserved2;//kintec
}USVStatusInfoConnectionFlag;

typedef struct stNavigationStatusInfo
{
	unsigned char EOCameraBit;
	unsigned char RadarBit;
	unsigned char LidarBit;
	unsigned char GPSBit;
	unsigned char reserved;
}NavigationStatusInfo;

typedef struct stNavigationInfo
{
	double 			Latitude;
	double 			Longitude;
	unsigned short 	Heading;		
	unsigned short 	Velocity;	
	double 			Roll;
	double 			Pitch;
	double 			Yaw;
	double			Altitude;
	double			GyroX;
	double			GyroY;
	double			GyroZ;
}NavigationInfo;
		
typedef struct stAutoNaviStatusInfo
{
	unsigned short DestinationPathID;
	unsigned short AutoNaviStatus;
	unsigned short Heading;
	unsigned short Velocity;
	unsigned short FailStatus;
}AutoNaviStatusInfo;


typedef struct stAllTargetInfo
{
	unsigned short  Object_ID	;	
	unsigned short  Course		;	
	unsigned short  Velocity	;	
	double          Latitude	;	
	double          Longitude	;	
}AllTargetInfo;

typedef struct stRadarTargetInfo
{
	unsigned short  TargetCount;
	AllTargetInfo	TargetInfo[DEF_NUM_TARGET];
}RadarTargetInfo;

typedef struct stLidarTargetInfo
{
	unsigned short  TargetCount;
	AllTargetInfo	TargetInfo[DEF_NUM_TARGET];
}LidarTargetInfo;

typedef struct stFlexTargetInfo
{
	unsigned short  TargetCount;
	AllTargetInfo	TargetInfo[DEF_NUM_TARGET];
}FlexTargetInfo;

typedef struct stPlatformControlState
{
	unsigned short 	ControlType;
	unsigned short 	RPMValue;
	short 			HandleAngle;
	unsigned short 	Velocity;
	unsigned short 	Heading;
	unsigned short 	Gear;
}PlatformControlState;

typedef struct stPlatformStatusInfo
{
	unsigned short 	BITState;
	unsigned short 	FuelState;
	unsigned short 	EngineStartStatus;
	unsigned char 	EngineSelectFlag;
	unsigned short	RPMValue;
	short			HandleAngle;
}PlatformStatusInfo;

typedef struct stPowerControlResult
{
	unsigned char AutoSailingSBC1_2;
	unsigned char AutoSailingSBC3;
	unsigned char GPS_INS;
	unsigned char EthernetSwitch;
	unsigned char DigitalCompass;
	unsigned char CCTV;
	unsigned char Radar;
	unsigned char LiDAR;
	unsigned char EO;
	unsigned char MissionEquip1_24V;
	unsigned char MissionEquip2_24V;
	unsigned char MissionEquip3_24V;
	unsigned char Platform1_24V;
	unsigned char Platform2_24V;
	unsigned char Platform3_24V;
	unsigned char Platform4_24V;
}PowerControlResult;

typedef struct stAcousticCommInfo
{
	unsigned char EquipmentSelect;
	unsigned char AuvID;
	double Latitude;
	double Longitude;	
	unsigned char Depth;			
}AcousticCommInfo;

typedef struct stAllEOMemberInfo
{
	unsigned short		Object_ID;
	unsigned short		Course;
	unsigned short		Velocity;
	double				Latitude;
	double				Longitude;
	unsigned short		BoundingBox_X_StartPoint;
	unsigned short		BoundingBox_Y_StartPoint;
	unsigned short		BoundingBox_X_Length;
	unsigned short		BoundingBox_Y_Length;
}AllEOMemberInfo;

typedef struct stEOTargetInfo
{
	unsigned short		TargetCount;
	AllEOMemberInfo		mEOMemberInfo[DEF_NUM_TARGET];
}EOTargetInfo;

/*******************************************************************************************/
// SystemCtrl <-> LARS(Kintec)
/*******************************************************************************************/
typedef struct stAuvLauncherControlCommand
{
	unsigned char	AuvL_Ctrl;
	unsigned char	reserved1;
	unsigned char	reserved2;
	unsigned char	reserved3;
}AuvLauncherControlCommand;

typedef struct stAuvLarsPowerControlCommand
{
	unsigned char Camera;
	unsigned char Actuator1_2;
	unsigned char reserved2;
}AuvLarsPowerControlCommand;

typedef struct stAuvLarsLockingValve
{
	unsigned char	Locking	;		
	unsigned char	reserved1;				
	unsigned char	reserved2;				
	unsigned char	reserved3;	
}AuvLarsLockingValve;

typedef struct stAuvLarsLockingValveRes
{
	unsigned char	Locking;
	unsigned char	reserved1;
	unsigned char	reserved2;
	unsigned char	reserved3;
}AuvLarsLockingValveRes;

typedef struct stAuvL_Status
{
	unsigned char AuvL_Status;
	unsigned char AuvL_DevStatus;
	unsigned char AuvL_PowerSatus;
	unsigned char reserved1;
}AuvL_Status;

/*******************************************************************************************/
// SystemCtrl <-> MSC(Firstec)
/*******************************************************************************************/
typedef struct stMscPowerControlCommand
{
	unsigned char	Strut_1;
	unsigned char	Strut_2;
	unsigned char	ElectroMagnet_1;
	unsigned char	ElectroMagnet_2;
	unsigned char	LED_1_2;
	unsigned char	LED_3;
	unsigned char	VoiceDev;
	unsigned char	MachineGun127;
	unsigned char	LifeVestDev;
	unsigned char	WarningLamp;
}MscPowerControlCommand;

typedef struct stStrutControlCommand
{
	unsigned char	StrutCtrl_1;
	unsigned char	StrutCtrl_2;
}StrutControlCommand;

typedef struct stStrut1_ControlCommand
{
	unsigned char StrutCtrl_1; 
}Strut1_ControlCommand;

typedef struct stStrut2_ControlCommand
{
	unsigned char StrutCtrl_2;
}Strut2_ControlCommand;

typedef struct stStrut1_ControlRes
{
	unsigned char	StrutCtrl_1;

}Strut1_ControlRes;

typedef struct stStrut2_ControlRes
{
	unsigned char	StrutCtrl_2;

}Strut2_ControlRes;


typedef struct stMachinGunControlCommand
{
	unsigned char	ControlMode;
	unsigned char	Pan_1_127mm;
	unsigned char	Tilt_1_127mm;
	float			Pan_2_127mm;
	float			Tilt_2_127mm;
	float			speed_127mm;
	unsigned char	Zero_127mm;
}MachinGunControlCommand;

typedef struct stLifeVestControlCommand
{
	unsigned char LifeVestDevCtrl ;
}LifeVestControlCommand;

typedef struct stMscPowerControlRes
{
	unsigned char	Strut_1;
	unsigned char	Strut_2;
	unsigned char	ElectroMagnet_1;
	unsigned char	ElectroMagnet_2;
	unsigned char	LED_1_2;
	unsigned char	LED_3;
	unsigned char	VoiceDev;
	unsigned char	MachineGun127;
	unsigned char	LifeVestDev;
	unsigned char	WarningLamp;
}MscPowerControlRes;

typedef struct stStrutControlRes
{
	unsigned char	StrutCtrl_1;
	unsigned char	StrutCtrl_2;
}StrutControlRes;

typedef struct stMachinGunControlRes
{
	float				Pan_2_127mm;
	float				Tilt_2_127mm;
	float				speed_127mm;
	unsigned char		ControlMode;
	unsigned char		Pan_1_127mm;
	unsigned char		Tilt_1_127mm;

}MachinGunControlRes;

typedef struct stLifeVestControlRes
{
	unsigned char	LifeVestDevCtrl;
}LifeVestControlRes;

typedef struct stStrutStateRes
{
	float			StrutCtrl_1;
	float			StrutCtrl_2;
	unsigned char	StrutCtrl_1_limitsw_1;
	unsigned char	StrutCtrl_1_limitsw_2;
	unsigned char	StrutCtrl_2_limitsw_1;
	unsigned char	StrutCtrl_2_limitsw_2;
}StrutStateRes;

/*******************************************************************************************/
// SystemCtrl -> AutoDocking
/*******************************************************************************************/
//USVModeCommand
//NavigationInfo

typedef struct stgps_data
{
	double latitude;
	double longitude;
	double altitude;
	double HDOP;
	double VDOP;
}gps_data;

typedef struct stimu_data
{
	double heading;
	double pitch;
	double roll;
	double gyroX;
	double gyroY;
	double gyroZ;
}imu_data;

typedef struct stAutoDockingPostion
{
	double DockLatitude;
	double DockLongitude;
	double UnDockLatitude;
	double UnDockLongitude;
}AutoDockingPostion;

typedef struct stAutoDockingResult
{
	double	velocity;
	double	heading;
}AutoDockingResult;

typedef struct stErrorMsg
{
	unsigned short ERR_SUCCESS;
	unsigned short ERR_SENSOR;
	unsigned short ERR_PROGRAM;
}ErrorMsg;

typedef struct stGridMap
{
	unsigned char gridmap[3000000];//1000*1000*3
}GridMap;

typedef struct stAutoDockingPostionRes
{
	double DockLatitude;
	double DockLongitude;
	double UnDockLatitude;
	double UnDockLongitude;
}AutoDockingPostionRes;

/*******************************************************************************************/
// PlatformCtrl-> SystemCtrl
/*******************************************************************************************/
typedef struct stUsvEmergencyStopRes
{
	unsigned char	E_Stop;
}UsvEmergencyStopRes;

typedef struct stApiBatGasState
{
	unsigned char	Heartbeat;
	unsigned short	SOC;
	unsigned short	Gas_Detector;
	unsigned char	ManUnmannedState;
}ApiBatGasState;

typedef struct stSteeringNozzlePIDGainState
{
	double	Kp;
	double	Ki;
	double	Kd;
}SteeringNozzlePIDGainState;

typedef struct stRpmPidGainState
{
	double	Kp;
	double	Ki;
	double	Kd;
}RpmPidGainState;

typedef struct stSteeringNozzlePidGainLutRes
{
	unsigned char	LUT_NO[LOOKUP_TABLE_NUM];
	double			Kp[LOOKUP_TABLE_NUM];
	double			Ki[LOOKUP_TABLE_NUM];
	double			Kd[LOOKUP_TABLE_NUM];
}SteeringNozzlePidGainLutRes;

typedef struct stCC_PowerControl{
    unsigned char ch1;
    unsigned char ch2;
    unsigned char ch3;
    unsigned char ch4;
    unsigned char ch5;
    unsigned char ch6;
    unsigned char ch7;
    unsigned char ch8;
}CC_PowerControl;


typedef struct stCC_PowerControlRes{
	unsigned char	ch1;	
	unsigned char	ch2;	
	unsigned char	ch3;	
	unsigned char	ch4;	
	unsigned char	ch5;	
	unsigned char	ch6;	
	unsigned char	ch7;	
	unsigned char	ch8;	
}CC_PowerControlRes;




/*******************************************************************************************/
// director ; kong yeong hun 
/*******************************************************************************************/

#define		NUM_WP				20
#define		DEF_NUM_TARGET		20	

#define		DEF_RAND_SEED		5

#define		DEF_DEG2RAD			3.1415926535897931/180.0
#define		DEF_RAD2DEG			180.0/3.1415926535897931
#define		PI					3.1415926535897931
#define     DEF_KTS2MPS         0.5144
#define     DEF_MPS2KTS         1/0.5144

typedef struct tWPInfo
{
	int 		stnWPnum;
	int 		stnWPsize;
	double 		stdWPUTMN[NUM_WP];
	double 		stdWPUTME[NUM_WP];
	double 		stdCOG[NUM_WP];
	double 		stdSOG[NUM_WP];
	double 		stdWPTime[NUM_WP];//wp 
	int 		stWPinfo[NUM_WP];//
	// 20171221 KAIST 
	double 		stdWPID[NUM_WP];
	double 		stdWPMaxVel[NUM_WP];// 
	double 		stdWPLeftTime[NUM_WP];
	double 		stdWPAcceptRadius[NUM_WP];
}stWPInfo;

typedef struct tUSVMotion
{
	double 	stdUTMN;
	double 	stdUTME;
	double 	stdSOG;// m/s
	double 	stdCOG;// degree
	double 	stdRoll;
	double 	stdPitch;
	double 	stdYaw;
	double 	stdVsurge;
	double 	stWVsway;
	double 	stWVHeave;
	double 	stWAccSurge;
	double 	stWAccSway;
	double 	stWAccHeave;
	double 	stErrCov_N;
	double 	stErrCov_NE;
	double 	stErrCov_E;
}stUSVMotion;

typedef struct tTargetMotion
{
	int 	stnNumObj;
	int 	stnMMSI[DEF_NUM_TARGET];
	double 	stdShipLOA[DEF_NUM_TARGET];
	double 	stdUTMN[DEF_NUM_TARGET];
	double 	stdUTME[DEF_NUM_TARGET];
	double 	stdSOG[DEF_NUM_TARGET];         // m/s
	double 	stdCOG[DEF_NUM_TARGET];         // degree, USV heading
	double 	stdDISTANCE[DEF_NUM_TARGET];
	double 	stErrCov_N[DEF_NUM_TARGET];     // m^2
	double 	stErrCov_NE[DEF_NUM_TARGET];	// m^2
	double 	stErrCov_E[DEF_NUM_TARGET];     // m^2
	double 	stErrCov_heading[DEF_NUM_TARGET]; // rad^2
}stTargetMotion;

typedef struct stUsvGoalHeadingNVelocity{
	double heading;
	double velocity;
}stUsvGoalHnV;

typedef struct PotentialFieldAlgorithmInit {
	double AVOIDANCE_RADIUS;//meter, 
	double AVOIDANCE_ANGLE;	
	double HEADING_LIMIT;
	double COLEGS_RULE_RADIUS;
}PF_INIT;

#pragma pack(pop)

