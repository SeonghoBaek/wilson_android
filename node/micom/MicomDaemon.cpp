//
// Created by major on 9/7/15.
//
#define _LOG_TAG "micomd"

#include <stdlib.h>
#include "Config.h"
#include "Types.h"
#include "NodeLooper.h"
#include "MicomDaemon.h"
#include "MicomThread.h"
#include "MicomBuffer.h"

MicomDaemon::MicomDaemon()
{
    this->mpLooper = new NodeLooper(this);
    this->mpMicomBuffer = new MicomBuffer(this->mpLooper);
    this->mpMicomThread = new MicomThread(this->mpMicomBuffer);

    /* Craete nodebus node for send/receive to/from other node(eg.java service) */
    this->mpNodeAdapter = NodeBus::createLocalNodeAdapter(MICOM_NODE_NAME, this);

    this->mpNodeBusService = NodeBus::getDefaltServiceInterface();  // Get default nodebus master server interface.
}

void MicomDaemon::start()
{
    this->mpNodeAdapter->start(); // Ready to receive nodebus message.

    this->mpNodeBusService->join(this->mpNodeAdapter); // Join to nodebus.

    this->mpMicomThread->setThreadName(MICOM_DISPATCH_THREAD_NAME);
    this->mpMicomThread->start();

    this->mpLooper->wait(-1);
}

/*
 * MicomBuffer Callback.
 * - MicomBuffer get data, calls back this method.
 * - Pop buffed data here.
 * - event: call back state. Typically, LOOPER_WAKEUP. You can ignore this.
 */
int MicomDaemon::looperCallback(const char *event)
{
    NBUS_CMD *cmd = this->mpMicomBuffer->popclear(); // Get stack top and flush all.

    /*
     * Build micom JSON message.
     * { "id": "micom", "text":"micom data" }
     */

    char micomMessage[MICOM_MESSAGE_LENGTH];

    memset(micomMessage, 0, MICOM_MESSAGE_LENGTH);

    char micomPacket[MICOM_PACKET_LENGTH];

    memset(micomPacket, 0, MICOM_PACKET_LENGTH);

    memcpy(micomPacket, cmd->mpData, MICOM_PACKET_LENGTH);

    sprintf(micomMessage, "{\"id\":\"%d\",\"text\":\"%s\"",0, micomPacket);


    this->mpNodeBusService->cast(MICOM_SVC_NAME, cmd->mpData, cmd->mLength, MICOM_MESSAGE);

    return 0;
}

/*
 * Other node send message to micom node.
 */
int MicomDaemon::nodeCallback(void *data, unsigned int length, int type)
{
    _GOODNESS(data, -1);

    switch (type)
    {
        case MICOM_REQUEST_MESSAGE:
        {
            // Set control data to micom h/w
        }
        break;
        default:
        break;
    }

    return 0;
}

int main(int argc, char **argv)
{
    MicomDaemon micomDaemon;

    micomDaemon.start();

    exit(EXIT_SUCCESS);
}