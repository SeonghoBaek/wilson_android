/** 
*
*  @author		Seongho Baek
*/
#define _LOG_TAG "LocalBusInit"

#include "Log.h"
#include "NodeBus.h"
#include "NodeLooper.h"
#include "DefaultService.h"

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
