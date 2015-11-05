/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#include "Bridge.h"
#include "Log.h"
#include "Configure.h"
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>

#define BUFFER_SIZE 512
#define BRIDGE_TIME_OUT 4000 

int DomainBridge::receiveFromSocket(int sd, char* buffer, int size)
{
    ssize_t recvSize = recv(sd, buffer, size - 1, 0);
    // If receive is failed
    if (-1 == recvSize)
    {
    	LOGE("Socket receive error\n");
    }
	else 
	{
        // NULL terminate the buffer to make it a string
        buffer[recvSize] = 0;
	}

    return (int)recvSize;
}

int DomainBridge::sendToDomain(char *name, char* buff, int size)
{
	int localSocket = -1;

	if ((localSocket = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		LOGE("Local Socket Creation Error\n");

		return -1;
	}

	struct sockaddr_un address;
	const size_t nameLength = strlen(name);

	size_t pathLength = nameLength;

	bool abstractNamespace = ('/' != name[0]);

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

		strcpy(sunPath, name);

#if defined(MAC_OS_X)
        address.sun_len = sizeof(address);
        socklen_t addressLength = (socklen_t)SUN_LEN(&address);
#else
		socklen_t addressLength = (offsetof(struct sockaddr_un, sun_path)) + (socklen_t)pathLength;
#endif

		if (connect(localSocket, (struct sockaddr*) &address, addressLength) < 0)
		{
			LOGE("Bridge: Local Socket Connect Error\n");
			close(localSocket);

			return -1;
		}
	}

	send(localSocket, buff, strlen(buff), 0);

	close(localSocket);

	return 0;
}

int DomainBridge::bindLocalSocketToName(int sd, const char* name)
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
#if defined(MAC_OS_X)
        address.sun_len = sizeof(address);
        socklen_t addressLength = (socklen_t)SUN_LEN(&address);
#else
		socklen_t addressLength = (offsetof(struct sockaddr_un, sun_path)) + (socklen_t)pathLength;
#endif
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

int DomainBridge::createSocket()
{
	int sd = -1;

	if ((sd = socket(PF_LOCAL, SOCK_STREAM, 0)) < 0)
	{
		LOGE("SocketBridge Local Socket Creation Error\n");
	}

	return sd;
}

int DomainBridge::thread()
{
	int localSocket = -1;
	char *buffer = NULL;
	int __exit = 0;

	if ((localSocket = createSocket()) < 0)
	{
		LOGE("Local Socket Creation Error\n");

		return -1;
	}

	//LOGI("Local Socket: %d %s\n", localSocket, pNode->getNamespace());

	if (bindLocalSocketToName(localSocket, this->in) < 0)
	{
		close(localSocket);

		return -1;
	}

	//LOGI("Binding Local Socket\n");

	if (listen(localSocket, 4) < 0)
	{
		close(localSocket);

		return -1;
	}

	//LOGI("Listen Local Socket\n");

	buffer = new char[BUFFER_SIZE];

	while (!__exit)
	{
		// Wait client.
		int clientSocket = accept(localSocket, NULL, NULL);

		if (clientSocket < 0)
		{
			LOGE("Accept Local Socket Error\n");

			__exit = 1;
		}

		ssize_t recvSize = receiveFromSocket(clientSocket, buffer, BUFFER_SIZE - 1);

		close(clientSocket);

		if (recvSize > 0)
		{
			LOCK(lock)
			{
				if (this->bsync == true) 
				{
					this->looper.wakeup();
					this->bsync = false;
				}
			}

			if (strcmp(buffer, "EXIT") == 0) __exit = 1;

			if (this->action) this->action(buffer, (int)recvSize);
		}
	}

	close(localSocket);

	delete [] buffer;

	return 0;
}

int DomainBridge::cross(void *stream, int length)
{
	int stat = -1;

	if (this->out == NULL ) return stat;

	LOCK(lock)
	{
		stat = sendToDomain(this->out, (char *)stream, length);
	}

	return stat;
}

int DomainBridge::cross(void *stream, int length, bool sync)
{
	this->bsync = sync;
	
	int res = cross(stream, length);

	if (sync == TRUE)
	{
		this->looper.wait(BRIDGE_TIME_OUT); // time out
	}

	return res;
}

int DomainBridge::cross(char *receiver, void *stream, int length)
{
	int stat = sendToDomain(receiver, (char *)stream, length);

	return stat;
}

