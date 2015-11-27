//
// Created by major on 11/27/15.
//

#ifndef MICOMD_MICOMBUFFER_H
#define MICOMD_MICOMBUFFER_H


#include <NodeLooper.h>
#include <Types.h>

#define STACK_SIZE 1024

class MicomBuffer : public NBUS_CMD_STACK
{
private:
    NodeLooper *mpLooper;
public:

    MicomBuffer();

    MicomBuffer(NodeLooper *pLooper);

    virtual ~MicomBuffer();

    virtual int push(NBUS_CMD& item);
};


#endif //MICOMD_MICOMBUFFER_H
