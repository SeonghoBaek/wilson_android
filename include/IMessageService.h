/** 
*
*  @author		Seongho Baek
*
*/

#ifndef __IMESSAGESERVICE_H
#define __IMESSAGESERVICE_H

class IMessageService
{
	public:
		virtual int cast(const char* nodename, const void *manifest, unsigned int length) = 0;
		virtual int cast(const char* nodename, const void *manifest, unsigned int length, int type) = 0;
		virtual int broadcast(const void *manifest, unsigned int length, int msgType) = 0;
		virtual int broadcast(const void *manifest, unsigned int length, int type, int msgType) = 0;
		virtual ~IMessageService() {}
};

#endif
