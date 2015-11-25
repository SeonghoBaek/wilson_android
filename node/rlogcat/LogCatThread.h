//
// Created by major on 9/7/15.
//

#ifndef ANDROID_AVN_KITKAT_LOGCATTHREAD_H
#define ANDROID_AVN_KITKAT_LOGCATTHREAD_H

#include <utils/Thread.h>
#include <unistd.h>
#include "Log.h"

using namespace android;

#define LOG_TEMP_BUFF_SIZE 2048
#define LOG_BUFF_SIZE 4096
#define LOCAL_ROUTER_NAME  "defaultrouter.router.local"

class LogCatThread : public Thread
{
private:
    int     mFd;
    char    mBuff[LOG_TEMP_BUFF_SIZE];
    char    mLogBuff[LOG_BUFF_SIZE];
    int     mLogLength;

public:
    LogCatThread(int fd);

    bool threadLoop();

    bool sendLog(int bytes);
};


#endif //ANDROID_AVN_KITKAT_LOGCATTHREAD_H
