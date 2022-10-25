
#pragma once
#ifndef SOCKET_HPP
#define SOCKET_HPP
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#include "struct.h"

using namespace std;

class SysCtrlSOCKET
{
public:
	SysCtrlSOCKET(void);
	~SysCtrlSOCKET(void);

	void UDP_SERVER_OPEN(string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort);
	void UDP_SERVER_OPEN(SysCtrlSOCKET mSysCtrlSOCKET, string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort);
	void UDP_SERVER_CLOSE();
	void UDP_CLIENT_OPEN(string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort);
	void UDP_CLIENT_OPEN(SysCtrlSOCKET mSysCtrlSOCKET, string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort);
	void UDP_CLIENT_CLOSE();

	void ServerToClient_Send(char *nBuffer, SysCtrlSOCKET nSysCtrlSOCKET, int Buffer_Size);//when this program are server.
	int ServerToClient_Recv(char *nBuffer, SysCtrlSOCKET nSysCtrlSOCKET);//when this program are server.
	void ClientToServer_Send(char *nBuffer, SysCtrlSOCKET nSysCtrlSOCKET, int Buffer_Size);//when this program are client.
	int ClientToServer_Recv(char *nBuffer, SysCtrlSOCKET nSysCtrlSOCKET);//when this program are client.

/*******************************************************************************************/
//MessageClassification
/*******************************************************************************************/

	void MessageClassification(char* Buffer, ROSStatusInfo* mROSStatusInfo);
	void MessageClassification(char* Buffer, USVModeCommand* mUSVModeCommand);
	void MessageClassification(char* Buffer, MissionRecordInfo* mMissionRecordInfo);
	void MessageClassification(char* Buffer, PowerControl* mPowerControl);
	void MessageClassification(char* Buffer, EOCameraControlCommand* mEOCameraControlCommand);
	void MessageClassification(char* Buffer, PlatformControlCommand* mPlatformControlCommand);
	void MessageClassification(char* Buffer, USVStatusInfo* mUSVStatusInfo);
	void MessageClassification(char* Buffer, NavigationStatusInfo* mNavigationStatusInfo);
	void MessageClassification(char* Buffer, NavigationInfo* mNavigationInfo);
	void MessageClassification(char* Buffer, AutoNaviStatusInfo* mAutoNaviStatusInfo);
	void MessageClassification(char* Buffer, RadarTargetInfo* mRadarTargetInfo);
	void MessageClassification(char* Buffer, LidarTargetInfo* mLidarTargetInfo);
	void MessageClassification(char* Buffer, FlexTargetInfo* mFlexTargetInfo);
	void MessageClassification(char* Buffer, PlatformControlState* mPlatformControlState);
	void MessageClassification(char* Buffer, PlatformStatusInfo* mPlatformStatusInfo);
	void MessageClassification(char* Buffer, PowerControlResult* mPowerControlResult);
	void MessageClassification(char* Buffer, AcousticCommInfo* mAcousticCommInfo);
	void MessageClassification(char* Buffer, EOTargetInfo* mEOTargetInfo);
	void MessageClassification(char* Buffer, MissionRecordInfoRes* mMissionRecordInfoRes);
	void MessageClassification(char* Buffer, IRCameraControlCommand* mIRCameraControlCommand);
	void MessageClassification(char* Buffer, SaeronEOTrackingCommand* mSaeronEOTrackingCommand);
	void MessageClassification(char* Buffer, EOCameraControlRes* mEOCameraControlRes);
	void MessageClassification(char* Buffer, IRCameraControlRes* mIRCameraControlRes);
	void MessageClassification(char* Buffer, SaeronEOTrackingCommandRes* mSaeronEOTrackingCommandRes);
	void MessageClassification(char* Buffer, LRFDisdanceRes* mLRFDisdanceRes);
	void MessageClassification(char* Buffer, MscPowerControlCommand* mMscPowerControlCommand);
	void MessageClassification(char* Buffer, StrutControlCommand* mStrutControlCommand);
	void MessageClassification(char* Buffer, MachinGunControlCommand* mMachinGunControlCommand);
	void MessageClassification(char* Buffer, LifeVestControlCommand* mLifeVestControlCommand);
	void MessageClassification(char* Buffer, MscPowerControlRes* mMscPowerControlRes);
	void MessageClassification(char* Buffer, StrutControlRes* mStrutControlRes);
	void MessageClassification(char* Buffer, MachinGunControlRes* mMachinGunControlRes);
	void MessageClassification(char* Buffer, LifeVestControlRes* mLifeVestControlRes);
	void MessageClassification(char* Buffer, StrutStateRes* mStrutStateRes);
	void MessageClassification(char* Buffer, UsvEmergencyStop* mUsvEmergencyStop);
	void MessageClassification(char* Buffer, SteeringNozzlePidGain* mSteeringNozzlePidGain);
	void MessageClassification(char* Buffer, RpmPidGain* mRpmPidGain);
	void MessageClassification(char* Buffer, GainLutCheckCommand* mGainLutCheckCommand);
	void MessageClassification(char* Buffer, UsvEmergencyStopRes* mUsvEmergencyStopRes);
	void MessageClassification(char* Buffer, ApiBatGasState* mApiBatGasState);
	void MessageClassification(char* Buffer, SteeringNozzlePIDGainState* mSteeringNozzlePIDGainState);
	void MessageClassification(char* Buffer, RpmPidGainState* mRpmPidGainState);
	void MessageClassification(char* Buffer, SteeringNozzlePidGainLutRes* mSteeringNozzlePidGainLutRes);
	void MessageClassification(char* Buffer, AuvLauncherControlCommand* mAuvLauncherControlCommand);
	void MessageClassification(char* Buffer, AuvL_Status* mAuvL_Status);
	void MessageClassification(char* Buffer, AuvLarsPowerControlCommand* mAuvLarsPowerControlCommand);
	void MessageClassification(char* Buffer, AutoDockingResult* mAutoDockingResult);
	void MessageClassification(char* Buffer, ErrorMsg* mErrorMsg);
	void MessageClassification(char* Buffer, AutoDockingPostionRes* mAutoDockingPostionRes);
	void MessageClassification(char* Buffer, AutoDockingPostion* mAutoDockingPostion);
	void MessageClassification(char* Buffer, PlatformTempMessage* mPlatformTempMessage);
	void MessageClassification(char* Buffer, PlatformTempMessageRes* mPlatformTempMessageRes);
	void MessageClassification(char* Buffer, AuvLarsLockingValve* mAuvLarsLockingValve);
	void MessageClassification(char* Buffer, WaveInfo* mWaveInfo);
	void MessageClassification(char* Buffer, AuvLarsLockingValveRes* mAuvLarsLockingValveRes);
	void MessageClassification(char* Buffer, Strut1_ControlCommand* mStrut1_ControlCommand);
	void MessageClassification(char* Buffer, Strut2_ControlCommand* mStrut2_ControlCommand);
	void MessageClassification(char* Buffer, Strut1_ControlRes* mStrut1_ControlRes);
	void MessageClassification(char* Buffer, Strut2_ControlRes* mStrut2_ControlRes);

/*******************************************************************************************/
//MessageCombination
/*******************************************************************************************/

