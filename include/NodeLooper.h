/**
*
*  @author		Seongho Baek
*
*/

#ifndef _NODELOOPER_H_
#define _NODELOOPER_H_

#include <stdio.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>

#define LOOPER_EVT_LENGTH 4
#define LOOPER_WAKEUP 	"wake"
#define LOOPER_EXIT		"exit"
#define LOOPER_TIMEOUT	"tout"

typedef int (*NodeLooperCallback_t)(const char* event);

class INodeLooper
{
	public:
		virtual int looperCallback(const char* event) = 0;
		virtual ~INodeLooper() {}
};

class NodeLooper
{
	private:
		NodeLooperCallback_t mUser_cb;
		INodeLooper* mpImpl;
		int mFd[2];
		int mTimeOut;

	public:
		NodeLooper();

		NodeLooper(NodeLooperCallback_t func);

		NodeLooper(INodeLooper* pImpl);

		virtual ~NodeLooper() {close(mFd[0]);close(mFd[1]);}

		// -1: infinite
		int wait(int mili);

		int timer();

		int wakeup();

		int exit();

		int sendMessage(const char* msg, int length);

		int getId() {return mFd[1];}

		int setTimeOut(int mili)
		{
			this->mTimeOut = mili;
			return mili;
		}

		static void exit(int id);
};
#endif
