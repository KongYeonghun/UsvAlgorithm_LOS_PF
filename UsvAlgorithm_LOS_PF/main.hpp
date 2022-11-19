
#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <WinSock2.h>
#include <windows.h>

#include <fstream>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>

#include <stdlib.h>
#include <stdio.h>

#include <thread>
#include <mutex>

#include "struct.h"
#include "socket.hpp"
#include "CoordinateConv.hpp"

PF_INIT			m_pf_init;

//SystemCtrl, UsvAlgorithm
SysCtrlSOCKET ServerSystemCtrl_UsvAlgorithm_Socket;

mutex m_mutex;

float mt = 1.0;
unsigned int Obs_avoidance_flag = 0;


AutoNaviStatusInfo PotentailField(USVModeCommand mUSVModeCommand, NavigationInfo mNavigationInfo,FlexTargetInfo mFlexTargetInfo );
stUsvGoalHnV Heading_LimitForSimulator(stUsvGoalHnV m_stUsvGoalHnV, stUsvGoalHnV pre_m_stUsvGoalHnV);
stUsvGoalHnV LOS_Tracking(stWPInfo m_stWPInfo, stUSVMotion m_stUSVMotion);
stUsvGoalHnV ObstacleAvoidance(stWPInfo m_stWPInfo, stUSVMotion m_stUSVMotion, stTargetMotion m_stTargetMotion);
void DataLogging(stWPInfo m_stWPInfo, stUSVMotion m_stUSVMotion, stTargetMotion m_stTargetMotion, stUsvGoalHnV m_stUsvGoalHnV, double distance_usv2wp);

void Setting_Init(void);
float velocity_noise(void);
float degree_noise(void);

CCoordinateConv gCCoordinateConv;


Header						    gHeader;
ROSStatusInfo				    gROSStatusInfo;
USVModeCommand				    gUSVModeCommand;
MissionRecordInfo			    gMissionRecordInfo;
PowerControl				    gPowerControl;
EOCameraControlCommand		    gEOCameraControlCommand;
PlatformControlCommand		    gPlatformControlCommand;
USVStatusInfo				    gUSVStatusInfo;
NavigationStatusInfo		    gNavigationStatusInfo;
NavigationInfo				    gNavigationInfo;
AutoNaviStatusInfo			    gAutoNaviStatusInfo;
RadarTargetInfo				    gRadarTargetInfo;
LidarTargetInfo				    gLidarTargetInfo;
FlexTargetInfo				    gFlexTargetInfo;
PlatformControlState		    gPlatformControlState;
PlatformStatusInfo			    gPlatformStatusInfo;
PowerControlResult			    gPowerControlResult;
AcousticCommInfo			    gAcousticCommInfo;
stWPInfo                        g_stWPInfo;
