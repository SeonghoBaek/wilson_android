/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef _BRIDGE_H_
#define _BRIDGE_H_

#include "Types.h"
#include "NodeLooper.h"
#include "Lock.h"
#include "Thread.h"

#define BRIDGE_DESC_LENGTH 64

class DomainBridge : public Thread
{
	private:
		Mutex_t		lock;
		bool		bsync;
		NodeLooper  looper;

		char *out;
		char *in;
		
		void (*action)(void *, unsigned int);

	public:
		IMPLEMENT_THREAD(thread())

		DomainBridge()
		{
			action = NULL;
			lock = Lock::createMutex();
			bsync = FALSE;
			in = out = NULL;

			this->setThreadName((char *)"DomainBrigde");
		}
		
		DomainBridge(const char *i, void(*f)(void *, unsigned int))
		{
			lock = Lock::createMutex();
			bsync = FALSE;
			out = NULL;

			in = new char[BRIDGE_DESC_LENGTH];

			memset(in, 0, BRIDGE_DESC_LENGTH);

			strncpy(in, i, BRIDGE_DESC_LENGTH);

			action = f;

			this->setThreadName((char *)"DomainBrigde");
		}

		DomainBridge(const char *i, const char *o, void(*f)(void *, unsigned int))
		{
			lock = Lock::createMutex();
			bsync = FALSE;

			in = new char[BRIDGE_DESC_LENGTH];
			out = new char[BRIDGE_DESC_LENGTH];

			memset(in, 0, BRIDGE_DESC_LENGTH);
			memset(out, 0, BRIDGE_DESC_LENGTH);

			strncpy(in, i, strlen(i));
			strncpy(out, o, strlen(o));
			action = f;

			this->setThreadName((char *)"DomainBrigde");
		}
		
		virtual ~DomainBridge() {Lock::deleteMutex(lock); if (in) delete [] in; if (out) delete [] out; }

		int setOutput(const char *desc) {strncpy(out, desc, strlen(desc)); return 0;}
		int setInput(const char *desc) {strncpy(in, desc, strlen(desc)); return 0;}
		
		const char* getInput() { return in; }
		const char* getOutput() { return out; }

		void setAction(void(*func)(void *, unsigned int)) { action = func; }

		// Thread virtual function		
		int thread();

		void run() { this->startThread(); }

		int cross(void *stream, int length); // Default Async.
		int cross(void *stream, int length, bool bsync);

		static int receiveFromSocket(int sd, char* buffer, int bufferSize);
		static int sendToDomain(char *name, char* buff, int size);
		static int bindLocalSocketToName(int sd, const char* name);
		static int createSocket();
		static int cross(char *receiver, void *stream, int length);
};

#endif
