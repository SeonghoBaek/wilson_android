/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef _THREAD_H_
#define _THREAD_H_

#include "Log.h"
#include "Types.h"

#define THREAD_NAME_LENGTH 64
// Should be thread safe.
void* _thread_func(void *);

typedef void* (*ThreadRun_t)(void *);

class ThreadHelper
{
	public:
		static void run(ThreadRun_t thread_run, void *arg, bool bDetached)
		{
			NBUS_THREAD_ID_T tid;

			if (pthread_create(&tid, NULL, thread_run, arg) == 0)
			{
				if (bDetached == TRUE)
				{
					pthread_detach(tid);
				}
				else
				{
					void *res;

					pthread_join(tid, &res);
				}
			}
		}
};

class Thread
{
	private:
		NBUS_THREAD_ID_T	tid;
	    char mThreadName[THREAD_NAME_LENGTH+1];
		
		static void *activateThread(void* instance)
		{
			((Thread *)instance)->loop();
			
			return NULL;
		}

	protected:
		bool 				detached;

	public:
		Thread():detached(TRUE), tid((NBUS_THREAD_ID_T)-1) {mThreadName[0] = '\0';}

		Thread(bool d):detached(d), tid((NBUS_THREAD_ID_T)-1) {mThreadName[0] = '\0';}

		void startThread()
		{
            //LOGI("Create pthread\n");
            
			if (pthread_create(&tid, NULL, activateThread, this) == 0)
			{
				if (this->detached == TRUE)
				{
					LOGI("Start Thread, Object: %s, ID: %u\n", mThreadName, tid);

					pthread_detach(tid);
				}
				else
				{
					void *res;

					pthread_join(tid, &res);
				}
			}
		}

		void setDetach(bool bDetached)
		{
			this->detached = bDetached;
		}

		void setThreadName(char *name)
		{
			memset(mThreadName, 0, THREAD_NAME_LENGTH+1);
			strncpy(mThreadName, name, THREAD_NAME_LENGTH);
		}
		
		NBUS_THREAD_ID_T getThreadID(){return this->tid;}	

		virtual ~Thread() {}

		virtual int loop() = 0;
};

#define IMPLEMENT_THREAD(_func) 		\
		virtual int loop() 	{ _func; return 0; } \

#endif /* THREAD_H_ */
