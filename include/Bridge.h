/**
*
*  @author		Seongho Baek
*
*/

#ifndef CBRIDGE_H_
#define CBRIDGE_H_

#define BUFFER_SIZE 512

typedef struct _bridge_arg
{
	void (*cb)(void*, int);
	char *name;
} bridge_arg;

int bridge_create(char* domain_name, void (*action)(void *, int));

int bridge_cross(const char* domain_name, void *stream, int length);

#endif /* CBRIDGE_H_ */
