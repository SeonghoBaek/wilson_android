/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#include "XMLParser.h"
#include "Lock.h"
#include "Log.h"
#include "XMLTypes.h"
#include "List.h"
#include "Configure.h"

#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdlib.h>
#include <string.h>

char *CMD_STR_TABLE[CMD_TYPE_NONE] =
		{
				(char*)"STAY",
				(char*)"INFO",
				(char*)"LOG",
				(char*)"LOGOFF",
				(char*)"IMG",
				(char*)"START",
				(char*)"STOP",
				(char*)"PAUSE",
				(char*)"WAIT",
				(char*)"RESUME",
				(char*)"EPK",
				(char*)"LUA",
				(char*)"FILE",
				(char*)"KEY",
				(char*)"RPC",
				(char*)"ARPC",
				(char*)"TC",
				(char*)"SH",
				(char*)"CFG",
				(char*)"REBOOT",
				(char*)"EXIT",
				(char*)"COMPLETE"
		};

char *EVT_STR_TABLE[EVT_TYPE_NONE] =
		{
				(char*)"REGOK",
				(char*)"REGFAIL",
				(char *)"READY",
				(char*)"LOG",
				(char*)"ERROR",
				(char*)"IMG",
				(char*)"TC",
				(char*)"CMD",
				(char*)"INFO"
		};

char *FORMAT_STR_TABLE[LOG_FORMAT_NONE] =
		{
				(char*)"JSON",
				(char*)"TXT",
				(char*)"XML",
				(char*)"JPG",
				(char*)"BIN"
		};

char *ERROR_STR_TABLE[ERROR_ID_NONE] =
		{
				(char*)"SYS",
				(char*)"CMD"
		};

XMLParser* XMLParser::getInstance()
{
	static bool bParserInit = false;
	static Mutex_t lock = Lock::createMutex();

	LOCK(lock)
	{
		if (bParserInit == false)
		{
			bParserInit = true;
			xmlInitParser();
		}
	}

	return new XMLParser();
}

