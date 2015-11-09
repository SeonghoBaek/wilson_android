//
// Created by major on 9/7/15.
//

#include <string.h>
#include "Packet.h"
#include "LogCatThread.h"
#include "Log.h"
#include "NBus.h"
#include "XMLTypes.h"

using namespace android;

LogCatThread::LogCatThread(int fd) : mFd(fd), mLogLength(0)
{
    LOGI("LogCatThread Created.\n");
}

bool LogCatThread::threadLoop()
{
    int numRead = -1;

    while (1)
    {
        numRead = read(mFd, mBuff, LOG_TEMP_BUFF_SIZE);

        if (numRead > 0)
        {
            memcpy(mLogBuff + mLogLength, mBuff, numRead);
            mLogLength += numRead;
            mLogBuff[mLogLength + 1] = '\0';

            if (mLogLength > (LOG_BUFF_SIZE / 2))
            {
                this->sendLog(mLogLength);

                mLogLength = 0;
            }
        }
        else if (numRead < 0) break;
    }

    return true;
}

bool LogCatThread::sendLog(int bytes)
{
    char* raw = (char *)this->mLogBuff;
    int length = bytes;
    NBUSPacket packet;

    EventXML eventXml;

    //LOGI("LogcatThread sendLog: %d", bytes);
    eventXml.setType(EVT_KEYWORD(LOG));
    eventXml.setFormat(LOG_KEYWORD(TXT));
    eventXml.setLength((unsigned int)length);

    char *header = eventXml.toXML();

    //LOGI("LogCatThread sendLog: %s", header);

    packet.copyHeader(header,(int)strlen(header));
    packet.copyData(raw, bytes);

    nbus_cast(LOCAL_ROUTER_NAME, packet.getPacket(), packet.getPacketLength());

    delete header;

    return true;
}