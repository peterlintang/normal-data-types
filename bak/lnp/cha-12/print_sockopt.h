
#ifndef PRINT_SOCKOPT_H__
#define PRINT_SOCKOPT_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

typedef union optval {
	int val;
	struct linger linger;
	struct timeval tv;
	unsigned char str[16];
} Sockopt_val;

typedef enum optvaltype {
	OPTVALINT,
	OPTVALINGER,
	OPTVALTIMEVAL,
	OPTVALUCHAR,
	OPTVALMAX,
}OptvalType;

typedef struct sockopts {
	int level;
	int optname;
	char *name;
	OptvalType valtype;
	Sockopt_val val;
} SockOpts;

#endif // end of PRINT_SOCKOPT_H__

