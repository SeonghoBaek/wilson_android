/** 
*
*  @author		Seongho Baek
*/

#include "Log.h"
#include "NodeBus.h"
#include "NodeLooper.h"
#include "Thread.h"
#include "XMLTypes.h"
#include "XMLParser.h"
#include "DefaultService.h"
#include "INodeBusService.h"
#include "Packet.h"

#include <stdlib.h>
#include <stdio.h>
#include <DefaultNodeCallback.h>

int main(int argc, char** argv)
{
	/*
	if (argc < 2)
	{
		LOGI("usage: %s <server address> <port>\n", argv[0]);

		return -1;
	}
	 */

	// Create NodeBus Service.
	LOGI("Create DefaultService\n");
	DefaultService *NodeBusLocalService = new DefaultService();
	NodeBusLocalService->init();
	//NodeBusLocalService->setGlobalServerAddress(argv[1], atoi(argv[2])); // Weak.

	//LOGI("Start %s:%d\n", argv[1], atoi(argv[2]));
	NodeBusLocalService->run();

	NodeLooper looper;
    LOGI("Looper Wait\n");
	looper.wait(-1);

	LOGI("Daemon\n");

	return 0;
}
