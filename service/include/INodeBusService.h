/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef __INODEBUSSERVICE_H
#define __INODEBUSSERVICE_H

#include "Configure.h"
#include "Types.h"
#include "Packet.h"

class Node;
class NodeAdapter;

class INodeBusService
{
	private:

		char *mpServiceName;

	public:

		INodeBusService(const char *serviceName)
		{
			mpServiceName = new char[SERVICE_NAME_LENGTH];
			memset(mpServiceName, 0, SERVICE_NAME_LENGTH);
			strncpy(mpServiceName, serviceName, SERVICE_NAME_LENGTH);
		}

		virtual ~INodeBusService() { if (mpServiceName) delete [] mpServiceName; }

		int drop(const char *pNodeName);

		int cast(const char *nodeName, void *manifest, unsigned int length);

		int broadcast(void *manifest, unsigned int length);

		int broadcast(void *manifest, unsigned int length, int type);

		int join(Node* pNode);

		int join(NodeAdapter *pNodeAdapter);

		char* getNodeList(const char* pResultReceiver);
    
        int setDefaultNode(const char *nodeName);
};

class IDefaultNodeBusService : public INodeBusService
{
	public:
		IDefaultNodeBusService(): INodeBusService(DEFAULT_LOCAL_SERVICE_NAME) {}
		virtual ~IDefaultNodeBusService() {}
};

class ICustomNodeBusService : public INodeBusService
{
	public:
		ICustomNodeBusService(const char* serviceName) : INodeBusService(serviceName) {}
		virtual ~ICustomNodeBusService() {}
};

#endif
