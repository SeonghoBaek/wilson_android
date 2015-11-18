//
// Created by major on 9/7/15.
//

#include <string.h>
#include "USBThread.h"
#include "Log.h"
#include "NBus.h"
#include <sys/poll.h>
#include <time.h>

using namespace android;

USBThread::USBThread(int fd) : mFd(fd), mLogLength(0)
{
    time_t ftm = time(NULL);
    char strTime[100];

    memset(strTime, 0, 100);

    strftime(strTime, 100, "%y%m%d%H%M%S", localtime(&ftm));

    char fileName[256];

    memset(fileName, 0, 256);

    sprintf(fileName, "/storage/usb0/logcat_%s.log", strTime);

    this->mpFile = fopen(fileName, "w");
}

void USBThread::stopThread()
{
    this->mExitThread = 1;
    //LOGI("Set ReportThread EXIT");
}

bool USBThread::threadLoop()
{
    int numRead = -1;
    struct pollfd fds[1];
    
    this->mExitThread = 0;
    
    fds[0].fd = mFd;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    
    for(;;)
    {
        poll(fds, 1, 100);
        
        if (!(fds[0].revents & POLLIN))
        {
            if (this->mExitThread == 1) break;
            
            continue;
        }
        
        numRead = read(mFd, mLogBuff, LOG_BUFF_SIZE);
        
        if (numRead > 0)
        {
            mLogLength = numRead;
            this->sendLog(mLogLength);
        }
        else break;
    }
    
    fclose(this->mpFile);
    close(mFd);
    
    LOGI("ReportThread Exited.");
    
    return true;
}

bool USBThread::sendLog(int bytes)
{
    char* raw = (char *)this->mLogBuff;
    int length = bytes;

    if (this->mpFile == NULL)
    {
        LOGE("Target file not exist");
    }
    else fwrite(raw, sizeof(char), bytes, this->mpFile);
    
    return true;
}