IXML* XMLParser::parseXML(const char *xml, int length)
{
	IXML* pXML = NULL;
	xmlDoc  *doc = NULL;
	xmlNode *rootElement = NULL;
	char* pStr = NULL;

	if ( (doc = xmlReadMemory(xml, length, "xml.xml", NULL, 0)) == NULL )
	{
		return pXML;
	}

	if ( (rootElement = xmlDocGetRootElement(doc)) == NULL )
	{
		xmlFreeDoc(doc);
		return pXML;
	}

	if ( xmlStrcmp(rootElement->name, (const xmlChar *)"nodebus") == 0)
	{
		NBUSXML *pBusXml = new NBUSXML;
		pXML = pBusXml;

		char* pType = (char *)xmlGetProp(rootElement, (xmlChar *)"type");
		char* pId = (char *)xmlGetProp(rootElement, (xmlChar *)"id");
		char* pIp = (char *)xmlGetProp(rootElement, (xmlChar *)"ip");
		char* pPort = (char *)xmlGetProp(rootElement, (xmlChar *)"port");
		char* pNode = (char *)xmlGetProp(rootElement, (xmlChar *)"node");
		char* pSyncId = (char *)xmlGetProp(rootElement, (xmlChar *)"syncid");
		char* pBuffStr = (char *)xmlGetProp(rootElement, (xmlChar *)"buffer");

		if (pType)
		{
			pStr = this->trimwhiteSpace(pType);
			pBusXml->setType(atoi(pStr));
			xmlFree(pType);
		}

		if (pId)
		{
			pStr = this->trimwhiteSpace(pId);
			pBusXml->setId(atoi(pStr));
			xmlFree(pId);
		}

		if (pIp)
		{
			pStr = this->trimwhiteSpace(pIp);
			pBusXml->setIp(pStr);
			xmlFree(pIp);
		}

		if (pPort)
		{
			pStr = this->trimwhiteSpace(pPort);
			pBusXml->setPort(atoi(pStr));
			xmlFree(pPort);
		}

		if (pNode)
		{
			pStr = this->trimwhiteSpace(pNode);
			pBusXml->setNodeName(pStr);
			xmlFree(pNode);
		}

		if (pSyncId)
		{
			pStr = this->trimwhiteSpace(pSyncId);
			pBusXml->setSyncId(atoi(pStr));
			xmlFree(pSyncId);
		}

		if (pBuffStr)
		{
			pStr = this->trimwhiteSpace(pBuffStr);
			//LOGI("Parsed Buff Addr Str:%s", pStr);
			pBusXml->setSharedBufferAddrStr(pStr);
			xmlFree(pBuffStr);
		}

		for (xmlNode *curNode = rootElement->xmlChildrenNode; curNode; curNode = curNode->next)
		{
			if (xmlIsBlankNode(curNode)) continue;

			if (xmlStrcmp(curNode->name, (const xmlChar *)"node") == 0)
			{
				xmlChar* pNodeName = xmlNodeListGetString(doc, curNode->xmlChildrenNode, 1);

				pStr = this->trimwhiteSpace((char *)pNodeName);

				pBusXml->addNode((char *)pStr);

				xmlFree(pNodeName);

				continue;
			}
		}
	}
	else if ( xmlStrcmp(rootElement->name, (const xmlChar *)"event") == 0)
	{
		EventXML *pEventXml = new EventXML;
		pXML = pEventXml;

		char* pTypeId = (char *)xmlGetProp(rootElement, (xmlChar *)"type");
		char* pFormat = (char *)xmlGetProp(rootElement, (xmlChar *)"format");
		char* pLength = (char *)xmlGetProp(rootElement, (xmlChar *)"length");
		char* pRes = (char *)xmlGetProp(rootElement, (xmlChar *)"res");
		char* pId = (char *)xmlGetProp(rootElement, (xmlChar *)"id");

		if (pTypeId)
		{
			pStr = this->trimwhiteSpace(pTypeId);

			do
			{
				if (strcmp(pStr, "REGOK") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(REGOK)); break;
				}
				if (strcmp(pStr, "REGFAIL") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(REGFAIL)); break;
				}
				if (strcmp(pStr, "READY") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(READY)); break;
				}
				if (strcmp(pStr, "LOG") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(LOG)); break;
				}
				if (strcmp(pStr, "ERROR") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(ERROR)); break;
				}
				if (strcmp(pStr, "IMG") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(IMG)); break;
				}
				if (strcmp(pStr, "TC") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(TC)); break;
				}
				if (strcmp(pStr, "CMD") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(CMD)); break;
				}
				if (strcmp(pStr, "INFO") == 0)
				{
					pEventXml->setType(EVT_KEYWORD(INFO)); break;
				}

				break;
			}
			while(1);

			xmlFree(pTypeId);
		}

		if (pFormat)
		{
			pStr = this->trimwhiteSpace(pFormat);

			do
			{
				if (strcmp(pStr, "JSON") == 0)
				{
					pEventXml->setFormat(LOG_KEYWORD(JSON)); break;
				}
				if (strcmp(pStr, "TXT") == 0)
				{
					pEventXml->setFormat(LOG_KEYWORD(TXT)); break;
				}
				if (strcmp(pStr, "XML") == 0)
				{
					pEventXml->setFormat(LOG_KEYWORD(XML)); break;
				}
				if (strcmp(pStr, "JPG") == 0)
				{
					pEventXml->setFormat(LOG_KEYWORD(JPG)); break;
				}
				if (strcmp(pStr, "BIN") == 0)
				{
					pEventXml->setFormat(LOG_KEYWORD(BIN)); break;
				}

				break;
			}
			while(1);

			xmlFree(pFormat);
		}

		//LOGI("xml: %s", pEventXml->toXML());

		if (pLength)
		{
			pStr = this->trimwhiteSpace(pLength);

			pEventXml->setLength(atoi(pStr));

			xmlFree(pLength);
		}

		if (pRes)
		{
			pStr = this->trimwhiteSpace(pRes);

			pEventXml->setRes(atoi(pStr));

			xmlFree(pRes);
		}

		if (pId)
		{
			pStr = this->trimwhiteSpace(pId);

			do
			{
				if (strcmp(pStr, "SYS") == 0)
				{
					pEventXml->setID(ERROR_KEYWORD(SYS));
				}
				else if (strcmp(pStr, "CMD") == 0)
				{
					pEventXml->setID(ERROR_KEYWORD(CMD));
				}
				else pEventXml->setID(atoi(pStr));  // Testcase ID

				break;

			}
			while(1);

			xmlFree(pId);
		}

		//LOGI("xml: %s", pEventXml->toXML());

		for (xmlNode *curNode = rootElement->xmlChildrenNode; curNode; curNode = curNode->next)
		{
			if (xmlIsBlankNode(curNode)) continue;

			if (xmlStrcmp(curNode->name, (const xmlChar *)"command") == 0)
			{
				CommandXML* pCmdXml = this->parseCommandXml(doc, curNode);

				pEventXml->setCommandXML(pCmdXml);

				break;
			}
		}

		//LOGI("xml: %s", pEventXml->toXML());
	}
	else if ( xmlStrcmp(rootElement->name, (const xmlChar *)"command") == 0)
	{
		CommandXML *pCmdXml = this->parseCommandXml(doc, rootElement);
		pXML = pCmdXml;
	}
	else if ( xmlStrcmp(rootElement->name, (const xmlChar *)"testcase") == 0)
	{
		TestCaseXML *pTestCaseXml = this->parseTestCaseXML(doc, rootElement);
		pXML = pTestCaseXml;
	}

	xmlFreeDoc(doc);

	return pXML;
}

