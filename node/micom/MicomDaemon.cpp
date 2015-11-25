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

int micom_callback(void *data, unsigned int length, int type)
{
    _GOODNESS(data, -1);

    /*
     * if type == MICOM_MESSAGE, we received from micom
     * else we received from other node(system), send to micom
     */

    if (type == MICOM_MESSAGE)
    {
        // Received from MICOM
        char buff[6];
        memset(buff, 0, 6);
        memcpy(buff, data, 6);
        LOGI("Received from MICOM: %s", buff);
    }
    else if ( type == MICOM_REQUEST_MESSAGE)
    {
        // Send data to MICOM
    }

    return 0;
}

int NodeLooperCallback(const char* event)
{
    // Received from looper.
    // No need to use, just ignore.

    return 0;
}

int main(int argc, char **argv)
{
    NodeLooper *pLooper = new NodeLooper(NodeLooperCallback);
    Mutex_t lock = Lock::createMutex();

    /* Craete NodeBus Node with NodeAdapter Class */
    NodeAdapter* pNodeAdapter = NodeBus::createLocalNodeAdapter(MICOM_NODE_NAME, micom_callback);
    /* Run Node Thread */
    pNodeAdapter->run();
    /* Join to default service */
    INodeBusService *pService = NodeBus::getDefaltServiceInterface();  // Get service interface
    pService->join(pNodeAdapter);  // Join to service.

    // Create Micom Thread
    MicomThread *mt = new MicomThread(pNodeAdapter);

    mt->setThreadName(MICOM_DISPATCH_THREAD_NAME);
    mt->startThread();

    pLooper->wait(-1);

    exit(EXIT_SUCCESS);
}