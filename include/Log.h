/**
*
*  @author		Seongho Baek
*
*/

#ifndef _LOG_H_
#define _LOG_H_

#include <android/log.h>
#include <stdarg.h>

#define LOG_PRINT_ENABLE 1

#define NODEBUS_LOG_ERROR -1

#define NODEBUS_LOG_WARN 0

#define NODEBUS_LOG_INFO 1

#define NODEBUS_LOG_DEBUG 2

#define NODEBUS_LOG_VERBOSE 3

//#define  _LOG_TAG "wilson"

// 0: full log, 1: error only
#define DEBUG_MODE 0

// 0: no log, 1: error only
#define RELEASE_MODE 0

void NBUS_PrintMessage(int type, const char *func, int line, const char *fmt, ...);

#ifndef _LOG_TAG
#define _LOG_TAG "wilson"
#endif

#if 1
#define  LOGE(...)
#define  LOGW(...)
#define  LOGI(...)
#define  LOGD(...)
#define  LOGV(...)

#else
#define  LOGE(...) \
           __android_log_print(ANDROID_LOG_ERROR,_LOG_TAG,__VA_ARGS__)

#define  LOGW(...) \
           __android_log_print(ANDROID_LOG_WARN,_LOG_TAG,__VA_ARGS__)

#define  LOGI(...) \
           __android_log_print(ANDROID_LOG_INFO,_LOG_TAG,__VA_ARGS__)

#define  LOGD(...) \
           __android_log_print(ANDROID_LOG_DEBUG,_LOG_TAG,__VA_ARGS__)

#define  LOGV(...) \
           __android_log_print(ANDROID_LOG_VERBOSE,_LOG_TAG,__VA_ARGS__)
#endif
#endif