TestCaseXML* XMLParser::parseTestCaseXML(xmlDoc* pDoc, xmlNode* pXmlNode)
{
	TestCaseXML *pTestCaseXml = new TestCaseXML;
	xmlNode* rootElement = pXmlNode;
	char* pStr = NULL;

	char* pId = (char *)xmlGetProp(rootElement, (xmlChar *)"id");

	if (pId)
	{
		pStr = this->trimwhiteSpace(pId);
		pTestCaseXml->setId(atoi(pStr));
		xmlFree(pId);
	}

	char* pRepeat = (char *)xmlGetProp(rootElement, (xmlChar *)"repeat");

	if (pRepeat)
	{
		pStr = this->trimwhiteSpace(pRepeat);
		pTestCaseXml->setId(atoi(pStr));
		xmlFree(pRepeat);
	}

	for (xmlNode *curNode = rootElement->xmlChildrenNode; curNode; curNode = curNode->next)
	{
		if (xmlIsBlankNode(curNode)) continue;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"command") == 0)
		{
			CommandXML* pCmdXml = this->parseCommandXml(pDoc, curNode);

			pTestCaseXml->addCommand(pCmdXml);

			break;
		}
	}

	return NULL;
}

CommandXML* XMLParser::parseCommandXml(xmlDoc* pDoc, xmlNode* pXmlNode)
{
	CommandXML *pCmdXml = new CommandXML;
	xmlNode* rootElement = pXmlNode;
	char* pStr = NULL;

	char* pTypeId = (char *)xmlGetProp(rootElement, (xmlChar *)"type");

	if (pTypeId)
	{
		pStr = this->trimwhiteSpace(pTypeId);

		do
		{
			if (strcmp(pStr, "STAY") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(STAY));
				break;
			}
			if (strcmp(pStr, "INFO") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(INFO));
				break;
			}
			if (strcmp(pStr, "LOG") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(LOG));
				break;
			}
			if (strcmp(pStr, "LOGOFF") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(LOGOFF));
				break;
			}
			if (strcmp(pStr, "IMG") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(IMG));
				break;
			}
			if (strcmp(pStr, "START") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(START));
			}
			if (strcmp(pStr, "STOP") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(STOP));
				break;
			}
			if (strcmp(pStr, "PAUSE") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(PAUSE));
				break;
			}
			if (strcmp(pStr, "RESUME") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(RESUME));
				break;
			}
			if (strcmp(pStr, "EPK") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(EPK));
				break;
			}
			if (strcmp(pStr, "LUA") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(LUA));
				break;
			}
			if (strcmp(pStr, "FILE") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(FILE));
				break;
			}
			if (strcmp(pStr, "KEY") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(KEY));
				break;
			}
			if (strcmp(pStr, "RPC") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(RPC));
				break;
			}
			if (strcmp(pStr, "ARPC") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(ARPC));
				break;
			}
			if (strcmp(pStr, "TC") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(TC));
				break;
			}
			if (strcmp(pStr, "SH") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(SH));
				break;
			}
			if (strcmp(pStr, "CFG") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(CFG));
				break;
			}
			if (strcmp(pStr, "WAIT") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(WAIT));
				break;
			}
			if (strcmp(pStr, "REBOOT") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(REBOOT));
				break;
			}
			if (strcmp(pStr, "EXIT") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(EXIT));
				break;
			}
			if (strcmp(pStr, "COMPLETE") == 0)
			{
				pCmdXml->setType(CMD_KEYWORD(COMPLETE));
				break;
			}

			break;
		}
		while (1);

		xmlFree(pTypeId);
	}

	char* pTime = (char *)xmlGetProp(rootElement, (xmlChar *)"time");

	if (pTime)
	{
		pStr = this->trimwhiteSpace(pTime);
		pCmdXml->setTime(atoi(pStr));
		xmlFree(pTime);
	}

	for (xmlNode *curNode = rootElement->xmlChildrenNode; curNode; curNode = curNode->next)
	{
		if (xmlIsBlankNode(curNode)) continue;

		if (xmlStrcmp(curNode->name, (const xmlChar *)"arg") == 0)
		{
			ArgumentXML *pArgXml = new ArgumentXML;

			xmlChar* pNodeName = xmlNodeListGetString(pDoc, curNode->xmlChildrenNode, 1);

			pStr = this->trimwhiteSpace((char *)pNodeName);

			pArgXml->setValue(pStr);

			char* pName = (char *)xmlGetProp(curNode, (xmlChar *)"name");

			if (pName)
			{
				pStr = this->trimwhiteSpace(pName);

				pArgXml->setName(pStr);

				xmlFree(pName);
			}

			pCmdXml->addArgument(pArgXml);

			continue;
		}
	}

	return pCmdXml;
}

