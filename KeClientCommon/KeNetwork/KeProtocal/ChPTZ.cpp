#include "ChPTZ.h"
#include "KeMessage.h"
#include "SocketHandler.h"
ChPTZ::ChPTZ(Channel *parent) :
    Channel(parent)
{
    this->setChannelID(parent->getChannelID());
    this->cloudProtocal = 0;
    this->cloudAddr = 1;
    this->setClientID(parent->getClientID());

}

void ChPTZ::OnRespond(QByteArray &msgData)
{
    unsigned char  msgType = msgData[1];
    switch(msgType)
    {
    case DevMsg_SerialData:
    {
        KEDeviceCommonResp * pMsg = (KEDeviceCommonResp *)msgData.data();
        if(pMsg->videoID != this->getChannelID()/256 ||
                pMsg->clientID != this->getClientID())
        {
            return;
        }
        this->wakeup();
    }
        break;
    }
}

static int DCloudCMD[] = {0x0008,//0 上
                          0x0010,//1 下
                          0x0004,//2 左
                          0x0002,//3 右
                          0x0400,//4 光圈关
                          0x0200,//5 光圈开
                          0x0100,//6
                          0x0080,//7
                          0x0040,//8
                          0x0020,//9
                          10,11,12,13,
                          0x9000,//14
                          0x0090,//15
                          0x0007,//16
                          0x001b,//17
                          0x001d,//18
                          0x0000,//19
                          20,21,22,23,
                          0x0003,//24
                          0x0007,//25
                          26,
                          0x001f,//27
                          0x0021,//28
                          0x0023,//29
                          0x0009,//30
                          0x000b,//31
                         };
static int PCloudCMD[] = {0x0008,//0 上
                          0x0010,//1 下
                          0x0004,//2 左
                          0x0002,//3 右
                          0x0800,//4 光圈关
                          0x0400,//5 光圈开
                          0x0100,//6
                          0x0200,//7
                          0x0040,//8
                          0x0020,//9
                          10,11,12,13,
                          0x0019,//14
                          0x0200,//15
                          16,17,18,
                          0x0000,//19
                          20,21,22,23,
                          0x0003,//24
                          0x0007,//25
                          26,
                          0x001f,//27
                          0x0021,//28
                          0x0023,//29
                          0x0009,//30
                          0x000b,//31
                         };

int ChPTZ::Request()
{
    //        int ret = KE_SUCCESS;

    //        if (!ptzInfo.isgot)
    //        {
    //            ret = GetPTZParam(cameraID);
    //            if (ret != KE_SUCCESS)		return ret;
    //            ptzInfo.isgot = true;
    //        }

    if (this->cloudCmd == 2 || this->cloudCmd == 3)
    {
        this->cloudSpeed = this->cloudSpeed << 8;
    }
    QByteArray cloudData = this->GetCloudData();
    QByteArray sendData;
    sendData.resize(sizeof(KEDevGetSerialDataHead));
    sendData.append(cloudData);

    KEDevGetSerialDataHead * head = (KEDevGetSerialDataHead *)sendData.data();
    head->protocal = PROTOCOL_HEAD;
    head->msgType = DevMsg_SETVIDEOPARAM;
    head->msgLength = sendData.size();
    head->videoID = this->getChannelID()/256;
    head->clientID = 0;
    head->dataLen = cloudData.length();

    return this->m_socketHandle->WriteData(sendData);
}

QByteArray ChPTZ::GetCloudData()
{
    QByteArray cloudData;
    switch (this->cloudProtocal)
    {
    case 0://_D
    {
        int dataLen = 7;
        cloudData.resize(dataLen);
        char * pBuf = cloudData.data();
        int cmd = DCloudCMD[this->cloudCmd];
        pBuf[0] = 0xff;
        pBuf[1] = this->cloudAddr;
        pBuf[2] = cmd >> 8;
        pBuf[3] = cmd & 0xff;
        pBuf[4] = this->cloudSpeed >> 8;
        pBuf[5] = this->cloudSpeed & 0xff;
        pBuf[6] = 0;
        for (int i = 1; i < 6; i ++)
        {
            pBuf[6] += pBuf[i];
        }
    }
        break;
    case 1://_P
    {
        int iLen = 8;
        cloudData.resize(iLen);
        char * pBuf = cloudData.data();
        int cmd = PCloudCMD[this->cloudCmd];
        pBuf[0] = 0xa0;
        pBuf[1] = this->cloudAddr-1;
        pBuf[2] = cmd >> 8;
        pBuf[3] = cmd & 0xff;
        pBuf[4] = this->cloudSpeed >> 8;
        pBuf[5] = this->cloudSpeed & 0xff;
        pBuf[6] = 0xaf;
        pBuf[7] = 0;
        for (int i = 1; i < 7; i ++)
        {
            pBuf[7] ^= pBuf[i];
        }
    } break;
    default: break;
    }

    return cloudData;

}
