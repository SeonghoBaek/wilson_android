//
// Created by major on 9/7/15.
//

#ifndef ANDROID_AVN_KITKAT_MICOMDAEMON_H
#define ANDROID_AVN_KITKAT_MICOMDAEMON_H

#include "MicomBuffer.h"
#include "MicomThread.h"

#define MICOM_MESSAGE_LENGTH    64
#define MICOM_PACKET_LENGTH  16 // 16 bytes
#define MICOM_DISPATCH_THREAD_NAME "micom_dispatch_thread"

class MicomDaemon : public INodeLooper, public INode
{
private:

    INodeBusService *mpNodeBusService;
    MicomBuffer     *mpMicomBuffer;
    NodeLooper      *mpLooper;
    MicomThread     *mpMicomThread;
    NodeAdapter     *mpNodeAdapter;

public:

    MicomDaemon();

    void start();

    virtual int looperCallback(const char* event);

    virtual int nodeCallback(void *data, unsigned int length, int type);
};

#endif