void NBUSXML::setSharedBufferAddr(char *pAddr)
{
	sprintf(this->mProcAddr, "%p", pAddr);
}

char* NBUSXML::getSharedBufferAddr()
{
	char* pAddr = NULL;

	sscanf(this->mProcAddr, "%p", (void **)&pAddr);

	return pAddr;
}

char *NBUSXML::toXML()
{
	char* pStrXml = NULL;

	pStrXml = new char[XML_MAX_LENGTH+1];

	pStrXml[0] = 0;

	switch (this->mType)
	{
		case NBUS_TYPE_EVENT:
		{
			switch (this->mId)
			{
				case NBUS_EVENT_NODE_ADDED:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\"/>",
							(char *)XML_HEADER,this->mType, this->mId, this->mNodeName);
				}
					break;
				case NBUS_EVENT_NODE_DROPPED:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\"/>",
							(char *)XML_HEADER,this->mType, this->mId, this->mNodeName);
				}
					break;
				case NBUS_EVENT_NETWORK_DETACHED:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\"/>",
							(char *)XML_HEADER,this->mType, this->mId);
				}
					break;
				case NBUS_EVENT_SET_DEFAULT_NODE:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\"/>",
							(char *)XML_HEADER,this->mType, this->mId, this->mNodeName);
				}
					break;
				default:
					break;
			}
		}
			break;
		case NBUS_TYPE_REQUEST:
		{
			switch (this->mId)
			{
				case NBUS_CMD_LIST:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\" syncid=\"%d\" buffer=\"%s\"/>",
							(char *)XML_HEADER,
							this->mType,
							this->mId,
							this->mNodeName,
							this->mSyncId,
							this->mProcAddr
					);
				}
					break;
				case NBUS_CMD_JOIN:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\"/>",
							(char *)XML_HEADER,this->mType, this->mId, this->mNodeName);
				}
					break;
				case NBUS_CMD_DROP:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\"/>",
							(char *)XML_HEADER,this->mType, this->mId, this->mNodeName);
				}
					break;
				default:
					break;
			}
		}
			break;
		case NBUS_TYPE_RESPONSE:
		{
			switch (this->mId)
			{
				case NBUS_CMD_LIST:
				{
					sprintf(pStrXml, "%s<nodebus type=\"%d\" id=\"%d\" node=\"%s\" syncid=\"%d\" buffer=\"%s\">",
							(char *)XML_HEADER,
							this->mType,
							this->mId,
							this->mNodeName,
							this->mSyncId,
							this->mProcAddr
					);

					NodeXML *pNodeXml = NULL;
					char* pStr = NULL;

					LIST_FOR_EACH(pNodeXml, this->mpNodeList)
					{
						pStr = pNodeXml->toXML();
						strcat(pStrXml, pStr);

						delete [] pStr;
					}

					strcat(pStrXml, "</nodebus>");
				}
					break;
				default:
					break;
			}
		}
			break;
		default:
			break;
	}

	return pStrXml;
}

