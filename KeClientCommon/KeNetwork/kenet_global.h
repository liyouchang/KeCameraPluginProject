#ifndef KENET_GLOBAL_H
#define KENET_GLOBAL_H

#if (defined(WIN32) || defined(WIN64))

#if defined(KENETSDK_LIBRARY)
#  define KENETSDKSHARED_EXPORT  __declspec(dllexport)
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
#define KE_Msg_Timeout     _EC(5)  //message timeout
#define KE_Login_ERROR     _EC(6)  //message timeout
#define KE_Wrong_Control    _EC(7) //id is wrong
#define KE_Not_Support_Function    _EC(8) //id is wrong
#define KE_File_Open_Error      _EC(9) // file not open
#define KE_No_Initial   _EC(10) //function need initial instance
#define KE_Parameter_Error _EC(11)
#define KE_Record_Find_End _EC(12)
#define KE_NoRecord  _EC(13)
#define KE_Channel_NotFree _EC(14) //channel is in used
#define KE_CreateInstance_Error _EC(15) //create instance error
/************************************************************************
 ** 枚举定义
 ***********************************************************************/

enum DeviceType{
    KE_DevType_None,
    KE_DevType_DVR,
    KE_DevType_08SERVER,
    KE_IPC_Simulator
};
enum CallBackType{

};
enum KE_RealDataType{
    KE_DataType_Video,
    KE_DataType_Audio
};
enum KE_RecordFileType{
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
    int                 waittime;				// 等待超时时间(毫秒为单位)，为0默认5000ms
    int                 connectTime;			// 连接超时时间(毫秒为单位)，为0默认1500ms
    int                 connectTryNum;			// 连接尝试次数，为0默认1次
    int                 subConnectSpaceTime;	// 子连接之间的等待时间(毫秒为单位)，为0默认10ms
    int                 getDevInfoTime;		// 获取设备信息超时时间，为0默认1000ms
    int					connectBufSize;		// 每个连接接收数据缓冲大小(字节为单位)，为0默认250*1024
    int					getConnInfoTime;		// 获取子连接信息超时时间(毫秒为单位)，为0默认1000ms
    int                 searchRecordTime;      // 按时间查询录像文件的超时时间(毫秒为单位),为0默认为3000ms
    int                 subDisconnetTime;      // 检测子链接断线等待时间(毫秒为单位)，为0默认为60000ms
    char				netType;				// 网络类型, 0-LAN, 1-WAN
    char                reserved1[3];         // 对齐
    int                 picBufSize;            // 实时图片接收缓冲大小（字节为单位），为0默认为2*1024*1024
    char				reserved[4];			// 保留字段
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
struct NET_DEVICEINFO
{
    char				sSerialNumber[32];	// 序列号
    char				byAlarmInPortNum;		// DVR报警输入个数
    char                byAlarmOutPortNum;		// DVR报警输出个数
    char				byDiskNum;				// DVR硬盘个数
    char				byDVRType;				// DVR类型, 见枚举DHDEV_DEVICE_TYPE
    char				byChanNum;				// DVR通道个数
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
