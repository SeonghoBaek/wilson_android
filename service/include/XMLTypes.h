/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef _XMLTYPES_H
#define _XMLTYPES_H

#include "List.h"
#include "Log.h"
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>

#define XML_MAX_LENGTH 1024
#define XML_MAX_TVC_LENGTH 256
#define XML_LENGTH_NODE_PROTOCOL 16
#define XML_LENGTH_NODE_NAMESPACE 64
#define XML_LENGTH_NODE_MANIFEST 512
#define XML_LENGTH_NODE_IP 16
#define PROC_ADDR_LENGTH 16
#define XML_IP_LENGTH 16
#define XML_PORT_LENGTH 5

#define XML_ARGUMENT_LENGTH 128

#define XML_HEADER "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"

enum // service event id
{
	NBUS_EVENT_NODE_ADDED,
	NBUS_EVENT_NODE_DROPPED,
	NBUS_EVENT_NETWORK_DETACHED,
    NBUS_EVENT_SET_DEFAULT_NODE,
	NBUS_EVENT_USER1, // undef
	NBUS_EVENT_USER2,
	NBUS_EVENT_USER3,
	NBUS_EVENT_USER4,
	NBUS_EVENT_USER5
};

enum // service event type
{
	NBUS_TYPE_EVENT, // service event
	NBUS_TYPE_REQUEST, // command - use command id
	NBUS_TYPE_RESPONSE, // response of command - use command id
	NBUS_TYPE_NONE
};

enum // service command id
{
	NBUS_CMD_JOIN,
	NBUS_CMD_DROP,
	NBUS_CMD_LIST,
	NBUS_CMD_CLIENT_BROADCAST,
    NBUS_CMD_REGISTER,
    NBUS_CMD_UNREGISTER,
	NBUS_CMD_NONE
};

enum // node command type
{
	CMD_TYPE_STAY,
	CMD_TYPE_INFO,
	CMD_TYPE_LOG,
	CMD_TYPE_LOGOFF,
	CMD_TYPE_IMG,
	CMD_TYPE_START,
	CMD_TYPE_STOP,
	CMD_TYPE_PAUSE,
	CMD_TYPE_WAIT,
	CMD_TYPE_RESUME,
	CMD_TYPE_EPK,
	CMD_TYPE_LUA,
	CMD_TYPE_FILE,
	CMD_TYPE_KEY,
	CMD_TYPE_RPC,
	CMD_TYPE_ARPC,
	CMD_TYPE_TC,
	CMD_TYPE_SH,
	CMD_TYPE_CFG,
	CMD_TYPE_REBOOT,
	CMD_TYPE_EXIT,
	CMD_TYPE_COMPLETE,
	CMD_TYPE_NONE
};

#define CMD_KEYWORD(name) CMD_TYPE_##name

enum  // node event id
{
	EVT_TYPE_REGOK,
	EVT_TYPE_REGFAIL,
	EVT_TYPE_READY,
	EVT_TYPE_LOG,
	EVT_TYPE_ERROR,
	EVT_TYPE_IMG,
	EVT_TYPE_TC,
	EVT_TYPE_CMD,
	EVT_TYPE_INFO,
	EVT_TYPE_NONE
};

#define EVT_KEYWORD(name) EVT_TYPE_##name

enum // log format
{
	LOG_FORMAT_JSON,
	LOG_FORMAT_TXT,
	LOG_FORMAT_XML,
	LOG_FORMAT_JPG,
	LOG_FORMAT_BIN,
	LOG_FORMAT_NONE
};

#define LOG_KEYWORD(name) LOG_FORMAT_##name

enum // error id
{
	ERROR_ID_SYS,
	ERROR_ID_CMD,
	ERROR_ID_NONE
};

#define ERROR_KEYWORD(name) ERROR_ID_##name

enum // xml type
{
	XML_TYPE_NODE,
	XML_TYPE_BUS,
	XML_TYPE_ARGUMENT,
	XML_TYPE_EVENT,
	XML_TYPE_COMMAND,
	XML_TYPE_NONE
};

