//
// Created by major on 9/7/15.
//

#ifndef ANDROID_AVN_KITKAT_MICOMTHREAD_H
#define ANDROID_AVN_KITKAT_MICOMTHREAD_H

#include <unistd.h>
#include "Log.h"
#include "NodeBus.h"
#include "Thread.h"

//using namespace android;

#define LOG_TEMP_BUFF_SIZE 4096

class MicomThread : public Thread
{
private:
    char    mBuff[LOG_TEMP_BUFF_SIZE];
    int     mLogLength;
    NodeAdapter *mpNodeAdapter;

public:
    IMPLEMENT_THREAD(run())

    MicomThread(NodeAdapter *pNodeAdapter);

    bool run();

    bool sendData(int bytes);

    int readFromMicom();
};

#endif
