
/*
 * Packet.cpp
 *
 *  Created on: Aug 29, 2014
 *      Author: major
 */


#include <string.h>
#include "Packet.h"
#include "Log.h"

NBUSPacket::~NBUSPacket()
{
	if (this->mpPacket) delete [] (char *)this->mpPacket;
    if (this->mpXml) delete mpXml;
}

int NBUSPacket::parseHeader(void *data, unsigned int length)
{
	XMLParser *pXmlParser = XMLParser::getInstance();

	IXML *pIXml = pXmlParser->parseXML((char *)data, length);

	if (pIXml == NULL)
	{
		delete pXmlParser;
		LOGE("Header XML corruption");
		return -1;
	}

	this->mHeaderLength = length;
	this->mpXml = pIXml;

	if (pIXml->getXMLType() == XML_TYPE_EVENT)
	{
		EventXML *pEventXml = (EventXML *)pIXml;

		this->mpXml = pEventXml;

		this->mDataLength = pEventXml->getLength();

		this->mPacketLength = strlen(NBUS_PACKET_START_CODE) + NBUS_PACKET_HEADER_LEGNTH_BYTES + this->mHeaderLength + this->mDataLength;
		this->mpPacket = new char[this->mPacketLength];

		memcpy(this->mpPacket, NBUS_PACKET_START_CODE, strlen(NBUS_PACKET_START_CODE));

		this->mpHeader = (char *)this->mpPacket + strlen(NBUS_PACKET_START_CODE) + NBUS_PACKET_HEADER_LEGNTH_BYTES;

		memcpy(this->mpHeader, data, length);

		if (this->mDataLength)
		{
			this->mpData = (char *)this->mpHeader + this->mHeaderLength;
		}
	}
	else if (pIXml->getXMLType() == XML_TYPE_COMMAND)
	{
		CommandXML *pCmdXml = (CommandXML *)pIXml;

		this->mpXml = pCmdXml;

		this->mPacketLength = strlen(NBUS_PACKET_START_CODE) + NBUS_PACKET_HEADER_LEGNTH_BYTES + this->mHeaderLength;
		this->mpPacket = new char[this->mPacketLength];

		memcpy(this->mpPacket, NBUS_PACKET_START_CODE, strlen(NBUS_PACKET_START_CODE));

		this->mpHeader = (char *)this->mpPacket + strlen(NBUS_PACKET_START_CODE) + NBUS_PACKET_HEADER_LEGNTH_BYTES;

		memcpy(this->mpHeader, data, length);
	}

	delete pXmlParser;

	return 0;
}

int NBUSPacket::parserPacket(void *packet, unsigned int length)
{
	int headerLength = strlen(NBUS_PACKET_START_CODE) + NBUS_PACKET_HEADER_LEGNTH_BYTES;
	int headerPosition = headerLength;

	//LOGI("Packet: %s", (char *)packet);

	if (length < headerLength)
	{
		LOGE("Invalid Packet: %d", length);
		return -1;
	}

	int startCodeBytes = strlen(NBUS_PACKET_START_CODE);
	char startCode[startCodeBytes];

	strncpy(startCode, (char *)packet, startCodeBytes);

	if (strncmp(startCode, (char *)NBUS_PACKET_START_CODE, startCodeBytes))
	{
		LOGE("Invalid Start Code");
		return -1;
	}

	int headerLengthBytes = *(int *)(startCodeBytes + (char *)packet);

	this->mHeaderLength = headerLengthBytes;

	headerLength += headerLengthBytes;

	if (length < headerLength)
	{
		LOGE("Incomplete Packet Header");
		return -1;
	}

	this->mpHeader = (char *)packet + headerPosition;

	// Parser XML header.
	char packetHeader[this->mHeaderLength + 1];
	memset(packetHeader, 0, this->mHeaderLength + 1);
	strncpy(packetHeader, this->mpHeader, this->mHeaderLength);

	//LOGI("Packet Header: %s", packetHeader);
	XMLParser *pXmlParser = XMLParser::getInstance();

	IXML *pIXml = pXmlParser->parseXML(packetHeader, this->mHeaderLength);

	if (pIXml == NULL)
	{
		LOGW("Invalid XML Header: %s", packetHeader);
		delete pXmlParser;

		return -1;
	}

	if (this->mpXml) delete this->mpXml;
	this->mpXml = pIXml;

	if (pIXml->getXMLType() == XML_TYPE_EVENT)
	{
		EventXML *pEventXml = (EventXML *)pIXml;

		this->mpXml = pEventXml;

		this->mDataLength = pEventXml->getLength();

		if (this->mpPacket) delete [] (char*)this->mpPacket;

		this->mpPacket = new char[headerLength + this->mDataLength+1];
		memset(this->mpPacket, 0, headerLength + this->mDataLength+1);
        this->mPacketLength = headerLength + this->mDataLength;

		memcpy(this->mpPacket, packet, headerLength + this->mDataLength);

		this->mpHeader = (char *)this->mpPacket + headerPosition;
		this->mpData = (char *)this->mpHeader + this->mHeaderLength;
	}
	else if (pIXml->getXMLType() == XML_TYPE_COMMAND)
	{
		CommandXML *pCmdXml = (CommandXML *)pIXml;

		this->mpXml = pCmdXml;

		if (this->mpPacket) delete [] (char*)this->mpPacket;

		this->mpPacket = new char[headerLength+1];
		memset(this->mpPacket, 0, headerLength+1);
        this->mPacketLength = headerLength;

        //LOGI("Packet Length: %d", headerLength);
		memcpy(this->mpPacket, packet, headerLength);

		this->mpHeader = (char *)this->mpPacket + headerPosition;
	}

	delete pXmlParser;

	return 0;
}

