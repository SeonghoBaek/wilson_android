/*
 * nbus.c
 *
 *  Created on: 2014. 8. 19.
 *      Author: major.baek
 */

#include <stdio.h>
#include <string.h>
#include "Configure.h"
#include "Bridge.h"
#include "NBus.h"

int nbus_join(const char* nodeName, const char* serverName)
{
	char joinXML[NBUS_BUFF_SIZE];
	char temp[NBUS_BUFF_SIZE + sizeof(unsigned int)*2];

	unsigned int type = 1; // Request
	unsigned int length = 0;

	memset(joinXML, 0, NBUS_BUFF_SIZE);

	strncpy(joinXML, (char *)&type, sizeof(unsigned int));

	memset(temp, 0, NBUS_BUFF_SIZE);
	sprintf(temp, "<nodebus type=\"1\" id=\"0\" node=\"%s\"/>", nodeName);

	length = strlen(temp);
	strncpy(joinXML + sizeof(unsigned int), (char *)&length, sizeof(unsigned int));
	strncpy(joinXML + sizeof(unsigned int)*2, temp, length);

	if (serverName == NULL)
	{
		bridge_cross(DEFAULT_LOCAL_SERVICE_NAME, joinXML, sizeof(joinXML));
	}
	else
	{
		bridge_cross(serverName, joinXML, sizeof(joinXML));
	}

	return 0;
}

int nbus_cast(const char* nodeName, void *data, int length)
{
	if (nodeName == NULL) // Broadcast
	{
		bridge_cross(DEFAULT_LOCAL_SERVICE_NAME, data, length);
	}
	else
	{
		bridge_cross(nodeName, data, length);
	}

	return 0;
}

int nbus_drop(const char *nodeName, const char* serverName)
{
	char dropXML[NBUS_BUFF_SIZE];
	char temp[NBUS_BUFF_SIZE + sizeof(unsigned int)*2];

	unsigned int type = 1; // Request
	unsigned int length = 0;

	memset(dropXML, 0, NBUS_BUFF_SIZE);

	strncpy(dropXML, (char *)&type, sizeof(unsigned int));

	memset(temp, 0, NBUS_BUFF_SIZE);
	sprintf(temp, "<nodebus type=\"1\" id=\"1\" node=\"%s\"/>", nodeName);

	length = strlen(temp);
	strncpy(dropXML + sizeof(unsigned int), (char *)&length, sizeof(unsigned int));
	strncpy(dropXML + sizeof(unsigned int)*2, temp, length);

	if (serverName == NULL)
	{
		bridge_cross(DEFAULT_LOCAL_SERVICE_NAME, dropXML, sizeof(dropXML));
	}
	else
	{
		bridge_cross(serverName, dropXML, sizeof(dropXML));
	}

	return 0;
}
