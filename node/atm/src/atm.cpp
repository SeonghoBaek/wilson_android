/**
*
*  @author		Seongho Baek
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
#include "Types.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// Global
#define ATM_NODE_NAME DOMAIN_PREFIX "atm.node"
#define PLOGMAN_NODE_NAME DOMAIN_PREFIX "plogman.node"

INodeBusService *gpService = NULL;

int logFd = 0;

int nodeGlobalCallback(void *data, unsigned int length)
{
	NBUSPacket packet;

	packet.parserPacket((char *)data, length);

	IXML *pXml = packet.getXML();

	if (pXml->getXMLType() == XML_TYPE_COMMAND)
	{
		CommandXML* pCmdXml = (CommandXML *)pXml;

		switch (pCmdXml->getType())
		{
			case CMD_TYPE_STAY:
			{

			}
			break;
			case CMD_TYPE_INFO:
			{
				EventXML eventXml;

				char* raw = (char *)"NodeAdapter* pNodeAdapter = NodeBus::createLocalNodeAdapter(ATM_NODE_NAME, nbus_callback);pNodeAdapter->run();\
				//gpBridge = new DomainBridge(ATM_BRIDGE_NAME, TARGET_BRIDGE_NAME, bridge_callback);//gpBridge->run();gpService = NodeBus::getDefaltServiceInterface();\
				gpService->join(pNodeAdapter, BUS_JOIN_FORCE);//char* pListXml = gpService->getNodeList(ATM_NODE_NAME);NBUSPacket packet;EventXML eventXml;eventXml.setType(EVT_KEYWORD(LOG));eventXml.setFormat(LOG_KEYWORD(TXT));eventXml.setLength(1024);char *header = eventXml.toXML();";

				eventXml.setType(EVT_KEYWORD(INFO));
				eventXml.setFormat(LOG_KEYWORD(JSON));
				eventXml.setLength((unsigned int)strlen(raw));

				char *header = eventXml.toXML();

				packet.copyHeader(header,(int)strlen(header));
				packet.copyData(raw, (unsigned int)strlen(raw));

				gpService->cast(LOCAL_ROUTER_NAME, packet.getPacket(), packet.getPacketLength());
			}
			break;
			case CMD_TYPE_LOGOFF:
			{
				LOGI("Received CMD_TYPE_LOGOFF");
				// Stop Log Sending.
				char *data = (char*)"stop";
				gpService->cast(PLOGMAN_NODE_NAME, data, strlen(data)+1);
			}
			break;
			case CMD_TYPE_LOG:
			{
				LOGI("Received CMD_TYPE_LOG");
				char *data = (char*)"start";
				gpService->cast(PLOGMAN_NODE_NAME, data, strlen(data)+1);
			}
			break;
			case CMD_TYPE_IMG:
			case CMD_TYPE_STOP:
			case CMD_TYPE_PAUSE:
			case CMD_TYPE_WAIT:
			case CMD_TYPE_RESUME:
			case CMD_TYPE_EPK:
			case CMD_TYPE_LUA:
			case CMD_TYPE_FILE:
			case CMD_TYPE_KEY:
			case CMD_TYPE_RPC:
			case CMD_TYPE_ARPC:
			case CMD_TYPE_TC:
			case CMD_TYPE_SH:
			case CMD_TYPE_CFG:
			{
			}
			break;
			default:
			break;
		}
	}
	else if (pXml->getXMLType() == XML_TYPE_EVENT)
	{
		EventXML *pEventXml = (EventXML *)pXml;

		switch (pEventXml->getType())
		{
			case EVT_TYPE_REGOK:
			{
				// Send Boot Log
				LOGI("Received EVT_TYPE_REGOK");
			}
			break;
			case EVT_TYPE_REGFAIL:
			{
				LOGW("Received EVT_TYPE_REGFAIL");
			}
			break;
			default:
			break;
		}
	}

	return 0;
}

int nodeCallback(void *data, unsigned int length, int type)
{
	if (type == GLOBAL_MESSAGE) return nodeGlobalCallback(data, length);

	//LOGI("Local Message Arrived: %s", (char *)data);

	return 0;
}

int main()
{
	logFd = open("/tmp/log/test.log", O_RDONLY, 0755);

    /* Craete NodeBus Node with NodeAdapter Class */
	NodeAdapter* pNodeAdapter = NodeBus::createLocalNodeAdapter(ATM_NODE_NAME, nodeCallback);

	/* Run Node Thread */
	pNodeAdapter->run();
    
    /* Join to default service */
	gpService = NodeBus::getDefaltServiceInterface();  // Get service interface
	gpService->join(pNodeAdapter);  // Join to service.
    gpService->setDefaultNode(ATM_NODE_NAME);    // Set default node. All global network message will be sent to this node.
    
    /*Default Node should report to server that this client is ready to run. */
    EventXML eventXml;

	eventXml.setType(EVT_TYPE_READY);

	char *header = eventXml.toXML();
	NBUSPacket packet;

	packet.copyHeader(header,(int)strlen(header));

	gpService->cast(LOCAL_ROUTER_NAME, packet.getPacket(), packet.getPacketLength());

	delete [] header;

    /* Get node list in current service.*/
    //char* pListXml = gpService->getNodeList(ATM_NODE_NAME);

    //LOGI("LIST:%s", pListXml);
    //delete [] pListXml;

    LOGI("ATMNODE DAEMON");
	NodeLooper looper;

	looper.wait(-1); // Set daemon

	close(logFd);

	return 0;
}

#ifdef __cplusplus
}
#endif
