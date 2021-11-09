/*
 * this implements threads control,
 * same as corenova.
 *
 */

#ifndef __quark_H__
#define __quark_H__

/* status flags */
#define QUARK_STAT_RUNNING 	0x8
#define QUARK_STAT_PAUSED	0x4

/* request flags */
#define QUARK_REQ_STOP	0x8
#define QUARK_REQ_PAUSE	0x4
#define QUARK_REQ_ONCE	0x3
#define QUARK_REQ_LOOP	0

#define QUARK_MAX_NAMELEN	128

#if defined (linux)
# include <bits/local_lim.h>	/* for PTHREAD_STACK_MIN (about 16k) */
#else
# include <limits.h>
#endif

#include <stdint.h>		/* for uintx_t */
#include <pthread.h>		/* for thread */

#ifndef PTHREAD_STACK_MIN
# define PTHREAD_STACK_MIN	16384
#endif

#define QUARK_DEFAULT_STACK_SIZE (PTHREAD_STACK_MIN * 16) /* about 256KB */

typedef int (*quark_func_t)(void *); /* return 1 on success, 0 on error */

typedef struct {
	uint8_t stat :4;	/* quark status flags */
	uint8_t req  :4;	/* quark request flags */

	pthread_t 	life;	/* one quark one "child" thread, 
					which will do the job */
	pthread_cond_t	ready;
	pthread_cond_t	dead;
	pthread_cond_t	pause;
	pthread_cond_t	unpause;
	pthread_mutex_t	lock;
	pthread_attr_t	attr;
	pthread_t	parent; /* one "parent" thread */

	char name[QUARK_MAX_NAMELEN];
	quark_func_t func;
	void *data;
} quark_t;

/* for inner usage */
void _request_pause(quark_t *this);
void *_quark_core(void *arg);

/* quark interfaces */
quark_t *_new_quark(quark_func_t func, void *data, int stack_size);
void _kill_quark(quark_t *quark, int signo);
int _spin_quark(quark_t *quark);
void _setname_quark(quark_t *quark, char *name);
void _once_quark(quark_t *quark);
void _stop_quark(quark_t *quark, uint32_t timeout_ms);
void _pause_quark(quark_t *quark);
void _unpause_quark(quark_t *quark);
void _set_kill_all_quarks(void);
void _destroy_quark(quark_t **quark_ptr);


#endif	/* __quark_H__ */