#define INFO_TYPE_SOC (char*)"soc"
#define INFO_TYPE_PLATFORM (char*)"platform"

class IXML
{
	protected:
		int mTypeId;

	public:
		IXML() { mTypeId = XML_TYPE_NONE; }
		virtual ~IXML() {}

		int getXMLType() {return mTypeId;}

		virtual char* toXML() = 0;
};

class NodeXML : public IXML
{
	LINKED_LIST(NodeXML);

	private:
		char mName[XML_LENGTH_NODE_NAMESPACE+1];
	public:
		NodeXML() {LIST_INIT(this);} // Dummy

		NodeXML(const char* pNodeName)
		{
			LIST_INIT(this);

			strncpy(mName, pNodeName, XML_LENGTH_NODE_NAMESPACE);

			this->mTypeId = XML_TYPE_NODE;
		}

		virtual ~NodeXML() {}

		char* toXML();
};

class NBUSXML : public IXML
{
	private:
		int 	mType;
		int 	mId;
		int 	mSyncId;
		char	mProcAddr[PROC_ADDR_LENGTH];
		char 	mNodeName[XML_LENGTH_NODE_NAMESPACE+1];
		char 	mIp[XML_IP_LENGTH];
        int	    mPort;
		int		mNumOfNode;
		NodeXML *mpNodeList;

	public:
		NBUSXML() {mpNodeList = NULL; mSyncId = -1; mNumOfNode = 0; this->mTypeId = XML_TYPE_BUS;}

		virtual ~NBUSXML()
        {
			if (this->mNumOfNode)
			{
				NodeXML *pNodeXML = NULL;

				LIST_FOR_EACH(pNodeXML, this->mpNodeList)
				{
					LIST_DEL(pNodeXML);
					delete pNodeXML;
					pNodeXML = this->mpNodeList;
				}

				delete this->mpNodeList;
			}
		}

		void setType(int type) {this->mType = type;}
		int	  getType() {return this->mType;}

		void setId(int id) {this->mId = id;}
		int  getId() {return this->mId;}

		void setSyncId(int id) {this->mSyncId = id;}
		int  getSyncId() {return this->mSyncId;}

		void setSharedBufferAddr(char* pAddr);
		char*	getSharedBufferAddr();
    
        void setSharedBufferAddrStr(char *pAddrStr) {strcpy(this->mProcAddr, pAddrStr);}
        char* getSharedBufferAddrStr() {return this->mProcAddr;}

		void addNode(const char* pNodeName)
		{
			if (mpNodeList == NULL) mpNodeList = new NodeXML;

			NodeXML *pNodeXML = new NodeXML(pNodeName);

			LIST_ADD(pNodeXML, mpNodeList);

			mNumOfNode++;
		}

		void setNodeName(const char* pNodeName)
		{
			strncpy(mNodeName, pNodeName, XML_LENGTH_NODE_NAMESPACE);
		}

		char* getNodeName() {return this->mNodeName;}

		void setIp(const char *pIp)
		{
			strncpy(mIp, pIp, XML_IP_LENGTH);
		}

		char* getIp() {return this->mIp;}

		void setPort(int port)
		{
            mPort = port;
		}

		int getPort() {return this->mPort;}

		char* toXML();
};

class ArgumentXML : public IXML
{
	LINKED_LIST(ArgumentXML);

	private:
		char mName[XML_LENGTH_NODE_NAMESPACE+1];
		char mValue[XML_ARGUMENT_LENGTH+1];

	public:
		ArgumentXML()
		{
			LIST_INIT(this);

			memset(mName, 0, XML_LENGTH_NODE_NAMESPACE+1);
			memset(mValue, 0, XML_ARGUMENT_LENGTH+1);
			this->mTypeId = XML_TYPE_ARGUMENT;
		}

		virtual ~ArgumentXML() {}

		void setName(const char* pName) {strncpy(mName, pName, XML_LENGTH_NODE_NAMESPACE);}
		char* getName() {return mName;}
		void setValue(const char* pValue) {strncpy(mValue, pValue, XML_ARGUMENT_LENGTH);}
		char* getValue() {return mValue;}

