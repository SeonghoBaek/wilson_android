/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef __NODEBUS_H_
#define __NODEBUS_H_

#include "XMLTypes.h"
#include "XMLParser.h"
#include "Thread.h"
#include "Configure.h"
#include "NodeNetwork.h"
#include "Types.h"
#include "Log.h"
#include "NodeLooper.h"
#include "INodeBusService.h"

class INode
{
	public:

		virtual int nodeCallback(void *data, unsigned int length, int type) = 0;

		virtual ~INode() {}
};

class Node
{
	protected:
		char mManifest[NODE_MANIFEST_LENGTH];  // XML manifest.

		char mName[NODE_NAME_LENGTH];

		INode* mpImpl;

	public:
		const char* getNamespace() {return mName;}
		
		char*		getManifest() {return mManifest;}

		Node(): mpImpl(NULL) {}
		virtual ~Node() {}

		virtual void run() = 0;
};

class LocalNode : public Node, public Thread
{
	private:

		void listenLocalNetwork()
		{
			NodeNetwork::setupLocalNode(getNamespace(), this->mpImpl);
		}
	
	public:

		IMPLEMENT_THREAD(listenLocalNetwork())

		LocalNode()
		{
			mpImpl = NULL;

			//LOGI("Create LocalNode\n");
			this->setThreadName((char *)"LocalNode");
		}

		LocalNode(const char *n, INode* pImpl)
		{
			setName(n);

			this->mpImpl = pImpl;

			this->setThreadName((char *)"LocalNode");
		}

		void setName(const char *n)
		{
			NodeXML* pNodeXml = new NodeXML(n);
			char *pXml = pNodeXml->toXML();

			strcpy(mName, n);
			strcpy(mManifest, pXml);

			delete [] pXml;
			delete pNodeXml;
		}

		void setActionImpl(INode* pImpl)
		{
			mpImpl = pImpl;
		}

		virtual ~LocalNode() {}

		// Implementation.
		virtual void run()
		{
			this->startThread();
		}
};

class NodeAdapter
{
	protected:

		NBUS_CMD_QUEUE mQ;
		NodeLooper	*mpLooper;
		NodeAction_t mNodeAction;
		INode		*mpNodeImpl;
		bool		mExited;
		bool		mSetup;

		char	mNodeName[NODE_NAME_LENGTH];

		class Coordinator : public LocalNode, public INode
		{
			private:
				NodeAdapter *mpOwner;

			public:
				Coordinator(NodeAdapter *pOwner) :
					LocalNode(pOwner->mNodeName, this),
					mpOwner(pOwner)
				{
					this->setThreadName((char *)"NodeAdapter::Coordinator");
				}

				virtual ~Coordinator() {}

				int localAction(void *data, unsigned int length, int type)
				{
					NBUS_CMD item(length);

					if (type != SETUP_MESSAGE)
					{
						_GOODNESS(data, -1);
						_GOODNESS(length, -1);

						if (data)
						{
							memcpy(item.mpData, data, length);
						}
					}

					item.mType = type;

					// Queueing
					this->mpOwner->mQ.push(item);
					//LOGD("Queue Size of %s:%d", this->mpOwner->mNodeName, this->mpOwner->mQ.getSize());
					this->mpOwner->mpLooper->wakeup();

					return 0;
				}

				int nodeCallback(void *data, unsigned int length, int type)
				{
					if (type ==  KILL_MESSAGE)
					{
						//LOGE("NodeAdapter::Coordinator EXIT: %s", this->mpOwner->mNodeName);
						this->mpOwner->mpLooper->exit();
						return -1;
					}

					return localAction(data, length, type);
				}
		} *mpLocalNode;

		class Dispatcher: public Thread, public INodeLooper
		{
			private:
				NodeAdapter *mpOwner;

			public:
				IMPLEMENT_THREAD(thread())

				Dispatcher(NodeAdapter *pOwner) : mpOwner(pOwner) {this->setThreadName((char *)"NodeAdapter::Dispatcher");}
				virtual ~Dispatcher() {}

