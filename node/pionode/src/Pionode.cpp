/** 
*
*  @author		Sanghyeun Kim(sanghyeun.kim@lge.com)
*  @date		2014.08.12
*/

#include "Pionode.h"

#define ACNode  "ATMgrCTLNode"

#define ACNode_OK 0
#define ACNode_ERR -1

#define ATMGR_CMD_BUF	256 /**< buffer size for ATMgr Command buffer */

//for debugging
//#undef LOGI
//#define LOGI(...)	

/**
 * Global variables
 */
INodeBusService *gpService = NULL;
AtmgrCtlNode * atmgrCtlNode = NULL; 

/**
 * Initialize Pioneer Nodebus Service.
 */
void InitNodebusService(char* address, int port)
{
	LOGI("Create Pioneer Local Service\n");
	DefaultService * pioneerSvc = new DefaultService(PIONEER_LOCAL_SERVICE_NAME);
	pioneerSvc->init();
	pioneerSvc->setGlobalServerAddress(address, port); 

	LOGI("Start %s:%d\n", address,port);
	pioneerSvc->run();
	pioneerSvc->waitForReady();
	

}

/**
 * Register ACNode to Pioneer Nodebus Service.
 */
void RegisterACNode()
{
	LOGI("Craete ATMgr Control Node\n"); 
	atmgrCtlNode = new AtmgrCtlNode();
    
    /* Run Node Thread */
	atmgrCtlNode->run();
	atmgrCtlNode->waitForReady();

    /* Join to default service */
	gpService = NodeBus::getServiceInterface(PIONEER_LOCAL_SERVICE_NAME);  // Get service interface
	sleep(2); // need some time....
	gpService->join(atmgrCtlNode);  // Join to service.
	
	// Set default node. All global network message will be sent to this node.
    gpService->setDefaultNode(ATMGR_CTL_NODE_NAME); 

	// Send Ready message to Global nodebus
	SendMsgToNodebus(EVT_TYPE_READY,0,NULL,0);
}

/**
 * Send Message to Nodebus Service.
 */
int SendMsgToNodebus(int type,int format,char* message,unsigned int length)
{

	EventXML eventXml;
	NBUSPacket packet;

	eventXml.setType(type);
	eventXml.setFormat(format);
	eventXml.setLength(length);

	char *header = eventXml.toXML();
	packet.copyHeader(header,(int)strlen(header));

	if ( message != NULL )
		packet.copyData(message, length);

	delete [] header;
	
	LOGI("Message sent to Server : %s(%d)", packet.getPacket(),packet.getPacketLength());
return gpService->cast(LOCAL_ROUTER_NAME, packet.getPacket(), packet.getPacketLength());
}

/**
 * Send a file to Nodebus Service.
 */
int SendFileToNodebus(int type,int format,char* file_name)
{
	int num=0;
	char data[LOG_BUF_SIZE]={0,};

	FILE* fp=fopen(file_name,"r");
	if(fp == NULL)return ACNode_ERR;
	
	while(1)
	{
		num=fread((void*)data,sizeof(char),LOG_BUF_SIZE,fp);
		if(num < 1 && feof(fp) != 0){fclose(fp);break;}

		SendMsgToNodebus(type,format,data,num);

		memset(data,0,LOG_BUF_SIZE);num=0;
	}

return ACNode_OK;
}

/**
 * Get Command arguments.
 *
 * @param cmd(char*)		one of ATMgr. Command
 * @param cmdXml(CommandXML*)  CommanXML which includes arguments of command.
 * @param async(bool)		true -> asynchronous call
 */
int SendCmdToATMgr(char* cmd, CommandXML* cmdXml,bool async=false)
{
	if ( cmd == NULL || cmdXml == NULL )return ACNode_ERR;

	char cmd_buf[ATMGR_CMD_BUF]= {0,};
	ArgumentXML* argHead = cmdXml->getArgumentList();
	ArgumentXML* arg = argHead->p_prev;//get first argument
	
	strcat(cmd_buf,cmd);

	// get arguments and append to cmd_buf
	do
	{
		if(arg->getValue() == NULL) return ACNode_ERR;
		strcat(cmd_buf," ");
		strcat(cmd_buf,arg->getValue()); 

	}while(argHead != (arg = arg->p_prev) );

	LOGI(" ATMgr CMD : %s \n", cmd_buf );

	if(!strcmp(cmd," get "))
	{
		 AI_GetTargetFile(&cmd_buf[strlen(" get  ")]);	
	}
	else
		AI_SendCommand(cmd_buf,async);

return ACNode_OK;
}

