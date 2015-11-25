/**
*
*  @author		Seongho Baek
*
*/

#ifndef		_TYPES_H_
#define		_TYPES_H_

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>
#include <sys/ipc.h>
//#include <sys/shm.h>
#include <stdarg.h>
#include <termios.h>
#include <ctype.h>
#include <netinet/in.h>

#define THREAD_INF_WAIT	        (    -1)		/* Wait for ever			*/
#define THREAD_NO_WAIT          (     0)		/* No wait					*/

/**< File Open Flags */
#define	FILE_ROPEN_FLAG		O_RDONLY
#define	FILE_WOPEN_FLAG		O_WRONLY
#define	FILE_RWOPEN_FLAG		O_RDWR
#define FILE_TRUNC_FLAG		O_TRUNC
#define	FILE_COPEN_FLAG		O_CREAT  | O_WRONLY    | O_TRUNC

#define	FILE_OPEN_MODE		00777
#define FILENAME_LENGTH		1024

#define IS_FILE_OPENED(fd) (fd >= 0)
#define IS_FILE_CLOSED(fd) (fd <  0)

#if (!defined(f_offset_t) && !defined(__f_offset_t))
typedef	 long long		__F_OFFSET_T;
#define f_offset_t		__F_OFFSET_T
#define __f_offset_t
#endif

#if (!defined(__cplusplus))
typedef	unsigned long			__BOOLEAN__;
#define bool					__BOOLEAN__
#endif

#define		NBUS_THREAD_PRIORITY_BASE		50

#define		NBUS_THREAD_STACK_SIZE_BASE 	1024 * 24

#define		NBUS_WAIT_FOREVER				THREAD_INF_WAIT

#define		NBUS_WAIT_NOWAIT				THREAD_NO_WAIT

#define		_GOODNESS(exp,r)		if( !(exp) ) return r

#define		_CLEAR(p_data, type)	memset(p_data, 0, sizeof(type))

/*********************************************************
	Structure / Type Define
**********************************************************/

#ifndef TRUE
#define TRUE 	1
#endif

#ifndef FALSE
#define FALSE 	0
#endif

#ifndef	NULL
#define NULL	0
#endif

typedef	enum
{
	NOT_OK = 0,
	OK
	
} NBUS_STATE_T;

typedef	 pthread_t	NBUS_THREAD_ID_T;

#define 	NBUS_SEMA_T			sem_t
#define	    NBUS_MUTEX_T		pthread_mutex_t

#define		NBUS_MAX_SEMA_NAME		(7)
#define		NBUS_MAX_MUTEX_NAME	(7)

//#define USE_NAMED_SEMAPHORE  // BSD, Mac OS

typedef pthread_mutex_t* Mutex_t;

enum
{
	GLOBAL_MESSAGE,
	LOCAL_MESSAGE,
	CLIENT_MESSAGE,
	KILL_MESSAGE,
	SETUP_MESSAGE,
	BC_LOCAL_MESSAGE,
	BC_CLIENT_MESSAGE,
	BUS_MESSAGE,
	DB_MESSAGE,
	CUSTOM_MESSAGE,
	JSON_MESSAGE,
	MICOM_MESSAGE,
	MICOM_REQUEST_MESSAGE,
	NONE_MESSAGE
};

enum   // Client Error ID
{
	CONNECTION_CLOSED,
	PACKET_HEADER_ERROR,
	PACKET_DATA_ERROR,
	INVALID_START_CODE,
	SYSTEM_ERROR,
	COMMAND_ERROR,
	TESTCASE_ERROR,

	NONE_ERROR
};
typedef int (*NodeAction_t)(void *data, unsigned int length, int type);

typedef enum
{
	BUS_READY,
	BUS_NOT_READY,
	BUS_FAILURE

} BusStatus_t;

typedef	 struct	_NBUS_SEMA_STRUCT_T
{
	NBUS_SEMA_T				sema;						/**< Operation Fetch Semaphore	*/
	char					name[NBUS_MAX_SEMA_NAME];	/**< Semaphore name	for debug	*/
	volatile bool		    locked;						/**< Semaphore Lock Status		*/
	volatile unsigned int	pending;    				/**< Pending Lock Request       */
	bool					created;

}	NBUS_SEMA_STRUCT_T;

typedef	struct	_NBUS_MUTEX_STRUCT_T
{
	NBUS_MUTEX_T		mutex;						/**< Linux Thread Mutex 	*/
	char				name[NBUS_MAX_MUTEX_NAME];	/**< mutex name for debug	*/
	volatile bool   	locked;						/**< mutex Lock Status  	*/

}	NBUS_MUTEX_STRUCT_T;

typedef struct ClientAddress
{
    char ipstr[INET6_ADDRSTRLEN+1];
    int  port;
} ClientAddress_t;

#define NBUS_INT_QUEUE_SIZE 512

class NBUS_CMD
{
	public:

		void *mpData;
		unsigned int mLength; // Char Bytes length
		int mCmd;    // reserv.
		int mType;

		NBUS_CMD();
		NBUS_CMD(unsigned int length);
		NBUS_CMD(const NBUS_CMD&);
		NBUS_CMD& operator=(const NBUS_CMD&);

		bool valid() { if (mpData == NULL) return FALSE; return TRUE; }

		virtual ~NBUS_CMD();
};

class NBUS_CMD_QUEUE
{
	private:
		int mBottom;
		int mTop;
		int mQSize; // Total Q Size.
		int mSize;  // Num in Q
		bool mFull;

		NBUS_CMD* mpQ;
		Mutex_t mMutex;

	public:
		NBUS_CMD_QUEUE();
		NBUS_CMD_QUEUE(int size);

		virtual ~NBUS_CMD_QUEUE();
		void 		clear();
		int 		push(NBUS_CMD& item);
		NBUS_CMD*	pop();
		int 		getSize();
		int 		getQSize() { return this->mQSize;}
};
#endif

