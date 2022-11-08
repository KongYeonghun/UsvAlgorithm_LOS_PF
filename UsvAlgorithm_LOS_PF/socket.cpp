//#pragma warning(disable:4996)
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <thread>
#include <WinSock2.h>
#include "struct.h"
#include "socket.hpp"

using namespace std;

SysCtrlSOCKET::SysCtrlSOCKET(void)
{
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		cout << "WSAStartup error" << endl;
        WSACleanup();
        exit(0);
	}
}

SysCtrlSOCKET::~SysCtrlSOCKET(void)
{
}

void SysCtrlSOCKET::UDP_SERVER_OPEN(string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort)
{
	memset( &ServerInfo, 0, sizeof(ServerInfo) ); 
    memset( &ClientInfo, 0, sizeof(ClientInfo) );

    ServerInfo.sin_family  = AF_INET; 
    ServerInfo.sin_addr.s_addr = inet_addr( ServerIpAddr.c_str() ); 
    ServerInfo.sin_port   = htons( ServerPort ); 

    ClientInfo.sin_family  = AF_INET; 
    ClientInfo.sin_addr.s_addr = inet_addr( ClientIpAddr.c_str() ); 
    ClientInfo.sin_port   = htons( ClientPort ); 

    ServerSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // udp. SOCK_DGRAM : UDP
    if( ServerSocket == INVALID_SOCKET ) 
    {
        cout<<"socket error."<<endl;
        closesocket( ServerSocket );
        WSACleanup();
        exit(0);
    }
    //  DWORD recvTimeout = 2000;  // 2초.
    //  setsockopt(ServerSocket, SOL_SOCKET, SO_RCVTIMEO, (char*)&recvTimeout, sizeof(recvTimeout));
    //ntohl : 리틀 엔디안 바이트 순서로 u_long 타입 반환
    //ntohs : 리틀 엔디안 바이트 순서로 u_short 타입 반환
    //htonl : 빅 엔디안 바이트 순서로 u_long 타입 반환
    //htons : 빅 엔디안 바이트 순서로 u_short 타입 반환
}

void SysCtrlSOCKET::UDP_SERVER_OPEN(SysCtrlSOCKET mSysCtrlSOCKET, string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort)
{
	memset( &ServerInfo, 0, sizeof(ServerInfo) ); 
    memset( &ClientInfo, 0, sizeof(ClientInfo) );

    ServerInfo.sin_family  = AF_INET; 
    ServerInfo.sin_addr.s_addr = inet_addr( ServerIpAddr.c_str() ); 
    ServerInfo.sin_port   = htons( ServerPort ); 

    ClientInfo.sin_family  = AF_INET; 
    ClientInfo.sin_addr.s_addr = inet_addr( ClientIpAddr.c_str() ); 
    ClientInfo.sin_port   = htons( ClientPort ); 

    ServerSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP ); // udp. SOCK_DGRAM : UDP
    if( ServerSocket == INVALID_SOCKET ) 
    {
        cout<<"socket error."<<endl;
        closesocket( ServerSocket );
        WSACleanup();
        exit(0);
    }

    if( ::bind(mSysCtrlSOCKET.ServerSocket, (struct sockaddr*)&mSysCtrlSOCKET.ServerInfo, sizeof(mSysCtrlSOCKET.ServerInfo)) < 0)
    {
        cout<<"bind error."<<endl;
        closesocket( mSysCtrlSOCKET.ServerSocket );
        WSACleanup();
        exit(0);
    }
}

void SysCtrlSOCKET::UDP_SERVER_CLOSE()
{
    closesocket( ServerSocket );
}

