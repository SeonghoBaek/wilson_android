/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/


#include "Log.h"
#include "Lock.h"

#include <stdio.h>
           
Mutex_t gPrintLock =  Lock::createMutex();

void NBUS_PrintMessage(int type, const char *func, int line, const char *fmt, ...)
{
	if (gPrintLock == NULL)
	{
		gPrintLock = Lock::createMutex();
	}

	LOCK(gPrintLock)
	{
		#if (LOG_PRINT_ENABLE == 1)
		va_list arg;
		char buff[80];
		char *prefix;

		if (type == NODEBUS_LOG_ERROR)
		{
			prefix = (char *)"ERROR";
		}
		else if (type == NODEBUS_LOG_INFO)
		{
			prefix = (char *)"INFO";
		}
		else if (type == NODEBUS_LOG_WARN)
		{
			prefix = (char *)"WARN";
		}
		else if (type == NODEBUS_LOG_DEBUG)
		{
			prefix = (char *)"DEBUG";
		}

		sprintf(buff, "[%s line %d at %s] ", prefix, line, func);
		printf("%s", buff);

		va_start (arg, fmt);

		vfprintf (stdout, fmt, arg);

		va_end (arg);

		fflush(stdout);

		printf("\n");

		#endif
	}

	return;
}

