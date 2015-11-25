/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/
#define _LOG_TAG "INodeBusService"

#include <stdio.h>
#include <string.h>

#include "Configure.h"
#include "Log.h"
#include "XMLParser.h"
#include "XMLTypes.h"
#include "NodeBus.h"
#include "Types.h"
#include "Packet.h"

int INodeBusService::drop(const char *pNodeName)
{
	_GOODNESS(pNodeName, -1);

	char* pXml = NULL;

	NBUSXML nbusXml;

	nbusXml.setNodeName(pNodeName);
	nbusXml.setType(NBUS_TYPE_REQUEST);
	nbusXml.setId(NBUS_CMD_DROP);

	pXml = nbusXml.toXML();

	NodeNetwork::sendNodeMessage(this->mpServiceName, pXml, (unsigned int)strlen(pXml), LOCAL_MESSAGE);

	delete [] pXml;

	return 0;
}

int INodeBusService::cast(const char *pNodeName, void *manifest, unsigned int length)
{
	_GOODNESS(pNodeName, -1);
	_GOODNESS(manifest, -1);

	if (NodeNetwork::sendNodeMessage(pNodeName, manifest, length, LOCAL_MESSAGE) < 0)
	{
		LOGE("Node Closed: Dropping %s", pNodeName);

		this->drop(pNodeName);
	}

	return 0;
}

int INodeBusService::broadcast(void *manifest, unsigned int length, int type)
{
	_GOODNESS(manifest, -1);

	NodeNetwork::sendNodeMessage(this->mpServiceName, manifest, length, type);

	return 0;
}

int INodeBusService::broadcast(void *manifest, unsigned int length)
{
	_GOODNESS(manifest, -1);

	NodeNetwork::sendNodeMessage(this->mpServiceName, manifest, length, LOCAL_MESSAGE);

	return 0;
}

int INodeBusService::join(NodeAdapter* pNodeAdapter)
{
	_GOODNESS(pNodeAdapter, -1);

	return join(pNodeAdapter->getNode());
}

int INodeBusService::join(Node* pNode)
{
	_GOODNESS(pNode, -1);

	char* pXml = NULL;
	NBUSXML nbusXml;

	nbusXml.setNodeName(pNode->getNamespace());
	nbusXml.setType(NBUS_TYPE_REQUEST);
	nbusXml.setId(NBUS_CMD_JOIN);

	pXml = nbusXml.toXML();

	NodeNetwork::sendNodeMessage(this->mpServiceName, pXml, (unsigned int)strlen(pXml), LOCAL_MESSAGE);

	delete [] pXml;

	return 0;
}

int INodeBusService::setDefaultNode(const char *nodeName)
{
    _GOODNESS(nodeName, -1);
    
    char *pXml = NULL;
    NBUSXML nbusXml;
    
    nbusXml.setNodeName(nodeName);
    nbusXml.setType(NBUS_TYPE_EVENT);
    nbusXml.setId(NBUS_EVENT_SET_DEFAULT_NODE);
    
    pXml = nbusXml.toXML();
    
    //LOGI("xml: %s", pXml);
    
    NodeNetwork::sendNodeMessage(this->mpServiceName, pXml, (unsigned int)strlen(pXml), LOCAL_MESSAGE);
    
    delete [] pXml;
    
    return 0;
}
    
char* INodeBusService::getNodeList(const char* pResultReceiver)
{
	NodeLooper looper;
	NBUSXML nbusXml;
    char *pBuff = new char[NODEBUS_MSG_BUFFER_SIZE];
    
    pBuff[0] = 0;
    
    nbusXml.setSharedBufferAddr(pBuff);
	nbusXml.setType(NBUS_TYPE_REQUEST);
	nbusXml.setId(NBUS_CMD_LIST);
	nbusXml.setNodeName(pResultReceiver);
	nbusXml.setSyncId(looper.getId());

	char *pStrXml = nbusXml.toXML();

	LOGI("List XML: %s", pStrXml);
    
	NodeNetwork::sendNodeMessage(this->mpServiceName, pStrXml, (unsigned int)strlen(pStrXml), LOCAL_MESSAGE);
    
	looper.wait(-1);
    
    delete [] pStrXml;
	
    return pBuff;
}

