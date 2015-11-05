/**
*
*  @author		Sanghyeun Kim(sanghyeun.kim@lge.com)
*  @date		2014.09.19
*/

#ifndef _PIONODE_H_
#define _PIONODE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "Log.h"
#include "NodeBus.h"
#include "NodeLooper.h"
#include "Thread.h"
#include "XMLTypes.h"
#include "XMLParser.h"
#include "DefaultService.h"
#include "INodeBusService.h"
#include "Packet.h"
#include "AtmgrInterface.h"

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PIONEER_LOCAL_SERVICE_NAME DOMAIN_PREFIX "pioneer.service.local"
#define ATMGR_CTL_NODE_NAME DOMAIN_PREFIX "atmgrctrl.node"
#define ATMGR_LOG_PIPE	"/tmp/Nodebus/atmgr.log"
#define LOG_BUF_SIZE	8*1024
#define ACNODE_CTL_PIPE	"/tmp/Nodebus/acnode.ctl"
#define ACNODE_CTL_BUF	1024

int ACNodeCallback(void *data, unsigned int length, int type);
int SendFileToNodebus(int type,int format,char* file_name);

/**
 * Class for ATMgr. Control Node. 
 */
class AtmgrCtlNode : public NodeAdapter, public Thread
{
	private:
		unsigned int mLogDumpInterval ; 
		Mutex_t			mMutex ;
		bool		mLogStatus;

	protected:
		IMPLEMENT_THREAD(logdump())
		void logdump();

	public:

		//AtmgrCtlNode();
		AtmgrCtlNode() : NodeAdapter(ATMGR_CTL_NODE_NAME,ACNodeCallback)
		{
			initLogDump();
		}

		void initLogDump()
		{
				mLogDumpInterval=3; /**< Default interval is 3sec */
				mLogStatus=false;
				mMutex = Lock::createMutex(true);
				startThread();
		}

		void startLogDump(unsigned int interval)
		{
				mLogDumpInterval=(interval == 0 ? 3 : interval);

				if(mLogStatus) return;
				pthread_mutex_unlock(mMutex);
				mLogStatus=true;
		}

		void stopLogDump()
		{
				if(!mLogStatus)return;
				mLogStatus=false;
			   	pthread_mutex_lock(mMutex);
		}

		void waitForReady()
		{ 
			while(this->getThreadID() == -1 || this->mpLocalNode->getThreadID() == -1 || \
			this->mpDispatcher->getThreadID() == -1 ) sleep(1);
		}

	
};

/**
 *  ATMgr. Control Node worker thread.
 */
void AtmgrCtlNode::logdump()
{
	int num=0;
	char data[LOG_BUF_SIZE]={0,};

	while(1)
	{
	pthread_mutex_lock(mMutex);	
	pthread_mutex_unlock(mMutex);	
	SendFileToNodebus(EVT_KEYWORD(LOG),LOG_KEYWORD(TXT),(char*)ATMGR_LOG_PIPE);

	sleep(mLogDumpInterval);
	}
}

#ifdef __cplusplus
}
#endif

#endif