		char* toXML();
};

class CommandXML : public IXML
{
	LINKED_LIST(CommandXML);

	private:
		int mType;
		int mNumOfArguments;
		int mWaitTime;

		ArgumentXML* mpArgumentsList;

	public:
		CommandXML()
		{
			LIST_INIT(this);
			mpArgumentsList = NULL;
			mNumOfArguments = 0;
			mType = CMD_TYPE_NONE;
			this->mTypeId = XML_TYPE_COMMAND;
			mWaitTime = 0;
		}

		virtual ~CommandXML();

		int addArgument(ArgumentXML *pArgument)
		{
			if (mpArgumentsList == NULL) mpArgumentsList = new ArgumentXML;

			LIST_ADD(pArgument, mpArgumentsList); // Add tail.

			mNumOfArguments++;

			return mNumOfArguments;
		}

		ArgumentXML* getArgumentList() {return this->mpArgumentsList;}

		void setType(int type) {this->mType = type;}

		int getType() {return this->mType;}

		void setTime(int time) {this->mWaitTime = time;}

		int getTime() {return this->mWaitTime;}

		char* toXML();
		char* toFullXML();
};

/*
<testcase id=”5123” repeat="10000">   // Testcase 5123. Repeat 10000 times
	<command type=”RPC”>
		<arg>KADP_FID_2</arg>
		<arg>ATLIB_KADP_SYS_ReadReg</arg>
		<arg>0xc0035009  0x10</arg>
	</command>
	<command type=”WAIT” time=”100”/>    // wait 100ms
	<command type=”RPC”>
		<arg>KADP_FID_2</arg>
		<arg>ATLIB_KADP_SYS_ReadReg</arg>
		<arg>0xc0035009  0x10</arg>
	</command>
	<command type=”WAIT” time=”200”/>    // wait 200ms
</testcase>
*/

class TestCaseXML : public IXML
{
	private:
		int mId; //Testcase ID
		int mRepeat; //Repeat Count
		CommandXML *mpCommandList;
		int mNumOfCommand;

	public:
		TestCaseXML()
		{
			mId = 0;
			mRepeat = 0;
			mpCommandList = NULL;
			mNumOfCommand = 0;
		}

		CommandXML* getCommandList() {return mpCommandList;}

		int addCommand(CommandXML *pCommand)
		{
			if (mpCommandList == NULL) mpCommandList = new CommandXML;

			LIST_ADD(pCommand, mpCommandList); // Add tail.

			mNumOfCommand++;

			return mNumOfCommand;
		}

		void setId(int id) {mId = id;}
		int getId() {return mId;}

		void setRepeat(int repeat) {mRepeat = repeat;}
		int getRepeat() {return mRepeat;}

		virtual ~TestCaseXML();
		virtual char* toXML();
};

class EventXML : public IXML
{
	private:
		int mType;
		int mFormat;
		unsigned int mLength;
		int mID;
		int mRes;
		CommandXML *mpCommandXML;

	public:
		EventXML()
		{
			mType = EVT_TYPE_NONE;
			mFormat = LOG_FORMAT_JSON;
			mLength = 0;
			mID = ERROR_ID_NONE;
			mRes = 0;
			mpCommandXML = NULL;
			this->mTypeId = XML_TYPE_EVENT;
		}

		virtual ~EventXML() { if (mpCommandXML) delete mpCommandXML; }

		void setType(int type) { this->mType = type; }
		int  getType() {return this->mType;}

		void setFormat(int format) {this->mFormat = format;}
		int  getFormat() {return this->mFormat;}

		void setLength(unsigned int length) {this->mLength = length;}
		unsigned int getLength() {return this->mLength;}

		void setID(int id) {this->mID = id;}
		int  getID() {return this->mID;}

		void setRes(int res) {this->mRes = res;}
		int  getRes() {return this->mRes;}

		void setCommandXML(CommandXML* pCommandXML) {this->mpCommandXML = pCommandXML;}
		CommandXML* getCommandXML() {return this->mpCommandXML;}

		char* toXML();
};

#endif