				int dispatch(const char* event)
				{
					while (mpOwner->mQ.getSize())
					{
						NBUS_CMD* pItem = mpOwner->mQ.pop();
						bool	bDoNotCallback = FALSE;

						if (pItem->mType == SETUP_MESSAGE)
						{
							mpOwner->mSetup = TRUE;

							delete pItem;

							return 0;
						}

						if (!pItem->valid())
						{
							delete pItem;

							return -1;
						}

						// Parse.
						if (pItem->mType == LOCAL_MESSAGE)
						{
							if (strncmp((char*)pItem->mpData, "<?xml", 5) == 0)
							{
								XMLParser* pXmlParser = XMLParser::getInstance();
								IXML *pIXml = pXmlParser->parseXML((char *)pItem->mpData, pItem->mLength);

								if (pIXml == NULL)
								{
									LOGW("Not Valid XML Message");
								}
								else if (pIXml->getXMLType() == XML_TYPE_BUS)
								{
									NBUSXML* pBusXml = (NBUSXML *)pIXml;

									if (pBusXml->getSyncId() > 0 && pBusXml->getType() == NBUS_TYPE_RESPONSE)
									{
										char *pSharedAddr = pBusXml->getSharedBufferAddr();
										char *pStrXml = pBusXml->toXML();

										strcpy(pSharedAddr, pStrXml);

										NodeLooper::exit(pBusXml->getSyncId());

										delete [] pStrXml;

										bDoNotCallback = TRUE;
									}

									delete pBusXml;
								}


								delete pXmlParser;
							}
							else
							{
								LOGI("Custom Message");
							}
						}
						else if (pItem->mType == GLOBAL_MESSAGE)
						{
							//LOGI("Global Client Message Arrived");
						}

						if (bDoNotCallback == FALSE && mpOwner->mNodeAction)
						{
							mpOwner->mNodeAction(pItem->mpData, pItem->mLength, pItem->mType);
						}

						if (bDoNotCallback == FALSE && mpOwner->mpNodeImpl)
						{
							mpOwner->mpNodeImpl->nodeCallback(pItem->mpData, pItem->mLength, pItem->mType);
						}

						delete pItem;
					}
					return 0;
				}

				int looperCallback(const char* pEvent)
				{
					if (strncmp(pEvent, LOOPER_EXIT, LOOPER_EVT_LENGTH) == 0)
					{
						//LOGE("NodeAdapter::Dispatcher Exit: %s", this->mpOwner->mNodeName);
						return -1;
					}

					return dispatch(pEvent);
				}

				void thread()
				{
					this->mpOwner->mpLooper->wait(-1); // Loop.

					//LOGW("NodeAdapter EXIT");
					this->mpOwner->mExited = TRUE;
				}
		} *mpDispatcher;

	public:

		// Dummy
		NodeAdapter() {}

		NodeAdapter(const char* pNodeName, NodeAction_t nodeAction)
		{
			this->mNodeAction = nodeAction;
			this->mpLooper = NULL;
			this->mpNodeImpl = NULL;
			this->mExited = FALSE;

			if (pNodeName) strncpy(this->mNodeName, pNodeName, NODE_NAME_LENGTH);

			this->mpLocalNode = NULL;
			this->mpDispatcher = NULL;
			this->mSetup = FALSE;
		}

		NodeAdapter(const char* pNodeName, INode* pNodeImpl)
		{
			this->mNodeAction = NULL;
			this->mpLooper = NULL;
			this->mpNodeImpl = pNodeImpl;
			this->mExited = FALSE;

			if (pNodeName) strncpy(this->mNodeName, pNodeName, NODE_NAME_LENGTH);

			this->mpLocalNode = NULL;
			this->mpDispatcher = NULL;
			this->mSetup = FALSE;
		}

		virtual ~NodeAdapter()
		{
			if (this->mpLooper) delete this->mpLooper;

			if (this->mpLocalNode) delete this->mpLocalNode;

			if (this->mpDispatcher) delete this->mpDispatcher;
		}

		void run()
		{
			this->mpLocalNode = new NodeAdapter::Coordinator(this);
			this->mpDispatcher = new NodeAdapter::Dispatcher(this);

			this->mpLooper = new NodeLooper(this->mpDispatcher);
			this->mpLocalNode->startThread();
			this->mpDispatcher->startThread();

			while (this->mSetup != TRUE)
			{
				LOGD("Wait NodeAdpater Setup..");
				sleep(1);
			}
		}

		void setNodeName(const char *pNodeName)
		{
			memset(this->mNodeName, 0, NODE_NAME_LENGTH);
			strncpy(this->mNodeName, pNodeName, NODE_NAME_LENGTH);
		}

		LocalNode* getNode() { return mpLocalNode;}

		bool isExited() {return this->mExited;}
};

class NodeBus
{
	public:
		virtual ~NodeBus() {}

        /* name: Node Name 
         * action: Callback Function - int (*NodeAction_t)(void *data, unsigned int length);
         */
		static NodeAdapter* createLocalNodeAdapter(const char *name, NodeAction_t action)
		{
			return new NodeAdapter(name, action);
		}

		static NodeAdapter* createLocalNodeAdapter(const char *name, INode* pNodeImpl)
		{
			return new NodeAdapter(name, pNodeImpl);
		}

		static INodeBusService *getDefaltServiceInterface()
		{
			return new IDefaultNodeBusService();
		}

		static INodeBusService *getServiceInterface(const char* serviceName)
		{
			INodeBusService *svc = new ICustomNodeBusService(serviceName);

			return svc;
		}

	private:
		NodeBus() {}
};

#endif