void SysCtrlSOCKET::UDP_CLIENT_OPEN(string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort)
{
    memset( &ServerInfo, 0, sizeof( ServerInfo ));
    memset( &ClientInfo, 0, sizeof( ClientInfo ));
 
    ServerInfo.sin_family   = AF_INET;
    ServerInfo.sin_addr.s_addr = inet_addr( ServerIpAddr.c_str() ); 
    ServerInfo.sin_port   = htons( ServerPort ); // 포트번호

    ClientInfo.sin_family   = AF_INET;
    ClientInfo.sin_addr.s_addr = inet_addr( ClientIpAddr.c_str() ); 
    ClientInfo.sin_port   = htons( ClientPort ); // 포트번호

    ClientSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if( ClientSocket == INVALID_SOCKET )
    {
        cout<<"socket error."<<endl;
        closesocket( ClientSocket );
        WSACleanup();
        exit(0);
    }
}

void SysCtrlSOCKET::UDP_CLIENT_OPEN(SysCtrlSOCKET mSysCtrlSOCKET, string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort)
{
    memset( &ServerInfo, 0, sizeof( ServerInfo ));
    memset( &ClientInfo, 0, sizeof( ClientInfo ));
 
    ServerInfo.sin_family   = AF_INET;
    ServerInfo.sin_addr.s_addr = inet_addr( ServerIpAddr.c_str() ); 
    ServerInfo.sin_port   = htons( ServerPort ); // 포트번호

    ClientInfo.sin_family   = AF_INET;
    ClientInfo.sin_addr.s_addr = inet_addr( ClientIpAddr.c_str() ); 
    ClientInfo.sin_port   = htons( ClientPort ); // 포트번호

    ClientSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if( ClientSocket == INVALID_SOCKET )
    {
        cout<<"socket error."<<endl;
        closesocket( ClientSocket );
        WSACleanup();
        exit(0);
    }

    if( ::bind(mSysCtrlSOCKET.ClientSocket, (struct sockaddr*)&mSysCtrlSOCKET.ClientInfo, sizeof(mSysCtrlSOCKET.ClientInfo)) < 0)
    {
        cout<<"bind error."<<endl;
        closesocket( mSysCtrlSOCKET.ClientSocket );
        WSACleanup();
        exit(0);
    }
}

void SysCtrlSOCKET::UDP_CLIENT_CLOSE()
{
	closesocket( ClientSocket ); //소켓 닫기
}

// void SysCtrlSOCKET::TCP_CLIENT_OPEN(string ServerIpAddr, string ClientIpAddr, int ServerPort, int ClientPort)
// {
//     memset( &ServerInfo, 0, sizeof( ServerInfo ));
//     memset( &ClientInfo, 0, sizeof( ClientInfo ));
 
//     ServerInfo.sin_family   = AF_INET;
//     ServerInfo.sin_addr.s_addr = inet_addr( ServerIpAddr.c_str() ); 
//     ServerInfo.sin_port   = htons( ServerPort ); // 포트번호

//     ClientInfo.sin_family   = AF_INET;
//     ClientInfo.sin_addr.s_addr = inet_addr( ClientIpAddr.c_str() ); 
//     ClientInfo.sin_port   = htons( ClientPort ); // 포트번호

//     ClientSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
//     if( ClientSocket == INVALID_SOCKET )
//     {
//         cout<<"socket error."<<endl;
//         closesocket( ClientSocket );
//         WSACleanup();
//         exit(0);
//     }
// }

/*******************************************************************************************/
//FromServerToClient_Send
//FromServerToClient_Recv
//FromClientToServer_Send
//FromClientToServer_Recv
/*******************************************************************************************/

void SysCtrlSOCKET::ServerToClient_Send(char *Buffer, SysCtrlSOCKET mSysCtrlSOCKET, int Buffer_Size)//when this program are server., size of buffer size
{
    int Send_Size = sendto( mSysCtrlSOCKET.ServerSocket, Buffer, Buffer_Size, 0, (struct sockaddr*) &mSysCtrlSOCKET.ClientInfo, sizeof( mSysCtrlSOCKET.ClientInfo ) );

    if( Send_Size != Buffer_Size )
    { 
        memset(Buffer, 0, sizeof(Buffer));
    }
}

