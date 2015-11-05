/**
*
*  @author		Sanghyeun Kim(sanghyeun.kim@lge.com)
*  @date		2014.09.19
*/

#ifndef _ATMGR_INTERFACE_H_
#define _ATMGR_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef void*(*RUN_JOB_T)(void*);

int InitATMgrInterface(void);
int FinalATMgrInterface(void);
int AI_EnableDumpLog(void);
int AI_DisableDumpLog(void);
int AI_SendCommand(char* cmd,bool async);
int AI_GetTargetFile(char* filename);
int AI_LaunchJobTask(char* job,RUN_JOB_T job_runner);

// Interface of ATMgr. Control Node
int SendMsgToNodebus(int type,int format,char* message,unsigned int length);
int SendFileToNodebus(int type,int format,char* file_name);
#ifdef __cplusplus
}
#endif

#endif