/**
 * ACNode Global callback ; Get the message from Global Nodebus.
 */
int ACNodeGlobalCallback(void *data, unsigned int length)
{
	//LOGI("Global Message Arrived");

	NBUSPacket packet;
	packet.parserPacket((char *)data, length);
	IXML *pXml = packet.getXML();

	if (pXml->getXMLType() == XML_TYPE_COMMAND)
	{
		CommandXML* pCmdXml = (CommandXML *)pXml;

		LOGI("Global Message Arrived(%d)",pCmdXml->getType());
		switch (pCmdXml->getType())
		{
			case CMD_TYPE_STAY:
			{
				char * test=(char *)"OK I will freeze!!!";
				SendMsgToNodebus(EVT_KEYWORD(INFO),LOG_KEYWORD(TXT),test,strlen(test));
			}
			break;
			case CMD_TYPE_LOGOFF:
			{
				atmgrCtlNode->stopLogDump();
			}
			break;
			case CMD_TYPE_LOG:
			{
				atmgrCtlNode->startLogDump(3);
			}
			break;
			case CMD_TYPE_IMG:
			case CMD_TYPE_STOP:
			case CMD_TYPE_PAUSE:
			case CMD_TYPE_WAIT:
			case CMD_TYPE_RESUME:
			break;
			case CMD_TYPE_INFO:
				SendCmdToATMgr((char*)" info ",pCmdXml);
			break;
			case CMD_TYPE_EPK:
				SendCmdToATMgr((char*)" epk ",pCmdXml);
				break;
			case CMD_TYPE_LUA:
				SendCmdToATMgr((char*)" lua ",pCmdXml);
				break;
			case CMD_TYPE_FILE:
				SendCmdToATMgr((char*)" get ",pCmdXml);
				break;
			case CMD_TYPE_KEY:
				SendCmdToATMgr((char*)" key ",pCmdXml);
				break;
			case CMD_TYPE_RPC:
				SendCmdToATMgr((char*)" func ",pCmdXml);
				break;
			case CMD_TYPE_ARPC:
				SendCmdToATMgr((char*)" func ",pCmdXml,true);
				break;
			case CMD_TYPE_TC:
				SendCmdToATMgr((char*)" tc ",pCmdXml);
				break;
			case CMD_TYPE_SH:
				SendCmdToATMgr((char*)" sh ",pCmdXml);
				break;
			case CMD_TYPE_CFG:
				SendCmdToATMgr((char*)" cfg ",pCmdXml);
				break;
			default:
			break;
		}
	}

	return 0;
}

/**
 * Job runner for ACNode Control.
 */
void* ACNodeCTL(void* job)
{
	int num=0;
	char* ctlCmd=(char*)calloc(ACNODE_CTL_BUF,sizeof(char));
	if(ctlCmd == NULL) return NULL;

	while(1)
	{
		FILE* fp=fopen(ACNODE_CTL_PIPE,"r");
		if(fp != NULL)
		{
			num=fread((void*)ctlCmd,sizeof(char),ACNODE_CTL_BUF,fp); 
			ACNodeGlobalCallback((void*)ctlCmd, 11 + 4 + *((int*)(ctlCmd+11)) );
			//LOGI("%s",ctlCmd);//for testing

			fclose(fp);
			memset(ctlCmd,0,ACNODE_CTL_BUF);
		}
		else
			sleep(5);

	}

	free(ctlCmd);

return NULL;
}

/**
 * ACNode default callback; Catch all kind of message .
 */
int ACNodeCallback(void *data, unsigned int length, int type)
{
	if (type == GLOBAL_MESSAGE) return ACNodeGlobalCallback(data, length);

	LOGI("Local Message Arrived: %s", (char *)data);

	return 0;
}

/**
 * PIONEER Nodebus Service.
 */
int main(int argc, char** argv)
{
	if (argc != 3 )
	{
		LOGI("usage: %s <server address> <port>\n", argv[0]);
		return -1;
	}

	// Initialize Local Nodebus
	InitNodebusService(argv[1], atoi(argv[2])); 
	RegisterACNode();
	
	// Be ready to control ATMgr. 
	InitATMgrInterface();
	AI_EnableDumpLog();
	AI_LaunchJobTask(NULL,ACNodeCTL);

    LOGI("Looper Wait\n");
	NodeLooper looper;
	looper.wait(-1);
	LOGI("Daemon\n");

	AI_DisableDumpLog();
	FinalATMgrInterface();

	return 0;
}
