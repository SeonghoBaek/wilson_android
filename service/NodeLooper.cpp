/**
*
*  @author		Seongho Baek
*/
#define _LOG_TAG "NodeLooper"

#include "NodeLooper.h"
#include "Log.h"
#include "Lock.h"
#include <sys/poll.h>
#include <string.h>

int debugLooperCallback(const char *event)
{
	LOGE("fd: %d, events: %d\n", event);

	return 0;
}

NodeLooper::NodeLooper()
{
	this->mUser_cb = NULL;
	this->mpImpl = NULL;

	pipe(this->mFd);

	this->mTimeOut = -1;
    
    //LOGI("PIPE: %d %d", this->mFd[0], this->mFd[1]);
}

NodeLooper::NodeLooper(NodeLooperCallback_t func)
{
	this->mpImpl = NULL;

	pipe(this->mFd);

	this->mUser_cb = func;

	this->mTimeOut = -1;
}

NodeLooper::NodeLooper(INodeLooper* pImpl)
{
	this->mUser_cb = NULL;

	pipe(this->mFd);

	this->mpImpl = pImpl;

	this->mTimeOut = -1;
}

int NodeLooper::timer()
{
	bool exit = FALSE;
	int res;
	char buff[BUFSIZ + 1];
	struct pollfd fds[1];

	fds[0].fd = this->mFd[0];
	fds[0].events = POLLIN;

	int mili = -1;

	for (;;)
	{
		mili = this->mTimeOut;
		fds[0].revents = 0;

		poll(fds, 1, mili);

		if (fds[0].revents & POLLIN)
		{
			res = 0;
			int nReadBytes = (int)read(fds[0].fd, buff, BUFSIZ);
			int nMsg = nReadBytes/LOOPER_EVT_LENGTH;

			//LOGI("LOOPER: %s, %d", buff, nMsg);

			for (int i = 0; i < nMsg; i++)
			{
				if (this->mpImpl)
				{
					this->mpImpl->looperCallback(buff);
				}

				if (this->mUser_cb)
				{
					this->mUser_cb(buff);
				}

				if (strncmp((char *)&buff[i*LOOPER_EVT_LENGTH], LOOPER_EXIT, LOOPER_EVT_LENGTH) == 0)
				{
					exit = TRUE;
					break;
				}
			}

			if (exit == TRUE) break;

			memset(buff, 0, sizeof(buff));
		}
		else
		{
			if (this->mpImpl)
			{
				this->mpImpl->looperCallback(LOOPER_TIMEOUT);
			}

			if (this->mUser_cb)
			{
				this->mUser_cb(LOOPER_TIMEOUT);
			}
		}
	}

	close(fds[0].fd);

	return res;
}

// -1: infinite
int NodeLooper::wait(int mili)
{
	bool exit = FALSE;
	int res;
	char buff[BUFSIZ + 1];
	struct pollfd fds[1];

	fds[0].fd = this->mFd[0];
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	for (;;)
	{
		poll(fds, 1, mili);

		if (fds[0].revents & POLLIN)
		{
			res = 0;
			int nReadBytes = (int)read(fds[0].fd, buff, BUFSIZ);
			int nMsg = nReadBytes/LOOPER_EVT_LENGTH;

			//LOGI("LOOPER: %s, %d", buff, nMsg);

			for (int i = 0; i < nMsg; i++)
			{
				if (this->mpImpl)
				{
					this->mpImpl->looperCallback(buff);
				}

				if (this->mUser_cb)
				{
					this->mUser_cb(buff);
				}

				if (strncmp((char *)&buff[i*LOOPER_EVT_LENGTH], LOOPER_EXIT, LOOPER_EVT_LENGTH) == 0)
				{
					exit = TRUE;
					break;
				}
			}

			if (exit == TRUE) break;

			memset(buff, 0, sizeof(buff));
		}
		else
		{
			if (this->mpImpl)
			{
				this->mpImpl->looperCallback(LOOPER_TIMEOUT);
			}

			if (this->mUser_cb)
			{
				this->mUser_cb(LOOPER_TIMEOUT);
			}
		}
	}

	return res;
}

int NodeLooper::wakeup()
{
	write(this->mFd[1], LOOPER_WAKEUP, LOOPER_EVT_LENGTH);

	return 0;
}

int NodeLooper::exit()
{
	write(this->mFd[1], LOOPER_EXIT, LOOPER_EVT_LENGTH);

	return 0;
}

void NodeLooper::exit(int fd)
{
	write(fd, LOOPER_EXIT, LOOPER_EVT_LENGTH);
}

int NodeLooper::sendMessage(const char* msg, int length)
{
	return (int)write(this->mFd[1], msg, length);
}

