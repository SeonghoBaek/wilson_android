/**
*
*  @author		Seongho Baek
*/
#define _LOG_TAG "NodeNetwork"

#include <pthread.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>

#include "Configure.h"
#include "NodeNetwork.h"
#include "XMLTypes.h"
#include "XMLParser.h"
#include "NodeBus.h"
#include "Lock.h"
#include "Types.h"

#define SOCK_PAGE_SIZE 4096

int NodeNetwork::receiveFromSocket(int sd, void* buffer, size_t bufferSize)
{
    //ssize_t recvSize = recv(sd, buffer, bufferSize, 0);
    ssize_t recvSize = read(sd, buffer, bufferSize);
    // If receive is failed
    if (-1 == recvSize)
    {
    	LOGE("Socket receive error: %s\n", strerror(errno));
    }

    return (int)recvSize;
}

int NodeNetwork::bindLocalSocketToName(int sd, const char* name)
{
    struct sockaddr_un address;

	const size_t nameLength = strlen(name);
	
	size_t pathLength = nameLength;

	// If name is not starting with a slash it is
	// in the abstract namespace
	bool abstractNamespace = ('/' != name[0]);
	
	// Abstract namespace requires having the first
	// byte of the path to be the zero byte, update
	// the path length to include the zero byte
	if (abstractNamespace)
	{
		pathLength++;
	}
	
	if (pathLength > sizeof(address.sun_path))
	{
		LOGE("Socket Path Too Long\n");

		return -1;
	}
	else
	{
		memset(&address, 0, sizeof(address));
	
		address.sun_family = PF_LOCAL;
	
		char* sunPath = address.sun_path;
	
		// First byte must be zero to use the abstract namespace
		if (abstractNamespace)
		{
			*sunPath++ = 0;
		}

		strcpy(sunPath, name);

		socklen_t addressLength = (offsetof(struct sockaddr_un, sun_path)) + (socklen_t)pathLength;
		
		// Unlink if the socket name is already binded
		unlink(address.sun_path);
		
		if (bind(sd, (struct sockaddr*) &address, addressLength) < 0)
		{
			LOGE("Bridge Bind Error\n");

			return -1;
		}
	}

	return 0;
}

int NodeNetwork::acceptOnSocket(int sd, struct ClientAddress* pClientAddr)
{
    struct sockaddr_storage client_addr;
    socklen_t clientLen = sizeof(client_addr);
	int clientSocket = -1;
	
    if (pClientAddr == NULL)
    {
        clientSocket = accept(sd, NULL, NULL);
    }
    else
    {
        clientSocket = accept(sd, (struct sockaddr *)&client_addr, &clientLen);
    }
    
	if (clientSocket < 0)
	{
		LOGE("%s", strerror(errno));
		return -1;
	}

    if (pClientAddr)
    {
        struct sockaddr_in addr_in;
        
        addr_in = *(struct sockaddr_in *)&client_addr;

        inet_ntop(AF_INET, &addr_in.sin_addr, pClientAddr->ipstr, (socklen_t)sizeof(pClientAddr->ipstr));
        
        pClientAddr->port = ntohs(addr_in.sin_port);
        
        LOGI("Client Address: %s : %d", pClientAddr->ipstr, pClientAddr->port);
    }
    
	return clientSocket;
}

int NodeNetwork::listenOnSocket(int sd, int backlog)
{
	if (listen(sd, backlog) < 0)
	{	
		LOGE("Local Socket Listen Error: %d %s\n", sd, strerror(errno));

		return -1;
	}

	return 0;
}

int NodeNetwork::sendToSocket(int sd, const void* buffer, size_t bufferSize)
{
	ssize_t sentSize = 0;
	int length = (int)bufferSize;
	int nrPages = length / SOCK_PAGE_SIZE + 1;
	int sizeToSend = 0;

	//sentSize = write(sd, buffer, bufferSize);

	for (int i = 0; i < nrPages; i++)
	{
		if (length < SOCK_PAGE_SIZE)
		{
			sizeToSend = length;
		}
		else
		{
			sizeToSend = SOCK_PAGE_SIZE;
		}

		//send(sd, buffer + i*SOCK_PAGE_SIZE, sizeToSend, 0);
		int sent = 0;

		do
		{
			int w = (int)write(sd, (char *)buffer + sent + i*SOCK_PAGE_SIZE, sizeToSend);

			if (w <= 0)
			{
				//LOGE("Send Failure.");
				sent = 0;
				break;
			}

			sent += w;
			sizeToSend -= w;

			//LOGI("Socket Write Size: %d", w);
		}
		while(sizeToSend);

		length -= sent;
		sentSize += sent;
	}

	return (int)sentSize;
}

