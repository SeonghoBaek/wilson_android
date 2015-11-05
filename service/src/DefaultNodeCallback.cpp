//
// Created by major on 11/2/15.
//

#include "DefaultNodeCallback.h"

int DefaultNodeCallback::nodeCallback(void *data, unsigned int length, int type)
{
    if (type == GLOBAL_MESSAGE) return this->nodeGlobalCallback(data, length);

    return 0;
}

int DefaultNodeCallback::nodeGlobalCallback(void *data, unsigned int length)
{
    NBUSPacket packet;

    packet.parserPacket((char *)data, length);

    IXML *pXml = packet.getXML();

    if (pXml->getXMLType() == XML_TYPE_COMMAND)
    {
        CommandXML* pCmdXml = (CommandXML *)pXml;

        switch (pCmdXml->getType())
        {
            case CMD_TYPE_STAY:
            {

            }
            break;
            case CMD_TYPE_INFO:
            {

            }
            break;
            case CMD_TYPE_LOGOFF:
            {

            }
            break;
            case CMD_TYPE_LOG:
            {
                LOGI("Received CMD_TYPE_LOG");

                // Create logger node.

                pid_t childPid = -1;

                if ( (childPid = fork()) < 0 )
                {
                    LOGE("Fork rlogcat node failure");
                }
                else if (childPid == 0) // child
                {
                    char* eargv[] = {"rlogcat", NULL};

                    if (execvp("rlogcat", eargv) == -1)
                    {
                        LOGE("rlogcat exec failed");
                    }
                }
                else  // parent
                {
                    LOGI("Ready to send log");
                }
            }
            break;
            case CMD_TYPE_IMG:
            case CMD_TYPE_STOP:
            case CMD_TYPE_PAUSE:
            case CMD_TYPE_WAIT:
            case CMD_TYPE_RESUME:
            case CMD_TYPE_EPK:
            case CMD_TYPE_LUA:
            case CMD_TYPE_FILE:
            case CMD_TYPE_KEY:
            case CMD_TYPE_RPC:
            case CMD_TYPE_ARPC:
            case CMD_TYPE_TC:
            case CMD_TYPE_SH:
            case CMD_TYPE_CFG:
            {
            }
            break;
            default:
            break;
        }
    }
    else if (pXml->getXMLType() == XML_TYPE_EVENT)
    {
        EventXML *pEventXml = (EventXML *)pXml;

        switch (pEventXml->getType())
        {
            case EVT_TYPE_REGOK:
            {
                // Send Boot Log
                LOGI("Received EVT_TYPE_REGOK");
            }
            break;
            case EVT_TYPE_REGFAIL:
            {
                LOGW("Received EVT_TYPE_REGFAIL");
            }
            break;
            default:
            break;
        }
    }

    return 0;
}