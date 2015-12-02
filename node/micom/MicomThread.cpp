//
// Created by major on 9/7/15.
//
#define _LOG_TAG "micom_thread"

#include <unistd.h>
#include "Types.h"
#include "MicomThread.h"
#include "MicomDaemon.h"

MicomThread::MicomThread(MicomBuffer *pMicomBuffer)
{
    if (pMicomBuffer)
    {
        this->mpMicomBuffer = pMicomBuffer;
    }
}

bool MicomThread::run()
{
    int readBytes = -1;

    // Read micom data
    while (1)
    {
        readBytes = readFromMicom();

        if (readBytes > 0)
        {
            sendData(readBytes);
        }
    }

    return true;
}

int MicomThread::readFromMicom()
{
    /*
     * Read micom data to this->mBuff.
     * Return read bytes.
     */

     sleep(1);

     strncpy(this->mBuff, "hello", 5);

     return 5;
}

bool MicomThread::sendData(int bytes)
{
    if (this->mpMicomBuffer)
    {
        NBUS_CMD cmd(MICOM_PACKET_LENGTH);

        cmd.mType = MICOM_MESSAGE;
        memcpy(cmd.mpData, this->mBuff, MICOM_PACKET_LENGTH);

        this->mpMicomBuffer->push(cmd);
    }

    return true;
}