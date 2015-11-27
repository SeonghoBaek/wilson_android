//
// Created by major on 11/27/15.
//

#include "MicomBuffer.h"

MicomBuffer::MicomBuffer(NodeLooper *pLooper) : NBUS_CMD_STACK(STACK_SIZE)
{
    if (pLooper)
    {
        this->mpLooper = pLooper;
    }
}

MicomBuffer::~MicomBuffer()
{}

int MicomBuffer::push(NBUS_CMD &item)
{
    NBUS_CMD_STACK::push(item);

    // Notify looper.
    if (this->mpLooper)
    {
        this->mpLooper->wakeup();
    }

    return 0;
}
