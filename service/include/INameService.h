/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef __INAMESERVICE_H
#define __INAMESERVICE_H

class INameService
{
	public:

		virtual int join(const char *pNodeName) = 0;
		virtual int join(const char *pNodeName, int type) = 0;
		virtual int drop(const char *pNodeName) = 0;
		virtual int exist(const char *pNodeName) = 0;
		virtual ~INameService() {}
};

#endif
