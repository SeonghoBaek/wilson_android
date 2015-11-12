//
// Created by major on 9/7/15.
//

#include <string.h>
#include "ReportThread.h"
#include "Log.h"
#include "NBus.h"
#include <sys/poll.h>

using namespace android;

ReportThread::ReportThread(int fd) : mFd(fd), mLogLength(0)
{
    LOGI("LogCatThread Created.\n");
    unlink("/stoarget/usb0/bugreport.log");
    this->mpFile = fopen("/storage/usb0/bugreport.log", "w");
}

void ReportThread::stopThread()
{
    this->mExitThread = 1;
    LOGI("Set ReportThread EXIT");
}

bool ReportThread::threadLoop()
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
        else break;
    }
    
    fclose(this->mpFile);
    close(mFd);
    
    LOGI("ReportThread Exited.");
    
    return true;
}

bool ReportThread::sendLog(int bytes)
{
    char* raw = (char *)this->mLogBuff;
    int length = bytes;
    
    fwrite(raw, sizeof(char), bytes, this->mpFile);
    
    return true;
}