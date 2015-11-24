//
// Created by major on 9/7/15.
//

#include <unistd.h>
#include "Types.h"
#include "MicomThread.h"

//using namespace android;

MicomThread::MicomThread(NodeAdapter *pNodeAdapter)
{
    if (pNodeAdapter)
    {
        this->mpNodeAdapter = pNodeAdapter;
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
    if (this->mpNodeAdapter)
    {
        this->mpNodeAdapter->addQueue(this->mBuff, bytes, MICOM_MESSAGE);
    }

    return true;
}