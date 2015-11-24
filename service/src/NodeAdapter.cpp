//
// Created by major on 11/24/15.
//

#include "NodeBus.h"

LocalNode::LocalNode()
{
    mpImpl = NULL;

    //LOGI("Create LocalNode\n");
    this->setThreadName((char *)"LocalNode");
}

LocalNode::LocalNode(const char *n, INode* pImpl)
{
    setName(n);

    this->mpImpl = pImpl;

    this->setThreadName((char *)"LocalNode");
}

void LocalNode::listenLocalNetwork()
{
    NodeNetwork::setupLocalNode(getNamespace(), this->mpImpl);
}

void LocalNode::setName(const char *n)
{
    NodeXML* pNodeXml = new NodeXML(n);
    char *pXml = pNodeXml->toXML();

    strcpy(mName, n);
    strcpy(mManifest, pXml);

    delete [] pXml;
    delete pNodeXml;
}

void LocalNode::setActionImpl(INode* pImpl)
{
    mpImpl = pImpl;
}

LocalNode::~LocalNode() {}

void LocalNode::run()
{
    this->startThread();
}

NodeAdapter::Coordinator::Coordinator(NodeAdapter *pOwner):
LocalNode(pOwner->mNodeName, this),
mpOwner(pOwner)
{
    this->setThreadName((char *)"NodeAdapter::Coordinator");
}

int NodeAdapter::Coordinator::localAction(void *data, unsigned int length, int type)
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

int NodeAdapter::Coordinator::nodeCallback(void *data, unsigned int length, int type)
{
    if (type ==  KILL_MESSAGE)
    {
        //LOGE("NodeAdapter::Coordinator EXIT: %s", this->mpOwner->mNodeName);
        this->mpOwner->mpLooper->exit();
        return -1;
    }

    return localAction(data, length, type);
}


NodeAdapter::Dispatcher::Dispatcher(NodeAdapter *pOwner):
mpOwner(pOwner)
{
    this->setThreadName((char *)"NodeAdapter::Dispatcher");
}

int NodeAdapter::Dispatcher::dispatch(const char* event)
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
                //LOGI("Custom Message");
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

int NodeAdapter::Dispatcher::looperCallback(const char* pEvent)
{
    if (strncmp(pEvent, LOOPER_EXIT, LOOPER_EVT_LENGTH) == 0)
    {
        //LOGE("NodeAdapter::Dispatcher Exit: %s", this->mpOwner->mNodeName);
        return -1;
    }

    return dispatch(pEvent);
}

void NodeAdapter::Dispatcher::thread()
{
    this->mpOwner->mpLooper->wait(-1); // Loop.

    //LOGW("NodeAdapter EXIT");
    this->mpOwner->mExited = TRUE;
}

NodeAdapter::NodeAdapter() {}

NodeAdapter::NodeAdapter(const char* pNodeName, NodeAction_t nodeAction)
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

NodeAdapter::NodeAdapter(const char* pNodeName, INode* pNodeImpl)
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

int NodeAdapter::addQueue(void *data, unsigned int length, int type)
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
    this->mQ.push(item);
    //LOGD("Queue Size of %s:%d", this->mpOwner->mNodeName, this->mpOwner->mQ.getSize());
    this->mpLooper->wakeup();

    return 0;
}

NodeAdapter::~NodeAdapter()
{
    if (this->mpLooper) delete this->mpLooper;

    if (this->mpLocalNode) delete this->mpLocalNode;

    if (this->mpDispatcher) delete this->mpDispatcher;
}

void NodeAdapter::run()
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

void NodeAdapter::setNodeName(const char *pNodeName)
{
    memset(this->mNodeName, 0, NODE_NAME_LENGTH);
    strncpy(this->mNodeName, pNodeName, NODE_NAME_LENGTH);
}

LocalNode* NodeAdapter::getNode() { return mpLocalNode;}

bool NodeAdapter::isExited() {return this->mExited;}


NodeAdapter* NodeBus::createLocalNodeAdapter(const char *name, NodeAction_t action)
{
    return new NodeAdapter(name, action);
}

NodeAdapter* NodeBus::createLocalNodeAdapter(const char *name, INode* pNodeImpl)
{
    return new NodeAdapter(name, pNodeImpl);
}

INodeBusService* NodeBus::getDefaltServiceInterface()
{
    return new IDefaultNodeBusService();
}

INodeBusService* NodeBus::getServiceInterface(const char* serviceName)
{
    INodeBusService *svc = new ICustomNodeBusService(serviceName);

    return svc;
}