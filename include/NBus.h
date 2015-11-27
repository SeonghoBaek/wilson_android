#ifndef NBUS_H_
#define NBUS_H_

int nbus_join(const char* nodeName, const char* serverName);

int nbus_cast(const char* nodeName, void *data, int length);

int nbus_drop(const char* nodeName, const char* serverName);

#endif /* NBUS_H_ */
