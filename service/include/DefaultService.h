/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef __LOCALSERVICE_H
#define __LOCALSERVICE_H

#include "Lock.h"
#include "NodeEntry.h"
#include "List.h"
#include "INameService.h"
#include "IMessageService.h"
#include "Configure.h"
#include "NodeBus.h"

#include "NodeLooper.h"
#include "NodeBusService.h"
#include "Thread.h"
#include "Types.h"
#include "NodePort.h"
#include "NodeNetwork.h"

class DefaultService: public NodeBusService
{
    private:
    
        char        mDefaultNode[NODE_NAME_LENGTH+1];
        NodePort    mNodePort;
        NodeAdapter *mpDefaultNodeAdapter;
        INode       *mpDefaultNodeCallback;
    
        friend class NodePort;
    
	protected:

		NBUS_CMD_QUEUE 	mQ;
		NodeLooper		*mpLooper;
		
		friend class ServerNodePort;
		friend class NodeLooper;

		class Coordinator : public LocalNode, public INode
		{
			private:
				DefaultService *mpOwner;
				Mutex_t			mCLock;

			public:
				Coordinator(DefaultService *pOwner) : mpOwner(pOwner)
				{
					this->setName(mpOwner->mServiceName);
					this->setActionImpl(this);
					this->setThreadName((char *)"DefaultService::Coordinator");
					mCLock = Lock::createMutex();
				}

				virtual ~Coordinator() { if (mCLock) Lock::deleteMutex(mCLock); }

				int localAction(void *data, unsigned int length, int type);

				int nodeCallback(void *data, unsigned int length, int type)
				{
					return localAction(data, length, type);
				}
		};

		class Dispatcher: public Thread, public INodeLooper
		{
			private:
				DefaultService *mpOwner;

			public:
				IMPLEMENT_THREAD(thread())

				Dispatcher(DefaultService *pOwner) : mpOwner(pOwner) {this->setThreadName((char *)"DefaultService::Dispatcher");}
				virtual ~Dispatcher() {}

				int dispatch(const char* event);

				void thread();

				int looperCallback(const char*event)
				{
					return dispatch(event);
				}
		};

		char 			*mpServerAddress;
		int  			mServerPort;
		char			mServiceName[SERVICE_NAME_LENGTH];
        char            mServiceClientName[NODE_NAME_LENGTH];
		int				mNumOfNode;
		NodeEntry		*mpNodeListHead;
		Mutex_t			mLock;
		int				mInitialized;
		BusStatus_t		mLastStatus;
		Coordinator*	mpCoordinator;
		Dispatcher*		mpDispatcher;

		int			getNumOfNode() {return mNumOfNode;}
		NodeEntry	*getNodeListHead() {return mpNodeListHead;}
		NodeEntry	*addNodeEntry(NodeEntry *entry);

	public:

		DefaultService();

		DefaultService(const char *serviceName);

		virtual ~DefaultService() {}

		void setupService(const char *serviceName);

		virtual int cast(const char* nodename, const void *manifest, unsigned int length);
		virtual int cast(const char* nodename, const void *manifest, unsigned int length, int type);
		virtual int broadcast(const void *manifest, unsigned int length);
		virtual int broadcast(const void *manifest, unsigned int length, int type);
		virtual int join(const char *pNodeName);
		virtual int join(const char *pNodeName, int type);
		virtual int drop(const char *pNodeName);
		virtual int exist(const char *pNodeName);
		virtual char* getName() { return mServiceName; }
    
		virtual int  init()
		{
			LOCK(mLock)
			{
				mLastStatus = BUS_READY;
			}

			return 0;
		}

		virtual BusStatus_t getStatus() 
		{
			BusStatus_t lastStatus = BUS_NOT_READY;

			LOCK(mLock)
			{
				lastStatus = mLastStatus;
			}

			return lastStatus;
		}

		virtual void setGlobalServerAddress(char *address, int port) { mpServerAddress = address; mServerPort = port; }

		virtual void run();

		virtual void processMessage();
    
        void setDefaultNode(const char *pNodeName)
        {
            //LOGI("Set DefaultNodeCallback to %s", pNodeName);
            memset(mDefaultNode, 0, NODE_NAME_LENGTH+1);
            strncpy(mDefaultNode, pNodeName, NODE_NAME_LENGTH);
        }
    
        char* getDefaultNode()
        {
            return mDefaultNode;
        }

		void waitForReady()
		{
			while(this->mpCoordinator->getThreadID() == (NBUS_THREAD_ID_T)-1 || this->mpDispatcher->getThreadID() == (NBUS_THREAD_ID_T)-1)
			sleep(1);
		}

        int isServiceReady()
        {
            int isReady = 0;

            LOCK(mLock)
            {
                isReady = this->mInitialized;
            }

            return isReady;
        }
};

#endif
