/** 
*
*  @author		Seongho Baek
*
*/

#ifndef __NODEENTRY_H
#define __NODEENTRY_H

#include "Types.h"
#include "List.h"
#include "XMLTypes.h"
#include "Configure.h"

// Node Entry Type
enum
{
	NODE_ENTRY_LOCAL = 1,
	NODE_ENTRY_CLIENT,
	NODE_ENTRY_NONE

};

class NodeEntry
{
	LINKED_LIST(NodeEntry);

	private:
		int   mType;
		char  mDescriptor[NODE_NAME_LENGTH];

	public:
		NodeEntry() {LIST_INIT(this); mType = NODE_ENTRY_LOCAL;}

		NodeEntry(const char *desc, int t)
		{
			LIST_INIT(this);

			memset(mDescriptor, 0, NODE_NAME_LENGTH);
			strcpy(mDescriptor, desc);

			//LOGI("NodeEntry Name: %s", desc);
			mType = t;	
		}

		virtual ~NodeEntry()
		{
			LIST_DEL(this);
		}

		char* getDescriptor() {return mDescriptor;}

		int   getType() {return mType;}
};

class LocalNodeEntry : public NodeEntry 
{
	public:
		LocalNodeEntry(const char *desc) : NodeEntry(desc, NODE_ENTRY_LOCAL) {}
};

class ClientNodeEntry : public NodeEntry
{
	public:
		
		ClientNodeEntry(const char *desc) : NodeEntry(desc, NODE_ENTRY_CLIENT) {}
};

#endif