int SysCtrlSOCKET::ServerToClient_Recv(char *Buffer, SysCtrlSOCKET mSysCtrlSOCKET)//when this program are server.
{
    int Client_size = sizeof( mSysCtrlSOCKET.ClientInfo );
    int cRecv_Size = recvfrom( mSysCtrlSOCKET.ServerSocket, Buffer, BUFFER_SIZE, 0, (struct sockaddr*) &(mSysCtrlSOCKET.ClientInfo), &Client_size );

    if( cRecv_Size <= 0 )
    { 
        memset(Buffer, 0, sizeof(Buffer));
    }
    return cRecv_Size;
}

void SysCtrlSOCKET::ClientToServer_Send(char *Buffer, SysCtrlSOCKET mSysCtrlSOCKET, int Buffer_Size)//when this program are client.
{
    int Send_Size = sendto(mSysCtrlSOCKET.ClientSocket, Buffer, Buffer_Size,  0, (struct sockaddr*) &mSysCtrlSOCKET.ServerInfo, sizeof( mSysCtrlSOCKET.ServerInfo ) );

    if( Send_Size != Buffer_Size )
    { 
        memset(Buffer, 0, sizeof(Buffer));
    }
}

int SysCtrlSOCKET::ClientToServer_Recv(char *Buffer, SysCtrlSOCKET mSysCtrlSOCKET)//when this program are client.
{
    int Server_Size = sizeof( mSysCtrlSOCKET.ServerInfo );
    int Recv_Size = recvfrom( mSysCtrlSOCKET.ClientSocket, Buffer, BUFFER_SIZE, 0, (struct sockaddr*) &(mSysCtrlSOCKET.ServerInfo), &Server_Size );

    if( Recv_Size <= 0 )
    { 
        memset(Buffer, 0, sizeof(Buffer));
    }
    return Recv_Size;
}

/*******************************************************************************************/
//MessageClassification
/*******************************************************************************************/

