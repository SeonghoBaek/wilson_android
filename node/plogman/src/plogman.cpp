/**
*
*  @author		Seongho Baek(major.baek@lge.com)
*  @date		2014.08.12
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "NodeBus.h"
#include "Bridge.h"
#include "XMLParser.h"
#include "Configure.h"
#include "Packet.h"
#include "NodeLooper.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PLOGMAN_NODE_NAME DOMAIN_PREFIX "plogman.node"

#define START "start"
#define STOP "stop"

// Global

INodeBusService *gpService = NULL;
NodeLooper *gpLooper;
int logFd = -1;
bool gSendLog = FALSE;
Mutex_t gLock;

int nbus_callback(void *data, unsigned int length, int type)
{
	_GOODNESS(data, -1);

    LOGI("plogman received: %s", (char *)data);

    if (strncmp((char *)data, (char *)START, strlen(START)) == 0)
    {
    	if (logFd == -1)
    	{
    		logFd = open("/opt/log/test.log", O_RDONLY, 0755);
    	}

    	LOCK(gLock)
		{
			gSendLog = TRUE;
			gpLooper->wakeup();
		}
    }
    else
    {
    	LOCK(gLock)
		{
			gSendLog = FALSE;
			gpLooper->wakeup();
		}
    }

	return 0;
}

int NodeLooperCallback(const char* event)
{
	LOCK(gLock)
	{
		if (gSendLog != TRUE) return -1;
	}

	if (strncmp(event, LOOPER_EXIT, LOOPER_EVT_LENGTH) == 0)
	{
		return -1;
	}

	// Read log data and send to router.
	// Start Log Sending
	char *raw = new char[1024 * 8];

	int nrRead = read(logFd, raw, 1024*8);

	if (nrRead <= 0)
	{
		close(logFd);
		logFd = open("/opt/log/test.log", O_RDONLY, 0755);
	}

	// Send Test Log.
	EventXML eventXml;

	eventXml.setType(EVT_KEYWORD(LOG));
	eventXml.setFormat(LOG_KEYWORD(TXT));
	eventXml.setLength(1024*16);
	char *header = eventXml.toXML();

	/* Create packet.*/
	NBUSPacket logPacket;

	logPacket.copyHeader(header, (int)strlen((char *)header)); // Copy header XML to packet
	logPacket.copyData(raw, 1024*16);  // Copy data to packet

	//LOGI("packet header: %s", logPacket.getPacket());

	gpService->cast(LOCAL_ROUTER_NAME, logPacket.getPacket(), logPacket.getPacketLength());

	delete [] header;
	delete [] raw;

	return 0;
}

int main()
{
	gpLooper = new NodeLooper(NodeLooperCallback);
	gLock = Lock::createMutex();

	 /* Craete NodeBus Node with NodeAdapter Class */
	NodeAdapter* pNodeAdapter = NodeBus::createLocalNodeAdapter(PLOGMAN_NODE_NAME, nbus_callback);
	/* Run Node Thread */
	pNodeAdapter->run();
    /* Join to default service */
	gpService = NodeBus::getDefaltServiceInterface();  // Get service interface
	gpService->join(pNodeAdapter);  // Join to service.

	gpLooper->setTimeOut(3000);
	gpLooper->timer();

	if (logFd > 0) close(logFd);

	return 0;
}


#ifdef __cplusplus
}
#endif