int NodeNetwork::setupLocalNode(const char* names, INode* pINode)
{
	int localSocket = -1;
	char *buffer = NULL;
	//XMLParser* xmlParser = NULL;
	int __exit = 0;
    
	if ( (localSocket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		LOGE("Local Socket Creation Error: %s\n", strerror(errno));

		return -1;
	}

	//LOGI("Local Socket: %d %s\n", localSocket, names);

	if (bindLocalSocketToName(localSocket, names) < 0)
	{
		close(localSocket);

        LOGE("Binding Failure\n");
        
		return -1;
	}

	//LOGI("Binding Local Socket\n");

	if (listenOnSocket(localSocket, 4) < 0)
	{
		close(localSocket);

		return -1;
	}

	pINode->nodeCallback(NULL, 0, SETUP_MESSAGE);

	//xmlParser = XMLParser::getInstance();
	buffer = new char[ACTION_BUFFER_SIZE+1];

	if (buffer) memset(buffer, 0, ACTION_BUFFER_SIZE+1);

	while (!__exit)
	{
		// Wait client.
		int clientSocket = acceptOnSocket(localSocket, NULL);
        
        //LOGI("Accept local client");

		if (clientSocket < 0)
		{
			LOGE("Accept Local Socket Error\n");

			__exit = 1;
		}

		if (buffer == NULL) break;

		ssize_t recvSize = read(clientSocket, buffer, sizeof(unsigned int));
        
        //LOGI("recvSize: %d", (int)recvSize);

		if (recvSize <= 0)
		{
			close(clientSocket);
			continue;
		}

		int msgType = *(int *)buffer;

		if (msgType == KILL_MESSAGE)
		{
			//LOGI("KILL: %s", names);
			close(clientSocket);
			close(localSocket);

			localSocket = -1;
			pINode->nodeCallback(NULL, 0, KILL_MESSAGE);
			break;
		}
        
        //LOGI("MSG Type: %d", msgType);

    	recvSize = read(clientSocket, buffer, sizeof(unsigned int));

    	//LOGI("recvSize: %d", (int)recvSize);

    	if (recvSize > 0)
    	{
    		int length = *(int *)buffer;
    		
            //LOGI("XML Length: %d", length);
    		
            char *tempBuff = new char[length+1];
    		int sent = 0;

    		memset(tempBuff, 0, length+1);

    		do
    		{
    			recvSize = read(clientSocket, tempBuff + sent, length);

    			if (recvSize <= 0)
    			{
    				sent = 0;
    				break;
    			}

    			length -= recvSize;
    			sent += recvSize;
    		}
    		while(length);

    		//LOGI("Recv Size: %d", recvSize);
    		//tempBuff[sent] = '\0';
    		//LOGI("Str: %d, %s", sent, tempBuff);
    		if( sent )
    		{
    			sent = pINode->nodeCallback(tempBuff, sent, msgType);
    		}

    		//LOGI("Sent: %d", sent);

    		delete [] tempBuff;
    	}

    	close(clientSocket);
	}

	if (localSocket != -1) close(localSocket);

	//if (xmlParser) delete xmlParser;
	if (buffer) delete [] buffer;

	return 0;

}

int NodeNetwork::sendNodeMessage(int* s, const char *pNodeName, const void *data, unsigned int length, int type)
{
	int localSocket = *s;

	struct sockaddr_un address;
	const size_t nameLength = strlen(pNodeName);

	if (localSocket == -1)
    {
        if ((localSocket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0)
        {
            LOGE("Local Socket Creation Error: %s\n", strerror(errno));

            return -1;
        }

        size_t pathLength = nameLength;

        bool abstractNamespace = ('/' != pNodeName[0]);

        if (abstractNamespace)
        {
            pathLength++;
        }

        if (pathLength > sizeof(address.sun_path))
        {
            LOGE("Socket Path Too Long\n");
            close(localSocket);

            return -1;
        }
        else
        {
            memset(&address, 0, sizeof(address));

            address.sun_family = PF_LOCAL;

            char *sunPath = address.sun_path;

            // First byte must be zero to use the abstract namespace
            if (abstractNamespace)
            {
                *sunPath++ = 0;
            }

            strcpy(sunPath, pNodeName);

#if defined(MAC_OS_X)
            address.sun_len = sizeof(address);
            socklen_t addressLength = (socklen_t)SUN_LEN(&address);
#else
            socklen_t addressLength = (offsetof(struct sockaddr_un, sun_path)) + (socklen_t) pathLength;
#endif
            if (connect(localSocket, (struct sockaddr *) &address, addressLength) < 0)
            {
                //LOGE("Local Socket Connect Error\n");
                close(localSocket);

                return -1;
            }
        }

        *s = localSocket;
    }

	int sent = 0;

	sent = (int)write(localSocket, (void*)&type, sizeof(type));

	if (sent <= 0)
	{
		strerror(errno);
		close(localSocket);
		*s = -1;

		return -1;
	}

	if (type == KILL_MESSAGE)
	{
		close(localSocket);
		*s = -1;

		return 0;
	}

	sent = (int)write(localSocket, (void*)&length, sizeof(length));

	if (sent <= 0)
	{
		strerror(errno);
		close(localSocket);
		*s = -1;

		return -1;
	}

	int nrPages = length / SOCK_PAGE_SIZE + 1;
	int sizeToSend = 0;

	for (int i = 0; i < nrPages; i++)
	{
		if (length < SOCK_PAGE_SIZE)
		{
			sizeToSend = length;
		}
		else
		{
			sizeToSend = SOCK_PAGE_SIZE;
		}

		sent = 0;

		do
		{
			int w = (int)write(localSocket, (char *)data + sent + i*SOCK_PAGE_SIZE, sizeToSend);

			if (w <= 0)
			{
				strerror(errno);
				close(localSocket);
				*s = -1;

				return -1;
			}

			sent += w;
			sizeToSend -= w;
		}
		while(sizeToSend);

		length -= sent;
	}

	return 0;
}

int NodeNetwork::sendNodeMessage(const char *pNodeName, const void *data, unsigned int length, int type)
{
	int localSocket = -1;

	if ( (localSocket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		LOGE("Local Socket Creation Error: %s\n", strerror(errno));

		return -1;
	}

	struct sockaddr_un address;
	const size_t nameLength = strlen(pNodeName);

	size_t pathLength = nameLength;

	bool abstractNamespace = ('/' != pNodeName[0]);

	if (abstractNamespace)
	{
		pathLength++;
	}

	if (pathLength > sizeof(address.sun_path))
	{
		LOGE("Socket Path Too Long\n");
		close(localSocket);

		return -1;
	}
	else
	{
		memset(&address, 0, sizeof(address));

		address.sun_family = PF_LOCAL;

		char* sunPath = address.sun_path;

		// First byte must be zero to use the abstract namespace
		if (abstractNamespace)
		{
			*sunPath++ = 0;
		}

		strcpy(sunPath, pNodeName);

#if defined(MAC_OS_X)
		address.sun_len = sizeof(address);
		socklen_t addressLength = (socklen_t)SUN_LEN(&address);
#else
		socklen_t addressLength = (offsetof(struct sockaddr_un, sun_path)) + (socklen_t)pathLength;
#endif
		if (connect(localSocket, (struct sockaddr*) &address, addressLength) < 0)
		{
			LOGE("Local Socket Connect Error\n");
			close(localSocket);

			return -1;
		}
	}

	int sent = 0;

	sent = (int)write(localSocket, (void*)&type, sizeof(type));

	if (sent <= 0)
	{
		strerror(errno);
		close(localSocket);
		return -1;
	}

	if (type == KILL_MESSAGE)
	{
		close(localSocket);
		return 0;
	}

	sent = (int)write(localSocket, (void*)&length, sizeof(length));

	if (sent <= 0)
	{
		strerror(errno);
		close(localSocket);
		return -1;
	}

	int nrPages = length / SOCK_PAGE_SIZE + 1;
	int sizeToSend = 0;

	for (int i = 0; i < nrPages; i++)
	{
		if (length < SOCK_PAGE_SIZE)
		{
			sizeToSend = length;
		}
		else
		{
			sizeToSend = SOCK_PAGE_SIZE;
		}

		sent = 0;

		do
		{
			int w = (int)write(localSocket, (char *)data + sent + i*SOCK_PAGE_SIZE, sizeToSend);

			if (w <= 0)
			{
				strerror(errno);
				close(localSocket);
				return -1;
			}

			sent += w;
			sizeToSend -= w;
		}
		while(sizeToSend);

		length -= sent;
	}

	close(localSocket);

    return 0;
}

int NodeNetwork::connectGlobal(const char *address, int port)
{
	struct sockaddr_in server_addr;
	int sock;

	memset(&server_addr, 0, sizeof(server_addr));
    
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr(address);

    LOGI("Connect to Server...\n");
	sock = socket(PF_INET, SOCK_STREAM, 0);

	//setsockopt(sock, SOL_TCP, TCP_NODELAY, (char *) &flag, sizeof(int));

	if (sock < 0)
    {
        LOGE("socket -1\n");
        
        return -1;
    }
    
	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
        LOGE("server connect error: %s :%d, %s\n", address, port, strerror(errno));
		return -1;
	}

	return sock;
}

int NodeNetwork::setupServerSocket(const char *address, int port)
{
	int sock = 0;
	int socketKeepAlive = TRUE;
	int socketReuseAddress = TRUE;

	struct sockaddr_in server_addr;

	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	sock = socket(PF_INET, SOCK_STREAM, 0);

	if (sock < 0)
	{
		LOGE("%s", strerror(errno));
		return -1;
	}

	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &socketKeepAlive, sizeof socketKeepAlive);
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &socketReuseAddress, sizeof socketReuseAddress);

	if (strcmp(address, "*") != 0)
	{
		server_addr.sin_addr.s_addr = inet_addr(address);

		if (server_addr.sin_addr.s_addr == INADDR_NONE)
		{
			LOGE("Invalid server address.\n");
			return -1;
		}
	}
	else
	{
		server_addr.sin_addr.s_addr = INADDR_ANY;
	}

	if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		LOGE("Server Socket Bind Error\n");
		return -1;
	}

	if (listen(sock, 10) < 0)
	{
		LOGE("Server Socket Listen Error\n");
		return -1;
	}

	return sock;
}
