/**
*
*  @author		Seongho Baek
*  @date		2014.08.12
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
