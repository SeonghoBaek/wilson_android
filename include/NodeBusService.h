/**
*
*  @author		Seongho Baek
*
*/

#ifndef NODEBUSSERVICE_H_
#define NODEBUSSERVICE_H_

#include "INameService.h"
#include "IMessageService.h"

class NodeBusService: public IMessageService, public INameService
{
	public:
		virtual ~NodeBusService() {}
		virtual void processMessage() = 0;
};


#endif /* NODEBUSSERVICE_H_ */