char *NodeXML::toXML()
{
	char* pNodeXml = new char[NODE_NAME_LENGTH+14];
	pNodeXml[0] = 0;

	sprintf(pNodeXml,"<node>%s</node>",this->mName);
	return pNodeXml;
}

char* EventXML::toXML()
{
	char* pStrXml = NULL;

	pStrXml = new char[XML_MAX_LENGTH+1];

	pStrXml[0] = '\0';

	//LOGI("type: %d", this->mType);

	switch (this->mType)
	{
		case EVT_TYPE_REGOK:
		{
			sprintf(pStrXml, "%s<event type=\"%s\"/>", (char *)XML_HEADER, EVT_STR_TABLE[this->mType]);
		}
			break;
		case EVT_TYPE_REGFAIL:
		{
			sprintf(pStrXml, "%s<event type=\"%s\"/>", (char *)XML_HEADER, EVT_STR_TABLE[this->mType]);
		}
			break;
		case EVT_TYPE_READY:
		{
			sprintf(pStrXml, "%s<event type=\"%s\"/>", (char *)XML_HEADER, EVT_STR_TABLE[this->mType]);
		}
			break;
		case EVT_TYPE_LOG:
		{
			sprintf(pStrXml, "%s<event type=\"%s\" format=\"%s\" length=\"%d\"/>",
					(char *)XML_HEADER, EVT_STR_TABLE[this->mType], FORMAT_STR_TABLE[this->mFormat], this->mLength);
		}
			break;
		case EVT_TYPE_ERROR:
		{
			sprintf(pStrXml, "%s<event type=\"%s\" id=\"%s\"/>",
					(char *)XML_HEADER, EVT_STR_TABLE[this->mType], ERROR_STR_TABLE[this->mID]);
		}
			break;
		case EVT_TYPE_IMG:
		{
			sprintf(pStrXml, "%s<event type=\"%s\" format=\"%s\" length=\"%d\"/>",
					(char *)XML_HEADER, EVT_STR_TABLE[this->mType], FORMAT_STR_TABLE[this->mFormat], this->mLength);
		}
			break;
		case EVT_TYPE_TC:
		{
			sprintf(pStrXml, "%s<event type=\"%s\" id=\"%s\" res=\"%d\"/>",
					(char *)XML_HEADER, EVT_STR_TABLE[this->mType], ERROR_STR_TABLE[this->mID], this->mRes);
		}
			break;
		case EVT_TYPE_CMD:
		{
			sprintf(pStrXml, "%s<event type=\"%s\" res=\"%d\">", (char *)XML_HEADER, EVT_STR_TABLE[this->mType], this->mRes);

			if (this->mpCommandXML)
			{
				strcat(pStrXml, this->mpCommandXML->toXML());
			}

			strcat(pStrXml, (char *)"</event>");
		}
			break;
		case EVT_TYPE_INFO:
		{
			sprintf(pStrXml, "%s<event type=\"%s\" format=\"%s\" length=\"%d\"/>",
					(char *)XML_HEADER, EVT_STR_TABLE[this->mType], FORMAT_STR_TABLE[this->mFormat], this->mLength);
		}
			break;
		default:
			break;
	}

	return pStrXml;
}

