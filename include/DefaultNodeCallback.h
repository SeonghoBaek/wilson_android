//
// Created by major on 11/2/15.
//

#ifndef NODE_DEFAULTNODE_H
#define NODE_DEFAULTNODE_H


#include "NodeBus.h"

class DefaultNodeCallback : public INode
{
public:
    DefaultNodeCallback() {}
    virtual int nodeCallback(void *data, unsigned int length, int type);
    virtual int nodeGlobalCallback(void *data, unsigned int length);
    virtual ~DefaultNodeCallback() {}
};


#endif //NODE_DEFAULTNODE_H