	void MessageCombination(char* Buffer, Header mHeader, ROSStatusInfo mROSStatusInfo);
	void MessageCombination(char* Buffer, Header mHeader, USVModeCommand mUSVModeCommand);
	void MessageCombination(char* Buffer, Header mHeader, MissionRecordInfo mMissionRecordInfo);
	void MessageCombination(char* Buffer, Header mHeader, PowerControl mPowerControl);
	void MessageCombination(char* Buffer, Header mHeader, EOCameraControlCommand mEOCameraControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, PlatformControlCommand mPlatformControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, USVStatusInfo mUSVStatusInfo);
	void MessageCombination(char* Buffer, Header mHeader, NavigationStatusInfo mNavigationStatusInfo);
	void MessageCombination(char* Buffer, Header mHeader, NavigationInfo mNavigationInfo);
	void MessageCombination(char* Buffer, Header mHeader, AutoNaviStatusInfo mAutoNaviStatusInfo);
	void MessageCombination(char* Buffer, Header mHeader, RadarTargetInfo mRadarTargetInfo);
	void MessageCombination(char* Buffer, Header mHeader, LidarTargetInfo mLidarTargetInfo);
	void MessageCombination(char* Buffer, Header mHeader, FlexTargetInfo mFlexTargetInfo);
	void MessageCombination(char* Buffer, Header mHeader, PlatformControlState mPlatformControlState);
	void MessageCombination(char* Buffer, Header mHeader, PlatformStatusInfo mPlatformStatusInfo);
	void MessageCombination(char* Buffer, Header mHeader, PowerControlResult mPowerControlResult);
	void MessageCombination(char* Buffer, Header mHeader, AcousticCommInfo mAcousticCommInfo);
	void MessageCombination(char* Buffer, Header mHeader, EOTargetInfo mEOTargetInfo);
	void MessageCombination(char* Buffer, Header mHeader, MissionRecordInfoRes mMissionRecordInfoRes);
	void MessageCombination(char* Buffer, Header mHeader, IRCameraControlCommand mIRCameraControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, SaeronEOTrackingCommand mSaeronEOTrackingCommand);
	void MessageCombination(char* Buffer, Header mHeader, EOCameraControlRes mEOCameraControlRes);
	void MessageCombination(char* Buffer, Header mHeader, IRCameraControlRes mIRCameraControlRes);
	void MessageCombination(char* Buffer, Header mHeader, SaeronEOTrackingCommandRes mSaeronEOTrackingCommandRes);
	void MessageCombination(char* Buffer, Header mHeader, LRFDisdanceRes mLRFDisdanceRes);
	void MessageCombination(char* Buffer, Header mHeader, MscPowerControlCommand mMscPowerControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, StrutControlCommand mStrutControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, MachinGunControlCommand mMachinGunControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, LifeVestControlCommand mLifeVestControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, MscPowerControlRes mMscPowerControlRes);
	void MessageCombination(char* Buffer, Header mHeader, StrutControlRes mStrutControlRes);
	void MessageCombination(char* Buffer, Header mHeader, MachinGunControlRes mMachinGunControlRes);
	void MessageCombination(char* Buffer, Header mHeader, LifeVestControlRes mLifeVestControlRes);
	void MessageCombination(char* Buffer, Header mHeader, StrutStateRes mStrutStateRes);
	void MessageCombination(char* Buffer, Header mHeader, PcHeartBeat mPcHeartBeat);
	void MessageCombination(char* Buffer, Header mHeader, CurrentTime mCurrentTime);
	void MessageCombination(char* Buffer, Header mHeader, SteeringNozzlePidGain mSteeringNozzlePidGain);
	void MessageCombination(char* Buffer, Header mHeader, RpmPidGain mRpmPidGain);
	void MessageCombination(char* Buffer, Header mHeader, GainLutCheckCommand mGainLutCheckCommand);
	void MessageCombination(char* Buffer, Header mHeader, UsvEmergencyStop mUsvEmergencyStop);
	void MessageCombination(char* Buffer, Header mHeader, UsvEmergencyStopRes mUsvEmergencyStopRes);
	void MessageCombination(char* Buffer, Header mHeader, ApiBatGasState mApiBatGasState);
	void MessageCombination(char* Buffer, Header mHeader, SteeringNozzlePIDGainState mSteeringNozzlePIDGainState);
	void MessageCombination(char* Buffer, Header mHeader, RpmPidGainState mRpmPidGainState);
	void MessageCombination(char* Buffer, Header mHeader, SteeringNozzlePidGainLutRes mSteeringNozzlePidGainLutRes);
	void MessageCombination(char* Buffer, Header mHeader, AuvLauncherControlCommand mAuvLauncherControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, AuvL_Status mAuvL_Status);
	void MessageCombination(char* Buffer, Header mHeader, AuvLarsPowerControlCommand mAuvLarsPowerControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, AutoDockingResult mAutoDockingResult);
	void MessageCombination(char* Buffer, Header mHeader, ErrorMsg mErrorMsg);
	void MessageCombination(char* Buffer, Header mHeader, AutoDockingPostionRes mAutoDockingPostionRes);
	void MessageCombination(char* Buffer, Header mHeader, AutoDockingPostion mAutoDockingPostion);
	void MessageCombination(char* Buffer, Header mHeader, PlatformTempMessage mPlatformTempMessage);
	void MessageCombination(char* Buffer, Header mHeader, PlatformTempMessageRes mPlatformTempMessageRes);
	void MessageCombination(char* Buffer, Header mHeader, AuvLarsLockingValve mAuvLarsLockingValve);
	void MessageCombination(char* Buffer, Header mHeader, WaveInfo mWaveInfo);
	void MessageCombination(char* Buffer, Header mHeader, AuvLarsLockingValveRes mAuvLarsLockingValveRes);
	void MessageCombination(char* Buffer, Header mHeader, Strut1_ControlCommand mStrut1_ControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, Strut2_ControlCommand mStrut2_ControlCommand);
	void MessageCombination(char* Buffer, Header mHeader, Strut1_ControlRes mStrut1_ControlRes);
	void MessageCombination(char* Buffer, Header mHeader, Strut2_ControlRes mStrut2_ControlRes);

/*******************************************************************************************/
//HeaderSetting
/*******************************************************************************************/

	void HeaderSetting(Header* mHeader, unsigned char Source_Application_ID, unsigned char Destination_Application_ID, unsigned short Message_ID,unsigned int Payload_Length, unsigned int Sequence_Number);

	WSADATA wsaData; 

	//udp server
	SOCKET ServerSocket;
	SOCKET ClientSocket;
	SOCKADDR_IN ServerInfo;
    SOCKADDR_IN ClientInfo; 
	
};

#endif








