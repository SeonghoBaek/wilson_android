/** 
*
*  @author		Seongho Baek
*  @date		2014.08.12
*/

#ifndef __CONFIGURE_H
#define __CONFIGURE_H

//#define USE_STD_FILE_IO

//#define MAC_OS_X


#if defined(MAC_OS_X)
#define DOMAIN_PREFIX "/var/tmp/"
#else
#define DOMAIN_PREFIX
#endif

#define LOG_DIR "/tmp/log/"

#define TMP_DIR "/tmp/"

#define LOCK_DIR "/tmp/nbus"

#define LOG_EXT ".log"

#define INFO_EXT ".info"

#define VEC_DIR "/tmp/log/"

#define VEC_FILE_NAME "vector.def"

#define SERVER_CONF_DIR "/tmp/log/"

#define SERVER_CONF_FILE_NAME "server.conf"

#define LOG_FILE_NAME_LENGTH 256

#define LINE_BUFFER_LENGTH 80

#define SERVICE_NAME_LENGTH 64

#define DEFAULT_GLOBAL_SERVICE_NAME DOMAIN_PREFIX "defaultservice.service.global"

#define DEFAULT_LOCAL_SERVICE_NAME DOMAIN_PREFIX "defaultservice.service.local"

#define LOCAL_ROUTER_NAME  DOMAIN_PREFIX "defaultrouter.router.local"

#define DEFAULT_NODE_NAME   DOMAIN_PREFIX "default.node.local"

#define BIGNODE_NODE_NAME DOMAIN_PREFIX "bignode.node"

#define WILSON_NODE_NAME DOMAIN_PREFIX "wilson.node"

#define MGS_NODE_NAME DOMAIN_PREFIX "mgs.node"

#define LOCAL_COORDINATOR_IP_ADDRESS "127.0.0.1"

#define MAX_NUM_OF_NODE_SUPPORT 64

#define LOCAL_COORDINATOR_PORT	32768

#define ACTION_BUFFER_SIZE		4096

#define NODEBUS_MSG_BUFFER_SIZE 4096

#define NODE_MANIFEST_LENGTH	512

#define NODE_NAME_LENGTH		64

#define NODE_IP_LENGTH			16

#define SOCKET_TIME_OUT			1000 // mili

#define SAVE_PARTIAL_LOG		0

#define LOG_SEGMENT_SIZE		10

#endif
