//
// Created by major on 9/7/15.
//

#ifndef ANDROID_AVN_KITKAT_REPORTTHREAD_H
#define ANDROID_AVN_KITKAT_REPORTTHREAD_H

#include <utils/Thread.h>
#include <unistd.h>
#include <stdio.h>
#include "Log.h"

using namespace android;

#define LOG_TEMP_BUFF_SIZE 2048
#define LOG_BUFF_SIZE 8192
#define LOCAL_ROUTER_NAME  "defaultrouter.router.local"

class USBThread : public Thread
{
private:
    int     mFd;
    char    mBuff[LOG_TEMP_BUFF_SIZE];
    char    mLogBuff[LOG_BUFF_SIZE];
    int     mLogLength;
    FILE    *mpFile;
    int     mExitThread;

public:
    USBThread(int fd);

    bool threadLoop();

    bool sendLog(int bytes);
    
    void stopThread();
};


#endif
