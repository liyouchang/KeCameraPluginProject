#ifndef KEUPDATEMESSAGE_H
#define KEUPDATEMESSAGE_H

#include <QHostAddress>
#include <QtEndian>

enum KEMsgType_Update
{
    Update_Register = 0x01,
    Update_HeartBeat= 0x02,
    Update_Data = 0x03,
    PROTOCOL_HEAD = 0xff
};

#define RESP_ACK 0x0d
#define RESP_NAK 0x05
#define RESP_END 0x06

#define KE_Terminal_Port 22616


inline void CopyIP(char * dstIP,int srcIP){
    int tmpIp = qToBigEndian<quint32>(srcIP);
    QHostAddress addr(tmpIp);
    strcpy(dstIP,addr.toString().toLatin1().data());
}


#pragma pack(1)


struct KEUpdateMsgHead
{
    unsigned char protocal;
    unsigned char msgType;
    unsigned int msgLength;
};
struct KEUpdateReq
{
    unsigned char protocal;
    unsigned char msgType;
    unsigned int msgLength;
    char mac[16];
    char version[16];
    char puType[16];
};

struct KEUpdateResp
{
    unsigned char protocal;
    unsigned char msgType;
    unsigned int msgLength;
    char resp;
};

struct KEUpdateDataReq
{
    unsigned char protocal;
    unsigned char msgType;
    unsigned int msgLength;
    short totalPack;
    short currentPack;
    //char sendData[1024];
};

struct KEUpdateDataResp
{
    unsigned char protocal;
    unsigned char msgType;
    unsigned int msgLength;
    short currentPack;
};
#pragma pack()




#endif // KEUPDATEMESSAGE_H