void SysCtrlSOCKET::MessageClassification(char* Buffer, ROSStatusInfo* mROSStatusInfo)
{
    char temp_buffer[COMM_BODY_SIZE];

	memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mROSStatusInfo));
    memcpy(mROSStatusInfo, (ROSStatusInfo*)&temp_buffer, sizeof(*mROSStatusInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, USVModeCommand* mUSVModeCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mUSVModeCommand));
    memcpy(mUSVModeCommand, (USVModeCommand*)&temp_buffer, sizeof(*mUSVModeCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, MissionRecordInfo* mMissionRecordInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mMissionRecordInfo));
    memcpy(mMissionRecordInfo, (MissionRecordInfo*)&temp_buffer, sizeof(*mMissionRecordInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PowerControl* mPowerControl)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPowerControl));
    memcpy(mPowerControl, (PowerControl*)&temp_buffer, sizeof(*mPowerControl));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, EOCameraControlCommand* mEOCameraControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mEOCameraControlCommand));
    memcpy(mEOCameraControlCommand, (EOCameraControlCommand*)&temp_buffer, sizeof(*mEOCameraControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PlatformControlCommand* mPlatformControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPlatformControlCommand));
    memcpy(mPlatformControlCommand, (PlatformControlCommand*)&temp_buffer, sizeof(*mPlatformControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, USVStatusInfo* mUSVStatusInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mUSVStatusInfo));
    memcpy(mUSVStatusInfo, (USVStatusInfo*)&temp_buffer, sizeof(*mUSVStatusInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, NavigationStatusInfo* mNavigationStatusInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mNavigationStatusInfo));
    memcpy(mNavigationStatusInfo, (NavigationStatusInfo*)&temp_buffer, sizeof(*mNavigationStatusInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, NavigationInfo* mNavigationInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mNavigationInfo));
    memcpy(mNavigationInfo, (NavigationInfo*)&temp_buffer, sizeof(*mNavigationInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AutoNaviStatusInfo* mAutoNaviStatusInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAutoNaviStatusInfo));
    memcpy(mAutoNaviStatusInfo, (AutoNaviStatusInfo*)&temp_buffer, sizeof(*mAutoNaviStatusInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, RadarTargetInfo* mRadarTargetInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mRadarTargetInfo));
    memcpy(mRadarTargetInfo, (RadarTargetInfo*)&temp_buffer, sizeof(*mRadarTargetInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, LidarTargetInfo* mLidarTargetInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mLidarTargetInfo));
    memcpy(mLidarTargetInfo, (LidarTargetInfo*)&temp_buffer, sizeof(*mLidarTargetInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, FlexTargetInfo* mFlexTargetInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mFlexTargetInfo));
    memcpy(mFlexTargetInfo, (FlexTargetInfo*)&temp_buffer, sizeof(*mFlexTargetInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PlatformControlState* mPlatformControlState)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPlatformControlState));
    memcpy(mPlatformControlState, (PlatformControlState*)&temp_buffer, sizeof(*mPlatformControlState));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PlatformStatusInfo* mPlatformStatusInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPlatformStatusInfo));
    memcpy(mPlatformStatusInfo, (PlatformStatusInfo*)&temp_buffer, sizeof(*mPlatformStatusInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PowerControlResult* mPowerControlResult)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPowerControlResult));
    memcpy(mPowerControlResult, (PowerControlResult*)&temp_buffer, sizeof(*mPowerControlResult));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AcousticCommInfo* mAcousticCommInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAcousticCommInfo));
    memcpy(mAcousticCommInfo, (AcousticCommInfo*)&temp_buffer, sizeof(*mAcousticCommInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, EOTargetInfo* mEOTargetInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mEOTargetInfo));
    memcpy(mEOTargetInfo, (EOTargetInfo*)&temp_buffer, sizeof(*mEOTargetInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, MissionRecordInfoRes* mMissionRecordInfoRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mMissionRecordInfoRes));
    memcpy(mMissionRecordInfoRes, (MissionRecordInfoRes*)&temp_buffer, sizeof(*mMissionRecordInfoRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, IRCameraControlCommand* mIRCameraControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mIRCameraControlCommand));
    memcpy(mIRCameraControlCommand, (IRCameraControlCommand*)&temp_buffer, sizeof(*mIRCameraControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, SaeronEOTrackingCommand* mSaeronEOTrackingCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mSaeronEOTrackingCommand));
    memcpy(mSaeronEOTrackingCommand, (SaeronEOTrackingCommand*)&temp_buffer, sizeof(*mSaeronEOTrackingCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, EOCameraControlRes* mEOCameraControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mEOCameraControlRes));
    memcpy(mEOCameraControlRes, (EOCameraControlRes*)&temp_buffer, sizeof(*mEOCameraControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, IRCameraControlRes* mIRCameraControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mIRCameraControlRes));
    memcpy(mIRCameraControlRes, (IRCameraControlRes*)&temp_buffer, sizeof(*mIRCameraControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, SaeronEOTrackingCommandRes* mSaeronEOTrackingCommandRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mSaeronEOTrackingCommandRes));
    memcpy(mSaeronEOTrackingCommandRes, (SaeronEOTrackingCommandRes*)&temp_buffer, sizeof(*mSaeronEOTrackingCommandRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, LRFDisdanceRes* mLRFDisdanceRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mLRFDisdanceRes));
    memcpy(mLRFDisdanceRes, (LRFDisdanceRes*)&temp_buffer, sizeof(*mLRFDisdanceRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, MscPowerControlCommand* mMscPowerControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mMscPowerControlCommand));
    memcpy(mMscPowerControlCommand, (MscPowerControlCommand*)&temp_buffer, sizeof(*mMscPowerControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, StrutControlCommand* mStrutControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrutControlCommand));
    memcpy(mStrutControlCommand, (StrutControlCommand*)&temp_buffer, sizeof(*mStrutControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, MachinGunControlCommand* mMachinGunControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mMachinGunControlCommand));
    memcpy(mMachinGunControlCommand, (MachinGunControlCommand*)&temp_buffer, sizeof(*mMachinGunControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, LifeVestControlCommand* mLifeVestControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mLifeVestControlCommand));
    memcpy(mLifeVestControlCommand, (LifeVestControlCommand*)&temp_buffer, sizeof(*mLifeVestControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, MscPowerControlRes* mMscPowerControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mMscPowerControlRes));
    memcpy(mMscPowerControlRes, (MscPowerControlRes*)&temp_buffer, sizeof(*mMscPowerControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, StrutControlRes* mStrutControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrutControlRes));
    memcpy(mStrutControlRes, (StrutControlRes*)&temp_buffer, sizeof(*mStrutControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, MachinGunControlRes* mMachinGunControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mMachinGunControlRes));
    memcpy(mMachinGunControlRes, (MachinGunControlRes*)&temp_buffer, sizeof(*mMachinGunControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, LifeVestControlRes* mLifeVestControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mLifeVestControlRes));
    memcpy(mLifeVestControlRes, (LifeVestControlRes*)&temp_buffer, sizeof(*mLifeVestControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, StrutStateRes* mStrutStateRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrutStateRes));
    memcpy(mStrutStateRes, (StrutStateRes*)&temp_buffer, sizeof(*mStrutStateRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, UsvEmergencyStop* mUsvEmergencyStop)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mUsvEmergencyStop));
    memcpy(mUsvEmergencyStop, (UsvEmergencyStop*)&temp_buffer, sizeof(*mUsvEmergencyStop));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, SteeringNozzlePidGain* mSteeringNozzlePidGain)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mSteeringNozzlePidGain));
    memcpy(mSteeringNozzlePidGain, (SteeringNozzlePidGain*)&temp_buffer, sizeof(*mSteeringNozzlePidGain));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, RpmPidGain* mRpmPidGain)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mRpmPidGain));
    memcpy(mRpmPidGain, (RpmPidGain*)&temp_buffer, sizeof(*mRpmPidGain));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, GainLutCheckCommand* mGainLutCheckCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mGainLutCheckCommand));
    memcpy(mGainLutCheckCommand, (GainLutCheckCommand*)&temp_buffer, sizeof(*mGainLutCheckCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, UsvEmergencyStopRes* mUsvEmergencyStopRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mUsvEmergencyStopRes));
    memcpy(mUsvEmergencyStopRes, (UsvEmergencyStopRes*)&temp_buffer, sizeof(*mUsvEmergencyStopRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, ApiBatGasState* mApiBatGasState)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mApiBatGasState));
    memcpy(mApiBatGasState, (ApiBatGasState*)&temp_buffer, sizeof(*mApiBatGasState));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, SteeringNozzlePIDGainState* mSteeringNozzlePIDGainState)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mSteeringNozzlePIDGainState));
    memcpy(mSteeringNozzlePIDGainState, (SteeringNozzlePIDGainState*)&temp_buffer, sizeof(*mSteeringNozzlePIDGainState));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, RpmPidGainState* mRpmPidGainState)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mRpmPidGainState));
    memcpy(mRpmPidGainState, (RpmPidGainState*)&temp_buffer, sizeof(*mRpmPidGainState));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, SteeringNozzlePidGainLutRes* mSteeringNozzlePidGainLutRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mSteeringNozzlePidGainLutRes));
    memcpy(mSteeringNozzlePidGainLutRes, (SteeringNozzlePidGainLutRes*)&temp_buffer, sizeof(*mSteeringNozzlePidGainLutRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AuvLauncherControlCommand* mAuvLauncherControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAuvLauncherControlCommand));
    memcpy(mAuvLauncherControlCommand, (AuvLauncherControlCommand*)&temp_buffer, sizeof(*mAuvLauncherControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AuvL_Status* mAuvL_Status)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAuvL_Status));
    memcpy(mAuvL_Status, (AuvL_Status*)&temp_buffer, sizeof(*mAuvL_Status));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AuvLarsPowerControlCommand* mAuvLarsPowerControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAuvLarsPowerControlCommand));
    memcpy(mAuvLarsPowerControlCommand, (AuvLarsPowerControlCommand*)&temp_buffer, sizeof(*mAuvLarsPowerControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AutoDockingResult* mAutoDockingResult)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAutoDockingResult));
    memcpy(mAutoDockingResult, (AutoDockingResult*)&temp_buffer, sizeof(*mAutoDockingResult));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, ErrorMsg* mErrorMsg)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mErrorMsg));
    memcpy(mErrorMsg, (ErrorMsg*)&temp_buffer, sizeof(*mErrorMsg));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AutoDockingPostionRes* mAutoDockingPostionRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAutoDockingPostionRes));
    memcpy(mAutoDockingPostionRes, (AutoDockingPostionRes*)&temp_buffer, sizeof(*mAutoDockingPostionRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AutoDockingPostion* mAutoDockingPostion)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAutoDockingPostion));
    memcpy(mAutoDockingPostion, (AutoDockingPostion*)&temp_buffer, sizeof(*mAutoDockingPostion));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PlatformTempMessage* mPlatformTempMessage)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPlatformTempMessage));
    memcpy(mPlatformTempMessage, (PlatformTempMessage*)&temp_buffer, sizeof(*mPlatformTempMessage));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, PlatformTempMessageRes* mPlatformTempMessageRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mPlatformTempMessageRes));
    memcpy(mPlatformTempMessageRes, (PlatformTempMessageRes*)&temp_buffer, sizeof(*mPlatformTempMessageRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AuvLarsLockingValve* mAuvLarsLockingValve)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAuvLarsLockingValve));
    memcpy(mAuvLarsLockingValve, (AuvLarsLockingValve*)&temp_buffer, sizeof(*mAuvLarsLockingValve));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, WaveInfo* mWaveInfo)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mWaveInfo));
    memcpy(mWaveInfo, (WaveInfo*)&temp_buffer, sizeof(*mWaveInfo));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, AuvLarsLockingValveRes* mAuvLarsLockingValveRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mAuvLarsLockingValveRes));
    memcpy(mAuvLarsLockingValveRes, (AuvLarsLockingValveRes*)&temp_buffer, sizeof(*mAuvLarsLockingValveRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, Strut1_ControlCommand* mStrut1_ControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrut1_ControlCommand));
    memcpy(mStrut1_ControlCommand, (Strut1_ControlCommand*)&temp_buffer, sizeof(*mStrut1_ControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, Strut2_ControlCommand* mStrut2_ControlCommand)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrut2_ControlCommand));
    memcpy(mStrut2_ControlCommand, (Strut2_ControlCommand*)&temp_buffer, sizeof(*mStrut2_ControlCommand));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, Strut1_ControlRes* mStrut1_ControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrut1_ControlRes));
    memcpy(mStrut1_ControlRes, (Strut1_ControlRes*)&temp_buffer, sizeof(*mStrut1_ControlRes));
}

