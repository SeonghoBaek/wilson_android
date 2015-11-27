//
// Created by major on 11/27/15.
//

/**
*
*  @author		Seongho Baek
*/

#define _LOG_TAG "NBUS_CMD_STACK"

#include <string.h>
#include "Log.h"
#include "Lock.h"
#include "Types.h"

NBUS_CMD_STACK::NBUS_CMD_STACK()
{
    this->mBottom = 0;
    this->mQSize = NBUS_INT_QUEUE_SIZE;
    this->mSize = 0;
    this->mTop = 0;
    this->mpQ = new NBUS_CMD[NBUS_INT_QUEUE_SIZE];
    this->mMutex = Lock::createMutex();
    this->mFull = FALSE;
}

NBUS_CMD_STACK::NBUS_CMD_STACK(int size)
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

NBUS_CMD_STACK::~NBUS_CMD_STACK()
{
    for (int i = 0; i < this->mQSize; i++)
    {
        if (this->mpQ[i].mpData) delete [] (char *)this->mpQ[i].mpData;
        this->mpQ[i].mpData = NULL;
    }

    Lock::deleteMutex(this->mMutex);

    delete [] this->mpQ;
}

void NBUS_CMD_STACK::clear()
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

int NBUS_CMD_STACK::push(NBUS_CMD& item)
{
    int nr_q = 0;

    LOCK(mMutex)
    {
        if (mFull == TRUE)
        {
            nr_q = mQSize;
            LOGW("Stack Full");
        }
        else
        {
            mpQ[mTop] = item;

            mTop++;
            //mTop %= mQSize;
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

NBUS_CMD* NBUS_CMD_STACK::popclear()
{
    NBUS_CMD *pCmd = NULL;
    int numQ = 0;

    LOCK(mMutex)
    {
        if (this->mSize == 0)
        {
            LOGW("Stack Empty");
        }
        else
        {
            pCmd = new NBUS_CMD;

            mTop--;
            *pCmd = mpQ[mTop];

            if (mpQ[mTop].mpData)
            {
                delete [] (char *)mpQ[mTop].mpData;
                mpQ[mTop].mpData = NULL;
            }

            mSize--;
            mFull = FALSE;
        }

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

    return pCmd;
}

NBUS_CMD* NBUS_CMD_STACK::pop()
{
    NBUS_CMD *item = NULL;

    LOCK(mMutex)
    {
        if (this->mSize == 0) // empty.
        {
            LOGW("Stack Empty");
        }
        else
        {
            item = new NBUS_CMD;

            mTop--;

            *item = mpQ[mTop];

            if (mpQ[mTop].mpData)
            {
                delete [] (char *)mpQ[mTop].mpData;
                mpQ[mTop].mpData = NULL;
            }

            //mTop %= mQSize;
            mSize--;
            mFull = FALSE;
        }
    }

    return item;
}

int NBUS_CMD_STACK::getSize()
{
    int size = 0;

    LOCK(mMutex)
    {
        size = mSize;
    }

    return size;
}

