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

		void listenLocalNetwork();
	
	public:

		IMPLEMENT_THREAD(listenLocalNetwork())

		LocalNode();

		LocalNode(const char *n, INode* pImpl);

		void setName(const char *n);

		void setActionImpl(INode* pImpl);

		virtual ~LocalNode();

		// Implementation.
		virtual void run();
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
				Coordinator(NodeAdapter *pOwner);

				virtual ~Coordinator() {}

				int localAction(void *data, unsigned int length, int type);

				int nodeCallback(void *data, unsigned int length, int type);

		} *mpLocalNode;

		class Dispatcher: public Thread, public INodeLooper
		{
			private:
				NodeAdapter *mpOwner;

			public:
				IMPLEMENT_THREAD(thread())

				Dispatcher(NodeAdapter *pOwner);
				virtual ~Dispatcher() {}

				int dispatch(const char* event);

				int looperCallback(const char* pEvent);

				void thread();
		} *mpDispatcher;

	public:

		// Dummy
		NodeAdapter();

		NodeAdapter(const char* pNodeName, NodeAction_t nodeAction);

		NodeAdapter(const char* pNodeName, INode* pNodeImpl);

		int addQueue(void *data, unsigned int length, int type);

		virtual ~NodeAdapter();

		void run();

		void setNodeName(const char *pNodeName);

		LocalNode* getNode();

		bool isExited();
};

class NodeBus
{
	public:
		virtual ~NodeBus() {}

        /* name: Node Name 
         * action: Callback Function - int (*NodeAction_t)(void *data, unsigned int length);
         */
		static NodeAdapter* createLocalNodeAdapter(const char *name, NodeAction_t action);

		static NodeAdapter* createLocalNodeAdapter(const char *name, INode* pNodeImpl);

		static INodeBusService *getDefaltServiceInterface();

		static INodeBusService *getServiceInterface(const char* serviceName);

	private:
		NodeBus() {}
};

#endif
