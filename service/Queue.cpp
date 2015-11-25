/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/


#include <string.h>
#include "Log.h"
#include "Lock.h"
#include "Types.h"

NBUS_CMD::NBUS_CMD()
{
	this->mpData = NULL;
	this->mLength = 0;
	this->mCmd = 0;
	this->mType = NONE_MESSAGE;
}

NBUS_CMD::NBUS_CMD(unsigned int length)
{
	if (length)
	{
		this->mpData = new char[length];
		memset(this->mpData, 0, length);
	}
	else
	{
		this->mpData = NULL;
	}

	this->mLength = length;
	this->mCmd = 0;
	this->mType = NONE_MESSAGE;
}

NBUS_CMD::~NBUS_CMD()
{
	if (this->mpData)
	{
		//LOGI("Delete mpData");
		delete [] (char *)mpData;
	}
}

NBUS_CMD::NBUS_CMD(const NBUS_CMD& n)
{
	this->mCmd = n.mCmd;
	this->mLength = n.mLength;

	if (n.mLength)
	{
		this->mpData = new char[n.mLength];
		memset(this->mpData, 0, n.mLength);
		memcpy((char *)this->mpData, (char *)n.mpData, n.mLength);
	}
	else
	{
		this->mpData = NULL;
	}

	this->mType = n.mType;
}

NBUS_CMD& NBUS_CMD::operator=(const NBUS_CMD& n)
{
	if (this != &n)
	{
		if (this->mpData) delete [] (char *)this->mpData;

		this->mpData = NULL;
		this->mCmd = n.mCmd;
		this->mLength = n.mLength;

		if (n.mLength)
		{
			this->mpData = new char[n.mLength];
			memcpy((char *)this->mpData, (char *)n.mpData, n.mLength);
		}

		this->mType = n.mType;
	}

	return *this;
}

NBUS_CMD_QUEUE::NBUS_CMD_QUEUE()
{
	this->mBottom = 0;
	this->mQSize = NBUS_INT_QUEUE_SIZE;
	this->mSize = 0;
	this->mTop = 0;
	this->mpQ = new NBUS_CMD[NBUS_INT_QUEUE_SIZE];
	this->mMutex = Lock::createMutex();
	this->mFull = FALSE;
}

NBUS_CMD_QUEUE::NBUS_CMD_QUEUE(int size)
{
	this->mBottom = 0;
	this->mQSize = 0;
	this->mSize = 0;
	this->mQSize = size;
	this->mTop = 0;
	this->mpQ = NULL;
	this->mpQ = new NBUS_CMD[size];
	this->mMutex = Lock::createMutex();
	this->mFull = FALSE;
}

NBUS_CMD_QUEUE::~NBUS_CMD_QUEUE()
{
	//LOGI("Delete QUEUE");
	for (int i = 0; i < this->mQSize; i++)
	{
		if (this->mpQ[i].mpData) delete [] (char *)this->mpQ[i].mpData;
		this->mpQ[i].mpData = NULL;
	}

	Lock::deleteMutex(this->mMutex);

	delete [] this->mpQ;
}

void NBUS_CMD_QUEUE::clear()
{
	int numQ = 0;

	LOCK(mMutex)
	{
		for (numQ = 0; numQ < mQSize; numQ++)
		{
			if (mpQ[numQ].mpData != NULL)
			{
				delete [] (char *)mpQ[numQ].mpData;

				mpQ[numQ].mpData = NULL;
			}
		}

		mSize = 0;
		mTop = 0;
		mBottom = 0;
		mFull = FALSE;
	}
}

int NBUS_CMD_QUEUE::push(NBUS_CMD& item)
{
	int nr_q = 0;

	LOCK(mMutex)
	{
		if (mFull == TRUE)
		{
			nr_q = mQSize;
			LOGW("Queue Full");
		}
		else
		{
			mpQ[mTop] = item;

			mTop++;
			mTop %= mQSize;
			mSize++;

			nr_q = mSize;

			if (mSize == mQSize)
			{
				mFull = TRUE;
			}
		}
	}

	return nr_q;
}

NBUS_CMD* NBUS_CMD_QUEUE::pop()
{
	NBUS_CMD *item = new NBUS_CMD;

	LOCK(mMutex)
	{
		if (this->mSize == 0) // empty.
		{
			LOGW("Queue Empty");
		}
		else
		{
			*item = mpQ[mBottom];

			if (mpQ[mBottom].mpData)
			{
				delete [] (char *)mpQ[mBottom].mpData;
				mpQ[mBottom].mpData = NULL;
			}

			mBottom++;
			mBottom %= mQSize;
			mSize--;
			mFull = FALSE;
		}
	}

	return item;
}

int NBUS_CMD_QUEUE::getSize()
{
	int size = 0;

	LOCK(mMutex)
	{
		size = mSize;
	}

	return size;
}