char* TestCaseXML::toXML()
{
	char* pStrXml = NULL;

	pStrXml = new char[XML_MAX_LENGTH+1];

	pStrXml[0] = '\0';

	sprintf(pStrXml, "%s<testcase id=\"%d\" repeat=\"%d\">", (char *)XML_HEADER,this->mId, this->mRepeat);

	if (this->mNumOfCommand)
	{
		CommandXML *pCommand = NULL;

		LIST_FOR_EACH_PREV(pCommand, this->mpCommandList)
		{
			char *cmdXml = pCommand->toXML();
			strcat(pStrXml, cmdXml);
			delete [] cmdXml;
		}
	}

	strcat(pStrXml, (char *)"</testcase>");

	return pStrXml;
}

char* CommandXML::toFullXML()
{
	char* pStrXml = NULL;

	pStrXml = new char[XML_MAX_LENGTH+1];

	pStrXml[0] = '\0';

	sprintf(pStrXml, "%s<command type=\"%s\">", (char *)XML_HEADER, CMD_STR_TABLE[this->mType]);

	if (this->mNumOfArguments)
	{
		ArgumentXML *pArgument = NULL;

		LIST_FOR_EACH_PREV(pArgument, this->mpArgumentsList)
		{
			char *argStr = pArgument->toXML();
			strcat(pStrXml, argStr);
			delete [] argStr;
		}
	}

	strcat(pStrXml, (char *)"</command>");

	return pStrXml;
}

char* CommandXML::toXML()
{
	char* pStrXml = NULL;

	pStrXml = new char[XML_MAX_LENGTH+1];

	pStrXml[0] = '\0';

	sprintf(pStrXml, "<command type=\"%s\">", CMD_STR_TABLE[this->mType]);

	if (this->mNumOfArguments)
	{
		ArgumentXML *pArgument = NULL;

		LIST_FOR_EACH_PREV(pArgument, this->mpArgumentsList)
		{
			char *argStr = pArgument->toXML();
			strcat(pStrXml, argStr);
			delete [] argStr;
		}
	}

	strcat(pStrXml, (char *)"</command>");

	return pStrXml;
}

TestCaseXML::~TestCaseXML()
{
	if (this->mNumOfCommand)
	{
		CommandXML *pCommand = NULL;

		LIST_FOR_EACH(pCommand, this->mpCommandList)
		{
			LIST_DEL(pCommand);
			delete pCommand;
			pCommand = this->mpCommandList;
		}

		delete this->mpCommandList;
	}
}

CommandXML::~CommandXML()
{
	if (this->mNumOfArguments)
	{
		ArgumentXML *pArgument = NULL;

		LIST_FOR_EACH(pArgument, this->mpArgumentsList)
		{
			LIST_DEL(pArgument);
			delete pArgument;
			pArgument = this->mpArgumentsList;
		}

		delete this->mpArgumentsList;
	}
}

char* ArgumentXML::toXML()
{
	char* pStrXml = NULL;

	pStrXml = new char[XML_MAX_LENGTH+1];

	pStrXml[0] = '\0';

	if (strlen(this->mName) == 0)
	{
		sprintf(pStrXml, "<arg>%s</arg>", this->mValue);
	}
	else
	{
		sprintf(pStrXml, "<arg name=\"%s\">%s</arg>", this->mName, this->mValue);
	}

	return pStrXml;
}

