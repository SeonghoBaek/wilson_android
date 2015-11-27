/**
*
*  @author		Seongho Baek
*
*/

#ifndef _NODENETWORK_H_
#define _NODENETWORK_H_

#include "Types.h"
#include "Lock.h"
    
class INode;
class NodeNetwork
{
	public:
		static NodeNetwork* createAdapter() { return new NodeNetwork();}

		//static int sendLocalMessage(const char *pNodeName, const void *manifest, unsigned int length);

		static int sendNodeMessage(const char *pNodeName, const void *data, unsigned int length, int type);

		static int connectGlobal(const char *address, int port);

		static int sendToSocket(int sd, const void* buffer, size_t bufferSize);

		static int receiveFromSocket(int sd, void* buffer, size_t bufferSize);

		static int setupServerSocket(const char *address, int port);

		static int acceptOnSocket(int sd, struct ClientAddress* pClientAddr);

		static int listenOnSocket(int sd, int backlog);

		static int setupLocalNode(const char* names, INode *pINode);

		static int bindLocalSocketToName(int sd, const char* name);

		virtual ~NodeNetwork();

	private:
		NodeNetwork() {}
};

#endif