void SysCtrlSOCKET::MessageClassification(char* Buffer, Strut2_ControlRes* mStrut2_ControlRes)
{
    char temp_buffer[COMM_BODY_SIZE];
    
    memset(temp_buffer, 0, sizeof(temp_buffer));
    memcpy(temp_buffer, Buffer + sizeof(Header), sizeof(*mStrut2_ControlRes));
    memcpy(mStrut2_ControlRes, (Strut2_ControlRes*)&temp_buffer, sizeof(*mStrut2_ControlRes));
}


/*******************************************************************************************/
//MessageCombination
/*******************************************************************************************/

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, ROSStatusInfo mROSStatusInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mROSStatusInfo, sizeof(mROSStatusInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, USVModeCommand mUSVModeCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mUSVModeCommand, sizeof(mUSVModeCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, MissionRecordInfo mMissionRecordInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mMissionRecordInfo, sizeof(mMissionRecordInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PowerControl mPowerControl)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPowerControl, sizeof(mPowerControl));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, EOCameraControlCommand mEOCameraControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mEOCameraControlCommand, sizeof(mEOCameraControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PlatformControlCommand mPlatformControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPlatformControlCommand, sizeof(mPlatformControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, USVStatusInfo mUSVStatusInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mUSVStatusInfo, sizeof(mUSVStatusInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, NavigationStatusInfo mNavigationStatusInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mNavigationStatusInfo, sizeof(mNavigationStatusInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, NavigationInfo mNavigationInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mNavigationInfo, sizeof(mNavigationInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AutoNaviStatusInfo mAutoNaviStatusInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAutoNaviStatusInfo, sizeof(mAutoNaviStatusInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, RadarTargetInfo mRadarTargetInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mRadarTargetInfo, sizeof(mRadarTargetInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, LidarTargetInfo mLidarTargetInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mLidarTargetInfo, sizeof(mLidarTargetInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, FlexTargetInfo mFlexTargetInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mFlexTargetInfo, sizeof(mFlexTargetInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PlatformControlState mPlatformControlState)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPlatformControlState, sizeof(mPlatformControlState));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PlatformStatusInfo mPlatformStatusInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPlatformStatusInfo, sizeof(mPlatformStatusInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PowerControlResult mPowerControlResult)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPowerControlResult, sizeof(mPowerControlResult));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AcousticCommInfo mAcousticCommInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAcousticCommInfo, sizeof(mAcousticCommInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, EOTargetInfo mEOTargetInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mEOTargetInfo, sizeof(mEOTargetInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, MissionRecordInfoRes mMissionRecordInfoRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mMissionRecordInfoRes, sizeof(mMissionRecordInfoRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, IRCameraControlCommand mIRCameraControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mIRCameraControlCommand, sizeof(mIRCameraControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, SaeronEOTrackingCommand mSaeronEOTrackingCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mSaeronEOTrackingCommand, sizeof(mSaeronEOTrackingCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, EOCameraControlRes mEOCameraControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mEOCameraControlRes, sizeof(mEOCameraControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, IRCameraControlRes mIRCameraControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mIRCameraControlRes, sizeof(mIRCameraControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, SaeronEOTrackingCommandRes mSaeronEOTrackingCommandRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mSaeronEOTrackingCommandRes, sizeof(mSaeronEOTrackingCommandRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, LRFDisdanceRes mLRFDisdanceRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mLRFDisdanceRes, sizeof(mLRFDisdanceRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, MscPowerControlCommand mMscPowerControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mMscPowerControlCommand, sizeof(mMscPowerControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, StrutControlCommand mStrutControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrutControlCommand, sizeof(mStrutControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, MachinGunControlCommand mMachinGunControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mMachinGunControlCommand, sizeof(mMachinGunControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, LifeVestControlCommand mLifeVestControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mLifeVestControlCommand, sizeof(mLifeVestControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, MscPowerControlRes mMscPowerControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mMscPowerControlRes, sizeof(mMscPowerControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, StrutControlRes mStrutControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrutControlRes, sizeof(mStrutControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, MachinGunControlRes mMachinGunControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mMachinGunControlRes, sizeof(mMachinGunControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, LifeVestControlRes mLifeVestControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mLifeVestControlRes, sizeof(mLifeVestControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, StrutStateRes mStrutStateRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrutStateRes, sizeof(mStrutStateRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PcHeartBeat mPcHeartBeat)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPcHeartBeat, sizeof(mPcHeartBeat));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, CurrentTime mCurrentTime)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mCurrentTime, sizeof(mCurrentTime));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, SteeringNozzlePidGain mSteeringNozzlePidGain)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mSteeringNozzlePidGain, sizeof(mSteeringNozzlePidGain));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, RpmPidGain mRpmPidGain)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mRpmPidGain, sizeof(mRpmPidGain));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, GainLutCheckCommand mGainLutCheckCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mGainLutCheckCommand, sizeof(mGainLutCheckCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, UsvEmergencyStop mUsvEmergencyStop)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mUsvEmergencyStop, sizeof(mUsvEmergencyStop));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, UsvEmergencyStopRes mUsvEmergencyStopRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mUsvEmergencyStopRes, sizeof(mUsvEmergencyStopRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, ApiBatGasState mApiBatGasState)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mApiBatGasState, sizeof(mApiBatGasState));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, SteeringNozzlePIDGainState mSteeringNozzlePIDGainState)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mSteeringNozzlePIDGainState, sizeof(mSteeringNozzlePIDGainState));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, RpmPidGainState mRpmPidGainState)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mRpmPidGainState, sizeof(mRpmPidGainState));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, SteeringNozzlePidGainLutRes mSteeringNozzlePidGainLutRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mSteeringNozzlePidGainLutRes, sizeof(mSteeringNozzlePidGainLutRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AuvLauncherControlCommand mAuvLauncherControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAuvLauncherControlCommand, sizeof(mAuvLauncherControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AuvL_Status mAuvL_Status)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAuvL_Status, sizeof(mAuvL_Status));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AuvLarsPowerControlCommand mAuvLarsPowerControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAuvLarsPowerControlCommand, sizeof(mAuvLarsPowerControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AutoDockingResult mAutoDockingResult)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAutoDockingResult, sizeof(mAutoDockingResult));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, ErrorMsg mErrorMsg)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mErrorMsg, sizeof(mErrorMsg));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AutoDockingPostionRes mAutoDockingPostionRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAutoDockingPostionRes, sizeof(mAutoDockingPostionRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AutoDockingPostion mAutoDockingPostion)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAutoDockingPostion, sizeof(mAutoDockingPostion));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PlatformTempMessage mPlatformTempMessage)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPlatformTempMessage, sizeof(mPlatformTempMessage));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, PlatformTempMessageRes mPlatformTempMessageRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mPlatformTempMessageRes, sizeof(mPlatformTempMessageRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AuvLarsLockingValve mAuvLarsLockingValve)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAuvLarsLockingValve, sizeof(mAuvLarsLockingValve));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, WaveInfo mWaveInfo)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mWaveInfo, sizeof(mWaveInfo));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, AuvLarsLockingValveRes mAuvLarsLockingValveRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mAuvLarsLockingValveRes, sizeof(mAuvLarsLockingValveRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, Strut1_ControlCommand mStrut1_ControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrut1_ControlCommand, sizeof(mStrut1_ControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, Strut2_ControlCommand mStrut2_ControlCommand)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrut2_ControlCommand, sizeof(mStrut2_ControlCommand));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, Strut1_ControlRes mStrut1_ControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrut1_ControlRes, sizeof(mStrut1_ControlRes));
}

void SysCtrlSOCKET::MessageCombination(char* Buffer, Header mHeader, Strut2_ControlRes mStrut2_ControlRes)
{
    memset(Buffer, 0, BUFFER_SIZE);
    memcpy(Buffer, (char*)&mHeader, sizeof(mHeader));
    memcpy(Buffer + sizeof(mHeader), (char*)&mStrut2_ControlRes, sizeof(mStrut2_ControlRes));
}

/*******************************************************************************************/
//Header
/*******************************************************************************************/

void SysCtrlSOCKET::HeaderSetting(Header* mHeader, unsigned char Source_Application_ID, unsigned char Destination_Application_ID, unsigned short Message_ID,unsigned int Payload_Length, unsigned int Sequence_Number)
{
    mHeader->Source_Application_ID = Source_Application_ID;
    mHeader->Destination_Application_ID = Destination_Application_ID;
    mHeader->Message_ID = Message_ID;
    mHeader->Payload_Length = Payload_Length;
    mHeader->Sequence_Number = Sequence_Number;
}