int NBUSPacket::copyData(void *data, unsigned int length)
{
	if (this->mpData == NULL)
	{
		LOGE("No Packet Header");
		return -1;
	}

	memcpy((char *)this->mpData + this->mDataCopied, (char *)data, length);

	this->mDataCopied += length;

	return 0;
}

int NBUSPacket::copyHeader(void *header, unsigned int length)
{
	char *buff = NULL;
	unsigned int packetLength = 0;
	int wPos = 0;

    if (this->mpPacket)
    {
        delete [] (char *)this->mpPacket;
        this->mpPacket = NULL;
    }
    
    mPacketLength = 0;
    mHeaderLength = 0;
	mDataLength = 0;
	mDataCopied = 0;

	if (this->mpXml)
	{
		delete this->mpXml;
		this->mpXml = NULL;
	}

	//LOGI("Header: %s, Length: %d", (char *)header, length);
	packetLength = strlen(NBUS_PACKET_START_CODE) + NBUS_PACKET_HEADER_LEGNTH_BYTES + length;

	//LOGI("Length : %d", packetLength);

	buff = new char[length+1];

	memset(buff, 0, length+1);

	strncpy(buff, (char *)header, length);

	//LOGI("buff : %s", buff);

	this->mHeaderLength = length;

	// Parser XML header.
	XMLParser *pXmlParser = XMLParser::getInstance();

	IXML *pIXml = pXmlParser->parseXML(buff, this->mHeaderLength);

	if (pIXml == NULL)
	{
		LOGW("Invalid XML Header: %s", buff);
		delete pXmlParser;
		delete [] buff;

		return -1;
	}

	this->mpXml = pIXml;

	delete [] buff;

	if (pIXml->getXMLType() == XML_TYPE_EVENT)
	{
		EventXML *pEventXml = (EventXML *)pIXml;

		//LOGI("Parsed Header:%s", pEventXml->toXML());

		this->mDataLength = pEventXml->getLength();
		packetLength += this->mDataLength;

		//LOGI("Data Length: %d, Packet Length: %d", this->mDataLength, packetLength);
	}

	this->mpPacket = new char[packetLength];

	this->mPacketLength = packetLength;

	strncpy((char *)this->mpPacket, (char *)NBUS_PACKET_START_CODE, strlen(NBUS_PACKET_START_CODE));

	wPos = strlen(NBUS_PACKET_START_CODE);

	*(int *)(wPos + (char *)this->mpPacket) = this->mHeaderLength;

	//LOGI("Header Length: %d", *(int *)(wPos + (char *)this->mpPacket));

	wPos += sizeof(int);

	this->mpHeader = (char *)(wPos + (char *)this->mpPacket);

	strncpy(this->mpHeader, (char *)header, this->mHeaderLength);

	wPos += this->mHeaderLength;

	this->mpData = (char *)(wPos + (char *)this->mpPacket);

	delete pXmlParser;

	return 0;
}

bool NBUSPacket::isCompleted()
{
	if (this->mDataCopied >= this->mDataLength)
	{
		return TRUE;
	}

	return FALSE;
}


