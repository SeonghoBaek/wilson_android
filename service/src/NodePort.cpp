/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#include "NodeNetwork.h"
#include "Types.h"
#include "NodePort.h"
#include "Log.h"
#include "DefaultService.h"
#include "Packet.h"
#include "Configure.h"
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/file.h>

unsigned int gGlobalClientID = 0;

int NodePort::Router::routerAction(void *data, unsigned int length, int type)
{
	//LOGI("Router: %d", length);

	int sent = this->mpOwner->send(data, length);

	return sent;
}

NodePort::NodePort()
{
	mSd = -1;
	mpOwner = NULL;
	mpRouter = new NodePort::Router(this);

	this->setThreadName((char *)"NodePort");
}

void NodePort::run()
{
    LOGI("NodePort::run\n");
	this->startThread();
    
	//mpRouter->startThread();
	mpRouter->run();
    
	//this->mpOwner->join(mpRouter->getNamespace());
	this->mpOwner->join(mpRouter->getNode()->getNamespace());
}

int NodePort::listen()
{
	int nReadBytes = 0;
	void *buff = new char[ACTION_BUFFER_SIZE+1];
	char *pUniqueClientName = new char[NODE_NAME_LENGTH+1];

	sockaddr_in localSockAddr;
    socklen_t addrLen = sizeof(localSockAddr);
    
    LOGI("NodePort::listen\n");
    
    getsockname(this->mSd, (sockaddr*) &localSockAddr, &addrLen);
    
    LOGI("NodePort::listen\n");
    
    char* localIp = inet_ntoa(localSockAddr.sin_addr);

    LOGI("Client Local IP: %s", localIp);

	/* Start Register */

	nReadBytes = (int)read(this->mSd, buff, ACTION_BUFFER_SIZE);

    LOGI("NodePort read bytes: %d\n", nReadBytes);
    
	if (nReadBytes <= 0)
	{
		delete [] (char *)buff;
		delete [] pUniqueClientName;

		LOGE("Connection Closed.\n");

		if (this->mSd > 0) close(this->mSd);
		this->mSd = -1;

		return -1;
	}

	LOGI("Client External IP: %s", buff);

	sprintf(pUniqueClientName, "%s_%s", localIp, (char *)buff);

	// Send local ip : external ip : port
	this->send(pUniqueClientName, NODE_NAME_LENGTH);

	delete [] pUniqueClientName;
	delete [] (char *)buff;

	int packetPreffixLength = NBUS_PACKET_HEADER_LEGNTH_BYTES + strlen(NBUS_PACKET_START_CODE);
    char *packetPreffix = new char[packetPreffixLength];

	for (;;)
	{
		int readBytes = 0;
		int r = 0;
		int sizeToRead = packetPreffixLength;
		do
		{
			r = (int)read(this->mSd, packetPreffix + readBytes, sizeToRead);

			if (r <= 0)
			{
				readBytes = 0;
				break;
			}

			readBytes += r;
			sizeToRead -= r;
		}
		while(sizeToRead);

		if (readBytes <= 0)
		{
			// Connection Broken
			// Fail action here.
			LOGE("Connection Closed.\n");

			if (this->mSd > 0) close(this->mSd);
			this->mSd = -1;

			break;
		}

		if (strncmp(packetPreffix, (char *)NBUS_PACKET_START_CODE, strlen(NBUS_PACKET_START_CODE)))
		{
			LOGE("Invalid Start Code");
			continue;
		}

		int headerLength = 0;

		headerLength = *(int *)(strlen(NBUS_PACKET_START_CODE) + packetPreffix);

		char *packetHeader = new char[headerLength];

		readBytes = (int)read(this->mSd, packetHeader, headerLength);

		if (readBytes <= 0)
		{
			// Connection Broken
			// Fail action here.
			LOGE("Connection Closed.\n");

			if (this->mSd > 0) close(this->mSd);
			this->mSd = -1;

			delete [] packetHeader;

			break;
		}

		NBUSPacket packet;

		if (packet.parseHeader(packetHeader, headerLength) < 0)
		{
			LOGE("Packet Error");
			delete [] packetHeader;
			continue;
		}

		memcpy(packet.getPacket(), packetPreffix, packetPreffixLength);

		delete [] packetHeader;

		if (packet.getDataLength())
		{
			readBytes = 0;
			sizeToRead = packet.getDataLength();
			do
			{
				int r = (int)read(this->mSd, (char *)packet.getData() + readBytes, sizeToRead);

				if ( r <= 0 )
				{
					readBytes = 0;
					break;
				}

				readBytes += r;
				sizeToRead -= r;
			}
			while(sizeToRead);

			if (readBytes <= 0)
			{
				// Connection Broken
				// Fail action here.
				LOGE("Connection Closed.\n");

				if (this->mSd > 0) close(this->mSd);
				this->mSd = -1;

				delete [] packetHeader;

				break;
			}
			else if (readBytes != packet.getDataLength())
			{
				LOGE("Packet Data Error");
				delete [] packetHeader;
				continue;
			}
		}

		NBUS_CMD item;

        item.mpData = new char[packet.getPacketLength()];
		memcpy(item.mpData, packet.getPacket(), packet.getPacketLength());
		item.mLength = packet.getPacketLength();
		item.mType = GLOBAL_MESSAGE;

		LOGI("packet: %s", packet.getHeader());
		// Queueing
		this->mpOwner->mQ.push(item);
		this->mpOwner->mpLooper->wakeup();
	}

	if (packetPreffix) delete packetPreffix;

	return 0;
}

int NodePort::send(void *data, int length)
{
	int nBytes = NodeNetwork::sendToSocket(this->mSd, data, length);

	LOGI("Send to wilson server: %d bytes", nBytes);
	return nBytes;
}

int NodePort::connect(const char *ip, int port)
{
	int sock = NodeNetwork::connectGlobal(ip, port);

	if (sock < 0) return -1;

	this->mSd = sock;

	return sock;
}