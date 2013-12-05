#ifndef KENET_GLOBAL_H
#define KENET_GLOBAL_H
#if (defined(WIN32) || defined(WIN64))

#if defined(KENETSDK_LIBRARY)
#define KENETSDKSHARED_EXPORT  __declspec(dllexport)
#else
#  define KENETSDKSHARED_EXPORT __declspec(dllimport)
#endif

#define CALLBACK __stdcall
#define CALL_METHOD  __stdcall  //__cdecl


#else

//#include "kenetsdk_global.h"

#endif//win32




/************************************************************************
 ** 常量定义
 ***********************************************************************/
// 错误类型代号，对应CLIENT_GetLastError接口的返回值
#define _EC(x)						(0-x)
#define KE_SUCCESS 				0			// no error
#define KE_ERROR					-1			//
#define KE_SYSTEM_ERROR			_EC(1)		//
#define KE_NETWORK_ERROR			_EC(2)		//
#define KE_NETCONNECT_ERROR		_EC(3)		//
#define KE_NETWRITE_ERROR          _EC(4)
#define KE_Network_Invalid _EC(5) //not connect to host
#define KE_Msg_Timeout     _EC(6)  //message timeout

#define KE_Wrong_Control    _EC(7) //id is wrong
#define KE_Not_Support_Function    _EC(8) //
#define KE_File_Open_Error      _EC(9) // file not open
#define KE_No_Initial   _EC(10) //function need initial instance
#define KE_Parameter_Error _EC(11)
#define KE_Record_Find_End _EC(12)
#define KE_NoRecord  _EC(13)
#define KE_Channel_NotFree _EC(14) //channel is in used
#define KE_CreateInstance_Error _EC(15) //create instance error
#define KE_Login_Error     _EC(16)
#define KE_Relogin     _EC(17)
#define KE_Not_Login _EC(18)
/************************************************************************
 ** 枚举定义
 ***********************************************************************/

enum DeviceType{
    KE_DevType_None,
    KE_DevType_DVR,
    KE_DevType_08Client,
    KE_DevType_08Proxy,
    KE_IPC_Simulator

};
enum ConnectStatus{
    Connect_Success = 0,//connected
    Connect_Miss = 1,//disconnect
    Connect_Again = 2//reconnect

};
enum KE_RealDataType{
    KE_DataType_Video,
    KE_DataType_Audio
};
enum KE_RecordFileType{
    KE_Terminal_All_Record = 0,//终端录像
    KE_Terminal_Plan_Record = 1,//终端计划录像
    KE_Terminal_Senser_Record = 2,//终端传感器报警录像
    KE_Terminal_Motion_Record = 3,//终端移动侦测报警录像
    KE_Terminal_Picture_Record = 4,//终端抓拍照片

    KE_Center_All_Record = 10
    //0：普通录象；1：报警录象；2：移动检测；3：卡号录象；4：图片
};

enum AlarmType{
    Alarm_Motion_Detection = 0,
    Alarm_Video_Shade = 1,
    Alarm_Video_Lost = 2,
    Alarm_Sensor = 10

};

// 视频监视断开事件类型
enum EM_REALPLAY_DISCONNECT_EVENT_TYPE
{
    DISCONNECT_EVENT_REAVE,                     // 表示高级用户抢占低级用户资源
    DISCONNECT_EVENT_NETFORBID,                 // 禁止入网
    DISCONNECT_EVENT_SUBCONNECT               // 动态子链接断开
};

/************************************************************************
 ** 结构体定义
 ***********************************************************************/

// 设置登入时的相关参数
struct NET_PARAM
{
    int waitTimeout;				// 等待超时时间(毫秒为单位)，为0默认5000ms
    int connectTimeout;			// 连接超时时间(毫秒为单位)，为0默认1500ms
    int connectTryNum;			// 连接尝试次数，为0默认1次
    int heartbeatIntervalTime;  //心跳间隔时间,default 10s/次
    int realDataType;   //0:kaer real data;1:normal h264.default:0
};

struct NET_TIME
{
    short				year;					// 年
    short				month;				// 月
    short				day;					// 日
    short				hour;					// 时
    short				minute;				// 分
    short				second;				// 秒
    short             millisecond;          // 毫秒
    short             reserved[1];          // 保留字段
};

// 录像文件信息
struct NET_RECORDFILE_INFO
{
    unsigned int		ch;						// 通道号
    int                fileNo;					// 文件定位索引
    unsigned int		size;					// 文件长度
    NET_TIME			starttime;				// 开始时间
    NET_TIME			endtime;				// 结束时间
    char				filename[128];			// 文件名
    char				nRecordFileType;		// 录象文件类型  0：普通录象；1：报警录象；2：移动检测；3：卡号录象；4：图片
    char                bRecType;               // 0-主码流录像 1-辅码1流录像 2-辅码流2 3-辅码流3录像
    int reserved[4];// 保留字段
};
// 设备信息
struct NET_ChannelInfo{
    int type;//0:通道,1:报警器
    int enable;//0:无效,1:有效
    char channelName[40];
};

struct NET_DEVICEINFO
{
    char szIp[16];							//终端ip地址
    char szMask[16];						//终端子网掩码
    char szGateway[16];						//终端网关
    char deviceName[40];                    //终端名称
    char deviceSN[20];                      //设备标志码
    int  hardwareVer;						//硬件型号
    char softwareVer[10];                   //软件版本号
    int  iPort;								//端口号
    char szUsrName[12];						//登录用户名
    char szUsrPwd[12];						//登录密码
    int  channelCount;                      //通道数
    NET_ChannelInfo channelInfoArray[32];   //通道信息
    //    int  iTerminalType;						//终端型号 0:KESV 1:KEJM01(解码器) 2:KEDVR 3:KECAM01
    //    char bDNS;								//1: 使用DNS, 0: 使用IP
    //    char szDnsName[260];					//DNS名称
};

struct DeviceTreeNode{
    int devType;
    int devID;
    int parentID;
    char name[64];
    void * instance;
};

struct KEAlarmInfo{
    int channelID;
    char state;//状态0/1=开始/结束
    int second;
    int msecond;
    char number;
    char coordinate[9];//坐标： 区域号+区域的坐标
};

#endif // KENET_GLOBAL_H
