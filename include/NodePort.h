/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef NODEPORT_H_
#define NODEPORT_H_

#include "Thread.h"
#include "List.h"
#include "Configure.h"
#include "NodeBus.h"

#include <stdio.h>

#define USE_NODE_CREATION_THREAD 1
#define USE_IP_CHECKER 0

class DefaultService;
class DefaultGlobalService;

class NodePort : public Thread
{
	private:

		// Receive packet from local node and send to global server.
		//class Router : public LocalNode, public INode
		class Router : public NodeAdapter, public INode
		{
			private:
				NodePort *mpOwner;

			public:
				Router(NodePort *pOwner) : NodeAdapter((char *) LOCAL_ROUTER_NAME, this)
				{
					this->mpOwner = pOwner;
					//this->setActionImpl(this);
					//this->setName((char *) LOCAL_ROUTER_NAME);
					//this->setThreadName((char *)"NodePort::Router");
                    
				}

				virtual ~Router() {}

				int routerAction(void *data, unsigned int length, int type);

				int nodeCallback(void *data, unsigned int length, int type)
				{
					return routerAction(data, length, type);
				}
		};

		int	 mSd;

		char mHostAddress[NODE_IP_LENGTH];

		DefaultService* mpOwner;
		Router *mpRouter;

	public:

		IMPLEMENT_THREAD(listen())

		int listen();

		int connect(const char* ip, int port);

		int send(void *data, int length);

		void run();

		void setOwner(DefaultService *pOwner) { this->mpOwner = pOwner; }

		NodePort();

		virtual ~NodePort() {}
};

class ServerNodePort : public Thread
{
	private:

		DefaultGlobalService *mpOwner;

	public:

		IMPLEMENT_THREAD(waitNodeConnection())

		int waitNodeConnection();

		void setOwner(DefaultGlobalService *pOwner) { this->mpOwner = pOwner; }

		ServerNodePort();

		virtual ~ServerNodePort() {};
};

#endif /* NODEPORT_H_ */
