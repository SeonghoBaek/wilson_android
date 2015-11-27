#ifndef _CONFIG_H_
#define _CONFIG_H_

//#define MAC_OS_X
#if defined(MAC_OS_X)
#define DOMAIN_PREFIX "/var/tmp/"
#else
#define DOMAIN_PREFIX
#endif

#define SERVICE_NAME_LENGTH 64

#define NODE_NAME_LENGTH	64

#define NBUS_BUFF_SIZE 		2048

#define DEFAULT_GLOBAL_SERVICE_NAME DOMAIN_PREFIX "nodebus.global.service.defaultservice"

#define DEFAULT_LOCAL_SERVICE_NAME DOMAIN_PREFIX "nodebus.local.service.defaultservice"

#define MICOM_NODE_NAME "node.micom"

#define MICOM_SVC_NAME "svc.micom"

#ifndef NULL
#define NULL 0
#endif

#endif
