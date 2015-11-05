/**
*  Interfaces between ATMgr. and ATMgr Control Node of Nodebus.
*
*  @author		Sanghyeun Kim(sanghyeun.kim@lge.com)
*  @date		2014.09.18
*/

#ifdef __cplusplus
extern "C" {
#endif

#include "sqlite3.h"
#include "AtmgrInterface.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Definitions. 
 */
#define AI "ATMgrInterface"

#define ON_SUCCESS 0 
#define ON_ERR -1

#define AI_OK  0	/**< ATMgr. Infterface OK */
#define AI_ERR 	-1	/**< ATMgr. Infterface ERROR */
#define SET_ERR_VALUE(ret,n)	(ret-=n)

#define QUERY_BUF	128 /**< buffer size for SQL query */
#define ATMGR_RET_BUF	128 /**< buffer size for ATMgr Command buffer */

#define ATMGR_CTL_DB "./ATMgrCTL.db"
#define ATMGR_CMD_PIPE "/tmp/atmgr.cmd"
#define ATMGR_RET_PIPE "/tmp/atmgr.ret"
#define ATMGR_LOG_HOME "/tmp/atmgr_log/"

// Nodebus message type
#define NBUS_MSG_LOG	3
#define NBUS_MSG_INFO	8

// Nodebus message format
#define NBUS_FMT_JSON	0	
#define NBUS_FMT_TXT	1	

#define AI_Debug(...)	printf(__VA_ARGS__)
//#define AI_Debug(...)

/**
 * Global variables. 
 */
sqlite3 * db = NULL;
pthread_attr_t attr;

/**
 * Initialize ATMgr(Auto Test Manager) interfaces.
 */
int InitATMgrInterface(void)
{
	if( sqlite3_open(ATMGR_CTL_DB, &db) != SQLITE_OK )
	{ 
			sqlite3_close(db); 
			db=NULL;
			AI_Debug("Failed to open DB\n");
			return AI_ERR; 
	}

	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

return AI_OK;
}

/**
 * Finalize ATMgr interfaces.
 */
int FinalATMgrInterface(void)
{
	if( db != NULL )
	{ 
			sqlite3_close(db); 
			db=NULL;
	}

	pthread_attr_destroy(&attr);
return AI_OK;
}

/**
 * Execute System Command.
 *
 * The calling function should allocate memory for the command.
 * The allocated memory is freed here after running the command.
 *
 * @param  char* cmd	heap memory command		
 */
int AI_ExecSystemCmd(char* cmd)
{
	int ret=AI_OK;	

	if( cmd == NULL )SET_ERR_VALUE(ret,1); 
	else if( system(cmd) == ON_ERR )SET_ERR_VALUE(ret,2);

	// Remark point
	free(cmd);

return ret;
}

/**
 *  Run a job.
 */
void* RunningCmd(void* job)
{
	AI_ExecSystemCmd((char*)job);

return NULL;
}
/**
 *  Create asynchronous worker.
 *
 * @param	char* cmd	command to be launched on the target			
 */
static int AI_CreateAsyncWorker(char* job,RUN_JOB_T job_runner)
{
	pthread_t worker;
	if(pthread_create( &worker,&attr,job_runner,(void*)job)!=ON_SUCCESS) return AI_ERR;

return AI_OK;
}

#if 0
/**
 * Routine for debugging. 
 */
int callback(void* param,int col_num,char** col_value,char** col_name)
{
	for(int i=0; i < col_num ; i++)
	{
		printf("%s = %s \n ", col_name[i],col_value[i]? col_value[i]:"NULL");	
	
	}	
return 0;
}
char* errMsg = NULL;
if(sqlite3_exec(db,sql,callback,NULL,&errMsg) != SQLITE_OK)
	{
		printf("SQL error : %s \n",errMsg);
		sqlite3_free(errMsg);	
	}
#endif

/**
 * Duplicate  a command.
 *
 * Returned char* buffer must be freed somewhere.
 */
static char* AI_DuplicateCmd(char* cmd)
{
	char* CtlCmd = NULL;

	int cmd_len = (strlen(cmd)+1);
	CtlCmd=(char*)calloc(cmd_len,sizeof(char));
	if(CtlCmd == NULL ) return NULL;

	memcpy(CtlCmd,cmd,cmd_len);	

return CtlCmd;
}

/**
 * Run a command which is queryed from ATMgr Contorl Database. 
 *
 * @param	char* cmd_name	command name for query 			
 * @param	bool async 		true : run command asynchronously 			
 */
int AI_RunDBCmd(char* cmd_name,bool async)
{
	int ret=AI_OK;
	sqlite3_stmt * stmt=NULL;
	char* cmd = NULL;
	char sql[QUERY_BUF]={0,};
	sprintf(sql,"select command from atmgr_cmd where cmd_name='%s';",cmd_name);

	sqlite3_prepare_v2(db,sql,strlen(sql)+1,&stmt,NULL);

	if(stmt == NULL)SET_ERR_VALUE(ret,1);
	else if(sqlite3_step(stmt) != SQLITE_ROW)SET_ERR_VALUE(ret,2);
	else
	{
		int status=AI_OK;
		cmd=(char*)sqlite3_column_text(stmt,0);
		//AI_Debug("%s\n",cmd);

		if(async)
			status=AI_CreateAsyncWorker(AI_DuplicateCmd(cmd),RunningCmd);
		else
			status=AI_ExecSystemCmd(AI_DuplicateCmd(cmd));

		if(status!=AI_OK)SET_ERR_VALUE(ret,3);
	}

	sqlite3_finalize(stmt);

return ret;
}

/**
 * Enable to dump log messages. 
 */
int AI_EnableDumpLog(void)
{
return AI_RunDBCmd((char*)"nbus_dumplog",true); 
}

/**
 * Disable to dump log messages. 
 */
int AI_DisableDumpLog(void)
{
return AI_RunDBCmd((char*)"nbus_stoplog",false); 
}

/**
 * Get return value of ATMgr.
 */
void* AI_GetRetValue(void* job)
{
	int num=0;
	char data[ATMGR_RET_BUF]={0,};

	FILE* fp=fopen(ATMGR_RET_PIPE,"r");
	if(fp == NULL) return NULL;

	if(fread((void*)data,sizeof(char),ATMGR_RET_BUF,fp) <= 0);

	fclose(fp);

	// for debugging 
	AI_Debug("ATMGR ret value : %s",data);
	SendMsgToNodebus(NBUS_MSG_INFO,NBUS_FMT_TXT,data,strlen(data));
return NULL;
}


/**
 * Send command to ATMgr.
 *
 * @param	char* cmd  ATMgr Command
 * @param	bool async	true -> asynchronous mode
 */
int AI_SendCommand(char* cmd,bool async)
{
	int ret = AI_OK;

	FILE* fp=fopen(ATMGR_CMD_PIPE,"w");
	if(fp == NULL) return SET_ERR_VALUE(ret,1);
	else if( cmd == NULL ) SET_ERR_VALUE(ret,2);
	else if( fwrite((void*)cmd,sizeof(char),strlen(cmd),fp) <= 0 )SET_ERR_VALUE(ret,3);

	fclose(fp);

	if(async)
		AI_CreateAsyncWorker(NULL,AI_GetRetValue);
	else
		AI_GetRetValue(NULL);

return ret;
}

/**
 * Dump a target file or a function log file to nodebus .
 *
 * @param	filename(char*)	either an absolute file name of target or an ATLib function name	
 */
int AI_GetTargetFile(char* filename)
{
	int ret=AI_OK;

	if ( filename[0] != '/' )
	{// in the case of ATLib function name
		char hostname[ATMGR_RET_BUF]={0,};
		char func_log[ATMGR_RET_BUF]={0,};
		gethostname(hostname,ATMGR_RET_BUF);
		sprintf(func_log,"%s%s_%s.log",ATMGR_LOG_HOME,hostname,filename);
		ret=SendFileToNodebus(NBUS_MSG_LOG,NBUS_FMT_JSON,func_log);
	}
	else	
		ret=SendFileToNodebus(NBUS_MSG_LOG,NBUS_FMT_TXT,filename);

return ret;
}

/**
 * Interface to launch job task.
 *
 * @param		
 */
int AI_LaunchJobTask(char* job,RUN_JOB_T job_runner)
{
return	AI_CreateAsyncWorker(job,job_runner);
}


#ifdef __cplusplus
}
#endif
